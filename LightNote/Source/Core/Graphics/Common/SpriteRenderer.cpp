//==============================================================================
// SpriteRenderer 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <algorithm>
#include "../../Resource/LNResource.h"
#include "../Manager.h"
#include "GraphicsUtil.h"
#include "SpriteRenderer.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ SpriteRenderer クラス
//==============================================================================

    /*
        ソートする要素は、
            ・プライオリティ
            ・Z座標 or ビュー行列の位置成分からの距離
            ・テクスチャ


        行列計算のテスト…

            頂点毎にワールド行列を覚えておいて (Vector3 の Right Up Front Position の4x3)、
            シェーダ内で計算するのと、CPU 側で4頂点に対して transform() するのとどっちが早いのかテスト。

            方法…
                drawRequest() を 6400 回実行。
                そのループと、シーン全体の描画処理の時間を計測する。

                Intel(R) Core(TM)2 Duo CPU     E8200  @ 2.66GHz (2 CPUs), ~2.7GHz
                NVIDIA Quadro FX 1700

            結果…

                １．CPU 側で座標変換

                    drawRequest : 約 6500ns
                    Render      : 約 3500ns

                ２．シェーダ内で計算

                    drawRequest : 約 11000ns
                    Render      : 約 5000ns

                ３．２の環境で、行列の代入を切ってみる

                    drawRequest : 約 5000ns
                    Render      : 約 5000ns

                おまけ.
                    シェーダ内の mul 回数を 1 回だけにしてみた。(↑のは行列作成→mul→ViewProj mul の2回)

                    drawRequest : 約 5000ns
                    Render      : 約 5000ns

                    あんまり変わらない。

            考察…

                行列の計算よりも、値コピー×4 (4頂点分) のほうがはるかに重い。
                シェーダの方は渡す頂点データのサイズが一番影響大きかったみたい。
                
    */

    BatchSpriteData* SpriteRenderer::sSpriteRequests = NULL;

	//----------------------------------------------------------------------
	// ● コンストラクタ
	//----------------------------------------------------------------------
    SpriteRenderer::SpriteRenderer( Manager* manager_ )
        : mManager                  ( manager_ )
        , mGraphicsDevice           ( NULL )
        , mVertexBuffer             ( NULL )
        , mIndexBuffer              ( NULL )
        , mMaxSprites               ( 0 )
        , mSpriteRequests           ( NULL )
        , mNextIndex                ( 0 )
        , mLastSpriteNum            ( 0 )
        , mSpriteIndexArray         ( NULL )
        , mSpriteIndexArraySource   ( NULL )
        , mComFunc                  ( NULL )
        , mSpriteSortMode           ( LN_SPRITESORT_NONE )
        , mEnableViewPosDepth       ( false )
        , mIs3DSystem               ( false )
		, mCurrentRenderStateIndex	( NULL )
    {
		memset( &mShaderParam, 0, sizeof(mShaderParam) );
    }

	//----------------------------------------------------------------------
	// ● デストラクタ
	//----------------------------------------------------------------------
    SpriteRenderer::~SpriteRenderer()
    {
        LN_SAFE_RELEASE( mShaderParam.Shader );
        LN_SAFE_RELEASE( mGraphicsDevice );
        LN_SAFE_RELEASE( mVertexBuffer );
        LN_SAFE_RELEASE( mIndexBuffer );
        SAFE_DELETE_ARRAY( mSpriteRequests );
        SAFE_DELETE_ARRAY( mSpriteIndexArray );
        SAFE_DELETE_ARRAY( mSpriteIndexArraySource );
    }

	//----------------------------------------------------------------------
	// ● 初期化
	//----------------------------------------------------------------------
    LNRESULT SpriteRenderer::initialize( IGraphicsDevice* device_, int max_sprites_, bool is_3d_ )
    {
        LNRESULT lr;

        mGraphicsDevice = device_;
        mMaxSprites     = max_sprites_;
        mIs3DSystem     = is_3d_;
        LN_SAFE_ADDREF( mGraphicsDevice );

		mMaxSprites *= 2;
        //-----------------------------------------------------
        // 頂点バッファとインデックスバッファ

        mVertexBuffer = mGraphicsDevice->createVertexBuffer(
            BatchSpriteVertex::elements(), mMaxSprites * 4, NULL, true );

        mIndexBuffer = mGraphicsDevice->createIndexBuffer(
            mMaxSprites * 6, NULL, true, true );

        // インデックスバッファの値は最初から固定でOKなのでここで書き込む
        lnU16* ib = (lnU16*)mIndexBuffer->lock();
        int idx = 0;
        int i2 = 0;
        for ( int i = 0; i < mMaxSprites; ++i )
        {
            i2  = i * 6;
            idx = i * 4;
            ib[ i2 + 0 ] = idx;
            ib[ i2 + 1 ] = idx + 1;
            ib[ i2 + 2 ] = idx + 2;
            ib[ i2 + 3 ] = idx + 2;
            ib[ i2 + 4 ] = idx + 1;
            ib[ i2 + 5 ] = idx + 3;
        }
        mIndexBuffer->unlock();


        //-----------------------------------------------------
        // シェーダ
        
        // 埋め込みリソースから読み込む
		mShaderParam.Shader = Util::createShader(mManager, Resource::ID_Shader_SpriteRenderer, NULL);
		
        mShaderParam.varTexture         = mShaderParam.Shader->getVariableByName( "gMaterialTexture" );
        mShaderParam.varViewProjMatrix  = mShaderParam.Shader->getVariableByName( "gViewProjMatrix" );
		mShaderParam.varViewPixelSize   = mShaderParam.Shader->getVariableByName( "gViewportSize" );
        mShaderParam.varTexture         = mShaderParam.Shader->getVariableByName( "gMaterialTexture" );
        mShaderParam.techMainDraw       = mShaderParam.Shader->getTechniqueByName( "MainDraw" );


        //-----------------------------------------------------
        // スプライトとソート用インデックス配列

        mSpriteRequests = LN_NEW BatchSpriteData[ mMaxSprites ];
        memset( mSpriteRequests, 0, sizeof( BatchSpriteData ) * mMaxSprites );

        mSpriteIndexArray = LN_NEW lnU16[ mMaxSprites ];
        for ( int i = 0; i < mMaxSprites; ++i )
        {
            mSpriteIndexArray[ i ] = i;
        }
        mSpriteIndexArraySource = LN_NEW lnU16[ mMaxSprites ];
        memcpy( mSpriteIndexArraySource, mSpriteIndexArray, sizeof( *mSpriteIndexArraySource ) * mMaxSprites );

		// ステートにはデフォルトをひとつ詰めておく
		mRenderStateArray.push_back( LRenderState() );
		mCurrentRenderStateIndex = 0;

        mNextIndex = 0;
        mLastSpriteNum = 0;

		if (is_3d_) {
			// 視点からの距離が大きいものを先に描画する
			setSortMode(LN_SPRITESORT_TEXTURE | LN_SPRITESORT_DEPTH_BACK_TO_FRONT, true);
		}
		else {
			// スプライトの Z 値が小さいものを先に描画する
			setSortMode(LN_SPRITESORT_TEXTURE | LN_SPRITESORT_DEPTH_BACK_TO_FRONT, false);
		}

		return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	// ● 座標変換行列の設定
	//----------------------------------------------------------------------
    void SpriteRenderer::setTransform( const LMatrix& matrix_ )
    {
        mTransformMatrix = matrix_;
    }

	//----------------------------------------------------------------------
	// ● ビュー、プロジェクション行列の設定
	//----------------------------------------------------------------------
    void SpriteRenderer::setViewProjMatrix( const LMatrix& view_, const LMatrix& proj_ )
    {
		mViewDirection.Set(view_.M[0][2], view_.M[1][2], view_.M[2][2]);

		mViewInverseMatrix = LMatrix::Inverse(view_);

        mViewPosition = mViewInverseMatrix.GetPosition();
        //mViewInverseMatrix.transpose();
        //LMath::MatrixTranspose( &mViewProjMatrix, ( view_ * proj_ ) );

        mViewProjMatrix = ( view_ * proj_ );

		mViewPixelSize = LVector2::Zero;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SpriteRenderer::setViewProjMatrix( const LMatrix& view, const LMatrix& proj, const LVector2& viewPixelSize )
	{
		setViewProjMatrix( view, proj );
		mViewPixelSize = viewPixelSize;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SpriteRenderer::setViewSize(const LVector2& viewSize)
	{
		LMatrix view;
		LMatrix proj;
		LMatrixUtils::perspective2DLH(
			&proj,
			viewSize.X,
			viewSize.Y,
			0.0f,
			10000.0f);
		setViewProjMatrix(view, proj, viewSize);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SpriteRenderer::setRenderState( const LRenderState& state )
	{
		// 同じものがあればカレントに
		size_t count = mRenderStateArray.size();
		for ( size_t i = 0; i < count; ++i ) {
			if ( state == mRenderStateArray[i] ) {
				mCurrentRenderStateIndex = i;
				return;
			}
		}

		// 見つからなかったら登録
		mRenderStateArray.push_back( state );
		mCurrentRenderStateIndex = count;
	}

	//----------------------------------------------------------------------
	// ● ソート方法の設定
	//----------------------------------------------------------------------
    void SpriteRenderer::setSortMode( lnU32 flags_, bool enable_view_pos_depth_ )
    {
        mSpriteSortMode = flags_;
        mEnableViewPosDepth = enable_view_pos_depth_;
        mComFunc = NULL;

        if ( mSpriteSortMode & LN_SPRITESORT_TEXTURE )
        {
            if ( mSpriteSortMode & LN_SPRITESORT_DEPTH_BACK_TO_FRONT )
            {
                mComFunc = _cmpTexDepthBackToFront;
            }
            else if ( mSpriteSortMode & LN_SPRITESORT_DEPTH_FRONT_TO_BACK )
            {
                mComFunc = _cmpTexDepthFrontToBack;
            }
        }
        else
        {
            if ( mSpriteSortMode & LN_SPRITESORT_DEPTH_BACK_TO_FRONT )
            {
                mComFunc = _cmpDepthBackToFront;
            }
            else if ( mSpriteSortMode & LN_SPRITESORT_DEPTH_FRONT_TO_BACK )
            {
                mComFunc = _cmpDepthFrontToBack;
            }
        }
    }

	//----------------------------------------------------------------------
	// ● 描画リクエスト
	//----------------------------------------------------------------------
    LNRESULT SpriteRenderer::drawRequest2D(
        const LVector3& position_,
        const LVector3& center_,
        Texture* texture_,
        const LRectF& src_rect_,
        lnU32 color_ )
    {
        lnU32 color_table[ 4 ] = { color_, color_, color_, color_ };
        return drawRequest3D( 
			position_, 
			center_, 
			LVector2( src_rect_.w, src_rect_.h ), 
			texture_, 
			src_rect_, 
			color_table, 
			LN_AADIR_RZ );
    }

	//----------------------------------------------------------------------
	// ● 描画リクエスト
	//----------------------------------------------------------------------
    LNRESULT SpriteRenderer::drawRequest3D(
        const LVector3& position_,
        const LVector3& center_,
        const LVector2& size_,
        Texture* texture_,
        const LRectF& src_rect_,
        const lnU32* color_table_,
        LNAxisAlignedDir front_ )
    {
		LN_ERR2_ASSERT( mNextIndex < mMaxSprites );

        BatchSpriteData& sprite = mSpriteRequests[ mNextIndex ];

        // 3D の場合の頂点座標
        if ( mIs3DSystem )
        {
            /*
            LVector3 origin(
                position_.x - center_.x,
                position_.y - center_.y,
                position_.z - center_.z );
                */

			LVector3 origin(-center_);

			LVector2 harf_size(size_ * 0.5f);

            lnFloat l, t, r, b;
            r =  harf_size.X;
            b = -harf_size.Y;
            l = -r;
            t = -b;

#define LN_WRITE_V3( x_, y_, z_ ) origin.X + x_, origin.Y + y_, origin.Z + z_

            switch ( front_ )
            {
                case LN_AADIR_X:
                    sprite.Vertices[ 0 ].Position.Set( LN_WRITE_V3( 0, t, l ) );     // 左上
                    sprite.Vertices[ 1 ].Position.Set( LN_WRITE_V3( 0, t, r ) );     // 右上
                    sprite.Vertices[ 2 ].Position.Set( LN_WRITE_V3( 0, b, l ) );     // 左下
                    sprite.Vertices[ 3 ].Position.Set( LN_WRITE_V3( 0, b, r ) );     // 右下
                    break;
                case LN_AADIR_Y:
                    sprite.Vertices[ 0 ].Position.Set( LN_WRITE_V3( l, 0, t ) );
                    sprite.Vertices[ 1 ].Position.Set( LN_WRITE_V3( r, 0, t ) );
                    sprite.Vertices[ 2 ].Position.Set( LN_WRITE_V3( l, 0, b ) );
                    sprite.Vertices[ 3 ].Position.Set( LN_WRITE_V3( r, 0, b ) );
                    break;
                case LN_AADIR_Z:
                    /*
                    sprite.Vertices[ 0 ].Position.set( LN_WRITE_V3( r, t, 0 ) );
                    sprite.Vertices[ 1 ].Position.set( LN_WRITE_V3( l, t, 0 ) );
                    sprite.Vertices[ 2 ].Position.set( LN_WRITE_V3( r, b, 0 ) );
                    sprite.Vertices[ 3 ].Position.set( LN_WRITE_V3( l, b, 0 ) );
                    */
                    sprite.Vertices[ 0 ].Position.Set( LN_WRITE_V3( r, t, 0 ) );
                    sprite.Vertices[ 1 ].Position.Set( LN_WRITE_V3( l, t, 0 ) );
                    sprite.Vertices[ 2 ].Position.Set( LN_WRITE_V3( r, b, 0 ) );
                    sprite.Vertices[ 3 ].Position.Set( LN_WRITE_V3( l, b, 0 ) );
                    break;
                case LN_AADIR_RX:
                    sprite.Vertices[ 0 ].Position.Set( LN_WRITE_V3( 0, t, r ) );
                    sprite.Vertices[ 1 ].Position.Set( LN_WRITE_V3( 0, t, l ) );
                    sprite.Vertices[ 2 ].Position.Set( LN_WRITE_V3( 0, b, r ) );
                    sprite.Vertices[ 3 ].Position.Set( LN_WRITE_V3( 0, b, l ) );
                    break;
	            case LN_AADIR_RY:
                    sprite.Vertices[ 0 ].Position.Set( LN_WRITE_V3( r, 0, t ) );
                    sprite.Vertices[ 1 ].Position.Set( LN_WRITE_V3( l, 0, t ) );
                    sprite.Vertices[ 2 ].Position.Set( LN_WRITE_V3( r, 0, b ) );
                    sprite.Vertices[ 3 ].Position.Set( LN_WRITE_V3( l, 0, b ) );
                    break;
                case LN_AADIR_RZ:
                    sprite.Vertices[ 0 ].Position.Set( LN_WRITE_V3( l, t, 0 ) );
                    sprite.Vertices[ 1 ].Position.Set( LN_WRITE_V3( r, t, 0 ) );
                    sprite.Vertices[ 2 ].Position.Set( LN_WRITE_V3( l, b, 0 ) );
                    sprite.Vertices[ 3 ].Position.Set( LN_WRITE_V3( r, b, 0 ) );
                    /* 右手用
                    sprite.Vertices[ 0 ].Position.set( LN_WRITE_V3( l, t, 0 ) );
                    sprite.Vertices[ 1 ].Position.set( LN_WRITE_V3( l, b, 0 ) );
                    sprite.Vertices[ 2 ].Position.set( LN_WRITE_V3( r, t, 0 ) );
                    sprite.Vertices[ 3 ].Position.set( LN_WRITE_V3( r, b, 0 ) );
                    */
                    break;
            }
#undef LN_WRITE_V3
        }
        // 2D の場合の頂点座標
        else
        {
            /*
            LVector3 origin(
                position_.x - center_.x,
                position_.y - center_.y,
                position_.z - center_.z );
                */
			LVector3 origin(-center_);

            sprite.Vertices[ 0 ].Position.Set( origin.X,           origin.Y,           origin.Z );
            sprite.Vertices[ 1 ].Position.Set( origin.X + size_.X, origin.Y,           origin.Z );
            sprite.Vertices[ 2 ].Position.Set( origin.X,           origin.Y + size_.Y, origin.Z );
            sprite.Vertices[ 3 ].Position.Set( origin.X + size_.X, origin.Y + size_.Y, origin.Z );
        }

        //mTransformMatrix.rotationZ( 0.0001f );

		LMatrix mat = mTransformMatrix.GetRotationMatrix();


        // ビルボード (Scene から使う場合は SceneNode が面倒見てるので、Scene 以外で必要になるまで保留…)
        if ( 0 )
        {
            //mat.setMul3x3( mViewInverseMatrix );
        }
        // ビルボード・Y 軸のみに適用
        else if ( 0 )
        {
            if ( mViewDirection.X > 0.0f )
		    {
                mat.RotationY( -atanf( mViewDirection.Z / mViewDirection.X ) + LMath::PI / 2 );
		    }   
		    else if ( mViewDirection.X == 0.0f )
		    {
			    //D3DXMatrixIdentity(&matWorld); // 0除算を防ぐため
		    }
		    else
		    {
                mat.RotationY( -atanf( mViewDirection.Z / mViewDirection.X ) - LMath::PI / 2 );
		    }
            
        }
        // ビルボードではない
        else
        {
        }

        mat.Translation( position_ );
        mat.Translation( mTransformMatrix.GetPosition() );

        // 座標変換
		sprite.Vertices[0].Position.TransformCoord(mat);
		sprite.Vertices[1].Position.TransformCoord(mat);
		sprite.Vertices[2].Position.TransformCoord(mat);
		sprite.Vertices[3].Position.TransformCoord(mat);

        //sprite.Vertices[ 0 ].Position.transform( mViewProjMatrix );
        //sprite.Vertices[ 0 ].Position.cdump();
        /*
        sprite.Vertices[ 0 ].Position.transform( mTransformMatrix );
        sprite.Vertices[ 1 ].Position.transform( mTransformMatrix );
        sprite.Vertices[ 2 ].Position.transform( mTransformMatrix );
        sprite.Vertices[ 3 ].Position.transform( mTransformMatrix );
        */

        // 色
        if ( color_table_ )
        {
            sprite.Vertices[ 0 ].Color = color_table_[ 0 ];
            sprite.Vertices[ 1 ].Color = color_table_[ 1 ];
            sprite.Vertices[ 2 ].Color = color_table_[ 2 ];
            sprite.Vertices[ 3 ].Color = color_table_[ 3 ];
        }
        else
        {
            sprite.Vertices[ 0 ].Color =
            sprite.Vertices[ 1 ].Color =
            sprite.Vertices[ 2 ].Color =
            sprite.Vertices[ 3 ].Color = 0xffffffff;
        }
        /*
        sprite.Vertices[ 0 ].Color.setFrom32Bit( color_table_[ 0 ] );
        sprite.Vertices[ 1 ].Color.setFrom32Bit( color_table_[ 1 ] );
        sprite.Vertices[ 2 ].Color.setFrom32Bit( color_table_[ 2 ] );
        sprite.Vertices[ 3 ].Color.setFrom32Bit( color_table_[ 3 ] );
        */

        if ( texture_ )
        {
            // テクスチャ座標
            LVector2 texsize_inv = texture_->getRealSize( true );
            Geometry::RectF sr( src_rect_ );
            //sr.x = src_rect_.x;
            //sr.y = src_rect_.y;
            //sr.w = src_rect_.w;
            //sr.h = src_rect_.h;
            lnFloat l = sr.x * texsize_inv.X;
            lnFloat t = sr.y * texsize_inv.Y;
            lnFloat r = (sr.x + sr.w) * texsize_inv.X;
            lnFloat b = (sr.y + sr.h) * texsize_inv.Y;
            sprite.Vertices[ 0 ].TexUV.X = l;
            sprite.Vertices[ 0 ].TexUV.Y = t;
            sprite.Vertices[ 1 ].TexUV.X = r;
            sprite.Vertices[ 1 ].TexUV.Y = t;
            sprite.Vertices[ 2 ].TexUV.X = l;
            sprite.Vertices[ 2 ].TexUV.Y = b;
            sprite.Vertices[ 3 ].TexUV.X = r;
            sprite.Vertices[ 3 ].TexUV.Y = b;
          
            // テクスチャ
            sprite.Texture = texture_;
        }
        else
        {
            sprite.Vertices[ 0 ].TexUV.X = 0;
            sprite.Vertices[ 0 ].TexUV.Y = 0;
            sprite.Vertices[ 1 ].TexUV.X = 1;
            sprite.Vertices[ 1 ].TexUV.Y = 0;
            sprite.Vertices[ 2 ].TexUV.X = 0;
            sprite.Vertices[ 2 ].TexUV.Y = 1;
            sprite.Vertices[ 3 ].TexUV.X = 1;
            sprite.Vertices[ 3 ].TexUV.Y = 1;
            sprite.Texture = mManager->getDummyTexture();
        }

        // カメラからの距離をソート用Z値にする場合
        if ( mEnableViewPosDepth )
        {
            sprite.Depth = ( mViewPosition - position_ ).GetLengthSquared();
        }
        else
        {
            sprite.Depth = position_.Z;
        }

        sprite.Visible = true;
        sprite.Assigned = false;
        sprite.Priority = 0;
		sprite.RenderStateIndex = mCurrentRenderStateIndex;

        ++mNextIndex;
        return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	// ● バッチ処理されているスプライトの描画
	//----------------------------------------------------------------------
    LNRESULT SpriteRenderer::flash()
    {
        int sprite_num = mNextIndex;            // 描画するスプライトの数
		if (sprite_num == 0) return ResultCode_OK;    // 0 個ならなにもしない

        memcpy( mSpriteIndexArray, mSpriteIndexArraySource, sizeof( *mSpriteIndexArray ) * mLastSpriteNum );

        // インデックスの並び替え
        if ( mComFunc )
        {
            sSpriteRequests = mSpriteRequests;
            std::stable_sort( mSpriteIndexArray, mSpriteIndexArray + sprite_num, mComFunc );
        }


        //printf( "flash()\n" );
        //for ( int i = 0; i < sprite_num; ++i )
        //{
        //    printf( "%4d: no:%4d depth:%4f tex:%4p\n", i, mSpriteIndexArray[ i ], mSpriteRequests[ mSpriteIndexArray[ i ] ].Depth, mSpriteRequests[ mSpriteIndexArray[ i ] ].Texture );
        //}


        int si = 0;     // Sprite Index (mSpriteIndexArray)
        int vi = 0;     // Vertex Index
        int ri = 0;     // Request Index (mSpriteRequests)
        int start_idx = 0;
        int prim_num = 0;
        Texture* current_tex = mSpriteRequests[ mSpriteIndexArray[ 0 ] ].Texture;
		int currnetRenderStateIndex = mSpriteRequests[ mSpriteIndexArray[ 0 ] ].RenderStateIndex;

        //-----------------------------------------------------
        // 属性リストを作る

        mAttributeList.clear();
        while ( true )
        {
            while ( true )
            {
                ++si;
                ++prim_num;

#if 1
				// 次のスプライトのテクスチャが、処理中のテクスチャと異なる場合は次の属性作成に移る
                if ( si >= sprite_num || 
					mSpriteRequests[ mSpriteIndexArray[ si ] ].Texture != current_tex ||
					mSpriteRequests[ mSpriteIndexArray[ si ] ].RenderStateIndex != currnetRenderStateIndex
					)
                {
                    break;
                }
#else
                // 次のスプライトのテクスチャが、処理中のテクスチャと異なる場合は次の属性作成に移る
                if ( si >= sprite_num || mSpriteRequests[ mSpriteIndexArray[ si ] ].Texture != current_tex )
                {
                    break;
                }
#endif
            }

            Attribute attr;
            attr.StartIndex = start_idx;
            attr.PrimitiveNum = prim_num * 2;
            attr.Texture = current_tex;
			attr.RenderStateIndex = currnetRenderStateIndex;
            mAttributeList.push_back( attr );

            if ( si >= sprite_num )
            {
                break;
            }

            start_idx = si * 6;
            prim_num = 0;
            current_tex = mSpriteRequests[ mSpriteIndexArray[ si ] ].Texture;
            currnetRenderStateIndex = mSpriteRequests[ mSpriteIndexArray[ si ] ].RenderStateIndex;
        }

        //-----------------------------------------------------
        // 頂点データをコピー
        BatchSpriteVertex* vb = static_cast< BatchSpriteVertex* >( mVertexBuffer->lock() );
        si = 0;
        vi = 0;
        for ( ; si < sprite_num; ++si )
        {
            ri = mSpriteIndexArray[ si ];
            memcpy( &vb[ vi ], mSpriteRequests[ ri ].Vertices, sizeof( mSpriteRequests[ 0 ].Vertices ) );


            
                //printf( "%x\n", vb[ vi ].Color );
                //printf( "%x\n", vb[ vi + 1 ].Color );
                //printf( "%x\n", vb[ vi + 2 ].Color );
                //printf( "%x\n", vb[ vi + 3 ].Color );
            vi += 4;
        }
        mVertexBuffer->unlock();

        //-----------------------------------------------------
        // 描画

        IRenderer* r = mGraphicsDevice->getRenderer();
        r->setVertexBuffer( mVertexBuffer, true );
        r->setIndexBuffer( mIndexBuffer );
        mShaderParam.varViewProjMatrix->setMatrix( mViewProjMatrix );
		mShaderParam.varViewPixelSize->setVector( LVector4( mViewPixelSize.x, mViewPixelSize.y, 0, 0 ) );

        IShaderPass* pass = mShaderParam.techMainDraw->getPassByIndex( 0 );
       
        std::list< Attribute >::iterator itr = mAttributeList.begin();
        std::list< Attribute >::iterator end = mAttributeList.end();
        for ( ; itr != end; ++itr )
        {
			r->setRenderState( mRenderStateArray[itr->RenderStateIndex] );
            mShaderParam.varTexture->setTexture( itr->Texture );
            pass->begin();
			r->drawPrimitiveIndexed(PrimitiveType_TriangleList, itr->StartIndex, itr->PrimitiveNum);
            pass->end();
        }
		
		//-----------------------------------------------------
		// クリーンアップ
		clear();
		return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SpriteRenderer::clear()
	{
		mRenderStateArray.clear();
		mRenderStateArray.push_back(LRenderState());
		mCurrentRenderStateIndex = 0;

		mLastSpriteNum = mNextIndex;
		mNextIndex = 0;
	}

	//----------------------------------------------------------------------
	// ● Z 値の大きい方から小さい方へソートする比較関数
	//----------------------------------------------------------------------
    bool SpriteRenderer::_cmpDepthBackToFront( const lnU16& l_, const lnU16& r_ )
    {
        if ( sSpriteRequests[ l_ ].Priority == sSpriteRequests[ r_ ].Priority )
        {
			if ( sSpriteRequests[ l_ ].RenderStateIndex == sSpriteRequests[ r_ ].RenderStateIndex )
			{

				if ( sSpriteRequests[ l_ ].Depth > sSpriteRequests[ r_ ].Depth )
				{
					return true;
				}
				if ( sSpriteRequests[ l_ ].Depth == sSpriteRequests[ r_ ].Depth )
				{
					return sSpriteRequests[ l_ ].Texture < sSpriteRequests[ r_ ].Texture;
				}
				return false;
			}
			return sSpriteRequests[ l_ ].RenderStateIndex < sSpriteRequests[ r_ ].RenderStateIndex;
        }
        return sSpriteRequests[ l_ ].Priority < sSpriteRequests[ r_ ].Priority;
    }

	//----------------------------------------------------------------------
	// ● Z 値の小さい方から大きい方へソートする比較関数
	//----------------------------------------------------------------------
    bool SpriteRenderer::_cmpDepthFrontToBack( const lnU16& l_, const lnU16& r_ )
    {
        if ( sSpriteRequests[ l_ ].Priority == sSpriteRequests[ r_ ].Priority )
        {
			if ( sSpriteRequests[ l_ ].RenderStateIndex == sSpriteRequests[ r_ ].RenderStateIndex )
			{
				if ( sSpriteRequests[ l_ ].Depth < sSpriteRequests[ r_ ].Depth )
				{
					return true;
				}
				if ( sSpriteRequests[ l_ ].Depth == sSpriteRequests[ r_ ].Depth )
				{
					return sSpriteRequests[ l_ ].Texture < sSpriteRequests[ r_ ].Texture;
				}
				return false;
			}
			return sSpriteRequests[ l_ ].RenderStateIndex < sSpriteRequests[ r_ ].RenderStateIndex;
        }
        return sSpriteRequests[ l_ ].Priority < sSpriteRequests[ r_ ].Priority;
    }

	//----------------------------------------------------------------------
	// ● Z 値の大きい方から小さい方へソートする比較関数 (テクスチャ優先)
	//----------------------------------------------------------------------
    bool SpriteRenderer::_cmpTexDepthBackToFront( const lnU16& l_, const lnU16& r_ )
    {
        if ( sSpriteRequests[ l_ ].Priority == sSpriteRequests[ r_ ].Priority )
        {
			if ( sSpriteRequests[ l_ ].RenderStateIndex == sSpriteRequests[ r_ ].RenderStateIndex )
			{
				if ( sSpriteRequests[ l_ ].Texture < sSpriteRequests[ r_ ].Texture )
				{
					return true;
				}
				if ( sSpriteRequests[ l_ ].Texture == sSpriteRequests[ r_ ].Texture )
				{
					// Depth 降順
					return sSpriteRequests[ l_ ].Depth > sSpriteRequests[ r_ ].Depth;
				}
				return false;
			}
			return sSpriteRequests[ l_ ].RenderStateIndex < sSpriteRequests[ r_ ].RenderStateIndex;
        }
        return sSpriteRequests[ l_ ].Priority < sSpriteRequests[ r_ ].Priority;
    }

	//----------------------------------------------------------------------
	// ● Z 値の小さい方から大きい方へソートする比較関数 (テクスチャ優先)
	//----------------------------------------------------------------------
    bool SpriteRenderer::_cmpTexDepthFrontToBack( const lnU16& l_, const lnU16& r_ )
    {
        if ( sSpriteRequests[ l_ ].Priority == sSpriteRequests[ r_ ].Priority )
        {
			if ( sSpriteRequests[ l_ ].RenderStateIndex == sSpriteRequests[ r_ ].RenderStateIndex )
			{
				if ( sSpriteRequests[ l_ ].Texture < sSpriteRequests[ r_ ].Texture )
				{
					return true;
				}
				if ( sSpriteRequests[ l_ ].Texture == sSpriteRequests[ r_ ].Texture )
				{
					return sSpriteRequests[ l_ ].Depth < sSpriteRequests[ r_ ].Depth;
				}
				return false;
			}
			return sSpriteRequests[ l_ ].RenderStateIndex < sSpriteRequests[ r_ ].RenderStateIndex;
        }
        return sSpriteRequests[ l_ ].Priority < sSpriteRequests[ r_ ].Priority;
    }

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================