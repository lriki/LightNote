//==============================================================================
//  LogFile 
//==============================================================================

#include "stdafx.h"
#include <time.h>
#include <string>
#include "../Threading/Mutex.h"
#include "../System/Environment.h"
#include "RefString.h"
#include "LogFile.h"

namespace LNote
{
namespace Core
{
namespace Base
{

//==============================================================================
// ■ Log
//==============================================================================

	//----------------------------------------------------------------------
	// ● コンストラクタ
	//----------------------------------------------------------------------
    LogFile::LogFile()
        : mMutex        ( NULL )
        , mLogStartTime ( 0 )
    {
    }

	//----------------------------------------------------------------------
	// ● デストラクタ
	//----------------------------------------------------------------------
    LogFile::~LogFile()
    {
        if ( mLogFilePath[ 0 ] != _T( '\0' ) )
        {
            FILE* stream = _tfopen( mLogFilePath, _T( "a+" ) );
            if ( stream )
            {
                _ftprintf(
                    stream,
                    _T( "===============================================================\n" )
                    _T( "                          end of file\n" )
                    _T( "===============================================================" ) );

                fclose( stream );
            }
                

            LN_SAFE_RELEASE( mMutex );
            //::DeleteCriticalSection( &mCriticalSection );
        }
    }

	//----------------------------------------------------------------------
	// ● 初期化
	//----------------------------------------------------------------------
    bool LogFile::initialize( const lnChar* log_name_, const lnChar* log_dir_ )
    {
        if ( log_dir_ )
        {
            _tcscpy( mLogFilePath, log_dir_ );
            size_t len = _tcslen( log_dir_ );
            mLogFilePath[ len ] = '/';//'\\';
            _tcscpy( &mLogFilePath[ len + 1 ], log_name_ );
        }
        else
        {
            System::Environment::getCurrentDirectory( mLogFilePath );
            size_t len = _tcslen( mLogFilePath );
            mLogFilePath[ len ] = '/';//'\\';
            _tcscpy( &mLogFilePath[ len + 1 ], log_name_ );

            /*
            // mLogFilePath にログファイルのフルパスを作る
            DWORD  dir_len  = ::GetCurrentDirectory( 0, NULL );
            //size_t name_len = _tcslen( log_name_ );
            ::GetCurrentDirectory( MAX_PATH, mLogFilePath );
            mLogFilePath[ dir_len - 1 ] = _T( '\\' );
            _tcscpy( &mLogFilePath[ dir_len ], log_name_ );
            */
        }

        
        FILE* stream = _tfopen( mLogFilePath, _T( "w+" ) );

        if ( !stream )
        {
            mLogFilePath[ 0 ] = _T( '\0' );
            return false;
        }

        time_t timer;
        time( &timer );
        char* ts = asctime( localtime( &timer ) );
        ln_std_tstring time_str( ts, strlen( ts ) - 1 );

        _ftprintf( stream, _T( "===============================================================\n" ) );
        _ftprintf( stream, _T( " LNote Log    Version %s    %s\n" ), LNOTE_VERSION_STR, time_str.c_str() );
        _ftprintf( stream, _T( "---------------------------------------------------------------\n" ) );

#ifdef LNOTE_DEBUG
        _ftprintf( stream, _T( "BuildConfig : Debug\n" ) );
#else
        _ftprintf( stream, _T( "BuildConfig : Release\n" ) );
#endif

#ifdef LNOTE_UNICODE
        _ftprintf( stream, _T( "CharSet     : Unicode\n" ) );
#else
        _ftprintf( stream, _T( "CharSet     : MultiByte\n" ) );
#endif

        _ftprintf( stream, _T( "Flags\n" ) );

#ifdef LNOTE_SPECIFIED_CHARCODESET
        _ftprintf( stream, _T( "    LNOTE_SPECIFIED_CHARCODESET\n" ) );
#endif

        
/*
#ifdef UNICODE
        _ftprintf( stream, _T( "UNICODE " ) );
#endif
#ifdef _UNICODE
        _ftprintf( stream, _T( "_UNICODE " ) );
#endif
#ifdef _UNICODE
        _ftprintf( stream, _T( "_MBCS " ) );
#endif
        _ftprintf( stream, _T( ")\n" ) );
        */


        _ftprintf( stream, _T( "===============================================================\n" ) );


        fclose( stream );

        mMutex = LN_NEW Threading::Mutex();
        //::InitializeCriticalSection( &mCriticalSection );

        mLogStartTime = System::Environment::getSystemTime();
       
        return true;
    }

