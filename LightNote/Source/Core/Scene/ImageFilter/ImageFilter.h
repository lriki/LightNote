//==============================================================================
// ImageFilter 
//------------------------------------------------------------------------------
///**
//  @file       ImageFilter.h
//  @brief      ImageFilter
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../Animation/FadeValue.h"
#include "../../Graphics/Interface.h"
#include "../../Graphics/DeviceObjects.h"
#include "../Layer.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// ImageFilter
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class ImageFilter
	: public LayerBase//Base::ReferenceObject
{
	LN_TYPE_INFO_ACCESS_DECL;
public:
	ImageFilter();
	virtual ~ImageFilter();

public:
	/// �����X�V�̗L���ݒ�
	void setEnableAutoUpdate( bool enabled ) { mIsEnableAutoUpdate = enabled; }

	/// �����X�V�̗L������
	bool isEnableAutoUpdate() { return mIsEnableAutoUpdate; }

public:
	// override LayerBase
	virtual void updateContext();
	virtual LayerContextBase* getContext();

public:
	Manager* getManager() { return mManager; }

protected:

	/// �쐬
	void create( Manager* manager, ImageFilterContext* context );

protected:
	Manager*				mManager;
	ImageFilterContext*		mDrawContext;
	volatile bool			mIsEnableAutoUpdate;
};

//==============================================================================
// ImageFilterContext
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class ImageFilterContext
    : public LayerContextBase//Base::ReferenceObject
{
public:
	ImageFilterContext( ImageFilter* owner )
		: LayerContextBase(owner)
	{ /*mOwner = owner;*/ }

	virtual ~ImageFilterContext() {}
	
public:
	/// �`����X�V
	virtual void updateContext() {}

	/// �O�`��
	virtual void preRender( ContextDrawParam* param ) {}

	/// ��`��
	virtual void postRender( ContextDrawParam* param ) {}

protected:
	//ImageFilter*	mOwner;
};

//==============================================================================
// PPShaderImageFilter
//------------------------------------------------------------------------------
///**
//  @brief      MME �� postprocess
//*/
//==============================================================================
class PPShaderImageFilter
    : public ImageFilter
{
public:
	PPShaderImageFilter();
	virtual ~PPShaderImageFilter();

public:
	/// �쐬
	void create( SceneShader* postShader );

public:
	virtual void update( float elapsedTime ) {}
	virtual void updateContext() {}
	
private:
	SceneShader* mShader;
};

#if 0
//==============================================================================
// ImageFilterRenderer
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class ImageFilterRenderer
    : public Base::ReferenceObject
{
public:
	ImageFilterRenderer();
	virtual ~ImageFilterRenderer();

public:
	/// �쐬
	void create();
	
	/// �t�B���^�̃A�^�b�`
	void attachImageFilter( ImageFilter* filter, int priority );

	/// �t�B���^�̃f�^�b�`
	void detachImageFilter( ImageFilter* filter );

	/// �t���[���X�V (�A�^�b�`����Ă���t�B���^�̂����A�����X�V���L���Ȃ��̂͂����ōX�V)
	void update( float elapsedTime );

	/// �`����X�V
	void updateContext();

	/// �`����̎擾
	ImageFilterRendererContext* getDrawContext() { return mImageFilterRendererContext; }

protected:
	struct ImageFilterEntry
	{
		ImageFilter*	Filter;
		int				Priority;

		bool operator ==( const ImageFilterEntry& right ) const {
			return Filter == right.Filter;
		}
		bool operator <( const ImageFilterEntry& right ) const {
			return Priority < right.Priority;
		}
	};

	typedef std::vector<ImageFilterEntry>	ImageFilterEntryArray;

	ImageFilterEntryArray			mAttachedImageFilterArray;
	ImageFilterRendererContext*		mImageFilterRendererContext;

	friend class ImageFilterRendererContext;
};

//==============================================================================
// ImageFilterRendererContext
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class ImageFilterRendererContext
    : public Base::ReferenceObject
{
public:
	ImageFilterRendererContext();
	virtual ~ImageFilterRendererContext();

public:

	/// �`����X�V
	void updateContext( ImageFilterRenderer* owner );

	/// �O�`��
	void preRender( ContextDrawParam* param );

	/// ��`��
	void postRender( ContextDrawParam* param );

protected:
	ImageFilterContextArray	mImageFilterContextArrayArray;
};

#endif

} // namespace Scene
} // namespace Core
} // namespace LNote
