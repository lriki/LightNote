//=============================================================================
//【 FreeTypeFontManager 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <ft2build.h>
#include FT_FREETYPE_H	/* <freetype/freetype.h> */
#include FT_CACHE_H	/* <freetype/ftcache.h> */
#include FT_TRUETYPE_TAGS_H	/* <freetype/tttags.h> */
#include FT_TRUETYPE_TABLES_H	/* <freetype/tttables.h> */
#include FT_SFNT_NAMES_H
#include "../../../FileIO/Interface.h"
#include "../../../FileIO/Manager.h"
#include "FreeTypeFont.h"
#include "FreeTypeFontManager.h"

#include "../../../System/Common/PerfStopwatch.h"
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

//=============================================================================
// ■ FreeTypeFontManager クラス
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    FreeTypeFontManager::FreeTypeFontManager()
        : mFileManager      ( NULL )
        , mFTLibrary        ( NULL )
        , mFTCacheManager   ( NULL )
        , mFTCMapCache      ( NULL )
        , mFTImageCache     ( NULL )
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    FreeTypeFontManager::~FreeTypeFontManager()
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void FreeTypeFontManager::initialize( FileIO::Manager* file_manager )
    {
        mFileManager = file_manager;

        FT_Error err = FT_Init_FreeType( &mFTLibrary );
        LN_THROW_InvalidCall( err == 0, "failed initialize font library : %d\n", err );

        err = FTC_Manager_New(
            mFTLibrary,
            0, 0, 0,
		    callbackFaceRequester,
            this,
		    &mFTCacheManager);
        LN_THROW_InvalidCall( err == 0, "failed initialize font cache manager : %d\n", err );

        err = FTC_CMapCache_New( mFTCacheManager, &mFTCMapCache );
        LN_THROW_InvalidCall( err == 0, "failed initialize font cache map : %d\n", err );

	    err = FTC_ImageCache_New( mFTCacheManager, &mFTImageCache );
        LN_THROW_InvalidCall( err == 0, "failed initialize font image cache : %d\n", err );

        FontManagerBase::initialize();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void FreeTypeFontManager::finalize()
    {
        FontManagerBase::finalize();

        FontFileDataMap::iterator itr = mFontFileDataMap.begin();
        for ( ; itr != mFontFileDataMap.end(); ++itr )
        {
            LN_SAFE_RELEASE( itr->second );
        }
        mFontFileDataMap.clear();

        if ( mFTCacheManager != NULL )
        {
		    FTC_Manager_Done( mFTCacheManager );
            mFTCacheManager = NULL;
        }

        if ( mFTLibrary != NULL )
        {
	        FT_Done_FreeType( mFTLibrary );
            mFTLibrary = NULL;
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void FreeTypeFontManager::addFontFile( const lnChar* filepath )
    {
        LRefPtr<FileIO::InFile> file( mFileManager->createInFile( filepath ) );

        LRefPtr<Base::ReferenceBuffer> buffer( LN_NEW Base::ReferenceBuffer() );
        buffer->reserve( file->getSize() );
        file->read( buffer->getPointer(), file->getSize() );

        // ファミリ名を取得するため、一度生成する
        FT_Face face;
        FT_Error err = FT_New_Memory_Face(
            mFTLibrary,
            (const FT_Byte*)buffer->getPointer(),
            buffer->getSize(),
            0, 
            &face );
        LN_THROW_InvalidCall( err == FT_Err_Ok, "failed FT_New_Memory_Face : %d\n", err );

        // ファミリ名でデータバッファ登録
        lnString family_name( face->family_name );
        FontFileDataMap::iterator itr = mFontFileDataMap.find( family_name );
        if ( itr == mFontFileDataMap.end() )
        {
            mFontFileDataMap.insert( FontFileDataPair( family_name, buffer ) );
            buffer->addRef();

#if 0
            printf( "register font: %s\n", face->family_name );
            FT_UInt count = FT_Get_Sfnt_Name_Count(face);
            _p(count);
            for (int i = 0 ; i < count; ++i)
            {
                FT_SfntName ft_name;
                FT_Get_Sfnt_Name(face, i, &ft_name);

                printf("####\n");
                _p(ft_name.platform_id);
                _p(ft_name.encoding_id);
                _p(ft_name.language_id);
                _p(ft_name.name_id);

                if (ft_name.encoding_id)
                {
                    std::wstring n((wchar_t*)ft_name.string, ft_name.string_len);
                    wprintf(L"%s\n", n.c_str());
                    //_pw(n.c_str());
                }
                else
                {
                    lnString n((char*)ft_name.string, ft_name.string_len);
                    _p(n.c_str());
                }
            }
#endif
        }

        FT_Done_Face( face );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    IFont* FreeTypeFontManager::createFont()
    {
        FreeTypeFont* font = LN_NEW FreeTypeFont( this );
        font->initialize("", 0, LColor(), false , false);
        return font;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    FT_Error FreeTypeFontManager::faceRequester(
		FTC_FaceID face_id,
		FT_Library library,
		FT_Pointer request_data,
		FT_Face* aface )
    {
        lnString family_name( (lnChar*)face_id );
        FontFileDataMap::iterator itr = mFontFileDataMap.find( family_name );
        if ( itr != mFontFileDataMap.end() )
        {
            FT_Face face;
            FT_Error err = FT_New_Memory_Face(
                mFTLibrary,
                (const FT_Byte*)itr->second->getPointer(),
                itr->second->getSize(),
                0, 
                &face );
            LN_THROW_InvalidCall( err == FT_Err_Ok, "failed FT_New_Memory_Face : %d\n", err );

            err = FT_Select_Charmap( face, FT_ENCODING_UNICODE );
            if ( err != FT_Err_Ok )
            {
                FT_Done_Face( face );
                LN_THROW_InvalidCall( err == FT_Err_Ok, "failed FT_Select_Charmap : %d\n", err );
            }

            *aface = face;
            return FT_Err_Ok;
        }

        return -1;

#if 0
        lnString name( (lnChar*)face_id );

        FaceMap::iterator itr = mFilenameMap.find( name );
        if ( itr != mFilenameMap.end() )
        {


            //LRefPtr<FileIO::InFile> file( mFileManager->createInFile( itr->first.c_str() ) );
            //file->load();

            // 確保した領域はフェイス使用終了まで解放してはならない


            FileIO::InFile* file = mFileManager->createInFile( itr->first.c_str() );
            file->load();

            FT_Open_Args args;
            memset( &args, 0, sizeof(args) );
            args.flags = FT_OPEN_MEMORY;
            args.memory_base = (const FT_Byte*)file->getData();
            args.memory_size = file->getSize();

System::PerfStopwatch sw;
            FT_Face face;
            FT_Error err = FT_Open_Face( mFTLibrary, &args, 0, &face );
            LN_THROW_InvalidCall( err == FT_Err_Ok, "failed FT_Open_Face : %d\n", err );
            sw.stopS();

            _p(face->family_name);


            FT_UInt count = FT_Get_Sfnt_Name_Count(face);
            _p(count);
            for (int i = 0 ; i < count; ++i)
            {
                FT_SfntName ft_name;
                FT_Get_Sfnt_Name(face, i, &ft_name);

                printf("####\n");
                _p(ft_name.platform_id);
                _p(ft_name.encoding_id);
                _p(ft_name.language_id);
                _p(ft_name.name_id);

                if (ft_name.encoding_id)
                {
                    std::wstring n((wchar_t*)ft_name.string, ft_name.string_len);
                    wprintf(L"%s\n", n.c_str());
                    //_pw(n.c_str());
                }
                else
                {
                    lnString n((char*)ft_name.string, ft_name.string_len);
                    _p(n.c_str());
                }
            }

            //FT_Face face;
            //FT_Error err = FT_New_Face( mFTLibrary, filepath, 0, &face );
            //LN_THROW_InvalidCall( err == FT_Err_Ok, "failed FT_New_Face : %d\n", err );
            
            err = FT_Select_Charmap( face, FT_ENCODING_UNICODE );
            if ( err != FT_Err_Ok )
            {
                FT_Done_Face(face);
                LN_THROW_InvalidCall( err == FT_Err_Ok, "failed FT_Select_Charmap : %d\n", err );
            }



            *aface = face;//itr->second;
            return FT_Err_Ok;
        }
        
        return -1;
#endif
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    FT_Error FreeTypeFontManager::callbackFaceRequester(
		FTC_FaceID face_id,
		FT_Library library,
		FT_Pointer request_data,
		FT_Face* aface )
    {
        return ((FreeTypeFontManager*)request_data)->faceRequester( face_id, library, request_data, aface );
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//
//=============================================================================