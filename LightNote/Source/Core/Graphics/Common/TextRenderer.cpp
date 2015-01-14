//==============================================================================
// TextRenderer 
//==============================================================================

#include "stdafx.h"
#include "../../Math/LMath.h"
#include "../Manager.h"
#include "GraphicsUtil.h"
#include "Bitmap.h"
#include "TextRenderer.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// TextRenderer::RenderingContext
//==============================================================================
class TextRenderer::RenderingContext
{
private:
	struct GlyphInfo
	{
		int			Index;
		LPoint		Offset;
		LSize		Size;
	};

public:

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	RenderingContext()
		: mOwner(NULL)
		, mFont(NULL)
		, mRequestArray(NULL)
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	~RenderingContext()
	{
		LN_SAFE_RELEASE(mFont);
		SAFE_DELETE(mRequestArray);
		LN_SAFE_RELEASE(mGlyphsTexture);
		LN_SAFE_RELEASE(mSpriteRenderer);
	}

public:

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void creaet(TextRenderer* owner, Manager* manager, Font* font)
	{
		mOwner = owner;
		LN_REFOBJ_SET(mFont, font);
		mGlyphWidth = mFont->getSize();

		mSpriteRenderer = new SpriteRenderer(manager);
		mSpriteRenderer->initialize(manager->getGraphicsDevice(), mOwner->mMaxCharacters, false);
		mRequestArray = new RequestArray();

		// 横方向に並べる数
		// +1.0 は切り捨て対策。テクスチャサイズはmaxCharactersが収まる大きさであれば良い。
		// (小さくなければOK)
		lnU32 width = (lnU32)(sqrt((double)mOwner->mMaxCharacters) + 1.0);
		mGlyphWidthCount = width;
		width *= mGlyphWidth;

		// テクスチャ作成
		mGlyphsTexture = manager->getGraphicsDevice()->createTexture(width, width, 1, SurfaceFormat_A8R8G8B8);
	
		// キャッシュインデックス作成
		for (int i = 0; i < mOwner->mMaxCharacters; i++) {
			mIndexStack.push(i);
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void prepareRendering()
	{
		std::swap(mRequestArray, mOwner->mRequestArray);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void render(const LVector2& viewSize)
	{
		mSpriteRenderer->setViewSize(viewSize);

		ln_foreach(Request& req, (*mRequestArray))
		{
			int x = 0;
			const wchar_t* s = req.Text.getString();
			for (; *s != L'\0'; s++)
			{
				GlyphInfoMap::iterator itr = mGlyphInfoMap.find(*s);
				if (itr != mGlyphInfoMap.end())
				{
					drawSprite(itr->second, req.DrawArea.x + x, req.DrawArea.y);
					x += itr->second.Size.w + itr->second.Offset.x;
				}
				else
				{
					if (mIndexStack.size() == 0) {
						// TODO: 古いキャッシュ破棄
						LN_ERR2_ASSERT_S(0);
					}

					// 新しく作ってキャッシュに登録
					FontGlyphData* glyhp = mFont->makeGlyphData(*s, NULL);

					int cacheIndex = mIndexStack.top();
					mIndexStack.pop();

					{
						TextureScopedLock lock(mGlyphsTexture);
						Bitmap bmp;
						lock.createRefBitmap(&bmp);

						LRect destRect(
							(cacheIndex % mGlyphWidthCount) * mGlyphWidth,
							(cacheIndex / mGlyphWidthCount) * mGlyphWidth,
							glyhp->GlyphBitmap->getWidth(), glyhp->GlyphBitmap->getHeight());
						LRect srcRect(
							0, 0,
							glyhp->GlyphBitmap->getWidth(),
							glyhp->GlyphBitmap->getHeight());
						Bitmap::bitblt(&bmp, destRect, glyhp->GlyphBitmap, srcRect, 0xFFFFFFFF, false);
					}

					// キャッシュマップに登録
					GlyphInfo e;
					e.Index = cacheIndex;
					e.Offset = LPoint(glyhp->GlyphOffsetX, glyhp->GlyphOffsetY);
					e.Size = LSize(glyhp->GlyphBitmap->getWidth(), glyhp->GlyphBitmap->getHeight());
					mGlyphInfoMap.insert(GlyphInfoPair(*s, e));

					// 描画
					drawSprite(e, req.DrawArea.x + x, req.DrawArea.y);

					x += e.Size.w + e.Offset.x;
				}


			}
		}




		//mSpriteRenderer->drawRequest2D(
		//	LVector3(0, 0, 0),
		//	LVector3::ZERO,
		//	NULL,
		//	LRectF(0, 0, 256, 256),
		//	0xFF777777);

		mSpriteRenderer->flash();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void drawSprite(const GlyphInfo& info, int x, int y)
	{
		// キャッシュ内に見つかった
		LRectF srcRect(
			(info.Index % mGlyphWidthCount) * mGlyphWidth,
			(info.Index / mGlyphWidthCount) * mGlyphWidth,
			info.Size.w, info.Size.h);

		mSpriteRenderer->drawRequest2D(
			LVector3(x, y, 0),
			LVector3::ZERO,
			mGlyphsTexture,
			srcRect,
			0xFFFFFFFF);
	}

private:
	typedef std::map<lnU32, GlyphInfo>	GlyphInfoMap;
	typedef std::pair<lnU32, GlyphInfo>	GlyphInfoPair;

private:
	TextRenderer*		mOwner;
	SpriteRenderer*		mSpriteRenderer;
	Font*				mFont;
	RequestArray*		mRequestArray;
	Texture*			mGlyphsTexture;
	GlyphInfoMap		mGlyphInfoMap;
	int					mGlyphWidthCount;
	int					mGlyphWidth;
	std::stack<int>		mIndexStack;
};

//==============================================================================
// TextRenderer
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	TextRenderer::TextRenderer()
		: mFont(NULL)
		, mRenderingContext(NULL)
		//, mRequestArrayInfo(NULL)
		, mRequestArray(NULL)
	{

	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	TextRenderer::~TextRenderer()
	{
		LN_SAFE_RELEASE(mFont);
		SAFE_DELETE(mRenderingContext);
		SAFE_DELETE(mRequestArray);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void TextRenderer::creaet(Manager* manager, Font* font, int maxCharacters)
	{
		LN_REFOBJ_SET(mFont, font)
		mMaxCharacters = maxCharacters;

		mRenderingContext = LN_NEW RenderingContext();
		mRenderingContext->creaet(this, manager, mFont);

		//mRequestArrayInfo = new RequestArrayInfo();
		//mRequestArrayInfo->ValidCount = 0;
		mRequestArray = new RequestArray();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void TextRenderer::drawRequest(const lnChar* text, const LRect& drawArea, int length, TextAlign align)
	{
		Request* request;
#if 0
		if (mRequestArrayInfo->ValidCount >= mRequestArrayInfo->Requests.size())
		{
			// バッファが埋まっているので新しく作る必要がある
			mRequestArrayInfo->Requests.push_back(Request());
			request = &mRequestArrayInfo->Requests[mRequestArrayInfo->ValidCount];
			mRequestArrayInfo->ValidCount++;
		}
		else
		{
			// 空いているバッファを使用する
			request = &mRequestArrayInfo->Requests[mRequestArrayInfo->ValidCount];
			mRequestArrayInfo->ValidCount++;
		}
#endif
		mRequestArray->push_back(Request());
		request = &(*mRequestArray->rbegin());//&(*mRequestArray)[0];
		
		request->Text.setDecodeBytes(text, length);// = lnString(text, length);

		LRect textSize;
		mFont->getTextSize(text, length, &textSize);

		switch (align)
		{
		default:
		case LNote::Core::Graphics::TextAlign_Left:
			request->DrawArea = drawArea;
			break;
		case LNote::Core::Graphics::TextAlign_Center:
			request->DrawArea.x = drawArea.x + (drawArea.w - textSize.w) / 2;
			request->DrawArea.y = drawArea.y;
			request->DrawArea.w = drawArea.w;
			request->DrawArea.h = drawArea.h;
			break;
		case LNote::Core::Graphics::TextAlign_Right:
			request->DrawArea.x = drawArea.x + (drawArea.w - textSize.w);
			request->DrawArea.y = drawArea.y;
			request->DrawArea.w = drawArea.w;
			request->DrawArea.h = drawArea.h;
			break;
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void TextRenderer::prepareRendering()
	{
		mRenderingContext->prepareRendering();
		mRequestArray->clear();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void TextRenderer::render(const LVector2& viewSize)
	{
		mRenderingContext->render(viewSize);
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote
