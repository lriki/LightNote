//==============================================================================
// Light 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Graphics/Common/GeometryRenderer.h"
#include "../Manager.h"
#include "../SceneGraph.h"
#include "../Pane.h"
#include "Light.h"
#include "Camera.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// ■ Light クラス
//==============================================================================

	//----------------------------------------------------------------------
	// ● コンストラクタ
	//----------------------------------------------------------------------
    Light::Light( SceneGraph* scene_ )
        : SceneNode             ( scene_ )//mScene                ( scene_ )
        , mType                 ( LN_LIGHTTYPE_DIRECTIONAL )
        //, mPosition             ( 10.0f, 10.0f, -10.0f ) 
        , mDirection            ( 0, 0, 1 )
        , mDiffuse              ( 1.0f, 1.0f, 1.0f, 1.0f )
        , mAmbient              ( 1.0f, 1.0f, 1.0f, 1.0f )//( 0.5f, 0.5f, 0.5f, 1.0f )
        , mSpecular             ( 1.0f, 1.0f, 1.0f, 1.0f )//( 0.5f, 0.5f, 0.5f, 1.0f )
        , mLightingPriority     ( 0 )
        , mRange                ( 500.0f )
        , mSpotInAngle          ( LMath::PI * 0.2f )
        , mSpotOutAngle         ( LMath::PI * 0.25f )
        //, mShadowStrength       ( 0.5f )
        //, mShadowBuffer         ( NULL )
        , mLightContext         ( NULL )
        , mEnable               ( true )
        , mNeedUpdateMatrices   ( true )
    {
        if ( this->mSceneGraph )
        {
            this->mSceneGraph->addLight( this );
        }
        //LN_SAFE_ADDREF( mScene );
    }

	//----------------------------------------------------------------------
	// ● デストラクタ
	//----------------------------------------------------------------------
    Light::~Light()
    {
        SAFE_DELETE( mLightContext );
		//LN_SAFE_RELEASE( mScene );
    }

	//----------------------------------------------------------------------
	// ● 初期化
	//----------------------------------------------------------------------
    void Light::create()
    {

        /*
        // シャドウバッファ作成
        LN_CALL_R(
            mScene->getGraphicsManager()->getGraphicsDevice()->getResourceManager()->createRenderTarget(
                &mShadowBuffer,
                static_cast< lnU32 >( shadowbuf_size_.x ),
                static_cast< lnU32 >( shadowbuf_size_.y ),
                1,
                LN_FMT_A16B16G16R16F ) );
                */

        
        //LN_PRINT_NOT_IMPL_FUNCTION;

        SceneNode::initialize( 1, LN_DRAWCOORD_3D );


        //mPosition.set( 100.0f, 100.0f, -100.0f );
        //this->setPosition( LVector3( 120.0f, 120.0f, -120.0f ) );   // 行列計算に反映させるため、関数で設定
        setPosition( LVector3( 50.0f, 50.0f, -120.0f ) );


		this->mDirection = LVector3::Zero - getPosition();
		this->mDirection.Normalize();

        mLightContext = LN_NEW LightContext();
        mLightContext->initialize( this );
    }

	//----------------------------------------------------------------------
	// ● シャドウバッファを作成する
	//----------------------------------------------------------------------
    /*
    LNRESULT Light::createShadowBuffer( lnU32 width_, lnU32 height_ )
    {
        LNRESULT lr;

        LN_PRINT_NOT_IMPL_FUNCTION;

        //LN_SAFE_RELEASE( mShadowBuffer );

        //if ( width_ != 0 && height_ != 0 )
        //{
        //    LN_CALL_R( mScene->getGraphicsManager()->getGraphicsDevice()->getResourceManager()->createTexture(
        //        &mShadowBuffer, width_, height_, 1, LN_FMT_A16B16G16R16F ) );

        //    // アスペクト比
        //    mViewFrustum.setAspect( width_ / height_ );
        //    mViewFrustum.setFar( 100 ); // とりあえず

        //    LN_PRINT_NOT_IMPL_FUNCTION;
        //}
        return LN_OK;
    }
    */

	//----------------------------------------------------------------------
	// ● 現在の状態からビュー行列を作成して out_ に格納する
	////---------------------------------------------------------------------
	//void Light::getViewMatrix( LMatrix* out_, const LVector3& up_ )
	//{
	//    LMath::MatrixViewTransformRH( out_, mPosition, mPosition + mDirection, up_ );
	//}

	//----------------------------------------------------------------------
	// ● 現在の状態からプロジェクション行列を作成して out_ に格納する
	//----------------------------------------------------------------------
	//void Light::getProjectionMatrix( LMatrix* out_, lnFloat fov_y_, lnFloat near_, lnFloat far_ )
	//{
	//    const LVector2& size = mShadowBuffer->getRealSize();

	//    LMath::MatrixPerspectiveFovRH( out_, fov_y_, size.x, size.y, near_, far_ );
	//}

	//----------------------------------------------------------------------
	// ● フレーム更新 (SceneBase::update() から呼ばれる)
	//----------------------------------------------------------------------
	//void Light::update( const Engine::Game::GameTime& game_time_ )
	//{
	//    LN_NOT_IMPL_FUNCTION
	//    /*
	//    // シャドウを使う場合は視錐台を更新する
	//    if ( mShadowBuffer )
	//    {
	//        LMatrix vmat;
	//        LMath::MatrixInverse( &vmat, getViewMatrix() );

	//        mViewFrustum.update( &vmat );

	//        // デバッグ描画が必要であれば描画する
	//        if ( mScene->isEnableDebugDraw() )
	//        {
	//            DebugShapes* ds = mScene->getDebugShapes();

	//            LMatrix mat;
	//            mat.translation( mPosition );

	//            ds->setSolidColor( 0xff888888 );
	//            ds->requestDrawSphere( mat, 0.1f );
	//            ds->requestDrawFrustum( mViewFrustum, mPosition );
	//        }
	//    }
	//    */
	//}

	//----------------------------------------------------------------------
	// ● デフォルトライトかを判定
	//----------------------------------------------------------------------
	//bool Light::isDefaultLight() const
	//{
	//    return ( mScene->getDefaultLight() == this );
	//}

	//----------------------------------------------------------------------
	// ● デバッグ用の描画
	//----------------------------------------------------------------------
    void Light::drawDebug( lnU32 flags_ )
    {
        _updateMatrices();

        if ( flags_ & LN_SCENEDEBUGDRAW_LIGHT )
        {
            //LN_PRINT_NOT_IMPL_FUNCTION;
        }

        if ( flags_ & LN_SCENEDEBUGDRAW_LIGHT_FRUSTUM )
        {
            //this->mSceneGraph->getGeometryRenderer()->setMatrix( LMatrix::IDENTITY );
            //this->mSceneGraph->getGeometryRenderer()->setViewProjMatrix( LMatrix::IDENTITY );
            //this->mSceneGraph->getGeometryRenderer()->setTexture( NULL );

            //this->mSceneGraph->getGeometryRenderer()->begin();
            //this->mSceneGraph->getGeometryRenderer()->drawViewFrustum(
            //    mViewFrustum, this->mCombinedWorldMatrix.getPosition(), 0xff888888 );
            //this->mSceneGraph->getGeometryRenderer()->end();
        }
    }

	//----------------------------------------------------------------------
	// ● mNeedUpdateMatrices が true の場合、行列計算を行う
	//----------------------------------------------------------------------
    void Light::_updateMatrices() const
    {
        //if ( mShadowBuffer && mNeedUpdateMatrices )
        if ( mNeedUpdateMatrices )
        {
			// TODO: いまはメインカメラのサイズ。シャドウマップを作るようにする場合は、そのサイズを使う
			const LVector2& size = mSceneGraph->getDefaultCamera()->getViewSize();
            LVector3 look_at = mPosition + mDirection;
            LVector3 up = LVector3( 0, 1, 0 );

            //LMatrix::viewTransformRH( &mViewMatrix, mPosition, look_at, up );
            //LMatrix::perspectiveFovRH( &mProjectionMatrix, mViewFrustum.Fov, size.x, size.y, mViewFrustum.Near, mViewFrustum.Far );
			mViewMatrix = LMatrix::LookAtLH(mPosition, look_at, up);

            // 視錐台の更新
            mViewFrustum.Fov    = LMath::PI / 4.0f;
            mViewFrustum.Aspect = size.X / size.Y;
            mViewFrustum.Near   = 0.01f;
            mViewFrustum.Far    = mRange;
			LMatrix vmat = LMatrix::Inverse(mViewMatrix);
            
            mViewFrustum.update( &vmat );

            // プロジェクション行列の更新
			mProjectionMatrix = LMatrix::PerspectiveFovLH(mViewFrustum.Fov, size.X / size.Y, mViewFrustum.Near, mViewFrustum.Far);

            mViewProjMatrix = mViewMatrix * mProjectionMatrix;

            mNeedUpdateMatrices = false;
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Light::updateContext()
    {
        mLightContext->updateContext();
    }

//==============================================================================
// ■ LightContext
//==============================================================================

    LightContext LightContext::DUMMY;

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    LightContext::LightContext()
        : mLight            ( NULL )
        , mType             ( LN_LIGHTTYPE_DIRECTIONAL )
        , mLightingPriority ( 0 )
        , mRange            ( 0 )
        , mSpotInAngle      ( 0 )
        , mSpotOutAngle     ( 0 )
        , mEnable           ( true )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void LightContext::initialize( Light* light_ )
    {
        mLight = light_;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void LightContext::updateContext()
    {
        mType = mLight->mType;
        mPosition = mLight->mPosition;
        mDirection = mLight->mDirection;
        mDiffuse = mLight->mDiffuse;
        mAmbient = mLight->mAmbient;
        mSpecular = mLight->mSpecular;
        mLightingPriority = mLight->mLightingPriority;
        mRange = mLight->mRange;
        mSpotInAngle = mLight->mSpotInAngle;
        mSpotOutAngle = mLight->mSpotOutAngle;
        mViewMatrix = mLight->mViewMatrix;
        mProjectionMatrix = mLight->mProjectionMatrix;
        mViewProjMatrix = mLight->mViewProjMatrix;
        mEnable = mLight->mEnable;
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================