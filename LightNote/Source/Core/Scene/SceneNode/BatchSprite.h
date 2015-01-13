//==============================================================================
// BatchSprite 
//------------------------------------------------------------------------------
///**
//  @file       BatchSprite.h
//  @brief      BatchSprite
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
// �� BatchSprite3D
//------------------------------------------------------------------------------
///**
//  @brief		�o�b�`�`��3D�X�v���C�g (���V�F�[�_�g�p�s��)
//	@note
//				�|�X�g�G�t�F�N�g�ɂ͕`�悳��邪�A�I�t�X�N���[���V�[���ɂ͕`�悳��Ȃ��B
//*/
//==============================================================================
class BatchSprite3D
    : public VisualNode
{
public:
    BatchSprite3D( SceneGraph* scene );
    virtual ~BatchSprite3D();

public:

	/// ������
    void create( lnFloat w_size, lnFloat h_size, LNAxisAlignedDir dir = LN_AADIR_RZ );

	/// �傫���̐ݒ�
    void setSize( const LVector2& size ) { mSize = size; }

	/// �傫���̎擾
    const LVector2& getSize() const { return mSize; }

	/// �e�N�X�`���̐ݒ�
    void setTexture( Graphics::Texture* texture );

	/// �e�N�X�`���̎擾
    Graphics::Texture* getTexture() { return mTexture; }

	/// �e�N�X�`���̓]����`�̐ݒ�
    void setSrcRect( const LRectF& rect );

	/// �e�N�X�`���̓]����`�̎擾
    const LRectF& getSrcRect() const { return mSrcRect; }

	/// ���]���@�̐ݒ�
    void setFlipFlag( lnU8 flags ) { mFlipFlags = flags; }

	/// ���]���@�̎擾
    lnU8 getFlipFlag() const { return mFlipFlags; }

public:
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_BATCHSPRITE3D; }

private:
    LNAxisAlignedDir    mAADir;
    LVector2            mSize;
    Graphics::Texture* mTexture;
    LRectF		mSrcRect;
    lnU8                mFlipFlags;
    bool                mCalledSetSrcRect;

    class BatchSprite3DContext;
    friend class BatchSprite3DContext;
};


//==============================================================================
// �� BatchSprite
//------------------------------------------------------------------------------
///**
//  @brief		�o�b�`�`��2D�X�v���C�g (���V�F�[�_�g�p�s��)
//*/
//==============================================================================
class BatchSprite
    : public VisualNode
{
public:
    BatchSprite( SceneGraph* scene );
    virtual ~BatchSprite();

public:

	/// �쐬
    void create();

	/// �e�N�X�`���̐ݒ�
    void setTexture( Graphics::Texture* texture );

	/// �e�N�X�`���̎擾
    Graphics::Texture* getTexture() { return mTexture; }

	/// �e�N�X�`���̓]����`�̐ݒ�
    void setSrcRect( const LRectF& rect );

	/// �e�N�X�`���̓]����`�̎擾
    const LRectF& getSrcRect() const { return mSrcRect; }

	/// ���]���@�̐ݒ�
    void setFlipFlag( lnU8 flags ) { mFlipFlags = flags; }

	/// ���]���@�̎擾
    lnU8 getFlipFlag() const { return mFlipFlags; }

public:
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_BATCHSPRITE; }

private:
    Graphics::Texture* mTexture;
    LRectF		mSrcRect;
    lnU8                mFlipFlags;
    bool                mCalledSetSrcRect;

    class BatchSpriteContext;
    friend class BatchSpriteContext;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================