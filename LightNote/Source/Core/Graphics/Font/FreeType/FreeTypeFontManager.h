//=============================================================================
//【 FreeTypeFontManager 】
//-----------------------------------------------------------------------------
///**
//  @file       FreeTypeFontManager.h
//  @brief      FreeTypeFontManager
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <map>
#include "../../../Base/ReferenceBuffer.h"
#include "../FontManagerBase.h"

typedef int  FT_Error;
typedef void*  FT_Pointer;
typedef FT_Pointer  FTC_FaceID;
typedef struct FT_LibraryRec_  *FT_Library;
typedef struct FTC_ManagerRec_*  FTC_Manager;
typedef struct FTC_CMapCacheRec_*  FTC_CMapCache;
typedef struct FTC_ImageCacheRec_*  FTC_ImageCache;
typedef struct FT_FaceRec_*  FT_Face;
typedef struct FT_StrokerRec_*  FT_Stroker;

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
class FreeTypeFont;

//=============================================================================
// ■ FreeTypeFontManager
//-----------------------------------------------------------------------------
///**
//  @brief     フォントの管理クラス
//*/
//=============================================================================
class FreeTypeFontManager
    : public FontManagerBase
{
public:

    FreeTypeFontManager();
    virtual ~FreeTypeFontManager();

public:

    /// 初期化
    void initialize( FileIO::Manager* file_manager );

    /// 終了処理
    virtual void finalize();

    /// フォントファイルを追加する (ttf)
    virtual void addFontFile( const lnChar* filepath );

    /// フォント作成
    virtual IFont* createFont();

public:

    FT_Library getFTLibrary() const { return mFTLibrary; }
    FTC_Manager getFTCacheManager() const { return mFTCacheManager; }
    FTC_CMapCache getFTCacheMapCache() const { return mFTCMapCache; }
    FTC_ImageCache getFTCImageCache() const { return mFTImageCache; }

private:

    FT_Error faceRequester(
		FTC_FaceID face_id,
		FT_Library library,
		FT_Pointer request_data,
		FT_Face* aface );

    static FT_Error callbackFaceRequester(
		FTC_FaceID face_id,
		FT_Library library,
		FT_Pointer request_data,
		FT_Face* aface );

private:

    typedef std::map<lnString, Base::ReferenceBuffer*>     FontFileDataMap;
    typedef std::pair<lnString, Base::ReferenceBuffer*>    FontFileDataPair;

    FileIO::Manager*  mFileManager;

    FT_Library      mFTLibrary;
    FTC_Manager     mFTCacheManager;
    FTC_CMapCache   mFTCMapCache;
    FTC_ImageCache  mFTImageCache;

    FontFileDataMap mFontFileDataMap;


    // 削除予定
    typedef std::map<lnString, FT_Face>     FaceMap;
    typedef std::pair<lnString, FT_Face>    FacePair;
    FaceMap         mFilenameMap;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//
//=============================================================================