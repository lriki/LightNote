//==============================================================================
// BatchSprite 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Graphics/Common/SpriteRenderer.h"
#include "../../Graphics/Manager.h"
#include "../SceneGraph.h"
#include "Util.h"
#include "Proxy.h"
#include "BatchSprite.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// ■ BatchSprite3D::BatchSprite3DContext
//==============================================================================
class BatchSprite3D::BatchSprite3DContext
    : public VisualNodeContext
{
public:

    BatchSprite3DContext()
        : mTexture      ( NULL )
        , mFlipFlags    ( LN_FLIP_NONE )
    {}

	virtual ~BatchSprite3DContext() {}

private:

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    virtual void updateContext( SceneNodeContext* parentContext )
    {
        VisualNodeContext::updateContext( parentContext );

        BatchSprite3D* t	= (BatchSprite3D*)this->mSceneNode;
		this->mAADir		= t->mAADir;
        this->mSize			= t->mSize;
        this->mTexture		= t->mTexture;
        LN_SAFE_ADDREF( this->mTexture );
        this->mSrcRect		= t->mSrcRect;
        this->mFlipFlags	= t->mFlipFlags;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    virtual void releaseAllResource()
    {
        VisualNodeContext::releaseAllResource();
        LN_SAFE_RELEASE( mTexture );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	virtual void render( ContextDrawParam& param )
	{
		if ( mEnableDrawing &&
			 mTexture &&
			 param.SpriteRenderer &&
			 !param.OffscreenScene )
		{
			param.SpriteRenderer->setTransform(
				getCombinedWorldMatrix() );

			param.SpriteRenderer->setRenderState( mRenderState );


			const LColorF& cs = getSubsetRenderParam( 0 ).ColorScale;
			LColorF c(
				cs.R,
				cs.G,
				cs.B,
				cs.A * mSubsetRenderParamArray[0].Opacity );
			lnU32 color = c.to32Bit( mSceneNode->getSceneGraph()->getGraphicsManager()->getGraphicsDevice()->getGraphicsAPI() );
			lnU32 color_table[ 4 ] = { color, color, color, color };
            
			param.SpriteRenderer->drawRequest3D(
				LVector3( 0, 0, 0 ),//this->mPosition,
				LVector3( 0, 0, 0 ),//this->mCenter,
				this->mSize,
				this->mTexture,
				this->mSrcRect,
				color_table,
				this->mAADir );
		}

		// 子の処理
		SceneNodeContext::render( param );
	}

private:
	LNAxisAlignedDir    mAADir;
    LVector2            mSize;
    Graphics::Texture* mTexture;
    LRectF		mSrcRect;
    lnU8                mFlipFlags;
};

//==============================================================================
// ■ BatchSprite3D
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    BatchSprite3D::BatchSprite3D( SceneGraph* scene )
        : VisualNode         ( scene )
        , mAADir            ( LN_AADIR_RZ )
        , mTexture          ( NULL )
        , mFlipFlags        ( LN_FLIP_NONE )
        , mCalledSetSrcRect ( false )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    BatchSprite3D::~BatchSprite3D()
    {
        LN_SAFE_RELEASE( mTexture );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void BatchSprite3D::create( lnFloat w_size, lnFloat h_size, LNAxisAlignedDir dir )
    {
        BatchSprite3DContext* c = LN_NEW BatchSprite3DContext();
        c->initialize( this );
        VisualNode::initialize( 1, LN_DRAWCOORD_3D, c );
        mAADir = dir;
        mSize.set( w_size, h_size );
        this->setShader( NULL );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void BatchSprite3D::setTexture( Graphics::Texture* texture )
    {
		LN_REFOBJ_SET( mTexture, texture );
        
        if ( mTexture )
        {
            // 一度も setSrcRect() が呼ばれていなければ、テクスチャ全体を転送するようにする
            if ( !mCalledSetSrcRect )
            {
                mSrcRect.set(
                    0, 0,
                    mTexture->getSize().x,
                    mTexture->getSize().y );
                mCalledSetSrcRect = true;
            }
        }
        else
        {
            mCalledSetSrcRect = false;
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void BatchSprite3D::setSrcRect( const LRectF& rect )
    {
        mSrcRect = rect;
        mCalledSetSrcRect = true;
    }


//==============================================================================
// ■ BatchSprite::BatchSpriteContext
//==============================================================================
class BatchSprite::BatchSpriteContext
    : public VisualNodeContext
{
public:
    BatchSpriteContext()
        : mTexture      ( NULL )
        , mFlipFlags    ( LN_FLIP_NONE )
    {}

	virtual ~BatchSpriteContext() {}

public:

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    virtual void updateContext( SceneNodeContext* parent_context_ )
    {
        VisualNodeContext::updateContext( parent_context_ );

        BatchSprite* t = (BatchSprite*)this->mSceneNode;
        mTexture = t->mTexture;
        LN_SAFE_ADDREF( mTexture );
        mSrcRect	= t->mSrcRect;
        mFlipFlags	= t->mFlipFlags;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    virtual void releaseAllResource()
    {
        VisualNodeContext::releaseAllResource();
        LN_SAFE_RELEASE( mTexture );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	virtual void render( ContextDrawParam& param )
	{
		if ( mEnableDrawing &&
			 this->mTexture && 
			 param.SpriteRenderer &&
			 !param.OffscreenScene )
		{
			param.SpriteRenderer->setTransform( getCombinedWorldMatrix() );
			param.SpriteRenderer->setRenderState( mRenderState );
			param.SpriteRenderer->drawRequest2D(
				LVector3( 0, 0, 0 ),
				LVector3( 0, 0, 0 ),
				this->mTexture,
				this->mSrcRect,
				getSubsetRenderParam( 0 ).ColorScale.to32Bit( mSceneNode->getSceneGraph()->getGraphicsManager()->getGraphicsDevice()->getGraphicsAPI() ) );
		}

		// 子の処理
		SceneNodeContext::render( param );
	}

private:
    Graphics::Texture* mTexture;
    LRectF		mSrcRect;
    lnU8                mFlipFlags;
};

//==============================================================================
// ■ BatchSprite
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    BatchSprite::BatchSprite( SceneGraph* scene )
        : VisualNode         ( scene )
        , mTexture          ( NULL )
        , mFlipFlags        ( LN_FLIP_NONE )
        , mCalledSetSrcRect ( false )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    BatchSprite::~BatchSprite()
    {
        LN_SAFE_RELEASE( mTexture );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void BatchSprite::create()
    {
        BatchSpriteContext* c = LN_NEW BatchSpriteContext();
        c->initialize( this );
        VisualNode::initialize( 1, LN_DRAWCOORD_2D, c );
        this->setShader( NULL );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void BatchSprite::setTexture( Graphics::Texture* texture )
    {
        LN_REFOBJ_SET( mTexture, texture );
        
        if ( mTexture )
        {
            // 一度も setSrcRect() が呼ばれていなければ、テクスチャ全体を転送するようにする
            if ( !mCalledSetSrcRect )
            {
                mSrcRect.set(
                    0, 0,
                    mTexture->getSize().x,
                    mTexture->getSize().y);
                mCalledSetSrcRect = true;
            }
        }
        else
        {
            mCalledSetSrcRect = false;
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void BatchSprite::setSrcRect( const LRectF& rect )
    {
        mSrcRect = rect;
        mCalledSetSrcRect = true;
    }

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================