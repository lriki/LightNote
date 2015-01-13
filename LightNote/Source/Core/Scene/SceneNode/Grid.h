//=============================================================================
// Grid 
//-----------------------------------------------------------------------------
///**
//  @file       Grid.h
//  @brief      Grid
//*/
//=============================================================================

#pragma once

#include "../../Graphics/Common/Grid.h"
#include "SceneNode.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//=============================================================================
// Grid
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class Grid
    : public SceneNode
{
public:
    Grid( SceneGraph* scene );
    virtual ~Grid();

public:

	/// 作成
    void create();

public:

	/// ノードの種類の取得
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_GRID; }

	/// サブセットを描画する
	//virtual LNRESULT drawSubset( lnU32 index_ );

private:

	class DrawingContext;
	friend class DrawingContext;
};

} // namespace Scene
} // namespace Core
} // namespace LNote
