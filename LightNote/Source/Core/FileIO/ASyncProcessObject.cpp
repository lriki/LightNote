//==============================================================================
// AsyncProcessObject 
//==============================================================================

#include "stdafx.h"
#include "Manager.h"
#include "AsyncProcessObject.h"

namespace LNote
{
namespace Core
{
namespace FileIO
{

//==============================================================================
// ■ AsyncLoadObject
//==============================================================================

	//--------------------------------------------------------------------------
	//
	//--------------------------------------------------------------------------
	AsyncLoadObject::AsyncLoadObject( Manager* manager )
		: mIOManager				( manager )
		, mAsyncIOState				( AsyncIOState_Idle )
		, mNotAsyncIOProccessing	( true )
	{
		LN_SAFE_ADDREF( mIOManager );
	}

	//--------------------------------------------------------------------------
	//
	//--------------------------------------------------------------------------
	AsyncLoadObject::~AsyncLoadObject()
	{
		/* 
		if ( mIOManager )
		{
			// 読み込みリクエストから外す
			mIOManager->removeAsyncLoadObject( this );
		}

		// 非同期処理中の場合は完了するまで待つ
		mNotAsyncIOProccessing.wait();
		*/

		LN_SAFE_RELEASE( mIOManager );
	}

	//--------------------------------------------------------------------------
	//
	//--------------------------------------------------------------------------
	void AsyncLoadObject::startLoadProc( bool async )
	{
		if ( async ) {
			if ( mAsyncIOState == AsyncIOState_Idle )
			{
				mIOManager->requestAsyncLoadObject( this );
			}
		}
		else {
			onLoadProc();
			onLoadProcFinished();
		}
	}

	//--------------------------------------------------------------------------
	//
	//--------------------------------------------------------------------------
	void AsyncLoadObject::_doLoadProc()
	{
		if ( onLoadProc() ) {
			mAsyncIOState = AsyncIOState_Completed;
		}
		else {
			mAsyncIOState = AsyncIOState_Failed;
		}
	}


//==============================================================================
// ■ AsyncSaveObject
//==============================================================================

	//--------------------------------------------------------------------------
	//
	//--------------------------------------------------------------------------
	AsyncSaveObject::AsyncSaveObject( Manager* manager )
		: mIOManager				( manager )
		, mAsyncIOState				( AsyncIOState_Idle )
		, mNotAsyncIOProccessing	( false )
	{
		LN_SAFE_ADDREF( mIOManager );
	}

	//--------------------------------------------------------------------------
	//
	//--------------------------------------------------------------------------
	AsyncSaveObject::~AsyncSaveObject()
	{
		/*
		if ( mIOManager )
		{
			// 読み込みリクエストから外す
			mIOManager->removeAsyncSaveObject( this );
		}

		// 非同期処理中の場合は完了するまで待つ
		mNotAsyncIOProccessing.wait();
		*/

		LN_SAFE_RELEASE( mIOManager );
	}

	//--------------------------------------------------------------------------
	//
	//--------------------------------------------------------------------------
	void AsyncSaveObject::startSaveProc( bool async )
	{
		if ( async ) {
			if ( mAsyncIOState == AsyncIOState_Idle )
			{
				mIOManager->requestAsyncSaveObject( this );
			}
		}
		else {
			onSaveProc();
			onSaveProcFinished();
		}
	}

	//--------------------------------------------------------------------------
	//
	//--------------------------------------------------------------------------
	void AsyncSaveObject::_doSaveProc()
	{
		mAsyncIOState = AsyncIOState_Processing;
		if ( onSaveProc() ) {
			mAsyncIOState = AsyncIOState_Completed;
		}
		else {
			mAsyncIOState = AsyncIOState_Failed;
		}
	}

} // namespace FileIO
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================