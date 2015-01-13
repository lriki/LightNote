//==============================================================================
// ToneImageFilter 
//==============================================================================

#pragma once

#include "stdafx.h"
#include "../../Resource/LNResource.h"
#include "../../Graphics/Common/GeometryRenderer.h"
#include "../../Graphics/Common/GraphicsUtil.h"
#include "../Common.h"
#include "../Manager.h"
#include "../SceneShader/SceneShader.h"
#include "ToneImageFilter.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// ToneImageFilter
//==============================================================================

	LN_TYPE_INFO_ACCESS_IMPL(ToneImageFilter);

	class ToneImageFilter::DrawContext
		: public ImageFilterContext
	{
	public:
		DrawContext( ToneImageFilter* owner )
			: ImageFilterContext	( owner )
			, mRenderTarget			( NULL )
			, mOldRenderTarget		( NULL )
		{
		}

		virtual ~DrawContext()
		{
			LN_SAFE_RELEASE( mRenderTarget );
		}
		
		virtual void updateContext()
		{
			ImageFilterContext::updateContext();
			ToneImageFilter* owner = static_cast<ToneImageFilter*>(mOwner);

			//if (!ttex)
			//ttex = owner->mManager->getGraphicsManager()->getGraphicsDevice()->createTexture( 640, 480, 1 );
			//ttex->clear( LColorF::GREEN );


			mTone = owner->mTone.getValue();
		}

		virtual void preRender( ContextDrawParam* param )
		{
			// 再作成の必要はあるか？
			bool recreate = false;
			if ( mRenderTarget == NULL )
			{
				recreate = true;	// 初回
			}
			else if ( mRenderTarget->getSize() != param->ViewSize )
			{
				recreate = true;	// ViewSize とサイズが違う
			}

			// ViewSize で再作成
			if ( recreate )
			{
				LN_SAFE_RELEASE(mRenderTarget);
				ImageFilter* owner = static_cast<ImageFilter*>(mOwner);
				mRenderTarget = owner->getManager()->getGraphicsManager()->getGraphicsDevice()->createRenderTarget(
					param->ViewSize.x, param->ViewSize.y, 1, Graphics::SurfaceFormat_X8R8G8B8 );
			}

			// レンダーターゲット交換
			mOldRenderTarget = param->Renderer->getRenderTarget( 0 );
			param->Renderer->setRenderTarget( 0, mRenderTarget );
			param->Renderer->clear(true, false, LColorF::Transparency, 1.0f);

		}

		virtual void postRender( ContextDrawParam* param )
		{ 
			ToneImageFilter* owner = static_cast<ToneImageFilter*>(mOwner);
			
			// レンダーターゲットを元に戻す
			param->Renderer->setRenderTarget( 0, mOldRenderTarget );

			owner->mVarTone->setVector( mTone );
			//param->Renderer->clear( true, false, LColorF::GREEN, 1.0 );
			//owner->mVarScreenTexture->setTexture( param->Renderer->getRenderTarget( 0 ) );	// 現在のターゲットをそのまま通す
			//owner->mVarScreenTexture->setTexture( ttex );	// 現在のターゲットをそのまま通す
			owner->mVarScreenTexture->setTexture( mRenderTarget );

			param->Renderer->setRenderState(Graphics::LRenderState::BUFFER_COPY_STATE);
			owner->mShader->getTechniqueByIndex( 0 )->getPassByIndex( 0 )->begin();
			param->GeometryRenderer->drawScreen( param->ViewSize );
			owner->mShader->getTechniqueByIndex( 0 )->getPassByIndex( 0 )->end();
	
		}

	private:
		LVector4				mTone;
		Graphics::Texture*		mRenderTarget;
		Graphics::Texture*		mOldRenderTarget;
		//Graphics::ITexture*	ttex;
	};

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ToneImageFilter::ToneImageFilter()
		: mShader			( NULL )
		, mVarTone			( NULL )
		, mVarScreenTexture	( NULL )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ToneImageFilter::~ToneImageFilter()
	{
		LN_SAFE_RELEASE( mShader );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ToneImageFilter::create( Manager* manager )
	{
		ImageFilter::create( manager, LN_NEW ToneImageFilter::DrawContext( this ) );

		mShader = Graphics::Util::createShader(manager->getGraphicsManager(), Resource::ID_Shader_ImageFilter_Tone, NULL);
		mVarTone = mShader->getVariableByName( _T( "gTone" ) );
		mVarScreenTexture = mShader->getVariableByName( _T( "gScreenTexture" ) );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ToneImageFilter::setTone( const LTone& tone, lnFloat duration )
    {
        mTone.start( mTone.getValue(), (const LVector4&)tone, (lnFloat)duration );
    }

} // namespace Scene
} // namespace Core
} // namespace LNote
