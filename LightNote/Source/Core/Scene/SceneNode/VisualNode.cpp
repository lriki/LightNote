//==============================================================================
// SceneNode 
//==============================================================================

#include "stdafx.h"
#include "../SceneGraph.h"
#include "../SceneShader/SceneShader.h"
#include "../SceneShader/SceneShaderTechnique.h"
#include "../SceneShader/OffscreenScene.h"
#include "Camera.h"
#include "VisualNode.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// ■ VisualNode
//==============================================================================

	LN_TYPE_INFO_ACCESS_IMPL(VisualNode);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    VisualNode::VisualNode( SceneGraph* scene )
		: SceneNode	( scene )
		, mRenderer	( NULL )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    VisualNode::~VisualNode()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void VisualNode::create( int subsetCount, LNDrawCoordSpace cs, SceneNodeContext* context )
    {
		if ( context )
        {
			//SceneNode::initialize( subsetCount, cs, context );
            //mContext = context_;
            //this->mSubsetCount = subsetCount;//context_->getSubsetCount();
        }
        else
        {
            //mSubsetCount = subsetCount;
            VisualNodeContext* ctx = LN_NEW VisualNodeContext();
            ctx->initialize( this );
			context = ctx;
        }
		SceneNode::initialize( subsetCount, cs, context );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void VisualNode::setShader( SceneShader* shader )
    {
        if ( shader )
        {
            // 共有不可で最初にセットされたものではない場合、この中から例外が投げられる
            //shader_->onSettingShader( this );
        }

		LN_REFOBJ_SET( mShader, shader );
       
        // シェーダから必要な適用ライト数を取得して、配列を準備しておく
        if ( mShader )
        {
            mApplyLights.resize( mShader->getRequiredLightNum() );

			// TODO:仮
			if ( mShader->hasScriptExternal() )//isPostEffect() )
			{
				this->mSceneGraph->getDefaultNode( LN_DEFAULT_SCENENODE_3D )->addPostEffectShader( mShader, 0 );
			}
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void VisualNode::updateContext()
	{
		SceneNode::updateContext();
	}

	
//==============================================================================
// ■ VisualNodeContext
//==============================================================================
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	VisualNodeContext::VisualNodeContext()
		: mShader	( NULL )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void VisualNodeContext::releaseAllResource()
    {
		SceneNodeContext::releaseAllResource();
        LN_SAFE_RELEASE( mShader );
    }
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void VisualNodeContext::updateContext( SceneNodeContext* parentContext )
	{
		// ※ 親の情報を継承したければ、先に親を更新する必要がある。
		//	  が、今は単純な for ループで更新しているため
		//	  生成順によってはまだ親が更新されていない・・・　ということになる。

		// 基底
		SceneNodeContext::updateContext(parentContext);

		VisualNode* ownerNode = static_cast<VisualNode*>( mSceneNode );

		//-----------------------------------------------------
		// レンダーステート系
        mRenderState = ownerNode->mNodeRenderParam.RenderState;

		// サブセット描画パラメータ配列のリサイズ
		if ( mSubsetRenderParamArray.size() != ownerNode->mNodeRenderParam.getSubsetCount() ) {
			mSubsetRenderParamArray.resize( ownerNode->mNodeRenderParam.getSubsetCount() );
		}

		// LMaterial 内ではリソースの参照カウントを変化させているので、memcpy は NG
		size_t subsetCount = mSubsetRenderParamArray.size();
		for ( lnU32 i = 0; i < subsetCount; ++i ) {
			mSubsetRenderParamArray[i] = ownerNode->mNodeRenderParam.Subset[i];
		}

#if 1	// 親からの引継ぎは姿勢だけでいいと思う
#else
        // 親がある場合は 0 番サブセットのコミット済み情報を受け継ぐ
        if ( parentContext && parentContext->getNodeType() == SceneNodeType_VisualNode )
        {
            const LNSubsetRenderParam& pp = static_cast<VisualNodeContext*>( parentContext )->mSubsetRenderParamArray[ 0 ];

            for ( lnU32 i = 0; i < subsetCount; ++i )
            {
                LNSubsetRenderParam& pa = mSubsetRenderParamArray[ i ];

                ((LVector4&)pa.ColorScale) = ((LVector4&)pa.ColorScale) * ((LVector4&)pp.ColorScale);
                ((LVector4&)pa.BlendColor) = ((LVector4&)pa.BlendColor) + ((LVector4&)pp.BlendColor);
                ((LVector4&)pa.Tone)       = ((LVector4&)pa.Tone)       + ((LVector4&)pp.Tone);
            }
        }
#endif

		//-----------------------------------------------------
		// シェーダ
		LN_REFOBJ_SET( mShader, mSceneNode->mShader );

        // 必要なライトの数で配列を resize
        if ( mShader )
        {
			if ( mAffectLightContextArray.size() < mShader->getRequiredLightNum() )
			{
				mAffectLightContextArray.resize( mShader->getRequiredLightNum() );
			}
        }


	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void VisualNodeContext::render( ContextDrawParam& param )
	{
		if ( this->mEnableDrawing )
        {
			// シェーダ選択
			SceneShader* shader = getShader();

			// 優先パラメータチェック
			bool drawing = true;
			if ( param.OffscreenScene )
			{
				LNPriorityParameter* pp = param.OffscreenScene->tryGetPriorityParameter( this );
				if ( pp )
				{
					shader = pp->Shader;
					drawing = !pp->Hide;
				}
			}

			if ( drawing )
			{
				if ( shader )
				{ 
					// 優先シェーダ等のため、必要なライト数に不足がある場合は再列挙
					if ( mAffectLightContextArray.size() < shader->getRequiredLightNum() )
					{
						mAffectLightContextArray.resize( shader->getRequiredLightNum() );
						param.SceneGraphContext->selectLights( &mAffectLightContextArray, mCombinedWorldMatrix.GetPosition() );
					}

					
					if ( !shader->hasScriptExternal() )
					{
						SceneShaderTechniqueArray* techs = shader->findTechnique( MMD_PASS_object, true, false, false );
						if ( techs )
						{
							shader->updateNodeParam( this );
							this->onSetNodeParameter( shader );

							int size = techs->size();
							for ( int i = 0; i < size; ++i )
							{
								DrawingCommandList* list = (*techs)[i]->getDrawingCommandList();
								list->setOwnerVisualNodeContext( this );	
								list->execute( param );
							}
						}
					}
				}
				// シェーダが無い場合はそのまま全部 drawSubset()
				else
				{
					param.Renderer->setRenderState(getRenderState());

					int subsetCount = getSubsetCount();
					for ( int i = 0 ; i < subsetCount; ++i )
					{
						drawSubset( param, i );
					}
				}
			}
		}

		// 子の描画
		SceneNodeContext::render( param );
	}

#if 0

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    VisualNodeContext::VisualNodeContext()
        : mSceneNode            ( NULL )
        , mSubsetNum            ( 0 )
        , mSubsetRenderParams   ( NULL )
        , mPriorityParameter    ( NULL )
        , mPriority             ( 0 )
        , mBillboardType        ( LN_BILLBOARD_NOT )
        , mShader               ( NULL )
        , mZDistance            ( 0 )
        , mAffectCameraContext  ( NULL )
        , mIsVisible            ( true )
		, mEnableDrawing		( true )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    VisualNodeContext::~VisualNodeContext()
    {
        SAFE_DELETE_ARRAY( mSubsetRenderParams );
        releaseAllResource();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void VisualNodeContext::initialize( SceneNode* node_/*, lnU32 subset_num_*/ )
    {
        mSceneNode = node_;

        //if ( subset_num_ > 0 )
        //{
        //    mSubsetRenderParams = LN_NEW LNSubsetRenderParam[subset_num_];
        //    mSubsetNum = subset_num_;
        //}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void VisualNodeContext::updateContext( VisualNodeContext* parent_context_ )
    {
        releaseAllResource();

		mName = mSceneNode->mName;

		if ( this->mSubsetNum != mSceneNode->getSubsetCount() )
		{
			this->mSubsetNum = mSceneNode->getSubsetCount();

			// サブセットパラメータ配列を再確保
			SAFE_DELETE_ARRAY( mSubsetRenderParams );
			if ( this->mSubsetNum > 0 )
			{
				mSubsetRenderParams = LN_NEW LNSubsetRenderParam[this->mSubsetNum];
			}
		}

		// LMaterial 内ではリソースの参照カウントを変化させているので、memcpy は NG
		for ( lnU32 i = 0; i < this->mSubsetNum; ++i )
		{
			mSubsetRenderParams[i] = mSceneNode->mNodeRenderParam.Subset[i];
		}

        // 親がある場合は 0 番サブセットのコミット済み情報を受け継ぐ
        if ( parent_context_ )
        {
            const LNSubsetRenderParam& pp = parent_context_->mSubsetRenderParams[ 0 ];

            for ( lnU32 i = 0; i < mSubsetNum; ++i )
            {
                LNSubsetRenderParam& pa = mSubsetRenderParams[ i ];

                ((LVector4&)pa.ColorScale) = ((LVector4&)pa.ColorScale) * ((LVector4&)pp.ColorScale);
                ((LVector4&)pa.BlendColor) = ((LVector4&)pa.BlendColor) + ((LVector4&)pp.BlendColor);
                ((LVector4&)pa.Tone)       = ((LVector4&)pa.Tone)       + ((LVector4&)pp.Tone);
            }
        }

        mRenderState = mSceneNode->mNodeRenderParam.RenderState;

        mPriorityParameter = mSceneNode->mPriorityParameter;    // NULL でいいかも…
        mCombinedWorldMatrix = mSceneNode->mCombinedWorldMatrix;
        mPriority = mSceneNode->mPriority;
        mBillboardType = mSceneNode->mBillboardType;
        mShader = mSceneNode->mShader;
        LN_SAFE_ADDREF( mShader );
        mIsVisible = mSceneNode->mIsVisible;

        // 必要なライトの数で配列を resize
        if ( mShader )
        {
			if ( mAffectLightContextArray.size() < mShader->getRequiredLightNum() )
			{
				mAffectLightContextArray.resize( mShader->getRequiredLightNum() );
			}
        }

        // 子ノードのコンテキスト
#if 1
		SceneNode::SceneNodeRefArray::ObjectArray& ary = mSceneNode->mChildNodeList;
        int n = ary.size();
        mChildSceneNodeContextArray.resize( n );
        for ( int i = 0; i < n; ++i )
        {
            mChildSceneNodeContextArray[i] = ary[i]->mContext;
        }
#else
		SceneNode::SceneNodeRefArray::ObjectArray& ary = mSceneNode->mChildNodeRefArray.getObjectArray();
        int n = ary.size();
        mChildSceneNodeContextArray.resize( n );
        for ( int i = 0; i < n; ++i )
        {
            mChildSceneNodeContextArray[i] = ary[i]->mContext;
        }
#endif
		// 子ノード
		mChildNodeList.resize( n );
        for ( int i = 0; i < n; ++i )
        {
            mChildNodeList[i] = ary[i];
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void VisualNodeContext::releaseAllResource()
    {
        LN_SAFE_RELEASE( mShader );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void VisualNodeContext::preRender( ContextDrawParam& param )
    {
		mAffectCameraContext = param.CameraContext;

        //-----------------------------------------------------
        // Z ソート用の基準値

		switch ( param.CameraContext->getZSortType() )
        {
            default:
            case LN_ZSORTF_IMM_Z:
                mZDistance = mCombinedWorldMatrix.getPosition().z;
                break;
            case LN_ZSORTF_CAMERA_DIST:
                mZDistance = LVector3::squareLength( mCombinedWorldMatrix.getPosition() - param.CameraContext->getPosition() );
                break;
            case LN_ZSORTF_CAMERA_SCREEN:
                mZDistance = LGeometry::PointPolyDistance(
                    mCombinedWorldMatrix.getPosition(), 
                    param.CameraContext->getPosition(),
                    param.CameraContext->getDirection() );
                break;
        }

		//std::stable_sort( mChildSceneNodeContextArray.begin(), mChildSceneNodeContextArray.end(), _cmpZAndPrioritySort );

        //-----------------------------------------------------
        // ビルボード行列計算

        // 正面を向くビルボードの場合
        if ( mBillboardType == LN_BILLBOARD_FRONT )
        {
            LMatrix mat;

            // 現在のワールド行列から回転部分だけを取り出す
            mat.setRotateMatrix( mCombinedWorldMatrix );

            // 回転部分だけを乗算
            mat.setMul3x3( param.CameraContext->getViewMatrixInverse() );

            // 移動成分を乗算しなおす
            mat.translation( mCombinedWorldMatrix.m30, mCombinedWorldMatrix.m31, mCombinedWorldMatrix.m32 );

            mCombinedWorldMatrix = mat;

            /*
            // 現在のワールド行列から回転部分だけを取り出す
            mBillboardMatrix.setRotateMatrix( mCombinedWorldMatrix );

            // 回転部分だけを乗算
            mBillboardMatrix.setMul3x3( camera_->getViewMatrixInverse() );

            // 移動成分を乗算しなおす
            mBillboardMatrix.translation( mCombinedWorldMatrix.m30, mCombinedWorldMatrix.m31, mCombinedWorldMatrix.m32 );
            */
       
        }
        // Y 軸回転のみ行うビルボードの場合
        else if ( mBillboardType == LN_BILLBOARD_ROT_Y )
        {
            const LMatrix& view = param.CameraContext->getViewMatrix();

            LMatrix mat;
            mat.setRotateMatrix( mCombinedWorldMatrix );

            if ( view.m02 > 0.0f )
		    {
                mat.rotationY( -atanf( view.m22 / view.m02 ) + LMath::PI / 2 );
		    }   
		    else if ( view.m02 == 0.0f )
		    {
                mat.identity();
		    }
		    else
		    {
                mat.rotationY( -atanf( view.m22 / view.m02 ) - LMath::PI / 2 );
		    }

            mat.translation( mCombinedWorldMatrix.getPosition() );

            mCombinedWorldMatrix = mat;
        }

        //-----------------------------------------------------
        // ライト列挙

        // ライトが必要な場合 (mAffectLightContextArray は updateContext で resize() 済み)
        if ( mAffectLightContextArray.size() > 0 )
        {
			// 自分の位置情報を渡して、最寄りのライトをもらう
            param.SceneGraphContext->selectLights( &mAffectLightContextArray, mCombinedWorldMatrix.getPosition() );
        }

		// TODO: 視錐台カリングとかできれば
		this->mEnableDrawing = this->mIsVisible;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void VisualNodeContext::render( ContextDrawParam& param )
	{
		if ( this->mEnableDrawing )
        {
			// レンダーステートの設定
			param.Renderer->setRenderState( this->mRenderState );

			// シェーダ選択
			SceneShader* shader = getShader();

			// 優先パラメータチェック
			bool drawing = true;
			if ( param.OffscreenScene )
			{
				LNPriorityParameter* pp = param.OffscreenScene->tryGetPriorityParameter( this );
				if ( pp )
				{
					shader = pp->Shader;
					drawing = !pp->Hide;
				}
			}
			//if ( mPriorityParameter )
			//{
			//	shader = mPriorityParameter->Shader;
			//}

			if ( drawing )
			{

				if ( shader )
				{ 
					// 優先シェーダ等のため、必要なライト数に不足がある場合は再列挙
					if ( mAffectLightContextArray.size() < shader->getRequiredLightNum() )
					{
						mAffectLightContextArray.resize( shader->getRequiredLightNum() );
						param.SceneGraphContext->selectLights( &mAffectLightContextArray, mCombinedWorldMatrix.getPosition() );
					}

					
					if ( !shader->hasScriptExternal() )
					{
						SceneShaderTechniqueArray* techs = shader->findTechnique( MMD_PASS_object, true, false, false );
						if ( techs )
						{
							shader->updateNodeParam( this );
							this->onSetNodeParameter( shader );

							int size = techs->size();
							for ( int i = 0; i < size; ++i )
							{
								DrawingCommandList* list = (*techs)[i]->getDrawingCommandList();
								list->setOwnerSceneNode( mSceneNode );	// TODO:この辺　Contextにした方が・・・
								list->execute( param );
							}
						}
					}
				}
				// シェーダが無い場合はそのまま全部 drawSubset()
				else
				{
					for ( lnU32 i = 0 ; i < mSubsetNum; ++i )
					{
						drawSubset( i );
					}
				}
			}
		}

		if ( !mChildSceneNodeContextArray.empty() )
		{
			// 子のZソート
			std::stable_sort( mChildSceneNodeContextArray.begin(), mChildSceneNodeContextArray.end(), _cmpZAndPrioritySort );

			// 子呼び出し
			//SceneNodeArray::iterator itr = mChildNodeRefArray.getObjectArray().begin();
			//SceneNodeArray::iterator end = mChildNodeRefArray.getObjectArray().end();
			SceneNodeContextArray::iterator itr = this->mChildSceneNodeContextArray.begin();
			SceneNodeContextArray::iterator end = this->mChildSceneNodeContextArray.end();
			for ( ;itr != end; ++itr )
			{
				(*itr)->render( param );
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool VisualNodeContext::getObjectMatrix( LMatrix* out_, MMEVariableRequest req_, int light_idx_ ) const
    {
        switch ( req_ )
        {
            case MME_VARREQ_MATRIX_World:
            {
                *out_ = mCombinedWorldMatrix;
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldView:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewMatrix();
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewProjectionMatrix();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldView:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewMatrix();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewProjectionMatrix();
                return true;
            }

            // 逆行列
            case MME_VARREQ_MATRIX_World_I:
            {
                LMatrix::inverse( out_, mCombinedWorldMatrix );
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldView_I:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewMatrix();
                out_->inverse();
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_I:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewProjectionMatrix();
                out_->inverse();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldView_I:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewMatrix();
                out_->inverse();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_I:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewProjectionMatrix();
                out_->inverse();
                return true;
            }

            // 転置行列
            case MME_VARREQ_MATRIX_World_T:
            {
                LMatrix::transpose( out_, mCombinedWorldMatrix );
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldView_T:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewMatrix();
                out_->transpose();
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_T:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewProjectionMatrix();
                out_->transpose();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldView_T:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewMatrix();
                out_->transpose();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_T:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewProjectionMatrix();
                out_->transpose();
                return true;
            }

            // 逆行列×転置行列
            case MME_VARREQ_MATRIX_World_IT:
            {
                LMatrix::inverse( out_, mCombinedWorldMatrix );
                out_->transpose();
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldView_IT:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewMatrix();
                out_->inverse();
                out_->transpose();
                return true;
            }
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_IT:
            {
                *out_ = mCombinedWorldMatrix * mAffectCameraContext->getViewProjectionMatrix();
                out_->inverse();
                out_->transpose();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldView_IT:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewMatrix();
                out_->inverse();
                out_->transpose();
                return true;
            }
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT:
            {
                *out_ = mCombinedWorldMatrix * mAffectLightContextArray[ light_idx_ ]->getViewProjectionMatrix();
                out_->inverse();
                out_->transpose();
                return true;
            }
        }

        return false;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool VisualNodeContext::_cmpZAndPrioritySort( const VisualNodeContext* l_, const VisualNodeContext* r_ )
    {
        if ( l_->mPriority == r_->mPriority )
        {
            // 降順 (遠い方から)
            return l_->mZDistance > r_->mZDistance;
        }
        // 降順 (高い方から)
        return l_->mPriority < r_->mPriority;
    }
#endif
  
} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================