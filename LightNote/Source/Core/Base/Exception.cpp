//==============================================================================
//  Exception 
//==============================================================================


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Resource/LNResource.h"
#include "../Resource/LocaleManager.h"
#include "../System/Environment.h"
#include "../Threading/Mutex.h"
#include "Exception.h"

#if defined(LNOTE_ANDROID)
    #include <EGL/egl.h>
    #include <EGL/eglext.h>
    #include <GLES/gl.h>
    #include <GLES/glext.h> // for glGenFramebuffersOES
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#endif

#include <GL/gl.h>

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Base
{

//==============================================================================
// ■ ExceptionManager
//==============================================================================

    Exception*       ExceptionManager::mLastException = NULL;
    static Threading::Mutex  gExceptionManagerMutex;

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ExceptionManager::initialize()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ExceptionManager::finalize()
    {
		SAFE_DELETE(mLastException);
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ExceptionManager::procException( const Exception& exp )
    {
        if ( exp.getErrorCode() )
        {
            gExceptionManagerMutex.lock();

			// 発生位置情報を文字列化
			char raisePointInfo[1024] = { 0 };
			snprintf(
				raisePointInfo, 1024, 
				"file:%s\nline:%d\nfunc:%s", 
				exp.setRaisePointData().File,
				exp.setRaisePointData().Line,
				exp.setRaisePointData().Func);

			// wchar_t へ
			wchar_t raisePointInfoW[1024] = { 0 };
			size_t len;
			mbstowcs_s(&len, raisePointInfoW, 1024, raisePointInfo, _TRUNCATE);

			// 結合
			wchar_t outputMessage[2048] = { 0 };
			wcscpy_s(outputMessage, 2048, exp.getMessage());
			wcscat_s(outputMessage, 2048, L"\n\n");
			wcscat_s(outputMessage, 2048, raisePointInfoW);

			// コンソールに表示しておく
			wprintf(L"--------------------------------\n");
			wprintf(L"%s\n", outputMessage);
			wprintf(L"--------------------------------\n");

			// ログファイルへ
            LN_LOG_WRITE(
                L"\n-------------------------------------------------------\n[ Error ]\n%s\n-------------------------------------------------------\n",
				outputMessage);

			// メッセージボックスへ
			System::Environment::showMessageBox(NULL, outputMessage, L"Exception");
            gExceptionManagerMutex.unlock();
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ExceptionManager::setLastException( const Exception* exp )
    {
        gExceptionManagerMutex.lock();
		SAFE_DELETE(mLastException);
        mLastException = exp->copy();
        gExceptionManagerMutex.unlock();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ExceptionManager::raise( ResultCode errCode, const lnChar* message, const lnChar* file, int line, const lnChar* func )
	{
		switch (errCode)
		{
		case ResultCode_FileNotFound:
			{
				FileNotFoundException e( message );
				e.setRaisePointData( file, line, func );
				throw e;
			}
		default:
			{
				InvalidOperationException e(message);
				e.setRaisePointData( file, line, func );
				throw e;
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ExceptionManager::raiseAssert(const char* filename, const int line, const char* format, ...)
	{
		static const int		CONTENTS_BUFFER_SIZE = 512;    ///< 内容、行、ファイル名部分のバッファサイズ
		const lnChar*  FORMAT_STRING =
			_T("%s\n")          // setLastErrorString() で設定された文字列
			//_T( "%s\n" )          // ミドルウェア等毎のエラー文字列 ( DirectX のエラー文字列とか )
			_T("\n")
			_T("file : %s\n")
			_T("line : %d\n");


		// 旧処理からそのまま持ってきたので未改名
		lnChar         sErrorContentsString[CONTENTS_BUFFER_SIZE] = { 0 };    ///< ヘッダ、フッタ以外の文字列
		lnChar         sPreFormatString[CONTENTS_BUFFER_SIZE] = { 0 };    ///< setPreErrorFormat() で設定される


		//if ( !sErrorHold || sLastErrorString[ 0 ] == _T( '\0' ) )
		{
			// ファイルパスのうち、ファイル名の部分だけを取る
			int i = _tcslen(filename);
			for (; (i > 0) && filename[i] != _T('\\') && filename[i] != _T(':'); --i){}
			const lnChar* fn = &filename[i + 1];

			// ファイル名の長さ + 行数 (20 桁まで) + '\n' + '\0'
			int meta_len = _tcslen(fn) + 20 + 1 + 1;

			// 先頭の # チェック (特に何か処理を行っているわけではない。昔の名残)
			const lnChar* format_top;
			if (format[0] == _T('#'))
			{
				format_top = &(format[1]);
			}
			else
			{
				format_top = &(format[0]);
			}

			// 可変引数から文字列を作って tmp_buf に格納する
			// シェーダのエラーとかは 1000 を余裕で超えることがあるので動的確保
			va_list args;
			va_start(args, format);

			// gcc にはこれに相当する関数がなかったので、今はコレで。
			int args_len = 2048;//_vsctprintf( format_, args ) + 1;
			lnChar* tmp_buf = static_cast< lnChar* >(malloc(args_len * sizeof(lnChar)));
			_vstprintf_s(tmp_buf, args_len, format_top, args);

			// CONTENTS_BUFFER_SIZE を超える場合は切る
			if (args_len > CONTENTS_BUFFER_SIZE)
			{
				tmp_buf[CONTENTS_BUFFER_SIZE - meta_len - 5] = _T(' ');
				tmp_buf[CONTENTS_BUFFER_SIZE - meta_len - 4] = _T('.');
				tmp_buf[CONTENTS_BUFFER_SIZE - meta_len - 3] = _T('.');
				tmp_buf[CONTENTS_BUFFER_SIZE - meta_len - 2] = _T('.');
				tmp_buf[CONTENTS_BUFFER_SIZE - meta_len - 1] = _T('\0');
			}

			_tcscpy(sErrorContentsString, tmp_buf);

			SAFE_FREE(tmp_buf);

			va_end(args);

			// 全体を結合したときの長さがバッファサイズ以上になる場合は、args_len を縮め、文字列を途中で区切る
			if (meta_len + args_len >= CONTENTS_BUFFER_SIZE)
			{
				args_len = (CONTENTS_BUFFER_SIZE - meta_len) - 1;
				sErrorContentsString[args_len] = _T('\0');
			}

			// sPreFormatString に書き出す
			_stprintf_s(sPreFormatString, CONTENTS_BUFFER_SIZE, FORMAT_STRING, sErrorContentsString, fn, line);

			System::Environment::showMessageBox(NULL, sPreFormatString, LNOTE_NAME _T(" Error"));
			assert(0 && sPreFormatString);
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	lnRefStringW ExceptionManager::makeOutputMessage(const lnChar* caption, const wchar_t* errorMsg)
	{
		LRefWString wstr(caption);
		return makeOutputMessage(wstr.c_str(), errorMsg);
		/*
		LRefTString tmp;
		tmp.format(
		_T( "%s\n\n%s" ),
		caption_,
		mErrorMessage.c_str() );
		mOutputMessage = tmp.c_str();

		ExceptionManager::setLastException( *this );
		*/
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	lnRefStringW ExceptionManager::makeOutputMessage(const wchar_t* caption, const wchar_t* errorMsg)
	{
		LRefWString tmp;
		tmp.format(
			L"%s\n\n%s",
			caption,
			errorMsg);
			//mErrorMessage.c_str());
		return tmp;
		//mOutputMessage = tmp.c_str();

		// 関数ライブラリの GetLastError 対応のため、
		// Manager::procException() 以前で設定しておく必要がある
		//ExceptionManager::setLastException(this);
	}



    static LRefWString makeErrorString(
		int captionStrResID,
        const char* format,
        va_list args )
    {
        // 先頭が # の場合はその次の文字から
        const lnChar* format_top;
        if ( format[ 0 ] == _T( '#' ) ) {
			format_top = &(format[1]);
        }
        else {
			format_top = &(format[0]);
        }

		static const int CONTENTS_BUFFER_SIZE = 1024;
		char tmpBuf[CONTENTS_BUFFER_SIZE + 1] = { 0 };
		int validLen = vsnprintf(tmpBuf, CONTENTS_BUFFER_SIZE, format, args);	// 書き込んだ文字数が帰る (終端ナル文字は含めない)

        // CONTENTS_BUFFER_SIZE を超える場合は終端を ... にする
		if (validLen >= CONTENTS_BUFFER_SIZE)
        {
			tmpBuf[CONTENTS_BUFFER_SIZE - 5] = _T(' ');
			tmpBuf[CONTENTS_BUFFER_SIZE - 4] = _T('.');
			tmpBuf[CONTENTS_BUFFER_SIZE - 3] = _T('.');
			tmpBuf[CONTENTS_BUFFER_SIZE - 2] = _T('.');
			tmpBuf[CONTENTS_BUFFER_SIZE - 1] = _T('\0');
        }

		// wchar_t へ
		wchar_t tmpBufW[CONTENTS_BUFFER_SIZE + 1] = { 0 };
		size_t validLenW;
		mbstowcs_s(&validLenW, tmpBufW, 1024, tmpBuf, _TRUNCATE);

		// 結合
		wchar_t outputMessage[2048] = { 0 };
		wcscpy_s(outputMessage, 2048, Resource::LocaleManager::getString(captionStrResID));
		wcscat_s(outputMessage, 2048, L"\n");
		wcscat_s(outputMessage, 2048, tmpBufW);

		return LRefWString(outputMessage);
    }

	//--------------------------------------------------------------------------
	// Exception

    Exception::Exception()
    {
		//_CrtDbgBreak();
		// 今のところ、FileManager が Archive 内を検索するときに無ければ例外を投げているので、
		// それを例外使わないようにしないと、ここで落ちる。

		memset(&mRaisePointInfo, 0, sizeof(mRaisePointInfo));
    }

    Exception::Exception( const Exception& e )
    {
        mErrorMessage = e.mErrorMessage;
		mRaisePointInfo = e.mRaisePointInfo;
    }

    Exception& Exception::operator = ( const Exception& e )
	{
		mErrorMessage = e.mErrorMessage;
		mRaisePointInfo = e.mRaisePointInfo;
        return *this;
    }

	// 例外のデフォルト処理
    void Exception::process() const
    {
         ExceptionManager::procException( *this );
    }

	void Exception::setRaisePointData( const char* file, int line, const char* func )
	{
		if ( file == NULL ) return;

		mRaisePointInfo.File = file;
		mRaisePointInfo.Line = line;
		mRaisePointInfo.Func = func;

		// 文字列を変更したので更新
		ExceptionManager::setLastException(this);
	}

//--------------------------------------------------------------------------
// InvalidOperationException
InvalidOperationException::InvalidOperationException()
{
}
InvalidOperationException::InvalidOperationException(const lnChar* format, ...)
{
    va_list args;
    va_start( args, format );
	mErrorMessage = makeErrorString(Resource::String_ERR_InvalidOperationException, format, args);
    va_end( args );
}

//--------------------------------------------------------------------------
// ArgumentException
ArgumentException::ArgumentException()
{
}
ArgumentException::ArgumentException(const lnChar* format, ...)
{
	va_list args;
	va_start(args, format);
	mErrorMessage = makeErrorString(Resource::String_ERR_ArgumentException, format, args);
	va_end(args);
}

//--------------------------------------------------------------------------
// OutOfMemoryException
OutOfMemoryException::OutOfMemoryException()
{
}
OutOfMemoryException::OutOfMemoryException( const lnChar* format, ... )
{
    va_list args;
    va_start( args, format );
	mErrorMessage = makeErrorString(Resource::String_ERR_OutOfMemoryException, format, args);
    va_end( args );
}

//--------------------------------------------------------------------------
// FileNotFoundException
FileNotFoundException::FileNotFoundException( const lnChar* filename )
{
	mErrorMessage = Resource::LocaleManager::getString(Resource::String_ERR_FileNotFoundException);
	mErrorMessage += L"\n";
    mErrorMessage += lnRefStringW(filename).c_str();
}
FileNotFoundException::FileNotFoundException( const lnChar* filename, const lnChar* format, ... )
{
    va_list args;
    va_start( args, format );
    mErrorMessage = filename;
    mErrorMessage += L"\n\n";
	mErrorMessage += makeErrorString(Resource::String_ERR_FileNotFoundException, format, args).c_str();
    va_end( args );
}
FileNotFoundException::FileNotFoundException( const wchar_t* filePath )
{
	mErrorMessage = Resource::LocaleManager::getString(Resource::String_ERR_FileNotFoundException);
	mErrorMessage += L"\n";
	mErrorMessage += filePath;
}

//--------------------------------------------------------------------------
// SystemCallException
SystemCallException::SystemCallException()
{
}
SystemCallException::SystemCallException( const lnChar* format, ... )
{
    va_list args;
    va_start( args, format );
	mErrorMessage = makeErrorString(Resource::String_ERR_SystemCallException, format, args);
    va_end( args );
}

//--------------------------------------------------------------------------
// NotImplementedException
NotImplementedException::NotImplementedException()
{
}
NotImplementedException::NotImplementedException( const lnChar* format, ... )
{
    va_list args;
    va_start( args, format );
	mErrorMessage = makeErrorString(Resource::String_ERR_NotImplementedException, format, args);
    va_end( args );
}

//--------------------------------------------------------------------------
// InvalidFormatException
InvalidFormatException::InvalidFormatException()
{
}
InvalidFormatException::InvalidFormatException(const lnChar* format, ...)
{
    va_list args;
    va_start( args, format );
	mErrorMessage = makeErrorString(Resource::String_ERR_InvalidFormatException, format, args);
    va_end( args );
}

//--------------------------------------------------------------------------
// ThreadException
ThreadException::ThreadException()
{
}
ThreadException::ThreadException(const lnChar* format, ...)
{
	va_list args;
	va_start(args, format);
	mErrorMessage = makeErrorString(Resource::String_ERR_ThreadException, format, args);
	va_end(args);
}

//--------------------------------------------------------------------------
// FatalException
FatalException::FatalException()
{
}
FatalException::FatalException( const lnChar* format, ... )
{
    va_list args;
    va_start( args, format );
	mErrorMessage = makeErrorString(Resource::String_ERR_FatalException, format, args);
    va_end( args );
}

    
#if defined(LNOTE_WIN32)
//--------------------------------------------------------------------------
// ComException
ComException::ComException( HRESULT hr )
{
    LRefTString tmp;
    tmp.format( _T( "HRESULT 0x%x" ), hr );
    mErrorMessage = tmp;
}
ComException::ComException( HRESULT hr, const lnChar* format, ... )
{
    va_list args;
    va_start( args, format );
	mErrorMessage = makeErrorString(Resource::String_ERR_ComException, format, args);
    va_end( args );

    LRefWString tmp;
    tmp.format( L"\nHRESULT 0x%x", hr );
    mErrorMessage += tmp.c_str();
}
#endif

//--------------------------------------------------------------------------
// OpenGLException
OpenGLException::OpenGLException( lnU32 gl_err )
{
    mGLErrorCode = gl_err;
        
    LRefTString tmp;
    tmp.format( _T( "GLenum 0x%x\n%s" ), gl_err, convertGLErrorToString(gl_err) );
    mErrorMessage = tmp;
}
OpenGLException::OpenGLException( lnU32 gl_err, const lnChar* format, ... )
{
    mGLErrorCode = gl_err;
        
    va_list args;
    va_start( args, format );
    mErrorMessage = makeErrorString(Resource::String_ERR_OpenGLException, format, args );
    va_end( args );

    LRefWString tmp;
    tmp.format( L"GLenum 0x%x\n%s", gl_err, convertGLErrorToString(gl_err) );
    mErrorMessage += tmp.c_str();
}
const lnChar* OpenGLException::convertGLErrorToString(lnU32 gl_enum_)
{
    switch (gl_enum_)
    {
        case GL_INVALID_ENUM: return _T("GL_INVALID_ENUM");
        case GL_INVALID_VALUE: return _T("GL_INVALID_VALUE");
        case GL_INVALID_OPERATION: return _T("GL_INVALID_OPERATION");
#if defined(LNOTE_GLES)
        case GL_INVALID_FRAMEBUFFER_OPERATION_OES: return _T("GL_INVALID_FRAMEBUFFER_OPERATION_OES");
#endif
    }
    return _T("");
}


#ifdef LNOTE_WIN32
//==============================================================================
// DirectXException
//==============================================================================
DirectXException::DirectXException(HRESULT hr)
	:mHRESULT(hr)
{
	LRefTString tmp;
	tmp.format(_T("HRESULT 0x%x"), hr);
	mErrorMessage = tmp;
}
DirectXException::DirectXException(const lnChar* format, ...)
	: mHRESULT(0)
{
	va_list args;
	va_start(args, format);
	mErrorMessage = makeErrorString(Resource::String_ERR_ComException, format, args);
	va_end(args);
}

//==============================================================================
// Win32Exception
//==============================================================================
Win32Exception::Win32Exception(DWORD dwLastError)
	: m_dwLastErrorCode(dwLastError)
{
	// エラーメッセージ取得
	memset(m_pFormatMessage, 0, sizeof(m_pFormatMessage));
	DWORD dwRet = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM, NULL, m_dwLastErrorCode,
		0, m_pFormatMessage, sizeof(m_pFormatMessage) / sizeof(m_pFormatMessage[0]), NULL);
}
Win32Exception::Win32Exception(DWORD dwLastError, const lnChar* format, ...)
	: m_dwLastErrorCode(dwLastError)
{
	// エラーメッセージ取得
	memset(m_pFormatMessage, 0, sizeof(m_pFormatMessage));
	DWORD dwRet = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM, NULL, m_dwLastErrorCode,
		0, m_pFormatMessage, sizeof(m_pFormatMessage) / sizeof(m_pFormatMessage[0]), NULL);

    va_list args;
	va_start(args, format);
	mErrorMessage = makeErrorString(Resource::String_ERR_SystemCallException, format, args);
    va_end( args );
}

#endif

} // namespace Base
} // namespace Core
} // namespace LNote
