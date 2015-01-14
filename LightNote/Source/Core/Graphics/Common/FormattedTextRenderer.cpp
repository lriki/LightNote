//==============================================================================
// FormattedTextRenderer 
//==============================================================================

#include "stdafx.h"
#include <algorithm>
#include "../../Base/Encoding.h"
#include "../../Math/LMath.h"
#include "GraphicsUtil.h"
#include "Bitmap.h"
#include "FormattedTextRenderer.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// FormattedTextRenderer
//==============================================================================

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
FormattedTextRenderer::FormattedTextRenderer()
	: mTargetTexture(NULL)
	, mBaseFont(NULL)
	, mOperatorEsc((UTF32)'@')
	, mCurrentFont(NULL)
	, mCurrentDrawDataIndex(0)
{
	UTF32 ID_Color_code[] = { 'C', '\0' };
	addOperatorCode(ID_Color_code, ID_Color);

	UTF32 ID_Ruby_code[] = { 'R', '\0' };
	addOperatorCode(ID_Ruby_code, ID_Ruby);

	UTF32 ID_Speed_code[] = { 'S', '\0' };
	addOperatorCode(ID_Speed_code, ID_Speed);

	UTF32 ID_FontSize_code[] = { 'F', '\0' };
	addOperatorCode(ID_FontSize_code, ID_FontSize);

	UTF32 ID_Wait4_code[] = { '.', '\0' };
	addOperatorCode(ID_Wait4_code, ID_Wait4);

	UTF32 ID_Wait2_code[] = { ';', '\0' };
	addOperatorCode(ID_Wait2_code, ID_Wait2);

	UTF32 ID_Wait1_code[] = { '|', '\0' };
	addOperatorCode(ID_Wait1_code, ID_Wait1);

	UTF32 ID_ImmediatelyBegin_code[] = { '>', '\0' };
	addOperatorCode(ID_ImmediatelyBegin_code, ID_ImmediatelyBegin);

	UTF32 ID_ImmediatelyEnd_code[] = { '<', '\0' };
	addOperatorCode(ID_ImmediatelyEnd_code, ID_ImmediatelyEnd);

	UTF32 ID_AlignLeft_code[] = { 'LT', '\0' };
	addOperatorCode(ID_AlignLeft_code, ID_AlignLeft);

	UTF32 ID_AlignCenter_code[] = { 'CT', '\0' };
	addOperatorCode(ID_AlignCenter_code, ID_AlignCenter);

	UTF32 ID_AlignRight_code[] = { 'RT', '\0' };
	addOperatorCode(ID_AlignRight_code, ID_AlignRight);

	UTF32 ID_BoldBegin_code[] = { 'B', '\0' };
	addOperatorCode(ID_BoldBegin_code, ID_BoldBegin);

	UTF32 ID_BoldEnd_code[] = { 'B0', '\0' };
	addOperatorCode(ID_BoldEnd_code, ID_BoldEnd);

	UTF32 ID_ItalicBegin_code[] = { 'I', '\0' };
	addOperatorCode(ID_ItalicBegin_code, ID_ItalicBegin);

	UTF32 ID_ItalicEnd_code[] = { 'I0', '\0' };
	addOperatorCode(ID_ItalicEnd_code, ID_ItalicEnd);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
FormattedTextRenderer::~FormattedTextRenderer()
{
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void FormattedTextRenderer::creaet(Manager* manager)
{
	mManager = manager;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void FormattedTextRenderer::setBaseFont(Font* font)
{
	LN_REFOBJ_SET(mBaseFont, font);
	if (mBaseFont)
	{
		mRubyFont = mBaseFont->copy();
		mRubyFont->setSize(mBaseFont->getSize() / 2);
	}
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void FormattedTextRenderer::setDrawArea(const LRect& area)
{
	mDrawArea = area;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void FormattedTextRenderer::setText(const UTF32* text)
{
	mText = text;
	mTextModified = true;

	// 制御文字を抽出する
	for (int i = 0; i < mText.size();)
	{
		// エスケープ文字が見つかった
		if (mText[i] == mOperatorEsc)
		{
			int opStart = i;
			i++;
			if (i >= mText.size()) {
				break;	// 終端
			}

			// 制御文字検索
			int id = 0;
			for (int iOp = 0; iOp < mOperatorCodeArray.size(); ++iOp)
			{
				if (Base::StringUtils::strcmpN(
					&mText[iOp],
					mOperatorCodeArray[iOp].Code.c_str(),
					mOperatorCodeArray[iOp].Code.size()))
				{
					id = mOperatorCodeArray[iOp].ID;
					i += mOperatorCodeArray[iOp].Code.size();	// i は制御文字の次を指す
					break;
				}
			}

			// 登録された制御文字である場合
			if (id != 0)
			{
				// 引数チェック
				if ((i + 2) < mText.size() &&				// '['']' 最低2文字分はある
					mText[i] == OperatorArgsLeftBracket)	// 先頭 [
				{
					i++;

					// 終端 ] を探す
					int pos = Base::StringUtils::indexOf<UTF32>(&mText[i], OperatorArgsRightBracket, 0);
					if (pos == -1) {
						// 終端 ] が見つからなかった
					}
					// [] が空でなければ分割
					else if (pos > 0)
					{
						utf32string t(mText, i, pos);
						// , 分割
						std::vector<utf32string> args;
						Base::StringUtils::split(t.c_str(), (UTF32)',', Base::StringSplitOptions_None, &args);
						
						i += pos + 1;	// i は ] の次を指す
						
						// 引数付制御文字としてリストアップ
						OperatorToken token;
						token.ID = id;
						token.Length = (i - opStart);
						ln_foreach(utf32string& s, args)
						{
							lnString tmp;
							tmp.ConvertFrom(s.c_str(), s.size() * sizeof(UTF32), Base::Encoding::GetUTF32Encoding());
							token.Args.push_back(tmp);
						}
						mOperatorTokenArray.push_back(token);
					}
				}
				else
				{
					// 引数無し制御文字としてリストアップ
					OperatorToken token;
					token.ID = id;
					token.Length = (i - opStart);
					mOperatorTokenArray.push_back(token);
				}
			}
			else
			{
				// 登録されていない制御文字だった。
				// この時点で i は @ の次の文字を指している。
			}
		}
		// 普通の文字だった場合
		else
		{
			i++;
		}
	}
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void FormattedTextRenderer::setOperatorEscCode(uint32_t code)
{
	mOperatorEsc = code;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void FormattedTextRenderer::addUserOperatorCode(uint32_t code, int id)
{
	LN_THROW_NotImpl(0);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void FormattedTextRenderer::update()
{
	// setText() 後の初回 update() ならコンパイル実行
	if (mTextModified) {
		compile();
		mTextModified = false;
		mCurrentDrawDataIndex = 0;
	}

	TextureScopedLock texLock(mTargetTexture, true);

	while (mCurrentDrawDataIndex < mDrawDataQueue.size())
	{
		DrawData& drawData = mDrawDataQueue[mCurrentDrawDataIndex];

		// 待ち時間がある場合は終了
		if (drawData.WaitCount > 0) {
			drawData.WaitCount--;
			break;
		}

		// 現在の行の描画範囲
		LRect drawArea(0, 0, 256, 256);

		// 文字の転送先
		LRect destRect(
			drawArea.x + drawData.GlyphData.GlyphOffsetX,
			drawArea.y + drawData.GlyphData.GlyphOffsetY,
			drawArea.w - drawData.GlyphData.GlyphOffsetX,
			drawArea.h - drawData.GlyphData.GlyphOffsetY);

		// アウトラインのある文字の描画
		if (drawData.GlyphData.OutlineBitmap)
		{
			Bitmap::bitblt(
				texLock.getRefBitmap(),
				destRect,
				drawData.GlyphData.OutlineBitmap,
				LRect(0, 0, drawData.GlyphData.OutlineBitmap->getWidth(), drawData.GlyphData.OutlineBitmap->getHeight()),
				drawData.EdgeColor.toU32ARGB(),
				true);

			destRect.x += drawData.GlyphData.OutlineOffset;
			destRect.y += drawData.GlyphData.OutlineOffset;
		}

		// 文字描画
		Bitmap::bitblt(
			texLock.getRefBitmap(),
			destRect,
			drawData.GlyphData.GlyphBitmap,
			LRect(0, 0, drawData.GlyphData.GlyphBitmap->getWidth(), drawData.GlyphData.GlyphBitmap->getHeight()),
			drawData.ForeColor.toU32ARGB(),
			true);

		mCurrentDrawDataIndex++;
	}

}

#if 0

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void FormattedTextRenderer::beginUpdate()
{
	while (mCompileCursor < mText.size())
	{



		// 描画キュー先頭のアイテムの待ち時間がある場合はカウントを減らし、何もしない

		// 解析し、描画キューを構築。
		// 見つかったユーザー定義制御文字列は専用のリストへ。
		// ルビなど、待ち時間を指定してキューに入れることもできる。
		// 右揃えなどのために行サイズが必要。最低でも1行丸ごと解析しないとダメ。

		// 描画キューの内容で描画


		// 


		/*
		・ユーザー定義制御文字
		・ユーザー定義制御文字の置換

		1次解析と2次解析に分ける
		*/
	}
}
//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
int FormattedTextRenderer::getProcessingOperatorCount()
{
	return 0;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
int FormattedTextRenderer::getProcessingOperatorID(int dataIndex)
{
	return 0;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
const wchar_t* FormattedTextRenderer::getProcessingOperatorArg(int dataIndex, int argIndex)
{
	return NULL;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void FormattedTextRenderer::setProcessingOperatorText(int dataIndex, const lnString& text)
{
}
#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
bool FormattedTextRenderer::isFinished() const
{
	return true;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void FormattedTextRenderer::addOperatorCode(const UTF32* code, int id)
{
	OperatorCode c;
	c.Code = code;
	c.ID = id;
	mOperatorCodeArray.push_back(c);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void FormattedTextRenderer::analyze1()
{
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void FormattedTextRenderer::resetCompile()
{
	//mCompileCursor = 0;
}


void FormattedTextRenderer::disposeCompiledData()
{
	ln_foreach(DrawData& data, mDrawDataQueue)
	{
		data.release();
	}
	mDrawDataQueue.clear();
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void FormattedTextRenderer::compile()
{
	LN_SAFE_RELEASE(mCurrentFont);

	mOperatorCodeCount = 0;
	LN_REFOBJ_SET(mCurrentFont, mBaseFont->copy());	// フォントリセット

	const UTF32* str = mText.c_str();
	const UTF32* end = &mText[mText.size()];
	LPoint nextLineStartPoint(0, 0);
	while (true)
	{
		LSize lineSize;

		// 現在位置から次の改行または終端まで検索
		const UTF32* nl = Base::StringUtils::findNextNewLine(str, end);
		if (nl == end){
			compileLine(str, end - str, nextLineStartPoint, &lineSize);	// 最後の行
			break;							//　終了
		}

		// 1行解析
		compileLine(str, nl - str/*end - str*/, nextLineStartPoint, &lineSize);
		str = nl + Base::StringUtils::checkNewLineSequence(nl, end);

		nextLineStartPoint.y += lineSize.h;
	}

	LN_SAFE_RELEASE(mCurrentFont);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void FormattedTextRenderer::compileLine(const UTF32* lineText, int len, const LPoint& startPoint, LSize* lineSize)
{
	FontGlyphData* glyphData = NULL;
	DrawDataQueue tmpDrawDataList;

	lineSize->set(0, 0);

	for (int iPos = 0; iPos < len;)
	{
		// エスケープ文字が見つかった
		if (lineText[iPos] == mOperatorEsc)
		{
			OperatorToken& opToken = mOperatorTokenArray[mOperatorCodeCount];
			iPos += opToken.Length;

			switch (opToken.ID)
			{
			case ID_Color:
				break;
			case ID_Ruby:
			{
				// ルビグリフを作成し、その描画領域サイズを計算する
				DrawDataQueue rubyDrawDataList;
				LSize rubyAreaSize;
				utf32string rubyText;
				rubyText.assignNativeString(opToken.Args[1].c_str());
				FontGlyphData* rubyGlyphData = NULL;
				for (int i = 0; i < rubyText.size(); i++)
				{
					// 描画データを1文字分作る
					DrawData data;
					rubyGlyphData = makeDrawData(mRubyFont, rubyGlyphData, rubyText[i], &data);
					rubyDrawDataList.push_back(data);

					// 行サイズ・最大グリフサイズ更新
					rubyAreaSize.w = std::max(rubyAreaSize.w, data.Area.x + data.Area.w);
					rubyAreaSize.h = std::max(rubyAreaSize.h, data.Area.y + data.Area.h);
				}

				// 被修飾文字グリフを作成し、その描画領域サイズを計算する
				DrawDataQueue srcDrawDataList;
				LSize srcAreaSize;
				lnString srcText;
				srcText.assignNativeString(opToken.Args[0].c_str());
				for (int i = 0; i < srcText.size(); i++)
				{
					// 描画データを1文字分作る
					DrawData data;
					glyphData = makeDrawData(mCurrentFont, glyphData, srcText[i], &data);
					srcDrawDataList.push_back(data);

					// 行サイズ・最大グリフサイズ更新
					srcAreaSize.w = std::max(srcAreaSize.w, data.Area.x + data.Area.w);
					srcAreaSize.h = std::max(srcAreaSize.h, data.Area.y + data.Area.h); 
				}

				// ルビの描画幅の方が小さい場合はルビ文字の位置を調整する
				if (rubyAreaSize.w < srcAreaSize.h) {
					alignGlyhpsH(rubyDrawDataList, srcAreaSize.w);
				}
				// 被修飾文字列の描画幅の方が小さい場合は被修飾文字列の位置を調整する
				else {
					alignGlyhpsH(srcDrawDataList, rubyAreaSize.w);
				}
				break;
			}
			case ID_Speed:
				break;
			case ID_FontSize:
				break;
			case ID_Wait4:
				break;
			case ID_Wait2:
				break;
			case ID_Wait1:
				break;
			case ID_ImmediatelyBegin:
				break;
			case ID_ImmediatelyEnd:
				break;
			case ID_AlignLeft:
				break;
			case ID_AlignCenter:
				break;
			case ID_AlignRight:
				break;
			case ID_BoldBegin:
				break;
			case ID_BoldEnd:
				break;
			case ID_ItalicBegin:
				break;
			case ID_ItalicEnd:
				break;
			default:
				break;
			}
		}
		// 普通の文字だったらここに来る
		else
		{
			// 描画データを1文字分作る
			DrawData drawData;
			glyphData = makeDrawData(mCurrentFont, glyphData, lineText[iPos], &drawData);
			tmpDrawDataList.push_back(drawData);

			// 行サイズ更新
			lineSize->w = std::max(lineSize->w, drawData.Area.x + drawData.Area.w);
			lineSize->h = std::max(lineSize->h, drawData.Area.y + drawData.Area.h);

			iPos++;
		}
	}

	// tmpDrawDataList 位置調整
	ln_foreach(DrawData& drawData, tmpDrawDataList)
	{
		drawData.GlyphData.GlyphOffsetX += startPoint.x;
		drawData.GlyphData.GlyphOffsetY += startPoint.y;
	}

	// 全体のリストに結合
	mDrawDataQueue.insert(mDrawDataQueue.end(), tmpDrawDataList.begin(), tmpDrawDataList.end());
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
FontGlyphData* FormattedTextRenderer::makeDrawData(Font* font, FontGlyphData* prevGlyphData, UTF32 ch, DrawData* drawData)
{
	// グリフ取得
	FontGlyphData* glyphData = font->makeGlyphData(ch, prevGlyphData);

	// 描画データとして格納
	DrawData data;
	data.setGlyphData(glyphData);
	data.WaitCount = mTypingSpeed;	// 通常のタイプスピード
	data.Area.set(glyphData->GlyphOffsetX, glyphData->GlyphOffsetY, glyphData->GlyphBitmap->getWidth(), glyphData->GlyphBitmap->getHeight());
	// アウトラインがある場合はそちらの方が大きくなるはず。
	if (glyphData->OutlineBitmap) {
		data.Area.w = glyphData->OutlineBitmap->getWidth();
		data.Area.h = glyphData->OutlineBitmap->getHeight();
	}

	return glyphData;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void FormattedTextRenderer::alignGlyhpsH(DrawDataQueue& glyphs, int areaWidth)
{
	// グリフの最大サイズを求める
	LSize maxGlyphSize;
	ln_foreach(DrawData& data, glyphs)
	{
		maxGlyphSize.w = std::max(maxGlyphSize.w, data.Area.w);
		maxGlyphSize.h = std::max(maxGlyphSize.h, data.Area.h);
	}

	int gh = maxGlyphSize.w / 2;
	int interval = (interval - (gh * 2)) / glyphs.size();	// 1文字ごとの間隔

	// 描画位置更新
	int x = gh;
	ln_foreach(DrawData& data, glyphs)
	{
		data.Area.x = x;
		x += interval;
	}
}

#if 0

//==============================================================================
// FormattedTextRenderer::RenderingContext
//==============================================================================
class FormattedTextRenderer::RenderingContext
{
public:
	RenderingContext()
		: mFont	(NULL)
	{
	}

	~RenderingContext()
	{
		LN_SAFE_RELEASE(mFont);
		LN_SAFE_RELEASE(mFontTexture);
	}

	void creaet(FormattedTextRenderer* owner, IGraphicsDevice* device)
	{
		mOwner = owner;

		// 横方向に並べる数
		// +1.0 は切り捨て対策。テクスチャサイズはmaxCharactersが収まる大きさであれば良い。
		// (小さくなければOK)
		lnU32 width = (lnU32)(sqrt((double)mOwner->mMaxCharacters) + 1.0);

		// テクスチャ作成
		mFontTexture = device->createTexture(width, width, 1, SurfaceFormat_A8R8G8B8);
	}

	void prepareRendering(/*FormattedTextRenderer::CharImageNodeList& newCodes*/)
	{
		//mNewCodes = newCodes;
		LN_REFOBJ_SET(mFont, mOwner->mFont);
	}

	void render()
	{
		// フォントが無いと描画できない
		if (mFont == NULL) return;

		// テクスチャ更新は描画スレッドでやった方がロック衝突しづらいはず
		TextureScopedLock lock(mFontTexture);
		Bitmap bmp;
		lock.createRefBitmap(&bmp);

		//ln_foreach(FormattedTextRenderer::CharImage& c, mNewCodes)
		//{

		//}
	}

private:
	FormattedTextRenderer*		mOwner;
	Texture*			mFontTexture;
	//CharImageNodeList	mNewCodes;
	Font*				mFont;
};

//==============================================================================
// FormattedTextRenderer
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	FormattedTextRenderer::FormattedTextRenderer()
	{

	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	FormattedTextRenderer::~FormattedTextRenderer()
	{
		LN_SAFE_RELEASE(mFont);
		SAFE_DELETE(mRenderingContext);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void FormattedTextRenderer::creaet(IGraphicsDevice* device, int maxCharacters)
	{
		mMaxCharacters = maxCharacters;

		mRenderingContext = LN_NEW RenderingContext();
		mRenderingContext->creaet(this, device);

#if 0
		// 未使用インデックスを用意
		for (int i = 0; i < mMaxCharacters; ++i)
		{
			mImageIndexStack.push(i);
		}
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void FormattedTextRenderer::setText(const wchar_t* text)
	{
		mText = text;

		// 文字登録
		//for (size_t i = 0; i < mText.size(); ++i)
		//{
		//	tryRegisterChar(mText[i]);
		//}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void FormattedTextRenderer::update(float elapsedTime)
	{
	}

	////----------------------------------------------------------------------
	////
	////----------------------------------------------------------------------
	//void FormattedTextRenderer::prepareRendering()
	//{
	//	

	//	//mRenderingContext->prepareRendering(mNewCodes);
	//	mRenderingContext->prepareRendering();
	//	//mNewCodes.clear();
	//}

	////----------------------------------------------------------------------
	////
	////----------------------------------------------------------------------
	//void FormattedTextRenderer::render()
	//{
	//	mRenderingContext->render();
	//}

#if 0
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void FormattedTextRenderer::tryRegisterChar(lnU32 code)
	{
		// キャッシュ検索
		CharImageMap::iterator itr = mCharImageMap.find(code);

		// 見つかった。末尾に付け替えて最新とする
		if (itr != mCharImageMap.end())
		{
			mCharImageCacheList.remove(itr->second);
			mCharImageCacheList.push_back(itr->second);
		}
		// 見つからなかったので新しく作る
		else
		{
			// 今回のフレームで最大文字種類数を突破していないかチェック
			if (mNewCodes.size() == mMaxCharacters)
			{
				LN_THROW_InvalidCall(0);
			}

			// この追加処理でキャッシュを溢れる場合は一番古いものを削除
			if (mCharImageCacheList.size() >= mMaxCharacters)
			{
				CharImageNode* n = mCharImageCacheList.front();
				mCharImageCacheList.pop_front();
				mImageIndexStack.push(n->ImageIndex);	// インデックスを戻す
				SAFE_DELETE(n);
			}

			// 作成
			CharImageNode* n = LN_NEW CharImageNode;
			n->CharCode = code;
			mCharImageCacheList.push_back(n);

			// インデックス割り当て
			n->ImageIndex = mImageIndexStack.top();		
			mImageIndexStack.pop();

			// 今フレームで新しく追加された文字リストへ
			CharImage c;
			c.CharCode = n->CharCode;
			c.ImageIndex = n->ImageIndex;
			mNewCodes.push_back(c);
		}
	}
#endif

#endif

} // namespace Graphics
} // namespace Core
} // namespace LNote
