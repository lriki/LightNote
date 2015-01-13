//==============================================================================
// ArchiveMaker 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <string>
#include <camellia/camellia.h>
#include "../Base/Misc.h"
#include "Common.h"
#include "FileUtil.h"
#include "PathName.h"
#include "Archive.h"
#include "ArchiveMaker.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace FileIO
{

//==============================================================================
// ■ ArchiveMaker
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ArchiveMaker::ArchiveMaker()
		: mStream	( NULL )
		, mFileNum	( 0 )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ArchiveMaker::~ArchiveMaker()
	{
		close();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool ArchiveMaker::open(const lnChar* filePath, const char* key)
	{
		PathName path(filePath);
		return open(path.getPath(), key);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool ArchiveMaker::open(const wchar_t* filePath, const char* key)
	{
		mEncryptionKey = (key) ? key : "";
		mFileNum = 0;
		memset(mKeyTable, 0, sizeof(mKeyTable));

		// 拡張キーの作成
		if (key)
		{
			lnByte key_buf[KEY_SIZE] = { 0 };
			memcpy(key_buf, key, strlen(key));
			memset(mKeyTable, 0, sizeof(mKeyTable));
			Camellia_Ekeygen(KEY_SIZE, key_buf, mKeyTable);
		}

		mStream = _wfopen(filePath, L"wb");//_tfopen(filename_, ("wb"));
		if (!mStream) {
			return false;
		}

		ArchiveHeader header;
		header.ID[0] = 'l';
		header.ID[1] = 'n';
		header.ID[2] = 'a';
		header.ID[3] = ' ';
		header.Version = Archive::Version_100;
		/*
		header.KeyLength = mEncryptionKey.size();
		_tcsncpy( header.Key, mEncryptionKey.c_str(), 128 );
		*/
		// ヘッダ部が 16byte になるように 0 を詰める
		memset(header.Padding, 0, sizeof(header.Padding));

		fwrite(&header, sizeof(header), 1, mStream);
		//_writePadding16( &header, sizeof( header ) );

		// 内部キー16バイト
		_writePadding16(Archive::InternalKey, 16);

		return true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ArchiveMaker::close()
	{
		if ( mStream )
		{
			// 一番最後にファイルの数を書き込む
            _writeU32Padding16( mFileNum, 0 );

			mEncryptionKey = "";
			mFileNum = 0;
			fclose( mStream );
			mStream = NULL;
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool ArchiveMaker::addFile( const wchar_t* filePath, const wchar_t* accessName )
	{
		FILE* stream = _wfopen( filePath, L"rb" );
		if ( stream )
		{
            // アクセス用の名前がなければ、ファイル名を代わりに使う
            if ( !accessName )
            {
                accessName = filePath;
            }

            // アクセス用ファイル名のスラッシュをバックスラッシュ化
			std::wstring filename = accessName;//filePath;
			FileIO::Path::normalizePath( &filename );

            //-------------------------------------------------
            // ファイル名の長さとファイルのサイズを書き込む
			lnU32 name_size = filename.size();
			lnU32 file_size = FileIO::FileUtils::getFileSize(stream);

            _writeU32Padding16( name_size, file_size );

            //-------------------------------------------------
            // ファイル名とファイル内容を書き込む

            // ファイル名を書き込む (終端NULLはナシ)
            _writePadding16( (lnByte*)filename.c_str(), name_size * sizeof(wchar_t) );

            // サイズ分のメモリを確保して追加する内容を読み込む
		    lnByte* buffer = LN_NEW lnByte[ file_size ];
		    fseek( stream, 0, SEEK_SET );
		    fread( buffer, sizeof( lnByte ), file_size, stream );

		    // 内容を書き込む
		    _writePadding16( buffer, file_size );

		    SAFE_DELETE_ARRAY( buffer );

            fclose( stream );

            ++mFileNum;

            return true;
		}
        return false;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ArchiveMaker::_writePadding16( const lnByte* data, lnU32 size )
    {
        lnU32 ps = ( 16 - ( size % 16 ) ) % 16;
        char padding[ 16 ];
        memset( padding, ps, sizeof( padding ) );

        // 暗号化する場合
        if ( mEncryptionKey.size() > 0 )
        {
            int i = 0;
            lnU32 total = 0;
            lnByte src_data[ 16 ];
            lnByte write_data[ 16 ];
            while ( true )
            {
                if ( total + 16 > size )
                {
                    memcpy( src_data, &data[ i * 16 ], 16 - ps );
                    memcpy( &src_data[ 16 - ps ], padding, ps );
                }
                else
                {
                    memcpy( src_data, &data[ i * 16 ], 16 );
                }
				Camellia_EncryptBlock(KEY_SIZE, src_data, mKeyTable, write_data);
                fwrite( write_data, 1, 16, mStream );
                ++i;
                total += 16;
                if ( total >= size )
                {
                    break;
                }
            }
        }
        else
        {
            fwrite( data,   1, size, mStream );
            fwrite( padding, 1, ps, mStream );
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ArchiveMaker::_writeU32Padding16( lnU32 v0, lnU32 v1 )
    {
		// 16 byte のうち、先頭 8 byte に u32 を 2 つ書き込む

        lnByte b[ 16 ] = { 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8 };
        if ( Base::isLittleEndian() )
        {
            *((lnU32*)(b + 0)) = v0;
            *((lnU32*)(b + 4)) = v1;
        }
        else
        {
            *( b + 0 ) = static_cast< lnU8 >(   v0 & 0x000000ff );
            *( b + 1 ) = static_cast< lnU8 >( ( v0 & 0x0000ff00 ) >> 8 );
            *( b + 2 ) = static_cast< lnU8 >( ( v0 & 0x00ff0000 ) >> 16 );
            *( b + 3 ) = static_cast< lnU8 >( ( v0 & 0xff000000 ) >> 24 );

            *( b + 4 ) = static_cast< lnU8 >(   v1 & 0x000000ff );
            *( b + 5 ) = static_cast< lnU8 >( ( v1 & 0x0000ff00 ) >> 8 );
            *( b + 6 ) = static_cast< lnU8 >( ( v1 & 0x00ff0000 ) >> 16 );
            *( b + 7 ) = static_cast< lnU8 >( ( v1 & 0xff000000 ) >> 24 );
        }

        lnByte buf[ 16 ];
		Camellia_EncryptBlock(KEY_SIZE, b, mKeyTable, buf);
        fwrite( &buf, sizeof( b ), 1, mStream );
    }

} // namespace FileIO
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================