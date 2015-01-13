//==============================================================================
// DX9VertexBuffer 
//==============================================================================

#include "stdafx.h"
#include "DX9Module.h"
#include "DX9GraphicsDevice.h"
#include "DX9VertexBuffer.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ DX9VertexBuffer
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9VertexBuffer::DX9VertexBuffer()
        : mGraphicsDevice   ( NULL )
        , mVertexBuffer     ( NULL )
        , mVertices         ( NULL )
        , mVertexDecl       ( NULL )
        , mVertexCount      ( 0 )
        , mVertexStride     ( 0 )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9VertexBuffer::~DX9VertexBuffer()
    {
		SAFE_DELETE_ARRAY( mVertices );
		SAFE_RELEASE( mVertexBuffer );
        LN_SAFE_RELEASE( mGraphicsDevice );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9VertexBuffer::create( DX9GraphicsDevice* device, LNVertexElemenst* elements, lnU32 vertexCount, const void* data, bool isDynamic )
    {
		HRESULT hr;

		LN_REFOBJ_SET( mGraphicsDevice, device );
		mVertexCount = vertexCount;

		// 頂点宣言の数を調べる
		int elem_size = 0;
		while ( 1 )
		{
			if ( elements[ elem_size ].Type == LN_DECL_UNKNOWN ) {
				break;
			}
			++elem_size;
		}
		LN_THROW_Argument((elem_size != 0), "elements size is 0.");

		// D3DVERTEXELEMENT9 を作成して、elements から DirectX用の頂点宣言を作る
		D3DVERTEXELEMENT9* dxelem = LN_NEW D3DVERTEXELEMENT9[elem_size + 1];
		lnU8 offset = 0;
		lnU8 to;
		for ( int i = 0; i < elem_size; ++i )
		{
			dxelem[ i ].Stream = 0;
			dxelem[ i ].Offset = offset;
			dxelem[ i ].Method = D3DDECLMETHOD_DEFAULT;
			dxelem[ i ].UsageIndex = elements[ i ].UsageIndex;
			
            DX9Module::convertElementLNToDX( &elements[ i ], &dxelem[ i ].Type, &to, &dxelem[ i ].Usage );
			offset += to;
		}
		dxelem[ elem_size ].Stream = 0xff;
		dxelem[ elem_size ].Offset = 0;
		dxelem[ elem_size ].Type = D3DDECLTYPE_UNUSED;
		dxelem[ elem_size ].Method = 0;
		dxelem[ elem_size ].Usage = 0;
		dxelem[ elem_size ].UsageIndex = 0;

		// 頂点ひとつ分のサイズ
		mVertexStride = offset;
		

		if ( isDynamic )
		{
			mVertices = LN_NEW lnByte[mVertexStride * mVertexCount];
		}
		else
		{
			// 頂点バッファ作成
			LN_COMCALL(
				mGraphicsDevice->getIDirect3DDevice9()->CreateVertexBuffer(
				    mVertexStride * mVertexCount,
				    0, 0,
				    D3DPOOL_MANAGED,
				    &mVertexBuffer,
				    NULL ) );
		}

		// 頂点宣言作成
		LN_COMCALL( mGraphicsDevice->getIDirect3DDevice9()->CreateVertexDeclaration( dxelem, &mVertexDecl ) );
		
		SAFE_DELETE( dxelem );

		// 頂点として設定するデータがある場合
		if ( data )
		{
			if ( isDynamic ) {
				memcpy_s( mVertices, mVertexStride * mVertexCount, data, mVertexStride * mVertexCount );
			}
			else {
				void* v = NULL;
				LN_COMCALL( mVertexBuffer->Lock( 0, 0, &v, D3DLOCK_DISCARD ) );
				memcpy_s( v, mVertexStride * mVertexCount, data, mVertexStride * mVertexCount );
				LN_COMCALL( mVertexBuffer->Unlock() );
			}
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9VertexBuffer::create( DX9GraphicsDevice* device, IDirect3DVertexBuffer9* device_obj, D3DVERTEXELEMENT9* layout )
    {
		LN_REFOBJ_SET( mGraphicsDevice, device );

        mVertexBuffer = device_obj;
        SAFE_ADDREF( mVertexBuffer );
		
		HRESULT hr;
		LN_COMCALL( mGraphicsDevice->getIDirect3DDevice9()->CreateVertexDeclaration( layout, &mVertexDecl ) );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void* DX9VertexBuffer::lock()
	{
		if ( mVertexBuffer )
		{
			HRESULT hr;
			void* v = NULL;
			LN_COMCALL( mVertexBuffer->Lock( 0, 0, &v, 0 ) );
			return v;
		}
		else 
		{
			return mVertices;
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9VertexBuffer::unlock()
	{
		if ( mVertexBuffer ) {
			HRESULT hr;
			LN_COMCALL( mVertexBuffer->Unlock() );
		}
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================