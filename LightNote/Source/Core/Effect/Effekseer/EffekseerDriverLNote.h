//==============================================================================
// EffekseerDriverLNote
//------------------------------------------------------------------------------
///**
//  @file       EffekseerDriverLNote.h
//  @brief      EffekseerDriverLNote
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <Effekseer.h>

namespace LNote
{
namespace Core
{
namespace Effect
{

//==============================================================================
// EffectLoader
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class EffectLoader
	: public ::Effekseer::EffectLoader
{
public:
	EffectLoader( FileIO::Manager* fileManager )
		: mFileManager	( fileManager )
	{}

	virtual ~EffectLoader() {}

public:
	bool Load( const EFK_CHAR* path, void*& data, int32_t& size );

	void Unload( void* data, int32_t size );

private:

	FileIO::Manager* mFileManager;
};

//
////==============================================================================
//// SoundPlayer
////------------------------------------------------------------------------------
/////**
////  @brief
////*/
////==============================================================================
////class SoundImplemented 
////	: public ::Effekseer::Sound
////{
////}
//
////==============================================================================
//// SoundPlayer
////------------------------------------------------------------------------------
/////**
////  @brief
////*/
////==============================================================================
//class SoundPlayer
//	: public ::Effekseer::SoundPlayer
//{
//public:
//	SoundPlayer();
//	virtual ~SoundPlayer();
//
//public:
//	::Effekseer::SoundHandle Play( ::Effekseer::SoundTag tag, const ::Effekseer::SoundPlayer::InstanceParameter& parameter );
//	
//	void Stop( ::Effekseer::SoundHandle handle, ::Effekseer::SoundTag tag );
//
//	void Pause( ::Effekseer::SoundHandle handle, ::Effekseer::SoundTag tag, bool pause );
//
//	bool CheckPlaying( ::Effekseer::SoundHandle handle, ::Effekseer::SoundTag tag );
//
//	void StopTag( ::Effekseer::SoundTag tag );
//
//	void PauseTag( ::Effekseer::SoundTag tag, bool pause );
//	
//	bool CheckPlayingTag( ::Effekseer::SoundTag tag );
//
//	void StopAll();
//};

// TODO: 頂点バッファと頂点宣言を分離しないと効率的にバッファが使えないので、それまで保留
#if 0

#include <Effekseer/Effekseer.h>
#include <Effekseer/EffekseerRendererCommon/EffekseerRenderer.SpriteRendererBase.h>
#include <Effekseer/EffekseerRendererCommon/EffekseerRenderer.RibbonRendererBase.h>
#include <Effekseer/EffekseerRendererCommon/EffekseerRenderer.RingRendererBase.h>
#include <Effekseer/EffekseerRendererCommon/EffekseerRenderer.ModelRendererBase.h>
#include <Effekseer/EffekseerRendererCommon/EffekseerRenderer.TrackRendererBase.h>

using namespace ::EffekseerRenderer;

namespace LNote
{
namespace Core
{
namespace Effect
{
class SpriteRenderer;
class RibbonRenderer;
class RingRenderer;
class ModelRenderer;
class TrackRenderer;
class EffectLoader;
class TextureLoader;
class Shader;

struct Vertex
{
	::Effekseer::Vector3D	Pos;
	::Effekseer::Color		Color;	// GL のバイトオーダーを考慮してとりあえず float4
	//lnU32					Color;
	float					UV[2];

	void SetColor( const ::Effekseer::Color& color )
	{
		Color = color;
		//Col = D3DCOLOR_ARGB(
		//	color.A,
		//	color.R,
		//	color.G,
		//	color.B);
	}
};

//==============================================================================
// RendererImplemented
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class RendererImplemented
{
public:
	RendererImplemented();

public:
	void create( int squareMaxCount );

	SpriteRenderer*	getSpriteRenderer() { return mSpriteRenderer; }
	RibbonRenderer*	getRibbonRenderer() { return mRibbonRenderer; }
	RingRenderer*	getRingRenderer() { return mRingRenderer; }
	ModelRenderer*	getModelRenderer() { return mModelRenderer; }
	TrackRenderer*	getTrackRenderer() { return mTrackRenderer; }

	const ::Effekseer::Matrix44& GetCameraMatrix() const { return mCameraViewMatrix; }
	void SetCameraMatrix( const ::Effekseer::Matrix44& mat ) { mCameraViewMatrix = mat; }

	int	GetSquareMaxCount() const { return mSquareMaxCount; }

private:
	SpriteRenderer*	mSpriteRenderer;
	RibbonRenderer*	mRibbonRenderer;
	RingRenderer*	mRingRenderer;
	ModelRenderer*	mModelRenderer;
	TrackRenderer*	mTrackRenderer;

	::Effekseer::Matrix44	mCameraViewMatrix;

	int						mSquareMaxCount;	///< 共有頂点・インデックスバッファの確保基準

};

//==============================================================================
// SpriteRenderer
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class SpriteRenderer
	: public ::EffekseerRenderer::SpriteRendererBase
{
public:
	SpriteRenderer( RendererImplemented* effekseerRenderer );
	virtual ~SpriteRenderer();

protected:
	virtual void BeginRendering( const efkSpriteNodeParam& parameter, int32_t count,  void* userData );
	virtual void Rendering( const efkSpriteNodeParam& parameter, const efkSpriteInstanceParam& instanceParameter, void* userData );
	virtual void EndRendering( const efkSpriteNodeParam& parameter, void* userData );

private:
	RendererImplemented*	mEffekseerRenderer;
	Shader*					mShader;
	Shader*					mShaderNoTexture;
};

//==============================================================================
// RibbonRenderer
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class RibbonRenderer
	: public ::EffekseerRenderer::RibbonRendererBase
{

};

//==============================================================================
// RingRenderer
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class RingRenderer
	: public ::EffekseerRenderer::RingRendererBase
{

};

//==============================================================================
// ModelRenderer
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ModelRenderer
	: public ::EffekseerRenderer::ModelRendererBase
{

};

//==============================================================================
// TrackRenderer
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class TrackRenderer
	: public ::EffekseerRenderer::TrackRendererBase
{

};

//==============================================================================
// EffectLoader
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class EffectLoader
{

};

//==============================================================================
// TextureLoader
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class TextureLoader
{

};

//==============================================================================
// VertexBuffer
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class VertexBuffer
	: public VertexBufferBase
{
private:



	IDirect3DVertexBuffer9*	m_buffer;

	uint32_t				m_vertexRingOffset;
	bool					m_ringBufferLock;

	VertexBuffer( RendererImplemented* renderer, IDirect3DVertexBuffer9* buffer, int size, bool isDynamic );
public:
	virtual ~VertexBuffer();

	static VertexBuffer* Create( RendererImplemented* renderer, int size, bool isDynamic );

	IDirect3DVertexBuffer9* GetInterface() { return m_buffer; }

public:	// デバイス復旧用
	virtual void OnLostDevice();
	virtual void OnChangeDevice();
	virtual void OnResetDevice();

public:
	void Lock();
	bool RingBufferLock( int32_t size, int32_t& offset, void*& data );
	void Unlock();
};

//==============================================================================
// Shader
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class Shader
{
};

#endif

} // namespace Effect
} // namespace Core
} // namespace LNote
