//==============================================================================
// Sprite3D 
//------------------------------------------------------------------------------
///**
//  @file       Sprite3D.h
//  @brief      Sprite3D
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
// ■ Sprite3D
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class Sprite3D
    : public VisualNode
{
public:
    Sprite3D( SceneGraph* scene );
    virtual ~Sprite3D();

public:

	/// 作成
    void create( lnFloat widht, lnFloat height, LNAxisAlignedDir frontDir = LN_AADIR_RZ );

	/// 大きさの設定
    void setSize( const LVector2& size_ );

	/// 大きさの取得
    const LVector2& getSize() const { return mSize; }

	/// テクスチャの設定
    void setTexture( Graphics::Texture* texture_ );

	/// テクスチャの取得
    Graphics::Texture* getTexture();

	/// テクスチャの転送矩形の設定
    void setSrcRect( const Geometry::Rect& rect_ );

	/// テクスチャの転送矩形の取得
    const Geometry::Rect& getSrcRect() const { return mSrcRect; }

	/// 反転方法の設定
    void setFlipFlag( lnU32 flags_ ) { mFlipFlags = flags_; _updateTexUV(); }

	/// 反転方法の取得
    lnU32 getFlipFlag() const { return mFlipFlags; }

public:

	/// ノードの種類の取得
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_SPRITE3D; }

private:

	/// テクスチャ座標の更新
    void _updateTexUV();

private:
    LNAxisAlignedDir            mAADir;
    LVector2                    mSize;
    Geometry::Rect				mSrcRect;
    lnU32                       mFlipFlags;

    LVector3                    mUpperLeft;
    LVector3                    mLowerRight;
    LVector2                    mUVUpperLeft;
    LVector2                    mUVLowerRight;

    bool                        mCalledSetSrcRect;
	
	class DrawingContext;
	friend class DrawingContext;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================