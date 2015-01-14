﻿//==============================================================================
// LFInternal 
//------------------------------------------------------------------------------
///**
//  @file       LFInternal.h
//  @brief      LFInternal
//  @author     Riki
//*/
//==============================================================================

#include "../Core/BuildSetting.h"
#include "../LNote.hpp"
#include "FuncLibManager.h"

using namespace LNote;
using namespace LNote::Core;

//------------------------------------------------------------------------------

// bool → LNBool
#define LNOTE_BOOL_TO_LNBOOL( x )	( x ) ? LN_TRUE : LN_FALSE

// ポインタの int キャスト
#define LN_TO_INT( ptr )			reinterpret_cast<intptr_t>( ptr )
#define LN_INT_TO( type, value )	reinterpret_cast<type>( value )

// x_ y_ に Geometry::Size に代入
#define LNFUNC_LSize_TO( v_, w_, h_ ) \
        const Geometry::Size& v = ( v_ ); \
        w_ = v.w; \
        h_ = v.h;

// xyz に Vector3 に代入
#define LNFUNC_SUBSTITUTE_VECTOR3_TO_XYZ( vec3_, x_, y_, z_ ) \
        const LVector3& v3 = ( vec3_ ); \
        *x_ = v3.X; \
        *y_ = v3.Y; \
        *z_ = v3.Z;

// LNVector に Vector3 を代入
#define LNFUNC_SUBSTITUTE_VECTOR3_TO_LNVector( vec3, outVec ) \
        const LVector3& v3 = ( vec3 ); \
        outVec->X = v3.X; \
        outVec->Y = v3.Y; \
        outVec->Z = v3.Z;

// LNMatrix に LMatrix を代入
#define LNFUNC_SUBSTITUTE_Matrix_To_LNMatrix( mat, outMat ) \
		memcpy( outMat, &mat, sizeof(LNMatrix) );
       
// LNColor32 に LColorF を代入
#define LN_SET_CoreColor32_TO_LNColor32( color, outColor ) \
        const LColor& c = ( color ); \
        outColor->R = c.R; \
        outColor->G = c.G; \
        outColor->B = c.B; \
        outColor->A = c.A;

// LNColorF に LColorF を代入
#define LN_SET_CoreColorF_TO_LNColorF( color, outColor ) \
        const LColorF& c = ( color ); \
        outColor->R = c.red; \
        outColor->G = c.green; \
        outColor->B = c.blue; \
        outColor->A = c.alpha;

// FuncLibManager 簡略化
#define FuncLibManager ::LNote::Core::Function::Manager

// 引数チェック
#define LN_CHECK_ARG_HANDLE(h)	if (h == NULL) { return ::LN_ERR_ARGUMENT; }	// TODO 中身もチェック
#define LN_CHECK_ARG(exp)	if (!(exp)) { return ::LN_ERR_ARGUMENT; }

//------------------------------------------------------------------------------
// 関数ライブラリ内で共通のもの

namespace LNote
{
namespace Core
{
namespace Function
{
    extern Core::Manager* gFramework;
    extern Graphics::IGraphicsDevice* gGraphicsDevice;
    extern Graphics::IRenderer* gRenderer;
}
}
}

//---------------------------------------------------------------------
#define TO_HANDLE( v )		static_cast<LNHandle>( v )

#define TO_INDEX( h )		static_cast<LNHandle>( h )

//#define TO_OBJECT( h )		FuncLibManager::getObject( static_cast<int>( h ) )
//#define TO_INTERFACE( h )	FuncLibManager::getInterface( static_cast<int>( h ) )

#define TO_IN_FILE( h )		static_cast<FileIO::InFile*>( FuncLibManager::getReferenceObject( static_cast<int>( h ) ) )
#define TO_OUT_FILE( h )	static_cast<FileIO::OutFile*>( FuncLibManager::getReferenceObject( static_cast<int>( h ) ) )

//#define TO_TEXTURE( h )		static_cast<Core::Graphics::ITexture*>( FuncLibManager::getInterface( static_cast<int>( h ) ) )
//#define TO_FONT( h )		static_cast<Core::Graphics::IFont*>( FuncLibManager::getInterface( static_cast<int>( h ) ) )

#define TO_SOUND( h )		static_cast<Core::Audio::Sound*>( FuncLibManager::getReferenceObject( static_cast<int>( h ) ) )

#define TO_SCENENODE( h )	static_cast<Core::Scene::SceneNode*>( FuncLibManager::getReferenceObject( static_cast<int>( h ) ) )
#define TO_VISUAL_NODE( h )	static_cast<Core::Scene::VisualNode*>( FuncLibManager::getReferenceObject( static_cast<int>( h ) ) )
#define TO_SPRITE( h )		static_cast<Core::Scene::Sprite*>( FuncLibManager::getReferenceObject( static_cast<int>( h ) ) )
#define TO_SPRITE_PATH( h )	static_cast<Core::Scene::SpritePath*>( FuncLibManager::getReferenceObject( static_cast<int>( h ) ) )
#define TO_EFFECT( h )		static_cast<Core::Scene::Effect*>( FuncLibManager::getReferenceObject( static_cast<int>( h ) ) )
#define TO_SHADER( h )		static_cast<Core::Scene::SceneShader*>( FuncLibManager::getReferenceObject( static_cast<int>( h ) ) )
#define TO_VIEW_PANE( h )	static_cast<Core::Scene::Pane*>( FuncLibManager::getReferenceObject( static_cast<int>( h ) ) )
#define TO_CAMERA( h )		static_cast<Core::Scene::Camera*>( FuncLibManager::getReferenceObject( static_cast<int>( h ) ) )
//↓今後コレに変更
#define TO_REFOBJ( type, h )	static_cast<type*>( FuncLibManager::getReferenceObject( static_cast<int>( h ) ) )
//#define TO_INTERFACE( type, h )	static_cast<type*>( FuncLibManager::getReferenceObject( static_cast<int>( h ) ) )

