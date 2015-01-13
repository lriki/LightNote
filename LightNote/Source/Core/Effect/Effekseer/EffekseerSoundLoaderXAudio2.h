//==============================================================================
// EffekseerSoundLoaderXAudio2 
//------------------------------------------------------------------------------
///**
//  @file       EffekseerSoundLoaderXAudio2.h
//  @brief      EffekseerSoundLoaderXAudio2
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <algorithm>
#include <Effekseer/Effekseer.h>
#include <Effekseer/EffekseerSoundXAudio2/EffekseerSoundXAudio2.h>
#include "../../FileIO/BinaryReader.h"
#include "../../FileIO/Manager.h"

namespace LNote
{
namespace Core
{
namespace Effect
{
//==============================================================================
// ■ EffekseerSoundLoaderXAudio2
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class EffekseerSoundLoaderXAudio2
	: public ::Effekseer::SoundLoader
{
public:
	EffekseerSoundLoaderXAudio2( FileIO::Manager* fileManager )
		: mFileIOManager	( fileManager )
	{ }

	virtual ~EffekseerSoundLoaderXAudio2()
	{ }

public:

	void* Load( const EFK_CHAR* path )
	{
		LN_ERR2_ASSERT( path != NULL );
	
		//std::auto_ptr<::Effekseer::FileReader> 
		//	reader( m_fileInterface->OpenRead( path ) );
		//if( reader.get() == NULL ) return false;
		LRefPtr<FileIO::InFile> file( mFileIOManager->createInFile( (const wchar_t*)path ) );
		
		FileIO::BinaryReader reader( file );

		uint32_t chunkIdent, chunkSize;
		// RIFFチャンクをチェック
		reader.read(&chunkIdent, 4);
		reader.read(&chunkSize, 4);
		if (memcmp(&chunkIdent, "RIFF", 4) != 0) {
			return NULL;
		}

		// WAVEシンボルをチェック
		reader.read(&chunkIdent, 4);
		if (memcmp(&chunkIdent, "WAVE", 4) != 0) {
			return NULL;
		}
	
		WAVEFORMATEX wavefmt = {0};
		for (;;) {
			reader.read(&chunkIdent, 4);
			reader.read(&chunkSize, 4);

			if (memcmp(&chunkIdent, "fmt ", 4) == 0) {
				// フォーマットチャンク
				uint32_t size = std::min(chunkSize, sizeof(wavefmt));
				reader.read(&wavefmt, size);
				if (size < chunkSize) {
					reader.seek(reader.getPosition() + chunkSize - size);
				}
			} else if (memcmp(&chunkIdent, "data", 4) == 0) {
				// データチャンク
				break;
			} else {
				// 不明なチャンクはスキップ
				reader.seek(reader.getPosition() + chunkSize);
			}
		}
	
		// フォーマットチェック
		if (wavefmt.wFormatTag != WAVE_FORMAT_PCM || wavefmt.nChannels > 2) {
			return NULL;
		}

		BYTE* buffer;
		uint32_t size;
		switch (wavefmt.wBitsPerSample) {
		case 8:
			// 16bitPCMに変換
			size = chunkSize * 2;
			buffer = new BYTE[size];
			reader.read(&buffer[size / 2], chunkSize);
			{
				int16_t* dst = (int16_t*)&buffer[0];
				uint8_t* src = (uint8_t*)&buffer[size / 2];
				for (uint32_t i = 0; i < chunkSize; i++) {
					*dst++ = (int16_t)(((int32_t)*src++ - 128) << 8);
				}
			}
			break;
		case 16:
			// そのまま読み込み
			buffer = new BYTE[chunkSize];
			size = reader.read(buffer, chunkSize);
			break;
		}

		EffekseerSound::SoundData* soundData = new EffekseerSound::SoundData;
		memset(soundData, 0, sizeof(EffekseerSound::SoundData));
		soundData->channels = wavefmt.nChannels;
		soundData->sampleRate = wavefmt.nSamplesPerSec;
		soundData->buffer.Flags = XAUDIO2_END_OF_STREAM;
		soundData->buffer.AudioBytes = size;
		soundData->buffer.pAudioData = buffer;
	
		return soundData;
	}

	void Unload( void* data )
	{
	}

private:
	FileIO::Manager*	mFileIOManager;
};

} // namespace Effect
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================