//==============================================================================
// BitmapFont 
//------------------------------------------------------------------------------
///**
//  @file       BitmapFont.h
//  @brief      BitmapFont
//*/
//==============================================================================

#pragma once

#include "../../Interface.h"
#include "FreeTypeFontManager.h"
#include "FreeTypeFont.h"	// FreeTypeFont::FontGlyphData

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// BitmapFont
//------------------------------------------------------------------------------
///**
//  @brief     
//*/
//==============================================================================
class BitmapFont
    : /*public Base::ReferenceObject
    , */public Font
{
public:
	BitmapFont();
	virtual ~BitmapFont();
	//LN_REFOBJ_METHODS;

public:

	/// 作成
	void create(FreeTypeFontManager* manager, Texture* fontTexture);

	virtual void			setName(const lnChar* fontname) { LN_THROW_InvalidOperation(0); }
	virtual const lnChar*   getName() const { LN_THROW_InvalidOperation(0);  return NULL; }

	virtual void            setSize(lnU32 size) { LN_THROW_InvalidOperation(0); }
	virtual lnU32           getSize() const { return mCharHeight; }

	virtual void            setColor(const LColor& color) { mColor = color; }
	virtual const LColor&	getColor() const { return mColor; }

	virtual void            setEdgeColor(const LColor& color) { LN_THROW_InvalidOperation(0); }
	virtual const LColor&	getEdgeColor() const { LN_THROW_InvalidOperation(0); return LColor::Transparency; }

	virtual void            setEdgeSize(lnU32 size) { LN_THROW_InvalidOperation(0); }
	virtual lnU32           getEdgeSize() const { LN_THROW_InvalidOperation(0); return 0; }

	virtual void            setBold(bool flag) { LN_THROW_InvalidOperation(0); }
	virtual bool            isBold() const { LN_THROW_InvalidOperation(0); return false; }

	virtual void            setItalic(bool flag) { LN_THROW_InvalidOperation(0); }
	virtual bool            isItalic() const { LN_THROW_InvalidOperation(0); return false; }

	virtual void            setAntiAlias(bool flag) { LN_THROW_InvalidOperation(0); }
	virtual bool            isAntiAlias() const { LN_THROW_InvalidOperation(0); return false; }

	virtual void			getTextSize(const char* text, int length, Geometry::Rect* outRect);
	virtual void			getTextSize(const wchar_t* text, int length, Geometry::Rect* outRect);

	virtual Font*			copy();

public:

	/// グリフデータの取得 (最初の文字の場合、prevData に NULL を渡す。以降は戻り値を渡し続ける。非スレッドセーフ)
	virtual FontGlyphData* makeGlyphData(lnU32 utf32code, FontGlyphData* prevData);

	/// グリフデータの取得を終了する (メモリ解放。一連の makeGlyphData() を呼び終わった後、最後に呼ぶ)
	virtual void postGlyphData(FontGlyphData* glyphData) {}

private:
    void dispose();

private:
	FreeTypeFontManager*		mManager;
	Texture*					mFontTexture;
	Bitmap						mFontBitmap;
	LColor					mColor;
	int							mCharWidth;			///< 1文字分の幅
	int							mCharHeight;		///< 1文字分の高さ
	FontGlyphData				mFontGlyphData;
	Bitmap						mGlyphBitmap;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote
