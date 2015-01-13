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

	/// �t���[���X�V
	virtual void updateFrame(float elapsedTime) {}

	/// �`����X�V
	virtual void updateContext() {}

	/// �`��R���e�L�X�g�̎擾
	virtual LayerContextBase* getContext() = 0;

	/// �e Pane �̃T�C�Y�ύX���ɌĂ΂�� (�V�K add ���ɂ��R�[��)
	virtual void onUpdateViewSize(const LVector2& viewSize) {}

public:
	void	setParentPane(Pane* pane) { mParentPane = pane; }
	Pane*	getParentPane() { return mParentPane; }
	void	setPriority(int priority) { mPriority = priority; }
	int		getPriority() const { return mPriority; }

private:
	Pane*	mParentPane;
	int		mPriority;	///< �D��x�B�傫��������O (��ɕ`�悳���)


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

	/// �`����X�V
	virtual void updateContext() {}

	/// �O�`��
	virtual void preRender(ContextDrawParam* param) {}

	/// ��`��
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

	/// �쐬
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
