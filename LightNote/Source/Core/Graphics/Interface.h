//==============================================================================
// Graphics 
//------------------------------------------------------------------------------
///**
//  @file       Graphics.h
//  @brief      Graphics
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Base/SharingKey.h"
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include "../Math/Matrix.h"
#include "../Geometry/DrawingShapes.h"
#include "../System/Interface.h"
#include "../FileIO/Interface.h"
#include "Common/Common.h"
#include "Common/RenderState.h"
#include "Common/VertexTypes.h"
#include "Common.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ IGraphicsDevice
//------------------------------------------------------------------------------
///**
//  @brief      グラフィックデバイスのインターフェイス
//*/
//==============================================================================
class IGraphicsDevice
	: public Base::ReferenceObject
{
public:

	/// 使用中の GraphicsAPI の取得
    virtual LNGraphicsAPI getGraphicsAPI() = 0;

	/// 描画クラスの取得
    virtual IRenderer* getRenderer() = 0;

	/// デフォルトのバックバッファを示すキャンバスの取得
    virtual ICanvas* getDefaultCanvas() = 0;

	/// 頂点バッファの作成
    virtual IVertexBuffer* createVertexBuffer( LNVertexElemenst* elements, lnU32 vertexCount, const void* data, bool isDynamic = false ) = 0;

	/// インデックスバッファの作成
    virtual IIndexBuffer* createIndexBuffer( lnU32 indexCount, const void* data, bool isDynamic = false, bool is16bit = true ) = 0;

	/// テクスチャの作成
    virtual Texture* createTexture( lnU32 width, lnU32 height, lnU32 levels, SurfaceFormat format = SurfaceFormat_A8R8G8B8 ) = 0;

	///// テクスチャの作成 (入力ストリームから)
	virtual Texture* createTexture( FileIO::Stream* stream, lnU32 colorKey = 0, lnU32 levels = 1, SurfaceFormat format = SurfaceFormat_Unknown, lnSharingKey key = lnNullKey ) = 0;

	/// テクスチャの作成 (ファイルから) TODO:ユーティリティ関数化
    virtual Texture* createTexture( const lnChar* filePath, lnU32 colorKey = 0, lnU32 levels = 1, SurfaceFormat format = SurfaceFormat_Unknown, lnSharingKey key = lnNullKey ) = 0;

	/// レンダーターゲットテクスチャの作成
    virtual Texture* createRenderTarget( lnU32 width, lnU32 height, lnU32 mipLevels = 1, SurfaceFormat format = SurfaceFormat_A8R8G8B8 ) = 0;

	/// 深度バッファの作成
    virtual Texture* createDepthBuffer( lnU32 width, lnU32 height, SurfaceFormat format ) = 0;

	/// シェーダの作成
	//virtual IShader* createShader( const void* data, lnU32 size, lnSharingKey registerKey = lnNullKey ) = 0;

	/// シェーダの作成 (ファイルから)
	//virtual IShader* createShader( const lnChar* filePath, lnSharingKey registerKey = lnNullKey ) = 0;
	virtual IShader* createShader(FileIO::Stream* stream, lnSharingKey registerKey, lnString* errors) = 0;

	/// テクスチャキャッシュの有効設定
	virtual void setEnableTextureCache( bool enabled ) = 0;

	/// シェーダキャッシュの有効設定
	virtual void setEnableShaderCache( bool enabled ) = 0;

	/// テクスチャキャッシュのクリア
    virtual void clearTextureCache() = 0;

	/// シェーダキャッシュのクリア
    virtual void clearShaderCache() = 0;

	///// デバイス停止 (デバイスリセット前処理)
	//virtual void pauseDevice() = 0;

	///// デバイス再開 (リセット実行 & デバイスリセット後処理)
	//virtual void resumeDevice() = 0;

	/// デバイスをロストしているかを確認し、ロストしていればリセットする
	///		初期化を行ったスレッドで定期的に呼ぶこと。
	///		また、描画を別スレッドで行っている場合は描画中に呼ばないこと。
	///		DirectX のリセットは、必ず DirectX を初期化したスレッドで行わなければならない。
	///		例えば、バックバッファサイズを変更したいとき、別スレッドで描画を行っていた場合等は
	///		一度描画終了まで待つ必要がある。
	//virtual void testDeviceLost() = 0;

	virtual DeviceState getDeviceState() = 0;

	virtual void resetDevice() = 0;
	
	/// Graphics インターフェイスの関数を呼び出すスレッドで、最初に呼び出しておく (Manager を初期化したスレッド以外)
	virtual void attachCurrentThread() = 0;

protected:
    virtual ~IGraphicsDevice() {};
};