	//----------------------------------------------------------------------
	// ● ファイルに追加モードで書き込む
	//----------------------------------------------------------------------
    bool LogFile::write( const char *format_, ... )
	{
        //if ( mLogStartTime )
        {
            mMutex->lock();

            char* buf = (char*)mTempBuffer;

            // mTempBuffer に文字列を作成
            va_list args;
            va_start( args, format_ );
            vsnprintf( buf, TEMP_BUFFER_SIZE, format_, args );
            va_end( args );

            // ファイルを開いて文字列を書き込む
		    FILE* stream = _tfopen( mLogFilePath, _T( "a+" ) );
		    if ( stream )
		    {
                fprintf( stream, "%d:%s\n", System::Environment::getSystemTime() - mLogStartTime, buf );
			    fclose( stream );
		    }

            mMutex->unlock();



/*
            static const int BUF_SIZE = 256;
            char buf[ BUF_SIZE ];
            char* str_buf = buf;
		    va_list args;

		    // 引数リストを取得
		    va_start( args, format_ );

		    // 書式指定文字列内の文字数を記憶
		    int len = 2048;//_vscprintf( format_, args ) + 1;

            // 文字列がバッファサイズを超える場合は新しいものを確保
            if ( len >= BUF_SIZE )
            {
		        str_buf = (char*)malloc( len );
            }

		    if ( str_buf )
		    {
			    // バッファに文字列を書き込む
			    vsprintf( str_buf, format_, args );

			    // ファイルを開いて文字列を書き込む
			    FILE* stream = _tfopen( mLogFilePath, _T( "a+" ) );
			    if ( stream )
			    {
                    fprintf( stream, "%d:%s\n", System::getSystemTime() - mLogStartTime, str_buf );
				    fclose( stream );
			    }
    			
                // メモリが確保されていた場合は解放
			    if ( str_buf != buf )
                {
                    SAFE_FREE( str_buf );
                }
		    }

		    va_end( args );

            mMutex->unlock();
            //::LeaveCriticalSection( &mCriticalSection );]
            */
        }

        return true;
	}

	//----------------------------------------------------------------------
	// ● ファイルに追加モードで書き込む ( wchar_t )
	//----------------------------------------------------------------------
    bool LogFile::write( const wchar_t *format_, ... )
    {
        //if ( mLogStartTime )
        {
            mMutex->lock();

            // mTempBuffer に文字列を作成
            va_list args;
            va_start( args, format_ );
            vswprintf( mTempBuffer, TEMP_BUFFER_SIZE, format_, args );
            va_end( args );

            // ファイルを開いて文字列を書き込む
		    FILE* stream = _tfopen( mLogFilePath, _T( "a+" ) );
		    if ( stream )
		    {
                fwprintf( stream, L"%d:%s\n", System::Environment::getSystemTime() - mLogStartTime, mTempBuffer );
			    fclose( stream );
		    }

            mMutex->unlock();

            /*
            mMutex->lock();

            static const int BUF_SIZE = 256;
            wchar_t buf[ BUF_SIZE ];
            wchar_t* str_buf = buf;
		    va_list args;

		    va_start( args, format_ );

		    int len = 2048;//_vscwprintf( format_, args ) + 1;

            if ( len >= BUF_SIZE )
            {
		        str_buf = (wchar_t*)malloc( sizeof( wchar_t ) * ( len ) );
            }

		    if ( str_buf )
		    {
			    // バッファに文字列を書き込む
			    vswprintf( str_buf, format_, args );

			    // ファイルを開いて文字列を書き込む
			    FILE* stream = _tfopen( mLogFilePath, _T( "a+" ) );
			    if ( stream )
			    {
                    fwprintf( stream, L"%d:%s\n", ::timeGetTime() - mLogStartTime, str_buf );
				    fclose( stream );
			    }
    			
                // メモリが確保されていた場合は解放
			    if ( str_buf != buf )
                {
                    SAFE_FREE( str_buf );
                }
		    }

		    va_end( args );

            mMutex->unlock();
            //::LeaveCriticalSection( &mCriticalSection );
            */
        }

        return true;
    }


//==============================================================================
// Logger
//==============================================================================

