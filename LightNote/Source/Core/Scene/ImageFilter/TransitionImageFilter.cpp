//==============================================================================
// TransitionImageFilter 
//==============================================================================

#pragma once

#include "stdafx.h"
#include "../../Resource/LNResource.h"
#include "../../Graphics/Common/GeometryRenderer.h"
#include "../../Graphics/Common/GraphicsUtil.h"
#include "../Common.h"
#include "../Manager.h"
#include "../SceneShader/SceneShader.h"
#include "TransitionImageFilter.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// TransitionImageFilter::DrawContext
//==============================================================================
class TransitionImageFilter::DrawContext
	: public ImageFilterContext
{
public:

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	DrawContext( TransitionImageFilter* owner )
		: ImageFilterContext		( owner )
		, mTransitionMaskTexture	( NULL )
		, mTransitionVague			( 0.0f )
		, mOldRenderTarget			( NULL )
		, mPrimaryRenderTarget		( NULL )
	{
		memset( mBackendTextures, 0, sizeof(mBackendTextures) );
		mBackendTextureIndex = 0;
		mFreezedTextureIndex = 1;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	virtual ~DrawContext()
	{
		LN_SAFE_RELEASE(mTransitionMaskTexture);
		LN_SAFE_RELEASE( mPrimaryRenderTarget );
		LN_SAFE_RELEASE( mBackendTextures[0] );
		LN_SAFE_RELEASE( mBackendTextures[1] );
		LN_SAFE_RELEASE( mTransitionShader.Shader );
		LN_SAFE_RELEASE( mTransitionWithMaskShader.Shader );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void create( Manager* manager )
	{
		mTransitionShader.Shader = Graphics::Util::createShader( manager->getGraphicsManager(), Resource::ID_Shader_ImageFilter_TransitionWithoutMask, NULL );
        mTransitionShader.varTransitionTime = mTransitionShader.Shader->getVariableByName( _T( "gTransitionTime" ) );
        mTransitionShader.varTexture = mTransitionShader.Shader->getVariableByName( _T( "gTexture" ) );
        mTransitionShader.varTransitionFreezedTexture = mTransitionShader.Shader->getVariableByName( _T( "gTransitionFreezedTexture" ) );
        mTransitionShader.techMainDraw = mTransitionShader.Shader->getTechniqueByIndex( 0 );

		mTransitionWithMaskShader.Shader = Graphics::Util::createShader(manager->getGraphicsManager(), Resource::ID_Shader_ImageFilter_TransitionWithMask, NULL);
        mTransitionWithMaskShader.varTransitionTime = mTransitionWithMaskShader.Shader->getVariableByName( _T( "gTransitionTime" ) );
        mTransitionWithMaskShader.varTransitionVague = mTransitionWithMaskShader.Shader->getVariableByName( _T( "gTransitionVague" ) );
        mTransitionWithMaskShader.varTexture = mTransitionWithMaskShader.Shader->getVariableByName( _T( "gTexture" ) );
        mTransitionWithMaskShader.varTransitionFreezedTexture = mTransitionWithMaskShader.Shader->getVariableByName( _T( "gTransitionFreezedTexture" ) );
        mTransitionWithMaskShader.varTransitionMaskTexture = mTransitionWithMaskShader.Shader->getVariableByName( _T( "gTransitionMaskTexture" ) );
        mTransitionWithMaskShader.techMainDraw = mTransitionWithMaskShader.Shader->getTechniqueByIndex( 0 );
	}
		
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	virtual void updateContext()
	{
		ImageFilterContext::updateContext();
		TransitionImageFilter* owner = static_cast<TransitionImageFilter*>(mOwner);

		// �l�ۑ�
		mTransitionFactor = owner->mTransitionFactor.getValue();
		LN_REFOBJ_SET( mTransitionMaskTexture, owner->mTransitionMaskTexture );
		mTransitionVague = owner->mTransitionVague;
		
		// freeze �v��������
		if ( owner->mFreezeRequested )
		{
			// ���̎��_�ł͂܂��`���T�C�Y���m�肵�Ă��炸�A�e�N�X�`������邱�Ƃ��ł��Ȃ��B
			// (�ł��Ȃ��Ƃ������A���ʂɂȂ�\��������)
			// ���ʂɃe�N�X�`���̃|�C���^���X���b�v����ƁA�č쐬�Ń|�C���^���킩���Ă��܂����߁A�C���f�b�N�X�ŊǗ�
			std::swap( mBackendTextureIndex, mFreezedTextureIndex );

			// ���������B�t���OOFF
			owner->mFreezeRequested = false;
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	virtual void preRender( ContextDrawParam* param )
	{
		// Factor �� 1.0 �̎��͑J�ڐ�Ɠ����ł��邽�߁A�������s���K�v�͂Ȃ�
		//if ( mTransitionFactor >= 1.0f ) return;

		Graphics::Texture** texs[2] = { &mPrimaryRenderTarget, &mBackendTextures[mBackendTextureIndex] };

		for ( int i = 0 ; i < 2; ++i )
		{
			// �č쐬�̕K�v�͂��邩�H
			bool recreate = false;
			if ( *texs[i] == NULL )
			{
				recreate = true;	// ����
			}
			else if ( (*texs[i])->getSize() != param->ViewSize )
			{
				recreate = true;	// ViewSize �ƃT�C�Y���Ⴄ
			}

			// ViewSize �ōč쐬
			if ( recreate )
			{
				LN_SAFE_RELEASE(*texs[i]);
				ImageFilter* owner = static_cast<ImageFilter*>(mOwner);
				*texs[i] = owner->getManager()->getGraphicsManager()->getGraphicsDevice()->createRenderTarget(
					(lnU32)param->ViewSize.x, (lnU32)param->ViewSize.y, 1, Graphics::SurfaceFormat_X8R8G8B8);
			}
		}

		// �����_�[�^�[�Q�b�g����
		mOldRenderTarget = param->Renderer->getRenderTarget( 0 );
		param->Renderer->setRenderTarget( 0, mPrimaryRenderTarget );
		param->Renderer->clear( true, false, LColorF::Transparency, 1.0f );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	virtual void postRender( ContextDrawParam* param )
	{ 
		// Factor �� 1.0 �̎��͑J�ڐ�Ɠ����ł��邽�߁A�������s���K�v�͂Ȃ�
		//if ( mTransitionFactor >= 1.0f ) return;

		// Freeze ���ꂽ�Ƃ��̓g�����W�V�����������̉摜���Œ肵�����B
		// ���̂��ߍ����ς݂̉摜��ێ�����K�v�����邽�߁A���̕ۑ���Ƃ��Ă܂��� Baskend �ɕ`������
		param->Renderer->setRenderTarget( 0, mBackendTextures[mBackendTextureIndex] );
		param->Renderer->clear(true, false, LColorF::Transparency, 1.0f);

		// �V�F�[�_�p�����[�^�ݒ� & �e�N�j�b�N�I��
		Graphics::IShaderTechnique* tech;
        if ( mTransitionMaskTexture )
        {
			// �}�X�N�e�N�X�`���g�p
            mTransitionWithMaskShader.varTexture->setTexture( mPrimaryRenderTarget );
            mTransitionWithMaskShader.varTransitionTime->setFloat( mTransitionFactor );
            mTransitionWithMaskShader.varTransitionFreezedTexture->setTexture( mBackendTextures[mFreezedTextureIndex] );
            mTransitionWithMaskShader.varTransitionVague->setFloat( mTransitionVague );
            mTransitionWithMaskShader.varTransitionMaskTexture->setTexture( mTransitionMaskTexture );
			tech = mTransitionWithMaskShader.techMainDraw;
        }
        else
        {
			// �}�X�N�e�N�X�`����g�p
			mTransitionShader.varTexture->setTexture( mPrimaryRenderTarget );
			mTransitionShader.varTransitionTime->setFloat( mTransitionFactor );
			mTransitionShader.varTransitionFreezedTexture->setTexture( mBackendTextures[mFreezedTextureIndex] );
			tech = mTransitionShader.techMainDraw;
        }
		//�������N������� freeze ���ꂽ�Ƃ��́AmBackendTextures[mFreezedTextureIndex] �͓����̃e�N�X�`���ł���

		// �`��
		param->Renderer->setRenderState(Graphics::LRenderState::BUFFER_COPY_STATE_WITH_ALPHA);
		tech->getPassByIndex( 0 )->begin();
		param->GeometryRenderer->drawScreen( param->ViewSize );
		tech->getPassByIndex( 0 )->end();
	
		// �����_�[�^�[�Q�b�g�����ɖ߂�
		param->Renderer->setRenderTarget( 0, mOldRenderTarget );

		// Backend �����̃^�[�Q�b�g�ɏ�������
		param->GeometryRenderer->setMatrix( LMatrix::IDENTITY );
		param->GeometryRenderer->setViewProjMatrix( LMatrix::IDENTITY );
		param->GeometryRenderer->setTexture( mBackendTextures[mBackendTextureIndex] );
		param->GeometryRenderer->begin();
		param->GeometryRenderer->drawScreen( param->ViewSize );
		param->GeometryRenderer->end();
	}

private:
		
	lnFloat					mTransitionFactor;
	Graphics::Texture*		mTransitionMaskTexture;
	lnFloat					mTransitionVague;

	Graphics::Texture*		mOldRenderTarget;
	Graphics::Texture*		mPrimaryRenderTarget;

	Graphics::Texture*		mBackendTextures[2];
	int						mBackendTextureIndex;
	int						mFreezedTextureIndex;

	// �}�X�N�L��g�����W�V�����p�̃V�F�[�_���
	struct TransitionShader
    {
        Graphics::IShader*          Shader;
        Graphics::IShaderVariable*  varTransitionTime;
        Graphics::IShaderVariable*  varTexture;
        Graphics::IShaderVariable*  varTransitionFreezedTexture;
        Graphics::IShaderTechnique* techMainDraw;

    } mTransitionShader;

	// �}�X�N�L��g�����W�V�����p�̃V�F�[�_���
    struct TransitionWithMaskShader
    {
        Graphics::IShader*          Shader;
        Graphics::IShaderVariable*  varTransitionTime;
        Graphics::IShaderVariable*  varTransitionVague;
        Graphics::IShaderVariable*  varTexture;
        Graphics::IShaderVariable*  varTransitionFreezedTexture;
        Graphics::IShaderVariable*  varTransitionMaskTexture;
        Graphics::IShaderTechnique* techMainDraw;

    } mTransitionWithMaskShader;
};

//==============================================================================
// TransitionImageFilter
//==============================================================================

	LN_TYPE_INFO_ACCESS_IMPL(TransitionImageFilter);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	TransitionImageFilter::TransitionImageFilter()
		: mTransitionMaskTexture	( NULL )
		, mTransitionVague			( 0.0f )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	TransitionImageFilter::~TransitionImageFilter()
	{
		LN_SAFE_RELEASE( mTransitionMaskTexture );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void TransitionImageFilter::create( Manager* manager )
	{
		TransitionImageFilter::DrawContext* context = LN_NEW TransitionImageFilter::DrawContext( this );
		context->create( manager );
		ImageFilter::create( manager, context );

		mTransitionFactor.start( 1.0f, 1.0f, 0 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void TransitionImageFilter::freeze() 
	{ 
		mFreezeRequested = true;
		mTransitionFactor.start( 0.0f, 0.0f, 0.0 );
        mTransitionFactor.setTime( 0.0 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void TransitionImageFilter::transition( double duration, Graphics::Texture* mask, int vague )
	{
		if ( mask )
        {
			LN_REFOBJ_SET( mTransitionMaskTexture, mask );
                
            // �����܂����͈̔͂� 0.0�`1.0 �ɕϊ�����
            int v = LMath::limit( vague, 1, 255 );
			mTransitionVague = static_cast< lnFloat >( v ) / 255.f;

            mTransitionFactor.start( -mTransitionVague, 1.0f, duration );
            mTransitionFactor.setTime( 0.0 );

            /*
			// �g�����W�V�����J�n���Atime = 0.0 �̏�Ԃ���
			// �����܂����͈̔͂ɓ����Ă���F���ŏ����瓧�߂���Ă��܂��B
			// �J�n���Ԃ͂����܂����͈̔͂������̒l�ɃZ�b�g���Ă����B
			mTransitionTime = -mTransitionVague;
			mTransitionElapsedTime = 1.0 / duration_;
            mTransitionElapsedTime += mTransitionVague / duration_;

            // �Ȃ��A�g�����W�V������ mTransitionTime �� 1.0 �ȏ�ɂȂ�ƏI��
            */
        }
        else
        {
            mTransitionFactor.start( 0.0f, 1.0f, duration );
            mTransitionFactor.setTime( 0.0 );
        }
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void TransitionImageFilter::updateFrame(float elapsedTime)
	{
        if ( !mTransitionFactor.isFinished() )
        {
            mTransitionFactor.advanceTime( elapsedTime );
        }
	}
	

} // namespace Scene
} // namespace Core
} // namespace LNote
