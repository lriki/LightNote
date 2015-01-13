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
//  @brief      �p�t�H�[�}���X�v��
//*/
//==============================================================================
class Profiler
{
public:
	static Profiler	Instance;		///< �O���[�o���C���X�^���X

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
		lnRefString	Name;					///< �Z�N�V������
		int			ColorIndex;
		//lnU32		StartTime;				///< �J�n����
		System::Stopwatch	Stopwatch;		
		lnU64		ElapsedTime;			///< �o�ߎ��� (�Ƃ肠���� us �P��)
	};

	struct ThreadEntry
	{
		lnRefString	Name;					///< �X���b�h��
		lnThreadID	ThreadID;
		Section		Sections[MaxSections];
		lnFloat		LimitElapsedTime;		///< ���e���� (us �P�ʁB�t���[�����[�g�̋t��)
	};

public:
	float	MainFPS;
	float	MainFPSCapacity;
	LSize	WindowSize;
	LSize	BackbufferSize;

public:
	Profiler();

public:

	/// �w��ID�̃X���b�h�𑪒�ΏۂƂ��ēo�^���A���O��t���� (32�܂ŁB���̑���֐����Ăяo���O�ɓo�^���邱��)
	void registerThread(lnThreadID threadID, const lnChar* name);

	/// �X���b�h�Ɋ�t���[�����[�g��ݒ�
	void setBaseFrameRate(lnThreadID threadID, lnFloat frameRate);

	/// �v����Ԃ̖��O��ݒ肷��(sectionID �͎g���������R�Ɍ��߂��邪�A0�`31 �܂�) 
	void registerSection(lnThreadID threadID, int sectionID, const lnChar* sectionName);

	/// �v����Ԃ̊J�n (��Ԃ̃l�X�g�͂ł��Ȃ�)
	void startSection(lnThreadID threadID, int sectionID);

	/// �v����Ԃ̏I��
	void endSection(lnThreadID threadID, int sectionID);

	/// �v���̗L���ݒ�
	void setEnabled(bool enabled) { mEnabled = enabled; }

	/// �v���̗L���ݒ�
	bool isEnabled() const { return mEnabled; }

public:

	/// �o�^�ς݃X���b�h��
	int getRegisterdThreadCount() const { return mRegisterdThreadCount; }

	/// �o�^�ς݃X���b�h�̎擾
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
