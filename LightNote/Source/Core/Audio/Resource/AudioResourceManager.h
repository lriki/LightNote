//==============================================================================
// AudioResourceManager 
//------------------------------------------------------------------------------
///**
//  @file       AudioResourceManager.h
//  @brief      AudioResourceManager
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <map>
#include "../../Base/Cache.h"
#include "../../Threading/Mutex.h"
#include "../../FileIO/Interface.h"
#include "../Interface.h"

namespace LNote
{
namespace Core
{
namespace Audio
{

//==============================================================================
// ■ ResourceManager
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class ResourceManager
    : public Base::CacheManager
{
public:

	/// initialize() に渡す初期化データ
	struct ConfigData
	{
        lnU32	AudioSourceCacheSize;
    };

public:
	ResourceManager();
	virtual ~ResourceManager();

public:

	/// 初期化
    void initialize( const ConfigData& configData );

	/// 終了処理
    void finalize();

	/// キーに対応するオーディオソースを検索する (見つかった場合は addRef して返す)
    AudioSourceBase* findAudioSource( lnSharingKey key );

	/// オーディオソースの作成 (findAudioSource() で見つからなかった場合にのみ呼ぶこと)
    AudioSourceBase* createAudioSource( FileIO::Stream* stream, lnSharingKey key );

	/// キャッシュをクリアする
	void clearCache();

private:
    Threading::Mutex	mLock;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================