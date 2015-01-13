//==============================================================================
// FreeTypeFontManager 
//==============================================================================

#include "stdafx.h"
#include <ft2build.h>
#include FT_FREETYPE_H	/* <freetype/freetype.h> */
#include FT_CACHE_H	/* <freetype/ftcache.h> */
#include FT_TRUETYPE_TAGS_H	/* <freetype/tttags.h> */
#include FT_TRUETYPE_TABLES_H	/* <freetype/tttables.h> */
#include FT_SFNT_NAMES_H
#include "../../../Base/STLUtils.h"
#include "../../../FileIO/Interface.h"
#include "../../../FileIO/Manager.h"
#include "BitmapFont.h"
#include "FreeTypeFont.h"
#include "FreeTypeFontManager.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// FreeTypeFontManager
//==============================================================================

	const lnChar* FreeTypeFontManager::DefaultFontName = "MS PGothic";

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    FreeTypeFontManager::FreeTypeFontManager()
        : mFileManager      ( NULL )
        , mFTLibrary        ( NULL )
        , mFTCacheManager   ( NULL )
        , mFTCMapCache      ( NULL )
        , mFTImageCache     ( NULL )
		, mRequesterFaceName( NULL )
		, mUpdatedDefaultFontName	( false )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    FreeTypeFontManager::~FreeTypeFontManager()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void FreeTypeFontManager::initialize( FileIO::Manager* file_manager )
    {
        mFileManager = file_manager;

		// FreeType 初期化
        FT_Error err = FT_Init_FreeType( &mFTLibrary );
        LN_THROW_InvalidOperation( err == 0, "failed initialize font library : %d\n", err );

		// キャッシマネージャ
        err = FTC_Manager_New(
            mFTLibrary,
            0, 0, 0,
		    callbackFaceRequester,
            this,
		    &mFTCacheManager);
        LN_THROW_InvalidOperation( err == 0, "failed initialize font cache manager : %d\n", err );

		// キャッシュマップ
        err = FTC_CMapCache_New( mFTCacheManager, &mFTCMapCache );
        LN_THROW_InvalidOperation( err == 0, "failed initialize font cache map : %d\n", err );

		// イメージキャッシュ
	    err = FTC_ImageCache_New( mFTCacheManager, &mFTImageCache );
        LN_THROW_InvalidOperation( err == 0, "failed initialize font image cache : %d\n", err );

		FreeTypeFont* font = LN_NEW FreeTypeFont( this );
        font->initialize( DefaultFontName, 0, LColor::Gray, false, false );
		font->setAntiAlias( true );
        FontManagerBase::initialize( font );
		font->release();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void FreeTypeFontManager::finalize()
    {
        FontManagerBase::finalize();
#if 1
		// 登録したTTFファイルのメモリバッファをすべて解放
		TTFDataEntryMap::iterator itr = mTTFDataEntryMap.begin();
        for ( ; itr != mTTFDataEntryMap.end(); ++itr )
        {
            LN_SAFE_RELEASE( itr->second.DataBuffer );
        }
        mTTFDataEntryMap.clear();
#else
#if 0
		mFontFileEntryMap.clear();
#else
#if 0
		FontFaceMap::iterator itr = mFontFaceMap.begin();
        for ( ; itr != mFontFaceMap.end(); ++itr )
        {
            FT_Done_Face( itr->second );
        }
        mFontFaceMap.clear();
#else
		// 登録フォントをすべて解放
        FontFileDataMap::iterator itr = mFontFileDataMap.begin();
        for ( ; itr != mFontFileDataMap.end(); ++itr )
        {
            LN_SAFE_RELEASE( itr->second );
        }
        mFontFileDataMap.clear();
#endif
#endif
#endif

		// キャッシュマネージャ
        if ( mFTCacheManager != NULL )
        {
		    FTC_Manager_Done( mFTCacheManager );
            mFTCacheManager = NULL;
        }

		// FreeType
        if ( mFTLibrary != NULL )
        {
	        FT_Done_FreeType( mFTLibrary );
            mFTLibrary = NULL;
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void FreeTypeFontManager::addFontFile( const lnChar* fontFilePath )
    {
		// 保存用フルパス
		//wchar_t fullPath[LN_MAX_PATH] = { 0 };
		//mFileManager->getFullPath( fullPath, fontFilePath );


        LRefPtr<FileIO::InFile> file( mFileManager->createInFile( fontFilePath ) );
        LRefPtr<Base::ReferenceBuffer> buffer( LN_NEW Base::ReferenceBuffer() );
        buffer->reserve( file->getSize() );
        file->read( buffer->getPointer(), file->getSize() );

		// Face 作成 (ファミリ名・Face 数を調べるため。すぐ削除する)
		FT_Face face;
        FT_Error err = FT_New_Memory_Face(
            mFTLibrary,
            (const FT_Byte*)buffer->getPointer(),
            buffer->getSize(),
            0, 
            &face );
        LN_THROW_InvalidOperation( err == FT_Err_Ok, "failed FT_New_Memory_Face : %d\n", err );

#if 1
		// Fase ひとつだけ (.ttf)
		if ( face->num_faces == 1 )
		{
			lnString familyName( face->family_name );
			lnU32 key = Base::Hash::calcHash( familyName.c_str() );
			if ( !Base::STLUtils::contains( mTTFDataEntryMap, key/*familyName*/ ) )
			{
				TTFDataEntry e;
				e.DataBuffer = buffer;
				e.DataBuffer->addRef();
				e.CollectionIndex = 0;
				mTTFDataEntryMap.insert( TTFDataEntryPair( key/*familyName*/, e ) );

				LN_LOG_WRITE( "Registered font file. [%s]", face->family_name );

				// 初回登録の場合はデフォルトフォント名として登録する
				if ( !mUpdatedDefaultFontName )
				{
					mDefaultFont->setName( familyName.c_str() );
					mUpdatedDefaultFontName = true;
				}
			}
			FT_Done_Face( face );

		}
		// Fase が複数 (.ttc)
		else if ( face->num_faces > 1 )
		{
			int facesCount = face->num_faces;
			FT_Done_Face( face );	// 数を調べるためだけに使った。一度削除
			face = NULL;

			for ( int i = 0; i < facesCount; i++ )
			{
				err = FT_New_Memory_Face( 
					mFTLibrary, 
					(const FT_Byte*)buffer->getPointer(), 
					buffer->getSize(),
					i, 
					&face );
				LN_THROW_InvalidOperation( err == FT_Err_Ok, "failed FT_New_Memory_Face : %d\n", err );

				lnString familyName( face->family_name );
				lnU32 key = Base::Hash::calcHash( familyName.c_str() );
				if ( !Base::STLUtils::contains( mTTFDataEntryMap, key/*familyName*/ ) )
				{
					TTFDataEntry e;
					e.DataBuffer = buffer;
					e.DataBuffer->addRef();
					e.CollectionIndex = i;
					mTTFDataEntryMap.insert( TTFDataEntryPair( key/*familyName*/, e ) );
				
					LN_LOG_WRITE( "Registered font file. [%s]", face->family_name );

					// 初回登録の場合はデフォルトフォント名として登録する
					if ( !mUpdatedDefaultFontName )
					{
						mDefaultFont->setName( familyName.c_str() );
						mUpdatedDefaultFontName = true;
					}
				}
				FT_Done_Face( face );
			}
		}
		else {
			FT_Done_Face( face );
			LN_THROW_InvalidOperation( 0 );
		}

		
#else 
#if 0
		// Fase ひとつだけ (.ttf)
		if ( face->num_faces == 1 )
		{
			TTFFileEntry e;
			e.FileFullPath = fullPath;
			e.CollectionIndex = 0;
			e.Buffer = buffer;
			buffer->addRef();
			mFontFileEntryMap.insert( FontFileEntryPair( lnString( face->family_name ), e ) );

			FT_Done_Face( face );
		}
		// Fase が複数 (.ttc)
		else if ( face->num_faces > 1 )
		{
			int facesCount = face->num_faces;
			FT_Done_Face( face );	// 数を調べるためだけに使った。一度削除
			face = NULL;

			for ( int i = 0; i < facesCount; i++ )
			{
				err = FT_New_Memory_Face( 
					mFTLibrary, 
					(const FT_Byte*)buffer->getPointer(), 
					buffer->getSize(),
					i, 
					&face );
				LN_THROW_InvalidOperation( err == FT_Err_Ok, "failed FT_New_Memory_Face : %d\n", err );

				TTFFileEntry e;
				e.FileFullPath = fullPath;
				e.CollectionIndex = i;
				mFontFileEntryMap.insert( FontFileEntryPair( lnString( face->family_name ), e ) );

				FT_Done_Face( face );
			}

		}
		else {
			FT_Done_Face( face );
			LN_THROW_InvalidOperation( 0 );
		}

#else

#if 0

		// Fase ひとつだけ (.ttf)
		if ( face->num_faces == 1 )
		{
			err = FT_Select_Charmap( face, FT_ENCODING_UNICODE );
			LN_THROW_InvalidOperation( err == FT_Err_Ok, "failed FT_Select_Charmap : %d\n", err );

			lnString family_name( face->family_name );
			mFontFaceMap.insert( FontFacePair( family_name, face ) );
		}
		// Fase が複数 (.ttc)
		else if ( face->num_faces > 1 )
		{
			int facesCount = face->num_faces;
			FT_Done_Face( face );	// 数を調べるためだけに使った。一度削除
			face = NULL;

			for ( int i = 0; i < facesCount; i++ )
			{
				err = FT_New_Memory_Face( 
					mFTLibrary, 
					(const FT_Byte*)buffer->getPointer(), 
					buffer->getSize(),
					i, 
					&face );
				LN_THROW_InvalidOperation( err == FT_Err_Ok, "failed FT_New_Memory_Face : %d\n", err );
				
		
				err = FT_Select_Charmap( face, FT_ENCODING_UNICODE );
				LN_THROW_InvalidOperation( err == FT_Err_Ok, "failed FT_Select_Charmap : %d\n", err );

				lnString family_name( face->family_name );
				mFontFaceMap.insert( FontFacePair( family_name, face ) );

				// 例) "MS ゴシック" は和名。face->family_name では "MS Gothic" が取れる
				//_p(family_name.c_str());
			}
		}
		else {
			LN_THROW_InvalidOperation( 0 )
		}



#else

        // ファミリ名を取得するため、一度生成する
        //FT_Face face;
        //FT_Error err = FT_New_Memory_Face(
        //    mFTLibrary,
        //    (const FT_Byte*)buffer->getPointer(),
        //    buffer->getSize(),
        //    0, 
        //    &face );
        //LN_THROW_InvalidOperation( err == FT_Err_Ok, "failed FT_New_Memory_Face : %d\n", err );

		int facesCount = face->num_faces;
		if ( facesCount > 1 )
		{
			for ( int i = 0; i < facesCount; i++ )
			{
			}
			//printf( "\n----- Face number: %d -----\n\n", i );
		}
		else
		{

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
#endif
#endif
#endif
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Font* FreeTypeFontManager::createFont()
    {
        //FreeTypeFont* font = LN_NEW FreeTypeFont( this );
        //font->initialize("", 0, LColorF(), false , false);
        //return font;
		return mDefaultFont->copy();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Font* FreeTypeFontManager::createBitmapFont(Texture* texture)
	{
		LRefPtr<BitmapFont> font(LN_NEW BitmapFont());
		font->create(this, texture);
		font.addRef();
		return font;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    FT_Error FreeTypeFontManager::faceRequester(
		FTC_FaceID face_id,
		FT_Library library,
		FT_Pointer request_data,
		FT_Face* aface )
    {
		// キャッシュ検索のコールバック。map から取るようにしている。

		// face_id は、FTC_Manager_LookupFace() に渡した ID が入ってくる。
		// ID は自分で好きなように決められる。とりあえず文字列 (ファミリ名)としている

#if 1
		//lnString family_name( (lnChar*)face_id );
		lnU32 key = (lnU32)face_id;
        TTFDataEntryMap::iterator itr = mTTFDataEntryMap.find( key/*family_name*/ );
        if ( itr != mTTFDataEntryMap.end() )
        {
			FT_Face face;
            FT_Error err = FT_New_Memory_Face(
                mFTLibrary,
				(const FT_Byte*)itr->second.DataBuffer->getPointer(),
                itr->second.DataBuffer->getSize(),
				itr->second.CollectionIndex, 
                &face );
            LN_THROW_InvalidOperation( err == FT_Err_Ok, "failed FT_New_Memory_Face : %d\n", err );

            err = FT_Select_Charmap( face, FT_ENCODING_UNICODE );
            if ( err != FT_Err_Ok )
            {
                FT_Done_Face( face );
                LN_THROW_InvalidOperation( err == FT_Err_Ok, "failed FT_Select_Charmap : %d\n", err );
            }

            *aface = face;
            return FT_Err_Ok;
		}
#ifdef LNOTE_WIN32
		else if ( mRequesterFaceName != NULL ) 
		{
			// 名前からシステムフォント検索
			TSystemFontData* systemFont = getWindowsSystemFontData( mRequesterFaceName );
			mRequesterFaceName = NULL;
			if(systemFont == NULL){
				return FT_Err_Cannot_Open_Resource;
			}

			// リソースロック
			size_t size = 0;
			int index = 0;
			lnByte* data = lockWindowsSystemFontData( systemFont, &size, &index );
			if(data == NULL){
				freeWindowsSystemFontData( systemFont );
				return FT_Err_Cannot_Open_Resource;
			}

			// FreeType の読み取りストリーム
			FT_Stream stream = (FT_Stream)malloc(sizeof(FT_StreamRec));
			if(stream == NULL){
				freeWindowsSystemFontData(systemFont);
				return FT_Err_Out_Of_Memory;
			}
			memset(stream, 0, sizeof(FT_StreamRec));
			stream->base = 0;
			stream->size = size;
			stream->pos = 0;
			stream->descriptor.pointer = systemFont;
			stream->pathname.pointer = NULL;
			stream->read = StreamIoFunc;
			stream->close = StreamCloseFunc;

			FT_Open_Args args;
			memset(&args, 0, sizeof(args));
			args.flags = FT_OPEN_STREAM;
			args.stream = stream;
			args.driver = 0;
			args.num_params = 0;
			args.params = NULL;
			
			FT_Face face;
			FT_Error err = FT_Open_Face( mFTLibrary, &args, index, &face );
			if ( err != FT_Err_Ok ) {
				// 失敗した場合もstreamは勝手に解放される
				return err;
			}

			// Charmapを設定しておく
			err = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
			if ( err != FT_Err_Ok ) {
				FT_Done_Face(face);
				return err;
			}

			*aface = (FT_Face)face;
            return FT_Err_Ok;
		}
#endif
		return -1;

#else
#if 0
		lnString family_name( (lnChar*)face_id );
        FontFileEntryMap::iterator itr = mFontFileEntryMap.find( family_name );
        if ( itr != mFontFileEntryMap.end() )
        {
			TTFFileEntry& e = itr->second;

			// ファイルを開いてデータ読み込み
			LRefPtr<FileIO::InFile> file( mFileManager->createInFile( e.FileFullPath.c_str() ) );
			file->load();
			
			// Face 作成
			FT_Face face;
			FT_Error err = FT_New_Memory_Face(
				mFTLibrary,
				(const FT_Byte*)file->getData(),
				file->getSize(),
				0, 
				&face );
			LN_THROW_InvalidOperation( err == FT_Err_Ok, "failed FT_New_Memory_Face : %d\n", err );

			// 文字コードマップ
			err = FT_Select_Charmap( face, FT_ENCODING_UNICODE );
            if ( err != FT_Err_Ok )
            {
                FT_Done_Face( face );
                LN_THROW_InvalidOperation( err == FT_Err_Ok, "failed FT_Select_Charmap : %d\n", err );
            }

            *aface = face;
            return FT_Err_Ok;
		}
		return -1;

#else

#if 0
		lnString family_name( (lnChar*)face_id );
        FontFaceMap::iterator itr = mFontFaceMap.find( family_name );
        if ( itr != mFontFaceMap.end() )
        {
			
			*aface = itr->second;
            return FT_Err_Ok;
		}
#else
        lnString family_name( (lnChar*)face_id );
        FontFileDataMap::iterator itr = mFontFileDataMap.find( family_name );
        if ( itr != mFontFileDataMap.end() )
        {
			LRefPtr<Base::ReferenceBuffer> buffer( LN_NEW Base::ReferenceBuffer() );
			buffer->reserve( itr->second->getPointer(), itr->second->getSize() );

			FT_Byte* tream = LN_NEW lnByte[itr->second->getSize()];//(FT_Byte*)malloc(itr->second->getSize());
			memcpy( tream, itr->second->getPointer(), itr->second->getSize() );

            FT_Face face;
            FT_Error err = FT_New_Memory_Face(
                mFTLibrary,
                (const FT_Byte*)tream,
                itr->second->getSize(),
                0, 
                &face );
            LN_THROW_InvalidOperation( err == FT_Err_Ok, "failed FT_New_Memory_Face : %d\n", err );

			delete[] tream;

            err = FT_Select_Charmap( face, FT_ENCODING_UNICODE );
            if ( err != FT_Err_Ok )
            {
                FT_Done_Face( face );
                LN_THROW_InvalidOperation( err == FT_Err_Ok, "failed FT_Select_Charmap : %d\n", err );
            }

            *aface = face;
            return FT_Err_Ok;
        }
#endif
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
            LN_THROW_InvalidOperation( err == FT_Err_Ok, "failed FT_Open_Face : %d\n", err );
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
            //LN_THROW_InvalidOperation( err == FT_Err_Ok, "failed FT_New_Face : %d\n", err );
            
            err = FT_Select_Charmap( face, FT_ENCODING_UNICODE );
            if ( err != FT_Err_Ok )
            {
                FT_Done_Face(face);
                LN_THROW_InvalidOperation( err == FT_Err_Ok, "failed FT_Select_Charmap : %d\n", err );
            }



            *aface = face;//itr->second;
            return FT_Err_Ok;
        }
        
        return -1;
#endif
#endif
#endif
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    FT_Error FreeTypeFontManager::callbackFaceRequester(
		FTC_FaceID face_id,
		FT_Library library,
		FT_Pointer request_data,
		FT_Face* aface )
    {
        return ((FreeTypeFontManager*)request_data)->faceRequester( face_id, library, request_data, aface );
    }

	
#ifdef LNOTE_WIN32
	// http://kikyou.info/diary/?200510#i10 を参考にした
	#define TVP_TT_TABLE_ttcf  (('t' << 0) + ('t' << 8) + ('c' << 16) + ('f' << 24))
	#define TVP_TT_TABLE_name  (('n' << 0) + ('a' << 8) + ('m' << 16) + ('e' << 24))

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	FreeTypeFontManager::TSystemFontData* FreeTypeFontManager::getWindowsSystemFontData(LPCTSTR name)
	{
		DWORD result;
		TSystemFontData *p = (TSystemFontData*)malloc(sizeof(TSystemFontData));
		if(p == NULL){
			return NULL;
		}
		p->fontdata = NULL;
		p->index = 0;
		p->hdc = GetDC(NULL);
		if(p->hdc == NULL){
			free(p);
			return NULL;
		}
		// 名前以外適当
		p->hFont = CreateFont(
			12, 0, 0, 0, FW_NORMAL,
			FALSE, FALSE, FALSE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			name);
		if(p->hFont == NULL){
			ReleaseDC(NULL, p->hdc);
			free(p);
			return NULL;
		}
		p->hOldFont = (HFONT)::SelectObject(p->hdc, p->hFont);
		// フォントデータが得られそうかチェック
		result = ::GetFontData(p->hdc, TVP_TT_TABLE_name, 0, NULL, 0);
		if(result == GDI_ERROR){
			SelectObject(p->hdc, p->hOldFont);
			DeleteObject(p->hFont);
			ReleaseDC(NULL, p->hdc);
			free(p);
			return NULL;
		}
		return p;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	unsigned char* FreeTypeFontManager::lockWindowsSystemFontData(TSystemFontData *fnt, size_t *size, int *index)
	{
		unsigned char *name_content    = NULL; // Windows から取得した name タグの内容
		unsigned char *name_content_ft = NULL; // FreeType から取得した name タグの内容
		DWORD result;

		DWORD fontsize;
		BOOL IsTTC = FALSE;
		unsigned char buf[4];
		int name_content_size;

		FT_Face face = NULL;

		int i;

		if(fnt == NULL || index == NULL){
			return NULL;
		}
		if(fnt->fontdata != NULL){
			*size = fnt->size;
			*index = fnt->index;
			return fnt->fontdata;
		}
		// フォント名称取得処理
		result = GetFontData(fnt->hdc, TVP_TT_TABLE_name, 0, NULL, 0);
		if(result == GDI_ERROR){
			// エラー; GetFontData では扱えなかった
			return NULL;
		}
		name_content_size = result;
		name_content = (unsigned char*)malloc(name_content_size);
		name_content_ft = (unsigned char*)malloc(name_content_size);
		GetFontData(fnt->hdc, TVP_TT_TABLE_name, 0, name_content, name_content_size);

		// フォントサイズ取得処理
		result = GetFontData(fnt->hdc, TVP_TT_TABLE_ttcf, 0, &buf, 1);
		if(result == 1){
			// TTC ファイルだと思われる
			result = GetFontData(fnt->hdc, TVP_TT_TABLE_ttcf, 0, NULL, 0);
			IsTTC = TRUE;
		}
		else{
			result = GetFontData(fnt->hdc, 0, 0, NULL, 0);
			fnt->index = 0;
		}
		if(result == GDI_ERROR){
			// エラー; GetFontData では扱えなかった
			free(name_content);
			free(name_content_ft);
			return NULL;
		}
		fontsize = result;
		fnt->size = fontsize;
		fnt->fontdata = (unsigned char*)malloc(fontsize);
		if(fnt->fontdata == NULL){
			free(name_content);
			free(name_content_ft);
			return NULL;
		}
		if(IsTTC){
			GetFontData(fnt->hdc, TVP_TT_TABLE_ttcf, 0, fnt->fontdata, fontsize);
		}
		else{
			GetFontData(fnt->hdc, 0, 0, fnt->fontdata, fontsize);
		}
		if(!IsTTC){
			*size = fnt->size;
			*index = fnt->index;
			free(name_content);
			free(name_content_ft);
			return fnt->fontdata;
		}
		i = 0;
		while(1){
			FT_ULong length;
			FT_Error err;
			if(FT_New_Memory_Face(
				mFTLibrary,
				fnt->fontdata,
				fnt->size,
				i,
				&face)){
				free(name_content);
				free(name_content_ft);
				return NULL;
			}
			// FreeType から、name タグのサイズを取得する
			length = 0;
			err = FT_Load_Sfnt_Table(face, TTAG_name, 0, NULL, &length);
			if(err){
				FT_Done_Face(face);
				free(name_content);
				free(name_content_ft);
				return NULL;
			}
			// FreeType から得た name タグの長さを Windows から得た長さと比較
			if(length == (DWORD)name_content_size){
				// FreeType から name タグを取得
				err = FT_Load_Sfnt_Table(face, TTAG_name, 0, name_content_ft, &length);
				if(err){
					FT_Done_Face(face);
					free(name_content);
					free(name_content_ft);
					return NULL;
				}
				// FreeType から読み込んだ name タグの内容と、Windows から読み込んだ
				// name タグの内容を比較する。
				// 一致していればその index のフォントを使う。
				if(!memcmp(name_content, name_content_ft, name_content_size)){
					FT_Done_Face(face);
					fnt->index = i;
					break;
				}
			}
			FT_Done_Face(face);
			i++;
		}

		*size = fnt->size;
		*index = fnt->index;
		free(name_content);
		free(name_content_ft);
		return fnt->fontdata;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void FreeTypeFontManager::freeWindowsSystemFontData(TSystemFontData *fnt)
	{
		if(fnt == NULL){
			return;
		}
		if(fnt->fontdata != NULL){
			free(fnt->fontdata);
		}
		SelectObject(fnt->hdc, fnt->hOldFont);
		DeleteObject(fnt->hFont);
		ReleaseDC(NULL, fnt->hdc);
		free(fnt);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	unsigned long FreeTypeFontManager::StreamIoFunc(
		FT_Stream stream,
		unsigned long offset,
		unsigned char* buffer,
		unsigned long count)
	{
		if(count != 0){
			TSystemFontData* font = (TSystemFontData*)stream->descriptor.pointer;
			unsigned char* p;
			unsigned long copycount;
			if(font == NULL || font->fontdata == NULL)
				return 0;
			p = font->fontdata;
			if(offset > stream->size)
				return 0;	// error
			copycount = offset + count > stream->size ?
					stream->size - offset :
					count;
			p += offset;
			if(buffer != NULL)
				memcpy(buffer, p, copycount);
			return copycount;
		}
		return 0;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void FreeTypeFontManager::StreamCloseFunc(FT_Stream stream)
	{
		freeWindowsSystemFontData((TSystemFontData*)stream->descriptor.pointer);
		free(stream);
	}
#endif

} // namespace Graphics
} // namespace Core
} // namespace LNote
