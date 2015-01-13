//==============================================================================
// Sprite 
//------------------------------------------------------------------------------
///**
//  @file       Sprite.h
//  @brief      Sprite
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
// Sprite
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class Sprite
    : public VisualNode
{
	LN_TYPE_INFO_ACCESS_DECL;
public:
    Sprite( SceneGraph* scene );
    virtual ~Sprite();

public:

	/// 作成
    void create();

	/// 作成
    void create3D( /*lnFloat widht, lnFloat height,*/ LNAxisAlignedDir frontDir = LN_AADIR_RZ );

	/// テクスチャの設定
    void setTexture( Graphics::Texture* texture );

	/// テクスチャの取得
    Graphics::Texture* getTexture();

	/// サイズの設定
	void setSize( const LVector2& size ) { mSize = size; _updateVertexData(); }

	/// テクスチャの転送矩形の設定
    void setSrcRect( const LRectF& rect );

	/// テクスチャの転送矩形の取得
    const LRectF& getSrcRect() const { return mSrcRect; }

	/// 反転方法の設定
    void setFlipFlag( lnU32 flags ) { mFlipFlags = flags; _updateTexUV(); }

	/// 反転方法の取得
    lnU32 getFlipFlag() const { return mFlipFlags; }

	/// 3Dスプライトとして作成されているか
	bool is3DSprite() const { return (mDrawCoordSpace == LN_DRAWCOORD_3D); }

public:
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_SPRITE3D; }

private:
    void _updateTexUV();
    void _updateVertexData();

private:

    LRectF						mSrcRect;
    lnU32                       mFlipFlags;

    LVector3                    mUpperLeft;
    LVector3                    mLowerRight;
    LVector2                    mUVUpperLeft;
    LVector2                    mUVLowerRight;
	
    LNAxisAlignedDir            m3DAADir;
    LVector2                    mSize;

	class DrawingContext;
	friend class DrawingContext;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================