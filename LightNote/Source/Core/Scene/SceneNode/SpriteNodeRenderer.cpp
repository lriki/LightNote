//=============================================================================
//Åy SpriteNodeRenderer Åz
//-----------------------------------------------------------------------------
///**
//  @file       SpriteNodeRenderer.h
//  @brief      SpriteNodeRenderer
//  @author     Riki
//*/
//=============================================================================

#include "stdafx.h"
#include "../../Graphics/Manager.h"
#include "../../Graphics/Common/GeometryRenderer.h"
#include "../SceneShader/SceneShader.h"
#include "../SceneGraph.h"
#include "Util.h"
#include "SceneNode.h"
#include "SpriteNodeRenderer.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//=============================================================================
// Å° SpriteNodeRenderer
//=============================================================================

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    SpriteNodeRenderer::SpriteNodeRenderer()
		: mRenderer         ( NULL )
        , mFlipFlags        ( LN_FLIP_NONE )
        , mCalledSetSrcRect ( false )
	{
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    SpriteNodeRenderer::~SpriteNodeRenderer()
	{
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    void SpriteNodeRenderer::create( VisualNode* ownerNode )
	{
		VisualNodeRenderer::create( ownerNode, 1 );

		mRenderer = mOwnerNode->getSceneGraph()->getGraphicsManager()->getGeometryRenderer();

	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    void SpriteNodeRenderer::setTexture( Graphics::ITexture* texture_ )
    {
        this->mNodeRenderParam.Subset[ 0 ].Material.setTexture( texture_ );

        if ( texture_ )
        {
            // àÍìxÇ‡ setSrcRect() Ç™åƒÇŒÇÍÇƒÇ¢Ç»ÇØÇÍÇŒÅAÉeÉNÉXÉ`ÉÉëSëÃÇì]ëóÇ∑ÇÈÇÊÇ§Ç…Ç∑ÇÈ
            if ( !mCalledSetSrcRect )
            {
                setSrcRect(
                    Geometry::Rect(
                        0, 0,
                        static_cast< int >( texture_->getSize().x ),
                        static_cast< int >( texture_->getSize().y ) ) );
            }
        }
        else
        {
            mCalledSetSrcRect = false;
        }

        _updateTexUV();
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    Graphics::ITexture* SpriteNodeRenderer::getTexture()
    {
        return this->mNodeRenderParam.Subset[ 0 ].Material.getTexture();
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    void SpriteNodeRenderer::setSrcRect( const Geometry::Rect& rect_ )
    {
        mSrcRect = rect_;
        mCalledSetSrcRect = true;
        _updateTexUV();

        lnFloat r = (lnFloat)mSrcRect.w;
        lnFloat b = (lnFloat)mSrcRect.h;
        lnFloat l = 0;
        lnFloat t = 0;

        mUpperLeft.set(  l, t, 0 );
        mLowerRight.set( r, b, 0 );
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    void SpriteNodeRenderer::drawSubset( int index )
    {
        //return mRenderer->drawTransformAxis(1);_

        //this->mSceneGraph->getGraphicsManager()->getGraphicsDevice()->getRenderer()
        //->clear(true,true,0xff0000ff,1.0f);
        
        mRenderer->drawSquare(
            mUpperLeft.x,   mUpperLeft.y,  0xffffffff, mUVUpperLeft.x,  mUVUpperLeft.y,
            mLowerRight.x,  mUpperLeft.y,  0xffffffff, mUVLowerRight.x, mUVUpperLeft.y,
            mUpperLeft.x,   mLowerRight.y, 0xffffffff, mUVUpperLeft.x,  mUVLowerRight.y,
            mLowerRight.x,  mLowerRight.y, 0xffffffff, mUVLowerRight.x, mUVLowerRight.y );
/*
        return mRenderer->drawSquare(
            mUpperLeft.x,   mUpperLeft.y,  0xffffffff, mUVUpperLeft.x,  mUVUpperLeft.y,
            mUpperLeft.x,   mLowerRight.y, 0xffffffff, mUVUpperLeft.x,  mUVLowerRight.y,
            mLowerRight.x,  mUpperLeft.y,  0xffffffff, mUVLowerRight.x, mUVUpperLeft.y,
            mLowerRight.x,  mLowerRight.y, 0xffffffff, mUVLowerRight.x, mUVLowerRight.y );
            */
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    void SpriteNodeRenderer::_updateTexUV()
    {
        Graphics::ITexture* tex = getTexture();

        if ( tex )
        {
            lnFloat l, t, r, b;

            Util::normalizeSrcRect( &l, &t, &r, &b, mSrcRect, tex->getRealSize() );

            // êÇíºîΩì]
            if ( mFlipFlags & LN_FLIP_V )
            {
                mUVUpperLeft.y = b;
                mUVLowerRight.y = t;
            }
            else
            {
                mUVUpperLeft.y = t;
                mUVLowerRight.y = b;
            }
            // êÖïΩîΩì]
            if ( mFlipFlags & LN_FLIP_H )
            {
                mUVUpperLeft.x = r;
                mUVLowerRight.x = l;
            }
            else
            {
                mUVUpperLeft.x = l;
                mUVLowerRight.x = r;
            }
        }
        else
        {
        }
    }

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================