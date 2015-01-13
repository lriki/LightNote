//==============================================================================
// AudioSource 
//------------------------------------------------------------------------------
///**
//  @file       AudioSource.h
//  @brief      AudioSource
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <list>
#include "../../../Dependencies/libogg/include/ogg/ogg.h"
#include "../../../Dependencies/libvorbis/include/vorbis/vorbisfile.h"
#include "../Interface.h"
#include "AudioResourceManager.h"

namespace LNote
{
namespace Core
{
namespace Audio
{

//==============================================================================
// ■ AudioSourceBase
//------------------------------------------------------------------------------
///**
//  @brief      音声データを扱うクラスの基底
//*/
//==============================================================================
class AudioSourceBase
    : public Base::ReferenceObject
	, public Base::ICacheObject
{
	LN_CACHE_OBJECT_DECL;
public:
    AudioSourceBase( ResourceManager* manager );
    virtual ~AudioSourceBase();

public:

	/// ファイルフォーマットの取得
	virtual AudioSourceFormat getSourceFormat() const = 0;

	/// PCM フォーマットの取得
	virtual const WaveFormat* getWaveFormat() const = 0;

	/// 元データのサイズの取得 ( ストリーミング再生での終了判定等で使う )
	virtual lnU32 getSourceDataSize() const = 0;

	/// 全体の再生時間の取得 ( ミリ秒。後で無くなるかも )
	virtual lnU32 getTotalTime() const = 0;

	/// 全体のサンプル数の取得 ( Midi の場合はミュージックタイム単位 )
	virtual lnU32 getTotalUnits() const = 0;

	/// オンメモリ再生用のバッファの先頭アドレス取得 ( fillBufferAndReleaseStream() を呼んでいない場合は NULL )
	virtual lnU8* getOnmemoryPCMBuffer() const = 0;

	/// オンメモリ再生時の全バッファサイズの取得
	virtual lnU32 getOnmemoryPCMBufferSize() const = 0;

	/// 1 秒あたりのバイト数取得
	///	
	///	通常は PCM フォーマットから取得できるけど、MP3 の場合は
	///	API の都合(?)上、デコードに最適な 1 秒分のサイズは、
	///	普通のPCMのそれとは異なる。
	///	そのため、ちゃんとチェックできるようにこのメソッドを用意。
	///	いまのところは MP3 に限った話だけど、getWaveFormat() で
	///	取得した値から 1 秒分のサイズを計算するとバグので注意。
	virtual lnU32 getBytesPerSec() const = 0;

	/// ループ開始位置と終了位置の取得
	///
	///	Midi ファイルの場合は最初の CC111 位置のデルタタイムとベースタイム
	virtual void getLoopState( lnU32* begin, lnU32* length ) const = 0;

	/// オーディオファイルとして扱うストリームを設定する
	///
	///	受け取ったストリームは参照カウントがひとつ増え、
	///	インスタンスが解放されるか fillBuffer() が呼ばれるまで保持されます。
    virtual void setStream( FileIO::Stream* stream ) = 0;

	/// オンメモリ再生用に全てのデータを読み込む
	///
	/// デコード後の PCM データサイズ分のバッファを内部で確保し、
	/// そこに全てのデータを読み込みます。
	/// データ及びサイズは getOnmemoryPCMBuffer()、
	/// getOnmemoryPCMBufferSize() で取得してください。<br>
	/// <br>
	/// 複数回呼ばれても、すでにバッファが確保されている場合は
	/// なにもしません。<br>
	/// <br>
	/// 呼出し後、ストリームは解放され、このオーディオソースは
	/// ストリーミング再生には使用できなくなります。<br>
	virtual void fillBufferAndReleaseStream() = 0;

	//----------------------------------------------------------------------
	///**
	//  @brief      データをデコードし、buffer_ に書き込む
	//
	//  @param[out] buffer          : PCM データを書き込むバッファ
	//  @param[in]  buffer_size     : buffer_ のサイズ ( バイト単位 )
	//  @param[out] out_read_size   : ソースデータから読み込んだデータサイズ
	//  @param[out] out_write_size  : 実際に buffer_ に書き込んだサイズ
	//  @par
	//              できるだけ buffer_size を満たすようにデータをデコードし、
	//              buffer 書き込みます。
	//              通常、buffer_size は getBytesPerSec() と同じ値です。<br>
	//              <br>
	//              read_size はデコードの為にソースから読み込んだデータサイズです。
	//              通常、mp3 等の圧縮フォーマットでは write_size よりも小さい値になります。
	//              現在のファイルポインタに read_size の値を足した値が、
	//              次回の読み込み位置となります。<br>
	//              <br>
	//              write_size は、通常は buffer_size と同じ値ですが、
	//              ファイル終端などでは buffer_size よりも小さい値 ( 音声データがあるところまで )
	//              になります。	
	//*/
	//----------------------------------------------------------------------
	virtual void read( void* buffer, lnU32 buffer_size, lnU32* out_read_size, lnU32* out_write_size );

	/// ファイルポインタ移動 (先頭からのバイトオフセット)
	/// (このクラスで実装している read() と seek() は getOnmemoryPCMBuffer() に読み込んでいる事が前提)
	virtual void seek( lnU32 offset );

	/// デコード状態のリセット(再生開始直前に呼ばれる。MP3 用)
	virtual void reset() = 0;

public:

	/// fillBufferAndReleaseStream() スレッドセーフ
    virtual void fillBufferSafe()
    {
		// TODO: fillBufferAndReleaseStream() を直接読んでるところないかチェック
        Threading::ScopedLock lock( mMutex );
        this->fillBufferAndReleaseStream();
    }

	/// seek + reed + スレッドセーフ
    virtual void readSafe( void* buffer, lnU32 buffer_size, lnU32 offset, lnU32* read_size, lnU32* write_size )
    {
        Threading::ScopedLock lock( mMutex );
        this->seek( offset );
        this->read( buffer, buffer_size, read_size, write_size );
    }

public:
	//virtual lnS32 release()		  { return Base::CacheObject::release(); }		// TODO: fillBuffer() された場合にのみ登録するようにする
	//virtual lnS32 Release()		  { return Base::CacheObject::release(); }
	//virtual size_t getCacheMemorySize() const { return 0; }

protected:
    ResourceManager*    mManager;
    Threading::Mutex	mMutex;

private:
	int					mSeekPos;
};

//==============================================================================
// ■ Wave
//------------------------------------------------------------------------------
///**
//  @brief      Wave データを扱うクラス
//*/
//==============================================================================
class Wave
    : public AudioSourceBase
{
public:
	Wave( ResourceManager* manager );
	virtual ~Wave();

public:
	virtual AudioSourceFormat	getSourceFormat() const { return AUDIOSOURCE_WAVE; }
	virtual const WaveFormat*	getWaveFormat() const { return &mWaveFormat; }
	virtual lnU32				getSourceDataSize() const { return mSourceDataSize; }
	virtual lnU32				getTotalTime() const { return mTotalTime; }
    virtual lnU32				getTotalUnits() const { return mTotalSamples; }
	virtual lnU8*				getOnmemoryPCMBuffer() const { return mOnmemoryPCMBuffer; }
	virtual lnU32				getOnmemoryPCMBufferSize() const { return mOnmemoryPCMBufferSize; }
	virtual lnU32				getBytesPerSec() const { return mWaveFormat.AvgBytesPerSec; }
    virtual void				getLoopState( lnU32* begin, lnU32* length ) const { *begin = 0; *length = 0; }
	virtual void				setStream( FileIO::Stream* stream );
	virtual void				fillBufferAndReleaseStream();
	virtual void				read( void* buffer, lnU32 buffer_size, lnU32* read_size, lnU32* write_size );
	virtual void				seek( lnU32 offset );
	virtual void				reset() {}

private:

	/// wave ファイルのヘッダ
	struct WaveFileHeader
	{
		lnU32	RIFF;
		lnU32	Size;
		lnU32	WaveHeader;
	};

private:
    FileIO::Stream*		mInFile;				///< 入力ストリーム
	WaveFormat	        mWaveFormat;			///< PCM 用フォーマット
	lnU32				mSourceDataSize;		///< 音声データ部分のサイズ
	lnU32				mDataOffset;			///< ストリーム内の PCM データの先頭までのオフセットバイト数
	lnU32				mPCMDataSize;			///< ストリーム内の PCM データのサイズ
	lnU32				mTotalTime;				///< 全体の再生時間 ( ミリ秒 )
	lnU8*				mOnmemoryPCMBuffer;		///< オンメモリ再生用のデータを読み込むバッファ
	lnU32				mOnmemoryPCMBufferSize;	///< mOnmemoryPCMBuffer のサイズ ( 今のところ mDataOffset と同じ )
    lnU32				mTotalSamples;          ///< 全体の再生サンプル数
};


#if defined(LNOTE_MSVC)
//==============================================================================
// ■ MP3
//------------------------------------------------------------------------------
///**
//  @brief      MP3 データを扱うクラス
//
//  @note
//              ストリーミング再生時のコンバート情報はこのクラスの内部に
//              持っているため、複数のプレイヤークラスでこのソースを共有して
//              ストリーミング再生することはできない。( 実際はできるけど、
//              音が変な感じに飛ぶ )
//              そのため、このクラスは共有しないようにすること。
//              ( ストリーミング再生が必要なほど長い音声を複数再生することは
//              あまりないだろうし…。 )
//              <br>
//              <br>
//              ちなみにこれを解決するには、オーディオソースからストリーミング再生の情報
//              を取得し、read() を呼ぶときにそれを渡して音声データを取得するようにする。
//              ( これは次バージョンでの実装になるかなぁ… )
//*/
//==============================================================================
class MP3
    : public AudioSourceBase
{
public:
	MP3( ResourceManager* manager );
	virtual ~MP3();

public:
	virtual AudioSourceFormat	getSourceFormat() const { return AUDIOSOURCE_MP3; }
	virtual const WaveFormat*	getWaveFormat() const { return &mWaveFormat; }
	virtual lnU32				getSourceDataSize() const { return mSourceDataSize; }
	virtual lnU32				getTotalTime() const { return mTotalTime; }
    virtual lnU32				getTotalUnits() const { return mTotalSamples; }
	virtual lnU8*				getOnmemoryPCMBuffer() const { return mOnmemoryPCMBuffer; }
	virtual lnU32				getOnmemoryPCMBufferSize() const { return mOnmemoryPCMBufferSize; }
	virtual lnU32				getBytesPerSec() const { return mStreamingPCMBufferSize; }
    virtual void				getLoopState( lnU32* begin, lnU32* length ) const { *begin = 0; *length = 0; }
	virtual void				setStream( FileIO::Stream* stream );
	virtual void				fillBufferAndReleaseStream();
	virtual void				read( void* buffer, lnU32 buffer_size, lnU32* out_read_size, lnU32* out_write_size );
	virtual void				seek( lnU32 offset );
	virtual void				reset() { mResetFlag = true; }

private:

	/// どのフォーマットの mp3 か調べて、データまでのオフセットやタグフィールドのサイズをメンバに格納
	void _checkId3v();

	/// mp3 の PCM フォーマットを調べてメンバに格納する
	void _getPCMFormat();

private:

	/// ID3v2 形式のヘッダ情報
	struct ID3v2Header
	{
		lnU8	ID[ 3 ];
		lnU8	Version[ 2 ];
		lnU8	Flag;
		lnU8	Size[ 4 ];
	};

private:
    FileIO::Stream*			mInFile;					///< 入力ストリーム
	WaveFormat				mWaveFormat;				///< PCM 用フォーマット
	MPEGLAYER3WAVEFORMAT*	mMP3WaveFormat;				///< MPEGLAYER3WAVEFORMAT 構造体へのポインタ
	HACMSTREAM*				mACMStreamHandle;			///< HACMSTREAM 構造体へのポインタ
	lnU32					mSourceDataSize;			///< ソースデータのサイズ ( いまのところ mInFile.size() と同じ )
	lnU32					mDataOffset;				///< ストリーム内の PCM データの先頭までのオフセットバイト数
	lnU32					mID3vTagFieldSize;			///< Id3v2 形式の場合のタグフィールド(ヘッダ情報)部分のサイズ
	lnU8*					mOnmemoryPCMBuffer;			///< オンメモリ再生用のデータを読み込むバッファ
	lnU32					mOnmemoryPCMBufferSize;		///< ( fillBufferAndReleaseStream() で値が設定される )
	lnU32					mStreamingPCMBufferSize;	///< 1 秒分の mp3 データを変換した時の、最適な転送先 PCM バッファサイズ
	lnU8*					mMP3SourceBufferParSec;		///< デコード時にファイルから読む 1 秒分の mp3 データを一時的に格納するバッファ
	lnU32					mMP3SourceBufferSizeParSec;	///< mMP3SourceBufferParSec のサイズ
	lnU32					mTotalTime;					///< 全体の再生時間 ( ミリ秒 )
    lnU32                   mTotalSamples;              ///< 全体のサンプル数
	bool					mResetFlag;					///< デコード状態のリセットを要求するフラグ ( read() でのデコード時のフラグ指定に使う )

	/*
		id3v1 … MP3データはファイルの先頭から格納されている
		id3v2 … タグフィールドの後にMP3データが格納されている
	*/
};

#endif // LNOTE_MSVC

//==============================================================================
// ■ Ogg
//------------------------------------------------------------------------------
/**
//  @brief      Ogg Vorbis 音声データを扱うクラス
//
//  @note
//              1サンプルあたりのビット数は16ビットで固定してある。
//				一応問題ないはずだが、細かいところは未確認。
//*/
//==============================================================================
class Ogg
    : public AudioSourceBase
{
public:
	Ogg( ResourceManager* manager );
	virtual ~Ogg();

public:
	virtual AudioSourceFormat	getSourceFormat() const { return AUDIOSOURCE_OGG; }
	virtual const WaveFormat*	getWaveFormat() const { return &mWaveFormat; }
	virtual lnU32				getSourceDataSize() const { return mSourceDataSize; }
	virtual lnU32				getTotalTime() const { return mTotalTime; }
    virtual lnU32				getTotalUnits() const { return mTotalSamples; }
	virtual lnU8*				getOnmemoryPCMBuffer() const { return mOnmemoryPCMBuffer; }
	virtual lnU32				getOnmemoryPCMBufferSize() const { return mOnmemoryPCMBufferSize; }
	virtual lnU32				getBytesPerSec() const { return mWaveFormat.AvgBytesPerSec; }
    virtual void				getLoopState( lnU32* begin, lnU32* length ) const { *begin = mLoopStart; *length = mLoopLength; }
	virtual void				setStream( FileIO::Stream* stream );
	virtual void				fillBufferAndReleaseStream();
	virtual void				read( void* buffer, lnU32 buffer_size, lnU32* out_read_size, lnU32* out_write_size );
	virtual void				seek( lnU32 offset );
	virtual void				reset() {}

public:	// 以下は ogg API からのコールバックとして登録する関数。そのため public

	/// ogg 用 read コールバック
	static size_t readOggCallback( void* buffer, size_t element_size, size_t count, void* stream );

	/// ogg 用 seek コールバック
	static int seekOggCallback( void* stream, ogg_int64_t offset, int whence );

	/// ogg 用 close コールバック
	static int closeOggCallback( void* stream );

	/// ogg 用 tell コールバック
	static long tellOggCallback( void* stream );

private:
	static const int WORD_SIZE = sizeof( lnU16 );	/// ※1サンプルあたりのビット数
	static const int WORD_BITS = WORD_SIZE * 8;

    FileIO::Stream*		mInFile;				///< 入力ストリーム
	OggVorbis_File		mOggVorbisFile;			///< オープン済みの Ogg ファイル
	WaveFormat	        mWaveFormat;			///< PCM 用フォーマット
	lnU32				mSourceDataSize;		///< 元データのサイズ ( エンコードされた部分 )
	lnU8*				mOnmemoryPCMBuffer;	    ///< オンメモリ再生用のデータを読み込むバッファ
	lnU32				mOnmemoryPCMBufferSize;	///< ( fillBufferAndReleaseStream() で値が設定される )
	lnU32				mTotalTime;				///< 全体の再生時間 ( ミリ秒 )
    lnU32				mTotalSamples;          ///< 全体のサンプル数
    lnU32				mLoopStart;             ///< ループ領域の先頭サンプル数
    lnU32				mLoopLength;            ///< ループ領域の長さ ( サンプル数 )
};
    
#if defined(LNOTE_MSVC)
//==============================================================================
// ■ Midi
//------------------------------------------------------------------------------
///**
//  @brief      Midi データを扱うクラス
//*/
//==============================================================================
class Midi
    : public AudioSourceBase
{
public:
	Midi( ResourceManager* manager );
	virtual ~Midi();
	
public:

	/// メモリ上に展開された MIDI データの取得
	lnByte* getSourceData() const { return mSourceData; }

	/// 音量を正規化して読み込むかの設定 ( fillBufferAndReleaseStream() の前に呼ぶこと )
    void setEnableVolumeNormalize( bool flag ) { mVolumeNormalize = flag; }

public:
	virtual AudioSourceFormat	getSourceFormat() const { return AUDIOSOURCE_MIDI; }
	virtual const WaveFormat*	getWaveFormat() const { return NULL; }
	virtual lnU32				getSourceDataSize() const { return mSourceDataSize; }
	virtual lnU32				getTotalTime() const { return 0; }
    virtual lnU32				getTotalUnits() const { printf( "Midi::getTotalUnits() Undefined." ); return 0; }
	virtual lnU8*				getOnmemoryPCMBuffer() const { return NULL; }
	virtual lnU32				getOnmemoryPCMBufferSize() const { return 0; }
	virtual lnU32				getBytesPerSec() const { return 0; }
    virtual void				getLoopState( lnU32* cc111time, lnU32* base_time ) const { *cc111time = mCC111Time; *base_time = mBaseTime; }
	virtual void				setStream( FileIO::Stream* stream );
	virtual void				fillBufferAndReleaseStream();
	virtual void				read( void* buffer, lnU32 buffer_size, lnU32* out_read_size, lnU32* out_write_size ) { }
	virtual void				seek( lnU32 offset ) { }
	virtual void				reset() {}

private:

	/// midi データの中の必要なデータをチェックする
	void _searchData();

	/// デルタタイムの読み込み
	lnU32 _readDelta();

	/// トラック内の CC111 検索
	bool _searchTrack( lnU32* cc111_time );

private:

	/// Midi ファイルのヘッダ
    struct MidiHeader
    {
	    lnU8      mChunktype[ 4 ];	///< チャンクタイプ ( MThd )
	    lnU32     mLength;			///< データ長
	    lnU16     mFormat;			///< フォーマットタイプ
	    lnU16     mNumtrack;		///< トラック数
	    lnU16     mTimebase;		///< タイムベース
    };

	/// ボリュームチェンジの位置と値 ( 正規化に使う )
    struct VolumeEntry
	{
		lnU32     mPosition;		///< ボリュームチェンジの値の位置 [ 00 B0 07 46 ] の 07
		lnU32     mVolume;			///< ボリューム値
	};

private:
    FileIO::Stream*				mInFile;				///< 入力ストリーム
    lnByte*                     mSourceData;            ///< 元データ ( Midi データ )
	lnU32						mSourceDataSize;		///< 元データのサイズ
    lnU32						mCC111Time;
	lnU32						mBaseTime;
    std::list<VolumeEntry>		mVolumeEntryList;
    bool                        mVolumeNormalize;
};

#endif // LNOTE_MSVC

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================