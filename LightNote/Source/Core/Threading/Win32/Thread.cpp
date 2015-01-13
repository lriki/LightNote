//==============================================================================
// Thread 
//==============================================================================

#include "stdafx.h"
#include <sstream>
#include <iostream>
#include "../../System/Environment.h"
#include "../../Resource/LNResource.h"
#include "../EventFlag.h"
#include "../Thread.h"

namespace LNote
{
namespace Core
{
namespace Threading
{

//==============================================================================
// Thread
//==============================================================================
#if defined(LNOTE_WIN32)
    
	class Thread::_thr_prx
    {
    public:

#ifdef LNOTE_ENABLE_WIN32_RUNTIME_THREAD
        static DWORD LNOTE_STDCALL _execute_static( void* ptr )
        {
#else
        static unsigned int LNOTE_STDCALL _execute_static( void* ptr )
        {
#endif
            Thread* obj = static_cast< Thread* >( ptr );

            // 実行
            try
            {
                obj->run();
            }
            catch ( Base::Exception& e )
            {
				obj->mLastException = e.copy();
				//std::wostringstream oss;
				//oss << Resource::String::ERR_ThreadExceptionAbortW;
				//oss << L"\nThread:";
				//oss << obj;
				//oss << L"\nThreadID:";
				//oss << getCurrentThreadID();
				//oss << L"\n\n";
				//oss << e.getOutputMessage();
				//System::MessageBox::showError( NULL, oss.str().c_str() );
            }
            catch (...)
			{
				obj->mLastException = LN_NEW Base::ThreadException();
				//std::wostringstream oss;
				//oss << Resource::String::ERR_ThreadExceptionAbortW;
				//oss << L"\nThread:";
				//oss << obj;
				//oss << L"\nThreadID:";
				//oss << getCurrentThreadID();
				//oss << L"\n\n";
				//oss << L"Raising an exception of an unknown.";
				//System::MessageBox::showError( NULL, oss.str().c_str() );
            }

            // 終了フラグを立てる
            obj->mFinished->setTrue();
            return 0;
        }
    };

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Thread::Thread()
		: mFinished			( NULL )
		, mLastException	( NULL )
        , mThread			( NULL )
        , mThreadID			( 0 )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Thread::~Thread()
	{
        if ( mFinished )
        {
            printf( "< Thread::~Thread >\n" );
            printf( "スレッドが終了していません。wait() で待機します。\n" );
            printf( "もし応答が無くなった場合はスレッドが無限ループになっていないか確認してください。" );

            wait();
        }
		//LN_ASSERT_S_EXP_( ( mFinished ), LNException::Thread, "< Core::Threading::Threading::~Thread >\n\nstart() で開始したスレッドは\n必ず wait() を呼び出して終了してください。" );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Thread::start()
	{
		// 前のが終了していない場合は待つ
		wait();

		// 終了フラグを作って開始
		mFinished = LN_NEW EventFlag( false );

#ifdef LNOTE_ENABLE_WIN32_RUNTIME_THREAD
        //mThread = (HANDLE)(::AfxBeginThread(_thr_prx::_execute_static, this));

        mThread = CreateThread(
		    NULL, //セキュリティ属性
		    0, //スタックサイズ
		    _thr_prx::_execute_static, //スレッド関数
		    this, //スレッド関数に渡す引数
		    0, //作成オプション(0またはCREATE_SUSPENDED)
		    &mThreadID);//スレッドID
#else
        mThread = reinterpret_cast< HANDLE >( _beginthreadex( NULL, 0, _thr_prx::_execute_static, this, 0, &mThreadID ) );
#endif
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Thread::wait()
	{
		if ( mFinished )
		{
			mFinished->wait();
			LN_SAFE_RELEASE( mFinished );
			mThread = 0;
			mThreadID = 0;

			// スレッドで例外が発生していれば throw する
			if (mLastException != NULL) {
				throw *mLastException;
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Thread::isFinished()
	{
		bool res = false;
		if ( mFinished )
		{
			// 終わっている場合
			if ( mFinished->isTrue() )
			{
				LN_SAFE_RELEASE( mFinished );
				res = true;
			}
		}
		else
		{
			res = true;
		}
		return res;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    lnThreadID Thread::getThreadID() const
    { 
        return mThreadID;
    }
    
#else
    
//==============================================================================
// ■ Thread クラス (pthread)
//==============================================================================
    
    class _thr_prx
    {
    public:

        static void* _execute_static( void* ptr_ )
        {
            Thread* obj = static_cast< Thread* >( ptr_ );

            // 実行
            try
            {
                obj->run();
            }
            catch ( Base::Exception& e )
            {
                _tprintf( "%s\n", e.getOutputMessage() );
            }
            catch (...)
            {
                // TODO:単純にprintf でいいの？
                printf( "Raising an exception of an unknown\n" );
            }

            // 終了フラグを立てる
            obj->mFinished->setTrue();

            return 0;
        }
    };

	//----------------------------------------------------------------------
	// ● コンストラクタ
	//----------------------------------------------------------------------
	Thread::Thread()
		: mFinished		( NULL )
	{
		}

	//----------------------------------------------------------------------
	// ● デストラクタ
	//----------------------------------------------------------------------
	Thread::~Thread()
	{
        if ( mFinished )
        {
            printf( "< Thread::~Thread >\n" );
            printf( "スレッドが終了していません。wait() で待機します。\n" );
            printf( "もし応答が無くなった場合はスレッドが無限ループになっていないか確認してください。" );

            wait();
        }
        //LN_ASSERT_S_EXP_( ( mFinished ), LNException::Thread, "< Core::Threading::Threading::~Thread >\n\nstart() で開始したスレッドは\n必ず wait() を呼び出して終了してください。" );
    }

	//----------------------------------------------------------------------
	// ● 実行開始
	//----------------------------------------------------------------------
    void Thread::start()
    {
        // 前のが終了していない場合は待つ
        wait();

        // 終了フラグを作って開始
        mFinished = LN_NEW EventFlag( false );

        pthread_create( &mThread,  NULL, _thr_prx::_execute_static, this );
    }

	//----------------------------------------------------------------------
	// ● 終了待ち
	//----------------------------------------------------------------------
    void Thread::wait()
    {
        if ( mFinished )
        {
            mFinished->wait();
            LN_SAFE_RELEASE( mFinished );
        }
    }

	//----------------------------------------------------------------------
	// ● 終了しているかどうかの判定
	//----------------------------------------------------------------------
    bool Thread::isFinished()
    {
        bool res = false;
        if ( mFinished )
        {
            // 終わっている場合
            if ( mFinished->isTrue() )
            {
                LN_SAFE_RELEASE( mFinished );
                res = true;
            }
        }
        else
        {
            res = true;
        }
        return res;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    lnThreadID Thread::getThreadID() const
    { 
        return mThread;
    }
    
#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void Thread::sleep(int ms)
{
#if defined(LNOTE_WIN32)
    ::Sleep( ms );
#else
    ::usleep( ms_ * 1000 );
#endif
}

//---------------------------------------------------------------------
// 
//---------------------------------------------------------------------
lnThreadID Thread::getCurrentThreadID()
{
#if defined(LNOTE_WIN32)
    return ::GetCurrentThreadId();
#else
    return pthread_self();
#endif
}

} // namespace Threading
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================