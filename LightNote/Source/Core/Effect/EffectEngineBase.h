//==============================================================================
// EffectEngineBase 
//------------------------------------------------------------------------------
///**
//  @file       EffectEngineBase.h
//  @brief      EffectEngineBase
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "Common.h"
#include "../Math/LMath.h"
#include "../Base/Cache.h"

namespace LNote
{

namespace Core
{
namespace Effect
{
//==============================================================================
// ■ EffectEngineBase
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class EffectEngineBase
    : public Base::ReferenceObject
{
public:
	EffectEngineBase();
	virtual ~EffectEngineBase();

public:

	/// 初期化
	void initialize( Manager* manager, int cacheSize );

	/// 終了処理
	virtual void finalize();

	/// Manager 取得
	Manager* getManager() { return mManager; }

	/// EffectCore 検索
	Base::ICacheObject* findEffectCoreCache( const lnSharingKey& key );

	/// キャッシュクリア
	void clearEffectCoreCache();

	/// ビュー・プロジェクション行列の設定
	virtual void setViewProjection( const LMatrix& view, const LMatrix& proj ) = 0;

	/// 描画情報の更新
	virtual void updateRenderContents() = 0;

protected:
	Manager*				mManager;
	Base::CacheManager		mEffectCoreCache;
};

} // namespace Effect
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================