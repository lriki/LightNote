//=============================================================================
//�y SpriteNodeRenderer �z
//-----------------------------------------------------------------------------
///**
//  @file       SpriteNodeRenderer.h
//  @brief      SpriteNodeRenderer
//  @author     Riki
//*/
//=============================================================================

#pragma once

#include "VisualNodeRenderer.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//=============================================================================
// �� SpriteNodeRenderer
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
//class SpriteNodeRenderer
//    : public VisualNodeRenderer
//{
//public:
//    SpriteNodeRenderer();
//    virtual ~SpriteNodeRenderer();
//
//public:
//
//	/// �쐬
//    void create( VisualNode* ownerNode );
//
//public:
//
//	/// �e�N�X�`���̐ݒ�
//    void setTexture( Graphics::Texture* texture_ );
//
//	/// �e�N�X�`���̎擾
//    Graphics::ITexture* getTexture();
//
//	/// �e�N�X�`���̓]����`�̐ݒ�
//    void setSrcRect( const Geometry::Rect& rect_ );
//
//	/// �e�N�X�`���̓]����`�̎擾
//    const Geometry::Rect& getSrcRect() const { return mSrcRect; }
//
//	/// ���]���@�̐ݒ�
//    void setFlipFlag( lnU32 flags_ ) { mFlipFlags = flags_; _updateTexUV(); }
//
//	/// ���]���@�̎擾
//    lnU32 getFlipFlag() const { return mFlipFlags; }
//
//protected:
//
//	virtual void drawSubset( int index );
//
//private:
//
//	/// �e�N�X�`�����W�̍X�V
//    void _updateTexUV();
//
//private:
//
//    Graphics::GeometryRenderer* mRenderer;
//	//LNAxisAlignedDir            mAADir;
//	//LVector2                    mSize;
//    Geometry::Rect				mSrcRect;
//    lnU32                       mFlipFlags;
//
//    LVector3                    mUpperLeft;
//    LVector3                    mLowerRight;
//    LVector2                    mUVUpperLeft;
//    LVector2                    mUVLowerRight;
//
//    bool                        mCalledSetSrcRect;
//	
//};

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================