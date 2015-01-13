//==============================================================================
// PerfStopwatch 
//------------------------------------------------------------------------------
///**
//  @file       PerfStopwatch.h
//  @brief      PerfStopwatch
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{
#if defined(LNOTE_WIN32) && defined(LNOTE_ENABLE_DEBUG_SUPPORT)

//==============================================================================
// ■ PerfStopwatch クラス
//------------------------------------------------------------------------------
///**
//  @brief      プログラムの時間計測に使うクラス
//*/
//==============================================================================
class PerfStopwatch
{
public:

	/// コンストラクタ
    PerfStopwatch()
        : mBegan    ( false )
    {
        memset( &mFreq,   0x00, sizeof( mFreq ) );
        memset( &mBefore, 0x00, sizeof( mBefore ) );
        memset( &mAfter,  0x00, sizeof( mAfter ) );
        start();
    }
    
	/// デストラクタ
	~PerfStopwatch()
    {
        stopS();
    }

public:

	/// 計測開始
	void start()
    {
        ::QueryPerformanceFrequency( &mFreq );
        ::QueryPerformanceCounter( &mBefore );
        mBegan = true;
    }

	/// 計測終了
	lnU32 stop()
    {
        if ( mBegan )
        {
            mBegan = false;
            ::QueryPerformanceCounter( &mAfter );
            return static_cast< lnU32 >( ( mAfter.QuadPart - mBefore.QuadPart ) * 1000000 / mFreq.QuadPart );
        }
        return 0;
    }

	/// 計測終了 (結果を printf() で表示する)
	void stopS()
    {
        if ( mBegan )
        {
            lnprintf( "pasttime:%d (us)\n", stop() );
        }
    }

private:

    LARGE_INTEGER   mFreq;
    LARGE_INTEGER   mBefore;
    LARGE_INTEGER   mAfter;
    bool            mBegan;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

#endif
} // namespace System
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================