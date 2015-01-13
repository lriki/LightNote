//==============================================================================
// Effect 
//------------------------------------------------------------------------------
///**
//  @file       Effect.h
//  @brief      Effect
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../Effect/Interface.h"
#include "VisualNode.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// ■ Effect
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class Effect
    : public VisualNode
{
	LN_TYPE_INFO_ACCESS_DECL;
public:
    Effect( SceneGraph* scene );
    virtual ~Effect();

public:

	/// 作成
    void create( const lnChar* filePath );

	/// エフェクトを再生する
	///		overlap=true  : 再生中のエフェクトを停止せずに新しいエフェクトの再生を開始する
	///		overlap=false : 再生中のエフェクトを停止してから新しいエフェクトを再生する
	void play( bool overlap = true );

	/// エフェクトを停止する
	void stop();

	/// 再生中かを確認
	bool isPlaying() const;

public:
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_GRID; }
    virtual void update( float elapsedTime );

private:
	Core::Effect::Effect*	mEffect;
	
	class DrawingContext;
	friend class DrawingContext;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================