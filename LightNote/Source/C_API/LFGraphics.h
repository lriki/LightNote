//==============================================================================
// LFGraphics 
//------------------------------------------------------------------------------
///**
//  @file       LFGraphics.h
//  @brief      LFGraphics
//  @defgroup   group_lib_graphics Graphics
//  @{
//*/
//==============================================================================

#pragma once

#include "LFCommon.h"
#include "LFTypedef.h"

extern "C" {

//==============================================================================
// LNColor
//------------------------------------------------------------------------------
///**
//  @defgroup   group_color LNColor
//  @brief      色
//  @{
//*/
//==============================================================================

/** @} */	// defgroup

//==============================================================================
// LNColorF
//------------------------------------------------------------------------------
///**
//  @defgroup   group_colorf LNColorF
//  @brief      
//  @{
//*/
//==============================================================================

/** @} */	// defgroup

//==============================================================================
// LNTone
//------------------------------------------------------------------------------
///**
//  @defgroup   group_tone LNTone
//  @brief      
//  @{
//*/
//==============================================================================

/** @} */	// defgroup

//==============================================================================
// LNTexture
//------------------------------------------------------------------------------
///**
//  @defgroup   group_texture LNTexture
//  @brief      テクスチャ
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNTexture);

	//----------------------------------------------------------------------
	///**
	//  @brief		テクスチャの作成
	//  @param[out]	texture		: 作成されたテクスチャハンドルを格納する変数
	//  @param[in]	width		: 幅 (ピクセル数)
	//  @param[in]	height		: 高さ (ピクセル数)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	ミップマップレベル 1、フォーマット A8R8B8G8(透明色付フォーマット) で作成します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNTexture_Create(LNHandle* texture, int width, int height);

	//----------------------------------------------------------------------
	///**
	//  @brief		テクスチャの作成
	//  @param[out]	texture		: 作成されたテクスチャハンドルを格納する変数
	//  @param[in]	filePath	: テクスチャファイル名
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	サポートしているフォーマットは 
	//				.bmp、.dds、.dib、.jpg、.png、.tga の6種類です。<br>
	//				ミップマップレベル 1 で作成します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNTexture_CreateFromFile(LNHandle* texture, const LNChar* filePath);

	//----------------------------------------------------------------------
	///**
	//  @brief      テクスチャの作成
	//  @param[out]	texture		: 作成されたテクスチャハンドルを格納する変数
	//  @param[in]	width		: 幅 (ピクセル数)
	//  @param[in]	height		: 高さ (ピクセル数)
	//  @param[in]	levels		: ミップマップレベル (0で最大レベルまで作成)
	//  @param[in]	format		: サーフェイスフォーマット
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNTexture_CreateEx(LNHandle* texture, int width, int height, int levels, LNTextureFormat format);

	//----------------------------------------------------------------------
	///**
	//  @brief      テクスチャの作成
	//  @param[out]	texture		: 作成されたテクスチャハンドルを格納する変数
	//  @param[in]	filePath	: テクスチャファイル名
	//  @param[in]	colorKey	: 透明色として扱う色
	//  @param[in]	levels		: ミップマップレベル (0で最大レベルまで作成)
	//  @param[in]	format		: サーフェイスフォーマット
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	サポートしているフォーマットは 
	//				.bmp、.dds、.dib、.jpg、.png、.tga の6種類です。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNTexture_CreateFromFileEx(LNHandle* texture, const LNChar* filePath, uint32_t colorKey, int levels, LNTextureFormat format);

	//----------------------------------------------------------------------
	///**
	//  @brief		テクスチャの幅の取得 (ピクセル単位)
	//  @param[in]	texture		: テクスチャハンドル
	//  @param[out]	width		: テクスチャの幅を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNTexture_GetWidth(LNHandle texture, int* width);

	//----------------------------------------------------------------------
	///**
	//  @brief		テクスチャの高さの取得 (ピクセル単位)
	//  @param[in]	texture		: テクスチャハンドル
	//  @param[out]	height		: テクスチャの高さを格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNTexture_GetHeight(LNHandle texture, int* height);

	//----------------------------------------------------------------------
	///**
	//  @brief		テクスチャの実際の幅の取得 (ピクセル単位)
	//  @param[in]	texture		: テクスチャハンドル
	//  @param[out]	width		: テクスチャの実際の幅を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNTexture_GetRealWidth(LNHandle texture, int* width);

	//----------------------------------------------------------------------
	///**
	//  @brief		テクスチャの実際の高さの取得 (ピクセル単位)
	//  @param[in]	texture		: テクスチャハンドル
	//  @param[out]	height		: テクスチャの実際の高さを格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNTexture_GetRealHeight(LNHandle texture, int* height);

	//----------------------------------------------------------------------
	///**
	//  @brief		テクスチャをクリアする
	//  @param[in]	texture		: テクスチャハンドル
	//  @param[in]	color		: 塗りつぶす色
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNTexture_Clear(LNHandle texture, const LNColor* color);

	//----------------------------------------------------------------------
	///**
	//  @brief		テクスチャをクリアする
	//  @param[in]	texture		: テクスチャハンドル
	//  @param[in]	r			: 塗りつぶす色の R 値
	//  @param[in]	g			: 塗りつぶす色の G 値
	//  @param[in]	b			: 塗りつぶす色の B 値
	//  @param[in]	a			: 塗りつぶす色の A 値
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNTexture_ClearRGBA(LNHandle texture, int r, int g, int b, int a);

	//----------------------------------------------------------------------
	///**
	//  @brief		テクスチャへ文字列を描画する
	//  @param[in]	texture		: テクスチャハンドル
	//  @param[in]	text		: 描画する文字列
	//  @param[in]	rect		: 描画領域 (はみだした場合にクリッピングされます)
	//  @param[in]	font		: 描画に使用するフォント (0 の場合、デフォルトのフォントを使用する)
	//  @param[in]	length		: 描画文字数 (-1 の場合、文字列全体を描画する)
	//  @param[in]	align		: 描画基準位置の指定
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	レンダリングターゲットと深度バッファテクスチャには描画できません。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNTexture_DrawText(LNHandle texture, const LNChar* text, const LNRect* rect, LNHandle font LNOTE_DEFAULT_ARG(NULL), int length LNOTE_DEFAULT_ARG(-1), LNTextAlign align LNOTE_DEFAULT_ARG(LN_TEXTALIGN_LEFT));

	//----------------------------------------------------------------------
	///**
	//  @brief		テクスチャへ文字列を描画する
	//  @param[in]	texture		: テクスチャハンドル
	//  @param[in]	text		: 描画する文字列
	//  @param[in]	x			: 描画先の左上 X 座標
	//  @param[in]	y			: 描画先の左上 Y 座標
	//  @param[in]	font		: 描画に使用するフォント (0 の場合、デフォルトのフォントを使用する)
	//  @param[in]	length		: 描画文字数 (-1 の場合、文字列全体を描画する)
	//  @param[in]	align		: 描画基準位置の指定
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	レンダリングターゲットと深度バッファテクスチャには描画できません。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNTexture_DrawTextXY(LNHandle texture, const LNChar* text, int x, int y, LNHandle font LNOTE_DEFAULT_ARG(NULL), int length LNOTE_DEFAULT_ARG(-1), LNTextAlign align LNOTE_DEFAULT_ARG(LN_TEXTALIGN_LEFT));

	//----------------------------------------------------------------------
	///**
	//  @brief		テクスチャのブロック転送
	//  @param[in]	texture		: テクスチャハンドル
	//  @param[in]	x			: 転送先 X 座標
	//  @param[in]	y			: 転送先 Y 座標
	//  @param[in]	srcTexture	: 転送元テクスチャハンドル
	//  @param[in]	srcRect		: 転送元領域
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	srcTexture で指定されたテクスチャから texture へブロック転送を行います。<br>
	//				レンダリングターゲットと深度バッファテクスチャは
	//				転送元・先のどちらにも指定できません。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNTexture_BitBlt(LNHandle texture, int x, int y, LNHandle srcTexture, const LNRect* srcRect);

	/** @} */ // defgroup

