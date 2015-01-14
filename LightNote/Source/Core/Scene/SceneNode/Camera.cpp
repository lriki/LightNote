//==============================================================================
// Camera 
//------------------------------------------------------------------------------
/*
	■ Camera は SceneNode を継承する？
		させる場合は SceneNode の姿勢設定 (setPosition や setAngle 等) を
		オーバーライドする必要がある。
		そもそも setLookAt した時の setAngle ってどうするの？とか
		いろいろ考える必要がある。
		
		…が、基本的に SceneNode の姿勢はすべて無視して Camera は独自で持ちそう。
		そうなると、SceneNode を継承する意味は
		①別の Node の親や子にしたい
		②シェーダの CONTROLOBJECT で値を撮りたい
		とかそのあたりになってきそう。
		逆にこれらが必要にならない限り SceneNode を継承するべきじゃない。
		setLookAt って普通グローバルで見たいよね？とか、Camera 独特の要件もある。
*/
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Physics/Manager.h"
#include "../../Graphics/Common/GeometryRenderer.h"
#include "../Manager.h"
#include "../SceneGraph.h"
#include "../Layer.h"
#include "../SceneShader/SceneShaderManager.h"
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
// ■ Camera
//==============================================================================

	LN_TYPE_INFO_ACCESS_IMPL(Camera);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Camera::Camera( SceneGraph* scene )
        : //SceneNode				( scene )
		 mScene                ( scene )
		//, mParentPane			( NULL )
		, mParentLayer(NULL)
        , mZSortFunc            ( LN_ZSORTF_IMM_Z )
        , mContext              ( NULL )
        , mMemberChanged        ( false )
        //, mProjection2D         ( false )
#if LNOTE_SCENE_CAMERA_SCREEN_EFFECT
        , mScreenEffectMode		( LN_SCREENEFFECTMODE_NONE )
		, mScreenEffect			( NULL )
#endif
		, mRenderRootNode		( NULL )
