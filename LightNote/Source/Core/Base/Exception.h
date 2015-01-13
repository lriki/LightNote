//==============================================================================
// Exception 
//------------------------------------------------------------------------------
///**
//  @file       Exception.h
//  @brief      Exception
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <assert.h>
#include <string>
#include "Common.h"

#define LN_THROW( exp, type, ... )						{ if (!(exp)) { type e = type( __VA_ARGS__ ); e.setRaisePointData( __FILE__, __LINE__, __FUNCTION__ ); throw e; } }

#define LN_THROW_InvalidOperation( exp, ... )			{ LN_THROW( exp, LNote::Core::Base::InvalidOperationException, ## __VA_ARGS__ ); }
#define LN_THROW_Argument( exp, ... )					{ LN_THROW( exp, LNote::Core::Base::ArgumentException, ## __VA_ARGS__ ); }
#define LN_THROW_OutOfMemory( exp, ... )				{ LN_THROW( exp, LNote::Core::Base::OutOfMemoryException, ## __VA_ARGS__ ); }
#define LN_THROW_FileNotFound( exp, filename_, ... )	{ LN_THROW( exp, LNote::Core::Base::FileNotFoundException, filename_, ## __VA_ARGS__ ); }
#define LN_THROW_SystemCall( exp, ... )					{ LN_THROW( exp, LNote::Core::Base::SystemCallException, ## __VA_ARGS__ ); }
#define LN_THROW_NotImpl( exp, ... )					{ LN_THROW( exp, LNote::Core::Base::NotImplementedException, ## __VA_ARGS__ ); }
#define LN_THROW_InvalidFormat( exp, ... )				{ LN_THROW( exp, LNote::Core::Base::InvalidFormatException, ## __VA_ARGS__ ); }
#define LN_THROW_Fatal( exp, ... )						{ LN_THROW( exp, LNote::Core::Base::FatalException, ## __VA_ARGS__ ); } 
#define LN_THROW_Com( hr, ... )							{ if ( FAILED( hr ) ) { LN_THROW( 0, LNote::Core::Base::ComException, hr, ## __VA_ARGS__ ); } }//throw LNote::Base::ComException          ( _T( __FILE__ ), __LINE__, __FUNCTION__, hr_, __VA_ARGS__ ); } }

// x_ は HRESULT を返す関数呼び出し  例: LN_COMCALL( Test2() );
#define LN_COMCALL( x )									{ hr = ( x ); if ( FAILED( hr ) ) { LN_THROW_Com( hr, _T( "%s\n" ), #x ); } }
#define LN_COMCALLF( x, format, ... )					{ hr = ( x ); if ( FAILED( hr ) ) { LN_THROW_Com( hr, _T( format"\n\n%s\n" ), __VA_ARGS__, #x ); } }

// glGetError() でエラーチェック (各 gl～ の後で必ず呼ばないと正しいエラーが取れないので注意)
#define LN_CHECK_GLERROR( ... )							{ GLenum lnglerr = glGetError(); if ( lnglerr != GL_NO_ERROR ) { LN_THROW( 0, LNote::Core::Base::OpenGLException, lnglerr, ## __VA_ARGS__ ); } }

/// assert。
/// 通常起こりえない問題の発生。
/// 問題を無視して継続した場合、他の場所に影響して不具合となる可能性が高いもの。
#define LN_ERR2_ASSERT_S( expression ) if (!(expression)) { LNote::Core::Base::ExceptionManager::raiseAssert( _T( __FILE__ ), __LINE__, (# expression )); }//assert( expression )	//(void)( (!!(expression)) || /*(::LNote::Core::Base::Error2::raiseAssert( _T( __FILE__ ), __LINE__, (# expression ) ), 0)*/ )

#ifdef LNOTE_DEBUG
    #define LN_ERR2_ASSERT                      LN_ERR2_ASSERT_S
#else
    #define LN_ERR2_ASSERT( expression )
#endif

namespace LNote
{
namespace Core
{
namespace Base
{
class LogFile;
class Exception;

struct ExceptionRaisePointInfo
{
	const char*	File;
	int			Line;
	const char*	Func;
};

//==============================================================================
// ■ (通常、1スレッドにつきひとつ必要。今のところ static だけど…)
//==============================================================================
class ExceptionManager
{
public:

	/// 初期化
    static void initialize();

