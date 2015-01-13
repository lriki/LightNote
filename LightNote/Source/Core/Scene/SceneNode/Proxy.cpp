//=============================================================================
//y Grid z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Graphics/Manager.h"
#include "../../Effect/Manager.h"
#include "../../Physics/Manager.h"
#include "../SceneGraph.h"
#include "../Pane.h"	// •K—v‚È‚­‚È‚é‚Í‚¸
#include "Camera.h"
#include "Proxy.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{

//=============================================================================
// ¡ EffectBatchProxy ƒNƒ‰ƒX
//=============================================================================
#if 0
	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    EffectBatchProxy::EffectBatchProxy( SceneGraph* scene_ )
        : SceneNode         ( scene_ )
        , mEffectManager    ( NULL )
    {
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    EffectBatchProxy::~EffectBatchProxy()
    {
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    void EffectBatchProxy::initialize( Core::Effect::Manager* effect_manager_, LNDrawCoordSpace cs_ )
    {
        mEffectManager = effect_manager_;
        SceneNode::initialize( cs_ );

        this->setShader( NULL );
        this->setPriority( LN_SCENENODEPRIORITY_EFFECT_BATCH_SPRITE );
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    LNRESULT EffectBatchProxy::drawSubset( lnU32 index_ )
    {
        if ( this->getDrawCoordSpace() == LN_DRAWCOORD_3D )
        {
            //mEffectManager->drawBatch3D();
        }
        else
        {
            //mEffectManager->drawBatch3D();
        }

        return LN_OK;
    }
#endif
//=============================================================================
// ¡ PhysicsDebugDrawProxy
//=============================================================================

	//---------------------------------------------------------------------
	// Context
	//---------------------------------------------------------------------
    class PhysicsDebugDrawProxy::DrawingContext
        : public SceneNodeContext
    {
    public:

        virtual void preRender( ContextDrawParam& param )
        {
            SceneNodeContext::preRender( param );

            ((PhysicsDebugDrawProxy*)this->mSceneNode)->mGeometryRenderer->setViewProjMatrix(
				param.CameraContext->getViewProjectionMatrix() );
        }
    };

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    PhysicsDebugDrawProxy::PhysicsDebugDrawProxy( SceneGraph* scene_ )
        : SceneNode         ( scene_ )
        , mGeometryRenderer ( NULL )
    {
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    void PhysicsDebugDrawProxy::initialize()
    {
        mGeometryRenderer = this->mSceneGraph->getGeometryRenderer();

        DrawingContext* context = LN_NEW DrawingContext();
        context->initialize( this );
        SceneNode::initialize( 1, LN_DRAWCOORD_3D, context );

        this->setShader( NULL );
        //this->setShader( this->mSceneGraph->getDefaultShader( LN_DEFSHADER_NOLIGHTING ) );
        this->setPriority( LN_SCENENODEPRIORITY_PHYSICS_DEBUG );

        this->setEnableDepthTest( true );
        this->setEnableDepthWrite( false );
    }

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    LNRESULT PhysicsDebugDrawProxy::drawSubset( lnU32 index_ )
    {
        mGeometryRenderer->setMatrix( LMatrix::IDENTITY );
        mGeometryRenderer->begin();
        this->mSceneGraph->getPhysicsManager()->drawDebugShape( mGeometryRenderer );
        mGeometryRenderer->end();

        return LN_OK;
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================