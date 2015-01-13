//==============================================================================
// GraphicsDeviceBase 
//------------------------------------------------------------------------------
///**
//  @file       GraphicsDeviceBase.h
//  @brief      GraphicsDeviceBase
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <list>
#include "../../Base/Misc.h"
#include "../../Base/Cache.h"
#include "../Interface.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{


/**
	@brief
*/
class GraphicsDevice
	: public Base::ReferenceObject
{
};











//==============================================================================
// ■ GraphicsDeviceBase
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class GraphicsDeviceBase
    : /*public Base::ReferenceObject
    , */public IGraphicsDevice
{
public:

	struct ConfigData
	{
        Graphics::Manager*  Manager;
        System::Manager*    SystemManager;
        FileIO::Manager*	FileManager;
        lnU32               TextureCacheSize;
        lnU32               ShaderCacheSize;
    };

public:
	GraphicsDeviceBase();
	virtual ~GraphicsDeviceBase();
    LN_REFOBJ_METHODS;

public:

	/// 初期化
	void initialize( const ConfigData& configData );

	

	FileIO::Manager* getIOManager() { return mFileManager; }

#ifdef LNOTE_FOR_200
	
	Manager* getManager() { return mManager; }

	/// テクスチャキャッシュを検索 (見つかれば addRef())
	Base::ICacheObject* findTextureCache( const lnSharingKey& key );

	/// シェーダキャッシュを検索 (見つかれば addRef())
	Base::ICacheObject* findShaderCache( const lnSharingKey& key );

	/// リソース登録
	void addGraphicsResource( GraphicsResource* resource ) { mGraphicsResourceList.push_back( resource ); }

	/// リソース除外
	void removeGraphicsResource( GraphicsResource* resource ) { mGraphicsResourceList.remove( resource ); }

public:
	// IGraphicsDevice
	//virtual IShader* createShader( const void* data, lnU32 size, lnSharingKey registerKey ) = 0;
	//virtual IShader* createShader( const lnChar* filePath, lnSharingKey registerKey );/// テクスチャキャッシュの有効設定
	virtual void setEnableTextureCache( bool enabled ) { mEnableTextureCache = enabled; }
	virtual void setEnableShaderCache( bool enabled ) { mEnableShaderCache = enabled; }
	virtual void clearTextureCache();
	virtual void clearShaderCache();
    virtual void pauseDevice();
    virtual void resumeDevice();
	virtual void testDeviceLost(){};

public:
	/// 終了処理
    virtual void finalize();
	//virtual IShader* createShader( const void* data, lnU32 size, lnSharingKey registerKey ) = 0;

#else
public:
	virtual ITexture* findTextureCache( const lnSharingKey& key );
	virtual void clearTextureCache();
	virtual IShader* createShader( const lnChar* filePath, lnSharingKey registerKey );
    virtual IShader* createShader( const void* data, lnU32 size, lnSharingKey registerKey ) = 0;
	virtual IShader* findShaderCache( const lnSharingKey& key );
	virtual void clearShaderCache();
#endif

protected:

	typedef std::list<GraphicsResource*> GraphicsResourceList;
	
    Graphics::Manager*		mManager;
    System::Manager*		mSystemManager;
    FileIO::Manager*		mFileManager;
    Base::TempBuffer		mTempBuffer;

	Base::CacheManager		mTextureCache;
	Base::CacheManager		mShaderCache;

	GraphicsResourceList	mGraphicsResourceList;

	bool					mEnableTextureCache;
	bool					mEnableShaderCache;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================