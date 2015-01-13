//==============================================================================
// Stopwatch 
//------------------------------------------------------------------------------
///**
//  @file       Stopwatch.h
//  @brief      Stopwatch
//*/
//==============================================================================

#pragma once

namespace LNote
{
namespace Core
{
namespace System
{

//==============================================================================
// Stopwatch
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class Stopwatch
{
public:
	Stopwatch();
	~Stopwatch();

public:

	/// 計測開始
	void start();

	/// 経過時間の取得 (ns)
	lnU64 getElapsedTime();

private:
#ifdef LNOTE_WIN32
    LARGE_INTEGER   mFreq;
    LARGE_INTEGER   mBefore;
#endif
};

} // namespace System
} // namespace Core
} // namespace LNote
