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
	/// 自動更新の有効設定
	void setEnableAutoUpdate( bool enabled ) { mIsEnableAutoUpdate = enabled; }

	/// 自動更新の有効判別
	bool isEnableAutoUpdate() { return mIsEnableAutoUpdate; }

public:
	// override LayerBase
	virtual void updateContext();
	virtual LayerContextBase* getContext();

public:
	Manager* getManager() { return mManager; }

protected:

	/// 作成
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
	/// 描画情報更新
	virtual void updateContext() {}

	/// 前描画
	virtual void preRender( ContextDrawParam* param ) {}

	/// 後描画
	virtual void postRender( ContextDrawParam* param ) {}

protected:
	//ImageFilter*	mOwner;
};

//==============================================================================
// PPShaderImageFilter
//------------------------------------------------------------------------------
///**
//  @brief      MME の postprocess
//*/
//==============================================================================
class PPShaderImageFilter
    : public ImageFilter
{
public:
	PPShaderImageFilter();
	virtual ~PPShaderImageFilter();

public:
	/// 作成
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
	/// 作成
	void create();
	
	/// フィルタのアタッチ
	void attachImageFilter( ImageFilter* filter, int priority );

	/// フィルタのデタッチ
	void detachImageFilter( ImageFilter* filter );

	/// フレーム更新 (アタッチされているフィルタのうち、自動更新が有効なものはここで更新)
	void update( float elapsedTime );

	/// 描画情報更新
	void updateContext();

	/// 描画情報の取得
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

	/// 描画情報更新
	void updateContext( ImageFilterRenderer* owner );

	/// 前描画
	void preRender( ContextDrawParam* param );

	/// 後描画
	void postRender( ContextDrawParam* param );

protected:
	ImageFilterContextArray	mImageFilterContextArrayArray;
};

#endif

} // namespace Scene
} // namespace Core
} // namespace LNote
