//==============================================================================
// GameTime 
//------------------------------------------------------------------------------
/**
//  @file       GameTime.h
//  @brief      GameTime
//*/
//==============================================================================

#pragma once

namespace LNote
{
namespace Core
{
namespace Game
{

/// ���Ԋ
enum TimeBaseType
{
	TimeBaseType_GameTime,
	TimeBaseType_RealTime,
};

//==============================================================================
// GameTime
//------------------------------------------------------------------------------
///**
//  @brief      �Q�[�����ł̎��Ԃ������N���X
//*/
//==============================================================================
class GameTime
{
public:

	static const GameTime ZERO_TIME;

public:

	//----------------------------------------------------------------------
	///**
	//  @brief      ���݂� FPS �l�̎擾
	//*/
	//----------------------------------------------------------------------
	float getFPS() const { return _m.FPS; }

	//----------------------------------------------------------------------
	///**
	//  @brief      �O��̍X�V����o�߂����Q�[�����Ԃ̎擾
	//*/
	//----------------------------------------------------------------------
	//float getElapsedGameTime() const { return _m.ElapsedGameTime; }

	//----------------------------------------------------------------------
	///**
	//  @brief      �O��̍X�V����o�߂��������Ԃ̎擾
	//*/
	//----------------------------------------------------------------------
	//float getElapsedRealTime() const { return _m.ElapsedRealTime; }

	//----------------------------------------------------------------------
	///**
	//  @brief      �Q�[���̊J�n�ȍ~�̃Q�[�����Ԃ̎擾
	//*/
	//----------------------------------------------------------------------
	double getTotalGameTime() const { return _m.TotalGameTime; }

	//----------------------------------------------------------------------
	///**
	//  @brief      �Q�[���̊J�n�ȍ~�̎����Ԃ̎擾
	//*/
	//----------------------------------------------------------------------
	double getTotalRealTime() const { return _m.TotalRealTime; }

	//----------------------------------------------------------------------
	///**
	//  @brief      �Q�[���̊J�n�ȍ~�̌o�߃t���[�����̎擾
	//*/
	//----------------------------------------------------------------------
	lnU32 getTotalFrameCount() const { return _m.TotalFrameCount; }

	//----------------------------------------------------------------------
	///**
	//  @brief      �Q�[�����[�v�̎��Ԃ��t���[�����[�g�𒴂��Ă��邩�𔻒肷��
	//*/
	//----------------------------------------------------------------------
	bool isRunningSlowly() const { return _m.IsRunningSlowly; }

	double getTotalTime() const 
	{ 
		if (mTimeBaseType == TimeBaseType_GameTime) {
			return _m.TotalGameTime;
		}
		else {
			return _m.TotalRealTime;
		}
	}

	float getElapsedTime() const
	{
		if (mTimeBaseType == TimeBaseType_GameTime) {
			return _m.ElapsedGameTime;
		}
		else {
			return _m.ElapsedRealTime;
		}
	}

public:

	/// �R���X�g���N�^
    GameTime()
    {
        _m.FPS = 0;
        _m.ElapsedGameTime = 0;
        _m.ElapsedRealTime = 0;
        _m.TotalGameTime   = 0;
        _m.TotalRealTime   = 0;
        _m.IsRunningSlowly = false;
		mTimeBaseType = TimeBaseType_GameTime;
    }

private:

    struct
    {
        float   FPS;
        float   ElapsedGameTime;
        float   ElapsedRealTime;
        double  TotalGameTime;
        double  TotalRealTime;
        lnU32   TotalFrameCount;
        bool    IsRunningSlowly;
    } _m;

	TimeBaseType	mTimeBaseType;

    friend class FPSController;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Game
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================