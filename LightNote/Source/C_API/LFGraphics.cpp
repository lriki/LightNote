//==============================================================================
// LFGraphics 
//==============================================================================

#pragma once

#include "stdafx.h"
#include "LFInternal.h"
#include "LFBase.h"
#include "LFGraphics.h"

//==============================================================================
// LNTexture
//==============================================================================

LNOTE_TYPE_INFO_IMPL(Graphics::Texture, LNTexture);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNTexture_Create(LNHandle* texture, int width, int height)
	{
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Core::Graphics::Texture> obj(
			FuncLibManager::GraphicsManager->getGraphicsDevice()->createTexture( width, height, 1, Core::Graphics::SurfaceFormat_A8R8G8B8 ) );
		obj.addRef();
		*texture = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    LNResult LNTexture_CreateFromFile(LNHandle* texture,  const LNChar* filePath )
	{
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Core::Graphics::Texture> obj(
			FuncLibManager::GraphicsManager->getGraphicsDevice()->createTexture( filePath, 0x00000000, 1 ) );
		obj.addRef();
		*texture = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNTexture_CreateEx(LNHandle* texture, int width, int height, int levels, LNTextureFormat format)
	{
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Core::Graphics::Texture> obj(
			FuncLibManager::GraphicsManager->getGraphicsDevice()->createTexture( width, height, levels, ((Core::Graphics::SurfaceFormat)format) ) );
		obj.addRef();
		*texture = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNTexture_CreateFromFileEx(LNHandle* texture, const LNChar* filePath, uint32_t colorKey, int levels, LNTextureFormat format)
	{
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Core::Graphics::Texture> obj(
			FuncLibManager::GraphicsManager->getGraphicsDevice()->createTexture( filePath, colorKey, levels, ((Core::Graphics::SurfaceFormat)format) ) );
		obj.addRef();
		*texture = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNTexture_GetWidth(LNHandle texture, int* width)
	{
		LN_CHECK_ARG_HANDLE(texture);
		*width = static_cast<int>(TO_REFOBJ(Core::Graphics::Texture, texture)->getSize().X);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNTexture_GetHeight(LNHandle texture, int* width)
	{
		LN_CHECK_ARG_HANDLE(texture);
		*width = static_cast<int>(TO_REFOBJ(Core::Graphics::Texture, texture)->getSize().Y);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNTexture_GetRealWidth(LNHandle texture, int* width)
	{
		LN_CHECK_ARG_HANDLE(texture);
		*width = static_cast<int>(TO_REFOBJ(Core::Graphics::Texture, texture)->getRealSize().X);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNTexture_GetRealHeight(LNHandle texture, int* width)
	{
		LN_CHECK_ARG_HANDLE(texture);
		*width = static_cast<int>(TO_REFOBJ(Core::Graphics::Texture, texture)->getRealSize().Y);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNTexture_Clear(LNHandle texture, const LNColor* color)
	{
		LN_CHECK_ARG_HANDLE(texture);
		LN_FUNC_TRY_BEGIN;
		TO_REFOBJ(Core::Graphics::Texture, texture)->clear(TO_CORE_COLOR32(color));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNTexture_ClearRGBA(LNHandle texture, int r, int g, int b, int a)
	{
		LN_CHECK_ARG_HANDLE(texture);
		LNColor c = { r, g, b, a };
		LNTexture_Clear(texture, &c);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    LNResult LNTexture_DrawText( LNHandle texture, const LNChar* text, const LNRect* rect, LNHandle font, int length, LNTextAlign align )
	{
		LN_CHECK_ARG_HANDLE(texture);

		LN_FUNC_TRY_BEGIN;
		TO_REFOBJ(Core::Graphics::Texture, texture)->setFont(TO_REFOBJ(Core::Graphics::Font, font));
		TO_REFOBJ(Core::Graphics::Texture, texture)->drawText(text, TO_CORE_RECT(rect), (LNote::Core::Graphics::TextAlign)align, length);
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNTexture_DrawTextXY(LNHandle texture, const LNChar* text, int x, int y, LNHandle font, int length, LNTextAlign align)
	{
		LN_CHECK_ARG_HANDLE(texture);

		Core::Graphics::Texture* tex = TO_REFOBJ(Core::Graphics::Texture, texture);
		LNRect rc = { x, y, (int)tex->getSize().X, (int)tex->getSize().Y };

		LN_FUNC_TRY_BEGIN;
		tex->setFont(TO_REFOBJ(Core::Graphics::Font, font));
		tex->drawText(text, TO_CORE_RECT((&rc)), (LNote::Core::Graphics::TextAlign)align, length);
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNTexture_BitBlt( LNHandle texture, int x, int y, LNHandle srcTexture, const LNRect* srcRect )
	{
		LN_CHECK_ARG_HANDLE(texture);

		LN_FUNC_TRY_BEGIN;
		TO_REFOBJ(Core::Graphics::Texture, texture)->blt(x, y, TO_REFOBJ(Core::Graphics::Texture, texture), TO_CORE_RECT(srcRect));
		LN_FUNC_TRY_END_RETURN;
	}

//==============================================================================
// LNFont
//==============================================================================

	LNOTE_TYPE_INFO_IMPL(Graphics::Font, LNFont);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFont_Create(LNHandle* font)
	{
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Core::Graphics::Font> obj(
			FuncLibManager::GraphicsManager->getFontManager()->createFont() );
		obj.addRef();
		*font = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFont_CreateBitmapFontFromFile(LNHandle* font, const LNChar* filePath)
	{
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Core::Graphics::Font> obj(
			Graphics::Util::createBitmapFont(FuncLibManager::GraphicsManager, filePath));
		obj.addRef();
		*font = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFont_CreateCopy(LNHandle* font, LNHandle srcFont)
	{
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Core::Graphics::Font> obj(TO_REFOBJ(Core::Graphics::Font, srcFont)->copy());
		obj.addRef();
		*font = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFont_SetDefaultFont(LNHandle font)
	{
		LN_CHECK_ARG_HANDLE(font);
		FuncLibManager::GraphicsManager->getFontManager()->setDefaultFont(TO_REFOBJ(Core::Graphics::Font, font));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFont_SetName(LNHandle font, const LNChar* fontName)
	{
		LN_CHECK_ARG_HANDLE(font);
		TO_REFOBJ(Core::Graphics::Font, font)->setName(fontName);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFont_GetName(LNHandle font, const LNChar** fontName)
	{
		LN_CHECK_ARG_HANDLE(font);
		*fontName = TO_REFOBJ(Core::Graphics::Font, font)->getName();
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFont_SetSize(LNHandle font, int size)
	{
		LN_CHECK_ARG_HANDLE(font);
		TO_REFOBJ(Core::Graphics::Font, font)->setSize(size);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFont_GetSize(LNHandle font, int* size)
	{
		LN_CHECK_ARG_HANDLE(font);
		*size = TO_REFOBJ(Core::Graphics::Font, font)->getSize();
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFont_SetColor(LNHandle font, const LNColor* color)
	{
		LN_CHECK_ARG_HANDLE(font);
		TO_REFOBJ(Core::Graphics::Font, font)->setColor(TO_CORE_COLOR32(color));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFont_GetColor(LNHandle font, LNColor* color)
	{
		LN_CHECK_ARG_HANDLE(font);
		LN_SET_CoreColor32_TO_LNColor32(
			TO_REFOBJ(Core::Graphics::Font, font)->getColor(),
			color);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFont_SetEdgeColor(LNHandle font, const LNColor* color)
	{
		LN_CHECK_ARG_HANDLE(font);
		TO_REFOBJ(Core::Graphics::Font, font)->setEdgeColor(TO_CORE_COLOR32(color));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFont_GetEdgeColor(LNHandle font, LNColor* color)
	{
		LN_CHECK_ARG_HANDLE(font);
		LN_SET_CoreColor32_TO_LNColor32(
			TO_REFOBJ(Core::Graphics::Font, font)->getEdgeColor(),
			color);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFont_SetEdgeSize(LNHandle font, int size)
	{
		LN_CHECK_ARG_HANDLE(font);
		TO_REFOBJ(Core::Graphics::Font, font)->setEdgeSize(size);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFont_GetEdgeSize(LNHandle font, int* size)
	{
		LN_CHECK_ARG_HANDLE(font);
		*size = TO_REFOBJ(Core::Graphics::Font, font)->getEdgeSize();
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFont_SetBold(LNHandle font, LNBool enabled)
	{
		LN_CHECK_ARG_HANDLE(font);
		TO_REFOBJ(Core::Graphics::Font, font)->setBold(enabled != LN_FALSE);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFont_IsBold(LNHandle font, LNBool* enabled)
	{
		LN_CHECK_ARG_HANDLE(font);
		*enabled = LNOTE_BOOL_TO_LNBOOL(TO_REFOBJ(Core::Graphics::Font, font)->isBold());
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFont_SetItalic(LNHandle font, LNBool enabled)
	{
		LN_CHECK_ARG_HANDLE(font);
		TO_REFOBJ(Core::Graphics::Font, font)->setItalic(enabled != LN_FALSE);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFont_IsItalic(LNHandle font, LNBool* enabled)
	{
		LN_CHECK_ARG_HANDLE(font);
		*enabled = LNOTE_BOOL_TO_LNBOOL(TO_REFOBJ(Core::Graphics::Font, font)->isItalic());
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFont_SetAntiAlias(LNHandle font, LNBool enabled)
	{
		LN_CHECK_ARG_HANDLE(font);
		TO_REFOBJ(Core::Graphics::Font, font)->setAntiAlias(enabled != LN_FALSE);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFont_IsAntiAlias(LNHandle font, LNBool* enabled)
	{
		LN_CHECK_ARG_HANDLE(font);
		*enabled = LNOTE_BOOL_TO_LNBOOL(TO_REFOBJ(Core::Graphics::Font, font)->isAntiAlias());
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNFont_GetTextSize(LNHandle font, const LNChar* text, int length, LNSize* outSize)
	{
		LN_CHECK_ARG_HANDLE(font);

		LN_FUNC_TRY_BEGIN;
		LRect r;
		TO_REFOBJ(Core::Graphics::Font, font)->getTextSize(text, length, &r);
		outSize->Width  = r.w;
		outSize->Height = r.h;
		LN_FUNC_TRY_END_RETURN;
	}

