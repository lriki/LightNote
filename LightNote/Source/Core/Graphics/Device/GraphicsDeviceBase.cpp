//==============================================================================
// GraphicsDeviceBase 
//==============================================================================

#include "stdafx.h"
#include "../../FileIO/Manager.h"
#include "../../FileIO/InFile.h"
#include "GraphicsResource.h"
#include "GraphicsDeviceBase.h"

// 新方式シェーダ

#if 0

/*
GL と DX ではテクスチャ座標が上下逆。
gl_TextureMatrix を使うなりして、頂点シェーダ内でテクスチャ座標を逆転する必要がある。

DX11 は texture と Sampler ステートは別変数。これだけなら DX9 と同じだけど、
SamplerState 変数を名前で取得できる。
http://maverickproj.web.fc2.com/d3d11_07.html

DX11 ではサンプラとテクスチャは完全に別物。SamplerState のパラメータに Texture= は存在しない。
サンプリングはこんな感じ。
return g_Tex.Sample( g_Sampler, In.texel );
http://maverickproj.web.fc2.com/d3d11_07.html
http://www.gamedev.net/topic/623734-dx11-hlsl-5-setting-samplerstate-from-within-the-shader-isnt-working/

DX9でも、プログラム側で texture と sampler 変数分けて持つことは可能。
sampler 変数はピクセルシェーダの constant table から持ってくる。

■Android
	AndroidのOpenGL ESでのテクスチャの扱いについて
	http://blog.livedoor.jp/sen_ritsu/archives/51267682.html

	Android で shaerd context
	(複数スレッドからgl API 呼べる)
	http://eaglesakura.hatenablog.com/entry/2013/12/28/235121

	onPause() 時に egl が無効になるが、この時テクスチャなどのリソースはすべて自動解放される。
	特に自分で delete する必要はナシ。
	ただし、onResume() 時に自分で再作成する必要がある。
	つまり、元の画像ファイルデータは常にアクセスできる状態になってないとダメ。
	drawText が可能なテクスチャはメモリに常駐させる必要があるし、
	static なテクスチャでも再度ファイルを読みに行けるようにファイル名を持ってないとダメ。

	PMX頂点は1つ44*float で 176byte。
	Unityちゃんは 16,000 ポリゴン。
	インデックス数はわからないけど、単純に 3M。

Unityちゃんを OpenGL で、
http://ramemiso.hateblo.jp/entry/2014/06/21/150405


■リソース管理
	デバイスリソースの作成は、最初 create した時と、onResume の時に作れるようにする。
	create 時にできないと、シェーダ変数のルックアップができない。

■GLSL制限
	・GLSLは、sampler 変数には 以下のようなマクロを必須にする。
		uniform sampler g_Sampler LN_TEXTURE(g_Texture);
		g_Texture は テクスチャ型変数を表すダミー変数。IShaderVariable として展開する
		LN_TEXTURE は空定義の関数マクロ。
	・lnfx に変換しないで生GLSLを使う場合は、#ifdef LN_GLSL_VERTEX、#ifdef LN_GLSL_FLAGMENT を使用し、
		ひとつのファイルの中に2つのシェーダを書き込むこと。
		この場合は単一 テクニック、単一パスになる。
		→ 生GLSL は、頭には入れておくけど優先度低めでいいと思う。
			その代り lnfx の充実を。



*/

class Shader
{
	// ↓いらないか・・・？
	//		glsl は hlsl の register みたいなのはない。全て変数名で uniform に渡す。
	//		Unity も、Unityがシェーダ変数に渡す値がいくつかあるけど、
	//		サンプラは渡さないみたい。http://qiita.com/edo_m18/items/5f1773345e5d87bddbef
	//		glsl で強引にやるなら、ln_Samapler0 とか、独自変数を作る感じになる。
	void setSamplerState(int slotIndex);	// slot とか
	void setTexture(int slotIndex);
};

