//==============================================================================
// Manager 
//==============================================================================

/*
エフェクトの構成について
	Core … 単なるデータクラス。

	Instance
		実際に更新・子生成等を行う。
		最終目的は描画のための情報の生成。(行列、色etc...)
		描画を行うのは EmitterCore。

	親子関係について
		update() で親から影響を受けるパラメータは行列のみ。(生成タイミングは別として、更新中の要素)
		これ以外のパラメータ(例えばRing の径とか)は、NodeInstance が EmitterCore に
		描画時に現在の時間を伝えて、そこで値を更新する。

	乱数シードについて
		NodeInstance ごとに固有の値を持たなければならない。
		本来であればエディタから設定できるようにするべきだが、
		とりあえず this を使っている。

*/

#include "stdafx.h"
#include "../Base/STLUtils.h"
#include "../Graphics/Manager.h"
#include "../Audio/Manager.h"
//#include "Core/EffectCore.h"
#include "Effekseer/EffekseerEffectEngine.h"
#include "Effekseer/EffekseerEffectInstance.h"
#include "Manager.h"

#ifdef LNOTE_FOR_200

namespace LNote
{
namespace Core
{
namespace Effect
{

//==============================================================================
// ■ Manager
//==============================================================================
    
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Manager::Manager()
        : mFileManager				( NULL )
        , mGraphicsManager			( NULL )
		, mAudioManager				( NULL )
		, mEffekseerEffectEngine	( NULL )
		, mFrameRate				( 60 )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Manager::~Manager()
    {
		LN_SAFE_RELEASE( mEffekseerEffectEngine );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::initialize( const ConfigData& configData )
    {
        mFileManager        = configData.FileManager;
        mGraphicsManager    = configData.GraphicsManager;
		mAudioManager		= configData.AudioManager;

		mEffekseerEffectEngine = LN_NEW EffekseerEffectEngine();
		mEffekseerEffectEngine->initialize( this, 32 );

		mUpdateThread.setUpdateMethod( Base::createDelegate( this, &Manager::internalUpdate ) );
		mUpdateThread.start();
		mUpdateThread.waitForInitialized();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Effect* Manager::createEffect( const lnChar* filePath )
	{
		Effect* effect = NULL;
		if ( mEffekseerEffectEngine ) {
			effect = mEffekseerEffectEngine->createEffect( filePath );
			if ( effect ) {
				return effect;
			}
		}

		LN_THROW_InvalidFormat(0, filePath);
		return NULL;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::finalize()
    {
		mUpdateThread.endRequest();
		mUpdateThread.wait();

		mEffectBaseArray.clear();

		if ( mEffekseerEffectEngine ) {
			mEffekseerEffectEngine->finalize();
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::setViewProjection( const LMatrix& view, const LMatrix& proj )
	{
		if ( mEffekseerEffectEngine ) {
			mEffekseerEffectEngine->setViewProjection( view, proj );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::updateRenderContents()
	{
		// 更新スレッドが待機するまで待つ
		mUpdateThread.waitForTaskWait();

		mContext.mFrameRate = mFrameRate;

		mEffectBaseArray.commit();
		ln_foreach( EffectBase* n, mEffectBaseArray.getObjectArray() )
        {
            n->updateContext();
        }

		if ( mEffekseerEffectEngine ) {
			mEffekseerEffectEngine->updateRenderContents();
		}

		// 更新スレッド再開
		mUpdateThread.enableRunning();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::OnPauseDevice()
	{
		if ( mEffekseerEffectEngine ) {
			mEffekseerEffectEngine->getEffekseerRenderer()->OnLostDevice();
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::OnResumeDevice()
	{
		if ( mEffekseerEffectEngine ) {
			mEffekseerEffectEngine->getEffekseerRenderer()->OnResetDevice();
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::addEffect( EffectBase* effect )
	{
		mEffectBaseArray.registerObject( effect );
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::internalUpdate()
	{
		Threading::ScopedLock lock(mCacheFlash);

		ln_foreach( EffectBase* n, mEffectBaseArray.getObjectArray() )
        {
			n->update();
        }
	}

} // namespace Effect
} // namespace Core
} // namespace LNote

#else
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Effect
{

//==============================================================================
// ■ Manager クラス
//==============================================================================
    
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Manager::Manager()
        : mLogFile          ( NULL )
        , mFileManager      ( NULL )
        , mGraphicsManager  ( NULL )
        , mSprite3DRenderer ( NULL )
        , mSprite2DRenderer ( NULL )
        //, mAsyncBarrier     ( NULL )
        //, mEndRequested     ( NULL )
        , mTimeTick         ( 60.0f )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Manager::~Manager()
    {

    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::initialize( const InitData& init_data_ )
    {
        mFileManager        = init_data_.FileManager;
        mGraphicsManager    = init_data_.GraphicsManager;

        mSprite3DRenderer   = LN_NEW Graphics::SpriteRenderer( mGraphicsManager );
        mSprite3DRenderer->initialize( mGraphicsManager->getGraphicsDevice(), init_data_.MaxSpriteNum, true );

        mSprite2DRenderer   = LN_NEW Graphics::SpriteRenderer( mGraphicsManager );
        mSprite2DRenderer->initialize( mGraphicsManager->getGraphicsDevice(), init_data_.MaxSpriteNum, false );

        mRingRenderer       = LN_NEW Graphics::RingRenderer( mGraphicsManager );
        mRingRenderer->initialize( 4096, true );

        EffectCoreCache::initialize( init_data_.EffectCacheSize );

        //mAsyncUpdating = LN_NEW Threading::EventFlag( false );
        //mEndRequested = LN_NEW Threading::EventFlag( false );
        //mAsyncBarrier = LN_NEW Threading::Barrier( 2 );
        //mUpdateThread = LN_NEW UpdateThread( this );
        //mUpdateThread->start();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::finalize()
    {
        //if ( mUpdateThread )
        //{
        //    mEndRequested->setTrue();               // 終了フラグを立てて
        //    mAsyncBarrier->setNotifyFlag( true );   // バリア同期を行わないようにして
        //    mAsyncUpdating->setTrue();              // 更新スレッドを動かして
        //    mUpdateThread->wait();                  // 更新スレッドの終了を待つ

        //    SAFE_DELETE( mUpdateThread );
        //    LN_SAFE_RELEASE( mAsyncBarrier );
        //    LN_SAFE_RELEASE( mEndRequested );
        //    LN_SAFE_RELEASE( mAsyncUpdating );
        //}

        //ln_foreach( EffectInstance* e, mDrawingInstanceArray )
        //{
        //    e->release();
        //}
        //mDrawingInstanceArray.clear();

        ln_foreach( EffectInstance* e, mEffectInstanceArray )
        {
            e->release();
        }
        mEffectInstanceArray.clear();
		mEffectInstanceRemoveOrderList.clear();

        ln_foreach( EffectInstance* e, mAddedEffectInstanceList )
        {
            e->release();
        }
        mAddedEffectInstanceList.clear();

        EffectCoreCache::finalize();
        LN_SAFE_RELEASE( mSprite3DRenderer );
        LN_SAFE_RELEASE( mSprite2DRenderer );
        LN_SAFE_RELEASE( mRingRenderer );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::setViewProjection( const LMatrix& view_, const LMatrix& proj_, bool is_3d_pahse_ )
    {
        if ( is_3d_pahse_ )
        {
            mSprite3DRenderer->setViewProjMatrix( view_, proj_ );
            mRingRenderer->setViewProjMatrix( view_, proj_ );
        }
        else
        {
			LN_ERR2_ASSERT( "pixel size 未設定" );
            mSprite2DRenderer->setViewProjMatrix( view_, proj_ );
        }
    }

 //   //---------------------------------------------------------------------
	////
 //   //---------------------------------------------------------------------
 //   Effect* Manager::createEffect()
 //   {
 //       Effect* ef = LN_NEW Effect( this );
 //       ef->initialize( createEmptyEffectCore() );
 //       return ef;
 //   }

 //   //---------------------------------------------------------------------
	////
 //   //---------------------------------------------------------------------
 //   EffectCore* Manager::createEmptyEffectCore()
 //   {
 //       EffectCore* core = LN_NEW EffectCore( this );
 //       core->initialize();
 //       return core;
 //   }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//void Manager::advanceTime( lnTime delta_time_ )
	//{
	//    lnTime delta_frame = delta_time_ * mTimeTick;

	//    ln_foreach( EffectInstance* e, mEffectInstanceArray )
	//    {
	//        if ( e->isEnableAutoUpdate() )
	//            e->advanceFrame( delta_frame );
	//    }
	//    ln_foreach( EffectInstance* e, mAddedEffectInstanceList )
	//    {
	//        if ( e->isEnableAutoUpdate() )
	//            e->advanceFrame( delta_frame );
	//    }
	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::setGameTime( const Game::GameTime& game_time_ )
    {
        mGameTime = game_time_;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::update()
    {
        lnTime delta_frame = mGameTime.getElapsedGameTime() * mTimeTick;

        ln_foreach( EffectInstance* e, mEffectInstanceArray )
        {
            if ( e->isEnableAutoUpdate() )
            {
                e->advanceFrame( delta_frame );
                e->update();
            }
        }
        //ln_foreach( EffectInstance* e, mAddedEffectInstanceList )
        //{
        //    if ( e->isEnableAutoUpdate() )
        //    {
        //        e->advanceFrame( delta_frame );
        //        e->update();
        //    }
        //}


        //if ( mUpdateThread )
        //{
        //    // 更新スレッドもこの wait() を踏むまで待つ
        //    mAsyncBarrier->wait();

        //    // リスト更新
        //    ln_foreach( EffectInstance* e, mAddedEffectInstanceList )
        //    {
        //        mEffectInstanceArray.push_back( e );
        //    }
        //    mAddedEffectInstanceList.clear();

        //    // 更新スレッド開始
        //    mAsyncUpdating->setTrue();
        //}
        //else

        //{
        //    _update();
        //}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::updateDrawContents()
    {
        // release
        //ln_foreach( EffectInstance* e, mDrawingInstanceArray )
        //{
        //    e->release();
        //}
        //mDrawingInstanceArray.clear();
        
        // 追加リストからの移動
        ln_foreach( EffectInstance* e, mAddedEffectInstanceList )
        {
            mEffectInstanceArray.push_back( e );
        }
        mAddedEffectInstanceList.clear();

		// 削除リストにあるものは削除
		ln_foreach( EffectInstance* e, mEffectInstanceRemoveOrderList )
		{
			bool removed = Base::STLUtil::remove( mEffectInstanceArray, e );
			if ( removed ) e->release();
		}
		mEffectInstanceRemoveOrderList.clear();

        // Manager からしか参照されていない場合は削除
        EffectInstanceArray::iterator itr = mEffectInstanceArray.begin();
        while ( itr != mEffectInstanceArray.end() )
        {
            if ( (*itr)->getRefCount() == 1 )
            {
                (*itr)->release();
                itr = mEffectInstanceArray.erase( itr );
            }
            else
            {
                ++itr;
            }
        }

        // 描画準備
        ln_foreach( EffectInstance* e, mEffectInstanceArray )
        {
            e->updateDrawContents();
            //mDrawingInstanceArray.push_back( e );
        }

        // addref
        //ln_foreach( EffectInstance* e, mDrawingInstanceArray )
        //{
        //    e->addRef();
        //}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//void Manager::drawBatch3D()
	//{
	//    LRenderState s;
	//    s.BlendMode = LN_BLEND_ADD;
	//    s.Culling = LN_CULL_DOUBLE;
	//    s.AlphaTest = false;
	//    s.DepthTest = true;
	//    s.DepthWrite = false;
	//    mGraphicsManager->getGraphicsDevice()->getRenderer()->setRenderState( s );

	//    mSprite3DRenderer->flash();
	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//void Manager::drawBatch2D()
	//{
	//    LRenderState s;
	//    s.BlendMode = LN_BLEND_ADD;
	//    s.Culling = LN_CULL_DOUBLE;
	//    s.AlphaTest = false;
	//    s.DepthTest = false;
	//    s.DepthWrite = false;
	//    mGraphicsManager->getGraphicsDevice()->getRenderer()->setRenderState( s );

	//    mSprite2DRenderer->flash();
	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::addEffectInstance( EffectInstance* effect )
    {
		LN_ERR2_ASSERT( effect );

        mAddedEffectInstanceList.push_back( effect );
        effect->addRef();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::removeEffectInstance( EffectInstance* effect )
    {
		LN_ERR2_ASSERT( effect );
		mEffectInstanceRemoveOrderList.push_back( effect );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//void Manager::setAllModifiedFlag( bool flag_ )
	//{
	//    
	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//void Manager::_update()
	//{
	//    ln_foreach( EffectInstance* e, mEffectInstanceArray )
	//    {
	//        e->update();
	//    }
	//    
	//}

//==============================================================================
// ■ Manager クラス
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//void Manager::UpdateThread::operator()()
	//{
	//    while ( true )
	//    {
	//        mManager->mAsyncUpdating->setFalse();
	//        mManager->mAsyncBarrier->wait();
	//        mManager->mAsyncUpdating->wait();   // Manager からフラグが true になるまで待機

	//        if ( mManager->mEndRequested->isTrue() ) return;

	//        mManager->_update();
	//    }
	//}


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Effect
} // namespace Core
} // namespace LNote
#endif

//==============================================================================
//
//==============================================================================