//==============================================================================
// GameAudio 
//==============================================================================

#include "stdafx.h"
#include "Device/AudioPlayerBase.h"
#include "Manager.h"
#include "GameAudio.h"

namespace LNote
{
namespace Core
{
namespace Audio
{

//==============================================================================
// ■ GameAudio
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	GameAudio::GameAudio( Manager* mamager )
		: mManager	        ( mamager )
        , mLock             ( NULL )
		, mBGM		        ( NULL )
        , mBGS              ( NULL )
        , mME               ( NULL )
        , mBGMVolume        ( 0 )
        , mBGMPitch         ( 0 )
        , mBGSVolume        ( 0 )
        , mBGSPitch         ( 0 )
        , mBGMFadeOutTime   ( 500 )
        , mBGMFadeInTime    ( 1000 )
        , mMEPlaying        ( false )
        , mBGMRestart       ( false )
        , mBGSRestart       ( false )
	{
        mLock = LN_NEW Threading::Mutex();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	GameAudio::~GameAudio()
	{
        stopBGM( 0 );
        stopBGS( 0 );
        stopME();
		stopSE();

        // 再生終了後の解放リストに入っているサウンドを解放
        ReleaseAtPlayEndList::iterator itr = mReleaseAtPlayEndList.begin();
        ReleaseAtPlayEndList::iterator end = mReleaseAtPlayEndList.end();
        for ( ; itr != end; ++itr )
        {
            (*itr)->release();
        }
        mReleaseAtPlayEndList.clear();

		LN_SAFE_RELEASE( mBGM );
        LN_SAFE_RELEASE( mBGS );
        LN_SAFE_RELEASE( mME );
        LN_SAFE_RELEASE( mLock );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GameAudio::playBGM( const lnChar* filename, int volume, int pitch, int fade_time )
	{
        // 演奏再開チェック
        //if ( !mBGMRestart && mBGM )
        //{
        //    if ( mBGMName == filename_ )
        //    {
        //        return LN_OK;
        //    }
        //}

		Base::RefPtr<Sound> sound(
			mManager->createSound( filename, SOUNDPLAYTYPE_STREAMING, false ) );

		playBGMFromSound( sound, volume, pitch, fade_time );

        // ファイル名記憶
        mBGMName = filename;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GameAudio::playBGMFromSound( Sound* sound, int volume, int pitch, int fade_time )
    {
		LN_ERR2_ASSERT( sound );

        // 演奏再開チェック
		// TODO:フェードアウト中に再開すると無音になる
        //if ( !mBGMRestart && mBGM )
        //{
        //    if ( sound_ == mBGM && volume_ == mBGMVolume && pitch_ == mBGMPitch )
        //    {
        //        return LN_OK;
        //    }
        //}

		Base::RefPtr<Sound> prev_bgm( mBGM, false );

		{
			Threading::ScopedLock lock( *mLock );

			mBGM = sound;
			LN_SAFE_ADDREF( mBGM );

			mBGM->setPitch( pitch );
			mBGM->loop( true );
			mBGM->setGroupFlag( AUDIOPLAYER_GROUP_BGM );

			mBGMVolume = volume;
			mBGMPitch  = pitch;

			// ME が再生中ではないかを調べる
			bool me_not_play = true;    
			if ( mME )
			{
				AudioPlayState state;
				mME->getState( &state );
				if ( state.PlayingState == SOUNDPLAYSTATE_PLAYING )
				{
					me_not_play = false;
				}
			}

			// ME 再生中ではない場合
			if ( me_not_play )
			{
				if ( fade_time > 0 )
				{
					mBGM->setVolume( 0 );
					mBGM->play();
					mBGM->fadeVolume( volume, fade_time, SOUNDFADE_CONTINUE );
				}
				else
				{
					mBGM->setVolume( volume );
					mBGM->play();
				}
			}
			// ME 再生中の場合は、一度再生するけどすぐに一時停止する ( ME 終了後に再開 )
			else
			{
				mBGM->play();
				mBGM->pause( true );
			}
		}


        // フェード時間がある場合
        if ( fade_time > 0 )
		{
			if ( prev_bgm )
			{
				// ひとつ前のBGMは、fade_time_ 後に停止、解放するようにする
				prev_bgm->loop( false );
				prev_bgm->fadeVolume( 0, fade_time, SOUNDFADE_STOP_RESET );
                _pushReleaseAtPlayEndList( prev_bgm );
			}
		}
        // フェード時間がない場合はすぐに停止
		else
		{
			if ( prev_bgm )
            {
                prev_bgm->stop();
            }
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GameAudio::stopBGM( int fade_time )
	{
		if ( mBGM )
		{
			if ( fade_time > 0 )
		    {
                // フェード終了後に停止して、音量等を元に戻す
			    mBGM->fadeVolume( 0, fade_time, SOUNDFADE_STOP_RESET );
		    }
		    else
		    {
			    mBGM->stop();
                LN_SAFE_RELEASE( mBGM );
		    }

			mBGMName = _T("");
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GameAudio::playBGS( const lnChar* filename, int volume, int pitch, int fade_time )
    {
        // 演奏再開チェック
        //if ( !mBGSRestart && mBGS )
        //{
        //    if ( mBGSName == filename_ )
        //    {
        //        return LN_OK;
        //    }
        //}

		Base::RefPtr<Sound> sound(
			mManager->createSound( filename, SOUNDPLAYTYPE_STREAMING, false ) );

		playBGSFromSound( sound, volume, pitch, fade_time );
        
        // ファイル名記憶
        mBGSName = filename;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GameAudio::playBGSFromSound( Sound* sound, int volume, int pitch, int fade_time )
    {
        // 演奏再開チェック
        //if ( !mBGSRestart && mBGS )
        //{
        //    if ( sound_ == mBGS && volume_ == mBGSVolume && pitch_ == mBGSPitch )
        //    {
        //        return LN_OK;
        //    }
        //}

		Base::RefPtr<Sound> prev_bgs( mBGS, false );

		{
			Threading::ScopedLock lock( *mLock );

			mBGS = sound;
			LN_SAFE_ADDREF( mBGS );

			mBGS->setPitch( pitch );
			mBGS->loop( true );
			mBGS->setGroupFlag( AUDIOPLAYER_GROUP_BGS );

			mBGSVolume = volume;
			mBGSPitch  = pitch;

			if ( fade_time > 0 )
			{
				mBGS->setVolume( 0 );
				mBGS->play();
				mBGS->fadeVolume( volume, fade_time, SOUNDFADE_CONTINUE );
			}
			else
			{
				mBGS->setVolume( volume );
				mBGS->play();
			}
		}

        // フェード時間がある場合
        if ( fade_time > 0 )
		{
			if ( prev_bgs )
			{
				// ひとつ前のBGSは、fade_time_ 後に停止、解放するようにする
				prev_bgs->loop( false );
				prev_bgs->fadeVolume( 0, fade_time, SOUNDFADE_STOP_RESET );
                _pushReleaseAtPlayEndList( prev_bgs );
			}
		}
        // フェード時間がない場合はすぐに停止
		else
		{
			if ( prev_bgs )
            {
                prev_bgs->stop();
            }
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GameAudio::stopBGS( int fade_time )
	{
		if ( mBGS )
		{
			if ( fade_time > 0 )
		    {
                // フェード終了後に停止して、音量等を元に戻す
			    mBGS->fadeVolume( 0, fade_time, SOUNDFADE_STOP_RESET );
		    }
		    else
		    {
			    mBGS->stop();
		    }

			mBGSName = _T("");
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GameAudio::playME( const lnChar* filename, int volume, int pitch )
    {       
		Base::RefPtr<Sound> sound(
			mManager->createSound( filename, SOUNDPLAYTYPE_AUTO, false ) );

		playMEFromSound( sound, volume, pitch );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GameAudio::playMEFromSound( Sound* sound, int volume, int pitch )
    {
        stopME();

        mME = sound;
        LN_SAFE_ADDREF( mME );

		mME->setVolume( volume );
        mME->setPitch( pitch );
		mME->loop( false );
        mME->setGroupFlag( 0 );

        bool flag = false;  // BGM があり、再生されているかを示すフラグ

		// BGM がある場合
		if ( mBGM )
		{
            AudioPlayState state;
            mBGM->getState( &state );

			// 再生されている場合
            if ( state.PlayingState == SOUNDPLAYSTATE_PLAYING )
			{
				flag = true;
			}
		}

        // BGM が再生中の場合
        if ( flag )
		{
			// フェードアウト時間がある場合
			if ( mBGMFadeOutTime > 0 )
			{
				// フェードアウト後、一時停止する
				mBGM->fadeVolume( 0, mBGMFadeOutTime, SOUNDFADE_PAUSE );
			}
			// フェードアウト時間がない場合
			else
			{
				// すぐに一時停止
				mBGM->pause( true );
			}
		}
        // BGM がない場合はすぐ演奏開始
		else
		{
			mME->play();
			mMEPlaying = true;
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GameAudio::stopME()
	{
		if ( mME )
		{
			mME->stop();
            LN_SAFE_RELEASE( mME );
			
			// BGM があって、一時停止中の場合は再開
			if ( mBGM )
			{
                AudioPlayState state;
                mBGM->getState( &state );

                if ( state.PlayingState == SOUNDPLAYSTATE_PAUSING )
				{
					mBGM->fadeVolume( mBGMVolume, mBGMFadeInTime, SOUNDFADE_CONTINUE );
                    mBGM->pause( false );
				}
			}

			mMEPlaying = false;
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GameAudio::playSE( const lnChar* filename, int volume, int pitch )
	{
		Base::RefPtr<Sound> sound(
			mManager->createSound( filename, SOUNDPLAYTYPE_ONMEMORY, false ) );

        // ボリューム・ピッチ設定
        sound->setVolume( volume );
        sound->setPitch( pitch );

        // 再生途中で解放されようとしても再生終了までは解放されない & SE として再生する
        sound->setGroupFlag( AUDIOPLAYER_GROUP_SE );
        _pushReleaseAtPlayEndList( sound );

        sound->setGroupFlag( AUDIOPLAYER_GROUP_SE );

        // 再生
		sound->loop( false );
		sound->play();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GameAudio::playSE( const lnChar* filename, const LVector3& position, float distance, int volume, int pitch )
    {
        // サウンド作成
		Base::RefPtr<Sound> sound(
			mManager->createSound( filename, SOUNDPLAYTYPE_ONMEMORY, true ) );

        // 位置・ボリューム・ピッチ設定
        sound->setPosition( position );
        sound->setMaxDistance( distance );
        sound->setVolume( volume );
        sound->setPitch( pitch );

        // 再生途中で解放されようとしても再生終了までは解放されない & SE として再生する
        sound->setGroupFlag( AUDIOPLAYER_GROUP_SE );
        _pushReleaseAtPlayEndList( sound );
        
        // 再生
		sound->loop( false );
		sound->play();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GameAudio::playSEFromSound( Sound* srcSound, int volume, int pitch )
    {
        // 受け取った Sound が持っているソースをもとに新しい Sound を作成
		Base::RefPtr<Sound> sound(
			mManager->createSound(
				srcSound->getAudioPlayer()->getAudioSource(),
				SOUNDPLAYTYPE_ONMEMORY,
				srcSound->is3DSound() ) );

        // 位置・ボリューム・ピッチ設定
        sound->setVolume( volume );
        sound->setPitch( pitch );
        if ( srcSound->is3DSound() )
        {
            sound->setPosition( srcSound->getPosition() );
        }

        // 再生途中で解放されようとしても再生終了までは解放されない & SE として再生する
        sound->setGroupFlag( AUDIOPLAYER_GROUP_SE );
        _pushReleaseAtPlayEndList( sound );
        
        // 再生
		sound->loop( false );
		sound->play();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GameAudio::stopSE()
	{
        mManager->stopGroup( AUDIOPLAYER_GROUP_SE );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GameAudio::setMEFadeState( int begin, int end )
    {
        if ( begin >= 0 )
        {
            mBGMFadeOutTime = begin;
        }
        if ( end >= 0 )
        {
            mBGMFadeInTime = end;
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GameAudio::setBGMVolume( int volume, int fadeTime )
	{
		Threading::ScopedLock lock( *mLock );

		// GameAudio 内では SOUNDFADE_STOP_RESET == フェードアウト中
		if ( mBGM && mBGM->getFadeState() != SOUNDFADE_STOP_RESET )
		{
			mBGMVolume = volume;
			mBGM->fadeVolume( volume, fadeTime, SOUNDFADE_CONTINUE );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GameAudio::setBGSVolume( int volume, int fadeTime )
	{
		Threading::ScopedLock lock( *mLock );

		// GameAudio 内では SOUNDFADE_STOP_RESET == フェードアウト中
		if ( mBGS && mBGS->getFadeState() != SOUNDFADE_STOP_RESET )
		{
			mBGSVolume = volume;
			mBGS->fadeVolume( volume, fadeTime, SOUNDFADE_CONTINUE );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Sound* GameAudio::getInternalGameSound( InternalGameSound type )
	{
		switch ( type )
		{
			case InternalGameSound_BGM:
				return mBGM;
			case InternalGameSound_BGS:
				return mBGS;
			case InternalGameSound_ME:
				return mME;
		}
		return NULL;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GameAudio::polling()
	{
		Threading::ScopedLock lock( *mLock );

		// 演奏する ME がある場合
		if ( mME )
		{
            AudioPlayState mestate;
            mME->getState( &mestate );

			// BGM がある場合
			if ( mBGM )
			{
                AudioPlayState bgmstate;
                mBGM->getState( &bgmstate );

				// BGMのフェードアウトが終わって一時停止状態になっている場合
                if ( bgmstate.PlayingState == SOUNDPLAYSTATE_PAUSING )
				{
                    // ME 再生開始
					if ( !mMEPlaying )
					{
						mME->loop( false );
						mME->play();
						mMEPlaying = true;
					}
					// ME の再生が終了した場合
                    else if ( mestate.PlayingState != SOUNDPLAYSTATE_PLAYING )
					{
						// ME 再生中に BGM がストップしたとかで解放されている場合はなにもしない
						if ( mBGM )
						{
							mBGM->fadeVolume( mBGMVolume, mBGMFadeInTime, SOUNDFADE_CONTINUE );
                            mBGM->pause( false );
						}
						LN_SAFE_RELEASE( mME );
						mMEPlaying = false;
					}
				}
			}
			// BGM がない場合
			else
			{
				// ME が終了した場合
                if ( mestate.PlayingState != SOUNDPLAYSTATE_PLAYING )
				{
					LN_SAFE_RELEASE( mME );
                    mMEPlaying = false;
				}
			}
		}

        /// 再生終了している音声はリストから外して解放する
        AudioPlayState state;
        ReleaseAtPlayEndList::iterator itr = mReleaseAtPlayEndList.begin();
        ReleaseAtPlayEndList::iterator end = mReleaseAtPlayEndList.end();
        for ( ; itr != end;  )
        {
            (*itr)->getState( &state );
            if ( state.PlayingState != SOUNDPLAYSTATE_PLAYING )
            {
                (*itr)->release();
                itr = mReleaseAtPlayEndList.erase( itr );
                end = mReleaseAtPlayEndList.end();
            }
            else
            {
                ++itr;
            }
        }
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GameAudio::_pushReleaseAtPlayEndList( Sound* sound )
    {
        if ( sound )
        {
			Threading::ScopedLock lock( *mLock );
            mReleaseAtPlayEndList.push_back( sound );
            LN_SAFE_ADDREF( sound );
        }
    }

} // namespace Audio
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================