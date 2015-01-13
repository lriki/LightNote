//==============================================================================
// FPSController 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../System/Environment.h"
#include "../Threading/Thread.h"
#include "FPSController.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Game
{

const GameTime GameTime::ZERO_TIME;

//==============================================================================
// �� FPSController �N���X
//==============================================================================

	//----------------------------------------------------------------------
	// �� �R���X�g���N�^
	//----------------------------------------------------------------------
	FPSController::FPSController()
		: mFrameRate		( 0 )
		, mFrameRateRec		( 0.0 )
		, mFPS				( 0.0 )
		, mCurrentTime		( 0.0 )
		, mAverageTime		( 0.0 )
		, mFrameCount		( 0 )
		, mTerm				( 0.0 )
		, mLastTime			( 0.0 )
		, mBaseTime			( 0.0 )
		, mFrameTimes		( NULL )
        , mElapsedGameTime  ( 0.0 )
        , mLastRealTime     ( 0.0 )
        , mElapsedRealTime  ( 0.0 )
        , mStartTime        ( System::Environment::getSystemTime() )
        , mTotalGameTime    ( 0 )
        , mTotalRealTime    ( 0 )
        , mCapacityFPS      ( 0.0 )
        , mCapaFPSLastTime  ( 0.0 )
        , mCapaFrameTimes   ( NULL )
        , mCapaAverageTime  ( 0.0 )
        , mEnableFPSTest    ( false )
		, mEnableFrameWait	( true )
	{
		setFrameRate( 60 );

        mGameTime._m.FPS             = 0.0f;
        mGameTime._m.ElapsedGameTime = 0.0f;
        mGameTime._m.ElapsedRealTime = 0.0f;
        mGameTime._m.TotalGameTime   = 0.0;
        mGameTime._m.TotalRealTime   = 0.0;
        mGameTime._m.IsRunningSlowly = false;
        mGameTime._m.TotalFrameCount = 0;
	}

	//----------------------------------------------------------------------
	// �� �f�X�g���N�^
	//----------------------------------------------------------------------
	FPSController::~FPSController()
	{
		SAFE_DELETE_ARRAY( mFrameTimes );
        SAFE_DELETE_ARRAY( mCapaFrameTimes );
	}

	//----------------------------------------------------------------------
	// �� �t���[�����[�g�̐ݒ�
	//----------------------------------------------------------------------
	void FPSController::setFrameRate( int frame_rate_ )
	{
		mFrameRate = frame_rate_;
		mFrameRateRec = 1.0f / mFrameRate;
		mFrameCount = 0;

		SAFE_DELETE_ARRAY( mFrameTimes );
		mFrameTimes = LN_NEW lnFPSFloat[ mFrameRate ];

        SAFE_DELETE_ARRAY( mCapaFrameTimes );
        mCapaFrameTimes = LN_NEW lnFPSFloat[ mFrameRate ];

		mTerm = 0.0f;
	}

	//----------------------------------------------------------------------
	// �� �x�������Z�b�g����
	//----------------------------------------------------------------------
	void FPSController::refreshSystemDelay( lnU32 current_time_ )
	{
		mLastTime = mBaseTime = mCapaFPSLastTime = ( 0.001f * ( current_time_ - mStartTime ) );// + mFrameRateRec;
		mBaseTime -= 1.0f - mFrameRateRec;
		
		// ���݂� mFrameCount �܂ł̃t���[�������畽�ώ��Ԃ��v�Z����
		mAverageTime = 0;
		for( int i = 0; i < mFrameCount; ++i )
		{
			mAverageTime += mFrameTimes[ i ];
		}
		mAverageTime /= mFrameCount;

		mFrameCount = 0;
		mTerm = 0.0f;
	}

	//----------------------------------------------------------------------
	// �� ����
	//----------------------------------------------------------------------
	void FPSController::process()
	{
		if ( !mEnableFrameWait ) {
			processForMeasure();
			return;
		}

		mCurrentTime = 0.001f * ( System::Environment::getSystemTime() - mStartTime );


        mElapsedGameTime = mCurrentTime - mLastTime;
        mElapsedRealTime = mCurrentTime - mLastRealTime;

        mTotalGameTime += static_cast< lnU32 >( 1000.0f * mElapsedGameTime );
        mTotalRealTime += static_cast< lnU32 >( 1000.0f * mElapsedRealTime );

		// (frame_rate_)�t���[����1��ڂȂ�
		if ( mFrameCount == 0 )
		{
			// ���S�ɍŏ�
			if ( mLastTime == 0.0 )
			{
				mTerm = mFrameRateRec;
			}
			// �O��L�^�������Ԃ����Ɍv�Z
			else
			{
				mTerm = mBaseTime + 1.0f - mCurrentTime;
			}
            
            //printf( "f: a:%f bt:%f rc:%f ct:%f lt:%f\n", mTerm, mBaseTime, mFrameRateRec * mFrameCount, mCurrentTime, mCurrentTime - mLastRealTime );
		}
		else
		{
			// ����҂ׂ����� = ���݂���ׂ����� - ���݂̎���
			mTerm = ( mBaseTime + mFrameRateRec * mFrameCount ) - mCurrentTime;

            //if ( mTerm < 0 )
            //{
            //    printf( "c:%f\n", mTerm );
            //}
            //printf( "s: a:%f bt:%f rc:%f ct:%f lt:%f\n", mTerm, mBaseTime, mFrameRateRec * mFrameCount, mCurrentTime, mCurrentTime - mLastRealTime );
		}

        //printf( "frame:%d term:%f etime:%f\n", mFrameCount, mTerm, mElapsedRealTime );
        //printf( "etime:%f term:%f \n", mElapsedRealTime, mTerm );



        
        static int ii = 0;

        ++ii;

        


		// �҂ׂ����Ԃ����҂�
		if ( mTerm > 0.0 )
		{
            //printf( "t:%lf c:%d\n", mTerm, mFrameCount );

            
            //if ( ii > 120 && mTerm < 0.016 )
            {
                /*
                ii = 120;

                double bb = 0;
                for ( int i = 0; i < 60; ++i )
                {
                    _p( mFrameTimes[ i ] );
                    bb += mFrameTimes[ i ];
                }
                _p((double)ii / 60.0);

                printf( "a\n" );
                */
            }
            
            //::Sleep( 16 );

			Threading::Thread::sleep(static_cast< lnU32 >(mTerm * 1000));
		}
		//// �҂����Ԃ��Ȃ��ꍇ�A���̕`����X�L�b�v����t���O�� ON
		//else
		//{
		//}

		// ���݂̎���
		lnFPSFloat nt = mCurrentTime;

		// mFrameRate �t���[����1�x������
		if ( mFrameCount == 0 )
		{
            // + mFrameRateRec ��1�t���[�����炳�Ȃ��ƁA
            // 60 �t���[������ 1 �t���[���ڂő҂����Ԃ̌v�Z������Ƃ���
            // mCurrentTime �̎��Ԃ����݂���ׂ������𒴂��Ă��܂��āA
            // �҂����Ԃ� -0.0003 �Ƃ��ɂȂ��Ă��܂��B
            // ���ʁA��u��������Ƃ�����������邱�Ƃ��������B
			mBaseTime = nt + mFrameRateRec;
		}

		// ����̕���1���������Ԃ��L�^
		mFrameTimes[ mFrameCount ] = nt - mLastTime;

		// ���݂̎��Ԃ��A�ЂƂO�̎��ԂƂ��ċL��
		mLastRealTime = nt;
		mLastTime = nt;

		// mFrameRate �̍Ō�̃t���[�� ( 60 �̏ꍇ�� 59 �t���[���� ) �ɕ��ς��v�Z
		if ( mFrameCount == mFrameRate - 1 )
		{
			mAverageTime = 0;
			for( int i = 0; i < mFrameRate; ++i )
			{
				mAverageTime += mFrameTimes[ i ];
			}
			mAverageTime /= mFrameRate;
		}





        if ( mEnableFPSTest )
        {
            // ����̕���1���������Ԃ��L�^
		    mCapaFrameTimes[ mFrameCount ] = mCurrentTime - mCapaFPSLastTime;

            // mFrameRate �̍Ō�̃t���[�� ( 60 �̏ꍇ�� 59 �t���[���� ) �ɕ��ς��v�Z
		    if ( mFrameCount == mFrameRate - 1 )
		    {
			    mCapaAverageTime = 0;
			    for ( int i = 0; i < mFrameRate; ++i )
			    {
				    mCapaAverageTime += mCapaFrameTimes[ i ];
			    }

                if ( mCapaAverageTime == 0.0f )
                {
                    mCapaAverageTime = 0.01f;
                }
			    mCapaAverageTime /= mFrameRate;

                mCapacityFPS = 1.0f / mCapaAverageTime;
                if ( mCapacityFPS > 100 * mFrameRate )
                {
                    mCapacityFPS = 100.0f * mFrameRate;
                }
		    }

            mCapaFPSLastTime = 0.001f * ( System::Environment::getSystemTime() - mStartTime );
        }

		// mFrameCount �� frame �ň������悤�ɂ���
		mFrameCount = ( ++mFrameCount ) % mFrameRate;

        // GameTime �N���X�X�V
        mGameTime._m.FPS             = getFPS();
        mGameTime._m.ElapsedGameTime = getElapsedGameTime();
        mGameTime._m.ElapsedRealTime = getElapsedRealTime();
        mGameTime._m.TotalGameTime   = getTotalGameTime();
        mGameTime._m.TotalRealTime   = getTotalRealTime();
        mGameTime._m.IsRunningSlowly = isFrameSkipRequest();
        ++mGameTime._m.TotalFrameCount;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void FPSController::processForMeasure()
    {
        mCurrentTime = 0.001f * ( System::Environment::getSystemTime() - mStartTime );

        mElapsedGameTime = mCurrentTime - mLastTime;
        mElapsedRealTime = mCurrentTime - mLastRealTime;

        mTotalGameTime += static_cast< lnU32 >( 1000.0f * mElapsedGameTime );
        mTotalRealTime += static_cast< lnU32 >( 1000.0f * mElapsedRealTime );

		// (frame_rate_)�t���[����1��ڂȂ�
		if ( mFrameCount == 0 )
		{
			// ���S�ɍŏ�
			if ( mLastTime == 0.0 )
			{
				mTerm = mFrameRateRec;
			}
			// �O��L�^�������Ԃ����Ɍv�Z
			else
			{
				mTerm = mBaseTime + 1.0f - mCurrentTime;
			}            
		}
		else
		{
			// ����҂ׂ����� = ���݂���ׂ����� - ���݂̎���
			mTerm = ( mBaseTime + mFrameRateRec * mFrameCount ) - mCurrentTime;
		}

        static int ii = 0;

        ++ii;

		// �҂ׂ����Ԃ����҂�
		if ( mTerm > 0.0 )
		{
            //Threading::sleep( static_cast< lnU32 >( mTerm * 1000 ) );
		}
		
		// ���݂̎���
		lnFPSFloat nt = mCurrentTime;

		// mFrameRate �t���[����1�x������
		if ( mFrameCount == 0 )
		{
            // + mFrameRateRec ��1�t���[�����炳�Ȃ��ƁA
            // 60 �t���[������ 1 �t���[���ڂő҂����Ԃ̌v�Z������Ƃ���
            // mCurrentTime �̎��Ԃ����݂���ׂ������𒴂��Ă��܂��āA
            // �҂����Ԃ� -0.0003 �Ƃ��ɂȂ��Ă��܂��B
            // ���ʁA��u��������Ƃ�����������邱�Ƃ��������B
			mBaseTime = nt + mFrameRateRec;
		}

		// ����̕���1���������Ԃ��L�^
		mFrameTimes[ mFrameCount ] = nt - mLastTime;

		// ���݂̎��Ԃ��A�ЂƂO�̎��ԂƂ��ċL��
		mLastRealTime = nt;
		mLastTime = nt;
        

		// mFrameRate �̍Ō�̃t���[�� ( 60 �̏ꍇ�� 59 �t���[���� ) �ɕ��ς��v�Z
		if ( mFrameCount == mFrameRate - 1 )
		{
			mAverageTime = 0;
			for( int i = 0; i < mFrameRate; ++i )
			{
				mAverageTime += mFrameTimes[ i ];
			}
			mAverageTime /= mFrameRate;
		}

        if ( mEnableFPSTest )
        {
            // ����̕���1���������Ԃ��L�^
		    mCapaFrameTimes[ mFrameCount ] = mCurrentTime - mCapaFPSLastTime;

            // mFrameRate �̍Ō�̃t���[�� ( 60 �̏ꍇ�� 59 �t���[���� ) �ɕ��ς��v�Z
		    if ( mFrameCount == mFrameRate - 1 )
		    {
			    mCapaAverageTime = 0;
			    for ( int i = 0; i < mFrameRate; ++i )
			    {
				    mCapaAverageTime += mCapaFrameTimes[ i ];
			    }

                if ( mCapaAverageTime == 0.0f )
                {
                    mCapaAverageTime = 0.01f;
                }
			    mCapaAverageTime /= mFrameRate;

                mCapacityFPS = 1.0f / mCapaAverageTime;
                if ( mCapacityFPS > 100 * mFrameRate )
                {
                    mCapacityFPS = 100.0f * mFrameRate;
                }
		    }

            mCapaFPSLastTime = 0.001f * ( System::Environment::getSystemTime() - mStartTime );
        }

		// mFrameCount �� frame �ň������悤�ɂ���
		mFrameCount = ( ++mFrameCount ) % mFrameRate;

        // GameTime �N���X�X�V
        mGameTime._m.FPS             = getFPS();
        mGameTime._m.ElapsedGameTime = getElapsedGameTime();
        mGameTime._m.ElapsedRealTime = getElapsedRealTime();
        mGameTime._m.TotalGameTime   = getTotalGameTime();
        mGameTime._m.TotalRealTime   = getTotalRealTime();
        mGameTime._m.IsRunningSlowly = isFrameSkipRequest();
        ++mGameTime._m.TotalFrameCount;
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Game
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================