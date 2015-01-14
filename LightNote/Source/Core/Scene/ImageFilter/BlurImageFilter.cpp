//==============================================================================
// BlurImageFilter 
//==============================================================================

#pragma once

#include "stdafx.h"
#include "../../Resource/LNResource.h"
#include "../../Graphics/Common/GeometryRenderer.h"
#include "../../Graphics/Common/GraphicsUtil.h"
#include "../Common.h"
#include "../Manager.h"
#include "../SceneNode/Camera.h"
#include "../SceneShader/SceneShader.h"
#include "BlurImageFilter.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// BlurImageFilter::DrawContext
//==============================================================================
class BlurImageFilter::DrawContext
	: public ImageFilterContext
{
public:
	DrawContext( BlurImageFilter* owner )
		: ImageFilterContext		( owner )
		, mOldRenderTarget			( NULL )
		, mPrimaryRenderTarget		( NULL )
		, mSecondaryRenderTarget	( NULL )
	{
        mBufferingBlarShader.Shader = NULL;
	}

	virtual ~DrawContext()
	{
		LN_SAFE_RELEASE( mPrimaryRenderTarget );
		LN_SAFE_RELEASE(mSecondaryRenderTarget);
		LN_SAFE_RELEASE(mBufferingBlarShader.Shader);
	}

	void create( Manager* manager )
	{
		mBufferingBlarShader.Shader = Graphics::Util::createShader(manager->getGraphicsManager(), Resource::ID_Shader_ImageFilter_BufferingBlar, NULL);
		mBufferingBlarShader.varBlurPower = mBufferingBlarShader.Shader->getVariableByName( _T( "gBlurPower" ) );
		mBufferingBlarShader.varBlurColor = mBufferingBlarShader.Shader->getVariableByName( _T( "gBlurColor" ) );
		mBufferingBlarShader.varBlurMatrix = mBufferingBlarShader.Shader->getVariableByName( _T( "gBlurMatrix" ) );
		mBufferingBlarShader.varSecondaryTexture = mBufferingBlarShader.Shader->getVariableByName( _T( "gSecondaryTexture" ) );
		mBufferingBlarShader.techMainDraw = mBufferingBlarShader.Shader->getTechniqueByIndex( 0 );
	}
		
	virtual void updateContext()
	{
		ImageFilterContext::updateContext();
		BlurImageFilter* owner = static_cast<BlurImageFilter*>(mOwner);

		mBlurColor  = owner->mBlurColor;
        mBlurMatrix = owner->mBlurMatrix;
		mBlurPower = owner->mBlurPower.getValue();
	}

	virtual void preRender( ContextDrawParam* param )
	{
		// �č쐬�̕K�v�͂��邩�H
		bool recreate = false;
		if ( mPrimaryRenderTarget == NULL )
		{
			recreate = true;	// ����
		}
		else if ( mPrimaryRenderTarget->getSize() != param->ViewSize )
		{
			recreate = true;	// ViewSize �ƃT�C�Y���Ⴄ
		}

		// ViewSize �ōč쐬
		if ( recreate )
		{
			LN_SAFE_RELEASE(mPrimaryRenderTarget);
			ImageFilter* owner = static_cast<ImageFilter*>(mOwner);
			mPrimaryRenderTarget = owner->getManager()->getGraphicsManager()->getGraphicsDevice()->createRenderTarget(
				param->ViewSize.X, param->ViewSize.Y, 1, Graphics::SurfaceFormat_X8R8G8B8 );
		}

		// �����_�[�^�[�Q�b�g����
		mOldRenderTarget = param->Renderer->getRenderTarget( 0 );
		param->Renderer->setRenderTarget( 0, mPrimaryRenderTarget );
		param->Renderer->clear(true, false, LColorF::Transparency, 1.0f);
	}

	virtual void postRender( ContextDrawParam* param )
	{ 
		// �O��̕`�挋�ʂ��g�k���A�v���C�}���ɏ�������
		if ( 1 && mSecondaryRenderTarget )
		{
			mBufferingBlarShader.varBlurPower->setFloat( mBlurPower );
			mBufferingBlarShader.varBlurColor->setVector( mBlurColor );
			mBufferingBlarShader.varBlurMatrix->setMatrix( mBlurMatrix );
			mBufferingBlarShader.varSecondaryTexture->setTexture( mSecondaryRenderTarget );

			Graphics::IShaderTechnique* tech = mBufferingBlarShader.techMainDraw;
			param->Renderer->setRenderState(Graphics::LRenderState::BUFFER_COPY_STATE_WITH_ALPHA);
			tech->getPassByIndex( 0 )->begin();
			param->GeometryRenderer->drawScreen( param->ViewSize );
			tech->getPassByIndex( 0 )->end();
		}
		else
		{
			// ���ʂɕ`��
		}

		// �����_�[�^�[�Q�b�g�����ɖ߂�
		param->Renderer->setRenderTarget( 0, mOldRenderTarget );

		// �v���C�}�������̃^�[�Q�b�g�ɏ�������
		param->GeometryRenderer->setMatrix( LMatrix::Identity );
		param->GeometryRenderer->setViewProjMatrix(LMatrix::Identity);
		param->GeometryRenderer->setTexture( mPrimaryRenderTarget );
		param->GeometryRenderer->begin();
		param->GeometryRenderer->drawScreen( param->ViewSize );
		param->GeometryRenderer->end();

		// �X���b�v
		std::swap( mPrimaryRenderTarget, mSecondaryRenderTarget );
	}

private:
	Graphics::Texture*		mOldRenderTarget;
	Graphics::Texture*		mPrimaryRenderTarget;
	Graphics::Texture*		mSecondaryRenderTarget;
	
    lnFloat					mBlurPower;
    LVector4				mBlurColor;
    LMatrix					mBlurMatrix;

	struct BufferingBlarShader
	{
		Graphics::IShader*          Shader;
		Graphics::IShaderVariable*  varBlurPower;
		Graphics::IShaderVariable*  varBlurColor;
		Graphics::IShaderVariable*  varBlurMatrix;
		Graphics::IShaderVariable*  varSecondaryTexture;
		Graphics::IShaderTechnique* techMainDraw;

	} mBufferingBlarShader;
};

