//==============================================================================
// StreamObject 
//==============================================================================

#include "stdafx.h"
#include "../Base/Unicode.h"
#include "FileUtil.h"
#include "StreamObject.h"

namespace LNote
{
namespace Core
{
namespace FileIO
{

//==============================================================================
// ■ InFileStream
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    InFileStream::InFileStream()
        : mStream   ( NULL )
        , mSize     ( 0 )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    InFileStream::~InFileStream()
    {
        _close();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void InFileStream::open(const char* filePath, FileType type)
    {
        _close();

		mStream = fopen(filePath, (type == FileType_Binary) ? "rb" : "r");
        LN_THROW_FileNotFound( mStream, filePath );

		mSize = FileUtils::getFileSize(mStream);
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void InFileStream::open(const wchar_t* filePath, FileType type)
    {
        _close();

		mStream = Base::ln_wfopen(filePath, (type == FileType_Binary) ? L"rb" : L"r");

        LN_THROW_FileNotFound( mStream, filePath );

		mSize = FileUtils::getFileSize(mStream);
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    int InFileStream::read( void* buffer, int bufferSize, int readSize )
    {
        if ( readSize < 0 ) 
			readSize = readSize;

        return fread( buffer, 1, bufferSize, mStream );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void InFileStream::seek( int offset_, int origin_ = SEEK_SET )
    {
        LN_ERR2_ASSERT_S( fseek( mStream, offset_, origin_ ) == 0 );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void InFileStream::_close()
    {
        if ( mStream )
        {
            fclose( mStream );
			mStream = NULL;
            mSize = 0;
        }
    }

//==============================================================================
// ■ InManagedMemoryStream
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    InManagedMemoryStream::InManagedMemoryStream()
        : mSize         ( 0 )
        , mBuffer       ( NULL )
        , mSeekPoint    ( 0 )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    InManagedMemoryStream::~InManagedMemoryStream()
    {
        _dispose();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void InManagedMemoryStream::create( const void* buffer, int size )
    {
        _dispose();

        mSize = size;
        mBuffer = LN_NEW lnByte[mSize];
        memcpy( mBuffer, buffer, mSize );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    int InManagedMemoryStream::read( void* buffer, int buffer_size, int read_size )
    {
        if ( read_size < 0 )
		{
			read_size = buffer_size;
		}
        if ( mSeekPoint + read_size > mSize )
        {
            read_size = mSize - mSeekPoint;
        }

        memcpy( buffer, &mBuffer[mSeekPoint], read_size );

        mSeekPoint += read_size;

        return read_size;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void InManagedMemoryStream::seek( int offset, int origin )
    {
		mSeekPoint = FileUtils::setSeekPoint(mSeekPoint, mSize, offset, origin);
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void InManagedMemoryStream::_dispose()
    {
        SAFE_DELETE_ARRAY( mBuffer );
        mSize = 0;
        mSeekPoint = 0;
    }

//==============================================================================
// ■ InUnManagedMemoryStream
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    InUnManagedMemoryStream::InUnManagedMemoryStream()
        : mSize         ( 0 )
        , mBuffer       ( NULL )
        , mSeekPoint    ( 0 )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    InUnManagedMemoryStream::~InUnManagedMemoryStream()
    {
        mSize = 0;
        mSeekPoint = 0;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void InUnManagedMemoryStream::create( const void* buffer, int size )
    {
        mSize = size;
        mBuffer = static_cast<const lnByte*>( buffer );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    int InUnManagedMemoryStream::read( void* buffer, int buffer_size, int read_size )
    {
        if ( read_size < 0 )
		{
			read_size = buffer_size;
		}
        if ( mSeekPoint + read_size > mSize )
        {
            read_size = mSize - mSeekPoint;
        }

        memcpy( buffer, &mBuffer[mSeekPoint], read_size );

        mSeekPoint += read_size;

        return read_size;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void InUnManagedMemoryStream::seek( int offset, int origin )
    {
		mSeekPoint = FileUtils::setSeekPoint(mSeekPoint, mSize, offset, origin);
    }

} // namespace FileIO
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================