//==============================================================================
// DX9Renderer 
//==============================================================================

#include "stdafx.h"
#include "DX9GraphicsDevice.h"
#include "DX9VertexBuffer.h"
#include "DX9IndexBuffer.h"
#include "DX9Texture.h"
#include "DX9Renderer.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ DX9Renderer
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9Renderer::DX9Renderer()
		: mGraphicsDevice	( NULL )
		, mDxDevice			( NULL )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9Renderer::~DX9Renderer()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9Renderer::initialize( DX9GraphicsDevice* device )
	{
		mGraphicsDevice = device;
		mDxDevice       = mGraphicsDevice->getIDirect3DDevice9();

        D3DVIEWPORT9 vp;
        mDxDevice->GetViewport( &vp );
        setViewport( LRect( 0, 0, vp.Width, vp.Height ) );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9Renderer::setRenderState( const LRenderState& state, bool reset ) 
	{
		// 合成方法
		if (state.Blend != mCurrentRenderState.Blend || reset)
	    {
		    switch ( state.Blend )
		    {
				case BlendMode_Normal:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  127 );
			        break;
				case BlendMode_Alpha:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  1 );
                    break;
				case BlendMode_Add:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
				case BlendMode_AddAlphaDisable:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
				case BlendMode_Sub:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_REVSUBTRACT );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
				case BlendMode_SubAlphaDisable:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_REVSUBTRACT );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
				case BlendMode_Mul:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
				case BlendMode_Screen:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCALPHASAT );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_INVDESTCOLOR );
					mDxDevice->SetRenderState(D3DRS_ALPHAREF, 255);
					break;
				case BlendMode_Reverse:
					mDxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
					mDxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
					mDxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
					mDxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCCOLOR);
					mDxDevice->SetRenderState(D3DRS_ALPHAREF, 1);
					break;
		    }
	    }

        // カリング
	    if ( state.Culling != mCurrentRenderState.Culling || reset )
	    {
		    const lnU32 tb[] = { D3DCULL_CCW, D3DCULL_CW, D3DCULL_NONE };
		    mDxDevice->SetRenderState( D3DRS_CULLMODE, tb[state.Culling] );
	    }
        // 塗りつぶし方法
		if (state.Fill != mCurrentRenderState.Fill || reset)
	    {
		    const lnU32 tb[] = { D3DFILL_SOLID, D3DFILL_WIREFRAME, D3DFILL_POINT };
			mDxDevice->SetRenderState(D3DRS_FILLMODE, tb[state.Fill]);
	    }
	    // アルファテスト
	    if ( state.AlphaTest != mCurrentRenderState.AlphaTest || reset )
	    {
		    const lnU32 tb[] = { FALSE, TRUE };
		    mDxDevice->SetRenderState( D3DRS_ALPHATESTENABLE, tb[state.AlphaTest] );
	    }
	    // 深度テスト
	    if ( state.DepthTest != mCurrentRenderState.DepthTest || reset )
	    {
		    const lnU32 tb[] = { FALSE, TRUE };
		    mDxDevice->SetRenderState( D3DRS_ZENABLE, tb[state.DepthTest] );
	    }
	    // 深度書き込み
        if ( state.DepthWrite != mCurrentRenderState.DepthWrite || reset )
	    {
		    const lnU32 tb[] = { FALSE, TRUE };
		    mDxDevice->SetRenderState( D3DRS_ZWRITEENABLE, tb[state.DepthWrite] );
	    }
        // ポイントスプライト
        if ( state.PointSprite != mCurrentRenderState.PointSprite || reset )
	    {
		    const lnU32 tb[] = { FALSE, TRUE };
            mDxDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, tb[state.PointSprite] );
	    }

		RendererBase::setRenderState( state, reset );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9Renderer::setRenderTarget( lnU32 index, Texture* texture, bool reset )
	{
		if ( mCurrentRenderTargets[index] != texture || reset )
        {
			if ( texture ) {
				mDxDevice->SetRenderTarget( index, ( (DX9TextureBase*)texture )->getIDirect3DSurface9() );
            }
            else {
                mDxDevice->SetRenderTarget( index, NULL );
            }

			RendererBase::setRenderTarget( index, texture, reset );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9Renderer::setDepthBuffer( Texture* texture, bool reset ) 
	{
		if ( mCurrentDepthBuffer != texture || reset )
        {
			if ( texture ) {
		        mDxDevice->SetDepthStencilSurface( ( (DX9TextureBase*)texture )->getIDirect3DSurface9() );
	        }
	        else {
		        mDxDevice->SetDepthStencilSurface( NULL );
	        }

			RendererBase::setDepthBuffer( texture, reset );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9Renderer::setViewport( const LRect& rect )
	{ 
		RendererBase::setViewport( rect );

        D3DVIEWPORT9 viewport;
	    viewport.X		= static_cast<DWORD>( rect.x );
	    viewport.Y      = static_cast<DWORD>( rect.y );
	    viewport.Width  = static_cast<DWORD>( rect.w );
	    viewport.Height = static_cast<DWORD>( rect.h );
		viewport.MinZ   = 0.0f;
		viewport.MaxZ   = 1.0f;

		HRESULT hr;
		LN_COMCALL( mDxDevice->SetViewport( &viewport ) );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9Renderer::setScissor( const LRect& rect )
	{ 
		// 有効無効の決定
		bool enable;
        if ( rect == mCurrentViewportRect ) {
            enable = false;
        }
        else if ( rect.x == 0 && rect.y == 0 && rect.w == 0 && rect.h == 0 ) {
            enable = false;
        }
        else {
            enable = true;
        }
		HRESULT hr;
		LN_COMCALL( mDxDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, enable ) );

		// 矩形セット
		if ( enable ) {
			RECT rc = {
				rect.x,
				rect.y,
				rect.x + rect.w,
				rect.y + rect.h,
			};
			LN_COMCALL( mDxDevice->SetScissorRect( &rc ) );
		}

		RendererBase::setScissor( rect );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9Renderer::setVertexBuffer( IVertexBuffer* vertexBuffer, bool reset ) 
	{ 
		if ( mCurrentVertexBuffer != vertexBuffer || reset )
        {
			DX9VertexBuffer* vb = static_cast<DX9VertexBuffer*>( vertexBuffer );
			HRESULT hr;
                
			if ( vb ) {
				LN_COMCALL( mDxDevice->SetVertexDeclaration( vb->getDxVertexDeclaration() ) );
				LN_COMCALL( mDxDevice->SetStreamSource( 0, vb->getDxVertexBuffer(), 0, vb->getVertexStride() ) );
			}
			else {
				LN_COMCALL( mDxDevice->SetVertexDeclaration( NULL ) );
				LN_COMCALL( mDxDevice->SetStreamSource( 0, NULL, 0, 0 ) );
			}

			RendererBase::setVertexBuffer( vb, reset );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9Renderer::setIndexBuffer( IIndexBuffer* indexBuffer, bool reset ) 
	{ 
		if ( mCurrentIndexBuffer != indexBuffer || reset )
        {
			DX9IndexBuffer* ib = static_cast<DX9IndexBuffer*>( indexBuffer );
			HRESULT hr;

			LN_COMCALL( mDxDevice->SetIndices( 
				( ib ) ? ib->getDxIndexBuffer() : NULL ) );

			RendererBase::setIndexBuffer( indexBuffer, reset );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9Renderer::clear( bool target, bool depth, const LColorF& color, lnFloat z )
	{
		// ※レンダリングターゲットと深度バッファのサイズが一致している必要がある。
		//   していない場合、エラーとならないがクリアされない。
		LN_ERR2_ASSERT_S(
			mCurrentDepthBuffer == NULL ||
			(mCurrentDepthBuffer != NULL && (mCurrentRenderTargets[0]->getSize() == mCurrentDepthBuffer->getSize())) );

        DWORD flag = 0;
        if ( target ) flag |= D3DCLEAR_TARGET;  
        if ( mCurrentDepthBuffer && depth ) flag |= ( D3DCLEAR_ZBUFFER );// | D3DCLEAR_STENCIL
        if ( flag == 0 ) return;

		HRESULT hr;
		LN_COMCALL( mDxDevice->Clear( 0, NULL, flag, color.to32Bit( LN_GRAPHICSAPI_DIRECTX9 ), z, 0x00000000 ) );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9Renderer::drawPrimitive( PrimitiveType primitive, lnU32 startVertex, lnU32 primitiveCount )
	{
		HRESULT hr;
		D3DPRIMITIVETYPE dx_prim = D3DPT_TRIANGLELIST;
		switch ( primitive )
		{
			//case LN_PRIMITIVE_TRIANGLELIST:   dx_prim = D3DPT_TRIANGLELIST; break;
			case PrimitiveType_TriangleStrip:    
				dx_prim = D3DPT_TRIANGLESTRIP; break;
			case PrimitiveType_LineList:
				dx_prim = D3DPT_LINELIST; break;
			case PrimitiveType_LineStrip:
            	dx_prim = D3DPT_LINESTRIP; break;
			case PrimitiveType_PointList:
				dx_prim = D3DPT_POINTLIST; break;
		}

		// 動的な頂点バッファとして作成している場合は DrawPrimitiveUP で描画する
		if ( mCurrentVertexBuffer->isDynamic() )
		{
            const lnByte* vertices = (const lnByte*)mCurrentVertexBuffer->lock();
            lnU32 stride = mCurrentVertexBuffer->getVertexStride();
            vertices += stride * startVertex;

			LN_COMCALL( mDxDevice->DrawPrimitiveUP(
				dx_prim, primitiveCount, vertices, stride ) );
		}
		else
		{
            LN_COMCALL( mDxDevice->DrawPrimitive(
				dx_prim, startVertex, primitiveCount ) );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9Renderer::drawPrimitiveIndexed( PrimitiveType primitive, lnU32 startIndex, lnU32 primitiveCount )
	{
		HRESULT hr;
		D3DPRIMITIVETYPE dx_prim = D3DPT_TRIANGLELIST;
		switch ( primitive )
		{
			//case LN_PRIMITIVE_TRIANGLELIST:     dx_prim = D3DPT_TRIANGLELIST; break;
			case PrimitiveType_TriangleStrip:    
				dx_prim = D3DPT_TRIANGLESTRIP; break;
			case PrimitiveType_LineList:         
				dx_prim = D3DPT_LINELIST; break;
			case PrimitiveType_LineStrip:
            	dx_prim = D3DPT_LINESTRIP; break;
			case PrimitiveType_PointList:
				dx_prim = D3DPT_POINTLIST; break;
		}

		bool vd = mCurrentVertexBuffer->isDynamic();
		bool id = mCurrentIndexBuffer->isDynamic();

		// 両方が dynamic
		if ( vd && id )
		{
            D3DFORMAT ib_fmt;
            const void* index_data;
            if ( mCurrentIndexBuffer->getIndexStride() == 2 )
            {
                ib_fmt = D3DFMT_INDEX16;
                index_data = &((lnU16*)mCurrentIndexBuffer->lock())[startIndex];
            }
            else
            {
                ib_fmt = D3DFMT_INDEX32;
                index_data = &((lnU32*)mCurrentIndexBuffer->lock())[startIndex];
            }

            LN_COMCALL( 
                mDxDevice->DrawIndexedPrimitiveUP(
				    dx_prim,
				    0,
				    mCurrentVertexBuffer->getVertexCount(),
				    primitiveCount,
				    index_data,
				    ib_fmt,
				    mCurrentVertexBuffer->lock(),
				    mCurrentVertexBuffer->getVertexStride() ) );
		}
        // 両方が static
		else if ( !vd && !id )
		{
            LN_COMCALL(
			    mDxDevice->DrawIndexedPrimitive( 
				    dx_prim,
				    0,
				    0,
				    mCurrentVertexBuffer->getVertexCount(),
				    startIndex,
				    primitiveCount ) );
		}
		else {
			LN_ERR2_ASSERT_S( 0 );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9Renderer::beginScene()
	{
		HRESULT hr;
		LN_COMCALL( mDxDevice->BeginScene() );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9Renderer::endScene()
	{
		HRESULT hr;
		LN_COMCALL( mDxDevice->EndScene() );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9Renderer::pauseDevice()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9Renderer::resumeDevice()
	{
		HRESULT hr;

		//-----------------------------------------------------
		// 動作中変更のないステートの設定

        LN_COMCALL( mDxDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE ) );

		// アルファテストの時の基準アルファ値 ( Clearで塗りつぶす色に透明色が使えなく云々 )
		//hr += mDxDevice->SetRenderState( D3DRS_ALPHAREF, 0x00000001 );
		// アルファテスト時の比較関数の指定 ( D3DCMPFUNC 列挙型  デフォルトは D3DCMP_ALWAYS )
		LN_COMCALL( mDxDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL ) );
		// アルファブレンド有効
		//hr += mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		// 法線の正規化を設定する ( ちょっと時間かかるっぽい )
		//hr += mDxDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );

        /*
		// カリングモードの設定
		hr += mDxDevice->SetRenderState( D3DRS_CULLMODE, mCurrentCullMode );
		// Zバッファリングの有効設定
		hr += mDxDevice->SetRenderState( D3DRS_ZENABLE, mCurrentDepthTest );

		// Zバッファへの書き込み許可設定
		hr += mDxDevice->SetRenderState( D3DRS_ZWRITEENABLE, mCurrentDepthWrite );
		// アルファテストの有効設定
		hr += mDxDevice->SetRenderState( D3DRS_ALPHATESTENABLE, mCurrentAlphaTest );
		// テクスチャ縮小の補正
		hr += mDxDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, mCurrentMinFilter );
		// テクスチャ拡大の補正
		hr += mDxDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, mCurrentMagFilter );
		// ミップフィルタ
		hr += mDxDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, mCurrentMipFilter );
		// 異方性
		hr += mDxDevice->SetSamplerState( 0, D3DSAMP_MAXANISOTROPY, mMaxAnisotropy );
        */
		
		// テクスチャブレンディングの設定
		// テクスチャと
		LN_COMCALL( mDxDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE ) );
		// 頂点色の
		LN_COMCALL( mDxDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE ) );
		// 色を乗算
		LN_COMCALL( mDxDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE ) );
		// テクスチャと
		LN_COMCALL( mDxDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE ) );
		// 頂点色の
		LN_COMCALL( mDxDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE ) );
		// アルファ値を乗算
		LN_COMCALL( mDxDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE ) );

		//-----------------------------------------------------
		// デバイス側がリセットされているので再設定するもの

		// ステート
		setRenderState( mCurrentRenderState, true );

		// ターゲット
		for ( int i = 0 ; i < MaxMultiRenderTargets; ++i ) {
			// DX9の仕様で、[0] は NULL 不可。このif は、initialize() から呼ばれるときのフィルタ。
			// initialize() の時は[0]には触らないようにしておいて、Canvas::postInitialize() から
			// デフォルトバックバッファを [0] に設定するようにする。
			if ( i != 0 || mCurrentRenderTargets[i] != NULL ) {
				setRenderTarget( i, mCurrentRenderTargets[i], true );
			}
		}

		// 深度バッファ
		setDepthBuffer( mCurrentDepthBuffer, true );
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================