//==============================================================================
// DX9IndexBuffer 
//==============================================================================

#include "stdafx.h"
#include "DX9GraphicsDevice.h"
#include "DX9IndexBuffer.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ DX9IndexBuffer
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9IndexBuffer::DX9IndexBuffer()
        : mGraphicsDevice   ( NULL )
        , mIndexBuffer      ( NULL )
        , mIndices          ( NULL )
        , mIndexCount       ( 0 )
        , mIndexStride      ( 0 )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9IndexBuffer::~DX9IndexBuffer()
    {
		SAFE_DELETE_ARRAY( mIndices );
		SAFE_RELEASE( mIndexBuffer );
        LN_SAFE_RELEASE( mGraphicsDevice );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9IndexBuffer::create( DX9GraphicsDevice* device, int indexCount, const void* data, bool isDynamic, bool is16Bits )
	{
		HRESULT hr;

		LN_REFOBJ_SET( mGraphicsDevice, device );
		mIndexCount		= indexCount;
        mIndexStride	= ( is16Bits ) ? 2 : 4;

        // dynamic
		if ( isDynamic )
		{
            if ( is16Bits ) {
			    mIndices = LN_NEW lnU16[mIndexCount];
            }
            else {
                mIndices = LN_NEW lnU32[mIndexCount];
            }
		}
		// static
		else
		{
            LN_COMCALL(
				mGraphicsDevice->getIDirect3DDevice9()->CreateIndexBuffer( 
				    mIndexStride * mIndexCount,
				    0,
                    ( is16Bits ) ? D3DFMT_INDEX16 : D3DFMT_INDEX32,
				    D3DPOOL_MANAGED,
				    &mIndexBuffer,
				    NULL ) );
		}

		// data コピー
		if ( data ) {
			if ( isDynamic ) {
				memcpy( mIndices, data, mIndexStride * mIndexCount );
			}
			else {
				void* buf = NULL;
				LN_COMCALL( mIndexBuffer->Lock( 0, 0, &buf, D3DLOCK_DISCARD ) );
				memcpy( buf, data, mIndexStride * mIndexCount );
				LN_COMCALL( mIndexBuffer->Unlock() );
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void* DX9IndexBuffer::lock()
    {
		if ( mIndexBuffer )
		{
			HRESULT hr;
			void* r = NULL;
			LN_COMCALL( mIndexBuffer->Lock( 0, 0, &r, 0 ) );
			return r;
		}
		else 
		{
			return mIndices;
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9IndexBuffer::unlock()
    {
        if ( mIndexBuffer ) {
			HRESULT hr;
			LN_COMCALL( mIndexBuffer->Unlock() );
		}
    }

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================