//==============================================================================
// ■ IRenderer
//------------------------------------------------------------------------------
///**
//  @brief     描画とそのステート管理を行うインターフェイス
//*/
//==============================================================================
class IRenderer
	: public Base::ReferenceObject
{
public:

	/// レンダリングステートの設定
    virtual void setRenderState( const LRenderState& state, bool reset = false ) = 0;

	/// レンダリングステートの取得
    virtual const LRenderState& getRenderState() = 0;

	/// レンダリングターゲットの設定 (index 0 に NULL が渡されると例外となる)
    virtual void setRenderTarget( lnU32 index, Texture* texture, bool reset = false ) = 0;

	/// レンダリングターゲットの取得
    virtual Texture* getRenderTarget( lnU32 index ) = 0;

	/// 深度バッファの設定
    virtual void setDepthBuffer( Texture* texture, bool reset = false ) = 0;

	/// 深度バッファの取得
    virtual Texture* getDepthBuffer() = 0;

	/// ビューポート矩形の設定
	virtual void setViewport( const LRect& rect ) = 0;

	/// ビューポート矩形の取得
    virtual const LRect& getViewport() const = 0;

	/// シザー矩形の設定 (使用しない場合、要素がすべて 0 か、ビューポートと等しい rect を渡す)
	virtual void setScissor( const LRect& rect ) = 0;

	/// シザー矩形の取得
    virtual const LRect& getScissor() const = 0;

	/// 頂点バッファの設定
	virtual void setVertexBuffer( IVertexBuffer* vertexBuffer, bool reset = false ) = 0;

	/// インデックスバッファの設定
	virtual void setIndexBuffer( IIndexBuffer* indexBuffer, bool reset = false ) = 0;

	/// 設定されているカラーバッファ、深度バッファをクリアする
	virtual void clear( bool target, bool depth, const LColorF& color, lnFloat z = 1.0f ) = 0;

	/// プリミティブ描画
	virtual void drawPrimitive( PrimitiveType primitive, lnU32 startVertex, lnU32 primitiveCount ) = 0;

	/// プリミティブ描画 (インデックス付き。頂点、インデックスの両方のバッファのdynamic、static が一致している必要がある)
	virtual void drawPrimitiveIndexed( PrimitiveType primitive, lnU32 startIndex, lnU32 primitiveCount ) = 0;

	/// シーン開始
	virtual void beginScene() = 0;

	/// シーン終了
	virtual void endScene() = 0;
   
protected:
    virtual ~IRenderer() {}
};

//==============================================================================
// ■ ICanvas
//------------------------------------------------------------------------------
///**
//  @brief     バックバッファのインターフェイス
//*/
//==============================================================================
class ICanvas
	: public Base::ReferenceObject
{
public:

	/// キャンバスサイズ (バックバッファサイズ)を取得する
    virtual const LSize& getSize() = 0;

	/// キャンバスサイズ(バックバッファサイズ)を変更する
    virtual void resize( const LSize& size ) = 0;

	/// バックバッファの取得 (レターボックス使用時の手前のバッファ)
    virtual Texture* getBackbuffer() = 0;

	/// バックバッファへ描画できる状態にする (CanvasBufferを描画先レンダリングターゲットに設定する)
    virtual void activate() = 0;

	/// バックバッファの内容をウィンドウへ転送する
    virtual void present() = 0;

	/// キャンバス→ウィンドウへ転送する際の座標変換行列の取得 ( GUI 等でマウス座標の位置を合わせるために用意したもの)
    virtual const LMatrix& getScreenTransform() const = 0;

	/// バックバッファの取得 (最終的にウィンドウへ転送するバッファ)
	///		アスペクト比固定の場合はこれに対応する深度バッファは生成されない。
    virtual Texture* getBackendBuffer() = 0;

protected:
    virtual ~ICanvas() {}
};

