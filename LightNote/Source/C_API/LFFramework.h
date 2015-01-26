
#pragma once

#include "LFCommon.h"
#include "LFTypedef.h"

extern "C" {

/**
	@brief	LightNote の初期化やフレーム更新、終了処理等のアプリケーション全体に関わる機能です。
*/
LN_MODULE(Application)

//==============================================================================
/**
	@brief	LightNote の初期設定を行うための関数郡です。
*/
LN_STATIC_CLASS(LNConfig)
	
	/**
		@brief		デフォルトで作成されるウィンドウのタイトル文字列を設定します。
		@param[in]	title	: ウィンドウタイトル文字列
	*/
	LN_STATIC_API
	void LNConfig_SetWindowTitle(const LNChar* title);

	/**
		@brief		デフォルトで作成されるウィンドウのサイズ (クライアント領域) を設定します。
		@param[in]	width	: クライアント領域の幅 (default:640)
		@param[in]	height	: クライアント領域の高さ (default:480)
	*/
	LN_STATIC_API
	void LNConfig_SetWindowSize(int width, int height);

	/**
		@brief		デフォルトで作成されるバックバッファのサイズを設定します。
		@param[in]	width	: バックバッファの幅 (default:0)
		@param[in]	height	: バックバッファの高さ (default:0)
		@details	ウィンドウサイズとは異なるサイズでバックバッファを作成する時、
					バックバッファのサイズを指定します。<br>
					サイズが 0 の場合、ウィンドウサイズと同じサイズで作成します。
	*/
	LN_STATIC_API
	void LNConfig_SetBackbufferSize(int width, int height);
	
	/**
		@brief		フレームレートを設定します。
		@param[in]	frameRate   : フレームレート (default:60)
	*/
	LN_STATIC_API
	void LNConfig_SetFrameRate(int frameRate);

	/**
		@brief		ライブラリ更新時時のフレーム待ち処理の有無を設定します。
		@param[in]	enabled   : LN_TRUE=有効 / LN_FALSE=無効 (default:LN_TRUE)
	*/
	LN_STATIC_API
	void LNConfig_SetEnableFrameWait(LNBool enabled);

	/**
		@brief		自動更新時のタイムスケールを設定します。
		@param[in]	timeScale   : タイムスケール (default:0.0)
		@details	SceneNode や ViewFilter の自動フレーム更新の際、
					前回フレームからの経過時間に乗算される値を設定します。
					この値を 1.0 にすると、エフェクト等の遷移時間を時間単位、
					フレームレートと同じ値にするとフレーム単位で指定できるようになります。
					0.0 の場合、フレームレートの値を使用します。
	*/
	LN_STATIC_API
	void LNConfig_SetAutoUpdateTimeScale(float timeScale);

	/**
		@brief		デフォルトで作成されるウィンドウがリサイズ可能であるかを設定します。
		@param[in]	enabled	: LN_TRUE の場合、ウィンドウ枠をD&Dすることでサイズ変更が可能 (default:LN_TRUE)
	*/
	LN_STATIC_API
	void LNConfig_SetWindowResizeEnabled(LNBool enabled);

	/**
		@brief		アクセラレータキーとして割り当てるキーを設定します。
		@param[in]	acceleratorKey	: アクセラレータの種別
		@param[in]	keyCode			: 割り当てるキーコード
		@param[in]	isShift			: LN_TRUE を指定すると、Shift キーを修飾子とする
		@param[in]	isCtrl			: LN_TRUE を指定すると、Ctrl キーを修飾子とする
		@param[in]	isAlt			: LN_TRUE を指定すると、Alt キーを修飾子とする
		@details	初期状態では以下のように割り当てられています。<br>
					- LN_ACCELERATORKEY_TOGGLE_FULLSCREEN<br>
						Alt + Enter キー
					- LN_ACCELERATORKEY_SHOW_DEBUG_STATISTICS<br>
						なし
	*/
	LN_STATIC_API
	void LNConfig_SetAcceleratorKey(LNAcceleratorKey acceleratorKey, LNKeyCode keyCode, LNBool isShift, LNBool isCtrl, LNBool isAlt);

	/**
		@brief		ログファイルの出力有無を設定します。
		@param[in]	flag    : LN_TRUE=ログファイルを出力する / LN_FALSE=しない (default:LN_FALSE)
	*/
	LN_STATIC_API
	void LNConfig_SetLogFileEnabled(LNBool flag);

	/**
		@brief		コンソールウィンドウの割り当て有無を設定します。
		@param[in]	flag    : LN_TRUE=割り当てる / LN_FALSE=割り当てない (default:LN_FALSE)
	*/
	LN_STATIC_API
	void LNConfig_SetConsoleEnabled(LNBool flag);

	/**
		@brief		ファイルを開くときにアクセスする暗号化アーカイブを登録します。
		@param[in]	filePath	: アーカイブファイル名
		@param[in]	password	: アーカイブファイルを開くためのパスワード
	*/
	LN_STATIC_API
	void LNConfig_RegisterArchive(const LNChar* filePath, const LNChar* password);

	/**
		@brief		ファイルへのアクセス優先順位を設定します。
		@param[in]	priority	: 制限方法 (default:LN_FILEACCESSPRIORITY_DIRECTORY_FIRST)
	*/
	LN_STATIC_API
	void LNConfig_SetFileAccessPriority(LNFileAccessPriority priority);

	/**
		@brief		DirectMusic の初期化方法を設定します。
		@param[in]	mode    : DirectMusic の初期化方法 (default:LN_DMINITMODE_NOT_USE)
	*/
	LN_STATIC_API
	void LNConfig_SetDirectMusicInitMode(LNDirectMusicInitMode mode);

	/**
		@brief		音声データの最大キャッシュ数を設定します。
		@param[in]	count   : 最大数 (default:32)
	*/
	LN_STATIC_API
	void LNConfig_SetSoundMaxCacheCount(int count);

	/**
		@brief		音声再生方法の自動選択の音声データバイト数閾値を設定します。
		@param[in]	bytes   : 最大数 (default:100000)
		@details	再生方法(オンメモリorストリーミング)の自動選択時、
					音声データのバイト数がこの閾値よりも大きい場合ストリーミング、
					以下の場合はオンメモリで再生されます。
					MP3等の圧縮フォーマットではデコード後のサイズでチェックします。
	*/
	LN_STATIC_API
	void LNConfig_SoundPlayTypeThreshold(int bytes);

	/**
		@brief		使用するフォントファイルを登録使用します。
		@param[in]	filePath	: フォントファイル名
		@details	登録したフォントファイルはフォントオブジェクト作成時に
						ファミリ名を指定することで使用できます。
	*/
	LN_STATIC_API
	void LNConfig_RegisterFontFile(const LNChar* filePath);

	/**
		@brief		テクスチャの最大キャッシュ数を設定します。
		@param[in]	count		: 最大数 (default:32)
	*/
	LN_STATIC_API
	void LNConfig_SetTextureCacheSize(int count);

	/**
		@brief		シェーダの最大キャッシュ数を設定します。
		@param[in]	count		: 最大数 (default:32)
	*/
	LN_STATIC_API
	void LNConfig_SetShaderCacheSize(int count);

	/**
		@brief		例外エラー発生時のメッセージボックス表示有無を設定します。
		@param[in]	enabled		: LN_TRUE の場合、表示する (default:LN_TRUE)
	*/
	LN_STATIC_API
	void LNConfig_SetExceptionMessageBoxEnabled(LNBool enabled);

	/**
		@brief		ユーザー定義のウィンドウハンドルを設定します。
		@param[in]	hWnd	: ユーザー定義のウィンドウハンドル (default:NULL)
	*/
	LN_STATIC_API
	void LNConfig_SetWin32UserWindowHandle(intptr_t hWnd);
	/*Option
		@disable[ruby]
	Option*/

LN_CLASS_END

//==============================================================================
/**
	@brief		LightNote アプリケーションのクラスです。
*/
LN_STATIC_CLASS(LNApplication)
	
	/**
		@brief		LightNote を初期化します。
		@details	ライブラリのすべての機能を使用できるように初期化を行います。
	*/
	LN_STATIC_API
	LN_LIBRARY_INITIALIZER
	LNResult LNApplication_Initialize();
	/*Option
		@override[cpp]
		@override_end
		@override[hsp]
		@override_end
	Option*/

	/**
		@brief		ファイル入出力機能を初期化します。
	*/
	LN_STATIC_API
	LN_LIBRARY_INITIALIZER
	LNResult LNApplication_InitializeFileIO();

	/**
		@brief		音声機能を初期化します。
		@details	ファイル入出力機能も同時に初期化されます。
	*/
	LN_STATIC_API
	LN_LIBRARY_INITIALIZER
	LNResult LNApplication_InitializeAudio();
	/*Option
		@override[hsp]
		@override_end
	Option*/

	/**
		@brief		入力機能を初期化します。
	*/
	LN_STATIC_API
	LN_LIBRARY_INITIALIZER
	LNResult LNApplication_InitializeInput();
	/*Option
		@override[hsp]
		@override_end
	Option*/

	/**
		@brief		フレームを更新します。
		@details	LightNote の状態を更新し、時間を1フレーム分進めます。
					この関数は必ず1フレームに1度だけ呼び出す必要があります。
	*/
	LN_STATIC_API
	LNResult LNApplication_Update();
	/*Option
		@override[hsp]
		@override_end
	Option*/

	/**
		@brief		画面の更新タイミングをリセットします。
		@details	時間のかかる処理の後にこのメソッドを呼ぶことで、
					極端なフレームスキップが発生しないようにすることができます。
	*/
	LN_STATIC_API
	LNResult LNApplication_ResetFrameDelay();

	/**
		@brief		アプリケーションを終了するべきかを確認します。
		@param[out]	requested	: 終了要求の有無を格納する変数
		@details	ウィンドウのクローズボタンが押された場合等、
					アプリケーションを終了するべき時には LN_FALSE を返します。
	*/
	LN_STATIC_API
	LNResult LNApplication_IsEndRequested(LNBool* requested);

	/**
		@brief		LightNote の終了処理を行います。
	*/
	LN_STATIC_API
	LN_LIBRARY_TERMINATOR
	void LNApplication_Terminate();


	LN_INTERNAL_API void* LNApplication_GetInternalObject();

LN_CLASS_END

LN_MODULE_END

} // extern "C"
