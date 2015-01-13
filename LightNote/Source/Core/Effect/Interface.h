//==============================================================================
// Interface 
//------------------------------------------------------------------------------
///**
//  @file       Interface.h
//  @brief      Interface
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../System/Interface.h"
#include "../Math/Matrix.h"

namespace LNote
{
namespace Core
{
namespace Effect
{

//==============================================================================
// Effect
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class Effect
	: public Base::ReferenceObject
{
public:
	virtual ~Effect() {};

public:

	/// エフェクトの座標変換行列を設定する
	virtual void setMatrix( const LMatrix& matrix ) = 0;

	/// エフェクトを再生する
	///		overlap=true  : 再生中のエフェクトを停止せずに新しいエフェクトの再生を開始する
	///		overlap=false : 再生中のエフェクトを停止してから新しいエフェクトを再生する
	///		古いエフェクトは、以降座標変換を更新しない。
	virtual void play( bool overlap ) = 0;

	/// エフェクトを停止する
	virtual void stop() = 0;

	/// 再生中かを確認
	virtual bool isPlaying() = 0;

	/// 時間を進める
	///		時間単位は Manager に設定したフレームレートによる。
	///		時間を進めるだけで、実際の更新処理は別スレッドで行われる。
	virtual void advanceTime( lnFloat deltaTime ) = 0;

	/// エフェクトを描画する
	virtual void draw() = 0;
};

#if 0

//==============================================================================
// ■ IEffect
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class IEffect
    : public Base::Interface
{
public:

	/// エフェクトの座標変換行列を設定する
	virtual void setMatrix( const LMatrix& matrix ) = 0;

	/// エフェクトを再生する
	///		overlap=true  : 再生中のエフェクトを停止せずに新しいエフェクトの再生を開始する
	///		overlap=false : 再生中のエフェクトを停止してから新しいエフェクトを再生する
	///		古いエフェクトは、以降座標変換を更新しない。
	virtual void play( bool overlap ) = 0;

	/// エフェクトを停止する
	virtual void stop() = 0;

	/// 再生中かを確認
	virtual bool isPlaying() = 0;

	/// 時間を進める
	///		時間単位は Manager に設定したフレームレートによる。
	///		時間を進めるだけで、実際の更新処理は別スレッドで行われる。
	virtual void advanceTime( lnFloat deltaTime ) = 0;

	/// エフェクトを描画する
	virtual void draw() = 0;

protected:
    virtual ~IEffect() {};
};
#endif

} // namespace Effect
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================