#define TO_CORE_VEC3_PTR( v )	reinterpret_cast<LNote::LVector3*>( v )
#define TO_CORE_VEC3_REF( v )	reinterpret_cast<const LNote::LVector3&>( *v )
#define TO_CORE_VEC2_PTR( v )	reinterpret_cast<LNote::LVector2*>( v )
#define TO_CORE_VEC2_REF( v )	reinterpret_cast<const LNote::LVector2&>( *v )
#define TO_CORE_MAT_PTR( v )	reinterpret_cast<LNote::LMatrix*>( v )
#define TO_CORE_MAT_REF( v )	reinterpret_cast<const LNote::LMatrix&>( *v )
#define TO_CORE_QUA_PTR( v )	reinterpret_cast<LNote::LQuaternion*>( v )
#define TO_CORE_QUA_REF( v )	reinterpret_cast<const LNote::LQuaternion&>( *v )

#define TO_CORE_COLOR32( v )	*((LColor*)v)
#define TO_CORE_COLOR( v )		*((LColorF*)v)
#define TO_CORE_TONE( v )		*((LTone*)v)
#define TO_CORE_RECT( v )		*((LRect*)v)//LRect( v->X, v->Y, v->Width, v->Height )
#define TO_CORE_RECTF( v )		LRectF( static_cast<float>(v->X), static_cast<float>(v->Y), static_cast<float>(v->Width), static_cast<float>(v->Height) )

#define TO_LN_COLOR( v )		(const LNColor*)&(v)

//---------------------------------------------------------------------
/// try 空間の開始
#define LN_FUNC_TRY_BEGIN	try{

/// try 空間の終了
#define LN_FUNC_TRY_END		} \
    catch (LNote::Core::Base::Exception& e) { \
		if ( LNote::Core::Function::Manager::ExceptionCallback ) { \
			LNote::Core::Function::Manager::ExceptionCallback( (LNErrorCode)e.getErrorCode() ); \
				}  \
				else { \
			LNote::Core::Function::Manager::procException( &e ); \
		} \
	}

/// try 空間の終了
#define LN_FUNC_TRY_END_RETURN		} \
    catch (LNote::Core::Base::Exception& e) { \
		if ( LNote::Core::Function::Manager::ExceptionCallback ) { \
			LNote::Core::Function::Manager::ExceptionCallback( (LNErrorCode)e.getErrorCode() ); \
						}  \
								else { \
			LNote::Core::Function::Manager::procException( &e ); \
		} \
		return (LNErrorCode)e.getErrorCode(); \
	} \
	return ::LN_OK;

extern bool gEnableWrappedException;

void checkCommonDefinition();

//---------------------------------------------------------------------
// 言語バインダ用にエクスポートするが、BinderMaker で自動宣言しないもの

extern "C"
{

// HSP by DLL 内部用。WIN32_MESSAGE_FOR_HSP_EXCEPTION を send する
LNOTEAPI void _LException_SendToWin32Message(LNErrorCode errCode);

LNOTEAPI void LException_SetEnableWrappedException(LNBool f);

//LNOTEAPI int LHSPInternal_GetIntPtrStringLength(int string);
//LNOTEAPI void LHSPInternal_GetIntPtrString(int string, char* buf);


//==============================================================================
// LArchiveMaker
//------------------------------------------------------------------------------
///**
//  @defgroup   group_archive_maker ArchiveMaker
//  @brief      暗号化アーカイブの作成
//  @{
//*/
//==============================================================================

//----------------------------------------------------------------------
///**
//  @brief      新規にファイルを開いてアーカイブ作成の準備をする
//  @param[in]  filePath	: ファイル名 
//  @param[in]  password	: 暗号化に使う文字列 (127文字まで)
//  @return     LN_TRUE = 存在する / LN_FALSE = 存在しない
//*/
//----------------------------------------------------------------------
LNOTEAPI LNBool LArchiveMaker_Open(const TCHAR* filePath, const char* password);

//----------------------------------------------------------------------
///**
//  @brief		ファイルを追加する
//  @param[in]  filePath    : 追加するファイルの名前
//  @param[in]  accessName	: プログラムからアクセスする時の名前
//  @return     LN_TRUE = 存在する / LN_FALSE = 存在しない
//  @details   filePath のファイルをアーカイブに追加します。<br>
//              accessName を指定した場合、ファイルに別名をつけた上で追加します。
//              LInFile_Create によってアーカイブ内のファイルにアクセスする場合、
//              この名前でアクセスするようにします。<br>
//              accessName が NULL の場合は filePath がそのまま使われます。
//*/
//----------------------------------------------------------------------
LNOTEAPI LNBool LArchiveMaker_AddFile(const wchar_t* filePath, const wchar_t* accessName);

//----------------------------------------------------------------------
///**
//  @brief		アーカイブファイルを閉じる
//*/
//----------------------------------------------------------------------
LNOTEAPI void LArchiveMaker_Close();

/** @} */	// defgroup LArchiveMaker

} // extern "C"

