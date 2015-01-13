//==============================================================================
// EffekseerEffectEngine 
//------------------------------------------------------------------------------
///**
//  @file       EffekseerEffectEngine.h
//  @brief      EffekseerEffectEngine
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <Effekseer/Effekseer.h>
#ifdef LNOTE_DIRECTX
#include <Effekseer/EffekseerSoundXAudio2/EffekseerSoundXAudio2.h>
#include <Effekseer/EffekseerRendererDX9/EffekseerRendererDX9.h>
#include "EffekseerTextureLoaderDX9.h"
#include "EffekseerSoundLoaderXAudio2.h"
#endif
#include "../EffectEngineBase.h"

namespace LNote
{
namespace Core
{
namespace Effect
{
//==============================================================================
// ■ EffekseerEffectEngine
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class EffekseerEffectEngine
    : public EffectEngineBase
{
public:
	EffekseerEffectEngine();
	virtual ~EffekseerEffectEngine();

public:

	/// 初期化
	void initialize( Manager* manager, int cacheSize );

	/// EffectCore の作成 (キャッシュ検索有効)
	///		エフェクトランタイムがファイルフォーマットエラーとなった場合は例外にせず、NULL を返す。
	///		(そもそもファイルが見つからない場合は例外となる)
	EffekseerEffectCore* createEffectCore( const lnChar* filePath );

	/// Effect の作成
	EffekseerEffectInstance* createEffect( const lnChar* filePath );

public:
	virtual void finalize();
	virtual void setViewProjection( const LMatrix& view, const LMatrix& proj );
	virtual void updateRenderContents();

public:
	::Effekseer::Manager* getEffekseerManager() { return mEffekseerManager; }
	::EffekseerRenderer::Renderer* getEffekseerRenderer() { return mEffekseerRenderer; }

protected:
	::Effekseer::Manager*			mEffekseerManager;
	::EffekseerRenderer::Renderer*	mEffekseerRenderer;
	::EffekseerSound::Sound*		mEffekseerSound;
};

} // namespace Effect
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================