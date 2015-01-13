//==============================================================================
// DirectMusic 
//==============================================================================

#include "stdafx.h"
#include <algorithm>
#include <cguid.h>
#include "DirectMusic.h"

#define LN_OUT_LOG  // 念のためログを出しておく

namespace LNote
{
namespace Core
{
namespace Audio
{
namespace DirectMusic
{

//==============================================================================
// ■ DirectMusicSegment
//==============================================================================

#define LN_DMUS_TYPE DMUS_APATH_DYNAMIC_STEREO  // DMUS_APATH_SHARED_STEREOPLUSREVERB にするとデフォでリバーブ付くけど、ピッチ変更ができなくなるみたい

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DirectMusicSegment::DirectMusicSegment( IDirectMusicPerformance8* dmPerformance, const Midi* audioSource )
        : mDMPerformance    ( dmPerformance )
        , mDMAudioPath      ( NULL )
        , mDMSegment        ( NULL )
        , mDMSegmentState   ( NULL )
        , mDSSoundBuffer    ( NULL )
    {
        HRESULT hr = S_OK;

        if ( mDMPerformance )
        {
            // DirectSound のバッファを取得するため、オーディオパスを取得する
		    hr = mDMPerformance->CreateStandardAudioPath(
			     LN_DMUS_TYPE,
			     64, FALSE, &mDMAudioPath );
            
            // DirectSound バッファの取得
		    hr = mDMAudioPath->GetObjectInPath( 
                DMUS_PCHANNEL_ALL,
                DMUS_PATH_BUFFER,
                0, GUID_NULL, 0,
                IID_IDirectSoundBuffer8, 
                (void**)&mDSSoundBuffer );


            // バッファにエフェクトを設定する
		    //		GUID_DSFX_STANDARD_CHORUS
		    //		GUID_DSFX_STANDARD_COMPRESSOR
		    //		GUID_DSFX_STANDARD_DISTORTION
		    //		GUID_DSFX_STANDARD_ECHO エコーありすぎ
		    //		GUID_DSFX_STANDARD_FLANGER
		    //		GUID_DSFX_STANDARD_GARGLE ぶるぶる
		    //		GUID_DSFX_STANDARD_I3DL2REVERB 音ちいさい
		    //		GUID_DSFX_STANDARD_PARAMEQ
		    //		GUID_DSFX_WAVES_REVERB
		    DSEFFECTDESC ds_desc;
		    ZeroMemory( &ds_desc, sizeof( DSEFFECTDESC ) );
		    ds_desc.dwSize        = sizeof( DSEFFECTDESC );
		    ds_desc.dwFlags       = 0;//DSFX_LOCSOFTWARE;
		    ds_desc.guidDSFXClass = GUID_DSFX_WAVES_REVERB;  // エフェクト識別子 ( ウェーブリバーブ )
		    ds_desc.dwReserved1   = 0;	// 予約済み
		    ds_desc.dwReserved2   = 0;	// 予約済み
		    hr = mDSSoundBuffer->SetFX( 1, &ds_desc, NULL );
		    

            // リバーブのパラメータを設定するためのインターフェースを取得する
		    // ( 普通は必要ないけど、できるだけ RGSS に近付けたいので )
		    IDirectSoundFXWavesReverb8* waves_reverb = NULL;
		    hr = mDSSoundBuffer->GetObjectInPath( GUID_All_Objects, 0, IID_IDirectSoundFXWavesReverb8, (void**)&waves_reverb );
		    
		    // パラメータを設定して バッファにセット
		    DSFXWavesReverb rv;
		    rv.fInGain			= 0.f;//DSFX_WAVESREVERB_INGAIN_MIN ;//DSFX_WAVESREVERB_INGAIN_DEFAULT;
		    rv.fReverbMix		= 0.f;//DSFX_WAVESREVERB_REVERBMIX_MIN  DSFX_WAVESREVERB_REVERBMIX_DEFAULT;
		    rv.fReverbTime		= 600.f;//DSFX_WAVESREVERB_REVERBTIME_DEFAULT;DSFX_WAVESREVERB_REVERBTIME_MIN 
		    rv.fHighFreqRTRatio = DSFX_WAVESREVERB_HIGHFREQRTRATIO_DEFAULT;//DSFX_WAVESREVERB_HIGHFREQRTRATIO_MIN; //DSFX_WAVESREVERB_HIGHFREQRTRATIO_DEFAULT;DSFX_WAVESREVERB_HIGHFREQRTRATIO_MAX
		    hr = waves_reverb->SetAllParameters( &rv );
		    

            IDirectMusicLoader8* loader = NULL;

		    //ローダーの作成
		    HRESULT hr = CoCreateInstance( CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC, IID_IDirectMusicLoader8, (void**)&loader );
    		
		    // キャッシュ無効 ( 有効にする場合、http://www.nomuraz.com/denpa/prog001.htm#IDPROG0009 のバグに注意 )
		    hr = loader->EnableCache( GUID_DirectMusicAllTypes, FALSE );

		    // セグメント作成のための情報を設定する
		    DMUS_OBJECTDESC desc;
		    ZeroMemory( &desc, sizeof( DMUS_OBJECTDESC ) );
		    desc.dwSize      = sizeof( DMUS_OBJECTDESC );
		    desc.dwValidData = DMUS_OBJ_MEMORY | DMUS_OBJ_CLASS;
		    desc.guidClass   = CLSID_DirectMusicSegment;
		    desc.llMemLength = (LONGLONG)audioSource->getSourceDataSize();	// バッファのサイズ
		    desc.pbMemData   = (LPBYTE)audioSource->getSourceData();	    // データの入っているバッファ

		    // セグメント作成
		    hr = loader->GetObject( &desc, IID_IDirectMusicSegment8, (void**)&mDMSegment );

            // SetParam で GUID_StandardMIDIFile を設定する前に呼ばないと
            // 第 1 トラックが再生されなくなる
            hr = mDMSegment->Download( mDMPerformance );

		    // SMF として再生することを伝える
		    hr = mDMSegment->SetParam( GUID_StandardMIDIFile, 0xFFFFFFFF, 0, 0, NULL );
    	
            // ローダーはもう使わないので解放
		    SAFE_RELEASE( loader );
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DirectMusicSegment::~DirectMusicSegment()
    {
        stop();
        
        if ( mDMSegment )
		{
            mDMSegment->Unload( mDMPerformance );
		}
		if ( mDMPerformance )
		{
			mDMPerformance->CloseDown();
		}

        SAFE_RELEASE( mDMSegmentState );
        SAFE_RELEASE( mDMSegment );
		
        SAFE_RELEASE( mDSSoundBuffer );
        SAFE_RELEASE( mDMAudioPath );
        SAFE_RELEASE( mDMPerformance );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DirectMusicSegment::setVolume( float volume )
    {
		// 10db ～ -100db が実用範囲。渡す値は 1000 ～ -10000
		// デフォルトは 1db ( 100 ) で、これを超えると大体音割れする。(リバーブエフェクト)
		long v = 70 * ( 100 - static_cast< long >( volume ) ) / 100;
		v *= v;
		v = -v + 100;
		if ( v > 100 )
		{
			v = 100;
		}

		mDMPerformance->SetGlobalParam( GUID_PerfMasterVolume, &v, sizeof( long ) );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DirectMusicSegment::setPitch( float pitch )
    {
		DWORD p = 22050 * static_cast< DWORD >( pitch ) / 100;	
		HRESULT hr = mDSSoundBuffer->SetFrequency( p );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    lnU32 DirectMusicSegment::getTotalTime() const
    {
        MUSIC_TIME segment_length = 0;
		mDMSegment->GetLength( &segment_length );
        return segment_length;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    lnU32 DirectMusicSegment::getPlayPosition() const
    {
        MUSIC_TIME time = 0;
        if ( mDMSegmentState )
        {
	        mDMSegmentState->GetSeek( &time );
		    return time;
        }
        return 0;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DirectMusicSegment::setPlayPosition( lnU32 time )
    {
        if ( mDMSegment )
		{
			mDMSegment->SetStartPoint( time );
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool DirectMusicSegment::isPlaying() const
    {
        return mDMPerformance->IsPlaying( mDMSegment, mDMSegmentState ) == S_OK;
    }

	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
	void DirectMusicSegment::setLoopState( bool isLoop, lnU32 begin, lnU32 length )
	{
        HRESULT hr;

		// ループ再生する場合
		if ( isLoop )
		{
			if ( length <= 0 )
			{
				// セグメント全体の長さ取得
				MUSIC_TIME time_length;
				hr = mDMSegment->GetLength( &time_length );
				LN_THROW_Com( hr );
				length = time_length;

                hr = mDMSegment->SetLoopPoints( begin, length );
				LN_THROW_Com( hr );
			}
            else
            {
			    hr = mDMSegment->SetLoopPoints( begin, begin + length );
				LN_THROW_Com( hr );
            }
				
			// 無限ループ
			mDMSegment->SetRepeats( DMUS_SEG_REPEAT_INFINITE );
		}
		// ループ再生しない場合
		else
		{
            mDMSegment->SetLoopPoints( 0, 0 );
			mDMSegment->SetRepeats( 0 );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DirectMusicSegment::play()
    {
        stop();

        SAFE_RELEASE( mDMSegmentState );

		HRESULT hr;
        hr = mDMAudioPath->Activate( TRUE );
		LN_THROW_Com( hr );


        IDirectMusicSegmentState* state = NULL;
		hr = mDMPerformance->PlaySegmentEx( mDMSegment, NULL, NULL, 0, 0, &state, NULL, mDMAudioPath );
		LN_THROW_Com( hr );

		state->QueryInterface( IID_IDirectMusicSegmentState8, (LPVOID*)&mDMSegmentState );
		state->Release();
      
        // 非同期で初期化してた場合、ボリュームなどは、再生開始直前に設定する必要がある
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DirectMusicSegment::stop()
    {
		HRESULT hr;

        if ( mDMAudioPath )
		{
			hr = mDMAudioPath->Activate( FALSE );
			LN_THROW_Com( hr );
		}
        if ( mDMPerformance )
		{
			hr = mDMPerformance->Stop( mDMSegment, NULL, 0, 0 );
			LN_THROW_Com( hr );
		}
    }


//==============================================================================
// ■ DirectMusicManager
//==============================================================================

	// PlayerObject クラスのデストラクタ
    DirectMusicManager::PlayerObject::~PlayerObject()
    {
        DirectMusicManager::getInstance()->removePlayRequest( this );
    }

    DirectMusicManager* DirectMusicManager::sInstance = NULL;

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DirectMusicManager::initialize( const ConfigData& configData )
    {
        if ( !sInstance && configData.DMInitMode != DMINITMODE_NOTUSE )
        {
            sInstance = LN_NEW DirectMusicManager();
            sInstance->_initialize( configData );
        }
    } 

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DirectMusicManager::finalize()
    {
        SAFE_DELETE( sInstance );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DirectMusicManager::DirectMusicManager()
        : mWindowHandle         ( NULL )
        , mInitMode             ( DMINITMODE_THREAD_WAIT )
        , mDirectSound          ( NULL )
        , mDirectMusic          ( NULL )
        , mFirstPerformance     ( NULL )
        , mInitThread           ( NULL )
        , mPerformanceInited    ( NULL )
        , mLock                 ( NULL )
        , mListLock             ( NULL )
        , mErrorState           ( 1 )       // 最初はエラーにしておく。正常に初期化できたら 0
    {
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DirectMusicManager::~DirectMusicManager()
    {
        LN_LOG_FIN_BEGIN;
        // スレッドの終了を待って解放
		if ( mInitThread )
		{
			mInitThread->wait();
			SAFE_DELETE( mInitThread );
		}

        LN_SAFE_RELEASE( mListLock );
        LN_SAFE_RELEASE( mLock );
        LN_SAFE_RELEASE( mPerformanceInited );

        SAFE_RELEASE( mFirstPerformance );
        SAFE_RELEASE( mDirectSound );
	    SAFE_RELEASE( mDirectMusic );

        //::CoUninitialize();

        LN_LOG_FIN_END;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    lnU32 DirectMusicManager::getErrorState() const
    {
		Threading::ScopedLock lock( *mLock );
        return mErrorState;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DirectMusicManager::addPlayRequest( PlayerObject* obj )
    {
		Threading::ScopedLock lock( *mLock );

        // 再生要求リストに追加されていない場合は追加しておく
        PlayRequestList::iterator pos;
        pos = std::find( mPlayRequestList.begin(), mPlayRequestList.end(), obj );
        if ( pos == mPlayRequestList.end() )
        {
            mPlayRequestList.push_back( obj );
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DirectMusicManager::removePlayRequest( PlayerObject* obj )
    {
		Threading::ScopedLock lock( *mLock );
        mPlayRequestList.remove( obj );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    IDirectMusicPerformance8* DirectMusicManager::createDMPerformance()
    {
        // 一応初期化済みかをチェック
        if ( !isInitialized() ) return NULL;

        mLock->lock();
        IDirectMusicPerformance8* performance = mFirstPerformance;
        mLock->unlock();

        // 一番最初に初期化したものがある場合はそれを返す
        if ( performance )
        {
            mLock->lock();
            mFirstPerformance = NULL;
            mLock->unlock();
            return performance;
        }

        // パフォーマンスの作成
        HRESULT hr = CoCreateInstance(
		    CLSID_DirectMusicPerformance,
		    NULL, CLSCTX_INPROC, IID_IDirectMusicPerformance8,
		    (void**)&performance );
		LN_THROW_Com( hr );

        // パフォーマンスの初期化
		//hr = mPerformance->Init( (IDirectMusic**)&direct_music, (IDirectSound*)direct_sound, LDirectMusic::instance()->windowHandle() );
		hr = performance->InitAudio(
			(IDirectMusic**)&mDirectMusic, (IDirectSound**)&mDirectSound,
			mWindowHandle,
			LN_DMUS_TYPE,	
			64, DMUS_AUDIOF_ALL, NULL );
		LN_THROW_Com( hr );

        return performance;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DirectMusicManager::polling()
    {
        if ( mPerformanceInited->isTrue() )
        {
			Threading::ScopedLock lock( *mLock );

            if ( mPlayRequestList.size() > 0 )
            {
                PlayRequestList::iterator it  = mPlayRequestList.begin();
                PlayRequestList::iterator end = mPlayRequestList.end();
                for ( ; it != end; ++it )
                {
                    (*it)->onFinishDMInit( createDMPerformance() );
                }
                mPlayRequestList.clear();
            }
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DirectMusicManager::_initialize( const ConfigData& configData )
    {
		LN_ERR2_ASSERT( configData.WindowHandle );

        LN_LOG_INIT_BEGIN;

        HRESULT hr;

        mWindowHandle = configData.WindowHandle;
        mInitMode     = configData.DMInitMode;

        // COM 初期化
        //HRESULT hr = ::CoInitializeEx( NULL, COINIT_MULTITHREADED );
        //LN_SET_DXERR_R_S( hr, "< DirectMusicManager::_initialize >\nCoInitializeEx" );

        // DirectSound の作成
        hr = ::CoCreateInstance(
				    CLSID_DirectSound8,
				    NULL, 
				    CLSCTX_ALL,
				    IID_IDirectSound8,
				    (void**)&mDirectSound );
		LN_THROW_Com( hr );

        // DirectSound の初期化
        hr = mDirectSound->Initialize( NULL );
		LN_THROW_Com( hr );


	    // 協調レベルの設定
	    hr = mDirectSound->SetCooperativeLevel( mWindowHandle, DSSCL_PRIORITY );
		LN_THROW_Com( hr );

	    // DirectMusic 作成
	    hr = CoCreateInstance( CLSID_DirectMusic, NULL, CLSCTX_INPROC, IID_IDirectMusic, (void**)&mDirectMusic );
		LN_THROW_Com( hr );

	    // 作成した DirectMusic に DirectSound を設定
	    hr = mDirectMusic->SetDirectSound( mDirectSound, mWindowHandle );
		LN_THROW_Com( hr );
    

        mPerformanceInited = LN_NEW Threading::EventFlag( false );
        mLock = LN_NEW Threading::Mutex();
        mListLock = LN_NEW Threading::Mutex();

        if ( mInitMode == DMINITMODE_THREAD_WAIT || mInitMode == DMINITMODE_THREAD_REQUEST )
		{
			mInitThread = LN_NEW PerformanceInitThread();
			mInitThread->start();
		}
		else
		{
			_initPerformance();
		}

        LN_LOG_INIT_END;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DirectMusicManager::_initPerformance()
    {
#ifdef LN_OUT_LOG
        LN_LOG_WRITE( "initialize IDirectMusicPerformance8 ..." );
#endif

        IDirectMusicPerformance8* performance = NULL;
        lnU32 error = 0;
        
        // パフォーマンスの作成
        HRESULT hr = CoCreateInstance(
		    CLSID_DirectMusicPerformance,
		    NULL, CLSCTX_INPROC, IID_IDirectMusicPerformance8,
		    (void**)&performance );
        if ( FAILED( hr ) ) error = 1; 

        if ( SUCCEEDED( hr ) )
        {
            // パフォーマンスの初期化
		    //hr = mPerformance->Init( (IDirectMusic**)&direct_music, (IDirectSound*)direct_sound, LDirectMusic::instance()->windowHandle() );
		    hr = performance->InitAudio(
			    (IDirectMusic**)&mDirectMusic, (IDirectSound**)&mDirectSound,
			    mWindowHandle,
			    LN_DMUS_TYPE,
			    64, DMUS_AUDIOF_ALL, NULL );
		    if ( FAILED( hr ) ) error = 2;
        }

#ifdef LN_OUT_LOG
        LN_LOG_WRITE( "lock IDirectMusicPerformance8" );
#endif

        mLock->lock();
        mFirstPerformance = performance;
        mErrorState = error;
        mLock->unlock();

        // 初期化完了
        mPerformanceInited->setTrue();

#ifdef LN_OUT_LOG
        LN_LOG_WRITE( "success initialize IDirectMusicPerformance8" );
#endif
    }

} // namespace DirectMusic
} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================
