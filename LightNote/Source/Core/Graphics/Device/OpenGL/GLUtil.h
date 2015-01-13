//==============================================================================
// GLUtil 
//------------------------------------------------------------------------------
///**
//  @file       GLUtil.h
//  @brief      GLUtil
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <glfw/include/GLFW/glfw3.h>
#include "../../../Threading/EventFlag.h"
#include "../../Interface.h"
#include "GLPlatformContext.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{


	
///// (とりあえず GLSL 用。これらの値を IShader::getVariableByDeclUsage() に渡して attribute 変数を取得する)
//enum VertexDeclUsage
//{
//	VertexDeclUsage_Unknown = 0,
//
//	VertexDeclUsage_Position,		///< 頂点位置
//	VertexDeclUsage_BlendWeight,	///< ブレンドウェイト値
//	VertexDeclUsage_BlendIndex,		///< ブレンドインデックス値
//	VertexDeclUsage_,///< 法線
//	VertexDeclUsage_,///< UV
//	VertexDeclUsage_,///< 追加UV1
//	VertexDeclUsage_,///< 追加UV2
//	VertexDeclUsage_,///< 追加UV3
//	VertexDeclUsage_,///< 追加UV4
//	VertexDeclUsage_,///< Sdef - C値
//	VertexDeclUsage_,///< Sdef - R0値
//	VertexDeclUsage_,///< Sdef - R1値
//	VertexDeclUsage_,///< エッジウェイト
//	VertexDeclUsage_,///< 頂点インデックス値
//	VertexDeclUsage_,///< 頂点カラー
//	VertexDeclUsage_,
//
//    LN_VERTEXDECLUSAGE_UNKNOWN = 0,
//
//    LN_VERTEXDECLUSAGE_POSITION,        ///< 頂点位置
//    LN_VERTEXDECLUSAGE_BLENDWEIGHT,
//    LN_VERTEXDECLUSAGE_BLENDINDICES,
//    LN_VERTEXDECLUSAGE_NORMAL,          
//    LN_VERTEXDECLUSAGE_TEXCOORD0,       
//    LN_VERTEXDECLUSAGE_TEXCOORD1,       
//    LN_VERTEXDECLUSAGE_TEXCOORD2,       ///< 追加UV2
//    LN_VERTEXDECLUSAGE_TEXCOORD3,       ///< 追加UV3
//    LN_VERTEXDECLUSAGE_TEXCOORD4,       
//    LN_VERTEXDECLUSAGE_TEXCOORD5,       ///< Sdef - C値
//    LN_VERTEXDECLUSAGE_TEXCOORD6,       ///< Sdef - R0値
//    LN_VERTEXDECLUSAGE_TEXCOORD7,       
//    LN_VERTEXDECLUSAGE_TEXCOORD8,       
//    LN_VERTEXDECLUSAGE_PSIZE15,         
//
//    LN_VERTEXDECLUSAGE_COLOR,           
//
//    LN_VERTEXDECLUSAGE_MAX,
//};
//
/// glVertexAttribPointer() の第2引数以降のまとめ
struct LNGLVertexAttribPointerData
{
	//LNVertexDeclUsage   Usage;          ///< IShader::getVariableByDeclUsage() に渡して attribute 変数を取得する
	LNDeclUsage			Usage;
	int					UsageIndex;

    GLint               Size;
    GLenum              Type;       ///< (終端は0)
    GLboolean           Normalized;
    GLsizei             Stride;
    lnU8                ByteOffset;
};




//==============================================================================
// ■ Util クラス
//------------------------------------------------------------------------------
///**
//  @brief     
//*/
//==============================================================================
class Utils
{
public:

	/// LNVertexElemenst 配列から glVertexAttribPointer() に設定する情報を作成する (要 delete[]。終端要素は Size == -1)
    static LNGLVertexAttribPointerData* createVertexAttribData( const LNVertexElemenst* elements_ );

	/// LNDeclType -> GLenum, size(要素数 LN_DECL_FLOAT3 なら 3), normalized(正規化の有無)
    static void convertDeclTypeLNToGL( LNDeclType ln_type_, GLenum* gl_type_, GLint* size_, GLboolean* normalized_ );

	/// LNDeclUsage, index -> LNVertexDeclUsage
	//static LNVertexDeclUsage convertDeclUsage( LNDeclUsage usage_, lnU32 index_ );

	/// シェーダ変数の型変換
	static void convertShaderVariableTypeGLToLN( GLenum glType, GLsizei glVarSize, LNShaderVariableTypeDesc* lnDesc, ShaderVariableType* lnType );
};

namespace OpenGL
{
//==============================================================================
// ■ GLResource クラス
//------------------------------------------------------------------------------
///**
//  @brief     
//
//  @note
//              このクラスのrequestCommitGLResource() 以外の関数は
//              すべて描画スレッドから呼び出すこと。
//*/
//==============================================================================
class GLResource
    : public Base::ReferenceObject
{
public:

    GLResource()
        : mInitialized(false)
    {}

    virtual ~GLResource()
    {
        // ダメ。サブクラスで呼ぶこと。
        //onDisposeGLResource();
    }

	/// (called from GraphicsDevice::commitChangesGLResource())
    void commitChanges()
    {
        if (!mInitialized.isTrue())
        {
            onCommitGLResource();
            mInitialized.setTrue();
        }
    }

    void onLostDevice() { onDisposeGLResource(); }

    void onResetDevice() { mInitialized.setFalse(); }

    void requestCommitGLResource() { mInitialized.setFalse(); }

protected:

	/// GLリソース作成 (Renderer::beginScene() からいろいろ経由で呼ばれる)
    virtual void onCommitGLResource() = 0;

	/// GLリソース解放
    virtual void onDisposeGLResource() = 0;

protected:

    Threading::EventFlag mInitialized;
};


} // namespace OpenGL
} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================