//==============================================================================
// RendererBase 
//==============================================================================

#include "stdafx.h"
#include "RendererBase.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ RendererBase
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    RendererBase::RendererBase()
		: mCurrentDepthBuffer	( NULL )
		, mCurrentVertexBuffer	( NULL )
		, mCurrentIndexBuffer	( NULL )
    {
		memset( mCurrentRenderTargets, 0, sizeof(mCurrentRenderTargets) );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    RendererBase::~RendererBase()
    {
		for ( int i = 0; i < MaxMultiRenderTargets; ++i ) {
			LN_SAFE_RELEASE( mCurrentRenderTargets[i] );
		}
		LN_SAFE_RELEASE( mCurrentDepthBuffer );
		LN_SAFE_RELEASE( mCurrentVertexBuffer );
		LN_SAFE_RELEASE( mCurrentIndexBuffer );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void RendererBase::setRenderTarget( lnU32 index, Texture* texture, bool reset )
	{
		LN_REFOBJ_SET( mCurrentRenderTargets[index], texture );

		// index 0 の場合はビューポートを再設定
		if ( index == 0 )
		{
			LVector2 viewsize = mCurrentRenderTargets[0]->getRealSize();
			setViewport( LRect( 0, 0, (int)viewsize.x, (int)viewsize.y ) );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void RendererBase::pauseDevice()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void RendererBase::resumeDevice()
	{
		setRenderState( mCurrentRenderState, true );

		for ( int i = 0; i < MaxMultiRenderTargets; ++i ) {
			setRenderTarget( i, mCurrentRenderTargets[i], true );
		}

		setDepthBuffer( mCurrentDepthBuffer, true );
		setViewport( mCurrentViewportRect );
		setViewport( mCurrentScissorRect );
		setVertexBuffer( mCurrentVertexBuffer, true );
		setIndexBuffer( mCurrentIndexBuffer, true );
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================