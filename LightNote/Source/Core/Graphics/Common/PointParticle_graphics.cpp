//==============================================================================
// PointParticle 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "PointParticle.h"

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
// ■ PointParticle クラス
//==============================================================================

	/// ポイントスプライトを使ったパーティクル用の頂点
    struct PointParticleVertex
    {
        LVector3	Position;	///< 位置
	    LVector3	Velocity;	///< 初速度 (秒速)
	    LVector3	Accel;		///< 加速度
	    LVector3	Axis;		///< 回転軸
	    LVector3	AngleData;	///< 回転情報 [ 初期角度, 角速度, 角加速度 ]
	    LVector3	SizeData;	///< 大きさ   [ 初期, 速度, 加速度 ]
        LVector4    TimeData;   ///< [ 開始時間, 存在時間, フェードイン時間, フェードアウト時間 ] ※全てフレーム数。頂点シェーダ内で除数考慮する方向で。
	    LVector4	FadeData;	///< [ 残像用アルファ, 残像用時間オフセット ]

	    /// 頂点レイアウト
	    static LNVertexElemenst* elements()
	    {
		    static LNVertexElemenst elements[] = {
			    { 0, LN_DECL_FLOAT3, LN_USAGE_POSITION, 0 },
			    { 0, LN_DECL_FLOAT3, LN_USAGE_TEXCOORD, 0 },
			    { 0, LN_DECL_FLOAT3, LN_USAGE_TEXCOORD, 1 },
			    { 0, LN_DECL_FLOAT3, LN_USAGE_TEXCOORD, 2 },
			    { 0, LN_DECL_FLOAT3, LN_USAGE_TEXCOORD, 3 },
			    { 0, LN_DECL_FLOAT3, LN_USAGE_TEXCOORD, 4 },
			    { 0, LN_DECL_FLOAT4, LN_USAGE_TEXCOORD, 5 },
			    { 0, LN_DECL_FLOAT4, LN_USAGE_TEXCOORD, 6 },
			      LN_DECL_END() };
		    return elements;
	    }
    };

	//----------------------------------------------------------------------
	// ● コンストラクタ
	//----------------------------------------------------------------------
    PointParticle::PointParticle()
        : mRenderer             ( NULL )
        , mVertexBuffer         ( NULL )
        , mOriginalPositions    ( NULL )
        , mCreatedPositions     ( NULL )
        , mParticleNum          ( 0 )
        , mAfterImageLevel      ( 0 )
        , mOneLoopFrameNum      ( 0 )
        , mTimeTick             ( 60 )
        , mTime                 ( 0 )
        , mLastTime             ( 0 )
        , mTexture              ( NULL )
        , mIsDynamic            ( false )
    {

    }

	//----------------------------------------------------------------------
	// ● デストラクタ
	//----------------------------------------------------------------------
    PointParticle::~PointParticle()
    {
        LN_SAFE_RELEASE( mTexture );
        LN_SAFE_RELEASE( mVertexBuffer );
        LN_SAFE_RELEASE( mRenderer );
        SAFE_DELETE_ARRAY( mOriginalPositions );
        SAFE_DELETE_ARRAY( mCreatedPositions );
    }

	//----------------------------------------------------------------------
	// ● 初期化
	//----------------------------------------------------------------------
    void PointParticle::initialize( Graphics::IGraphicsDevice* device_, const PointParticleParameter& data_ )
    {
        mParticleNum     = data_.ParticleNum;
        mAfterImageLevel = data_.AfterImageNum;
        mIsDynamic       = data_.Dynamic;

        mRenderer = device_->getRenderer();
        LN_SAFE_ADDREF( mRenderer );

        // 残像の数も考慮した頂点数を計算する
        lnU32 vert_num = data_.ParticleNum + ( data_.ParticleNum * data_.AfterImageNum );

        // 頂点バッファ作成
        mVertexBuffer = device_->createVertexBuffer(
            PointParticleVertex::elements(), vert_num, NULL, mIsDynamic );
        _createVertexData( data_ );

        mGlobalAccel = LVector4( data_.GlobalAccel, 0 );
        mGravityPower = data_.GravityPower;
        mIsSequential = ( data_.GenerationInterval == 0 );
        mIsParticleLoop = data_.Loop;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void PointParticle::setTexture( Texture* texture_ )
    {
		LN_REFOBJ_SET(mTexture, texture_);
        //LN_SAFE_ADDREF( texture_ );
        //LN_SAFE_RELEASE( mTexture );
        //mTexture = texture_;
    }

	//----------------------------------------------------------------------
	// ● 時間を進める
	//----------------------------------------------------------------------
    void PointParticle::advanceTime( lnFloat delta_time_ )
    {
        mTime += delta_time_;
    }

	//----------------------------------------------------------------------
	// ● 再生時間の設定 (時間を直接指定する。ツールなどで使う)
	//----------------------------------------------------------------------
    void PointParticle::setTime( lnFloat time_ )
    {
        mTime = time_;
    }

	//----------------------------------------------------------------------
	// ● 現在の時間を取得する
	//----------------------------------------------------------------------
    lnFloat PointParticle::getTime()
    {
        return mTime;
    }

	//----------------------------------------------------------------------
	// ● 頂点を更新する (advanceTime()、setTime() の後にこれを呼ぶ)
	//----------------------------------------------------------------------
    LNRESULT PointParticle::update( const LMatrix& world_matrix_ )
    {
        if ( mIsDynamic )
        {
			PointParticleVertex* vb = (PointParticleVertex*)mVertexBuffer->lock();

            lnU32 v_num = mVertexBuffer->getVertexCount();
            lnFloat t;
            lnFloat frame = mTime * mTimeTick;
            lnFloat last_frame = mLastTime * mTimeTick;
            lnU32 i;

            for ( i = 0; i < v_num; ++i )
            {
                t = fmod( frame - vb[ i ].TimeData.X, vb[ i ].TimeData.Y );
                
                if ( abs( t ) < frame - last_frame )
                {
                    // 本体の粒子の場合
					if ( i < mParticleNum )
					{
                        vb[ i ].Position = mOriginalPositions[ i ];
                        vb[ i ].Position.TransformCoord( world_matrix_ );
                        mCreatedPositions[ i ] = vb[ i ].Position;
                    }
                    // 残像の場合
                    else
                    {
                        vb[ i ].Position = mCreatedPositions[ ( ( i - mParticleNum ) / mAfterImageLevel ) ];
                    }
                }
            }

            mVertexBuffer->unlock();

            mLastTime = mTime;
        }

		return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	// ● 描画
	//----------------------------------------------------------------------
    LNRESULT PointParticle::draw()
    {
        LNRESULT lr;

        mRenderer->setVertexBuffer( mVertexBuffer );

		mRenderer->drawPrimitive(PrimitiveType_PointList, 0, mVertexBuffer->getVertexCount());

		return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	// ● 頂点バッファを埋める
	//----------------------------------------------------------------------
    void PointParticle::_createVertexData( const PointParticleParameter& data_ )
    {
        LRandom random;
		if (data_.RandSeed) {
			random.SetSeed(data_.RandSeed);
		}

        // 動的生成の場合はオリジナルの座標を覚えておく配列を作る
        if ( mIsDynamic )
        {
            mOriginalPositions = LN_NEW LVector3[ mParticleNum ];
            mCreatedPositions  = LN_NEW LVector3[ mParticleNum ];
        }
        
        mOneLoopFrameNum = 0;

        PointParticleVertex* vertices = (PointParticleVertex*)mVertexBuffer->lock();

        lnFloat begin_frame = 0;
        
		for ( lnU32 i = 0; i < mParticleNum; ++i )
		{
            lnFloat rand_link_rate = random.GetFloatRange( 0.0f, 1.0f );

            vertices[ i ].Position.Set(
                random.getFloatEx( data_.Position.X, data_.PositionRand.X, Math::ExRandType_Range, rand_link_rate, (Math::ExRandRateType)data_.PosRandTypeX ),
                random.getFloatEx( data_.Position.Y, data_.PositionRand.Y, Math::ExRandType_Range, rand_link_rate, (Math::ExRandRateType)data_.PosRandTypeY ),
                random.getFloatEx( data_.Position.Z, data_.PositionRand.Z, Math::ExRandType_Range, rand_link_rate, (Math::ExRandRateType)data_.PosRandTypeZ ) );

            vertices[ i ].Velocity.Set(
                random.getFloatEx( data_.Velocity.X, data_.VelocityRand.X, Math::ExRandType_Range, rand_link_rate, (Math::ExRandRateType)data_.VelRandTypeX ),
                random.getFloatEx( data_.Velocity.Y, data_.VelocityRand.Y, Math::ExRandType_Range, rand_link_rate, (Math::ExRandRateType)data_.VelRandTypeY ),
                random.getFloatEx( data_.Velocity.Z, data_.VelocityRand.Z, Math::ExRandType_Range, rand_link_rate, (Math::ExRandRateType)data_.VelRandTypeZ ) );

            vertices[ i ].Accel.Set(
                random.getFloatEx( data_.Accel.X, data_.AccelRand.X, Math::ExRandType_Range, rand_link_rate, (Math::ExRandRateType)data_.AccelRandTypeX ),
                random.getFloatEx( data_.Accel.Y, data_.AccelRand.Y, Math::ExRandType_Range, rand_link_rate, (Math::ExRandRateType)data_.AccelRandTypeY ),
                random.getFloatEx( data_.Accel.Z, data_.AccelRand.Z, Math::ExRandType_Range, rand_link_rate, (Math::ExRandRateType)data_.AccelRandTypeZ ) );

            vertices[ i ].Axis.Set(
                random.getFloatEx( data_.Axis.X, data_.AxisRand.X, Math::ExRandType_Range, rand_link_rate, (Math::ExRandRateType)data_.AxisRandTypeX ),
                random.getFloatEx( data_.Axis.Y, data_.AxisRand.Y, Math::ExRandType_Range, rand_link_rate, (Math::ExRandRateType)data_.AxisRandTypeY ),
                random.getFloatEx( data_.Axis.Z, data_.AxisRand.Z, Math::ExRandType_Range, rand_link_rate, (Math::ExRandRateType)data_.AxisRandTypeZ ) );

            vertices[ i ].AngleData.Set(
                random.getFloatEx( data_.Angle,         data_.AngleRand,         Math::ExRandType_Range, rand_link_rate, (Math::ExRandRateType)data_.AngleRandType ),
                random.getFloatEx( data_.AngleVelocity, data_.AngleVelocityRand, Math::ExRandType_Range, rand_link_rate, (Math::ExRandRateType)data_.AngVelRandType ),
                random.getFloatEx( data_.AngleAccel,    data_.AngleAccelRand,    Math::ExRandType_Range, rand_link_rate, (Math::ExRandRateType)data_.AngAccelRandType ) );

            vertices[ i ].SizeData.Set(
                random.getFloatEx( data_.Size,         data_.SizeRand,         Math::ExRandType_Range, rand_link_rate, (Math::ExRandRateType)data_.SizeRandType ),
                random.getFloatEx( data_.SizeVelocity, data_.SizeVelocityRand, Math::ExRandType_Range, rand_link_rate, (Math::ExRandRateType)data_.SizeVelRandType ),
                random.getFloatEx( data_.SizeAccel,    data_.SizeAccelRand,    Math::ExRandType_Range, rand_link_rate, (Math::ExRandRateType)data_.SizeAccelRandType ) );
            
            vertices[ i ].TimeData.Set(
                begin_frame,
                random.getFloatEx( data_.Life,    data_.LifeRand,    Math::ExRandType_Range, rand_link_rate, (Math::ExRandRateType)data_.LifeRandType ),
                random.getFloatEx( data_.FadeIn,  data_.FadeInRand,  Math::ExRandType_Range, rand_link_rate, (Math::ExRandRateType)data_.FadeInRandType ),
                random.getFloatEx( data_.FadeOut, data_.FadeOutRand, Math::ExRandType_Range, rand_link_rate, (Math::ExRandRateType)data_.FadeOutRandType ) );
            
            vertices[ i ].FadeData.Set( 1.0f, 0, 0, 0 );

            // 軸は正規化が必要
            if ( vertices[ i ].Axis == LVector3::Zero )
            {
                vertices[ i ].Axis.Set( 0, 1, 0 );
            }
            else
            {
                vertices[ i ].Axis.Normalize();
            }

            // フェードイン・アウトの時間が、生存時間内に収めると重なってしまう場合は生存時間を延ばす
			if ( vertices[ i ].TimeData.Z + vertices[ i ].TimeData.W > vertices[ i ].TimeData.Y )
			{
				vertices[ i ].TimeData.Y = vertices[ i ].TimeData.Z + vertices[ i ].TimeData.W;
			}
           

            if ( mOriginalPositions )
            {
                mOriginalPositions[ i ] = vertices[ i ].Position;
            }

            begin_frame += data_.GenerationInterval;
        }

        // 残像
        lnU32 idx;
        lnFloat ai_level_inv = 1.0f / mAfterImageLevel;

        for ( lnU32 i = 0; i < mAfterImageLevel; ++i )
        {
            for ( lnU32 j = 0; j < mParticleNum; ++j )
            {
                idx = ( i + 1 ) * mParticleNum + j;
                vertices[ idx ] = vertices[ j ];

                vertices[ idx ].TimeData.X += 1.0f * ( i + 1 );

                vertices[ idx ].FadeData.X = ai_level_inv * ( mAfterImageLevel - i );
            }
        };
        
        mVertexBuffer->unlock();
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================