//==============================================================================
// MemoryManager 
//------------------------------------------------------------------------------
///**
//  @file       MemoryManager.h
//  @brief      MemoryManager
//  @author     Riki
//*/
//==============================================================================

#pragma once
#if 1

#if defined(LNOTE_DEBUG) && defined(LNOTE_ENABLE_CRT_DEBUG)
	#include <crtdbg.h>
	#define LN_NEW new(_NORMAL_BLOCK,__FILE__,__LINE__)
#else
    #define LN_NEW new
#endif

#else

// ランタイムDLLを使用してビルドする場合は、new, delete をオーバーロードしないようにしておく。
// こうしておかないと、delete 時に不正アクセスでランタイムエラーとなってしまう。
#if defined(_DLL) || defined(LNOTE_ENABLE_CRT_DEBUG)
	#include <crtdbg.h>
	//#define LN_NEW new
	#define LN_NEW new(_NORMAL_BLOCK,__FILE__,__LINE__)
#else // LNOTE_ENABLE_CRT_DEBUG

#ifdef LNOTE_DEBUG
    #define LN_NEW new( LN_MM_NORMAL_BLOCK, __FILE__, __LINE__ )
#else
    #define LN_NEW new
#endif

#endif // LNOTE_ENABLE_CRT_DEBUG

#endif

namespace LNote
{
namespace Core
{

enum LNMemoryDumpDest
{
    LN_MEMDIST_CONSOLE = 0,     ///< 標準出力に出力する (printf)
    LN_MEMDIST_LOGFILE,         ///< MemoryManager に設定されているログファイルに出力する
    LN_MEMDIST_WIN32DEBUG       ///< Win32 のデバッグ関数で出力する (OutputDebugString)
};

namespace Base
{
class LogFile;

//==============================================================================
// ■ MemoryManager
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class MemoryManager
{
public:

	/// メモリ情報の出力先を設定する
    static void setMemoryDumpDest( LNMemoryDumpDest d_ );

	/// メモリ情報の出力先のログファイルを設定する
    static void setLogFile( Base::LogFile* logfile_ );

    static void attachLogger();
};

} // namesapce Base
} // namespace Core
} // namespace LNote

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
enum LNMMFlag
{
    LN_MM_NORMAL_BLOCK = 1,
};
typedef char LNMMChar;

// VC++ では定義しないとダメ。逆に Android では定義するとダメ。
// LN_CLR_DLL は ReleaseDLL 構成でプロジェクトプロパティから定義される
#if defined(LNOTE_MSVC) && !defined( LN_CLR_DLL )
    void* LNOTE_CDECL operator new( size_t size_ );
    void LNOTE_CDECL operator delete ( void* ptr_ );
#endif

void* LNOTE_CDECL operator new( size_t size_, LNMMFlag flag_, const LNMMChar* filename_, int line_ );
void* LNOTE_CDECL operator new[]( size_t size_, LNMMFlag flag_, const LNMMChar*  filename_, int line_ );
void LNOTE_CDECL operator delete ( void* ptr_, LNMMFlag flag_, const LNMMChar*, int );
void LNOTE_CDECL operator delete[] ( void* ptr_, LNMMFlag flag_, const LNMMChar*, int );


//==============================================================================
//
//==============================================================================