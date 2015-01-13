//==============================================================================
// Profiler 
//------------------------------------------------------------------------------
///**
//  @file       Profiler.h
//  @brief      Profiler
//*/
//==============================================================================

#pragma once

#include "../Threading/Thread.h"
#include "../System/Stopwatch.h"
#include "../Geometry/DrawingShapes.h"

namespace LNote
{
namespace Core
{
namespace Base
{

//==============================================================================
// Profiler
//------------------------------------------------------------------------------
///**
//  @brief      パフォーマンス計測
//*/
//==============================================================================
class Profiler
{
public:
	static Profiler	Instance;		///< グローバルインスタンス

	static const int		MainThread_ID = 1;
	static const int		RenderThread_ID = 2;

	static const int		MainThread_UpdateSection = 0;
	static const int		MainThread_PrepareRendering = 1;

	static const lnChar*	RenderThread_Name;
	static const int		RenderThread_Section0 = 0;
	static const int		RenderThread_Section_Present = 1;
	///static const int		RenderThread_Section_PrepareRendering = 2;



public:
	static const int MaxThreads		= 32;
	static const int MaxSections	= 32;

	struct Section
	{
		lnRefString	Name;					///< セクション名
		int			ColorIndex;
		//lnU32		StartTime;				///< 開始時間
		System::Stopwatch	Stopwatch;		
		lnU64		ElapsedTime;			///< 経過時間 (とりあえず us 単位)
	};

	struct ThreadEntry
	{
		lnRefString	Name;					///< スレッド名
		lnThreadID	ThreadID;
		Section		Sections[MaxSections];
		lnFloat		LimitElapsedTime;		///< 許容時間 (us 単位。フレームレートの逆数)
	};

public:
	float	MainFPS;
	float	MainFPSCapacity;
	LSize	WindowSize;
	LSize	BackbufferSize;

public:
	Profiler();

public:

	/// 指定IDのスレッドを測定対象として登録し、名前を付ける (32個まで。他の測定関数を呼び出す前に登録すること)
	void registerThread(lnThreadID threadID, const lnChar* name);

	/// スレッドに基準フレームレートを設定
	void setBaseFrameRate(lnThreadID threadID, lnFloat frameRate);

	/// 計測区間の名前を設定する(sectionID は使う側が自由に決められるが、0～31 まで) 
	void registerSection(lnThreadID threadID, int sectionID, const lnChar* sectionName);

	/// 計測区間の開始 (区間のネストはできない)
	void startSection(lnThreadID threadID, int sectionID);

	/// 計測区間の終了
	void endSection(lnThreadID threadID, int sectionID);

	/// 計測の有効設定
	void setEnabled(bool enabled) { mEnabled = enabled; }

	/// 計測の有効設定
	bool isEnabled() const { return mEnabled; }

public:

	/// 登録済みスレッド数
	int getRegisterdThreadCount() const { return mRegisterdThreadCount; }

	/// 登録済みスレッドの取得
	const ThreadEntry& getRegisterdThread(int index) { return mThreadEntries[index]; }

private:
	ThreadEntry& findThreadEntry(lnThreadID threadID);

private:
	ThreadEntry	mThreadEntries[MaxThreads];
	int			mRegisterdThreadCount;
	int			mNextColorIndex;
	bool		mEnabled;
};

//==============================================================================
// ScopedProfilerSection
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class ScopedProfilerSection
{
public:
	ScopedProfilerSection(Profiler& profiler, lnThreadID threadID, int sectionID)
		: mProfiler	(profiler)
		, mThreadID(threadID)
		, mSectionID(sectionID)
	{
		mProfiler.startSection(mThreadID, mSectionID);
	}

	~ScopedProfilerSection()
	{
		mProfiler.endSection(mThreadID, mSectionID);
	}

private:
	Profiler&	mProfiler;
	lnThreadID	mThreadID;
	int			mSectionID;
};

} // namespace Base
} // namespace Core
} // namespace LNote