class IDeviceShaderVariable
{
	/// ブール値の設定
	virtual void setBool(bool b) = 0;

	/// 整数値の設定
	virtual void setInt(int value) = 0;

	/// 実数値の設定
	virtual void setFloat(lnFloat value) = 0;

	/// ベクトルの設定
	virtual void setVector(const LVector4& vec) = 0;

	/// ベクトル配列の設定
	virtual void setVectorArray(const LVector4* vec, lnU32 count) = 0;

	/// 行列の設定
	virtual void setMatrix(const LMatrix& matrix) = 0;

	/// 行列の配列の設定
	virtual void setMatrixArray(const LMatrix* matrices, lnU32 count) = 0;

	/// テクスチャの設定 (サンプラステートは更新しない)
	virtual void setTexture(Texture* texture) = 0;
};

/// こっちでは取得もサポートする
class ShaderVariable
{
public:

	/// 型に応じて mDeviceShaderVariable に set
	void apply();

private:
	IDeviceShaderVariable*	mDeviceShaderVariable;
};


class IDeviceTexture
{
private:
};

class DX9Texture : public IDeviceTexture
{
private:
	IDirect3DTexture9* mDxTexture;
}

class Texture
{
public:

	void onResume()
	{
		mDeviceTexture = device->createtexture(mBitmap);

	}

private:
	IDeviceTexture* mDeviceTexture;
	Bitmap* mBitmap;
};

#endif

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ GraphicsDeviceBase
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	GraphicsDeviceBase::GraphicsDeviceBase()
		: mManager				( NULL )
		, mSystemManager		( NULL )
		, mFileManager			( NULL )
		, mEnableTextureCache	( true )
		, mEnableShaderCache	( true )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	GraphicsDeviceBase::~GraphicsDeviceBase() 
	{
		
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GraphicsDeviceBase::initialize( const ConfigData& configData )
	{
		this->mManager			= configData.Manager;
		this->mSystemManager	= configData.SystemManager;
		this->mFileManager		= configData.FileManager;
		this->mTextureCache.initialize( configData.TextureCacheSize, 0 );
		this->mShaderCache.initialize( configData.ShaderCacheSize, 0 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GraphicsDeviceBase::finalize()
	{
		this->mTextureCache.finalize();
		this->mShaderCache.finalize();
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Base::ICacheObject* GraphicsDeviceBase::findTextureCache( const lnSharingKey& key )
	{
		return mTextureCache.findCacheAddRef( key );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GraphicsDeviceBase::clearTextureCache()
	{
		this->mTextureCache.clearCacheList();
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//IShader* GraphicsDeviceBase::createShader( const lnChar* filePath, lnSharingKey registerKey )
	//{
	//	// キャッシュ検索
	//	IShader* shader = findShaderCache( registerKey );
	//	if ( shader ) {
	//		return shader;
	//	}

	//	LRefPtr<FileIO::InFile> file(
	//		this->mFileManager->createInFile( filePath ) );

 //       // mTempBuffer に全部読み込む
 //       lnU32 size = file->getSize();

 //       Base::TempBufferLock tlock( this->mTempBuffer, size + 1 );
 //       char* buf = (char*)tlock.getBuffer();
 //       file->read( buf, size );
 //       buf[ size ] = '\0';

	//	return createShader( (const void*)buf, size, registerKey );
	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Base::ICacheObject* GraphicsDeviceBase::findShaderCache( const lnSharingKey& key )
	{
		return mShaderCache.findCacheAddRef( key );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GraphicsDeviceBase::clearShaderCache()
	{
		this->mShaderCache.clearCacheList();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GraphicsDeviceBase::pauseDevice()
	{
		ln_foreach( GraphicsResource* resource, mGraphicsResourceList ) {
			resource->onPauseDevice();
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GraphicsDeviceBase::resumeDevice()
	{
		ln_foreach( GraphicsResource* resource, mGraphicsResourceList ) {
			resource->onResumeDevice();
		}
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================