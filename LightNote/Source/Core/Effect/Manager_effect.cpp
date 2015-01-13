//==============================================================================
// Manager 
//==============================================================================

/*
�G�t�F�N�g�̍\���ɂ���
	Core �c �P�Ȃ�f�[�^�N���X�B

	Instance
		���ۂɍX�V�E�q���������s���B
		�ŏI�ړI�͕`��̂��߂̏��̐����B(�s��A�Fetc...)
		�`����s���̂� EmitterCore�B

	�e�q�֌W�ɂ���
		update() �Őe����e�����󂯂�p�����[�^�͍s��̂݁B(�����^�C�~���O�͕ʂƂ��āA�X�V���̗v�f)
		����ȊO�̃p�����[�^(�Ⴆ��Ring �̌a�Ƃ�)�́ANodeInstance �� EmitterCore ��
		�`�掞�Ɍ��݂̎��Ԃ�`���āA�����Œl���X�V����B

	�����V�[�h�ɂ���
		NodeInstance ���ƂɌŗL�̒l�������Ȃ���΂Ȃ�Ȃ��B
		�{���ł���΃G�f�B�^����ݒ�ł���悤�ɂ���ׂ������A
		�Ƃ肠���� this ���g���Ă���B

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
// �� Manager
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
		// �X�V�X���b�h���ҋ@����܂ő҂�
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

		// �X�V�X���b�h�ĊJ
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
// �� Manager �N���X
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
        //    mEndRequested->setTrue();               // �I���t���O�𗧂Ă�
        //    mAsyncBarrier->setNotifyFlag( true );   // �o���A�������s��Ȃ��悤�ɂ���
        //    mAsyncUpdating->setTrue();              // �X�V�X���b�h�𓮂�����
        //    mUpdateThread->wait();                  // �X�V�X���b�h�̏I����҂�

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
			LN_ERR2_ASSERT( "pixel size ���ݒ�" );
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
        //    // �X�V�X���b�h������ wait() �𓥂ނ܂ő҂�
        //    mAsyncBarrier->wait();

        //    // ���X�g�X�V
        //    ln_foreach( EffectInstance* e, mAddedEffectInstanceList )
        //    {
        //        mEffectInstanceArray.push_back( e );
        //    }
        //    mAddedEffectInstanceList.clear();

        //    // �X�V�X���b�h�J�n
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
        
        // �ǉ����X�g����̈ړ�
        ln_foreach( EffectInstance* e, mAddedEffectInstanceList )
        {
            mEffectInstanceArray.push_back( e );
        }
        mAddedEffectInstanceList.clear();

		// �폜���X�g�ɂ�����͍̂폜
		ln_foreach( EffectInstance* e, mEffectInstanceRemoveOrderList )
		{
			bool removed = Base::STLUtil::remove( mEffectInstanceArray, e );
			if ( removed ) e->release();
		}
		mEffectInstanceRemoveOrderList.clear();

        // Manager ���炵���Q�Ƃ���Ă��Ȃ��ꍇ�͍폜
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

        // �`�揀��
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
// �� Manager �N���X
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
	//        mManager->mAsyncUpdating->wait();   // Manager ����t���O�� true �ɂȂ�܂őҋ@

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