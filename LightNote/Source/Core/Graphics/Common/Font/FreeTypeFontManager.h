//==============================================================================
// FreeTypeFontManager 
//------------------------------------------------------------------------------
///**
//  @file       FreeTypeFontManager.h
//  @brief      FreeTypeFontManager
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <map>
#include "../../../Base/ReferenceBuffer.h"
#include "FontManagerBase.h"

typedef int  FT_Error;
typedef void*  FT_Pointer;
typedef FT_Pointer  FTC_FaceID;
typedef struct FT_LibraryRec_  *FT_Library;
typedef struct FTC_ManagerRec_*  FTC_Manager;
typedef struct FTC_CMapCacheRec_*  FTC_CMapCache;
typedef struct FTC_ImageCacheRec_*  FTC_ImageCache;
typedef struct FT_FaceRec_*  FT_Face;
typedef struct FT_StrokerRec_*  FT_Stroker;
typedef struct FT_OutlineGlyphRec_*  FT_OutlineGlyph;
typedef struct FT_StreamRec_*  FT_Stream;

namespace LNote
{
namespace Core
{
namespace Graphics
{
class FreeTypeFont;

//==============================================================================
// ■ FreeTypeFontManager
//------------------------------------------------------------------------------
///**
//  @brief     フォントの管理クラス
//*/
//==============================================================================
class FreeTypeFontManager
    : public FontManagerBase
{
public:
	static const lnChar* DefaultFontName;	// デフォルトフォント名 (MS PGothic)

public:
    FreeTypeFontManager();
    virtual ~FreeTypeFontManager();

public:

	/// 初期化
    void initialize( FileIO::Manager* file_manager );

	/// 終了処理
    virtual void finalize();

	/// フォントファイルを追加する (ttf)
    virtual void addFontFile( const lnChar* fontFilePath );

	/// フォント作成
	virtual Font* createFont();

	/// ビットマップフォントの作成
	virtual Font* createBitmapFont(Texture* texture);

public:
    FT_Library getFTLibrary() const { return mFTLibrary; }
    FTC_Manager getFTCacheManager() const { return mFTCacheManager; }
    FTC_CMapCache getFTCacheMapCache() const { return mFTCMapCache; }
    FTC_ImageCache getFTCImageCache() const { return mFTImageCache; }

private:

	// キャッシュ検索コールバック
    FT_Error faceRequester(
		FTC_FaceID face_id,
		FT_Library library,
		FT_Pointer request_data,
		FT_Face* aface );

	// faceRequester() の呼び出し元
    static FT_Error callbackFaceRequester(
		FTC_FaceID face_id,
		FT_Library library,
		FT_Pointer request_data,
		FT_Face* aface );

#ifdef LNOTE_WIN32
	typedef struct TSystemFontData{
		HDC hdc;
		HFONT hFont;
		HFONT hOldFont;
		unsigned char *fontdata;
		size_t size;
		int index;
	} TSystemFontData;

	/// Windowsに登録されているフォントのバイナリデータを名称から取得
	TSystemFontData* getWindowsSystemFontData( LPCTSTR name );
	unsigned char* lockWindowsSystemFontData(TSystemFontData *fnt, size_t *size, int *index);
	static void freeWindowsSystemFontData(TSystemFontData *fnt);
	static unsigned long StreamIoFunc(
		FT_Stream stream,
		unsigned long offset,
		unsigned char* buffer,
		unsigned long count);
	static void FreeTypeFontManager::StreamCloseFunc(FT_Stream stream);
#endif

private:
	
#if 1
	struct TTFDataEntry
	{
		Base::ReferenceBuffer*	DataBuffer;
		int						CollectionIndex;

		/* メモリ上のデータからFaceを作る場合、FT_Done_Face() するまでメモリを開放してはならない。
		 * ファイルストリームを使うこともできるが、そうするとフォントが存在している間
		 * ストリームを1つ占有してしまうことになる。
		 * フォントファイルひとつ当たり 4M とか普通にあるけど、大体1000x1000ビットマップひとつ分。
		 * 知らないところでストリームが開きっぱなしになるのはどうかと思うし、
		 * (音声のストリーミングと違って、フォントのストリーミングなんて言っても普通はピンと来ないだろうし…)
		 * 文字列描画は速度的にかなりクリティカルなもの。メモリ上に置きっぱなしでいいと思う。
		 */
	};
	typedef std::map<lnU32, TTFDataEntry>     TTFDataEntryMap;
    typedef std::pair<lnU32, TTFDataEntry>    TTFDataEntryPair;
	TTFDataEntryMap		mTTFDataEntryMap;
#else
#if 0
	struct TTFFileEntry
	{
		std::wstring	FileFullPath;
		int				CollectionIndex;

		Base::ReferenceBuffer*	Buffer;
	};
	typedef std::map<lnString, TTFFileEntry>     FontFileEntryMap;
    typedef std::pair<lnString, TTFFileEntry>    FontFileEntryPair;
	FontFileEntryMap		mFontFileEntryMap;

#else
#if 0	// addFont() で Face を作ってしまう方式。同名・異サイズフォントでテストしてダメだったら戻す
    typedef std::map<lnString, FT_Face>     FontFaceMap;
    typedef std::pair<lnString, FT_Face>    FontFacePair;
	FontFaceMap		mFontFaceMap;
#else
    typedef std::map<lnString, Base::ReferenceBuffer*>     FontFileDataMap;
    typedef std::pair<lnString, Base::ReferenceBuffer*>    FontFileDataPair;
	FontFileDataMap mFontFileDataMap;
#endif
#endif
#endif

    FileIO::Manager*  mFileManager;

    FT_Library      mFTLibrary;
    FTC_Manager     mFTCacheManager;
    FTC_CMapCache   mFTCMapCache;
    FTC_ImageCache  mFTImageCache;


	// faceRequester() で Windows のシステムフォントを拾うための細工。
	// FreeType は FTC_Manager_LookupFace() に渡されたアドレスを直接辞書のキーとする。
	// (アドレスの中身までは見ない。そのため、文字列をキーにすることはできない)
	// つまり、faceRequester() にフォント名を伝えることはできない。なので、外部に一度とっておく必要がある。
	// この変数には、FTC_Manager_LookupFace() の直前でフォント名をセットしておく。
	// ローカル変数のポインタでよい。faceRequester() で NULL が格納される。
	const lnChar*		mRequesterFaceName;

	friend class FreeTypeFont;
	//
	bool			mUpdatedDefaultFontName;


	// 削除予定
    typedef std::map<lnString, FT_Face>     FaceMap;
    typedef std::pair<lnString, FT_Face>    FacePair;
    FaceMap         mFilenameMap;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote
