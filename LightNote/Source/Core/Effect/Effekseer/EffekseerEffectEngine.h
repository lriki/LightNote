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
// �� EffekseerEffectEngine
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

	/// ������
	void initialize( Manager* manager, int cacheSize );

	/// EffectCore �̍쐬 (�L���b�V�������L��)
	///		�G�t�F�N�g�����^�C�����t�@�C���t�H�[�}�b�g�G���[�ƂȂ����ꍇ�͗�O�ɂ����ANULL ��Ԃ��B
	///		(���������t�@�C����������Ȃ��ꍇ�͗�O�ƂȂ�)
	EffekseerEffectCore* createEffectCore( const lnChar* filePath );

	/// Effect �̍쐬
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