//==============================================================================
// EffekseerTextureLoaderDX9 
//------------------------------------------------------------------------------
///**
//  @file       EffekseerTextureLoaderDX9.h
//  @brief      EffekseerTextureLoaderDX9
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <Effekseer/Effekseer.h>
#ifdef LNOTE_DIRECTX
#include "../../Graphics/Device/DirectX9/DX9Texture.h"
#endif
#include "../../FileIO/Manager.h"

namespace LNote
{
namespace Core
{
namespace Effect
{
//==============================================================================
// Å° EffekseerTextureLoaderDX9
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class EffekseerTextureLoaderDX9
	: public ::Effekseer::TextureLoader
{
public:
	EffekseerTextureLoaderDX9( Graphics::IGraphicsDevice* device, FileIO::Manager* fileManager )
		: mGraphicsDevice	( device )
		, mFileIOManager	( fileManager )
	{
	}

	virtual ~EffekseerTextureLoaderDX9()
	{
	}

public:
	void* Load( const EFK_CHAR* path )
	{
		LRefPtr<FileIO::InFile> file( mFileIOManager->createInFile( (const wchar_t*)path ) );

		LRefPtr<Graphics::Texture> tex( mGraphicsDevice->createTexture( file ) );

		Graphics::DX9Texture* dxTex = static_cast<Graphics::DX9Texture*>( tex.getPtr() );

		IDirect3DTexture9* dx9Tex = dxTex->getIDirect3DTexture9();

		dx9Tex->AddRef();
		return (void*)dx9Tex;
	}

	void Unload( void* data )
	{
		if( data != NULL )
		{
			IDirect3DTexture9* texture = (IDirect3DTexture9*)data;
			texture->Release();
		}
	}

private:
	Graphics::IGraphicsDevice*	mGraphicsDevice;
	FileIO::Manager*			mFileIOManager;
};

} // namespace Effect
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================