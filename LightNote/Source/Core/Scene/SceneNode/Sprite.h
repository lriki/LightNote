//==============================================================================
// Sprite 
//------------------------------------------------------------------------------
///**
//  @file       Sprite.h
//  @brief      Sprite
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
// Sprite
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class Sprite
    : public VisualNode
{
	LN_TYPE_INFO_ACCESS_DECL;
public:
    Sprite( SceneGraph* scene );
    virtual ~Sprite();

public:

	/// �쐬
    void create();

	/// �쐬
    void create3D( /*lnFloat widht, lnFloat height,*/ LNAxisAlignedDir frontDir = LN_AADIR_RZ );

	/// �e�N�X�`���̐ݒ�
    void setTexture( Graphics::Texture* texture );

	/// �e�N�X�`���̎擾
    Graphics::Texture* getTexture();

	/// �T�C�Y�̐ݒ�
	void setSize( const LVector2& size ) { mSize = size; _updateVertexData(); }

	/// �e�N�X�`���̓]����`�̐ݒ�
    void setSrcRect( const LRectF& rect );

	/// �e�N�X�`���̓]����`�̎擾
    const LRectF& getSrcRect() const { return mSrcRect; }

	/// ���]���@�̐ݒ�
    void setFlipFlag( lnU32 flags ) { mFlipFlags = flags; _updateTexUV(); }

	/// ���]���@�̎擾
    lnU32 getFlipFlag() const { return mFlipFlags; }

	/// 3D�X�v���C�g�Ƃ��č쐬����Ă��邩
	bool is3DSprite() const { return (mDrawCoordSpace == LN_DRAWCOORD_3D); }

public:
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_SPRITE3D; }

private:
    void _updateTexUV();
    void _updateVertexData();

private:

    LRectF						mSrcRect;
    lnU32                       mFlipFlags;

    LVector3                    mUpperLeft;
    LVector3                    mLowerRight;
    LVector2                    mUVUpperLeft;
    LVector2                    mUVLowerRight;
	
    LNAxisAlignedDir            m3DAADir;
    LVector2                    mSize;

	class DrawingContext;
	friend class DrawingContext;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================