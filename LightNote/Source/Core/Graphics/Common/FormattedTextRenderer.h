//==============================================================================
// FormattedTextRenderer 
//------------------------------------------------------------------------------
///**
//  @file       FormattedTextRenderer.h
//  @brief      FormattedTextRenderer
//*/
//==============================================================================

#pragma once

#include <string>
#include <stack>
#include <map>
#include "../../Base/Container/NodeList.h"
#include "SpriteRenderer.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// FormattedTextRenderer
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//------------------------------------------------------------------------------
/*
�~	# \A[���l(,���l)]: �A�N�^�[��O���t�B�b�N�̕\��
��	# \R[�{��,���r] : �{���̏�Ƀ��r��}��
�~	# \E[���l(,���l)]: �C�x���gX �Ԃ́c�w(1):���(2):���Ɂx�����o���\��
��	# \O[��]  : �O���̕\���B"Letter_0.png"�Ƃ����`��Windowskins�t�H���_�ɓ���Ă����ƁA
	#           �Y���摜��4�~4�̊O���摜�Ɣ��f���A�����0,�E�ׂ�1,2,3,��i��������4,5,6�c�Ƃ����`�œǂݍ��ށB
	#           16�𒴂���ƍ��x��"Letter_1.png"��ǂݍ���ł����B
��	# \S[���l]      : �����X�s�[�h��ʏ�̐��l�{�ɂ���i�����s�j
��	# \F[���l]      : �����̑傫���ύX�B�ʏ��22
��	# \.            : 1/4�b�E�F�C�g
��	# \;            : 1/2�b�E�F�C�g
��	# \|            : 1�b�E�F�C�g
�~	# \!            : �����̕\������U��~���A�L�[���͂�҂�		�� pause() �Ƃ�
��	# \>\<          : \>\<�̊Ԃɂ��镶����u�ŕ\������
�~	# \^            : �L�[���͂�҂����ɋ����I�ɃE�B���h�E�����
�~	# \I            : �C���f���g�̐ݒ�B���s���A����}���n�_����܂�Ԃ��\��������
��	# \LT           : ���̍��l�B�C���f���g�̉���
��	# \CT           : ���̒����\��
��	# \RT           : ���̉E�l
�~	# \N            : �������s
�~	# \P            : ���̉��s�𖳎�
��	# <B>           : �����̊J�n					�� \b
��	# </B>          : �����̉���					�� \b0
��	# <I>           : �C�^���b�N�i�΂ߕ����j�̊J�n	�� \i
��	# </I>          : �C�^���b�N�i�΂ߕ����j�̉���	�� \i0

��	# \C			: �����F�̐؂�ւ�
��	# �_�u�� Esc	: Esc �����̕\��

���O�̒u���n�͂ǂ����́H

*/
//==============================================================================
class FormattedTextRenderer
{
public:
	typedef uint32_t	UTF32;
	static const int MaxOperatorCodeLength = 4;
	static const UTF32	OperatorArgsLeftBracket = (UTF32)'[';
	static const UTF32	OperatorArgsRightBracket = (UTF32)']';

	static const int ID_Color = -11;
	static const int ID_Ruby	= -12;
	static const int ID_Speed = -13;
	static const int ID_FontSize = -14;
	static const int ID_Wait4 = -15;
	static const int ID_Wait2 = -16;
	static const int ID_Wait1 = -17;
	static const int ID_ImmediatelyBegin = -18;
	static const int ID_ImmediatelyEnd = -19;
	static const int ID_AlignLeft = -20;
	static const int ID_AlignCenter = -21;
	static const int ID_AlignRight = -22;
	static const int ID_BoldBegin = -23;
	static const int ID_BoldEnd = -24;
	static const int ID_ItalicBegin = -25;
	static const int ID_ItalicEnd = -26;

public:
	FormattedTextRenderer();
	~FormattedTextRenderer();

public:
	/// �쐬 
	void creaet(Manager* manager);