//==============================================================================
// ■ IVertexBuffer
//------------------------------------------------------------------------------
///**
//  @brief     頂点バッファのインターフェイス
//*/
//==============================================================================
class IVertexBuffer
	: public Base::ReferenceObject
{
public:

	/// 頂点数の取得
	virtual lnU32 getVertexCount() const = 0;

	/// 頂点ひとつ分のサイズの取得
	virtual lnU32 getVertexStride() const = 0;

	/// 動的な頂点かを判定する
	virtual bool isDynamic() const = 0;

	/// ロック (dynamic 以外は動作保障外)
	virtual void* lock() = 0;

	/// アンロック
	virtual void unlock() = 0;

protected:
    virtual ~IVertexBuffer() {};
};

//==============================================================================
// ■ IIndexBuffer
//------------------------------------------------------------------------------
///**
//  @brief     インデックスバッファのインターフェイス
//*/
//==============================================================================
class IIndexBuffer
	: public Base::ReferenceObject
{
public:

	/// インデックス数の取得
    virtual lnU32 getIndexCount() const = 0;

	/// インデックスひとつ分のサイズの取得 (2 または 4)
	virtual lnU32 getIndexStride() const = 0;

	/// 動的なインデックスバッファかを判定する	
	virtual bool isDynamic() const = 0;

	/// ロック (dynamic 以外は動作保障外)
	virtual void* lock() = 0;

	/// アンロック
	virtual void unlock() = 0;

protected:
    virtual ~IIndexBuffer() {};
};

//==============================================================================
// ■ ITexture
//------------------------------------------------------------------------------
///**
//  @brief     テクスチャのインターフェイス
//*/
//==============================================================================
//class ITexture
//    : public Base::ReferenceObject
//{
//public:
//
//	/// テクスチャの種類の取得
//	virtual LNTextureType getType() const = 0;
//
//	/// サーフェイスフォーマットの取得
//	virtual SurfaceFormat getSurfaceFormat() const = 0;
//
//	/// サイズの取得
//    virtual const LVector2& getSize() const = 0;
//
//	/// 実際のサイズの取得
//	virtual const LVector2& getRealSize( bool isReciprocal = false ) const = 0;
//
//	/// クリア
//	virtual void clear( const LColor& color = LColor::Transparency ) = 0;
//
//	/// ロック
//	virtual lnU32* lock() = 0;
//
//	/// アンロック
//	virtual void unlock() = 0;
//
//	/// drawText() での文字列描画に使用するフォントの設定 (レンダリングターゲット、深度バッファの場合は無効)
//    virtual void setFont( IFont* font ) = 0;
//
//	/// drawText() での文字列描画に使用するフォントの取得
//    virtual IFont* getFont() const = 0;
//
//	/// 文字列の描画
//    virtual void drawText( const lnChar* text, const LRect& rect, TextAlign align = TextAlign_Left, int length = -1 ) = 0;
//
//	/// テクスチャのブロック転送 (現在、普通のテクスチャ (RT、深度バッファではない) にのみ有効)
//    virtual void blt( int x, int y, ITexture* srcTexture, const LRect& srcRect ) = 0;
//
//	/// ファイルから作成したときのファイル名
//    virtual const lnChar* getFilename() const = 0;
//
//protected:
//    virtual ~ITexture() {}
//};

