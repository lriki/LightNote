//==============================================================================
// Texture 
//------------------------------------------------------------------------------
///**
//  @file       Texture.h
//  @brief      Texture
//*/
//==============================================================================

#pragma once

#include "../../Math/LMath.h"
#include "../Common/Types.h"
#include "../Common/Common.h"
#include "../Common/Font/Font.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// Texture
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class Texture
    : public Base::ReferenceObject
{
	LN_TYPE_INFO_ACCESS_DECL;
public:
	Texture() {}
	virtual ~Texture() {}

public:

	/// テクスチャの種類の取得
	virtual LNTextureType getType() const = 0;

	/// サーフェイスフォーマットの取得
	virtual SurfaceFormat getSurfaceFormat() const = 0;

	/// サイズの取得
	virtual const LVector2& getSize() const = 0;

	/// 実際のサイズの取得
	virtual const LVector2& getRealSize(bool isReciprocal = false) const = 0;

	/// クリア
	virtual void clear(const LColor& color = LColor::Transparency) = 0;

	/// ロック
	virtual lnU32* lock() = 0;

	/// アンロック
	virtual void unlock() = 0;

	/// drawText() での文字列描画に使用するフォントの設定 (レンダリングターゲット、深度バッファの場合は無効)
	virtual void setFont(Font* font) = 0;

	/// drawText() での文字列描画に使用するフォントの取得
	virtual Font* getFont() const = 0;

	/// 文字列の描画
	virtual void drawText(const lnChar* text, const LRect& rect, TextAlign align = TextAlign_Left, int length = -1) = 0;

	/// 文字列の描画
	//virtual void drawTextUTF32(const uint32_t* text, const LRect& rect, TextAlign align = TextAlign_Left, int length = -1) = 0;

	/// テクスチャのブロック転送 (現在、普通のテクスチャ (RT、深度バッファではない) にのみ有効)
	virtual void blt(int x, int y, Texture* srcTexture, const LRect& srcRect) = 0;

	/// ファイルから作成したときのファイル名
	virtual const lnChar* getFilename() const = 0;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote
