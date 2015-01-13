//==============================================================================
// AudioUtil 
//==============================================================================

#include "stdafx.h"
#include "Resource/AudioSource.h"
#include "AudioUtil.h"

namespace LNote
{
namespace Core
{
namespace Audio
{

//==============================================================================
// ■ AudioUtil
//==============================================================================

#if defined(LNOTE_WIN32)

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void AudioUtil::convLNWaveFormatToWAVEFORMATEX( const WaveFormat& lnFmt, WAVEFORMATEX* wavFmt )
    {
        wavFmt->wFormatTag       = lnFmt.FormatTag;
        wavFmt->nChannels        = lnFmt.Channels;
        wavFmt->nSamplesPerSec   = lnFmt.SamplesPerSec;
        wavFmt->nAvgBytesPerSec  = lnFmt.AvgBytesPerSec;
        wavFmt->nBlockAlign      = lnFmt.BlockAlign;
        wavFmt->wBitsPerSample   = lnFmt.BitsPerSample;
        wavFmt->cbSize           = lnFmt.EXSize;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void AudioUtil::convWAVEFORMATEXToLNWaveFormat( const WAVEFORMATEX& wavFmt, WaveFormat* lnFmt )
    {
        lnFmt->FormatTag      = wavFmt.wFormatTag;
        lnFmt->Channels       = wavFmt.nChannels;
        lnFmt->SamplesPerSec  = wavFmt.nSamplesPerSec;
        lnFmt->AvgBytesPerSec = wavFmt.nAvgBytesPerSec;
        lnFmt->BlockAlign     = wavFmt.nBlockAlign;
        lnFmt->BitsPerSample  = wavFmt.wBitsPerSample;
        lnFmt->EXSize         = wavFmt.cbSize;
    }   

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void AudioUtil::dumpWAVEFORMATEX( const WAVEFORMATEX& wavFmt, const char* str )
    {
        if ( str )
		{
			printf( "%s\n", str );
		}
		else
		{
            printf( "AudioUtil::dumpWAVEFORMATEX\n" );
		}
		printf( "    wFormatTag                %hu\n", wavFmt.wFormatTag );
		printf( "    nChannels                 %hu\n", wavFmt.nChannels );
		printf( "    nSamplesPerSec            %lu\n", wavFmt.nSamplesPerSec );
		printf( "    nAvgBytesPerSec(Byte/sec) %lu\n", wavFmt.nAvgBytesPerSec );
		printf( "    nBlockAlign               %hu\n", wavFmt.nBlockAlign );
		printf( "    wBitsPerSample            %hu\n", wavFmt.wBitsPerSample );
		printf( "    cbSize                    %hu\n", wavFmt.cbSize );
    }

#endif

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    AudioSourceFormat AudioUtil::checkFormat( FileIO::Stream* stream )
	{
        if ( !stream || stream->getSize() < 4 ) return AUDIOSOURCE_UNKNOWN;

        AudioSourceFormat format = AUDIOSOURCE_UNKNOWN;

		lnU8 head[ 4 ];
		stream->read( head, 4, 4 );
		if ( memcmp( head, "RIFF", 4 ) == 0 )
		{
			stream->seek( 4, SEEK_CUR );
			lnU8 wave[ 4 ];
			stream->read( wave, 4, 4 );
			if ( memcmp( wave, "WAVE", 4 ) == 0 )
			{
				format = AUDIOSOURCE_WAVE;
			}
		}
		else if ( memcmp( head, "OggS", 4 ) == 0 )
		{
			format = AUDIOSOURCE_OGG;
		}
		else if ( memcmp( head, "MThd", 4 ) == 0 )
		{
			format = AUDIOSOURCE_MIDI;
		}
		else if ( memcmp( head, "ID3", 3 ) == 0 )
		{
			format = AUDIOSOURCE_MP3;
		}

        if ( format == AUDIOSOURCE_UNKNOWN )
        {
		    // mp3 の場合はヘッダが ID3 だったり後ろの方に TAG があったり
		    // 同期ヘッダだけあったりするので全部チェックする
		    if ( head[ 0 ] == 0xff && head[ 1 ] >> 5 == 0x07 )
		    {
			    format = AUDIOSOURCE_MP3;
		    }
            else
            {
		        stream->seek( -128, SEEK_END );
		        char data[ 3 ];
		        stream->read( data, 3 );
		        if ( data[ 0 ] == 'T' && data[ 1 ] == 'A' && data[ 2 ] == 'G' )
		        {
			        format = AUDIOSOURCE_MP3;
		        }
            }
        }
    		
		stream->seek( 0 );
		return format;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    SoundPlayType AudioUtil::checkAudioPlayType( SoundPlayType type, AudioSourceBase* audio_source, lnU32 limit_size )
    {
		// 作成するオーディオプレイヤーの種類を決めていく
		if ( audio_source->getSourceFormat() == AUDIOSOURCE_MIDI )
		{
			type = SOUNDPLAYTYPE_MIDI;
		}
		else
		{
			if ( type == SOUNDPLAYTYPE_AUTO )
			{
				if ( audio_source->getOnmemoryPCMBufferSize() > limit_size )
				{
					type = SOUNDPLAYTYPE_STREAMING;
				}
				else
				{
					type = SOUNDPLAYTYPE_ONMEMORY;
				}
			}
		}

        return type;
    }

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================