//		, mRenderTarget			( NULL )
    {
        if ( mScene )
        {
            mScene->addCamera( this );
        }
        LN_SAFE_ADDREF( mScene );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Camera::~Camera()
    {
#if LNOTE_SCENE_CAMERA_SCREEN_EFFECT
		_disposeScreenEffectResource();
#endif
        SAFE_DELETE( mContext );
        LN_SAFE_RELEASE( mScene );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Camera::initialize( bool projection_2d )
    {
        mCoreCamera.initialize( projection_2d );
        mContext = LN_NEW CameraContext();
        mContext->initialize( this );
    }

#if LNOTE_SCENE_CAMERA_SCREEN_EFFECT
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Camera::setScreenEffectMode( LNScreenEffectMode mode )
    {
        if ( mode != mScreenEffectMode )
        {
            _disposeScreenEffectResource();

            if ( mode == LN_SCREENEFFECTMODE_BUFFERINGEFFECT )
            {
				_createScreenEffectResource();
            }

            mScreenEffectMode = mode;
        }
    }
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Camera::blur(
        double duration,
        lnFloat power,
        lnFloat scale,
        const LVector3& center,
        bool center_is_3d )
    {
        LVector3 pos;

        if ( mProjection2D )
        {
            LVector3::transformCoord( &pos, center, getProjectionMatrix() );
        }
        else
        {
            LVector3::transformCoord( &pos, center, getViewProjectionMatrix() );
        }

        mScreenEffect->blur( duration, power, scale, pos );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Camera::updateFrame( const Game::GameTime& gameTime )
	{
		if ( mScreenEffectMode != LN_SCREENEFFECTMODE_BUFFERINGEFFECT )
		{
#if 0
			if ( mRootNode3D->getSceneNodeType() == LN_SCENENODE_VIEWPORT )
			{
				((Viewport*)mRootNode3D)->setTone( (const LTone&)mTone.getValue(), 0 );
			}
			if ( mRootNode2D->getSceneNodeType() == LN_SCENENODE_VIEWPORT )
			{
				((Viewport*)mRootNode2D)->setTone( (const LTone&)mTone.getValue(), 0 );
			}
#endif
		}
        else
        {
			if ( mScreenEffect )
			{
				mScreenEffect->update( gameTime.getElapsedTime() );
			}
        }
	}
#endif

	////---------------------------------------------------------------------
	////
 //   //---------------------------------------------------------------------
	//void Camera::setRenderTarget( Graphics::ITexture* texture )
	//{ 
	//	LN_REFOBJ_SET( mRenderTarget, texture );
 //       mCoreCamera.setViewSize( getViewSize() );
	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	const LVector2& Camera::getViewSize() const
	{
		LN_ERR2_ASSERT(mParentLayer);
		return mParentLayer->getParentPane()->getViewSize();
	}

	//----------------------------------------------------------------------
	// ● カメラから見た 3D 座標を 2D 座標に変換する
	//----------------------------------------------------------------------
    void Camera::convertWorldPosToScreenPos( LVector3* out, const LVector3& world_pos ) const
    {
		mCoreCamera.convertWorldPosToScreenPos( out, world_pos, getViewSize() );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Camera::updateContext() 
	{ 
#if LNOTE_SCENE_CAMERA_SCREEN_EFFECT
		if ( mScreenEffect ) mScreenEffect->updateDrawContent();
#endif
		mContext->updateContext();
	}

	//----------------------------------------------------------------------
	// ● カメラ単位でシェーダに送るデータの取得
	//----------------------------------------------------------------------
    LNCameraSceneParam* Camera::getCameraSceneParam()
    {
        mCameraSceneParam.Mat_CameraView     = &mCoreCamera.getViewMatrix();
        mCameraSceneParam.Mat_CameraProj     = &mCoreCamera.getProjectionMatrix();
        mCameraSceneParam.Mat_CameraViewProj = &mCoreCamera.getViewProjectionMatrix();
        mCameraSceneParam.CameraPosition     = LVector4( mCoreCamera.getPosition(), 0.0f );
        mCameraSceneParam.CameraDirection    = LVector4( mCoreCamera.getDirection(), 0.0f );
        mCameraSceneParam.calcMatrices();
        return &mCameraSceneParam;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool Camera::onEvent( const System::EventArgs& e )
    {
        if ( mCoreCamera.onEvent( e ) ) return true;
        return false;
    }

#if LNOTE_SCENE_CAMERA_SCREEN_EFFECT
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Camera::_createScreenEffectResource()
	{
        mScreenEffect = LN_NEW Graphics::ScreenEffect();
        mScreenEffect->initialize( mScene->getGraphicsManager()->getGraphicsDevice(), getViewSize() );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Camera::_disposeScreenEffectResource()
    {
        LN_SAFE_RELEASE( mScreenEffect );
    }
#endif

//==============================================================================
// ■ CameraContext
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    CameraContext::CameraContext()
        : mCamera       ( NULL )
        , mFovY         ( 0 )
        , mNearClip     ( 0 )
        , mFarClip      ( 0 )
        , mZSortFunc    ( LN_ZSORTF_IMM_Z )
        , mProjection2D ( false )
        , mOrthographic ( false )
//		, mRenderTarget	( NULL )
#if LNOTE_SCENE_CAMERA_SCREEN_EFFECT
		, mScreenEffect	( NULL )
#endif
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    CameraContext::~CameraContext()
    {
//		LN_SAFE_RELEASE( mRenderTarget );
#if LNOTE_SCENE_CAMERA_SCREEN_EFFECT
		LN_SAFE_RELEASE( mScreenEffect );
#endif
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void CameraContext::initialize( Camera* camera_ )
    {
        mCamera = camera_;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void CameraContext::updateContext()
    {
		mViewSize = mCamera->getViewSize();




        mPosition = LVector4( mCamera->mCoreCamera.getPosition(), 0 );
        mDirection = mCamera->mCoreCamera.getDirection();

        mFovY = mCamera->mCoreCamera.getFovY();
        mNearClip = mCamera->mCoreCamera.getNearClip();
        mFarClip = mCamera->mCoreCamera.getFarClip();

        mViewMatrix = mCamera->mCoreCamera.getViewMatrix();
        mProjectionMatrix= mCamera->mCoreCamera.getProjectionMatrix();
        mViewProjMatrix = mCamera->mCoreCamera.getViewProjectionMatrix();

        mZSortFunc = mCamera->mZSortFunc;

        mCameraSceneParam.Mat_CameraView = &mViewMatrix;
        mCameraSceneParam.Mat_CameraProj = &mProjectionMatrix;
        mCameraSceneParam.Mat_CameraViewProj = &mViewProjMatrix;
        mCameraSceneParam.CameraPosition = mPosition;
        mCameraSceneParam.CameraDirection = LVector4( mDirection, 0.0f );
		mCameraSceneParam.ViewPixelSize.Set( mViewSize.X, mViewSize.Y, 0, 0 );
		mCameraSceneParam.calcMatrices();

        mProjection2D = mCamera->isProjection2D();
        mOrthographic = mCamera->mOrthographic;

		this->mRenderRootNode = mCamera->mRenderRootNode;
//		LN_REFOBJ_SET( this->mRenderTarget, mCamera->mRenderTarget );

#if LNOTE_SCENE_CAMERA_SCREEN_EFFECT
		LN_REFOBJ_SET( this->mScreenEffect, mCamera->mScreenEffect );
		this->mBackgroundColor = mCamera->mBackgroundColor;
#endif
		
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void CameraContext::render( ContextDrawParam* param )
	{
		param->CameraContext = this;
		param->RenderRootNode = mRenderRootNode;

		//param->Renderer->setRenderTarget( 0, mRenderTarget );

#if LNOTE_SCENE_CAMERA_SCREEN_EFFECT
		// スクリーンエフェクトの前処理
		Graphics::ITexture* oldRenderTarget = NULL;
		Graphics::ITexture* oldDepthBuffer = NULL;
		if ( mScreenEffect )
		{
			oldRenderTarget = param.Renderer->getRenderTarget( 0 );
			oldDepthBuffer  = param.Renderer->getDepthBuffer();

			param.Renderer->setRenderTarget( 0, mScreenEffect->getPrimaryRenderTarget(), true );
			param.Renderer->setDepthBuffer( mScreenEffect->getPrimaryDepthBuffer() );
			param.Renderer->clear( true, true, mBackgroundColor, 1.0f );
		}
#endif

		mCamera->mScene->getSceneGraphContext()->render( *param );

		if ( !mCamera->isProjection2D() )
		{
			// デバッグ描画
			// TODO: 本当にここでいいのか検討。
			//		 とりあえずカメラ行列が必要なのでここに置いている。
			param->GeometryRenderer->setViewProjMatrix( mViewProjMatrix );
			param->GeometryRenderer->begin();
			mCamera->mScene->getPhysicsManager()->drawDebugShape( param->GeometryRenderer );
			param->GeometryRenderer->end();
		}

#if LNOTE_SCENE_CAMERA_SCREEN_EFFECT
		// スクリーンエフェクトの後処理
		if ( mScreenEffect )
		{
			mScreenEffect->renderBlurAndTransition( param.Renderer, param.GeometryRenderer );

			param.Renderer->setRenderTarget( 0, oldRenderTarget, true );
			param.Renderer->setDepthBuffer( oldDepthBuffer );

			mScreenEffect->renderScreen( param.Renderer, param.GeometryRenderer );
			mScreenEffect->swapBuffers();
		}
#endif
	}

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================