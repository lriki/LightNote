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
×	# \A[数値(,数値)]: アクター顔グラフィックの表示
○	# \R[本文,ルビ] : 本分の上にルビを挿入
×	# \E[数値(,数値)]: イベントX 番の…『(1):上に(2):下に』吹き出し表示
△	# \O[数]  : 外字の表示。"Letter_0.png"という形でWindowskinsフォルダに入れておくと、
	#           該当画像を4×4の外字画像と判断し、左上を0,右隣を1,2,3,一段下がって4,5,6…という形で読み込む。
	#           16を超えると今度は"Letter_1.png"を読み込んでいく。
○	# \S[数値]      : 文字スピードを通常の数値倍にする（小数不可）
○	# \F[数値]      : 文字の大きさ変更。通常は22
○	# \.            : 1/4秒ウェイト
○	# \;            : 1/2秒ウェイト
○	# \|            : 1秒ウェイト
×	# \!            : 文字の表示を一旦停止し、キー入力を待つ		→ pause() とか
○	# \>\<          : \>\<の間にある文を一瞬で表示する
×	# \^            : キー入力を待たずに強制的にウィンドウを閉じる
×	# \I            : インデントの設定。改行時、文を挿入地点から折り返し表示させる
○	# \LT           : 文の左詰。インデントの解除
○	# \CT           : 文の中央構え
○	# \RT           : 文の右詰
×	# \N            : 強制改行
×	# \P            : 次の改行を無視
○	# <B>           : 太字の開始					→ \b
○	# </B>          : 太字の解除					→ \b0
○	# <I>           : イタリック（斜め文字）の開始	→ \i
○	# </I>          : イタリック（斜め文字）の解除	→ \i0

○	# \C			: 文字色の切り替え
○	# ダブル Esc	: Esc 文字の表示

名前の置換系はどうやるの？

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
	/// 作成 
	void creaet(Manager* manager);

	/// 描画先テクスチャの設定
	void setTargetTexture(Texture* texture) { LN_REFOBJ_SET(mTargetTexture, texture); }

	/// フォントの設定 (このサイズがベースサイズとなる。ルビのサイズはこれを元に決まる)
	void setBaseFont(Font* font);

	/// 描画領域の設定 (0, 0, 0, 0) の場合は描画先テクスチャ全体
	void setDrawArea(const LRect& area);

	/// タイピングスピードの設定
	void setTypingSpeed(int speed) { mTypingSpeed = speed; }

	/// 表示文字列の設定
	void setText(const UTF32* text);

	/// 色コードに対応する色の設定
	void setIndexColor(int index, const LColor& color);

	/// エスケープ文字の設定 (ASCII で設定すること。デフォルトは '@')
	void setOperatorEscCode(uint32_t code);

	/// ユーザー定義制御文字の追加 (id は 0 より大きい値であること)
	void addUserOperatorCode(uint32_t code, int id);

	/// フレーム更新
	void update();

#if 0
	/// フレーム更新
	void beginUpdate();

	/// フレーム更新
	void endUpdate();

	/// 最後の update() で出力した範囲に含まれるユーザー定義制御文字数
	int getProcessingOperatorCount();

	/// 最後の update() で出力した範囲に含まれるユーザー定義制御文字のID
	int getProcessingOperatorID(int dataIndex);

	/// 最後の update() で出力した範囲に含まれるユーザー定義制御文字の引数
	const wchar_t* getProcessingOperatorArg(int dataIndex, int argIndex);

	/// 最後の update() で出力した範囲に含まれるユーザー定義制御文字の置換文字列
	void setProcessingOperatorText(int dataIndex, const lnString& text);
#endif

	/// 全ての処理が完了しているかを判定する
	bool isFinished() const;

private:

	// ※この構造体はコピーされる
	struct DrawData
	{
		int				WaitCount;	///< 残り待ち時間
		FontGlyphData	GlyphData;
		LRect			Area;		///< 描画する領域
		LColor			ForeColor;
		LColor			EdgeColor;

		void setGlyphData(const FontGlyphData* data)
		{
			GlyphData.GlyphBitmap = data->GlyphBitmap->copy();
			GlyphData.OutlineBitmap = (data->OutlineBitmap != NULL) ? data->OutlineBitmap->copy() : NULL;
			GlyphData.GlyphOffsetX = data->GlyphOffsetX;	// 使わないけど
			GlyphData.GlyphOffsetY = data->GlyphOffsetY;	// 使わないけど
			GlyphData.OutlineOffset = data->OutlineOffset;
			GlyphData.MetricsHeight = data->MetricsHeight;	// 使わないけど
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
		int			Length;			///< この制御文字列シーケンスを構成する文字数 (先頭 @ を含む)
		utf32string ReplacementText;
		std::vector<lnString> Args;	// 外部に公開するため lnString
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

	OperatorTokenArray	mOperatorTokenArray;	///< setText() で抽出した制御文字情報

	// compile 中情報
	int					mOperatorCodeCount;
	Font*				mCurrentFont;
	DrawDataQueue		mDrawDataQueue;
	//LPoint				mCurrentLineTop;
	//int					mCompileCursor;

	// 描画中情報
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
	/// 作成 (キャッシュ最大文字数を指定。異なる文字を最大maxCharacters描画できる)
	void creaet(IGraphicsDevice* device, int maxCharacters);

	/// フォントの設定
	void setFont(Font* font) { LN_REFOBJ_SET(mFont, font); }

	/// 表示文字列の設定
	void setText(const char* text);

	/// 表示文字列の設定
	void setText(const wchar_t* text);

	/// タイピングスピードの設定
	void setTypingSpeed(int speed) { mTypingSpeed = speed; }

	/// フレーム更新
	void update(float elapsedTime);

	///// 描画準備 (同期フェーズ)
	//void prepareRendering();

	///// 描画
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
