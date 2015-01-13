//=============================================================================
//【 Canvas 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "GraphicsDevice.h"
#include "Canvas.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
namespace DirectX9
{

//=============================================================================
// ■ CanvasBase クラス
//=============================================================================

	//---------------------------------------------------------------------
	// ● コンストラクタ
	//---------------------------------------------------------------------
    CanvasBase::CanvasBase( GraphicsDevice* device_ )
    {
        mGraphicsDevice = device_;
        mGraphicsDevice->addCanvas( this );
        LN_SAFE_ADDREF( mGraphicsDevice );
    }

	//---------------------------------------------------------------------
	// ● デストラクタ
	//---------------------------------------------------------------------
    CanvasBase::~CanvasBase()
    {
        if ( mGraphicsDevice )
        {
            mGraphicsDevice->removeCanvas( this );
            LN_SAFE_RELEASE( mGraphicsDevice );
        }
    }

//=============================================================================
// ■ DefaultCanvas
//=============================================================================

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    DefaultCanvas::DefaultCanvas( GraphicsDevice* device )
        : CanvasBase            ( device )
        , mDxDevice             ( NULL )
		, mTargetWindow			( NULL )
		, mDefaultBackbuffer	( NULL )
        , mBackbuffer           ( NULL )
        , mDepthBuffer          ( NULL )
        , mBackgroundColorCode  ( 0 )
		, mTargetWindowHandle	( NULL )
		, mFixedBackbufferSize	( false )
		, mModified				( false )
    {
        
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    DefaultCanvas::~DefaultCanvas()
    {
		_releaseResource();
        LN_SAFE_RELEASE( mBackbuffer );
        LN_SAFE_RELEASE( mDepthBuffer );

		if ( mTargetWindow ) {
			mTargetWindow->detachEventListener( this );
			LN_SAFE_RELEASE( mTargetWindow );
		}
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    void DefaultCanvas::initialize( System::IWindow* window, bool fixedBackbufferSize )
    {
		// この時点ではまだ DirectX9Device は初期化されていない。
		// ここで設定したバックバッファサイズを元に、初期化・リセットされる。

		LN_REFOBJ_SET( mTargetWindow, window );
		mTargetWindow->attachEventListener( this, 0 );

		mBackbufferSize			= mTargetWindow->getSize();
		mFixedBackbufferSize	= fixedBackbufferSize;
		mDefaultBackbuffer		= LN_NEW SystemCreatedTexture();
	
        setWindowSize( mBackbufferSize );
		mModified = false;
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void DefaultCanvas::postInitialize()
	{
		// この関数はデバイスが初期化された直後に呼ばれる

        mDxDevice = mGraphicsDevice->getDX9Device();
		_refreshResource();
	}

	//---------------------------------------------------------------------
	// ● 仮想スクリーンサイズを変更する
	//---------------------------------------------------------------------
    LNRESULT DefaultCanvas::resize( lnU32 width_, lnU32 height_ )
    {
        mBackbufferSize.set( width_, height_ );

		// ソフト的にデバイスロスト状態にして、
		// 次回のロストチェック時にバックバッファを作り直す
        mGraphicsDevice->postDeviceLostEvent();

        setWindowSize( mBackbufferSize );

		mModified = true;
        return LN_OK;
    }

	//---------------------------------------------------------------------
	// ● バックバッファのレンダリングターゲット、深度バッファを設定する
	//---------------------------------------------------------------------
    LNRESULT DefaultCanvas::activate()
    {
        LNRESULT lr;

        LN_CALL_R( mGraphicsDevice->getRenderer()->setRenderTarget( 0, mBackbuffer, true ) );
        LN_CALL_R( mGraphicsDevice->getRenderer()->setDepthBuffer( mDepthBuffer ) );

        return LN_OK;
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    LNRESULT DefaultCanvas::present()
    {
        if ( mGraphicsDevice->isDeviceLost() ) return LN_OK;

        LNRESULT lr;

        // ここでRT をバックバッファに戻しておく
        LN_CALL_R( activate() );

		HRESULT hr;


		//::InvalidateRect( mTargetWindow->getWindowHandle(), NULL, TRUE );
		//if ( mFixedBackbufferSize ) {
			hr = mDxDevice->Present( NULL, &mViewDestRect, mTargetWindowHandle, NULL );
		//}
		//else {
		//	hr = mDxDevice->Present( NULL, NULL, mTargetWindowHandle, NULL );
		//}

        // デバイスロスト状態のときは復帰処理
        if ( hr == D3DERR_DEVICELOST )
        {
            hr = mDxDevice->TestCooperativeLevel();

		    switch ( hr )
		    {
			    // デバイスロスト
		        case D3DERR_DEVICELOST:
			        //::SleepEx( 1000, true );      
			        break;
			    // デバイスロスト：リセット可能状態
		        case D3DERR_DEVICENOTRESET:
                    mGraphicsDevice->postDeviceLostEvent();
					mModified = true;
			        break;

                default:
                    return LN_ERR_FATAL;
		    }
        }
        return LN_OK;
    }

	//---------------------------------------------------------------------
	// ● 描画先のウィンドウサイズを設定する
	//---------------------------------------------------------------------
    void DefaultCanvas::setWindowSize( const Geometry::Size& size )
    { 
        if ( size.w <= 0 || size.h <= 0 ) return;

		mWindowSize = size;

        lnFloat sw = static_cast< lnFloat >( mBackbufferSize.w );   // 転送元
	    lnFloat sh = static_cast< lnFloat >( mBackbufferSize.h );
        lnFloat dw = static_cast< lnFloat >( mWindowSize.w );		// 転送先
	    lnFloat dh = static_cast< lnFloat >( mWindowSize.h );

	    lnFloat new_x, new_y;
	    lnFloat new_w, new_h;

        lnFloat ratio_w;
        lnFloat ratio_h;

	    // バックバッファサイズとスクリーンサイズが同じ場合
	    if ( sw == dw && sh == dh )
	    {
		    // そのまま設定
		    new_x = 0;
		    new_y = 0;
		    new_w = sw;
		    new_h = sh;
            ratio_w = 1.0f;
            ratio_h = 1.0f;
	    }
	    else
	    {
		    // 現在のスクリーンサイズ(デフォルトビューポートのサイズ)と画面サイズの比率計算
		    ratio_w = dw / sw;
		    ratio_h = dh / sh;

		    // 縦方向に合わせる ( 左右が余る )
		    if ( ratio_w > ratio_h )
		    {
			    new_w = static_cast< lnFloat >( sw * ratio_h );
			    new_h = static_cast< lnFloat >( dh );
			    new_x = static_cast< lnFloat >( ( dw / 2 ) - ( new_w / 2 ) );
			    new_y = 0;
		    }
		    //横方向にあわせる
		    else
		    {
			    new_w = static_cast< lnFloat >( dw );
			    new_h = static_cast< lnFloat >( sh * ratio_w );
			    new_x = 0;
			    new_y = static_cast< lnFloat >( ( dh / 2 ) - ( new_h / 2 ) );
		    }
	    }

        ::SetRect(
            &mViewDestRect,
            static_cast< int >( new_x ),
            static_cast< int >( new_y ),
            static_cast< int >( new_x + new_w ),
            static_cast< int >( new_y + new_h ) );

        mScreenTransformMatrix.identity();
        mScreenTransformMatrix.translation( -new_x, -new_y, 0 );
        mScreenTransformMatrix.scaling( 1.0f / ( new_w / sw ), 1.0f / ( new_h / sh ), 1 );
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    LNRESULT DefaultCanvas::onLostDevice()
    {
        _releaseResource();

		return LN_OK;
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    LNRESULT DefaultCanvas::onResetDevice()
    {
        _refreshResource();

        return LN_OK;
    }
#define SHER 0
	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void DefaultCanvas::_refreshResource()
	{
		_releaseResource();

		// バックバッファサーフェイスを保持
		HRESULT hr;
		IDirect3DSurface9* surface;
		LN_COMCALL( mDxDevice->GetRenderTarget( 0, &surface ) );
		mDefaultBackbuffer->setTextureAndSurface( NULL, surface );
		SAFE_RELEASE( surface );

		// バックバッファ固定の場合 (ウィンドウリサイズ時、レターボックス表示)
		if ( mFixedBackbufferSize )
		{
#if SHER
			mGraphicsDevice->createRenderTarget( 
				&mBackbuffer, mBackbufferSize.w, mBackbufferSize.h, 1, LN_FMT_D24S8 );
#else
			mBackbuffer = mDefaultBackbuffer;
#endif
			
		}
		// ウィンドウリサイズ時、バックバッファごと変更
		else
		{
			mBackbuffer = mDefaultBackbuffer;
		}

		// 深度バッファは自分で作成
		mGraphicsDevice->createDepthBuffer( 
			&mDepthBuffer, mBackbufferSize.w, mBackbufferSize.h, LN_FMT_D24S8 );
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void DefaultCanvas::_releaseResource()
	{
		LN_SAFE_RELEASE( mDepthBuffer );
		mDefaultBackbuffer->setTextureAndSurface( NULL, NULL );

		if ( mFixedBackbufferSize )
		{
#if SHER
			LN_SAFE_RELEASE( mBackbuffer );
#endif
		}
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	bool DefaultCanvas::onEvent( const System::EventArgs& e )
	{
#if SHER
		if ( e.Type == LN_EVENT_WINDOW_SIZE_CHANGED ) {
			const LSize& size = mTargetWindow->getSize();
			resize( size.w, size.h );
		}
#endif
		return false;
	}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace DirectX9
} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//
//=============================================================================