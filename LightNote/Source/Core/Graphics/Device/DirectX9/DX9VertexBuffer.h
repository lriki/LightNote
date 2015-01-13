//==============================================================================
// DX9VertexBuffer 
//------------------------------------------------------------------------------
///**
//  @file       DX9VertexBuffer.h
//  @brief      DX9VertexBuffer
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../Interface.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ DX9VertexBuffer
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class DX9VertexBuffer
    : /*public Base::ReferenceObject
    , */public IVertexBuffer
{
public:
	DX9VertexBuffer();
	virtual ~DX9VertexBuffer();
	LN_REFOBJ_METHODS;

public:

	/// 作成
    void create( DX9GraphicsDevice* device, LNVertexElemenst* elements, lnU32 vertexCount, const void* data, bool isDynamic );

	/// 作成 (渡された IDirect3DVertexBuffer9 と D3DVERTEXELEMENT9 を参照する)
    void create( DX9GraphicsDevice* device, IDirect3DVertexBuffer9* device_obj, D3DVERTEXELEMENT9* layout );

	/// IDirect3DVertexBuffer9 へのポインタ取得
	IDirect3DVertexBuffer9* getDxVertexBuffer() { return mVertexBuffer; }

	/// IDirect3DVertexDeclaration9 へのポインタ取得
	IDirect3DVertexDeclaration9* getDxVertexDeclaration() { return mVertexDecl; }

public:
	virtual lnU32 getVertexCount() const { return mVertexCount; }
	virtual lnU32 getVertexStride() const { return mVertexStride; }
	virtual bool isDynamic() const { return ( !mVertexBuffer ); }
	virtual void* lock();
	virtual void unlock();

private:
	DX9GraphicsDevice*				mGraphicsDevice;
    IDirect3DVertexBuffer9*	        mVertexBuffer;	    ///< 頂点バッファ
	lnByte*					        mVertices;		    ///< 動的な頂点バッファとして作った場合はこっちに配列が作られる ( union でもいいかな・・・ )
	IDirect3DVertexDeclaration9*	mVertexDecl;	    ///< 頂点宣言
	lnU32							mVertexCount;		///< 頂点の数
	lnU32							mVertexStride;	    ///< 頂点ひとつ分のサイズ
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================