//==============================================================================
// ImageFilter 
//------------------------------------------------------------------------------
/*
	[14/3/1]
		ImageFilter は SceneGraph と Pane にぶら下がるオブジェクト。

		・何故 SceneGraph？
			こちらは基本的に、MME Shader の postprocess で生成したシェーダの自動割当先になる。
			理由は…
			・SceneNode はひとつの SceneGraph の子であり、複数の SceneGraph の子にはならない。
			・ひとつの SceneGraph は、複数の Pane から参照されることがある。
			つまり、Pane にセットしようとしても、本当に意図した Pane なのかわからない。
			例)
				・P1 Pane 生成
				・Mesh からポストエフェクト生成、P1 Pane にセット
				・P2 Pane 生成
				→P2 Pane のポストエフェクトはどこから持って来るの？
*/
//==============================================================================

#pragma once

#include "stdafx.h"
#include "../../Resource/LNResource.h"
#include "../../Graphics/Common/GeometryRenderer.h"
#include "../../Graphics/Common/GraphicsUtil.h"
#include "../Common.h"
#include "../Manager.h"
#include "../SceneShader/SceneShader.h"
#include "ImageFilter.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// ImageFilter
//==============================================================================

	LN_TYPE_INFO_ACCESS_IMPL(ImageFilter);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ImageFilter::ImageFilter()
		: mManager		( NULL )
		, mDrawContext	( NULL )
		, mIsEnableAutoUpdate(true)
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ImageFilter::~ImageFilter()
	{
		LN_SAFE_RELEASE( mDrawContext );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ImageFilter::create( Manager* manager, ImageFilterContext* context )
	{
		mManager = manager;
		mManager->addImageFilter( this );
		mDrawContext = context;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ImageFilter::updateContext()
	{
		mDrawContext->updateContext();
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LayerContextBase* ImageFilter::getContext() 
	{ 
		return mDrawContext;
	}

//==============================================================================
// ImageFilterContext
//==============================================================================

//==============================================================================
// PPShaderImageFilter
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	PPShaderImageFilter::PPShaderImageFilter()
		: mShader	( NULL )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	PPShaderImageFilter::~PPShaderImageFilter()
	{
		LN_SAFE_RELEASE( mShader );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void PPShaderImageFilter::create( SceneShader* postShader )
	{
		LN_REFOBJ_SET( mShader, postShader );
	}

#if 0
//==============================================================================
// ImageFilterRenderer
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ImageFilterRenderer::ImageFilterRenderer()
		: mImageFilterRendererContext	( NULL )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ImageFilterRenderer::~ImageFilterRenderer()
	{
		LN_SAFE_RELEASE( mImageFilterRendererContext );

		ln_foreach( ImageFilterEntry& e, mAttachedImageFilterArray ) {
			e.Filter->release();
		}
		mAttachedImageFilterArray.clear();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ImageFilterRenderer::create()
	{
		mImageFilterRendererContext = LN_NEW ImageFilterRendererContext();
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ImageFilterRenderer::attachImageFilter( ImageFilter* filter, int priority )
	{
		ImageFilterEntry e;
		e.Filter   = filter;
		e.Priority = priority;

		ImageFilterEntryArray::iterator itr =
			std::find(mAttachedImageFilterArray.begin(), mAttachedImageFilterArray.end(), e);
		if (itr != mAttachedImageFilterArray.end())
		{
			// 既に追加されている時は上書き
			*itr = e;
		}
		else
		{
			// 追加されていなければ新しく追加
			mAttachedImageFilterArray.push_back( e );
			filter->addRef();
		}
		
		std::stable_sort(mAttachedImageFilterArray.begin(), mAttachedImageFilterArray.end());
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ImageFilterRenderer::detachImageFilter( ImageFilter* filter )
	{
		ImageFilterEntry e;
		e.Filter   = filter;
		e.Priority = 0;
		if ( Base::STLUtils::remove( mAttachedImageFilterArray, e ) ) {
			LN_SAFE_RELEASE( filter );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ImageFilterRenderer::update( float elapsedTime )
	{
		ln_foreach( ImageFilterEntry& e, mAttachedImageFilterArray )
		{
			if ( e.Filter->isEnableAutoUpdate() ) {
				e.Filter->update( elapsedTime );
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ImageFilterRenderer::updateContext()
	{
		mImageFilterRendererContext->updateContext( this );

		// アタッチされているフィルタ
		ln_foreach( ImageFilterEntry& e, mAttachedImageFilterArray )
		{
			e.Filter->updateContext();
		}
	}

//==============================================================================
// ImageFilterRendererContext
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ImageFilterRendererContext::ImageFilterRendererContext()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ImageFilterRendererContext::~ImageFilterRendererContext()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ImageFilterRendererContext::updateContext( ImageFilterRenderer* owner )
	{
		mImageFilterContextArrayArray.clear();
		ln_foreach( ImageFilterRenderer::ImageFilterEntry& e, owner->mAttachedImageFilterArray ) {
			mImageFilterContextArrayArray.push_back( e.Filter->getDrawContext() );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ImageFilterRendererContext::preRender( ContextDrawParam* param )
	{
		ln_foreach( ImageFilterContext* f, mImageFilterContextArrayArray ) {
			f->preRender( param );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ImageFilterRendererContext::postRender( ContextDrawParam* param )
	{
		ImageFilterContextArray::reverse_iterator itr = mImageFilterContextArrayArray.rbegin();
		ImageFilterContextArray::reverse_iterator end = mImageFilterContextArrayArray.rend();
		for ( ; itr != end; ++itr )
		{
			(*itr)->postRender( param );
		}
		//ln_foreach( ImageFilterContext* f, mImageFilterContextArrayArray ) {
		//	f->postRender( param );
		//}
	}

#endif

} // namespace Scene
} // namespace Core
} // namespace LNote
