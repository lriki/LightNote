//==============================================================================
//　DX9Canvas 
//==============================================================================

#include "stdafx.h"
#include "../../../System/Environment.h"
#include "../../Common/GeometryRenderer.h"
#include "../../Manager.h"
#include "DX9GraphicsDevice.h"
#include "DX9Texture.h"
#include "DX9Canvas.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// DX9Canvas
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9Canvas::DX9Canvas()
		: mGraphicsDevice		( NULL )
		, mBackendBuffer		( NULL )
        , mBackbuffer           ( NULL )
        , mDepthBuffer          ( NULL )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9Canvas::~DX9Canvas()
    {
		_releaseResource();
		LN_SAFE_RELEASE( this->mBackendBuffer );
		LN_SAFE_RELEASE( this->mGraphicsDevice );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9Canvas::create(DX9GraphicsDevice* device, System::Window* window, const LSize& backbufferSize, BackbufferResizeMode resizeMode)
	{
		CanvasBase::create(device, window, backbufferSize, resizeMode);
		LN_REFOBJ_SET( this->mGraphicsDevice, device );

		// 描画先ウィンドウハンドル
		mTargetWindowHandle = System::Environment::getWindowHandle( window );

		// デバイスロスト発生時も getBackbuffer() 等で一応正常なポインタを
		// 返すようにするため、実態は常に持っておく。ラップするテクスチャは復帰時に再取得。
		this->mBackendBuffer = LN_NEW DX9SystemCreatedTexture();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9Canvas::postInitialize()
	{
		_refreshResource();
		activate();		// ※DX9Renderer::resumeDevice() 内のコメント参照
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9Canvas::resize( const LSize& size )
	{
        CanvasBase::resize( size );

		// ソフト的にデバイスロスト状態にして、
		// 次回のロストチェック時にバックバッファを作り直す。
		// (描画スレッドを分けている場合の対応)
		this->mGraphicsDevice->setDeviceLostFlag();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9Canvas::activate()
	{
		IRenderer* r = this->mGraphicsDevice->getRenderer();
		r->setRenderTarget( 0, this->mBackbuffer );
		r->setDepthBuffer( this->mDepthBuffer );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9Canvas::present()
	{
		if ( this->mGraphicsDevice->isDeviceLost() ) {
			return;
		}

		// ここでバックバッファに戻しておく
		IRenderer* r = this->mGraphicsDevice->getRenderer();
		r->setRenderTarget( 0, this->mBackendBuffer );
		r->setDepthBuffer( NULL );

		// バックエンドにコピー
		if (isLetterBox())
		{
			GeometryRenderer* gr = this->mGraphicsDevice->getManager()->getGeometryRenderer();
			const LVector2& size = this->mBackendBuffer->getSize();
			const LVector2& backbufferSize = this->mBackbuffer->getSize();

			// 0.5 オフセット
			LVector2 viewOffset( 0.5 / size.X, 0.5 / size.Y );

			// 単純コピー用のステートにする
			r->setRenderState(LRenderState::BUFFER_COPY_STATE, true);

			// コピー
			r->beginScene();
			gr->setMatrix( mBackbufferOffset );
			gr->setViewSize( size );
			gr->setTexture( this->mBackbuffer );
			gr->begin( GeometryRenderer::PASS_BASIC );
			gr->drawSquare(
				0.0f,				0.0f,				0xffffffff, 0.0f + viewOffset.X, 0.0f + viewOffset.Y, 
				backbufferSize.X,	0.0f,				0xffffffff, 1.0f + viewOffset.X, 0.0f + viewOffset.Y, 
				0.0f,				backbufferSize.Y,	0xffffffff, 0.0f + viewOffset.X, 1.0f + viewOffset.Y, 
				backbufferSize.X,	backbufferSize.Y,	0xffffffff, 1.0f + viewOffset.X, 1.0f + viewOffset.Y );
			gr->end();
			r->endScene();
		}

		//static lnU32 lastTime = 0;
		//lnU32 tt = ::timeGetTime();
		//_p(tt - lastTime);
		//lastTime = tt;

		//::Sleep(16);
		// 転送
		IDirect3DDevice9* dxDevice = this->mGraphicsDevice->getIDirect3DDevice9();
		HRESULT hr = dxDevice->Present( NULL, NULL, mTargetWindowHandle, NULL );

		// デバイスロスト確認
        if ( hr == D3DERR_DEVICELOST )
        {
            hr = dxDevice->TestCooperativeLevel();
		    switch ( hr )
		    {
			    // デバイスロスト
		        case D3DERR_DEVICELOST:
			        //::SleepEx( 1000, true );      
			        break;
			    // デバイスロスト：リセット可能状態
		        case D3DERR_DEVICENOTRESET:
					mGraphicsDevice->setDeviceLostFlag();
			        break;
                default:
					LN_THROW_Com( hr );
					break;
		    }
        }

		//if (mBackbufferResizeMode == BackbufferResizeMode_Resize ||
		//	mBackbufferResizeMode == BackbufferResizeMode_ResizeWithLetterBox)


		{
			// バックバッファの再構築が必要な時はフラグON。
			// 複数スレッドで動作させている時は同期フェーズ等、
			// 然るべき時にデバイスリセットを行う。
			if (mTargetWindow->getSize() != mWindowSize)
			{
				// フルスクリーンへ切り替えるときもここを通る
				mGraphicsDevice->setDeviceLostFlag();
			}
		}
		//else
		//{
		//	if (mTargetWindow->getSize() != mWindowSize)
		//	{
		//		mWindowSize = mTargetWindow->getSize();
		//		updateViewBoxMatrix();
		//	}
		//}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9Canvas::_refreshResource()
	{
		_releaseResource();

		// バックバッファサーフェイスを保持
		HRESULT hr;
		IDirect3DSurface9* surface;
		LN_COMCALL( this->mGraphicsDevice->getIDirect3DDevice9()->GetRenderTarget( 0, &surface ) );
		
		mBackendBuffer->create( NULL, surface );
		SAFE_RELEASE( surface );
		
		LSize size( (int)mBackendBuffer->getSize().X, (int)mBackendBuffer->getSize().Y );


		//if ( isFixedAspect() ) 
		if (isLetterBox())
		{
			//mCurrentBackbufferSize = size;

			// レターボックスを表示する場合はバックバッファとの間にもう一枚作る
			size = mRequestedBackbufferSize;
			mBackbuffer = this->mGraphicsDevice->createRenderTarget(
				size.w, size.h, 1, SurfaceFormat_X8R8G8B8 );
		}
		else 
		{
			// レターボックスを表示しない場合は同じものを指す
			LN_REFOBJ_SET( mBackbuffer, mBackendBuffer );
		}

		// 深度バッファ (バックバッファに対する)
		mDepthBuffer = mGraphicsDevice->createDepthBuffer( 
			size.w, size.h, SurfaceFormat_D24S8 );

		// バックバッファサイズを覚えておく
		const LVector2& backbufferSize = mBackbuffer->getSize();
		mCurrentBackbufferSize.set(backbufferSize.X, backbufferSize.Y);

		// ViewBox 更新
		mWindowSize = mTargetWindow->getSize();
		updateViewBoxMatrix();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9Canvas::_releaseResource()
	{
		LN_SAFE_RELEASE( mDepthBuffer );
		LN_SAFE_RELEASE( mBackbuffer );
		if ( mBackendBuffer ) {
			mBackendBuffer->create( NULL, NULL );
		}
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote
