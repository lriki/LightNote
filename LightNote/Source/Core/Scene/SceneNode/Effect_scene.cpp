//==============================================================================
// Effect 
//==============================================================================

#include "stdafx.h"
#include "../../Graphics/Manager.h"
#include "../../Effect/Manager.h"
#include "../SceneGraph.h"
#include "Effect.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// Effect::DrawingContext
//==============================================================================
class Effect::DrawingContext
    : public VisualNodeContext
{
public:
	DrawingContext(){}
	virtual ~DrawingContext() {}

protected:

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    virtual void updateContext( SceneNodeContext* parentContext )
    {
        VisualNodeContext::updateContext( parentContext );

		Effect* ownerNode = static_cast<Effect*>( mSceneNode );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	virtual void preRender( ContextDrawParam& param )
	{
		VisualNodeContext::preRender( param );
		
		Effect* ownerNode = static_cast<Effect*>( mSceneNode );
		ownerNode->mEffect->setMatrix( mCombinedWorldMatrix );
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    virtual void drawSubset( ContextDrawParam& param, int index )
    {
		Effect* ownerNode = static_cast<Effect*>( mSceneNode );
		ownerNode->mEffect->draw();
    }

private:
};

//==============================================================================
// Effect
//==============================================================================

	LN_TYPE_INFO_ACCESS_IMPL(Effect);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Effect::Effect( SceneGraph* scene )
        : VisualNode	( scene )
		, mEffect		( NULL )
	{
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Effect::~Effect()
    {
		LN_SAFE_RELEASE( mEffect );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Effect::create( const lnChar* filePath )
    {
		Effect::DrawingContext* context = LN_NEW Effect::DrawingContext();
		context->initialize( this );
        VisualNode::initialize( 1, LN_DRAWCOORD_3D, context );

		mEffect = mSceneGraph->getEffectManager()->createEffect( filePath );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Effect::play( bool overlap )
    {
		mEffect->play( overlap );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Effect::stop()
    {
		mEffect->stop();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool Effect::isPlaying() const
    {
		return mEffect->isPlaying();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Effect::update( float elapsedTime )
    {
		mEffect->advanceTime( elapsedTime );
    }


} // namespace Scene
} // namespace Core
} // namespace LNote
