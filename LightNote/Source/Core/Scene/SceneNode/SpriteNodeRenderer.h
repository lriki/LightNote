//=============================================================================
//【 SpriteNodeRenderer 】
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
// ■ SpriteNodeRenderer
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
//	/// 作成
//    void create( VisualNode* ownerNode );
//
//public:
//
//	/// テクスチャの設定
//    void setTexture( Graphics::Texture* texture_ );
//
//	/// テクスチャの取得
//    Graphics::ITexture* getTexture();
//
//	/// テクスチャの転送矩形の設定
//    void setSrcRect( const Geometry::Rect& rect_ );
//
//	/// テクスチャの転送矩形の取得
//    const Geometry::Rect& getSrcRect() const { return mSrcRect; }
//
//	/// 反転方法の設定
//    void setFlipFlag( lnU32 flags_ ) { mFlipFlags = flags_; _updateTexUV(); }
//
//	/// 反転方法の取得
//    lnU32 getFlipFlag() const { return mFlipFlags; }
//
//protected:
//
//	virtual void drawSubset( int index );
//
//private:
//
//	/// テクスチャ座標の更新
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