//==============================================================================
// DX9GraphicsDevice 
//------------------------------------------------------------------------------
///**
//  @file       DX9GraphicsDevice.h
//  @brief      DX9GraphicsDevice
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <list>
#include "../../../Threading/EventFlag.h"
#include "../GraphicsDeviceBase.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ DX9GraphicsDevice
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class DX9GraphicsDevice
    : public GraphicsDeviceBase
{
public:

	struct ConfigData
		: public GraphicsDeviceBase::ConfigData
	{
		LSize							BackbufferSize;
		Graphics::BackbufferResizeMode	BackbufferResizeMode;
		bool							FullScreen;
	    bool							EnableVSyncWait;
		bool							EnableFPUPreserve;
    };

public:
	DX9GraphicsDevice();
	virtual ~DX9GraphicsDevice();

public:

	/// 初期化
    void initialize( const ConfigData& configData );

	/// 終了処理
    virtual void finalize();

	/// フルスクリーンにできるか
    bool canFullScreen() const { return ( mCanvasList.size() == 1 ); }

	/// IDirect3DDevice9 取得
    IDirect3DDevice9* getIDirect3DDevice9() { return mDxDevice; }
	
	/// ID3DXEffectPool 取得
	ID3DXEffectPool* getD3DXEffectPool() { return mD3DXEffectPool; }

	/// デバイスロスト発生フラグのセット
	void setDeviceLostFlag() { mIsDeviceLost.setTrue(); }

	/// デバイスロスト発生中
	bool isDeviceLost() { return mIsDeviceLost.isTrue(); }

public:
	virtual LNGraphicsAPI getGraphicsAPI() { return LN_GRAPHICSAPI_DIRECTX9; }
	virtual IRenderer* getRenderer();
    virtual ICanvas* getDefaultCanvas();
    virtual IVertexBuffer* createVertexBuffer( LNVertexElemenst* elements, lnU32 vertexCount, const void* data, bool isDynamic );
    virtual IIndexBuffer* createIndexBuffer( lnU32 indexCount, const void* data, bool isDynamic, bool is16bit );
    virtual Texture* createTexture( lnU32 width, lnU32 height, lnU32 levels, Graphics::SurfaceFormat format );
	virtual Texture* createTexture( FileIO::Stream* stream, lnU32 colorKey, lnU32 levels, Graphics::SurfaceFormat format, lnSharingKey key );
    virtual Texture* createTexture( const lnChar* filePath, lnU32 colorKey, lnU32 levels, Graphics::SurfaceFormat format, lnSharingKey key );
    virtual Texture* createRenderTarget( lnU32 width, lnU32 height, lnU32 mipLevels, Graphics::SurfaceFormat format );
    virtual Texture* createDepthBuffer( lnU32 width, lnU32 height, Graphics::SurfaceFormat format );
	//virtual IShader* createShader( const void* data, lnU32 size, lnSharingKey registerKey );
	//virtual IShader* createShader( const lnChar* filePath, lnSharingKey registerKey );
	virtual IShader* createShader(FileIO::Stream* stream, lnSharingKey registerKey, lnString* errors);
    virtual void pauseDevice();
    virtual void resumeDevice();
	virtual DeviceState getDeviceState();
	virtual void resetDevice();
	//virtual void testDeviceLost();
	virtual void attachCurrentThread() {}

private:
	void _checkDeviceInformation();
	void _resetDevice( bool fullscreen/*, const LSize& backbufferSize*/ );
	void _setPresentParameters( const LSize& backbufferSize );

private:
    typedef std::list<DX9CanvasBase*>			CanvasList;
	//typedef std::list<DX9RenderTargetTexture*>	RenderTargetList;
	//typedef std::list<DX9DepthBuffer*>			DepthBufferList;
	//typedef std::list<DX9Shader*>				ShaderList;

private:
	DX9Canvas*					mDefaultCanvas;
	DX9Renderer*				mRenderer;

	IDirect3D9*		            mDirect3D;
    IDirect3DDevice9*	        mDxDevice;
    D3DPRESENT_PARAMETERS       mPresentParameters;	///< プレゼンテーションパラメータ
	ID3DXEffectPool*			mD3DXEffectPool;

	// 調査結果
    D3DCAPS9                    mDxCaps;
    D3DDISPLAYMODE              mDxDisplayMode;
    D3DTEXTUREFILTERTYPE        mCurrentMinFilter;
	D3DTEXTUREFILTERTYPE        mCurrentMagFilter;
	D3DTEXTUREFILTERTYPE        mCurrentMipFilter;
	D3DTEXTUREFILTERTYPE        mBestMinFilter;
	D3DTEXTUREFILTERTYPE        mBestMagFilter;
	D3DTEXTUREFILTERTYPE        mBestMipFilter;
    DWORD                       mMSAAQualityWindowed;
    DWORD                       mMSAAQualityFullScreen;
    D3DDEVTYPE				    mDeviceType;        ///< デバイスの種類。_checkDeviceInformation() でチェックして initialize() に伝えるために用意

	// デバイスロストで復帰処理を行うもの
	CanvasList                  mCanvasList;
	//RenderTargetList            mRenderTargetList;
	//DepthBufferList             mDepthBufferList;
	//ShaderList                  mShaderList;
	
    Threading::EventFlag		mIsDeviceLost;
    bool                        mEnableFPUPreserve;
	bool						mIsFullScreen;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================