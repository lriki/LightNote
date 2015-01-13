//==============================================================================
// PointParticle 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Graphics/Common/PointParticle.h"
#include "../../Graphics/Common/GeometryRenderer.h"
#include "../SceneGraph.h"
#include "Camera.h"
#include "PointParticle.h"

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
// ¡ PointParticle::DrawingContext
//==============================================================================
class PointParticle::DrawingContext
    : public VisualNodeContext
{
public:
	DrawingContext()
		: mPointParticle	( NULL )
		, mElapsedGameTime	( 0 )
	{}
	virtual ~DrawingContext()
	{
        LN_SAFE_RELEASE( mPointParticle );
	}

public:

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void create( SceneNode* node, const Graphics::PointParticleParameter& param )
	{
		VisualNodeContext::initialize( node );

		mPointParticle = LN_NEW Graphics::PointParticle();
        mPointParticle->initialize(
            node->getSceneGraph()->getGraphicsManager()->getGraphicsDevice(),
            param );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    virtual void updateContext( SceneNodeContext* parentContext )
    {
        VisualNodeContext::updateContext( parentContext );

		PointParticle* ownerNode = static_cast<PointParticle*>( mSceneNode );
		mElapsedGameTime = ownerNode->mElapsedGameTime;
		ownerNode->mElapsedGameTime = 0;

		mPointParticle->setTexture(ownerNode->getTexture());
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    virtual void drawSubset( ContextDrawParam& param, int index )
	{
		mPointParticle->advanceTime( mElapsedGameTime );
		mPointParticle->update( getCombinedWorldMatrix() );

		param.GeometryRenderer->drawPointParticle(
			getCombinedWorldMatrix(),
			getCameraContext()->getProjectionMatrix(),
			getCameraContext()->getViewProjectionMatrix(),
			getCameraContext()->getPosition(),
            mPointParticle );
    }

private:
    Graphics::PointParticle*    mPointParticle;
	lnFloat						mElapsedGameTime;
};


//==============================================================================
// ¡ PointParticle
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    PointParticle::PointParticle( SceneGraph* scene )
        : VisualNode        ( scene )
		, mElapsedGameTime	( 0 )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    PointParticle::~PointParticle()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void PointParticle::create( const Graphics::PointParticleParameter& param )
    {
		PointParticle::DrawingContext* context = LN_NEW PointParticle::DrawingContext();
		context->create( this, param );
        VisualNode::initialize( 1, LN_DRAWCOORD_3D, context );

        

        this->setShader( NULL );

        this->mNodeRenderParam.RenderState.PointSprite = true;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void PointParticle::setTexture( Graphics::Texture* texture )
    {
        this->mNodeRenderParam.Subset[0].Material.setTexture( texture );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Graphics::Texture* PointParticle::getTexture()
	{
		return this->mNodeRenderParam.Subset[0].Material.getTexture();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void PointParticle::update(float elapsedTime) //const Game::GameTime& time_ )
    {
		mElapsedGameTime += elapsedTime;//time_.getElapsedGameTime();
    }

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================