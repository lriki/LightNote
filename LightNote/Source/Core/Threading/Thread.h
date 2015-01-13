//==============================================================================
// Thread 
//------------------------------------------------------------------------------
///**
//  @file       Thread.h
//  @brief      Thread
//*/
//==============================================================================

#pragma once

namespace LNote
{
namespace Core
{
namespace Threading
{
class EventFlag;

//==============================================================================
// Thread
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class Thread
    : private Base::NonCopyable
{
public:
	Thread();
	virtual ~Thread();

public:

	/// スレッド関数 (これを自身で実装する)
	virtual void run() = 0;

	/// 実行開始
	void start();

	/// 終了待ち
	void wait();

	/// 終了しているかどうかの判定
	bool isFinished();

	/// スレッド識別子の取得 (start() 後に呼ぶこと)
    lnThreadID getThreadID() const;

public:

	/// 時間待ち
	static void sleep(int ms);

	/// 呼び出し側スレッドのスレッド識別子の取得
	static lnThreadID getCurrentThreadID();

private:
	EventFlag*			mFinished;
	Base::Exception*	mLastException;
    
#if defined(LNOTE_WIN32)
    HANDLE		mThread;
    lnThreadID	mThreadID;
#else
    lnThreadID	mThread;
#endif

	class _thr_prx;
    friend class _thr_prx;
};


} // namespace Threading
} // namespace Core
} // namespace LNote
