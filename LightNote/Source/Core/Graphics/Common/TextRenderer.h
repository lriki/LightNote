//==============================================================================
// TextRenderer 
//------------------------------------------------------------------------------
///**
//  @file       TextRenderer.h
//  @brief      TextRenderer
//*/
//==============================================================================

#pragma once

#include <string>
#include <stack>
#include <map>
#include "../../Base/Container/NodeList.h"
#include "../../Base/Unicode.h"
#include "SpriteRenderer.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// TextRenderer
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class TextRenderer
{
public:
	TextRenderer();
	~TextRenderer();

public:
	/// 作成 (maxCharacters : 1フレームに描画できる最大文字数)
	void creaet(Manager* manager, Font* font, int maxCharacters);

	/// 描画リクエスト
	void drawRequest(const lnChar* text, const LRect& drawArea, int length, TextAlign align);

	/// 描画準備 (同期フェーズ)
	void prepareRendering();

	/// 描画
	void render(const LVector2& viewSize);

private:

	struct Request
	{
		//lnString	Text;
		Base::UnicodeString	Text;
		LRect				DrawArea;
	};

	typedef std::vector<Request>	RequestArray;

	//struct RequestArrayInfo
	//{
	//	RequestArray	Requests;
	//	int				ValidCount;
	//};

	class RenderingContext;
	friend class RenderingContext;

private:
	int							mMaxCharacters;
	Font*						mFont;
	RenderingContext*			mRenderingContext;
	RequestArray*				mRequestArray;
	//RequestArrayInfo*			mRequestArrayInfo;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote
