//==============================================================================
// Layer 
//------------------------------------------------------------------------------
///**
//  @file       Layer.h
//  @brief      Layer
//*/
//==============================================================================

#pragma once

#include "SceneNode/SceneNode.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// LayerBase
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class LayerBase
	: public Base::ReferenceObject
{
	LN_TYPE_INFO_ACCESS_DECL;
public:
	LayerBase();
	virtual ~LayerBase() {}

public:

	/// フレーム更新
	virtual void updateFrame(float elapsedTime) {}

	/// 描画情報更新
	virtual void updateContext() {}

	/// 描画コンテキストの取得
	virtual LayerContextBase* getContext() = 0;

	/// 親 Pane のサイズ変更時に呼ばれる (新規 add 時にもコール)
	virtual void onUpdateViewSize(const LVector2& viewSize) {}

public:
	void	setParentPane(Pane* pane) { mParentPane = pane; }
	Pane*	getParentPane() { return mParentPane; }
	void	setPriority(int priority) { mPriority = priority; }
	int		getPriority() const { return mPriority; }

private:
	Pane*	mParentPane;
	int		mPriority;	///< 優先度。大きい方が手前 (後に描画される)


};

//==============================================================================
// LayerContextBase
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class LayerContextBase
	: public Base::ReferenceObject
{
public:
	LayerContextBase(LayerBase* owner) { mOwner = owner; }
	virtual ~LayerContextBase() {}

public:

	/// 描画情報更新
	virtual void updateContext() {}

	/// 前描画
	virtual void preRender(ContextDrawParam* param) {}

	/// 後描画
	virtual void postRender(ContextDrawParam* param) {}

protected:
	LayerBase*	mOwner;
};

//==============================================================================
// DrawingLayer
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class DrawingLayer
	: public LayerBase
{
public:
	DrawingLayer();
	virtual ~DrawingLayer();

public:

	/// 作成
	void create(Camera* camera);

public:
	virtual void				updateFrame(float elapsedTime);
	virtual void				updateContext();
	virtual LayerContextBase*	getContext();
	virtual void				onUpdateViewSize(const LVector2& viewSize);

private:
	class DrawingLayerContext;
	DrawingLayerContext*	mContext;

	Camera*					mCamera;
};


} // namespace Scene
} // namespace Core
} // namespace LNote
