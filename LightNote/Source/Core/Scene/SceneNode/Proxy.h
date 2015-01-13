//=============================================================================
//�y Proxy �z
//-----------------------------------------------------------------------------
///**
//  @file       Proxy.h
//  @brief      Proxy
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Graphics/Common/Grid.h"
#include "SceneNode.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{
#if 0
//=============================================================================
// �� EffectBatchProxy �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�G�t�F�N�g�̃X�v���C�g���A�o�b�`�`����s���N���X
//*/
//=============================================================================
class EffectBatchProxy
    : public SceneNode
{
public:

    EffectBatchProxy( SceneGraph* scene_ );
    virtual ~EffectBatchProxy();

public:

	/// ������
    void initialize( Core::Effect::Manager* effect_manager_, LNDrawCoordSpace cs_ );

public:

	/// �m�[�h�̎�ނ̎擾
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_EFFECTSPRITEPROXY; }

	/// �T�u�Z�b�g��`�悷��
    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    Core::Effect::Manager*    mEffectManager;
};

//=============================================================================
// �� PhysicsDebugDrawProxy
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class PhysicsDebugDrawProxy
    : public SceneNode
{
public:

    PhysicsDebugDrawProxy( SceneGraph* scene_ );

    virtual ~PhysicsDebugDrawProxy() {}

public:

    void initialize();

public:

    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_PHYSICSDEBUGDRAWPROXY; }

    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    Graphics::GeometryRenderer* mGeometryRenderer;

    class DrawingContext;
    friend class DrawingContext;
};
#endif
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//
//=============================================================================