    class FileClose
        : public Base::ReferenceObject
    {
    public:
        FileClose() {}
        virtual ~FileClose();
        //void set( FILE* stream ) { mStream = stream; }

    private:
        //FILE*   mStream;
    };

	//class FileRelease
	//{
	//public:
	//    FileRelease() : mFile( NULL ) {}
	//    virtual ~FileRelease() { LN_SAFE_RELEASE( mFile ); }
	//    void set( FileClose* file ) { mFile = file; }

	//private:
	//    FileClose*   mFile;
	//};

    static const int        TEMP_BUFFER_SIZE = 2048;
    static Threading::Mutex*   gMutex = NULL;
    static FileClose       gFileClose;// = NULL;
	//static FileRelease      gFileRelease;
    static lnChar           gLogFilePath[ LN_MAX_PATH ] = { 0 };
	//static FILE*            gStream;
    static lnU32            gLogStartTime;
    static wchar_t          gTempBuffer[ TEMP_BUFFER_SIZE ];    ///< char、wchar_t 共用

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    FileClose::~FileClose()
    {
        //printf("~FileClose()\n");

        //if ( mStream )  fclose( mStream );

        // デストラクタで書き込む場合は一度開きなおす必要がある (Win32)
        FILE* stream = _tfopen( gLogFilePath, _T( "a+" ) );
        if ( stream )
        {
            _ftprintf(
                stream,
                _T( "============== Application Exit ============== \n" ) );
            fclose( stream );
        }

		LN_SAFE_RELEASE( gMutex );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool Logger::initialize( const lnChar* log_name, const lnChar* log_dir )
    {
        if ( log_dir )
        {
            _tcscpy( gLogFilePath, log_dir );
            size_t len = _tcslen( log_dir );
            gLogFilePath[ len ] = '/';//'\\';
            _tcscpy( &gLogFilePath[ len + 1 ], log_name );
        }
        else
        {
            System::Environment::getCurrentDirectory( gLogFilePath );
            size_t len = _tcslen( gLogFilePath );
            gLogFilePath[ len ] = '/';//'\\';
            _tcscpy( &gLogFilePath[ len + 1 ], log_name );
        }

        FILE* stream = _tfopen( gLogFilePath, _T( "w+" ) );
        if ( !stream )
        {
            gLogFilePath[ 0 ] = _T( '\0' );
            return false;
        }

        //gFileClose = new FileClose();
        //gFileClose->set( gStream );
        //gFileRelease.set( gFileClose );

        time_t timer;
        time( &timer );
        char* ts = asctime( localtime( &timer ) );
        lnString time_str( ts, strlen( ts ) - 1 );

        _ftprintf( stream, _T( "===============================================================\n" ) );
        _ftprintf( stream, _T( " "LNOTE_NAME" Log    Version %s    %s\n" ), LNOTE_VERSION_STR, time_str.c_str() );
        _ftprintf( stream, _T( "---------------------------------------------------------------\n" ) );

#ifdef LNOTE_DEBUG
        _ftprintf( stream, _T( "BuildConfig : Debug\n" ) );
#else
        _ftprintf( stream, _T( "BuildConfig : Release\n" ) );
#endif

#ifdef LNOTE_UNICODE
        _ftprintf( stream, _T( "CharSet     : Unicode\n" ) );
#else
        _ftprintf( stream, _T( "CharSet     : MultiByte\n" ) );
#endif

        _ftprintf( stream, _T( "Flags       : \n" ) );

#ifdef LNOTE_SPECIFIED_CHARCODESET
        _ftprintf( stream, _T( "    LNOTE_SPECIFIED_CHARCODESET\n" ) );
#endif
        _ftprintf( stream, _T( "===============================================================\n" ) );


        fclose( stream );

        gLogStartTime = System::Environment::getSystemTime();
		gMutex = new Threading::Mutex(); 
       
        return true;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    const char* getInfoString( Logger::Level level )
    {
        switch ( level )
        {
            case Logger::LEVEL_INFO:    return "Info    ";
            case Logger::LEVEL_WARNING: return "Warning ";
            case Logger::LEVEL_ERROR:   return "Error   ";
            default:                    return "-       ";
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Logger::print( const char *format, ... )
    {
		// NULL チェックが必要なので、Threading::ScopedLock は使っていない。
        if ( gMutex ) {
			gMutex->lock();
		}

        if ( gLogFilePath[0] == '\0' ) return;
        FILE* stream = _tfopen( gLogFilePath, _T( "a+" ) );
        if ( stream )
        {
            char* buf = (char*)gTempBuffer;
            
            va_list args;
            va_start( args, format );
            vsnprintf( buf, TEMP_BUFFER_SIZE, format, args );
            va_end( args );

            fprintf( stream, "%s", buf );

            fclose( stream );
	    }

        if ( gMutex ) {
			gMutex->unlock();
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Logger::print( const wchar_t* format, ... )
    {
        if ( gMutex ) {
			gMutex->lock();
		}

        if ( gLogFilePath[0] == '\0' ) return;
        FILE* stream = _tfopen( gLogFilePath, _T( "a+" ) );
        if ( stream )
        {
            va_list args;
            va_start( args, format );
            vswprintf( gTempBuffer, TEMP_BUFFER_SIZE, format, args );
            va_end( args );

            fwprintf( stream, L"%s", gTempBuffer );

            fclose( stream );
        }

		if ( gMutex ) {
			gMutex->unlock();
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Logger::println( Level level, const char *format, ... )
    {
        if ( gMutex ) {
			gMutex->lock();
		}

        if ( gLogFilePath[0] == '\0' ) return;
        FILE* stream = _tfopen( gLogFilePath, _T( "a+" ) );
        if ( stream )
        {
            char* buf = (char*)gTempBuffer;
            
            va_list args;
            va_start( args, format );
            vsnprintf( buf, TEMP_BUFFER_SIZE, format, args );
            va_end( args );

            fprintf( stream, "%d %s: ", System::Environment::getSystemTime() - gLogStartTime, getInfoString( level ) );
            fprintf( stream, "%s\n", buf );

            fclose( stream );
	    }

		if ( gMutex ) {
			gMutex->unlock();
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Logger::println( Level level, const wchar_t* format, ... )
    {
        if ( gMutex ) {
			gMutex->lock();
		}

        if ( gLogFilePath[0] == '\0' ) return;
        FILE* stream = _tfopen( gLogFilePath, _T( "a+" ) );
        if ( stream )
        {
            va_list args;
            va_start( args, format );
            vswprintf( gTempBuffer, TEMP_BUFFER_SIZE, format, args );
            va_end( args );

            fprintf( stream, "%d %s: ", System::Environment::getSystemTime() - gLogStartTime, getInfoString( level ) );
            fwprintf( stream, L"%s\n", gTempBuffer );

            fclose( stream );
        }

		if ( gMutex ) {
			gMutex->unlock();
		}
    }

} // namespace Base
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================