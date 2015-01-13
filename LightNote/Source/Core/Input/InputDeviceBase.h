//==============================================================================
// InputDeviceBase 
//------------------------------------------------------------------------------
///**
//  @file       InputDeviceBase.h
//  @brief      InputDeviceBase
//  @version    1.0
//*/
//==============================================================================

#pragma once

#include <deque>
#include "Common.h"

namespace LNote
{
namespace Core
{
namespace Input
{

//==============================================================================
// ■ InputDeviceBase
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class InputDeviceBase
    : public Base::ReferenceObject
{
public:
	InputDeviceBase();
	virtual ~InputDeviceBase();

public:

	/// 接続済みのジョイパッド数の取得
	virtual int getJoypadNum() = 0;

	/// マウスの状態の取得
	virtual const LNMouseDeviceState& getMouseState() = 0;

	/// キーボードの状態の取得 (キーが押されているか)
	virtual bool getKeyState( LNVirtualKeyCode vkey );

	/// キーボードのバッファリングデータの取得 (LN_KEY_UNKNOWN を返すまで while で回して取得)
    virtual LNVirtualKeyCode getKeyBufferState();

	/// ジョイパッドの状態の取得
	virtual void getJoypadState( int joypadNumber, LNJoypadDeviceState* state ) = 0;

	/// ジョイパッドの振動を開始する (power の範囲は 0～1000。time はミリ秒)
	virtual void startVibration( int joypadNumber, int power, int time ) = 0;

	/// ジョイパッドの振動を停止する
	virtual void stopVibration( int joypadNumber ) = 0;
    
	/// タッチ情報の取得
    virtual void getTouchState( int touchID, LNTouchDeviceState* state ) = 0;

	//----------------------------------------------------------------------
	///**
	//  @brief      仮想ボタンの状態の取得
	//  @param[in]  button			: 調べる仮想ボタン (仮想キーコードまたは LNVirtualDeviceButton)
	//  @param[in]  keyboard		: true の場合、キーボードの状態を考慮する
	//  @param[in]  mouse			: true の場合、マウスの状態を考慮する
	//  @param[in]  joypadNumber	: 0 以上 の場合、その番号のジョイパッドの状態を考慮する
	//  @return
	//              0.0～1.0 の値
	//*/
	//----------------------------------------------------------------------
	virtual float getVirtualButtonState( lnU32 button, bool keyboard, bool mouse, int joypadNumber ) = 0;

	//----------------------------------------------------------------------
	///**
	//  @brief      マウス座標の設定
	//  @par
	//              座標はクライアント領域の左上を原点 (0, 0) とした値で設定します。
	//              ウィンドウがアクティブではない場合はなにもしません。
	//*/
	//----------------------------------------------------------------------
	virtual void setMousePoint( const LVector2& point ) = 0;

	/// マウスカーソルの位置をクライアント領域の中央に設定する
    virtual void setMouseCursorCenter() = 0;

	/// 入力情報の更新
	virtual void update() = 0;

	/// デバイスの再セットアップ (USB 機器の接続検知時などに呼び出す)
	virtual void refresh() = 0;

	/// 関連付けられている IWindow の取得
    virtual System::Window* getWindow() = 0;
    
public:

	/// イベント処理 (Manager::onEvent() から呼ばれる)
    virtual bool onEvent( const System::EventArgs& e );
	
private:
    typedef std::deque< LNVirtualKeyCode >     KeyBuffer;

private:
    bool                        mKeyStatus[LN_MAX_KEYS];
    KeyBuffer                   mKeyBuffer;
	bool						mEnableKeyBuffering;
};

} // namespace Input
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================