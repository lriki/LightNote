//==============================================================================
// FileUtil 
//==============================================================================

#include "stdafx.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "../Base/Misc.h"
#include "../Base/StringUtils.h"
#include "../Base/Unicode.h"
#include "Interface.h"
#include "StreamObject.h"
#include "FileUtil.h"

// '\' → '/' に変換する
#define DIR_DELIM_CONV_TO_SLASH

namespace LNote
{
namespace Core
{
namespace FileIO
{

//==============================================================================
// FileUtils
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool FileUtils::exists(const char* path)
	{
		if ( !path ) { 
			return false; 
		}
		FILE* fp = fopen( path, "rb" );
		if ( !fp ) { 
			return false; 
		}
		fclose( fp );
		return true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool FileUtils::exists(const wchar_t* path)
	{
		if ( !path ) { 
			return false; 
		}
		FILE* fp = Base::ln_wfopen( path, L"rb" );
		if ( !fp ) { 
			return false; 
		}
		fclose( fp );
		return true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	size_t FileUtils::getFileSize(FILE* stream)
	{
#ifdef LNOTE_WIN32
		struct _stat stbuf;
		int handle = _fileno( stream );
		if ( handle == 0 ) {
			return 0;
		}
		if ( _fstat( handle, &stbuf ) == -1 ) {
			return 0;
		}
		return stbuf.st_size;
#else
		struct stat stbuf;
		int handle = fileno( stream );
		if ( handle == 0 ) {
			return 0;
		}
		if (fstat(handle, &stbuf) == -1) {
			return 0;
		}
		return stbuf.st_size;
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void FileUtils::writeU32Little(FILE* stream, lnU32 v)
	{
	    if ( Base::isLittleEndian() )
	    {
	        fwrite( &v, sizeof( v ), 1, stream );
	    }
	    else
	    {
	        lnU8 b[ 4 ];
	        *( b + 0 ) =   v & 0x000000ff;
	        *( b + 1 ) = ( v & 0x0000ff00 ) >> 8;
	        *( b + 2 ) = static_cast< lnU8 >( ( v & 0x00ff0000 ) >> 16 );
	        *( b + 3 ) = ( v & 0xff000000 ) >> 24;
	        fwrite( &b, sizeof( b ), 1, stream );
	    }
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	int FileUtils::setSeekPoint(int curPoint, int maxSize, int offset, int origin)
	{
		int newPoint = curPoint;
		switch ( origin )
		{
			case SEEK_CUR:
				newPoint += offset;
				break;

			case SEEK_END:
				newPoint = maxSize + offset;
				break;

			default:
				newPoint = offset;
				break;
		}
		
		if ( newPoint < 0 ) { 
			newPoint = 0; 
		}
		if ( newPoint > maxSize ) { 
			newPoint = maxSize; 
		}
		return newPoint;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Stream* FileUtils::createInStreamFromMemoryManaged(const void* buffer, int size)
	{
		Base::FinallyReleasePtr< InManagedMemoryStream > st( LN_NEW InManagedMemoryStream() );
		st->create( buffer, size );
		return st.returnObject();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Stream* FileUtils::createInStreamFromMemoryUnManaged(const void* buffer, int size)
	{
		Base::FinallyReleasePtr< InUnManagedMemoryStream > st( LN_NEW InUnManagedMemoryStream() );
		st->create( buffer, size );
		return st.returnObject();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	lnU16 FileUtils::readU16(Stream* stream)
	{
		// TODO: このあたり、サイズが合わなかったらすべて例外扱い
		lnU8 buffer[ 2 ];
		stream->read( buffer, 2 );

		lnU16 r = buffer[ 0 ];
		r |= ( buffer[ 1 ] << 8 );
		return r;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	lnU32 FileUtils::readU32(Stream* stream)
	{
		lnU8 buffer[ 4 ];
		stream->read( buffer, 4, 4 );
		lnU32 r = buffer[ 0 ];
		r |= ( buffer[ 1 ] << 8 );
		r |= ( buffer[ 2 ] << 16 );
		r |= ( buffer[ 3 ] << 24 );
		return r;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	lnU16 FileUtils::readU16Big(Stream* stream)
	{
		lnU8 buffer[ 2 ];
		stream->read( buffer, 2, 2 );
		lnU16 ret = 0;
		for ( int i = 0; i < 2; i ++ )
		{
			ret = ret << 8;
			ret |= ( (lnU8*)&buffer )[ i ];
		}
		return ret;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	lnU32 FileUtils::readU32Big(Stream* stream)
	{
		lnU8 buffer[ 4 ];
		stream->read( buffer, 4, 4 );
		lnU32 ret = 0;
		for ( int i = 0; i < 4; i ++ )
		{
			ret = ret << 8;
			ret |= ( (lnU8*)&buffer )[ i ];
		}
		return ret;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Base::ReferenceBuffer* FileUtils::readAllBytes(const lnChar* filePath)
	{
		InFileStream stream;
		stream.open(filePath, FileType_Binary);

		size_t size = stream.getSize();

		LRefPtr<Base::ReferenceBuffer> buffer(LN_NEW Base::ReferenceBuffer());
		buffer->reserve(size);;
		stream.read(buffer->getPointer(), size, size);

		buffer.safeAddRef();
		return buffer;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Base::ReferenceBuffer* FileUtils::readAllText(const lnChar* filePath)
	{
		InFileStream stream;
		stream.open(filePath, FileType_Text);

		size_t size = stream.getSize();

		LRefPtr<Base::ReferenceBuffer> buffer(LN_NEW Base::ReferenceBuffer());
		buffer->reserve(size + 1);
		stream.read(buffer->getPointer(), size, size);
		buffer->getPointer()[size] = 0;	// 終端 NULL
		
		buffer.safeAddRef();
		return buffer;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Base::ReferenceBuffer* FileUtils::readAllData(Stream* stream)
	{
		size_t size = stream->getSize();
		Base::ReferenceBuffer* buf = LN_NEW Base::ReferenceBuffer();
		buf->reserve( size + 1 );
		stream->read( buf->getPointer(), size );
		buf->getPointer()[size] = 0;		// 念のための終端 NULL
		return buf;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void FileUtils::writeAllBytes(const lnChar* filePath, const lnByte* buffer, size_t size)
	{
		FILE* stream = _tfopen( filePath, _T("wb") );
		if ( stream ) {
			fwrite( buffer, 1, size, stream );
			fclose( stream );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void FileUtils::writeAllText(const lnChar* filePath, const lnChar* text)
	{
		FILE* stream = _tfopen( filePath, _T("w") );
		if ( stream ) {
			fprintf( stream, text );
			fclose( stream );
		}
	}

//==============================================================================
// ■ Path
//==============================================================================


	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	lnRefStringA Path::getDirectoryPath( const char* path_ )
	{
		int pos = strlen( path_ );

		// 後ろから前に調べて、最初に \\ か / が見つかるところを探す
		for ( ; pos >= 0; --pos ) {
			if ( path_[pos] == '\\' || path_[pos] == '/' ) {
				break;
			}
		}

		// 最後まで見つからなかった場合
		if ( pos == -1 )
		{
			return LRefTString();
		}

		return LRefTString( path_, pos );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	lnRefStringW Path::getDirectoryPath( const wchar_t* path )
	{
		int pos = wcslen( path );
		for ( ; pos >= 0; --pos ) {
			if ( path[pos] == L'\\' || path[pos] == L'/') {
				break;
			}
		}

		// 最後まで見つからなかった場合
		if ( pos == -1 ) {
			return lnRefStringW();
		}
		return lnRefStringW( path, pos );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LRefTString Path::getFileName( const lnChar* path_ )
	{
		int pos, len;
		pos = len = _tcslen( path_ );

		// 後ろから前に調べて、最初に \\ か / が見つかるところを探す
		for ( ; pos >= 0; --pos )
		{
			if ( path_[ pos ] == _T( '\\' ) || path_[ pos ] == _T( '/' ) )
			{
				++pos;
				break;
			}
		}
		if ( pos < 0 )
		{
			pos = 0;
		}

		return LRefTString( &path_[ pos ], len - pos );
	}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
bool Path::getDirPathAndFilePath( const char* path_, char* dir_, char* name_ )
{
    int i = 0;
    int dir_last = -1;      // ディレクトリ名の終端 ( 見つかればいずれかの区切り文字の位置を指す )
    while ( path_[ i ] )
    {
        if ( Base::StringUtils::checkMultiByteChar( &path_[ i ] ) )
        {
            i += 2;
        }
        else
        {
            if ( path_[ i ] == '\\' || path_[ i ] == '/' || path_[ i ] == '\0' || path_[ i ] == ':' )
            {
                dir_last = i;
            }
            ++i;
        }
    }

	// ディレクトリパス格納
    if ( dir_ )
    {
        // ディレクトリ部分が見つからなかった場合は空
        if ( dir_last == -1 )
        {
            dir_[ 0 ] = '\0';
        }
        else
        {
            strncpy( dir_, path_, dir_last );
		    dir_[ dir_last ] = '\0';
        }
    }

	// ファイル名格納
    if ( name_ )   
    {
        // ディレクトリ部分が見つからなかった場合は全部コピー
        if ( dir_last == -1 )
        {
            strcpy( name_, path_ );
        }
        else
        {
            strcpy( name_, &path_[ dir_last + 1 ] );
        }
    }
    return true;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
bool Path::getDirPathAndFilePath( const wchar_t* path_, wchar_t* dir_, wchar_t* name_ )
{
    int i = 0;
    int dir_last = -1; 
    while ( path_[ i ] )
    {
        if ( path_[ i ] == L'\\' || path_[ i ] == L'/' || path_[ i ] == L'\0' || path_[ i ] == L':' )
        {
            dir_last = i;
        }
        ++i;
    }

    if ( dir_ )
    {
        if ( dir_last == -1 )
        {
            dir_[ 0 ] = L'\0';
        }
        else
        {
            wcsncpy( dir_, path_, dir_last );
		    dir_[ dir_last ] = L'\0';
        }
    }

    if ( name_ )   
    {
        if ( dir_last == -1 )
        {
            wcscpy( name_, path_ );
        }
        else
        {
            wcscpy( name_, &path_[ dir_last + 1 ] );
        }
    }
    return true;
}

#if 0
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void Path::normalizePathName( lnChar* str_ )
{
	for ( lnChar* c = str_; *c != _T( '\0' ); ++c )
	{
		if ( *c == _T( '/' ) )
		{
			*c = _T( '\\' );
		}
	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void Path::normalizePathName( LRefTString* str_ )
{
	if ( str_ && str_->size() > 0 )
	{
		for ( lnChar* c = &(*str_)[ 0 ]; *c != _T( '\0' ); ++c )
		{
			if ( *c == _T( '/' ) )
			{
				*c = _T( '\\' );
			}
		}
	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void Path::normalizePathName( ln_std_tstring* out_, const lnChar* path_ )
{
	if ( _tcslen( path_ ) < 2 )
	{
		(*out_) = path_;
		return;
	}

	// 先頭が "./" ".\" の場合は飛ばす (././ とか続けて書くことはまずないと思うので、先頭2文字だけチェック)
	if ( path_[ 0 ] == _T( '.' ) && ( path_[ 1 ] == _T( '/' ) || path_[ 1 ] == _T( '\\' ) ) )
	{
		(*out_) = &path_[ 2 ];
	}
	else
	{
		(*out_) = path_;
	}

	// バックスラッシュ化
	for ( size_t i = 0; i < out_->size(); ++i )
	{
		if ( (*out_)[ i ] == _T( '/' ) )
		{
			(*out_)[ i ] = _T( '\\' );
		}
	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void Path::makeFileName( ln_std_tstring* out_, const lnChar* path_ )
{
    lnChar path[ LN_MAX_PATH ];
    _tcscpy( path, path_ );

    int path_len = _tcslen( path_ );
    int e = path_len;
    int last_dot = 0;
    int step = 0;
    int cnt = 0;

    for ( ; e > 2;  )
    {
        switch ( step )
        {
            // 最初の "../" 検索
            case 0:
            {
                if ( path_[ e - 2 ] == _T( '.' ) && path_[ e - 1 ] == _T( '.' ) && path_[ e ] == _T( '\\' ) )
                {
                    last_dot = e - 2;   // ../ の 先頭の . を指す
                    e -= 3;             // 次の / を指す
                    cnt = 1;
                    step = 1;
                    continue;
                }
                --e;
                break;
            }

            // "../" の連続数カウント
            case 1:
            {
                if ( path_[ e - 2 ] == _T( '.' ) && path_[ e - 1 ] == _T( '.' ) && path_[ e ] == _T( '\\' ) )
                {
                    last_dot = e - 2;   // ../ の 先頭の . を指す
                    e -= 3;             // 次の / を指す
                    ++cnt;
                    continue;
                }
                else
                {
                    //printf( "連続数カウント終了 %d\n", cnt );
                    path[ e ] = _T( '\0' );
                    step = 2;
                    continue;
                }
                break;
            }

            // カットする部分を \0 で埋めていく
            case 2:
            {
                // cnt が残っているけど "../" が見つかった
                if ( path_[ e - 2 ] == _T( '.' ) && path_[ e - 1 ] == _T( '.' ) && path_[ e ] == _T( '\\' ) )
                {
                    e -= 3; // 次の / を指す
                    cnt = 1;
                    step = 1;
                    continue;
                }
                else
                {
                    --e;
                    if ( path_[ e ] != _T( '\\' ) )
                    {
                        path[ e ] = _T( '\0' );
                    }
                    // フォルダ・ファイル名の後ろの / が見つかった
                    else
                    {
                        

                        path[ last_dot + 0 ] = _T( '\0' );
                        path[ last_dot + 1 ] = _T( '\0' );
                        path[ last_dot + 2 ] = _T( '\0' );

                        last_dot = last_dot + 3;

                        --cnt;
                        if ( cnt == 0 )
                        {
                            step = 0;
                            continue;
                        }

                        // / を消すが、"../" の場合は消さない
                        if ( path[ e - 1 ] != _T( '.' ) )
                        {
                            path[ e ] = _T( '\0' );
                        }
                    }
                }
                
                break;
            }
        }
    }

    if ( step == 2 )
    {
        for ( ;e > 0; )
        {
            --e;
            if ( path_[ e ] != _T( '\\' ) )
            {
                path[ e ] = _T( '\0' );
            }
        }

        path[ last_dot + 0 ] = _T( '\0' );
        path[ last_dot + 1 ] = _T( '\0' );
        path[ last_dot + 2 ] = _T( '\0' );
    }

    (*out_) = path;

	// 一度でも変更があった場合
    if ( _tcslen( path ) != path_len )
    {
        int i = _tcslen( path );    // \0 を指す
        for ( ;i < path_len; )
        {
            for ( ; path[ i ] == _T( '\0' ) && i < path_len; )
            {
                ++i;
            }
            (*out_) += &path[ i ];
            i += _tcslen( &path[ i ] );
        }
    }
}
#endif
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Base::RefString Path::getExtName( const char* filePath )
	{
		int pos = strlen( filePath );
		int dot_pos = pos;
		for ( ; pos >= 0; --pos ) {
			if ( filePath[ pos ] == _T( '.' ) ) {
				dot_pos = pos;
				break;
			}
			else if ( filePath[ pos ] == _T( '\\' ) || filePath[ pos ] == _T( '/' ) ) {
				pos = 0;
				break;
			}
		}

		// 最後まで見つからなかった場合
		if ( pos == -1 ) {
			return Base::RefString( filePath, -1 );
		}
		return Base::RefString( filePath, dot_pos );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Base::RefWString Path::getExtName( const wchar_t* filePath )
	{
		int pos = wcslen( filePath );
		int dot_pos = pos;
		for ( ; pos >= 0; --pos ) {
			if ( filePath[ pos ] == L'.' ) {
				dot_pos = pos;
				break;
			}
			else if ( filePath[ pos ] == L'\\' || filePath[ pos ] == L'/' ) {
				pos = 0;
				break;
			}
		}

		// 最後まで見つからなかった場合
		if ( pos == -1 ) {
			return Base::RefWString( filePath, -1 );
		}
		return Base::RefWString( filePath, dot_pos );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Path::isFullPath( const char* path )
	{
		if (*path == '/' || *path == '\\') return true;
		for (; *path; ++path)
		{
			if (*path == ':') return true;
		}
		return false;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Path::isFullPath( const wchar_t* path )
	{
		if (*path == L'/' || *path == L'\\') return true;
		for (; *path; ++path)
		{
			if (*path == L':') return true;
		}
		return false;
	}

	

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	template<typename CHAR_T> 
	static bool _analyzePath( const CHAR_T* inPath, CHAR_T* outPath, int outPathSize )
	{
#ifdef DIR_DELIM_CONV_TO_SLASH
	#define DIR_DLM	_lnTC('/')
#else
	#define DIR_DLM	_lnTC('\\')
#endif

		// inPath はフルパスで、先頭の \ を指していること
		if ( inPath[0] != DIR_DLM ) return false;

		int pathLen = ln_tcslen( inPath ) - 1;	// inPath[pos] で一番後ろの文字を指す
		if ( inPath[pathLen] == DIR_DLM )
		{
			// 終端が \ の時、それは考慮しない(出力もしない)
			--pathLen;
		}

		// ※文字列の終端から先頭へ見ていく
		int pos = pathLen;
		int moveUpDepth = 0;
		for ( ; pos >= 0; )
		{
			// ループ開始時、pos は \ のひとつ前を指している

			// 次の \ までの文字数
			int len = 0;
			for ( ; pos - len >= 1 && inPath[pos - len] != DIR_DLM; ++len );
			++len;	// \ も含む

			// ディレクトリ、またはファイル名部分が空の場合はエラーとする
			// (::SetCurrentDirectory() は // だけならOK。 //../ のような記述をした場合にエラーとなるが、
			//	いずれにしても不正なフォルダ・ファイル名なのでエラー)
			if ( len == 1 ) return false;

			// \.
			if ( len == 2 && inPath[pos - 1] == DIR_DLM && inPath[pos] == _lnTC('.') )
			{
				// スキップ
				outPath[pos - 1] = outPath[pos] = _lnTC('\0');
				pos -= 2;
			}
			// \..
			else if ( len == 3 && inPath[pos - 2] == DIR_DLM && inPath[pos - 1] == _lnTC('.') && inPath[pos] == _lnTC('.') )
			{
				// 深さを +1 してスキップ
				++moveUpDepth;
				outPath[pos - 2] = outPath[pos - 1] = outPath[pos] = _lnTC('\0');
				pos -= 3;
			}
			// \xxxx
			else
			{
				int writePos = pos - len + 1;
				// 深さがある場合は \0 で埋める
				if ( moveUpDepth > 0 )
				{
					memset( &outPath[writePos], 0, sizeof(CHAR_T) * len );
					--moveUpDepth;
				}
				// 通常コピー
				else
				{
					if ( ln_tcsncpy( &outPath[writePos], outPathSize - writePos, &inPath[pos - len + 1], len ) == false )
						return false;
				}
				pos -= len;
			}
		}

		// pathLen 以前の \0 をすべて詰めていく
		int writePos = 0;
		for ( int i = 0; i < pathLen + 1; ++i )
		{
			if ( outPath[i] )
			{
				outPath[writePos] = outPath[i];
				++writePos;
			}
		}
		outPath[writePos] = _lnTC('\0');
		return true;
	}

	// A1/../A2/A3/../A4/../../B.txt > B.txt
	// A1/../A2/A3/../A4/../A5/../B.txt > A2/B.txt
	// A/ のような終端の / は削除
	template<typename CHAR_T> 
	static bool _canonicalizePath( const CHAR_T* inPath, CHAR_T* outPath )
	{
		CHAR_T tmp[LN_MAX_PATH] = { 0 };

		const CHAR_T* source = inPath;
		CHAR_T* target = tmp;
		int len = 0;

		// /変換 + 文字数カウント
		for ( ; *source != _lnTC('\0'); ++source, ++target, ++len )
		{
#ifdef DIR_DELIM_CONV_TO_SLASH
			if ( *source == _lnTC('\\') ) {
				*target = _lnTC('/');
			}
#else
			if ( *source == _lnTC('/') ) {
				*target = _lnTC('\\');
			}
#endif
			else {
				*target = *source;
			}
		}

		// ドライブレターのチェック
		int offset = 0;
		if ( len >= 2 && isalpha( tmp[0] ) && tmp[1] == _lnTC(':') )
		{
			// "C:" 等は先に出力しておく。
			// この後の _analyzePath() には / で始まるパスを渡す必要がある。
			outPath[0] = tmp[0];
			outPath[1] = tmp[1];
			offset = 2;
		}

		// 正規化
		return _analyzePath( tmp + offset, outPath + offset, LN_MAX_PATH - offset );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Path::canonicalizePath( const char* inPath, char* outPath )
	{
		return _canonicalizePath( inPath, outPath );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Path::canonicalizePath( const wchar_t* inPath, wchar_t* outPath )
	{
		return _canonicalizePath( inPath, outPath );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Path::normalizePath( const char* inPath, char* outPath )
	{
		for ( ; *inPath != '\0'; ++inPath, ++outPath )
		{
#ifdef DIR_DELIM_CONV_TO_SLASH
			if ( *inPath == '\\' ) {
				*outPath = '/';
			}
#else
			if ( *inPath == '/' ) {
				*outPath = '\\';
			}
#endif
			else {
				*outPath = *inPath;
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Path::normalizePath( const wchar_t* inPath, wchar_t* outPath )
	{
		for ( ; *inPath != L'\0'; ++inPath, ++outPath )
		{
#ifdef DIR_DELIM_CONV_TO_SLASH
			if ( *inPath == L'\\' ) {
				*outPath = L'/';
			}
#else
			if ( *inPath == L'/' ) {
				*outPath = L'\\';
			}
#endif
			else {
				*outPath = *inPath;
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Path::normalizePath( std::string* path )
	{
		if ( path->size() > 0 ) {
			for ( size_t i = 0; i < path->size(); ++i ) {
#ifdef DIR_DELIM_CONV_TO_SLASH
				if ( (*path)[i] == '\\' ) (*path)[i] = '/';
#else
				if ( (*path)[i] == '/' ) (*path)[i] = '\\';
#endif
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Path::normalizePath( std::wstring* path )
	{
		if ( path->size() > 0 ) {
			for ( size_t i = 0; i < path->size(); ++i ) {
#ifdef DIR_DELIM_CONV_TO_SLASH
				if ( (*path)[i] == L'\\' ) (*path)[i] = L'/';
#else
				if ( (*path)[i] == L'/' ) (*path)[i] = L'\\';
#endif
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Path::normalizePath( LRefString* path )
	{
		if ( path->size() > 0 ) {
			for ( int i = 0; i < path->size(); ++i ) {
#ifdef DIR_DELIM_CONV_TO_SLASH
				if ( (*path)[i] == L'\\' ) (*path)[i] = L'/';
#else
				if ( (*path)[i] == L'/' ) (*path)[i] = L'\\';
#endif
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Path::normalizePath( LRefWString* path )
	{
		if ( path->size() > 0 ) {
			for ( int i = 0; i < path->size(); ++i ) {
#ifdef DIR_DELIM_CONV_TO_SLASH
				if ( (*path)[i] == L'\\' ) (*path)[i] = L'/';
#else
				if ( (*path)[i] == L'/' ) (*path)[i] = L'\\';
#endif
			}
		}
	}

} // namespace FileIO
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================