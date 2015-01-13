//==============================================================================
// BatchSprite 
//==============================================================================

#include "stdafx.h"
#include "../../Graphics/Common/SpriteRenderer.h"
#include "../SceneGraph.h"
#include "Camera.h"
#include "SpritePath.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// SpritePath::DrawingContext
//==============================================================================
class SpritePath::DrawingContext
    : public VisualNodeContext
{
public:
    DrawingContext()
        : mSecondaryRenderer	( NULL )
    {}

	virtual ~DrawingContext()
	{
		LN_SAFE_RELEASE( mSecondaryRenderer );
	}

public:

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void create( SceneNode* node, int maxSpriteCount )
	{
		VisualNodeContext::initialize( node );
		mSecondaryRenderer = LN_NEW Graphics::SpriteRenderer( node->getSceneGraph()->getGraphicsManager() );
		mSecondaryRenderer->initialize( node->getSceneGraph()->getGraphicsManager()->getGraphicsDevice(), maxSpriteCount, false );
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    virtual void updateContext( SceneNodeContext* parentContext )
    {
        VisualNodeContext::updateContext( parentContext );

        SpritePath* ownerNode = (SpritePath*)this->mSceneNode;

		// “ü‚ê‘Ö‚¦
        Graphics::SpriteRenderer* t = mSecondaryRenderer;
		mSecondaryRenderer = ownerNode->mPrimaryRenderer;
		ownerNode->mPrimaryRenderer = t;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    virtual void releaseAllResource()
    {
        VisualNodeContext::releaseAllResource();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	virtual void render( ContextDrawParam& param )
	{
		if ( mEnableDrawing )
		{
			mSecondaryRenderer->setViewProjMatrix(
				param.CameraContext->getViewMatrix(),
                param.CameraContext->getProjectionMatrix(),
				param.CameraContext->getViewSize() );

			mSecondaryRenderer->flash();
		}
		else {
			mSecondaryRenderer->clear();
		}

		// Žq‚Ìˆ—
		SceneNodeContext::render( param );
	}

private:
	Graphics::SpriteRenderer*	mSecondaryRenderer;
};

//==============================================================================
// SpritePath
//==============================================================================

	LN_TYPE_INFO_ACCESS_IMPL(SpritePath);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    SpritePath::SpritePath( SceneGraph* scene )
        : VisualNode			( scene )
		, mPrimaryRenderer		( NULL )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    SpritePath::~SpritePath()
    {
		LN_SAFE_RELEASE( mPrimaryRenderer );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SpritePath::create(int maxSpriteCount, LNDrawCoordSpace coord)
    {
		mPrimaryRenderer = LN_NEW Graphics::SpriteRenderer( mSceneGraph->getGraphicsManager() );
		mPrimaryRenderer->initialize( 
			mSceneGraph->getGraphicsManager()->getGraphicsDevice(), 
			maxSpriteCount, 
			(coord == LN_DRAWCOORD_3D));

        DrawingContext* c = LN_NEW DrawingContext();
        c->create( this, maxSpriteCount );
		VisualNode::initialize(1, coord, c);
        this->setShader( NULL );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SpritePath::setPathBlendMode(Graphics::BlendMode blendMode)
	{
		Graphics::LRenderState state = mNodeRenderParam.RenderState;
		state.Blend = blendMode;
		mPrimaryRenderer->setRenderState( state );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SpritePath::setPathTransform( const LMatrix& matrix )
	{
		mPrimaryRenderer->setTransform( matrix );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SpritePath::drawSprite(
		const LVector3& position,
		const LVector3& center,
		const LVector2& size,
		Graphics::Texture* texture,
		const LRectF& srcRect,
		const LColorF* vertexColors,
		LNAxisAlignedDir front)
	{
		lnU32 colorTable[4];
		if (vertexColors != NULL)
		{
			colorTable[0] = vertexColors[0].to32Bit();
			colorTable[1] = vertexColors[1].to32Bit();
			colorTable[2] = vertexColors[2].to32Bit();
			colorTable[3] = vertexColors[3].to32Bit();
		}
		else
		{
			colorTable[0] = 0xFFFFFFFF;
			colorTable[1] = 0xFFFFFFFF;
			colorTable[2] = 0xFFFFFFFF;
			colorTable[3] = 0xFFFFFFFF;
		}
		mPrimaryRenderer->drawRequest3D(
			position,
			center,
			size,
			texture,
			srcRect,
			colorTable,
			front);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SpritePath::postFrameUpdate()
	{
		mPrimaryRenderer->clear();
	}

} // namespace Scene
} // namespace Core
} // namespace LNote
