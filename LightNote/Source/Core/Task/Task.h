//=============================================================================
//�y Task �z
//-----------------------------------------------------------------------------
///**
//  @file       Task.h
//  @brief      Task
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Base/Delegate.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Task
{

//=============================================================================
// �� TaskBase �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class TaskBase
{
public:

    TaskBase( int phase_ )
        : mTaskExecutePhase( phase_ )
    {}

    virtual ~TaskBase() {}

public:

	/// ���s
    virtual void execute() = 0;

protected:

    int mTaskExecutePhase;      ///< (�}�X�^�[�^�X�N�̏ꍇ�͑ҋ@����t�F�[�Y)

    friend class Manager;
};

//=============================================================================
// �� DelegateTask �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class DelegateTask
    : public TaskBase
{
public:

    DelegateTask( Base::Delegate00 method_, int phase_ )
        : TaskBase  ( phase_ )
        , mMethod   ( method_ )
    {}

    virtual ~DelegateTask() {}

public:

	/// ���s
    virtual void execute() { mMethod.call(); }

private:

    Base::Delegate00   mMethod;
};

#if 0
//=============================================================================
// �� MasterTask �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class MasterTask
    : public TaskBase
{
public:

    MasterTask( Base::Delegate00& method_, int sync_phase_ )
        : TaskBase  ( sync_phase_ )
        , mMethod   ( method_ )
        , mWaiting  ( false )
        , mStartRequested( false )
    {}

    virtual ~MasterTask() {}

public:

	/// ���s (�������[�v)
    virtual void execute();

    void startRequest() { mStartRequested.setTrue(); mWaiting.setFalse(); }

    void waitForTaskWait() { mWaiting.wait(); }

    void endRequest() { mEndRequested.setFalse(); }


private:

    Base::Delegate00&   mMethod;
    Threading::EventFlag   mWaiting;
    Threading::EventFlag   mStartRequested;
    Threading::EventFlag   mEndRequested;
};
#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Task
} // namespace Core
} // namespace LNote

//=============================================================================
//
//=============================================================================