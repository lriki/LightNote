//==============================================================================
// BatchSprite 
//------------------------------------------------------------------------------
///**
//  @file       BatchSprite.h
//  @brief      BatchSprite
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "VisualNode.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// ■ BatchSprite3D
//------------------------------------------------------------------------------
///**
//  @brief		バッチ描画3Dスプライト (※シェーダ使用不可)
//	@note
//				ポストエフェクトには描画されるが、オフスクリーンシーンには描画されない。
//*/
//==============================================================================
class BatchSprite3D
    : public VisualNode
{
public:
    BatchSprite3D( SceneGraph* scene );
    virtual ~BatchSprite3D();

public:

	/// 初期化
    void create( lnFloat w_size, lnFloat h_size, LNAxisAlignedDir dir = LN_AADIR_RZ );

	/// 大きさの設定
    void setSize( const LVector2& size ) { mSize = size; }

	/// 大きさの取得
    const LVector2& getSize() const { return mSize; }

	/// テクスチャの設定
    void setTexture( Graphics::Texture* texture );

	/// テクスチャの取得
    Graphics::Texture* getTexture() { return mTexture; }

	/// テクスチャの転送矩形の設定
    void setSrcRect( const LRectF& rect );

	/// テクスチャの転送矩形の取得
    const LRectF& getSrcRect() const { return mSrcRect; }

	/// 反転方法の設定
    void setFlipFlag( lnU8 flags ) { mFlipFlags = flags; }

	/// 反転方法の取得
    lnU8 getFlipFlag() const { return mFlipFlags; }

public:
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_BATCHSPRITE3D; }

private:
    LNAxisAlignedDir    mAADir;
    LVector2            mSize;
    Graphics::Texture* mTexture;
    LRectF		mSrcRect;
    lnU8                mFlipFlags;
    bool                mCalledSetSrcRect;

    class BatchSprite3DContext;
    friend class BatchSprite3DContext;
};


//==============================================================================
// ■ BatchSprite
//------------------------------------------------------------------------------
///**
//  @brief		バッチ描画2Dスプライト (※シェーダ使用不可)
//*/
//==============================================================================
class BatchSprite
    : public VisualNode
{
public:
    BatchSprite( SceneGraph* scene );
    virtual ~BatchSprite();

public:

	/// 作成
    void create();

	/// テクスチャの設定
    void setTexture( Graphics::Texture* texture );

	/// テクスチャの取得
    Graphics::Texture* getTexture() { return mTexture; }

	/// テクスチャの転送矩形の設定
    void setSrcRect( const LRectF& rect );

	/// テクスチャの転送矩形の取得
    const LRectF& getSrcRect() const { return mSrcRect; }

	/// 反転方法の設定
    void setFlipFlag( lnU8 flags ) { mFlipFlags = flags; }

	/// 反転方法の取得
    lnU8 getFlipFlag() const { return mFlipFlags; }

public:
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_BATCHSPRITE; }

private:
    Graphics::Texture* mTexture;
    LRectF		mSrcRect;
    lnU8                mFlipFlags;
    bool                mCalledSetSrcRect;

    class BatchSpriteContext;
    friend class BatchSpriteContext;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================