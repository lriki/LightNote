//==============================================================================
// CanvasBase 
//==============================================================================

#include "stdafx.h"
#include "CanvasBase.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ CanvasBase
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    CanvasBase::CanvasBase()
		: mTargetWindow		( NULL )
		, mBackbufferResizeMode(BackbufferResizeMode_Scaling)
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    CanvasBase::~CanvasBase()
    {
		LN_SAFE_RELEASE( this->mTargetWindow );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void CanvasBase::create(GraphicsDeviceBase* device, System::Window* window, const LSize& backbufferSize, BackbufferResizeMode resizeMode)
	{
		LN_REFOBJ_SET( mTargetWindow, window );
		mRequestedBackbufferSize = backbufferSize;
		mBackbufferResizeMode = resizeMode;

		if (mRequestedBackbufferSize.isZero())
			mRequestedBackbufferSize = mTargetWindow->getSize();

		//if ( isFixedAspect() ) {
		//	mCurrentBackbufferSize = backbufferSize;
		//}
		//else {
		//	mCurrentBackbufferSize = mTargetWindow->getSize();
		//}

		//updateWindowSize(mTargetWindow->getSize(), mRequestedBackbufferSize);
		GraphicsResource::registerResource( device );
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void CanvasBase::resize( const LSize& size )
	{
		mRequestedBackbufferSize = size;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void CanvasBase::present()
	{
		//// 転送直前、ウィンドウサイズに変更があれば再計算する
		//if ( mTargetWindow->getSize() != mWindowSize ) {
		//	mWindowSize = mTargetWindow->getSize();
		//	updateViewBoxMatrix();
		//	//this->updateWindowSize(/* this->mTargetWindow->getSize() */);
		//}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void CanvasBase::updateViewBoxMatrix(/*const LSize& newWindowSize, const const LSize& newBackbufferSize*/)
	{
		//if (newWindowSize.w <= 0 || newWindowSize.h <= 0) return;

		//this->mWindowSize = newWindowSize;

		LSize backbufferSize = mWindowSize;
		if (isLetterBox()) {
			backbufferSize = getSize();
		}

		lnFloat sw = static_cast< lnFloat >(backbufferSize.w);   // 転送元
		lnFloat sh = static_cast< lnFloat >(backbufferSize.h);
        lnFloat dw = static_cast< lnFloat >( this->mWindowSize.w );		// 転送先
	    lnFloat dh = static_cast< lnFloat >( this->mWindowSize.h );

		//if ( isFixedAspect() ) {
		//	sw = (lnFloat)mRequestedBackbufferSize.w;
		//	sh = (lnFloat)mRequestedBackbufferSize.h;
		//}

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

		this->mViewDestRect.set( (int)new_x, (int)new_y, (int)new_w, (int)new_h ); 

        this->mScreenTransform.identity();
        this->mScreenTransform.translation( -new_x, -new_y, 0 );
        this->mScreenTransform.scaling( 1.0f / ( new_w / sw ), 1.0f / ( new_h / sh ), 1 );

		this->mBackbufferOffset.identity();
        this->mBackbufferOffset.scaling( new_w / sw, new_h / sh, 1 );
        this->mBackbufferOffset.translation( new_x, new_y, 0 );

	}

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================