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
// �� Viewport �N���X
//==============================================================================

	//----------------------------------------------------------------------
	// �� �R���X�g���N�^
	//----------------------------------------------------------------------
    Viewport::Viewport( SceneGraph* scene_ )
        : VisualNode                 ( scene_ )
		, mSpriteRenderer			( NULL )
        
    {
        scene_->addViewport( this );
    }

	//----------------------------------------------------------------------
	// �� �f�X�g���N�^
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
	// �� ������
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
	// �� �v���v���Z�X�E�|�X�g�v���Z�X�p�̃V�F�[�_��ǉ�����
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
            //printf( "�V�F�[�_������A�r���[�{�[�g������O���悤�Ƀ`�F�b�N����Baddref �Ƃ��͂���Ȃ�\n" );
        }
        else
        {
			LN_THROW_InvalidOperation(0, Resource::String::ERR_SceneViewport);
        }
    }

	//----------------------------------------------------------------------
	// �� �v���v���Z�X�E�|�X�g�v���Z�X�p�̃V�F�[�_���O��
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
	// �� �t���[���X�V
	//----------------------------------------------------------------------
    void Viewport::update( float elapsedTime )
    {
        //mTone.advanceTime( mTone.getTime() + 1 );//time_.getElapsedGameTime() );
        mTone.advanceTime( 1 );
        VisualNode::setTone( (const LTone&)mTone.getValue() );
    }

//==============================================================================
// �� ViewportContext
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
		// TODO: ������ updateContext() �ŃR�s�[���邱��

		if ( mSpriteRenderer )
		{
			param.SpriteRenderer = mSpriteRenderer;

			mSpriteRenderer->setViewProjMatrix(
				param.CameraContext->getViewMatrix(),
                param.CameraContext->getProjectionMatrix(),
				param.CameraContext->getViewSize() );
		}

		// TODO: ���ꂪ�Ȃ��Ɓ@GodRay.fx �ő�ςȂ��ƂɂȂ��Ă�
		//param.Renderer->clear( false, true, LColorF(0, 0, 0), 1 );

		// OffscreenScene �`�撆�̓|�X�g�G�t�F�N�g�Ȃ�
		if ( !param.OffscreenScene )
        {
            // �|�X�g�G�t�F�N�g�� Script �́A�擪���� "ScriptExternal=Color" �܂ł̏���
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

            // TODO: �v���G�t�F�N�g�̏���
        }

		if ( !mChildSceneNodeContextArray.empty() )
		{
			// �q��Z�\�[�g
			std::stable_sort( mChildSceneNodeContextArray.begin(), mChildSceneNodeContextArray.end(), VisualNodeContext::_cmpZAndPrioritySort );

			// �q�̏���
			SceneNodeContextArray::iterator itr = mChildSceneNodeContextArray.begin();
			SceneNodeContextArray::iterator end = mChildSceneNodeContextArray.end();
			for ( ;itr != end; ++itr )
			{
				(*itr)->render( param );
			}
		}

		// OffscreenScene �`�撆�̓|�X�g�G�t�F�N�g�Ȃ�
		if ( !param.OffscreenScene )
        {
			// �X�v���C�g
			//		�q�̏����Ɠ����ʒu (�|�X�g�G�t�F�N�g��) �� flash() ���Ă��܂��ƁA�Ō�̃|�X�g�G�t�F�N�g�ɉe�����o��B
			//		GodRay �ł���΁A�����o�b�t�@�͔����łȂ���΂Ȃ�Ȃ��̂Ƀe�N�X�`���t�X�v���C�g��������Ă��܂��A���B
			if ( mSpriteRenderer )
			{
				// param.OffscreenScene ������ꍇ�� flash() ����Ȃ����߁A�o�b�t�@�����܂葱���Ă��܂��B
				// BatchSprite ���ŁAparam.OffscreenScene ������Ƃ��͕`�悵�Ȃ��悤�ɂ���B
				mSpriteRenderer->flash();
			}

            // �|�X�g�G�t�F�N�g�� Script �́A"ScriptExternal=Color" ����I�[�܂ł̏���
            // �|�X�g�G�t�F�N�g�� Script �́A�擪���� "ScriptExternal=Color" �܂ł̏���
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
						//list->setOwnerSceneNode( mSceneNode );	// TODO:���̕Ӂ@Context�ɂ��������E�E�E
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