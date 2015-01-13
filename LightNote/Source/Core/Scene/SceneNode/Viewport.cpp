//==============================================================================
// Viewport 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Base/STLUtils.h"
#include "../../Resource/LNResource.h"
#include "../../Graphics/Common/SpriteRenderer.h"
#include "../SceneGraph.h"
#include "../SceneShader/SceneShader.h"
#include "../SceneShader/SceneShaderTechnique.h"
#include "Camera.h"
#include "Viewport.h"

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
// ■ Viewport クラス
//==============================================================================

	//----------------------------------------------------------------------
	// ● コンストラクタ
	//----------------------------------------------------------------------
    Viewport::Viewport( SceneGraph* scene_ )
        : VisualNode                 ( scene_ )
		, mSpriteRenderer			( NULL )
        
    {
        scene_->addViewport( this );
    }

	//----------------------------------------------------------------------
	// ● デストラクタ
	//----------------------------------------------------------------------
    Viewport::~Viewport()
    {
        

        if ( this->mSceneGraph )
        {
            this->mSceneGraph->removeViewport( this );
        }
        //ln_foreach( PPShader& pps, mPPShaderArray )
        //{
        //    LN_SAFE_RELEASE( pps.Shader );
        //}
    }

	//----------------------------------------------------------------------
	// ● 初期化
	//----------------------------------------------------------------------
    void Viewport::create( const Geometry::Size& size_, Graphics::SpriteRenderer* spriteRenderer )
    {
		mSpriteRenderer = spriteRenderer;

		ViewportContext* context = LN_NEW ViewportContext();
		context->initialize( this, spriteRenderer );
        SceneNode::initialize( 1, LN_DRAWCOORD_UNKNOWN, context );

        mViewSize.set( (lnFloat)size_.w, (lnFloat)size_.h );
    }

    

	//----------------------------------------------------------------------
	// ● プリプロセス・ポストプロセス用のシェーダを追加する
	//----------------------------------------------------------------------
    void Viewport::addPostEffectShader( SceneShader* shader_, int priority_ )
    {
        if ( /*( shader_->getScriptClass() == MME_SCRCLS_scene || 
               shader_->getScriptClass() == MME_SCRCLS_sceneorobject ) &&*/
             ( shader_->getScriptOrder() == MME_SCRORDER_preprocess ||
               shader_->getScriptOrder() == MME_SCRORDER_postprocess ) )
        {
            PPShader pps;
            pps.Shader   = shader_;
            pps.Priority = priority_;
            mPPShaderArray.push_back( pps );
            std::stable_sort( mPPShaderArray.begin(), mPPShaderArray.end(), _cmpPPShader );

            //LN_SAFE_ADDREF( shader_ );
            //printf( "シェーダ解放時、ビューボートからも外すようにチェックする。addref とかはいらない\n" );
        }
        else
        {
			LN_THROW_InvalidOperation(0, Resource::String::ERR_SceneViewport);
        }
    }

	//----------------------------------------------------------------------
	// ● プリプロセス・ポストプロセス用のシェーダを外す
	//----------------------------------------------------------------------
    void Viewport::removePostEffectShader( SceneShader* shader_ )
    {
        if ( shader_ )
        {
            PPShader pps;
            pps.Shader = shader_;
            Base::STLUtils::remove_if( mPPShaderArray, pps );

            //LN_SAFE_RELEASE( shader_ );
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Viewport::setTone( const LTone& tone_, double duration_ )
    {
        mTone.start( mTone.getValue(), (const LVector4&)tone_, duration_ );
        VisualNode::setTone( tone_ );
    }

	//----------------------------------------------------------------------
	// ● フレーム更新
	//----------------------------------------------------------------------
    void Viewport::update( float elapsedTime )
    {
        //mTone.advanceTime( mTone.getTime() + 1 );//time_.getElapsedGameTime() );
        mTone.advanceTime( 1 );
        VisualNode::setTone( (const LTone&)mTone.getValue() );
    }

//==============================================================================
// ■ ViewportContext
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ViewportContext::ViewportContext()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ViewportContext::~ViewportContext()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ViewportContext::initialize( Viewport* viewport, Graphics::SpriteRenderer* spriteRenderer)
	{
		VisualNodeContext::initialize( viewport );
		mViewport = viewport;
		mSpriteRenderer = spriteRenderer;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ViewportContext::render( ContextDrawParam& param )
	{
		// TODO: ちゃんと updateContext() でコピーすること

		if ( mSpriteRenderer )
		{
			param.SpriteRenderer = mSpriteRenderer;

			mSpriteRenderer->setViewProjMatrix(
				param.CameraContext->getViewMatrix(),
                param.CameraContext->getProjectionMatrix(),
				param.CameraContext->getViewSize() );
		}

		// TODO: これがないと　GodRay.fx で大変なことになってた
		//param.Renderer->clear( false, true, LColorF(0, 0, 0), 1 );

		// OffscreenScene 描画中はポストエフェクトなし
		if ( !param.OffscreenScene )
        {
            // ポストエフェクトの Script の、先頭から "ScriptExternal=Color" までの処理
            Viewport::PPShaderArray::iterator itr = mViewport->mPPShaderArray.begin();
            Viewport::PPShaderArray::iterator end = mViewport->mPPShaderArray.end();
            for ( ; itr != end; ++itr )
            {
				SceneShaderTechniqueArray* techs = itr->Shader->findTechnique( MMD_PASS_object, true, false, false );
				if ( techs )
				{
					int size = techs->size();
					for ( int i = 0; i < size; ++i )
					{
						DrawingCommandList* list = (*techs)[i]->getDrawingCommandList();
						list->setOwnerVisualNodeContext( this );
						list->executePreScriptExternal_Color( param );
					}
				}
            }

            // TODO: プリエフェクトの処理
        }

		if ( !mChildSceneNodeContextArray.empty() )
		{
			// 子のZソート
			std::stable_sort( mChildSceneNodeContextArray.begin(), mChildSceneNodeContextArray.end(), VisualNodeContext::_cmpZAndPrioritySort );

			// 子の処理
			SceneNodeContextArray::iterator itr = mChildSceneNodeContextArray.begin();
			SceneNodeContextArray::iterator end = mChildSceneNodeContextArray.end();
			for ( ;itr != end; ++itr )
			{
				(*itr)->render( param );
			}
		}

		// OffscreenScene 描画中はポストエフェクトなし
		if ( !param.OffscreenScene )
        {
			// スプライト
			//		子の処理と同じ位置 (ポストエフェクト内) で flash() してしまうと、最後のポストエフェクトに影響が出る。
			//		GodRay であれば、光源バッファは白黒でなければならないのにテクスチャ付スプライトが書かれてしまう、等。
			if ( mSpriteRenderer )
			{
				// param.OffscreenScene がある場合は flash() されないため、バッファがたまり続けてしまう。
				// BatchSprite 側で、param.OffscreenScene があるときは描画しないようにする。
				mSpriteRenderer->flash();
			}

            // ポストエフェクトの Script の、"ScriptExternal=Color" から終端までの処理
            // ポストエフェクトの Script の、先頭から "ScriptExternal=Color" までの処理
            Viewport::PPShaderArray::reverse_iterator ritr = mViewport->mPPShaderArray.rbegin();
            Viewport::PPShaderArray::reverse_iterator rend = mViewport->mPPShaderArray.rend();
            for ( ; ritr != rend; ++ritr )
            {
                SceneShaderTechniqueArray* techs = ritr->Shader->findTechnique( MMD_PASS_object, true, false, false );
				if ( techs )
				{
					int size = techs->size();
					for ( int i = 0; i < size; ++i )
					{
						DrawingCommandList* list = (*techs)[i]->getDrawingCommandList();
						//list->setOwnerSceneNode( mSceneNode );	// TODO:この辺　Contextにした方が・・・
						list->executePostScriptExternal_Color( param );
					}
				}
            }
        }
	}



//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================