//==============================================================================
// Common
//------------------------------------------------------------------------------
///**
//  @file       Common.h
//  @brief      Common
//  @author     Riki
//*/
//==============================================================================

#pragma once

namespace LNote
{
namespace Core
{
namespace Graphics
{
	class Manager;

	class IGraphicsDevice;
	class IResourceManager;
	class IRenderer;
	class ICanvas;
	class IVertexBuffer;
	class IIndexBuffer;
	class Texture;
	class Font;
	class IShader;
	class IShaderVariable;
	class IShaderTechnique;
	class IShaderPass;

	class GraphicsResource;
	class GraphicsDeviceBase;

	class GeometryRenderer;
	class SpriteRenderer;
	class Image;

	class FontManagerBase;
	class FreeTypeFont;

	class ModelManager;

	class DX9GraphicsDevice;
	class DX9Renderer;
	class DX9CanvasBase;
	class DX9Canvas;
	class DX9RenderTargetTexture;
	class DX9DepthBuffer;
	class DX9SystemCreatedTexture;
	class DX9Shader;

	class GLPlatformContext;
	class GLGraphicsDevice;
	class GLRenderer;
	class GLCanvas;

	/// マルチレンダリングターゲットの最大数
	static const int MaxMultiRenderTargets = 4;

	/// サンプラステージの最大数
	static const int MaxSamplerStages = 8;

} // namespace Graphics
} // namespace Core
} // namespace LNote
