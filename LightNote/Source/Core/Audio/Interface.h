//==============================================================================
// Audio 
//------------------------------------------------------------------------------
///**
//  @file       Audio.h
//  @brief      Audio
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Base/SharingKey.h"
#include "../FileIO/Interface.h"
#include "Types.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Audio
{
class Sound;
class GameAudio;


#if 0
//==============================================================================
// ■ IManager クラス
//------------------------------------------------------------------------------
///**
//  @brief      オーディオ機能の管理インターフェイス
//*/
//==============================================================================
class IManager
    : public Base::Interface
{
public:

	/// GameAudio クラスの取得
    virtual GameAudio* getGameAudio() = 0;

	/// デバイスクラスの取得
    virtual AudioDeviceBase* getAudioDevice() = 0;

	/// リソース管理クラスの取得
    virtual ResourceManager* getResourceManager() = 0;

	/// Sound の作成 (stream_ = NULL でキーを使った検索だけ行う)
    virtual LNRESULT createSound( Sound** sound_, FileIO::IInStream* stream_, SoundPlayType type_, bool enable_3d_, Base::SharingKey key_ ) = 0;

	/// Sound の作成 (ファイル名指定)
    virtual LNRESULT createSound( Sound** sound_, const lnChar* filename_, SoundPlayType type_, bool enable_3d_ ) = 0;

	/// Sound の作成 (IAudioSource 指定)
    virtual LNRESULT createSound( Sound** sound_, AudioSourceBase* source_, SoundPlayType type_, bool enable_3d_ ) = 0;

	/// グループの停止
    virtual void stopGroup( lnU32 group_ ) = 0;

protected:

    virtual ~IManager() {}
};
#endif

#if 0
//==============================================================================
// ■ IAudioDevice クラス
//------------------------------------------------------------------------------
///**
//  @brief      オーディオの管理クラスのインターフェース
//*/
//==============================================================================
class IAudioDevice
    : public Base::Interface
{
public:

	/// IAudioPlayer を作成する (type_ に LN_SOUNDPLAYTYPE_AUTO は指定できないので注意)
    virtual LNRESULT createAudioPlayer( AudioPlayerBase** obj_, AudioSourceBase* source_, bool enable_3d_, SoundPlayType type_ ) = 0;

	/// 3Dサウンドリスナーの取得
    virtual LN3DSoundListener* get3DSoundListener() = 0;

	/// 更新 (更新スレッドから呼ばれる)
    virtual void update() = 0;

	/// 3D 空間の1メートル相当の距離の設定
    virtual void setMetreUnitDistance( lnFloat d ) = 0;

protected:

    virtual ~IAudioDevice() {};

};
#endif
#if 0
//==============================================================================
// ■ IAudioPlayer クラス
//------------------------------------------------------------------------------
///**
//  @brief      音声再生クラスのインターフェース
//*/
//==============================================================================
class IAudioPlayer
    : public Base::Interface
{
public:

	/// IAudioSource の取得
	virtual AudioSourceBase* getAudioSource() const = 0;

	/// 音量の取得
	virtual int getVolume() const = 0;

	/// 音量の設定
	virtual LNRESULT setVolume( int volume_ ) = 0;

	/// ピッチの取得
	virtual int getPitch() const = 0;

	/// ピッチの設定
	virtual LNRESULT setPitch( int pitch_ ) = 0;

	/// 再生したサンプル数の取得 ( Midi の場合はミュージックタイム )
    virtual lnU64 getPlayedSamples() const = 0;

	/// 再生中判定 (削除予定)
	virtual bool isPlaying() const = 0;

	// 一時停止中判定 (削除予定)
	virtual bool isPausing() const = 0;

	/// 再生状態の取得
    virtual SoundPlayState getPlayState() const = 0;

	//----------------------------------------------------------------------
	///**
	//  @brief      ループ再生の有効設定
	//
	//  @param[in]  loop_ : ループ再生する場合、true を設定する
	//*/
	//----------------------------------------------------------------------
	virtual void loop( bool loop_ ) = 0;

	/// ループ再生が有効かを判定する
	virtual bool isLoop() const = 0;

	//----------------------------------------------------------------------
	///**
	//  @brief      ループ情報の設定
	//
	//  @param[in]  loop_begin_  : ループされる領域の最初のサンプル
	//  @param[in]  loop_length_ : ループ領域の長さ (サンプル数単位)
	//   
	//  @par
	//              全体をループ再生する場合は loop_begin_ と loop_length_ に
	//              0 を設定してください。
	//*/
	//----------------------------------------------------------------------
	virtual LNRESULT setLoopState( lnU32 loop_begin_, lnU32 loop_length_ ) = 0;

	//----------------------------------------------------------------------
	///**
	//  @brief		再生
	//
	//  @note
	//              ・先頭から開始			<br>
	//              ・ポーズ解除			<br>
	//              ・フェード状態初期化	<br>
	//*/
	//----------------------------------------------------------------------
	virtual LNRESULT play() = 0;

	//----------------------------------------------------------------------
	///**
	//  @brief      停止
	// 
	//  @note
	//              ・ポーズ解除
	//*/
	//----------------------------------------------------------------------
	virtual LNRESULT stop() = 0;

	//----------------------------------------------------------------------
	///**
	//  @brief		一時停止
	//*/
	//----------------------------------------------------------------------
	virtual LNRESULT pause( bool is_pause_ ) = 0;

	//----------------------------------------------------------------------
	///**
	//  @brief      ポーリング更新
	// 
	//  @par
	//              再生終了(停止)している場合は true を返します。
	//              ループ再生の場合は常に true を返します。
	//*/
	//----------------------------------------------------------------------
	virtual bool polling() = 0;

	//----------------------------------------------------------------------
	///**
	//  @brief		3D 音源かを判定する
	//*/
	//----------------------------------------------------------------------
    virtual bool is3DSound() = 0;

	//----------------------------------------------------------------------
	///**
	//  @brief		3D 音源としての位置を設定する
	//*/
	//----------------------------------------------------------------------
    virtual LNRESULT setPosition( const LVector3& pos_ ) = 0;

	//----------------------------------------------------------------------
	///**
	//  @brief		3D 音源としての位置を取得する
	//*/
	//----------------------------------------------------------------------
    virtual const LVector3& getPosition() = 0;

	/// 速度の設定
    virtual void setVelocity( const LVector3& v ) = 0;

	/// 3D 音源の影響範囲 (音の届く距離) の設定
    virtual void setEmitterDistance( lnFloat distance_ ) = 0;

	/// 3D 音源の影響範囲 (音の届く距離) の取得
    virtual lnFloat getEmitterDistance() const = 0;

public:

	virtual ~IAudioPlayer() {}
};
#endif
#if 0
//==============================================================================
// ■ IAudioSource クラス
//------------------------------------------------------------------------------
///**
//  @brief      音声データのインターフェイス
//*/
//==============================================================================
class IAudioSource
    : public Base::Interface
{
public:

	/// ファイルフォーマットの取得
	virtual AudioSourceFormat getSourceFormat() const = 0;

	/// PCM フォーマットの取得
	virtual const WaveFormat* getWaveFormat() const = 0;

	/// 元データのサイズの取得 ( ストリーミング再生での終了判定等で使う )
	virtual lnU32 getSourceDataSize() const = 0;

	/// 全体の再生時間の取得 ( ミリ秒 後で無くなるかも )
	virtual lnU32 getTotalTime() const = 0;

	/// 全体のサンプル数の取得 ( Midi の場合はミュージックタイム単位 )
	virtual lnU32 getTotalUnits() const = 0;

	/// オンメモリ再生用のバッファの先頭アドレス取得 ( fillBufferAndReleaseStream() を呼んでいない場合は NULL )
	virtual lnU8* getOnmemoryPCMBuffer() const = 0;

	/// オンメモリ再生時の全バッファサイズの取得
	virtual lnU32 getOnmemoryPCMBufferSize() const = 0;

	//----------------------------------------------------------------------
	///**
	//	@brief      1 秒あたりのバイト数取得
	//
	//	@note
	//              通常は PCM フォーマットから取得できるけど、MP3 の場合は
	//              API の都合(?)上、デコードに最適な 1 秒分のサイズは、
	//              普通のPCMのそれとは異なる。
	//              そのため、ちゃんとチェックできるようにこのメソッドを用意。
	//              いまのところは MP3 に限った話だけど、getWaveFormat() で
	//              取得した値から 1 秒分のサイズを計算するとバグので注意。
	//*/
	//----------------------------------------------------------------------
	virtual lnU32 getBytesPerSec() const = 0;

	//----------------------------------------------------------------------
	///**
	//  @brief      ループ開始位置と終了位置の取得
	//
	//  @par
	//              Midi ファイルの場合は最初の CC111 位置のデルタタイムとベースタイム
	//*/
	//----------------------------------------------------------------------
	virtual void getLoopState( lnU32* begin_, lnU32* length_ ) const = 0;

	//----------------------------------------------------------------------
	///**
	//  @brief      オーディオファイルとして扱うストリームを設定する
	//
	//  @par
	//              受け取ったストリームは参照カウントがひとつ増え、
	//              インスタンスが解放されるか fillBuffer() が呼ばれるまで保持されます。
	//              (解放されたときに参照カウントをデクリメント)
	//*/
	//----------------------------------------------------------------------
    virtual LNRESULT setStream( FileIO::IInStream* stream_ ) = 0;

	//----------------------------------------------------------------------
	///**
	//  @brief      オンメモリ再生用に全てのデータを読み込む
	//
	//  @par
	//              デコード後の PCM データサイズ分のバッファを内部で確保し、
	//              そこに全てのデータを読み込みます。
	//              データ及びサイズは getOnmemoryPCMBuffer()、
	//              getOnmemoryPCMBufferSize() で取得してください。<br>
	//              <br>
	//              複数回呼ばれても、すでにバッファが確保されている場合は
	//              なにもしません。<br>
	//              <br>
	//              呼出し後、ストリームは解放され、このオーディオソースは
	//              ストリーミング再生には使用できなくなります。<br>
	//*/
	//----------------------------------------------------------------------
	virtual LNRESULT fillBufferAndReleaseStream() = 0;

	/// fillBufferAndReleaseStream() スレッドセーフ
    virtual void fillBufferSafe() = 0;

	//----------------------------------------------------------------------
	///**
	//  @brief      データをデコードし、buffer_ に書き込む
	//
	//  @param[out] buffer_         : PCM データを書き込むバッファ
	//  @param[in]  buffer_size_    : buffer_ のサイズ ( バイト単位 )
	//  @param[out] read_size_      : ソースデータから読み込んだデータサイズ
	//  @param[out] write_size_     : 実際に buffer_ に書き込んだサイズ
	//
	//  @par
	//              できるだけ buffer_size_ を満たすようにデータをデコードし、
	//              buffer_ 書き込みます。
	//              通常、buffer_size_ は getBytesPerSec() と同じ値です。<br>
	//              <br>
	//              read_size_ はデコードの為にソースから読み込んだデータサイズです。
	//              通常、mp3 等の圧縮フォーマットでは write_size_ よりも小さい値になります。
	//              現在のファイルポインタに read_size_ の値を足した値が、
	//              次回の読み込み位置となります。<br>
	//              <br>
	//              write_size_ は、通常は buffer_size_ と同じ値ですが、
	//              ファイル終端などでは buffer_size_ よりも小さい値 ( 音声データがあるところまで )
	//              になります。	
	//*/
	//----------------------------------------------------------------------
	virtual LNRESULT read( void* buffer_, lnU32 buffer_size_, lnU32* read_size_, lnU32* write_size_ ) = 0;

	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルポインタ移動
	//
	//  @param[in]  offset_ : 移動量 ( バイト単位 )
	//
	//  @par
	//              ソースデータの先頭から offset_ 分移動したところに設定します。
	//*/
	//----------------------------------------------------------------------
	virtual LNRESULT seek( lnU32 offset_ ) = 0;

	/// seek + reed + スレッドセーフ
    virtual void readSafe( void* buffer_, lnU32 buffer_size_, lnU32 offset_, lnU32* read_size_, lnU32* write_size_ ) = 0;


	//----------------------------------------------------------------------
	///**
	//  @brief      デコード状態のリセット
	//
	//  @note
	//              新しく再生を開始する前に呼ぶこと。
	//              mp3 のソースクラスで実装される。
	//*/
	//----------------------------------------------------------------------
	virtual void reset() = 0;

protected:

	virtual ~IAudioSource() {}
};
#endif
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================