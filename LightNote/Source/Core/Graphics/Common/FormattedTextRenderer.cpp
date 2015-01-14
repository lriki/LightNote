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

	// ���䕶���𒊏o����
	for (int i = 0; i < mText.size();)
	{
		// �G�X�P�[�v��������������
		if (mText[i] == mOperatorEsc)
		{
			int opStart = i;
			i++;
			if (i >= mText.size()) {
				break;	// �I�[
			}

			// ���䕶������
			int id = 0;
			for (int iOp = 0; iOp < mOperatorCodeArray.size(); ++iOp)
			{
				if (Base::StringUtils::strcmpN(
					&mText[iOp],
					mOperatorCodeArray[iOp].Code.c_str(),
					mOperatorCodeArray[iOp].Code.size()))
				{
					id = mOperatorCodeArray[iOp].ID;
					i += mOperatorCodeArray[iOp].Code.size();	// i �͐��䕶���̎����w��
					break;
				}
			}

			// �o�^���ꂽ���䕶���ł���ꍇ
			if (id != 0)
			{
				// �����`�F�b�N
				if ((i + 2) < mText.size() &&				// '['']' �Œ�2�������͂���
					mText[i] == OperatorArgsLeftBracket)	// �擪 [
				{
					i++;

					// �I�[ ] ��T��
					int pos = Base::StringUtils::indexOf<UTF32>(&mText[i], OperatorArgsRightBracket, 0);
					if (pos == -1) {
						// �I�[ ] ��������Ȃ�����
					}
					// [] ����łȂ���Ε���
					else if (pos > 0)
					{
						utf32string t(mText, i, pos);
						// , ����
						std::vector<utf32string> args;
						Base::StringUtils::split(t.c_str(), (UTF32)',', Base::StringSplitOptions_None, &args);
						
						i += pos + 1;	// i �� ] �̎����w��
						
						// �����t���䕶���Ƃ��ă��X�g�A�b�v
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
					// �����������䕶���Ƃ��ă��X�g�A�b�v
					OperatorToken token;
					token.ID = id;
					token.Length = (i - opStart);
					mOperatorTokenArray.push_back(token);
				}
			}
			else
			{
				// �o�^����Ă��Ȃ����䕶���������B
				// ���̎��_�� i �� @ �̎��̕������w���Ă���B
			}
		}
		// ���ʂ̕����������ꍇ
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
	// setText() ��̏��� update() �Ȃ�R���p�C�����s
	if (mTextModified) {
		compile();
		mTextModified = false;
		mCurrentDrawDataIndex = 0;
	}

	TextureScopedLock texLock(mTargetTexture, true);

	while (mCurrentDrawDataIndex < mDrawDataQueue.size())
	{
		DrawData& drawData = mDrawDataQueue[mCurrentDrawDataIndex];

		// �҂����Ԃ�����ꍇ�͏I��
		if (drawData.WaitCount > 0) {
			drawData.WaitCount--;
			break;
		}

		// ���݂̍s�̕`��͈�
		LRect drawArea(0, 0, 256, 256);

		// �����̓]����
		LRect destRect(
			drawArea.x + drawData.GlyphData.GlyphOffsetX,
			drawArea.y + drawData.GlyphData.GlyphOffsetY,
			drawArea.w - drawData.GlyphData.GlyphOffsetX,
			drawArea.h - drawData.GlyphData.GlyphOffsetY);

		// �A�E�g���C���̂��镶���̕`��
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

		// �����`��
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



		// �`��L���[�擪�̃A�C�e���̑҂����Ԃ�����ꍇ�̓J�E���g�����炵�A�������Ȃ�

		// ��͂��A�`��L���[���\�z�B
		// �����������[�U�[��`���䕶����͐�p�̃��X�g�ցB
		// ���r�ȂǁA�҂����Ԃ��w�肵�ăL���[�ɓ���邱�Ƃ��ł���B
		// �E�����Ȃǂ̂��߂ɍs�T�C�Y���K�v�B�Œ�ł�1�s�ۂ��Ɖ�͂��Ȃ��ƃ_���B

		// �`��L���[�̓��e�ŕ`��


		// 


		/*
		�E���[�U�[��`���䕶��
		�E���[�U�[��`���䕶���̒u��

		1����͂�2����͂ɕ�����
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
	LN_REFOBJ_SET(mCurrentFont, mBaseFont->copy());	// �t�H���g���Z�b�g

	const UTF32* str = mText.c_str();
	const UTF32* end = &mText[mText.size()];
	LPoint nextLineStartPoint(0, 0);
	while (true)
	{
		LSize lineSize;

		// ���݈ʒu���玟�̉��s�܂��͏I�[�܂Ō���
		const UTF32* nl = Base::StringUtils::findNextNewLine(str, end);
		if (nl == end){
			compileLine(str, end - str, nextLineStartPoint, &lineSize);	// �Ō�̍s
			break;							//�@�I��
		}

		// 1�s���
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
		// �G�X�P�[�v��������������
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
				// ���r�O���t���쐬���A���̕`��̈�T�C�Y���v�Z����
				DrawDataQueue rubyDrawDataList;
				LSize rubyAreaSize;
				utf32string rubyText;
				rubyText.assignNativeString(opToken.Args[1].c_str());
				FontGlyphData* rubyGlyphData = NULL;
				for (int i = 0; i < rubyText.size(); i++)
				{
					// �`��f�[�^��1���������
					DrawData data;
					rubyGlyphData = makeDrawData(mRubyFont, rubyGlyphData, rubyText[i], &data);
					rubyDrawDataList.push_back(data);

					// �s�T�C�Y�E�ő�O���t�T�C�Y�X�V
					rubyAreaSize.w = std::max(rubyAreaSize.w, data.Area.x + data.Area.w);
					rubyAreaSize.h = std::max(rubyAreaSize.h, data.Area.y + data.Area.h);
				}

				// ��C�������O���t���쐬���A���̕`��̈�T�C�Y���v�Z����
				DrawDataQueue srcDrawDataList;
				LSize srcAreaSize;
				lnString srcText;
				srcText.assignNativeString(opToken.Args[0].c_str());
				for (int i = 0; i < srcText.size(); i++)
				{
					// �`��f�[�^��1���������
					DrawData data;
					glyphData = makeDrawData(mCurrentFont, glyphData, srcText[i], &data);
					srcDrawDataList.push_back(data);

					// �s�T�C�Y�E�ő�O���t�T�C�Y�X�V
					srcAreaSize.w = std::max(srcAreaSize.w, data.Area.x + data.Area.w);
					srcAreaSize.h = std::max(srcAreaSize.h, data.Area.y + data.Area.h); 
				}

				// ���r�̕`�敝�̕����������ꍇ�̓��r�����̈ʒu�𒲐�����
				if (rubyAreaSize.w < srcAreaSize.h) {
					alignGlyhpsH(rubyDrawDataList, srcAreaSize.w);
				}
				// ��C��������̕`�敝�̕����������ꍇ�͔�C��������̈ʒu�𒲐�����
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
		// ���ʂ̕����������炱���ɗ���
		else
		{
			// �`��f�[�^��1���������
			DrawData drawData;
			glyphData = makeDrawData(mCurrentFont, glyphData, lineText[iPos], &drawData);
			tmpDrawDataList.push_back(drawData);

			// �s�T�C�Y�X�V
			lineSize->w = std::max(lineSize->w, drawData.Area.x + drawData.Area.w);
			lineSize->h = std::max(lineSize->h, drawData.Area.y + drawData.Area.h);

			iPos++;
		}
	}

	// tmpDrawDataList �ʒu����
	ln_foreach(DrawData& drawData, tmpDrawDataList)
	{
		drawData.GlyphData.GlyphOffsetX += startPoint.x;
		drawData.GlyphData.GlyphOffsetY += startPoint.y;
	}

	// �S�̂̃��X�g�Ɍ���
	mDrawDataQueue.insert(mDrawDataQueue.end(), tmpDrawDataList.begin(), tmpDrawDataList.end());
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
FontGlyphData* FormattedTextRenderer::makeDrawData(Font* font, FontGlyphData* prevGlyphData, UTF32 ch, DrawData* drawData)
{
	// �O���t�擾
	FontGlyphData* glyphData = font->makeGlyphData(ch, prevGlyphData);

	// �`��f�[�^�Ƃ��Ċi�[
	DrawData data;
	data.setGlyphData(glyphData);
	data.WaitCount = mTypingSpeed;	// �ʏ�̃^�C�v�X�s�[�h
	data.Area.set(glyphData->GlyphOffsetX, glyphData->GlyphOffsetY, glyphData->GlyphBitmap->getWidth(), glyphData->GlyphBitmap->getHeight());
	// �A�E�g���C��������ꍇ�͂�����̕����傫���Ȃ�͂��B
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
	// �O���t�̍ő�T�C�Y�����߂�
	LSize maxGlyphSize;
	ln_foreach(DrawData& data, glyphs)
	{
		maxGlyphSize.w = std::max(maxGlyphSize.w, data.Area.w);
		maxGlyphSize.h = std::max(maxGlyphSize.h, data.Area.h);
	}

	int gh = maxGlyphSize.w / 2;
	int interval = (interval - (gh * 2)) / glyphs.size();	// 1�������Ƃ̊Ԋu

	// �`��ʒu�X�V
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

		// �������ɕ��ׂ鐔
		// +1.0 �͐؂�̂đ΍�B�e�N�X�`���T�C�Y��maxCharacters�����܂�傫���ł���Ηǂ��B
		// (�������Ȃ����OK)
		lnU32 width = (lnU32)(sqrt((double)mOwner->mMaxCharacters) + 1.0);

		// �e�N�X�`���쐬
		mFontTexture = device->createTexture(width, width, 1, SurfaceFormat_A8R8G8B8);
	}

	void prepareRendering(/*FormattedTextRenderer::CharImageNodeList& newCodes*/)
	{
		//mNewCodes = newCodes;
		LN_REFOBJ_SET(mFont, mOwner->mFont);
	}

	void render()
	{
		// �t�H���g�������ƕ`��ł��Ȃ�
		if (mFont == NULL) return;

		// �e�N�X�`���X�V�͕`��X���b�h�ł�����������b�N�Փ˂��Â炢�͂�
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
		// ���g�p�C���f�b�N�X��p��
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

		// �����o�^
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
		// �L���b�V������
		CharImageMap::iterator itr = mCharImageMap.find(code);

		// ���������B�����ɕt���ւ��čŐV�Ƃ���
		if (itr != mCharImageMap.end())
		{
			mCharImageCacheList.remove(itr->second);
			mCharImageCacheList.push_back(itr->second);
		}
		// ������Ȃ������̂ŐV�������
		else
		{
			// ����̃t���[���ōő啶����ސ���˔j���Ă��Ȃ����`�F�b�N
			if (mNewCodes.size() == mMaxCharacters)
			{
				LN_THROW_InvalidCall(0);
			}

			// ���̒ǉ������ŃL���b�V��������ꍇ�͈�ԌÂ����̂��폜
			if (mCharImageCacheList.size() >= mMaxCharacters)
			{
				CharImageNode* n = mCharImageCacheList.front();
				mCharImageCacheList.pop_front();
				mImageIndexStack.push(n->ImageIndex);	// �C���f�b�N�X��߂�
				SAFE_DELETE(n);
			}

			// �쐬
			CharImageNode* n = LN_NEW CharImageNode;
			n->CharCode = code;
			mCharImageCacheList.push_back(n);

			// �C���f�b�N�X���蓖��
			n->ImageIndex = mImageIndexStack.top();		
			mImageIndexStack.pop();

			// ���t���[���ŐV�����ǉ����ꂽ�������X�g��
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
