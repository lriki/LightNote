//==============================================================================
// ImageFilter 
//------------------------------------------------------------------------------
/*
	[14/3/1]
		ImageFilter �� SceneGraph �� Pane �ɂԂ牺����I�u�W�F�N�g�B

		�E���� SceneGraph�H
			������͊�{�I�ɁAMME Shader �� postprocess �Ő��������V�F�[�_�̎���������ɂȂ�B
			���R�́c
			�ESceneNode �͂ЂƂ� SceneGraph �̎q�ł���A������ SceneGraph �̎q�ɂ͂Ȃ�Ȃ��B
			�E�ЂƂ� SceneGraph �́A������ Pane ����Q�Ƃ���邱�Ƃ�����B
			�܂�APane �ɃZ�b�g���悤�Ƃ��Ă��A�{���ɈӐ}���� Pane �Ȃ̂��킩��Ȃ��B
			��)
				�EP1 Pane ����
				�EMesh ����|�X�g�G�t�F�N�g�����AP1 Pane �ɃZ�b�g
				�EP2 Pane ����
				��P2 Pane �̃|�X�g�G�t�F�N�g�͂ǂ����玝���ė���́H
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
			// ���ɒǉ�����Ă��鎞�͏㏑��
			*itr = e;
		}
		else
		{
			// �ǉ�����Ă��Ȃ���ΐV�����ǉ�
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

		// �A�^�b�`����Ă���t�B���^
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
