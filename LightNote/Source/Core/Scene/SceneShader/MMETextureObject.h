//==============================================================================
// MMETextureObject 
//------------------------------------------------------------------------------
///**
//  @file       MMETextureObject.h
//  @brief      MMETextureObject
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../Graphics/Interface.h"
#include "../../Graphics/DeviceObjects.h"
#include "MMETypes.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// ■ MMETextureObject
//------------------------------------------------------------------------------
///**
//  @brief      
//  @note
//              Manager で管理される。
//              ベースのビューサイズが変更されれば、Manager はすべての
//              インスタンスに対して refresh を呼ぶ。
//              shared 修飾子付きでシェーダ変数が定義され、それにアノテーションが付いていた
//              (テクスチャ作成情報がある=テクスチャを作成する)場合は
//              すべてのインスタンスと equalTextureVariable() で同名・同セマンティクスで
//              既に作成されていないか調べて、作成されていたらエラーとする。
//
//				参照カウントは SceneShader が操作する。
//*/
//==============================================================================
class MMETextureObject
    : public Base::ReferenceObject
{
public:
	MMETextureObject( SceneShaderManager* manager );
	virtual ~MMETextureObject();

public:
    
	/// 初期化 (この後に refresh() を呼ぶこと)
    void initialize(
		SceneShader*		ownerShader,
		MMEVariableRequest	request,
        const lnRefString&  resourceName,
        const LVector3&     dimensions,
        const LVector2&     viewportRatio,
        int                 miplevels,
        Graphics::SurfaceFormat     format,
        bool                antiAlias );
    
	/// テクスチャリソースの再作成
    void refresh( const LVector2& viewSize );
    
	/// テクスチャを生成したシェーダ変数が同様の定義かを調べる (shared 用)
    bool equalTextureVariable( const MMETextureObject* textureObject );

	/// テクスチャの取得
	Graphics::Texture* getTexture() { return mTexture; }

private:
	void _dispose();
     
private:
    
	SceneShaderManager*	mManager;
	SceneShader*		mOwnerShader;
    Graphics::Texture*	mTexture;
    lnRefString			mVariableName;          ///< テクスチャ型の変数名
    MMESemantics		mVariableSemantics;     ///< テクスチャ型変数のセマンティクス

	MMEVariableRequest	mRequest;
    lnRefString         mResourceName;
    LVector3            mDimensions;
	LVector2			mViewportRatio;
    int                 mMiplevels;
    Graphics::SurfaceFormat     mFormat;
    bool                mAntiAlias;

};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================