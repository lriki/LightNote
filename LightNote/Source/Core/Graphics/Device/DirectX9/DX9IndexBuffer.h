//==============================================================================
// DX9IndexBuffer 
//------------------------------------------------------------------------------
///**
//  @file       DX9IndexBuffer.h
//  @brief      DX9IndexBuffer
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
// ■ DX9IndexBuffer
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class DX9IndexBuffer
    : /*public Base::ReferenceObject
    , */public IIndexBuffer
{
public:
	DX9IndexBuffer();
	virtual ~DX9IndexBuffer();
	LN_REFOBJ_METHODS;

public:

	/// 作成
    void create( DX9GraphicsDevice* device, int indexCount, const void* data, bool isDynamic, bool is16Bits );

	/// IDirect3DIndexBuffer9 へのポインタ取得
	IDirect3DIndexBuffer9* getDxIndexBuffer() const { return mIndexBuffer; }

public:
	virtual lnU32 getIndexCount() const { return mIndexCount; }
    virtual lnU32 getIndexStride() const { return mIndexStride; }
	virtual bool isDynamic() const { return ( !mIndexBuffer ); }
	virtual void* lock();
	virtual void unlock();

private:
	DX9GraphicsDevice*		mGraphicsDevice;
	IDirect3DIndexBuffer9*	mIndexBuffer;	    ///< IDirect3DIndexBuffer9 へのポインタ
	void*	                mIndices;			///< 動的なインデックスを作る場合はここに作る
	lnU32					mIndexCount;
	lnU32					mIndexStride;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================