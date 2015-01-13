//==============================================================================
// PointParticle 
//------------------------------------------------------------------------------
///**
//  @file       PointParticle.h
//  @brief      PointParticle
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "VisualNode.h"
#include "../../Graphics/Common/PointParticle.h"
#include "../../Graphics/Common/PointParticleParameter.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// ■ PointParticle
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class PointParticle
    : public VisualNode
{
public:
    PointParticle( SceneGraph* scene );
    virtual ~PointParticle();

public:

	/// 作成
    void create( const Graphics::PointParticleParameter& param );

	/// テクスチャの設定
    void setTexture( Graphics::Texture* texture );

	/// テクスチャの取得
	Graphics::Texture* getTexture();

public:
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_PARTICLE; }
	virtual void update(float elapsedTime);//const Game::GameTime& time );

private:
	lnFloat mElapsedGameTime;

	class DrawingContext;
	friend class DrawingContext;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================