	/// 終了処理
    static void finalize();

	/// 例外のデフォルト処理
    static void procException( const Exception& exp );

	/// 最後に発生した例外の取得
    static Exception* getLastException() { return mLastException; }

	/// エラーコードに対応する例外を発生させる (関数ライブラリ・HSP用)
	static void raise( ResultCode errCode, const lnChar* message, const lnChar* file, int line, const lnChar* func );

	static void raiseAssert(const lnChar* filename, const int line, const lnChar* format, ...);


	lnRefStringW makeOutputMessage(const lnChar* caption, const wchar_t* errorMsg);
	lnRefStringW makeOutputMessage(const wchar_t* caption, const wchar_t* errorMsg);

private:
    static void setLastException( const Exception* exp );

private:

	friend class Exception;
    static Exception* mLastException;

};

//==============================================================================
// Exception
//------------------------------------------------------------------------------
///**
//  @brief      全ての例外の基底
//*/
//==============================================================================
class Exception
{
public:
    Exception();
    virtual ~Exception() {}

public:
	const wchar_t* getMessage() const { return mErrorMessage.c_str(); }

	virtual LNRESULT getErrorCode() const { return ResultCode_Unknown; }

	/// 例外のデフォルト処理
    void process() const;

	void setRaisePointData(const char* file, int line, const char* func);
	const ExceptionRaisePointInfo& setRaisePointData() const { return mRaisePointInfo; }

public:
	/// 例外のコピーを作成する (別スレッドで発生した例外を保持するために使用)
	virtual Exception* copy() const { return LN_NEW Exception(*this); }

protected:
	Exception(const Exception& exp);
	Exception& operator = (const Exception& exp);

protected:
	LRefWString				mErrorMessage;
	ExceptionRaisePointInfo	mRaisePointInfo;
};

//==============================================================================
/// 無効な関数呼び出し

/**
 *	@brief	オブジェクトの現在の状態に対して無効な呼び出しが行われた
 */
class InvalidOperationException
    : public Exception
{
public:
	InvalidOperationException();
	InvalidOperationException(const lnChar* format, ...);

public:
	// override Exception
	virtual LNRESULT getErrorCode() const { return ResultCode_InvalidOperation; }
	virtual Exception* copy() const { return LN_NEW InvalidOperationException(*this); }
};

/**
 *	@brief	メソッドに渡された引数のいずれかが無効
 */
class ArgumentException
	: public Exception
{
public:
	ArgumentException();
	ArgumentException(const lnChar* format, ...);

public:
	// override Exception
	virtual LNRESULT getErrorCode() const { return ResultCode_Argument; }
	virtual Exception* Copy() const { return LN_NEW ArgumentException(*this); }
};

//==============================================================================
/// メモリ不足 (malloc() またはLN_NEW で確保する用。普通の new は bad_alloc)
class OutOfMemoryException
    : public Exception
{
public:
    OutOfMemoryException();
	OutOfMemoryException(const lnChar* format_, ...);

public:
	// override Exception
	virtual LNRESULT getErrorCode() const { return ResultCode_OutOfMemory; }
	virtual Exception* copy() const { return LN_NEW OutOfMemoryException(*this); }
};

//==============================================================================
/// ファイルが見つからなかった
class FileNotFoundException
    : public Exception
{
public:
    FileNotFoundException( const lnChar* filename_ );
    FileNotFoundException( const lnChar* filename_, const lnChar* format_, ... );
	FileNotFoundException( const wchar_t* filePath );

public:
	// override Exception
	virtual LNRESULT getErrorCode() const { return ResultCode_FileNotFound; }
	virtual Exception* copy() const { return LN_NEW FileNotFoundException(*this); }
};

//==============================================================================
/// システムコールエラー (API 呼び出しエラー)
class SystemCallException
    : public Exception
{
public:
    SystemCallException();
	SystemCallException(const lnChar* format, ...);

public:
	// override Exception
	virtual LNRESULT getErrorCode() const { return ResultCode_SystemCall; }
	virtual Exception* copy() const { return LN_NEW SystemCallException(*this); }
};

//==============================================================================
/// 未実装の機能
class NotImplementedException
	: public Exception
{
public:
    NotImplementedException();
	NotImplementedException(const lnChar* format, ...);

public:
	// override Exception
	virtual LNRESULT getErrorCode() const { return ResultCode_NotImplemented; }
	virtual Exception* copy() const { return LN_NEW NotImplementedException(*this); }
};

//==============================================================================
/**
 *	@brief		ファイルや文字列等の形式が不正
 */
class InvalidFormatException
    : public Exception
{
public:
	InvalidFormatException();
	InvalidFormatException(const lnChar* format, ...);

public:
	// override Exception
	virtual LNRESULT getErrorCode() const { return ResultCode_InvalidFormat; }
	virtual Exception* copy() const { return LN_NEW InvalidFormatException(*this); }
};

/**
 *	@brief		スレッド実行中にハンドルされない例外(このライブラリ以外の例外)が発生した
 */
class ThreadException
	: public Exception
{
public:
	ThreadException();
	ThreadException(const lnChar* format, ...);

public:
	// override Exception
	virtual LNRESULT getErrorCode() const { return ResultCode_Thread; }
	virtual Exception* copy() const { return LN_NEW ThreadException(*this); }
};

/**
 *	@brief		継続不能な内部エラー
 *	@details	外部ライブラリの初期化失敗等、原因がわからず対策が不可能なもの。
 */
class FatalException
    : public Exception
{
public:
    FatalException();
	FatalException(const lnChar* format, ...);

public:
	// override Exception
	virtual LNRESULT getErrorCode() const { return ResultCode_Fatal; }
	virtual Exception* copy() const { return LN_NEW FatalException(*this); }
};



#if LNOTE_INCLUDED_DIRECTX_HEADER
//==============================================================================
/// COM 呼び出しエラー (HRESULT を戻り値とする呼び出しエラー)
class ComException
    : public Exception
{
public:
    ComException( HRESULT hr );
	ComException(HRESULT hr, const lnChar* format, ...);

public:
	// override Exception
	virtual LNRESULT getErrorCode() const { return ResultCode_ComCall; }
	virtual Exception* copy() const { return LN_NEW ComException(*this); }
};
#endif

//==============================================================================
/// OpenGL エラー
class OpenGLException
    : public Exception
{
public:
    OpenGLException( lnU32 gl_enum );    //GLenum
    OpenGLException( lnU32 gl_enum, const lnChar* format, ... );
    lnU32 getGLErrorCode() const { return mGLErrorCode; }

public:
	static const lnChar* convertGLErrorToString(lnU32 gl_enum);

public:
	// override Exception
	virtual LNRESULT getErrorCode() const { return ResultCode_OpenGL; }
	virtual Exception* copy() const { return LN_NEW OpenGLException(*this); }

private:
    lnU32   mGLErrorCode;
};

#ifdef LNOTE_WIN32
//==============================================================================
// DirectXException
//------------------------------------------------------------------------------
///**
//  @brief      DirectX のエラーを示す例外クラス
//*/
//==============================================================================
class DirectXException
	: public Exception
{
public:
	DirectXException(HRESULT hr);
	DirectXException(const lnChar* format, ...);
	virtual ~DirectXException() {}

public:
	DWORD			getHRESULT() const { return mHRESULT; }

public:
	// override Exception
	virtual LNRESULT getErrorCode() const { return ResultCode_DirectX; }
	virtual Exception* copy() const { return LN_NEW DirectXException(*this); }

private:
	HRESULT		mHRESULT;
};

//==============================================================================
// Win32Exception
//------------------------------------------------------------------------------
///**
//  @brief      WindowsAPI のエラーを示す例外クラス (GetLastError())
//*/
//==============================================================================
class Win32Exception
	: public Exception
{
public:
	Win32Exception(DWORD dwLastError);
	Win32Exception(DWORD dwLastError, const lnChar* format, ...);
	virtual ~Win32Exception() {}

public:
	DWORD			getLastErrorCode() const { return m_dwLastErrorCode; }
	const TCHAR*	getFormatMessage() const { return m_pFormatMessage; }

public:
	// override Exception
	virtual LNRESULT getErrorCode() const { return ResultCode_WinAPI; }
	virtual Exception* copy() const { return LN_NEW Win32Exception(*this); }

private:
	DWORD		m_dwLastErrorCode;
	TCHAR		m_pFormatMessage[512];	///< FormatMessage() で取得したメッセージ
};
#endif

} // namespace Base
} // namespace Core
} // namespace LNote
