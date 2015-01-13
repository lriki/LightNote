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
// �� Effect
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

	/// �쐬
    void create( const lnChar* filePath );

	/// �G�t�F�N�g���Đ�����
	///		overlap=true  : �Đ����̃G�t�F�N�g���~�����ɐV�����G�t�F�N�g�̍Đ����J�n����
	///		overlap=false : �Đ����̃G�t�F�N�g���~���Ă���V�����G�t�F�N�g���Đ�����
	void play( bool overlap = true );

	/// �G�t�F�N�g���~����
	void stop();

	/// �Đ��������m�F
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