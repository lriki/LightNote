﻿//==============================================================================
//  LogFile 
//------------------------------------------------------------------------------
///**
//  @file       LogFile.h
//  @brief      LogFile
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "ReferenceObject.h"

//#if defined(LNOTE_GNUC)
//    // ログ出力 ( mLogFile をメンバに持つクラス用。ワイド文字列兼用 )
//    #define LN_LOG_WRITE( format_, ... )    { if ( mLogFile ) { mLogFile->write( ( format_ ), ## __VA_ARGS__ ); } }
//
//    #define LN_LOG_INIT_BEGIN \
//        mLogFile = init_data_.LogFile; \
//        LN_SAFE_ADDREF( mLogFile ); \
//        LN_LOG_WRITE( "%s 初期化を開始します...", __FUNCTION__ );
//
//    #define LN_LOG_INIT_END \
//        LN_LOG_WRITE( "%s 初期化が完了しました。", __FUNCTION__ );
//
//    #define LN_LOG_FIN_BEGIN \
//        LN_LOG_WRITE( "%s 終了処理を開始します...", __FUNCTION__ );
//
//    #define LN_LOG_FIN_END \
//        LN_LOG_WRITE( "%s 終了処理が完了しました。", __FUNCTION__ ); \
//        LN_SAFE_RELEASE( mLogFile );
//
//#else
//    // ログ出力 ( mLogFile をメンバに持つクラス用。ワイド文字列兼用 )
//    #define LN_LOG_WRITE( format_, ... )    { if ( mLogFile ) { mLogFile->write( ( format_ ), __VA_ARGS__ ); } }
//
//    #define LN_LOG_INIT_BEGIN \
//        mLogFile = init_data_.LogFile; \
//        LN_SAFE_ADDREF( mLogFile ); \
//        LN_LOG_WRITE( __FUNCTION__" 初期化を開始します..." );
//
//    #define LN_LOG_INIT_END \
//        LN_LOG_WRITE( __FUNCTION__" 初期化が完了しました。" );
//
//    #define LN_LOG_FIN_BEGIN \
//        LN_LOG_WRITE( __FUNCTION__" 終了処理を開始します..." );
//
//    #define LN_LOG_FIN_END \
//        LN_LOG_WRITE( __FUNCTION__" 終了処理が完了しました。" ); \
//        LN_SAFE_RELEASE( mLogFile );
//
//#endif
/*
#define LN_LOG_INIT_BEGIN( format_, ... ) \
    mLogFile = init_data_.LogFile; \
    LN_SAFE_ADDREF( mLogFile ); \
    LN_LOG_WRITE( format_ " 初期化を開始します...", __VA_ARGS__ );

#define LN_LOG_INIT_END( format_, ... ) \
    LN_LOG_WRITE( format_ " 初期化が完了しました。", __VA_ARGS__ );

#define LN_LOG_FIN_BEGIN( format_, ... ) \
    LN_LOG_WRITE( format_ " 終了処理を開始します...", __VA_ARGS__ );

#define LN_LOG_FIN_END( format_, ... ) \
    LN_LOG_WRITE( format_ " 終了処理が完了しました。", __VA_ARGS__ ); \
    LN_SAFE_RELEASE( mLogFile );
*/

#define LN_LOG_WRITE( format, ... )     { Base::Logger::println( Base::Logger::LEVEL_INFO, ( format ), ## __VA_ARGS__ ); } 
#define LN_LOG_INIT_BEGIN               LN_LOG_WRITE( "%s initializing...", __FUNCTION__ );
#define LN_LOG_INIT_END                 LN_LOG_WRITE( "%s success initialize.", __FUNCTION__ );
#define LN_LOG_FIN_BEGIN                LN_LOG_WRITE( "%s finalizing...", __FUNCTION__ );
#define LN_LOG_FIN_END                  LN_LOG_WRITE( "%s success finalize.", __FUNCTION__ ); 

namespace LNote
{
namespace Core
{
namespace Threading { class Mutex; }

namespace Base
{

//==============================================================================
// ■ LogFile
//------------------------------------------------------------------------------
///**
//	@brief      ログファイルのクラス
//
//  @par
//              一応スレッドセーフです。
//*/
//==============================================================================
class LogFile
    : public ReferenceObject
{
public:

	/// コンストラクタ
    LogFile();

	/// デストラクタ ( ファイルが開かれていれば閉じる )
    virtual ~LogFile();

public:

	//----------------------------------------------------------------------
	///**
	//  @brief      初期化
	//
	//  @param[in]  log_name_ : ログファイルの名前
	//  @param[in]  log_dir_  : ログファイルを作成するディレクトリ (NULL の場合はカレントディレクトリ)
	//
	//  @retval     true  : 成功
	//  @retval     false : 失敗
	//
	//  @par
	//              現在、Linux では常にカレントディレクトリに作成されます。
	//*/
	//----------------------------------------------------------------------
    bool initialize( const lnChar* log_name_, const lnChar* log_dir_ = NULL );

	//----------------------------------------------------------------------
	///**
	//  @brief      ファイルに追加モードで書き込む ( char )
	//*/
	//----------------------------------------------------------------------
    bool write( const char *format_, ... );

	/// ファイルに追加モードで書き込む ( wchar_t )
    bool write( const wchar_t *format_, ... );

private:

    static const int        TEMP_BUFFER_SIZE = 2048;

    lnChar                  mLogFilePath[ LN_MAX_PATH ];
    Threading::Mutex*          mMutex;
    unsigned int            mLogStartTime;
    wchar_t                 mTempBuffer[ TEMP_BUFFER_SIZE ];    ///< char、wchar_t 共用
};

//==============================================================================
// Logger
//------------------------------------------------------------------------------
///**
//	@brief      ログ出力ユーティリティ
//*/
//==============================================================================
class Logger
{
public:

	/// 警告レベル
    enum Level
    {
        LEVEL_INFO = 0,
        LEVEL_WARNING = 1,
        LEVEL_ERROR = 2
    };

public:

	//----------------------------------------------------------------------
	///**
	//  @brief      初期化
	//
	//  @param[in]  log_name_ : ログファイルの名前
	//  @param[in]  log_dir_  : ログファイルを作成するディレクトリ (NULL の場合はカレントディレクトリ)
	//
	//  @retval     true  : 成功
	//  @retval     false : 失敗
	//
	//  @par
	//              現在、Linux では常にカレントディレクトリに作成されます。
	//*/
	//----------------------------------------------------------------------
    static bool initialize( const lnChar* log_name, const lnChar* log_dir );

	/// ログ出力
    static void print( const char* format, ... );

	/// ログ出力
    static void print( const wchar_t* format, ... );

	/// ログ出力
    static void println( Level level, const char* format, ... );

	/// ログ出力
    static void println( Level level, const wchar_t* format, ... );
};

} // namespace Base
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================