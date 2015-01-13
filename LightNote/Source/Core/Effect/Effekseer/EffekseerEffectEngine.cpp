//==============================================================================
// EffekseerEffectEngine 
//==============================================================================

#include "stdafx.h"
#include "../../Graphics/Device/DirectX9/DX9GraphicsDevice.h"
#include "../../Audio/Device/XAudio2/XA2AudioDevice.h"
#include "../../Base/Unicode.h"
#include "../../Audio/Manager.h"
#include "../../Graphics/Manager.h"
#include "../Manager.h"
#include "EffekseerDriverLNote.h"
#include "EffekseerEffectCore.h"
#include "EffekseerEffectInstance.h"
#include "EffekseerEffectEngine.h"

namespace LNote
{
namespace Core
{
namespace Effect
{
//==============================================================================
// ■ EffekseerEffectEngine
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	EffekseerEffectEngine::EffekseerEffectEngine()
		: mEffekseerManager		( NULL )
		, mEffekseerRenderer	( NULL )
		, mEffekseerSound		( NULL )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	EffekseerEffectEngine::~EffekseerEffectEngine() 
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void EffekseerEffectEngine::initialize( Manager* manager, int cacheSize )
	{
		EffectEngineBase::initialize( manager, cacheSize );

		Graphics::DX9GraphicsDevice* dx9Device = dynamic_cast<Graphics::DX9GraphicsDevice*>( mManager->getGraphicsManager()->getGraphicsDevice() );
		if ( dx9Device )
		{
			// エフェクト管理用インスタンスの生成(自動フリップは行わない)
			mEffekseerManager = ::Effekseer::Manager::Create( 2000, false );

			// 描画用インスタンスの生成
			mEffekseerRenderer = ::EffekseerRendererDX9::Renderer::Create( dx9Device->getIDirect3DDevice9(), 2000 );

			// 描画用インスタンスから描画機能を設定
			mEffekseerManager->SetSpriteRenderer( mEffekseerRenderer->CreateSpriteRenderer() );
			mEffekseerManager->SetRibbonRenderer( mEffekseerRenderer->CreateRibbonRenderer() );
			mEffekseerManager->SetRingRenderer( mEffekseerRenderer->CreateRingRenderer() );
			mEffekseerManager->SetTrackRenderer( mEffekseerRenderer->CreateTrackRenderer() );
			mEffekseerManager->SetModelRenderer( mEffekseerRenderer->CreateModelRenderer() );

			// 描画用インスタンスからテクスチャの読込機能を設定
			mEffekseerManager->SetTextureLoader( LN_NEW EffekseerTextureLoaderDX9( dx9Device, mManager->getFileIOManager() )/*mEffekseerRenderer->CreateTextureLoader()*/ );
			mEffekseerManager->SetModelLoader( mEffekseerRenderer->CreateModelLoader() );
			mEffekseerManager->SetEffectLoader( LN_NEW EffectLoader( mManager->getFileIOManager() ) );
		}
		else {
			LN_THROW_NotImpl(0);
		}

		if ( mEffekseerManager )
		{
			Audio::XAudio2::AudioDevice* xa2Device = dynamic_cast<Audio::XAudio2::AudioDevice*>( mManager->getAudioManager()->getAudioDevice() );

			if ( xa2Device )
			{
				// 音再生用インスタンスの生成
				mEffekseerSound = ::EffekseerSound::Sound::Create( xa2Device->getXAudio2(), 16, 16 );

				// 音再生用インスタンスから再生機能を指定
				mEffekseerManager->SetSoundPlayer( mEffekseerSound->CreateSoundPlayer() );
	
				// 音再生用インスタンスからサウンドデータの読込機能を設定
				mEffekseerManager->SetSoundLoader( LN_NEW EffekseerSoundLoaderXAudio2( mManager->getFileIOManager() )/*mEffekseerSound->CreateSoundLoader()*/ );
			}
			else {
				// サウンドドライバが無いときや NullDevice を使用している時
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void EffekseerEffectEngine::finalize()
	{
		EffectEngineBase::finalize();

		// XAudio2 は finalize() でクリーンアップを行っている。
		// エフェクトがここで開放すると問題あるようであれば、
		// Audio 側も調整する必要がある。

		// 先にエフェクト管理用インスタンスを破棄
		if ( mEffekseerManager ) {
			mEffekseerManager->Destroy();
			mEffekseerManager = NULL;
		}
		if ( mEffekseerRenderer ) {
			mEffekseerRenderer->Destory();
			mEffekseerRenderer = NULL;
		}
		if ( mEffekseerSound ) {
			mEffekseerSound->Destory();
			mEffekseerSound = NULL;
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	EffekseerEffectCore* EffekseerEffectEngine::createEffectCore( const lnChar* filePath )
	{
		// キャッシュ検索
		EffekseerEffectCore* core = static_cast<EffekseerEffectCore*>( findEffectCoreCache( lnSharingKey( filePath ) ) );
		if ( core ) {
			return core;
		}

		// 現状、EFK_CHAR は uint16_t。gcc 環境用では、Effekseer コンパイル時に typedef を変える必要がある
		LN_ERR2_ASSERT( sizeof(EFK_CHAR) == sizeof(wchar_t) );

		// unicode へ
		Base::UnicodeString path;
		path.setDecodeBytes( filePath );

		// エフェクトの読込
		Effekseer::Effect* efkEffect = Effekseer::Effect::Create( 
			mEffekseerManager, 
			(const EFK_CHAR*)path.getString() );
		if ( efkEffect == NULL ) {
			return NULL;
		}

		return LN_NEW EffekseerEffectCore( this, efkEffect );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	EffekseerEffectInstance* EffekseerEffectEngine::createEffect( const lnChar* filePath )
	{
		LRefPtr<EffekseerEffectCore> core( createEffectCore( filePath ) );
		return LN_NEW EffekseerEffectInstance( core ); 
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void EffekseerEffectEngine::setViewProjection( const LMatrix& view, const LMatrix& proj )
	{
		if ( mEffekseerRenderer )
		{
			mEffekseerRenderer->SetCameraMatrix( (const ::Effekseer::Matrix44&)view );
			mEffekseerRenderer->SetProjectionMatrix( (const ::Effekseer::Matrix44&)proj );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void EffekseerEffectEngine::updateRenderContents()
	{
		if ( mEffekseerManager ) {
			mEffekseerManager->Flip();
			//mEffekseerManager->Update();
		}
	}

} // namespace Effect
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================