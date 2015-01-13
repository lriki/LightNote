//==============================================================================
// Font 
//------------------------------------------------------------------------------
///**
//  @file       Font.h
//  @brief      Font
//*/
//==============================================================================

#pragma once

#include "../../Interface.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// Font
//------------------------------------------------------------------------------
///**
//  @brief     
//*/
//==============================================================================
class Font
	: public Base::ReferenceObject
{
	LN_TYPE_INFO_ACCESS_DECL;
public:

	/// フォント名の設定
	virtual void setName(const lnChar* fontName) = 0;

	/// フォント名の取得
	virtual const lnChar* getName() const = 0;

	/// フォントサイズの設定
	virtual void setSize(lnU32 size) = 0;

	/// フォントサイズの取得
	virtual lnU32 getSize() const = 0;

	/// フォントカラーの設定
	virtual void setColor(const LColor& color) = 0;

	/// フォントカラーの取得
	virtual const LColor& getColor() const = 0;

	/// エッジカラーの設定
	virtual void setEdgeColor(const LColor& color) = 0;

	/// エッジカラーの取得
	virtual const LColor& getEdgeColor() const = 0;

	/// エッジの幅の設定 (0 でエッジ無効)
	virtual void setEdgeSize(lnU32 size) = 0;

	/// エッジの幅の取得
	virtual lnU32 getEdgeSize() const = 0;

	/// 太文字の設定
	virtual void setBold(bool flag) = 0;

	/// 太文字の判定
	virtual bool isBold() const = 0;

	/// イタリック体の設定
	virtual void setItalic(bool flag) = 0;

	/// イタリック体の判定
	virtual bool isItalic() const = 0;

	/// アンチエイリアスの有効設定
	virtual void setAntiAlias(bool flag) = 0;

	/// アンチエイリアスの有効判定
	virtual bool isAntiAlias() const = 0;

	/// 文字列を描画したときのサイズ (ピクセル単位) の取得 (length = -1 で \0 まで)
	virtual void getTextSize(const char* text, int length, Geometry::Rect* outRect) = 0;

	/// 文字列を描画したときのサイズ (ピクセル単位) の取得 (length = -1 で \0 まで)
	virtual void getTextSize(const wchar_t* text, int length, Geometry::Rect* outRect) = 0;

	/// このフォントのコピーを作成する
	virtual Font* copy() = 0;

	/// グリフデータの取得 (最初の文字の場合、prevData に NULL を渡す。以降は戻り値を渡し続ける。非スレッドセーフ)
	virtual FontGlyphData* makeGlyphData(lnU32 utf32code, FontGlyphData* prevData) = 0;

	/// グリフデータの取得を終了する (メモリ解放。一連の makeGlyphData() を呼び終わった後、最後に呼ぶ)
	virtual void postGlyphData(FontGlyphData* glyphData) = 0;

protected:
	virtual ~Font() {}
};

} // namespace Graphics
} // namespace Core
} // namespace LNote
