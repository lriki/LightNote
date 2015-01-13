//==============================================================================
// EffekseerDriverLNote 
//==============================================================================

#pragma once

#include "stdafx.h"
#include "../../FileIO/FileUtil.h"
#include "../../FileIO/Manager.h"
#include "EffekseerDriverLNote.h"

namespace LNote
{
namespace Core
{
namespace Effect
{

//==============================================================================
// EffectLoader
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool EffectLoader::Load( const EFK_CHAR* path, void*& data, int32_t& size )
	{
		assert( path != NULL );

		LRefPtr<FileIO::InFile> file( mFileManager->createInFile( (wchar_t*)path ) );
		size = file->getSize();
		data = new uint8_t[size];
		file->read( data, size );

		return true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void EffectLoader::Unload( void* data, int32_t size )
	{
		uint8_t* data8 = (uint8_t*)data;
		ES_SAFE_DELETE_ARRAY( data8 );
	}


#if 0

//==============================================================================
// RendererImplemented
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	RendererImplemented::RendererImplemented()
		: mSquareMaxCount	( 0 )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void RendererImplemented::create( int squareMaxCount )
	{
		mSquareMaxCount = squareMaxCount;
	}

//==============================================================================
// SpriteRenderer
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	SpriteRenderer::SpriteRenderer( RendererImplemented* effekseerRenderer )
		: mEffekseerRenderer	( effekseerRenderer )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	SpriteRenderer::~SpriteRenderer() 
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SpriteRenderer::BeginRendering( const efkSpriteNodeParam& parameter, int32_t count,  void* userData )
	{
		BeginRendering_<RendererImplemented, Vertex>( mEffekseerRenderer, count );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SpriteRenderer::Rendering( const efkSpriteNodeParam& parameter, const efkSpriteInstanceParam& instanceParameter, void* userData )
	{
		if( m_spriteCount == mEffekseerRenderer->GetSquareMaxCount() ) return;
		Rendering_<Vertex>( parameter, instanceParameter, userData, mEffekseerRenderer->GetCameraMatrix() );
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SpriteRenderer::EndRendering( const efkSpriteNodeParam& parameter, void* userData )
	{
		if( m_ringBufferData == NULL ) return;

		mEffekseerRenderer->GetVertexBuffer()->Unlock();

		if( m_spriteCount == 0 ) return;
	
		EndRendering_<RendererImplemented, Shader, IDirect3DTexture9*, Vertex>( mEffekseerRenderer, mShader, mShaderNoTexture, parameter );
	}

#endif

} // namespace Effect
} // namespace Core
} // namespace LNote