//==============================================================================
// LNFont
//------------------------------------------------------------------------------
///**
//  @defgroup   group_font LNFont
//  @brief      フォントオブジェクト
//  @{
//*/
//==============================================================================

	LNOTE_TYPE_INFO_DECL(LNFont);

	//----------------------------------------------------------------------
	///**
	//  @brief		フォントの作成
	//  @param[out]	font	: 作成されたフォントハンドルを格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	デフォルトのフォントを使用してフォントを作成します。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFont_Create(LNHandle* font);

	//----------------------------------------------------------------------
	///**
	//  @brief		ビットマップフォントの作成
	//  @param[out]	font		: 作成されたフォントハンドルを格納する変数
	//  @param[in]	filePath	: 画像ファイル名
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFont_CreateBitmapFontFromFile(LNHandle* font, const LNChar* filePath);

	//----------------------------------------------------------------------
	///**
	//  @brief		フォントのコピーの作成
	//  @param[out]	font	: 作成されたフォントハンドルを格納する変数
	//  @param[in]	srcFont	: コピー元のフォントハンドル
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFont_CreateCopy(LNHandle* font, LNHandle srcFont);

	//----------------------------------------------------------------------
	///**
	//  @brief		指定フォントをデフォルトフォントに設定する
	//  @param[in]	font	: フォントハンドル
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//	@details	テクスチャの DrawText 等でフォントハンドルを NULL にした場合、
	//				このフォントが使われるようになります。
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFont_SetDefaultFont(LNHandle font);
	/*DeclOpt
	AutoPropertyGenerate = true;
	ForceStaticMethod = true;
	DeclOpt*/

	//----------------------------------------------------------------------
	///**
	//  @brief		フォント名の設定
	//  @param[in]	font		: フォントハンドル
	//  @param[in]	fontName	: フォント名
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFont_SetName(LNHandle font, const LNChar* fontName);

	//----------------------------------------------------------------------
	///**
	//  @brief		フォント名の取得
	//  @param[in]	font		: フォントハンドル
	//  @param[out]	fontName	: フォント名を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFont_GetName(LNHandle font, const LNChar** fontName);

	//----------------------------------------------------------------------
	///**
	//  @brief		フォントサイズの設定
	//  @param[in]	font	: フォントハンドル
	//  @param[in]	size	: 設定するサイズ
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFont_SetSize(LNHandle font, int size);

	//----------------------------------------------------------------------
	///**
	//  @brief		フォントサイズの取得
	//  @param[in]	font	: フォントハンドル
	//  @param[out]	size	: フォントサイズを格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFont_GetSize(LNHandle font, int* size);

	//----------------------------------------------------------------------
	///**
	//  @brief		フォントカラーの設定
	//  @param[in]	font	: フォントハンドル
	//  @param[in]	color	: 設定する色
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFont_SetColor(LNHandle font, const LNColor* color);

	//----------------------------------------------------------------------
	///**
	//  @brief		フォントカラーの取得
	//  @param[in]	font	: フォントハンドル
	//  @param[out]	color	: 色を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFont_GetColor(LNHandle font, LNColor* color);

	//----------------------------------------------------------------------
	///**
	//  @brief		エッジカラーの設定
	//  @param[in]	font	: フォントハンドル
	//  @param[in]	color	: 設定する色
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFont_SetEdgeColor(LNHandle font, const LNColor* color);

	//----------------------------------------------------------------------
	///**
	//  @brief		エッジカラーの取得
	//  @param[in]	font	: フォントハンドル
	//  @param[out]	color	: 色を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFont_GetEdgeColor(LNHandle font, LNColor* color);

	//----------------------------------------------------------------------
	///**
	//  @brief		エッジの幅の設定 
	//  @param[in]	font	: フォントハンドル
	//  @param[in]	size	: エッジの幅 (0 でエッジ無効)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFont_SetEdgeSize(LNHandle font, int size);

	//----------------------------------------------------------------------
	///**
	//  @brief		エッジの幅の取得
	//  @param[in]	font	: フォントハンドル
	//  @param[out]	size	:　エッジの幅を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFont_GetEdgeSize(LNHandle font, int* size);

	//----------------------------------------------------------------------
	///**
	//  @brief		太文字有無の設定
	//  @param[in]	font	: フォントハンドル
	//  @param[in]	enabled	: 太文字の有無
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFont_SetBold(LNHandle font, LNBool enabled);

	//----------------------------------------------------------------------
	///**
	//  @brief		太文字有無の取得
	//  @param[in]	font	: フォントハンドル
	//  @param[out]	enabled	:　太文字の有無を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFont_IsBold(LNHandle font, LNBool* enabled);

	//----------------------------------------------------------------------
	///**
	//  @brief		イタリック体有無の設定
	//  @param[in]	font	: フォントハンドル
	//  @param[in]	enabled	: イタリック体の有無
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFont_SetItalic(LNHandle font, LNBool enabled);

	//----------------------------------------------------------------------
	///**
	//  @brief		イタリック体有無の取得
	//  @param[in]	font	: フォントハンドル
	//  @param[out]	enabled	: イタリック体の有無を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFont_IsItalic(LNHandle font, LNBool* enabled);

	//----------------------------------------------------------------------
	///**
	//  @brief		アンチエイリアス有無の設定
	//  @param[in]	font	: フォントハンドル
	//  @param[in]	enabled	: アンチエイリアスの有無
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFont_SetAntiAlias(LNHandle font, LNBool enabled);

	//----------------------------------------------------------------------
	///**
	//  @brief		アンチエイリアス有無の取得
	//  @param[in]	font	: フォントハンドル
	//  @param[out]	enabled	: アンチエイリアス有無を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNFont_IsAntiAlias(LNHandle font, LNBool* enabled);

	//----------------------------------------------------------------------
	///**
	//  @brief		文字列を描画したときのサイズ (ピクセル単位) の取得
	//  @param[in]	font	: フォントハンドル
	//  @param[in]	text	: サイズ計算に使用する文字列
	//  @param[in]	length	: 文字数 (-1 で文字列全体)
	//  @param[out]	outSize	: 結果を格納する変数
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
    LNOTEAPI LNResult LNFont_GetTextSize( LNHandle font, const LNChar* text, int length, LNSize* outSize );


/** @} */ // defgroup

} // extern "C"

/** @} */ // defgroup
