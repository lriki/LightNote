//==============================================================================
// MMETextureObject 
//==============================================================================

#include "stdafx.h"
#include "SceneShaderUtil.h"
#include "SceneShaderManager.h"
#include "SceneShader.h"
#include "MMETextureObject.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// ■ MMETextureObject
//==============================================================================
    
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	MMETextureObject::MMETextureObject( SceneShaderManager* manager )
		: mManager		( manager )
		, mTexture		( NULL )
		, mMiplevels	( 0 )
		, mFormat		( Graphics::SurfaceFormat_Unknown )
		, mAntiAlias	( false )
	{
		mManager->addMMETextureObject( this );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	MMETextureObject::~MMETextureObject()
	{
		_dispose();
		mManager->removeMMETextureObject( this );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void MMETextureObject::initialize(
		SceneShader*		ownerShader,
		MMEVariableRequest	request,
        const lnRefString&  resourceName,
        const LVector3&     dimensions,
        const LVector2&     viewportRatio,
        int                 miplevels,
        Graphics::SurfaceFormat     format,
        bool                antiAlias )
	{
		mOwnerShader = ownerShader;
		mRequest = request;
		mResourceName = resourceName;
		mDimensions = dimensions;
		mViewportRatio = viewportRatio;
		mMiplevels = miplevels;
		mFormat = format;
		mAntiAlias = antiAlias;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void MMETextureObject::refresh( const LVector2& viewSize )
	{
		if ( mTexture == NULL ||								// 初回生成
			(mViewportRatio.x != 0 || mViewportRatio.y != 0) )	// ビューサイズによってテクスチャサイズも変える必要がある
		{
			_dispose();

			// 作成するべきテクスチャのサイズを調べる
			int width, height;
			SceneShaderUtil::checkTextureSize(
				&width,
				&height,
				mDimensions,
				mViewportRatio, 
				viewSize );

			FileIO::PathNameW resourceName(mResourceName.c_str());
			FileIO::PathNameW path(mOwnerShader->getFilePath().getParent(), resourceName);

			// テクスチャ作成
			mManager->createTexture(
				&mTexture,
				mRequest,
				path,
				width,
				height,
				static_cast< lnU32 >( mDimensions.z ),
				mMiplevels,
				mFormat );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool MMETextureObject::equalTextureVariable( const MMETextureObject* textureObject )
	{
		return (
			this->mVariableName == textureObject->mVariableName &&
			this->mVariableSemantics == textureObject->mVariableSemantics );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void MMETextureObject::_dispose()
	{
		LN_SAFE_RELEASE( mTexture );
	}

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================