//==============================================================================
// BlurImageFilter
//==============================================================================

LN_TYPE_INFO_ACCESS_IMPL(BlurImageFilter);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	BlurImageFilter::BlurImageFilter()
		: mBlarTime	( 0.0 )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	BlurImageFilter::~BlurImageFilter()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void BlurImageFilter::create( Manager* manager )
	{
		BlurImageFilter::DrawContext* context = LN_NEW BlurImageFilter::DrawContext( this );
		context->create( manager );
		ImageFilter::create( manager, context );

		mBlurColor.Set( 1, 1, 1, 1 );
		mBlurPower.setTimeTickPerSec( 1.0f );
	    mBlurPower.setCapacity( 5 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void BlurImageFilter::blur(double duration, lnFloat power, lnFloat scale, const LVector3& center, Camera* baseCamera)
	{
		mBlurMatrix = LMatrix::Identity;

        // �u���[�̓K�p�𖳂��ɂ���ꍇ
        if ( power == 0.0 )
        {
            mBlurPower.clear();
            mBlarTime = 0.0;
            mBlurPower.setTime( 0.0 );
            return;
        }

        if ( scale != 1.0f )
        {
			if (baseCamera != NULL)
			{
				mBlurCenter = LVector4(
					LVector3::TransformCoord(center, baseCamera->getViewProjectionMatrix()),
					1.0f);
			}
			else {
				mBlurCenter = LVector4(center, 1.0f);
			}

            // mBlurCenter �𒆐S�Ɋg�傷��
            mBlurMatrix.Translation( -mBlurCenter.X, -mBlurCenter.Y, 0 );
            mBlurMatrix.Scaling( scale );
            mBlurMatrix.Translation( mBlurCenter.X, mBlurCenter.Y, 0 );
        }

        mBlurPower.clear();

        // �����J�ڂ��Ȃ��ꍇ
        if ( duration == 0 )
        {
            mBlurPower.addKeyFrame( 0, power );

            mBlarTime = -1.0;
            mBlurPower.setTime( 0.0 );
        }
        // �����J�ڂ���ꍇ
        else
        {
            mBlurPower.addKeyFrame( 0, power );
            mBlurPower.addKeyFrame( static_cast< lnFloat >( duration ), 0 );

            mBlarTime = 0.0;
            mBlurPower.setTime( 0.0 );
        }
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void BlurImageFilter::updateFrame(float elapsedTime)
	{
        if ( mBlarTime >= 0.0 && !mBlurPower.isFinished() )
        {
            mBlarTime += elapsedTime;
            mBlurPower.setTime( mBlarTime );
        }
        else if ( mBlarTime == -1.0 )
        {
            mBlurPower.setTime( 0.0 );
        }
	}


} // namespace Scene
} // namespace Core
} // namespace LNote