//==============================================================================
// ■ IFont
//------------------------------------------------------------------------------
///**
//  @brief      フォントのインターフェイス
//*/
//==============================================================================
//class IFont
//	: public Base::ReferenceObject
//{
//public:
//
//	/// フォント名の設定
//	virtual void setName( const lnChar* fontName ) = 0;
//
//	/// フォント名の取得
//	virtual const lnChar* getName() const = 0;
//
//	/// フォントサイズの設定
//	virtual void setSize( lnU32 size ) = 0;
//
//	/// フォントサイズの取得
//	virtual lnU32 getSize() const = 0;
//
//	/// フォントカラーの設定
//	virtual void setColor(const LColor& color) = 0;
//
//	/// フォントカラーの取得
//	virtual const LColor& getColor() const = 0;
//
//	/// エッジカラーの設定
//	virtual void setEdgeColor(const LColor& color) = 0;
//
//	/// エッジカラーの取得
//	virtual const LColor& getEdgeColor() const = 0;
//
//	/// エッジの幅の設定 (0 でエッジ無効)
//	virtual void setEdgeSize( lnU32 size ) = 0;
//
//	/// エッジの幅の取得
//	virtual lnU32 getEdgeSize() const = 0;
//
//	/// 太文字の設定
//	virtual void setBold( bool flag ) = 0;
//
//	/// 太文字の判定
//	virtual bool isBold() const = 0;
//
//	/// イタリック体の設定
//	virtual void setItalic( bool flag ) = 0;
//
//	/// イタリック体の判定
//	virtual bool isItalic() const = 0;
//
//	/// アンチエイリアスの有効設定
//	virtual void setAntiAlias( bool flag ) = 0;
//
//	/// アンチエイリアスの有効判定
//	virtual bool isAntiAlias() const = 0;
//
//	/// 文字列を描画したときのサイズ (ピクセル単位) の取得 (length = -1 で \0 まで)
//	virtual void getTextSize(const char* text, int length, Geometry::Rect* outRect) = 0;
//
//	/// 文字列を描画したときのサイズ (ピクセル単位) の取得 (length = -1 で \0 まで)
//	virtual void getTextSize(const wchar_t* text, int length, Geometry::Rect* outRect) = 0;
//
//	/// このフォントのコピーを作成する
//    virtual IFont* copy() = 0;
//
//	/// グリフデータの取得 (最初の文字の場合、prevData に NULL を渡す。以降は戻り値を渡し続ける。非スレッドセーフ)
//	virtual FontGlyphData* makeGlyphData(lnU32 utf32code, FontGlyphData* prevData) = 0;
//
//	/// グリフデータの取得を終了する (メモリ解放。一連の makeGlyphData() を呼び終わった後、最後に呼ぶ)
//	virtual void postGlyphData( FontGlyphData* glyphData ) = 0;
//
//protected:
//    virtual ~IFont() {}
//};

//==============================================================================
// ■ IShader
//------------------------------------------------------------------------------
///**
//  @brief      シェーダプログラムのインターフェイス
//*/
//==============================================================================
class IShader
	: public Base::ReferenceObject//Base::Interface
{
public:

	/// インデックスによって変数を取得する
	virtual IShaderVariable* getVariableByIndex( lnU32 index ) = 0;

	/// 名前によって変数を取得する
	virtual IShaderVariable* getVariableByName( const char* name ) = 0;

	/// セマンティクスによって変数を取得する
	//virtual IShaderVariable* getVariableBySemantic( const char* name_ ) = 0;

	/// インデックスによってテクニックを取得する
	virtual IShaderTechnique* getTechniqueByIndex( lnU32 index ) = 0;
    
	/// 名前によってテクニックを取得する
	virtual IShaderTechnique* getTechniqueByName( const char* name ) = 0;

#ifndef LNOTE_FOR_200
	/// シェーダの情報を取得する
	virtual const LNShaderDesc& getDesc() = 0;
#endif

	/// コンパイル結果を取得する
	virtual LNShaderCompileResult getCompileResult() = 0;

	/// コンパイルエラー・警告の文字列を返す
	virtual const lnChar* getCompileErrors() = 0;

protected:

    virtual ~IShader() {}
};

