//==============================================================================
// Common 
//------------------------------------------------------------------------------
///**
//  @file       Common.h
//  @brief      Common
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

/// 結果・エラーコード
///	今後これはすべて削除予定。Core は 例外でエラーを管理し、C_API を通して外部に出すときにエラーコードに変換する。
enum ResultCode
{
	ResultCode_OK				 = 0,

	ResultCode_Unknown			= -1,
	ResultCode_InvalidOperation = -2,
	ResultCode_Argument			= -3,
	ResultCode_OutOfMemory		= -4,	///< メモリ不足
	ResultCode_FileNotFound		= -5,	///< ファイルが無効
	ResultCode_SystemCall		= -6,	///< システム API 呼び出しエラー
	ResultCode_NotImplemented	= -7,	///< 実装されていない機能を呼び出そうとした
	ResultCode_InvalidFormat	= -8,
	ResultCode_Compilation		= -9,
	ResultCode_Thread			= -10,
	ResultCode_Fatal			= -11,	///< 継続不能なエラー
	ResultCode_ComCall			= -12,	///< COM 呼び出しエラー
	ResultCode_OpenGL			= -13,	///< OpenGL 関係のエラー
	ResultCode_DirectX			= -14,
	ResultCode_WinAPI			= -15,



#if 0
	
    LN_ERR_INVALIDCALL			= 0x00042002,   ///< 関数の引数が無効
    LN_ERR_OUTOFMEMORY			= 0x00042003,   ///< メモリ不足
    LN_ERR_FILE_NOT_FOUND       = 0x00043001,   ///< ファイルが無効
    LN_ERR_FORMAT_ERROR         = 0x00043002,   ///< 非対応ファイル形式
    LN_ERR_PASSWORD_NOT_MATCH   = 0x00043003,   ///< パスワードが一致しなかった
	LN_ERR_SYSTEM_API           = 0x00043007,   ///< システム API 呼び出しエラー
    LN_ERR_NOTIMPL				= 0x8004200c,   ///< 実装されていない機能を呼び出そうとした
    LN_ERR_NOINITIALIZE			= 0x8004200a,   ///< 初期化されていない
    LN_ERR_FATAL				= 0x8004200d,   ///< 継続不能なエラー
	LN_ERR_COM					= 0x8004200e,	///< COM 呼び出しエラー
	LN_ERR_OPENGL				= 0x8004200f,   ///< OpenGL 関係のエラー
	//Decoding failure







    //LN_FALSE                = 1,
    LN_HANDLED              = 0x00042002,    ///< 処理済み (OSメッセージ・GUI 用。削除予定)

    LN_ERR_ASSERT           = 0x00042001,   ///< 警告 (LN_ASSERT)
	//LN_ERR_INVALIDCALL      = 0x80042002,   ///< 関数の引数が無効
	//LN_ERR_OUTOFMEMORY      = 0x80042003,   ///< メモリ不足
    LN_ERR_INDEX            = LN_ERR_INVALIDCALL,   ///< 配列外参照 (LN_ERR_INVALIDCALL に統合予定)
    LN_ERR_NULLPOINTER      = 0x00042005,   ///< NULL アクセス
	//LN_ERR_FILENOTFOUND     = 0x80042006,   ///< ファイルが無効
    LN_ERR_SYSTEM           = 0x00042007,   ///< システム API 呼び出しエラー
    LN_ERR_INITIALIZED      = 0x00042009,   ///< 初期化済み
	//LN_ERR_NOINITIALIZE     = 0x8004200a,   ///< 初期化されていない
    LN_ERR_FORMAT           = 0x0004200b,   ///< 非対応ファイル形式

	LN_ERR_THREAD			= -1,
	//LN_ERR_NOTIMPL          = 0x8004200c,   ///< 実装されていない機能を呼び出そうとした
	//LN_ERR_FATAL            = 0x8004200d,   ///< 継続不能なエラー
    LN_ERR_DIRECTX          = 0x0004200e,   ///< DirectX 関係のエラー
	//LN_ERR_OPENGL           = 0x8004200f,   ///< OpenGL 関係のエラー

	//LN_ERR_COM  = LN_ERR_DIRECTX,

	// ※コンバータでの変換を容易にするため、MAKE_HRESULT ではなく直値で指定
#endif
};

} // namespace LNote

//==============================================================================
//
//==============================================================================