//==============================================================================
// AudioUtil 
//------------------------------------------------------------------------------
///**
//  @file       AudioUtil.h
//  @brief      AudioUtil
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "Interface.h"

namespace LNote
{
namespace Core
{
namespace Audio
{

//==============================================================================
// ■ AudioUtil
//------------------------------------------------------------------------------
///**
//  @brief      音声ユーティリティ
//*/
//==============================================================================
class AudioUtil
{
public:

#if defined(LNOTE_WIN32)

	//----------------------------------------------------------------------
	///**
	//  @brief      LNWaveFormat から WAVEFORMATEX への変換
	//*/
	//----------------------------------------------------------------------
    static void convLNWaveFormatToWAVEFORMATEX( const WaveFormat& lnFmt, WAVEFORMATEX* wavFmt );

	//----------------------------------------------------------------------
	///**
	//  @brief      WAVEFORMATEX から LNWaveFormat への変換
	//*/
	//----------------------------------------------------------------------
    static void convWAVEFORMATEXToLNWaveFormat( const WAVEFORMATEX& wavFmt, WaveFormat* lnFmt );

	//----------------------------------------------------------------------
	///**
	//  @brief      WAVEFORMATEX 構造体をコンソール画面に表示
	//
	//  @param[in]  wavFmt : WAVEFORMATEX
	//	@param[in]	str    : ヘッダ文字列
	//*/
	//----------------------------------------------------------------------
    static void dumpWAVEFORMATEX( const WAVEFORMATEX& wavFmt, const char* str = NULL );

#endif

	//----------------------------------------------------------------------
	///**
	//  @brief      フォーマットチェック
	//
	//  @param[in]  stream_ : 入力ストリーム
	//
	//  @par
	//              扱える音声ファイルかどうかを調べて、その種類を返します。<br>
	//              調べた後、ファイルポインタはファイルの先頭に戻ります。
	//*/
	//----------------------------------------------------------------------
    static AudioSourceFormat checkFormat( FileIO::Stream* stream );

	//----------------------------------------------------------------------
	///**
	//  @brief      要求している type と IAudioSouce から正しい type を返す
	//
	//  @param[in]  type       : 要求再生方法
	//  @param[in]  source     : チェックに使うオーディオソース
	//  @param[in]  limit_size : オンメモリとストリーミングの境界サイズ
	//
	//  @par
	//              LN_SOUNDPLAYTYPE_AUTO が要求された場合、オーディオソースのサイズが
	//              limit_size のサイズを超えると LN_SOUNDPLAYTYPE_STREAMING を返します。
	//*/
	//----------------------------------------------------------------------
    static SoundPlayType checkAudioPlayType( SoundPlayType type, AudioSourceBase* audio_source, lnU32 limit_size );
};

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================