	/// �`���e�N�X�`���̐ݒ�
	void setTargetTexture(Texture* texture) { LN_REFOBJ_SET(mTargetTexture, texture); }

	/// �t�H���g�̐ݒ� (���̃T�C�Y���x�[�X�T�C�Y�ƂȂ�B���r�̃T�C�Y�͂�������Ɍ��܂�)
	void setBaseFont(Font* font);

	/// �`��̈�̐ݒ� (0, 0, 0, 0) �̏ꍇ�͕`���e�N�X�`���S��
	void setDrawArea(const LRect& area);

	/// �^�C�s���O�X�s�[�h�̐ݒ�
	void setTypingSpeed(int speed) { mTypingSpeed = speed; }

	/// �\��������̐ݒ�
	void setText(const UTF32* text);

	/// �F�R�[�h�ɑΉ�����F�̐ݒ�
	void setIndexColor(int index, const LColor& color);

	/// �G�X�P�[�v�����̐ݒ� (ASCII �Őݒ肷�邱�ƁB�f�t�H���g�� '@')
	void setOperatorEscCode(uint32_t code);

	/// ���[�U�[��`���䕶���̒ǉ� (id �� 0 ���傫���l�ł��邱��)
	void addUserOperatorCode(uint32_t code, int id);

	/// �t���[���X�V
	void update();

#if 0
	/// �t���[���X�V
	void beginUpdate();

	/// �t���[���X�V
	void endUpdate();

	/// �Ō�� update() �ŏo�͂����͈͂Ɋ܂܂�郆�[�U�[��`���䕶����
	int getProcessingOperatorCount();

	/// �Ō�� update() �ŏo�͂����͈͂Ɋ܂܂�郆�[�U�[��`���䕶����ID
	int getProcessingOperatorID(int dataIndex);

	/// �Ō�� update() �ŏo�͂����͈͂Ɋ܂܂�郆�[�U�[��`���䕶���̈���
	const wchar_t* getProcessingOperatorArg(int dataIndex, int argIndex);

	/// �Ō�� update() �ŏo�͂����͈͂Ɋ܂܂�郆�[�U�[��`���䕶���̒u��������
	void setProcessingOperatorText(int dataIndex, const lnString& text);
#endif

	/// �S�Ă̏������������Ă��邩�𔻒肷��
	bool isFinished() const;

private:

	// �����̍\���̂̓R�s�[�����
	struct DrawData
	{
		int				WaitCount;	///< �c��҂�����
		FontGlyphData	GlyphData;
		LRect			Area;		///< �`�悷��̈�
		LColor			ForeColor;
		LColor			EdgeColor;

		void setGlyphData(const FontGlyphData* data)
		{
			GlyphData.GlyphBitmap = data->GlyphBitmap->copy();
			GlyphData.OutlineBitmap = (data->OutlineBitmap != NULL) ? data->OutlineBitmap->copy() : NULL;
			GlyphData.GlyphOffsetX = data->GlyphOffsetX;	// �g��Ȃ�����
			GlyphData.GlyphOffsetY = data->GlyphOffsetY;	// �g��Ȃ�����
			GlyphData.OutlineOffset = data->OutlineOffset;
			GlyphData.MetricsHeight = data->MetricsHeight;	// �g��Ȃ�����
		}

		void release()
		{
			LN_SAFE_RELEASE(GlyphData.GlyphBitmap);
			LN_SAFE_RELEASE(GlyphData.OutlineBitmap);
		}

		//~DrawData()
		//{
		//	LN_SAFE_RELEASE(GlyphData.GlyphBitmap);
		//	LN_SAFE_RELEASE(GlyphData.OutlineBitmap);
		//}
	};

	//struct DrawLineData
	//{
	//};

	typedef Base::BasicString<UTF32>	utf32string;
	typedef std::vector<DrawData>		DrawDataQueue;

	struct OperatorCode
	{
		utf32string		Code;
		int				ID;
	};

