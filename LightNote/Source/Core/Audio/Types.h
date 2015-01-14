//==============================================================================
// Types 
//------------------------------------------------------------------------------
///**
//  @file       Types.h
//  @brief      Types
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Math/LMath.h"

namespace LNote
{
namespace Core
{
namespace Audio
{
class ResourceManager;
class AudioSourceBase;
class AudioDeviceBase;
class AudioPlayerBase;
class Sound;
class GameAudio;

class ALAudioDevice;
	
/// 再生方法 ( オーディオプレイヤーの種類 )
enum SoundPlayType
{
	SOUNDPLAYTYPE_UNKNOWN = 0,		///< 不明な再生方法
	SOUNDPLAYTYPE_AUTO,				///< 自動選択 ( デフォルトではデコード後のサイズが 10000 バイト以上になる場合はストリーミング、そうでない場合はオンメモリ再生になります )
	SOUNDPLAYTYPE_ONMEMORY,			///< オンメモリ
	SOUNDPLAYTYPE_STREAMING,	    ///< ストリーミング
	SOUNDPLAYTYPE_MIDI,  			///< SMF

	SOUNDPLAYTYPE_MAX,
};
	
/// 音声の再生状態
enum SoundPlayState
{
    SOUNDPLAYSTATE_STOPPED = 0,		///< 停止中
    SOUNDPLAYSTATE_PLAYING,			///< 再生中
    SOUNDPLAYSTATE_PAUSING,			///< 一時停止中

    SOUNDPLAYSTATE_MAX,
};

/// 音量フェード完了時の動作の記述
enum SoundFadeState
{
    SoundFadeState_NotFading = 0,	///< フェードしていない
    SOUNDFADE_CONTINUE,				///< 継続(なにもしない)
	SOUNDFADE_STOP,					///< 停止する (stop())
	SOUNDFADE_STOP_RESET,			///< 停止して、音量を元の値に戻す
	SOUNDFADE_PAUSE,				///< 一時停止する (pause( true ))
	SOUNDFADE_PAUSE_RESET,			///< 一時停止して、音量を元の値に戻す

	SOUNDFADE_MAX,
};

/// DirectMusic の初期化方法
enum DirectMusicInitMode
{
    DMINITMODE_NOTUSE = 0,			///< DirectMusic を使用しない ( LConfig から設定するために用意 )
	DMINITMODE_NORMAL,				///< 通常
	DMINITMODE_THREAD_WAIT,			///< 別スレッドで初期化して、再生時に未完了の場合は待つ
	DMINITMODE_THREAD_REQUEST,		///< 別スレッドで初期化して、再生時に未完了の場合は再生を予約する

	DMINITMODE_MAX,
};

/// グループの記述
enum AudioPlayerGroupFlag
{
	AUDIOPLAYER_GROUP_BGM		= 0x0002,	///< BGM グループに属するフラグ
	AUDIOPLAYER_GROUP_BGS		= 0x0004,	///< BGS グループに属するフラグ
	AUDIOPLAYER_GROUP_SE		= 0x0008,	///< SE  グループに属するフラグ
    AUDIOPLAYER_GROUP_1			= 0x0010,   ///< グループ 1 に属するフラグ
    AUDIOPLAYER_GROUP_2			= 0x0020,   ///< グループ 2 に属するフラグ
    AUDIOPLAYER_GROUP_3			= 0x0040,   ///< グループ 3 に属するフラグ
    AUDIOPLAYER_GROUP_4			= 0x0080,   ///< グループ 4 に属するフラグ
};

/// 音声ファイルフォーマット
enum AudioSourceFormat
{
	AUDIOSOURCE_UNKNOWN = 0,		///< 不明なファイル
	AUDIOSOURCE_WAVE,				///< WAVE
	AUDIOSOURCE_MP3,				///< MP3
	AUDIOSOURCE_OGG,				///< OGG
	AUDIOSOURCE_MIDI,				///< MIDI

	AUDIOSOURCE_MAX,
};

/// PCM フォーマット
struct WaveFormat
{
    lnU16       FormatTag;
    lnU16       Channels;
    lnU32       SamplesPerSec;
    lnU32       AvgBytesPerSec;
    lnU16       BlockAlign;
    lnU16       BitsPerSample;
    lnU16       EXSize;
};

/// 音声の再生状態
struct AudioPlayState
{
    lnU32				TotalUnits;     ///< 全サンプル数 ( Midi の場合はミュージックタイム単位 ( 四分音符ひとつ分が 768 ) また、Midi の場合は1度でも play() で再生していないと取得できない )
    lnU32				CurrentUnits;   ///< 現在の再生サンプル数
    lnU32               SamplesPerSec;  ///< サンプリング周波数 (MIDI の場合は LN_MUSICTIMEBASE)
    SoundPlayState		PlayingState;
};

/// 3D サウンドリスナー
struct SoundListener
{
    LVector3    Position;
    LVector3    Direction;
    LVector3    UpDirection;
    LVector3    Velocity;

    SoundListener()
        : Direction( 0, 0, 1.0f )
        , UpDirection( 0, 1.0f, 0 )
    {}
};

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================