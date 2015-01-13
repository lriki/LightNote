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

/// 時間基準
enum TimeBaseType
{
	TimeBaseType_GameTime,
	TimeBaseType_RealTime,
};

//==============================================================================
// GameTime
//------------------------------------------------------------------------------
///**
//  @brief      ゲーム内での時間を扱うクラス
//*/
//==============================================================================
class GameTime
{
public:

	static const GameTime ZERO_TIME;

public:

	//----------------------------------------------------------------------
	///**
	//  @brief      現在の FPS 値の取得
	//*/
	//----------------------------------------------------------------------
	float getFPS() const { return _m.FPS; }

	//----------------------------------------------------------------------
	///**
	//  @brief      前回の更新から経過したゲーム時間の取得
	//*/
	//----------------------------------------------------------------------
	//float getElapsedGameTime() const { return _m.ElapsedGameTime; }

	//----------------------------------------------------------------------
	///**
	//  @brief      前回の更新から経過した実時間の取得
	//*/
	//----------------------------------------------------------------------
	//float getElapsedRealTime() const { return _m.ElapsedRealTime; }

	//----------------------------------------------------------------------
	///**
	//  @brief      ゲームの開始以降のゲーム時間の取得
	//*/
	//----------------------------------------------------------------------
	double getTotalGameTime() const { return _m.TotalGameTime; }

	//----------------------------------------------------------------------
	///**
	//  @brief      ゲームの開始以降の実時間の取得
	//*/
	//----------------------------------------------------------------------
	double getTotalRealTime() const { return _m.TotalRealTime; }

	//----------------------------------------------------------------------
	///**
	//  @brief      ゲームの開始以降の経過フレーム数の取得
	//*/
	//----------------------------------------------------------------------
	lnU32 getTotalFrameCount() const { return _m.TotalFrameCount; }

	//----------------------------------------------------------------------
	///**
	//  @brief      ゲームループの時間がフレームレートを超えているかを判定する
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

	/// コンストラクタ
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