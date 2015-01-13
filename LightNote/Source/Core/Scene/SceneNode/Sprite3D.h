//==============================================================================
// Sprite3D 
//------------------------------------------------------------------------------
///**
//  @file       Sprite3D.h
//  @brief      Sprite3D
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
// �� Sprite3D
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class Sprite3D
    : public VisualNode
{
public:
    Sprite3D( SceneGraph* scene );
    virtual ~Sprite3D();

public:

	/// �쐬
    void create( lnFloat widht, lnFloat height, LNAxisAlignedDir frontDir = LN_AADIR_RZ );

	/// �傫���̐ݒ�
    void setSize( const LVector2& size_ );

	/// �傫���̎擾
    const LVector2& getSize() const { return mSize; }

	/// �e�N�X�`���̐ݒ�
    void setTexture( Graphics::Texture* texture_ );

	/// �e�N�X�`���̎擾
    Graphics::Texture* getTexture();

	/// �e�N�X�`���̓]����`�̐ݒ�
    void setSrcRect( const Geometry::Rect& rect_ );

	/// �e�N�X�`���̓]����`�̎擾
    const Geometry::Rect& getSrcRect() const { return mSrcRect; }

	/// ���]���@�̐ݒ�
    void setFlipFlag( lnU32 flags_ ) { mFlipFlags = flags_; _updateTexUV(); }

	/// ���]���@�̎擾
    lnU32 getFlipFlag() const { return mFlipFlags; }

public:

	/// �m�[�h�̎�ނ̎擾
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_SPRITE3D; }

private:

	/// �e�N�X�`�����W�̍X�V
    void _updateTexUV();

private:
    LNAxisAlignedDir            mAADir;
    LVector2                    mSize;
    Geometry::Rect				mSrcRect;
    lnU32                       mFlipFlags;

    LVector3                    mUpperLeft;
    LVector3                    mLowerRight;
    LVector2                    mUVUpperLeft;
    LVector2                    mUVLowerRight;

    bool                        mCalledSetSrcRect;
	
	class DrawingContext;
	friend class DrawingContext;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================