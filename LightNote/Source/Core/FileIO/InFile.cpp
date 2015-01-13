//==============================================================================
// InFile 
//==============================================================================

#include "stdafx.h"
#include "Manager.h"
#include "InFile.h"

namespace LNote
{
namespace Core
{
namespace FileIO
{

//==============================================================================
// ■ InFile
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	InFile::InFile( Manager* manager, Stream* stream )
		: AsyncLoadObject	( manager )
		, mInStream				( stream )
        , mData                 ( NULL )
        , mLoadFinished         ( false )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	InFile::~InFile()
	{
		// 完了を待ってからストリームを解放する
        LN_SAFE_RELEASE( mInStream );
        SAFE_DELETE_ARRAY( mData );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	int InFile::read( void* buffer, int buffer_size, int read_size )
	{
		if ( read_size < 0 )
		{
			read_size = buffer_size;
		}
		if ( read_size > buffer_size )
		{
			read_size = buffer_size;
		}

        // 読み込んで、読み込めたバイト数を read_size に格納
        int validSize = mInStream->read( buffer, buffer_size, read_size );

		// 念のためファイルポインタを先頭に戻しておく
        //mInStream->seek( 0 );
		//fseek( mInStream->Stream, 0, SEEK_SET );

		return validSize;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void InFile::load( bool async )
	{
		startLoadProc( async );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool InFile::onLoadProc()
	{
		if ( mData == NULL )
		{
            int size = mInStream->getSize();
            lnByte* data = LN_NEW lnByte[size + 1];
            int read_size = mInStream->read( data, size );
            data[read_size] = 0;	// 念のための終端 NULL

            mData = data;
            mLoadFinished = true;
		}
		return true;
	}

} // namespace FileIO
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================