//==============================================================================
// ■ IShaderVariable
//------------------------------------------------------------------------------
///**
//  @brief      シェーダプログラム内の変数のインターフェイス
//	@par
//				getBool()、getInt() 等のシェーダ変数の値を取得する関数は、
//				OpenGL では使用できません。
//*/
//==============================================================================
class IShaderVariable
	: public Base::ReferenceObject
{
public:

	/// 変数の型の取得
	virtual ShaderVariableType getType() const = 0;

	/// 変数名の取得
	virtual const char* getName() const = 0;

	/// セマンティクス名の取得 (ない場合は NULL)
	virtual const char* getSemanticName() const = 0;

	/// ブール値の取得
	virtual bool getBool() = 0;

	/// ブール値の設定
	virtual void setBool( bool b ) = 0;

	/// 整数値の取得
	virtual int getInt() = 0;

	/// 整数値の設定
	virtual void setInt( int value ) = 0;

	/// 実数値の取得
    virtual lnFloat getFloat() = 0;

	/// 実数値の設定
    virtual void setFloat( lnFloat value ) = 0;

	/// ベクトルの取得
	virtual const LVector4& getVector() = 0;

	/// ベクトルの設定
	virtual void setVector( const LVector4& vec ) = 0;

	/// ベクトル配列の取得
	virtual const LVector4* getVectorArray() = 0;
    
	/// ベクトル配列の設定
	virtual void setVectorArray( const LVector4* vec, lnU32 count ) = 0;

	/// 行列の取得
    virtual const LMatrix& getMatrix() = 0;

	/// 行列の設定
    virtual void setMatrix( const LMatrix& matrix ) = 0;

	/// 行列の配列の取得
	virtual const LMatrix* getMatrixArray() = 0;

	/// 行列の配列の設定
	virtual void setMatrixArray( const LMatrix* matrices, lnU32 count ) = 0;

	/// テクスチャの取得
	virtual Texture* getTexture() = 0;

	/// テクスチャの設定 (サンプラステートは更新しない)
	virtual void setTexture( Texture* texture ) = 0;

	/// テクスチャの設定
	//virtual void setTexture( ITexture* texture, const LSamplerState& state ) = 0;

	/// 文字列の取得 (OpenGL 非対応)
	virtual const char* getString() = 0;

	/// 文字列の設定 (OpenGL 非対応)
	virtual void setString( const char* str ) = 0;

	/// 型情報の取得
    virtual const LNShaderVariableTypeDesc& getTypeDesc() const = 0;

	/// インデックスによってアノテーションを取得する
	virtual IShaderVariable* getAnnotationByIndex( lnU32 index ) = 0;

	/// 名前によってアノテーションを取得する
	virtual IShaderVariable* getAnnotationByName( const char* name ) = 0;

protected:
    virtual ~IShaderVariable() {}
};

//==============================================================================
// ■ IShaderTechnique クラス
//------------------------------------------------------------------------------
///**
//  @brief      シェーダプログラム内のテクニックのインターフェイス
//*/
//==============================================================================
class IShaderTechnique
	: public Base::ReferenceObject
{
public:

	/// テクニックの名前を取得する
    virtual const char* getName() const = 0;

	/// テクニック内のパスの数を取得する
	virtual lnU32 getPasses() const = 0;

	/// インデックスによってパスを取得する
	virtual IShaderPass* getPassByIndex( lnU32 index ) = 0;

	/// 名前によってパスを取得する
	virtual IShaderPass* getPassByName( const char* name ) = 0;

	/// インデックスによってアノテーションを取得する
	virtual IShaderVariable* getAnnotationByIndex( lnU32 index ) = 0;

	/// 名前によってアノテーションを取得する
	virtual IShaderVariable* getAnnotationByName( const char* name ) = 0;

protected:
    virtual ~IShaderTechnique() {}
};

//==============================================================================
// ■ IShaderPass
//------------------------------------------------------------------------------
///**
//  @brief      シェーダプログラム内のパスのインターフェイス
//*/
//==============================================================================
class IShaderPass
	: public Base::ReferenceObject
{
public:

	/// パスの名前を取得する
    virtual const lnChar* getName() const = 0;

	/// インデックスによってアノテーションを取得する
	virtual IShaderVariable* getAnnotationByIndex( lnU32 index ) = 0;

	/// 名前によってアノテーションを取得する
	virtual IShaderVariable* getAnnotationByName( const lnChar* name ) = 0;

	/// パスの適用を開始する
	virtual void begin() = 0;

	/// パスの適用を終了する
	virtual void end() = 0;

	/// パスの実行中に変化したステートをデバイスに送信する
	virtual void commit() = 0;

protected:
    virtual ~IShaderPass() {}
};


} // namespace Graphics
} // namespace Core
} // namespace LNote
