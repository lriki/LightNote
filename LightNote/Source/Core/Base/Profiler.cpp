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
		// �󂢂Ă���Ƃ���փZ�b�g
		for (int iThread = 0; iThread < MaxThreads; ++iThread)
		{
			if (mThreadEntries[iThread].ThreadID == threadID)
			{
				return;	// �o�^�ς�
			}
			if (mThreadEntries[iThread].ThreadID == NULL)
			{
				mThreadEntries[iThread].ThreadID = threadID;
				mThreadEntries[iThread].Name = name;
				mRegisterdThreadCount++;
				return;
			}
		}
		// ������t
		LN_THROW_InvalidOperation(0);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Profiler::setBaseFrameRate(lnThreadID threadID, lnFloat frameRate)
	{
		findThreadEntry(threadID).LimitElapsedTime = (1.0 / frameRate) * 1000 * 1000;	// us �P��
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Profiler::registerSection(lnThreadID threadID, int sectionID, const lnChar* sectionName)
	{
		// �ŏ��̐ݒ�̂�
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

		// �Ƃ肠����ms�P�ʂŁB
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
		// ������Ȃ�����
		LN_THROW_InvalidOperation(0);
	}

} // namespace Base
} // namespace Core
} // namespace LNote
