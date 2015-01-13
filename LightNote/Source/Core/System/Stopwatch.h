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

	/// �v���J�n
	void start();

	/// �o�ߎ��Ԃ̎擾 (ns)
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