	typedef std::vector<OperatorCode>	OperatorCodeArray;



	struct OperatorToken
	{
		int			ID;
		int			Length;			///< ���̐��䕶����V�[�P���X���\�����镶���� (�擪 @ ���܂�)
		utf32string ReplacementText;
		std::vector<lnString> Args;	// �O���Ɍ��J���邽�� lnString
	};
	typedef std::vector<OperatorToken>	OperatorTokenArray;


private:

	void addOperatorCode(const UTF32* code, int id);
	void analyze1();
	void resetCompile();
	void compile();
	void compileLine(const UTF32* lineText, int len, const LPoint& startPoint, LSize* lineSize);
	void disposeCompiledData();
	FontGlyphData* makeDrawData(Font* font, FontGlyphData* prevGlyphData, UTF32 ch, DrawData* drawData);
	void alignGlyhpsH(DrawDataQueue& glyphs, int areaWidth);

private:
	Manager*			mManager;
	Texture*			mTargetTexture;
	LRect				mDrawArea;
	int					mTypingSpeed;
	Font*				mBaseFont;
	Font*				mRubyFont;
	utf32string			mText;
	bool				mTextModified;
	wchar_t				mOperatorEsc;
	OperatorCodeArray	mOperatorCodeArray;

	OperatorTokenArray	mOperatorTokenArray;	///< setText() �Œ��o�������䕶�����

	// compile �����
	int					mOperatorCodeCount;
	Font*				mCurrentFont;
	DrawDataQueue		mDrawDataQueue;
	//LPoint				mCurrentLineTop;
	//int					mCompileCursor;

	// �`�撆���
	int					mCurrentDrawDataIndex;
};


#if 0

//==============================================================================
// FormattedTextRenderer
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class FormattedTextRenderer
{
public:
	FormattedTextRenderer();
	~FormattedTextRenderer();

public:
	/// �쐬 (�L���b�V���ő啶�������w��B�قȂ镶�����ő�maxCharacters�`��ł���)
	void creaet(IGraphicsDevice* device, int maxCharacters);

	/// �t�H���g�̐ݒ�
	void setFont(Font* font) { LN_REFOBJ_SET(mFont, font); }

	/// �\��������̐ݒ�
	void setText(const char* text);

	/// �\��������̐ݒ�
	void setText(const wchar_t* text);

	/// �^�C�s���O�X�s�[�h�̐ݒ�
	void setTypingSpeed(int speed) { mTypingSpeed = speed; }

	/// �t���[���X�V
	void update(float elapsedTime);

	///// �`�揀�� (�����t�F�[�Y)
	//void prepareRendering();

	///// �`��
	//void render();

private:
	void tryRegisterChar(lnU32 code);

private:

#if 0
	struct CharImage
	{
		lnU32	CharCode;
		lnU32	ImageIndex;
	};

	class CharImageNode 
		: public Base::NodeList<CharImageNode>::Node
	{
	public:
		lnU32	CharCode;
		lnU32	ImageIndex;
	};

	typedef std::map<lnU32, CharImageNode*>		CharImageMap;
	typedef Base::NodeList<CharImageNode>		CharImageCacheList;
	typedef std::vector<CharImage>				CharImageNodeList;

	class CharSpriteData
	{

	};

#endif

	struct TypingCommand
	{

	};

	class RenderingContext;
	friend class RenderingContext;

private:
	int mMaxCharacters;
	//Graphics::SpriteRenderer*	mPrimaryRenderer;
	//Graphics::SpriteRenderer*	mSecondaryRenderer;
	Font*						mFont;
	std::wstring				mText;
	float						mTypingSpeed;
	RenderingContext*			mRenderingContext;

#if 0
	CharImageMap				mCharImageMap;
	CharImageCacheList			mCharImageCacheList;
	std::stack<lnU32>			mImageIndexStack;


	CharImageNodeList	mNewCodes;
#endif
};
#endif

} // namespace Graphics
} // namespace Core
} // namespace LNote
