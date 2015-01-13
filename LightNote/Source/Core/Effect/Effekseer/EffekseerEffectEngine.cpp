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
// �� EffekseerEffectEngine
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
			// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���(�����t���b�v�͍s��Ȃ�)
			mEffekseerManager = ::Effekseer::Manager::Create( 2000, false );

			// �`��p�C���X�^���X�̐���
			mEffekseerRenderer = ::EffekseerRendererDX9::Renderer::Create( dx9Device->getIDirect3DDevice9(), 2000 );

			// �`��p�C���X�^���X����`��@�\��ݒ�
			mEffekseerManager->SetSpriteRenderer( mEffekseerRenderer->CreateSpriteRenderer() );
			mEffekseerManager->SetRibbonRenderer( mEffekseerRenderer->CreateRibbonRenderer() );
			mEffekseerManager->SetRingRenderer( mEffekseerRenderer->CreateRingRenderer() );
			mEffekseerManager->SetTrackRenderer( mEffekseerRenderer->CreateTrackRenderer() );
			mEffekseerManager->SetModelRenderer( mEffekseerRenderer->CreateModelRenderer() );

			// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
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
				// ���Đ��p�C���X�^���X�̐���
				mEffekseerSound = ::EffekseerSound::Sound::Create( xa2Device->getXAudio2(), 16, 16 );

				// ���Đ��p�C���X�^���X����Đ��@�\���w��
				mEffekseerManager->SetSoundPlayer( mEffekseerSound->CreateSoundPlayer() );
	
				// ���Đ��p�C���X�^���X����T�E���h�f�[�^�̓Ǎ��@�\��ݒ�
				mEffekseerManager->SetSoundLoader( LN_NEW EffekseerSoundLoaderXAudio2( mManager->getFileIOManager() )/*mEffekseerSound->CreateSoundLoader()*/ );
			}
			else {
				// �T�E���h�h���C�o�������Ƃ��� NullDevice ���g�p���Ă��鎞
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void EffekseerEffectEngine::finalize()
	{
		EffectEngineBase::finalize();

		// XAudio2 �� finalize() �ŃN���[���A�b�v���s���Ă���B
		// �G�t�F�N�g�������ŊJ������Ɩ�肠��悤�ł���΁A
		// Audio ������������K�v������B

		// ��ɃG�t�F�N�g�Ǘ��p�C���X�^���X��j��
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
		// �L���b�V������
		EffekseerEffectCore* core = static_cast<EffekseerEffectCore*>( findEffectCoreCache( lnSharingKey( filePath ) ) );
		if ( core ) {
			return core;
		}

		// ����AEFK_CHAR �� uint16_t�Bgcc ���p�ł́AEffekseer �R���p�C������ typedef ��ς���K�v������
		LN_ERR2_ASSERT( sizeof(EFK_CHAR) == sizeof(wchar_t) );

		// unicode ��
		Base::UnicodeString path;
		path.setDecodeBytes( filePath );

		// �G�t�F�N�g�̓Ǎ�
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