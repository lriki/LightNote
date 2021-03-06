//==============================================================================
// Profiler 
//==============================================================================

#include "stdafx.h"
#include "Profiler.h"
#include "../System/Environment.h"

namespace LNote
{
namespace Core
{
namespace Base
{

//==============================================================================
// Profiler
//==============================================================================

	Profiler		Profiler::Instance;
	const lnChar*	Profiler::RenderThread_Name = _T("Rendering");

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Profiler::Profiler()
		: mRegisterdThreadCount(0)
		, mNextColorIndex(0)
		, mEnabled(false)
	{

		MainFPS = 0.0f;
		MainFPSCapacity = 0.0;

		for (int iThread = 0; iThread < MaxThreads; ++iThread)
		{
			mThreadEntries[iThread].ThreadID = NULL;

			for (int iSection = 0; iSection < MaxSections; ++iSection)
			{
				mThreadEntries[iThread].Sections[iSection].ElapsedTime = 0.0f;
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Profiler::registerThread(lnThreadID threadID, const lnChar* name)
	{
		// 空いているところへセット
		for (int iThread = 0; iThread < MaxThreads; ++iThread)
		{
			if (mThreadEntries[iThread].ThreadID == threadID)
			{
				return;	// 登録済み
			}
			if (mThreadEntries[iThread].ThreadID == NULL)
			{
				mThreadEntries[iThread].ThreadID = threadID;
				mThreadEntries[iThread].Name = name;
				mRegisterdThreadCount++;
				return;
			}
		}
		// もう一杯
		LN_THROW_InvalidOperation(0);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Profiler::setBaseFrameRate(lnThreadID threadID, lnFloat frameRate)
	{
		findThreadEntry(threadID).LimitElapsedTime = (1.0 / frameRate) * 1000 * 1000;	// us 単位
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Profiler::registerSection(lnThreadID threadID, int sectionID, const lnChar* sectionName)
	{
		// 最初の設定のみ
		if (findThreadEntry(threadID).Sections[sectionID].Name.empty())
		{
			findThreadEntry(threadID).Sections[sectionID].Name = sectionName; 
			findThreadEntry(threadID).Sections[sectionID].ColorIndex = mNextColorIndex;
			mNextColorIndex++;
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Profiler::startSection(lnThreadID threadID, int sectionID)
	{
		if (!mEnabled) return;

		findThreadEntry(threadID).Sections[sectionID].Stopwatch.start();

		// とりあえずms単位で。
		//findThreadEntry(threadID).Sections[sectionID].StartTime = System::Environment::getSystemTime();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Profiler::endSection(lnThreadID threadID, int sectionID)
	{
		if (!mEnabled) return;

		Section& section = findThreadEntry(threadID).Sections[sectionID];
		section.ElapsedTime = section.Stopwatch.getElapsedTime() / 1000;///System::Environment::getSystemTime() - section.StartTime;
		//section.ElapsedTime *= 1000;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Profiler::ThreadEntry& Profiler::findThreadEntry(lnThreadID threadID)
	{
		for (int iThread = 0; iThread < MaxThreads; ++iThread)
		{
			if (mThreadEntries[iThread].ThreadID == threadID)
			{
				return mThreadEntries[iThread];
			}
		}
		// 見つからなかった
		LN_THROW_InvalidOperation(0);
	}

} // namespace Base
} // namespace Core
} // namespace LNote
