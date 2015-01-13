//==============================================================================
// SpritePath 
//------------------------------------------------------------------------------
///**
//  @file       SpritePath.h
//  @brief      SpritePath
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
// SpritePath
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class SpritePath
    : public VisualNode
{
	LN_TYPE_INFO_ACCESS_DECL;
public:
    SpritePath( SceneGraph* scene );
    virtual ~SpritePath();

public:

	/// 作成
	void create(int maxSpriteCount, LNDrawCoordSpace coord);

	/// 以降の drawSprite() で使用するブレンドモードの設定
	void setPathBlendMode(Graphics::BlendMode blendMode);

	/// 以降の drawSprite() で使用する座標変換行列の設定
	void setPathTransform( const LMatrix& matrix );

	/// 描画リクエスト
    void drawSprite(
        const LVector3& position,
		const LVector3& center,
		const LVector2& size,
        Graphics::Texture* texture,
        const LRectF& srcRect,
        const LColorF* vertexColors,
		LNAxisAlignedDir front);

public:
	// SceneNode
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_BATCHSPRITE; }
	virtual void postFrameUpdate();

private:
	Graphics::SpriteRenderer*	mPrimaryRenderer;

    class DrawingContext;
    friend class DrawingContext;
};

} // namespace Scene
} // namespace Core
} // namespace LNote
