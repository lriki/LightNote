//==============================================================================
// Manager 
//------------------------------------------------------------------------------
///**
//  @file       Manager.h
//  @brief      Manager
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Math/Matrix.h"
#include "Common.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Joypad.h"
#include "Touch.h"
#include "GameController.h"

namespace LNote
{
namespace Core
{
namespace Input
{

//==============================================================================
// ■ Manager
//------------------------------------------------------------------------------
///**
//  @brief      入力モジュールの管理クラス
//*/
//==============================================================================
class Manager
    : public Base::ReferenceObject
	, public System::IEventListener
{
public:
	static const int EventListenerPriority = 10000;

public:

	struct ConfigData
	{
        System::Window*		Window;		        ///< DirectInput の初期化に使うウィンドウハンドル
	};

public:
	Manager();
	virtual ~Manager();

public:

	/// 初期化
    void initialize( const ConfigData& configData );

	/// 終了処理
    void finalize();

	/// フレーム更新
	void update();

	/// InputDeviceBase 取得
	InputDeviceBase* getInputDevice() { return mInputDevice; }

	/// GameController へのポインタの取得
    GameController* getGameController( int index ) { return mGameControllers[ index ]; }

	/// Mouse へのポインタの取得
	Mouse* getMouse() { return mMouse; }

	/// Keyboard へのポインタの取得
	Keyboard* getKeyboard() { return mKeyboard; }

	/// Joypad へのポインタの取得
	Joypad* getJoypad( int index_ );
    
	/// Touch へのポインタの取得
	Touch* getTouch() { return mTouch; }

	/// 接続されているジョイパッドの数の取得
	int getJoypadNum() { return mInputDevice->getJoypadNum(); }

   

    void setMouseTransform( const LMatrix& transform ) { mMouseTransform = transform; }

public:

	/// マウス位置の変換行列の取得
    const LMatrix& getMouseTransform() const { return mMouseTransform; }

	/// マウス位置が設定できるか
    bool canSetMousePoint() const;

public:
	virtual bool onEvent( const System::EventArgs& e );

private:

	System::Window*				mMainWindow;
	InputDeviceBase*	        mInputDevice;
    Mouse*			            mMouse;
	Keyboard*		            mKeyboard;
	Joypad*			            mJoypads[ LN_MAX_JOYPADS ];
    Touch*                      mTouch;
	GameController*	            mGameControllers[ LN_MAX_GAME_CONTROLLERS ];
	LMatrix                     mMouseTransform;
    bool                        mCanSetMousePoint;

};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Input
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================