//==============================================================================
// BinaryReader 
//==============================================================================

#include "stdafx.h"
#include "BinaryReader.h"

namespace LNote
{
namespace Core
{
namespace FileIO
{

//==============================================================================
// ■ BinaryReader
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	BinaryReader::BinaryReader( Stream* stream )
		: mStream( stream )
	{
		LN_SAFE_ADDREF( mStream );
		mStream->seek( 0, SEEK_SET );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	BinaryReader::~BinaryReader()
	{
		LN_SAFE_RELEASE( mStream );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void BinaryReader::seek( int offset )
	{
		mStream->seek( offset, SEEK_CUR );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	int BinaryReader::getPosition()
	{
		return mStream->getPosition();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool BinaryReader::isEOF() const
	{
		return (mStream->getPosition() >= mStream->getSize());
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	int BinaryReader::read( void* buffer, int count )
	{
		return mStream->read( buffer, count );
		//TODO:EndOfStreamException
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    lnS32 BinaryReader::readInt( int byteCount )
	{
		switch ( byteCount )
		{
		case 1: return readInt8();
		case 2: return readInt16();
		case 4: return readInt32();
		}
		//TODO:Exception
		return 0;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    lnS8 BinaryReader::readInt8()
	{
		lnByte buffer;
		mStream->read( &buffer, 1 );
		return (lnS8)buffer;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    lnS16 BinaryReader::readInt16()
	{
		lnByte buffer[ 2 ];
		mStream->read( buffer, 2 );
		//TODO:EndOfStreamException

		lnS16 r = buffer[ 0 ];
		r |= ( buffer[ 1 ] << 8 );
		return r;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	lnS32 BinaryReader::readInt32()
	{
		lnByte buffer[ 4 ];
		mStream->read( buffer, 4, 4 );
		//TODO:EndOfStreamException

		lnS32 r = buffer[ 0 ];
		r |= ( buffer[ 1 ] << 8 );
		r |= ( buffer[ 2 ] << 16 );
		r |= ( buffer[ 3 ] << 24 );
		return r;
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    lnU32 BinaryReader::readUInt( int byteCount )
	{
		switch ( byteCount )
		{
		case 1: return readUInt8();
		case 2: return readUInt16();
		case 4: return readUInt32();
		}
		//TODO:Exception
		return 0;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    lnU8 BinaryReader::readUInt8()
	{
		lnByte buffer;
		mStream->read( &buffer, 1 );
		return buffer;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    lnU16 BinaryReader::readUInt16()
	{
		lnByte buffer[ 2 ];
		mStream->read( buffer, 2 );
		//TODO:EndOfStreamException

		lnU16 r = buffer[ 0 ];
		r |= ( buffer[ 1 ] << 8 );
		return r;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    lnU32 BinaryReader::readUInt32()
	{
		lnByte buffer[ 4 ];
		mStream->read( buffer, 4, 4 );
		//TODO:EndOfStreamException

		lnU32 r = buffer[ 0 ];
		r |= ( buffer[ 1 ] << 8 );
		r |= ( buffer[ 2 ] << 16 );
		r |= ( buffer[ 3 ] << 24 );
		return r;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    lnFloat BinaryReader::readFloat()
	{
		lnFloat buffer;
		mStream->read( &buffer, 4 );
		return buffer;
	}

} // namespace FileIO
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================