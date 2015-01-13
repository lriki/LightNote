//==============================================================================
// AsyncProcessObject 
//------------------------------------------------------------------------------
///**
//  @file       AsyncProcessObject.h
//  @brief      AsyncProcessObject
//*/
//==============================================================================

#pragma once

#include "../Threading/EventFlag.h"
#include "Interface.h"

namespace LNote
{
namespace Core
{
namespace FileIO
{

//==============================================================================
// IAsyncLoadableObject
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class IAsyncLoadableObject
	: public Base::Interface
{
protected:
	IAsyncLoadableObject() {}
	virtual ~IAsyncLoadableObject() {}	// Manager に追加したとき参照カウントがインクリメントされるため、終了を待つ必要はない

public:

	/// 読み込み処理を実装する
	virtual void onLoadProc() = 0;

	/// 読み込み後の処理を実装する 
	/// (Manager::broadcastAsyncIOProcFinished() が呼ばれたスレッドで呼ばれる。
	///  失敗した場合も呼ばれる。この関数から(他のオブジェクトへの呼び出しも含めて) startLoadProc() を呼ぶことはできない)
	virtual void onLoadProcFinished() = 0;

	/// 非同期処理の状態を取得する
	virtual AsyncIOState getAsyncLoadState() = 0;

protected:	// internal
	friend class Manager;

	/// 非同期処理の状態を設定する
	virtual void setAsyncLoadState(AsyncIOState state) = 0;

};

//==============================================================================
// IAsyncSavableObject
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class IAsyncSavableObject
	: public Base::Interface
{
protected:
	IAsyncSavableObject() {}
	virtual ~IAsyncSavableObject() {}	// Manager に追加したとき参照カウントがインクリメントされるため、終了を待つ必要はない

public:

	/// 読み込み処理を実装する
	virtual void onSaveProc() = 0;

	/// 読み込み後の処理を実装する 
	/// (Manager::broadcastAsyncIOProcFinished() が呼ばれたスレッドで呼ばれる。
	///  失敗した場合も呼ばれる。この関数から(他のオブジェクトへの呼び出しも含めて) startLoadProc() を呼ぶことはできない)
	virtual void onSaveProcFinished() = 0;

	/// 非同期処理の状態を取得する
	virtual AsyncIOState getAsyncSaveState() = 0;

protected:	// internal
	friend class Manager;

	/// 非同期処理の状態を設定する
	virtual void setAsyncSaveState(AsyncIOState state) = 0;
};



//==============================================================================
// ■ AsyncLoadObject
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class AsyncLoadObject
	: public Base::ReferenceObject
{
protected:
	AsyncLoadObject( Manager* manager );
	virtual ~AsyncLoadObject();

public:

	/// FileIO::Manager の取得
	Manager* getIOManager() const { return mIOManager; }

	/// 非同期処理の状態を調べる
	AsyncIOState getAsyncIOState() const { return mAsyncIOState; }

protected:

	/// 読み込み開始
	void startLoadProc( bool async );

	/// 読み込み処理を実装する (失敗した場合は false を返す)
	virtual bool onLoadProc() = 0;

	/// 読み込み後の処理を実装する 
	/// (Manager::broadcastAsyncIOProcFinished() が呼ばれたスレッドで呼ばれる。
	///  失敗した場合も呼ばれる。この関数から(他のオブジェクトへの呼び出しも含めて) startLoadProc() を呼ぶことはできない)
	virtual void onLoadProcFinished() = 0;

private:
	void _doLoadProc();	// (Manager からも呼ばれる)
 
private:

	Manager*				mIOManager;				///< 管理クラス
	AsyncIOState			mAsyncIOState;			///< 状態
	Threading::EventFlag	mNotAsyncIOProccessing;	///< AsyncIOState_Processing 以外か

	friend class Manager;
};

//==============================================================================
// ■ AsyncSaveObject
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class AsyncSaveObject
	: public Base::ReferenceObject
{
protected:
	AsyncSaveObject( Manager* manager );
	virtual ~AsyncSaveObject();

public:

	/// FileIO::Manager の取得
	Manager* getIOManager() const { return mIOManager; }

	/// 非同期処理の状態を調べる
	AsyncIOState getAsyncIOState() const { return mAsyncIOState; }

protected:

	/// 書き込み開始
	void startSaveProc( bool async );

	/// 書き込み処理を実装する (失敗した場合は false を返す)
	virtual bool onSaveProc() = 0;

	/// 書き込み後の処理を実装する
	/// (Manager::broadcastAsyncIOProcFinished() が呼ばれたスレッドで呼ばれる。
	///  失敗した場合も呼ばれる。この関数から(他のオブジェクトへの呼び出しも含めて) startLoadProc() を呼ぶことはできない)
	virtual void onSaveProcFinished() = 0;

private:
	void _doSaveProc();	// (Manager からも呼ばれる)
 
private:

	Manager*				mIOManager;				///< 管理クラス
	AsyncIOState			mAsyncIOState;			///< 状態
	Threading::EventFlag	mNotAsyncIOProccessing;	///< AsyncIOState_Processing 以外か

	friend class Manager;
};

} // namespace FileIO
} // namespace Core
} // namespace LNote
