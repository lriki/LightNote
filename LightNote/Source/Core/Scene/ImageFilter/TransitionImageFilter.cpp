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

		// 値保存
		mTransitionFactor = owner->mTransitionFactor.getValue();
		LN_REFOBJ_SET( mTransitionMaskTexture, owner->mTransitionMaskTexture );
		mTransitionVague = owner->mTransitionVague;
		
		// freeze 要求がある
		if ( owner->mFreezeRequested )
		{
			// この時点ではまだ描画先サイズが確定しておらず、テクスチャを作ることができない。
			// (できないというか、無駄になる可能性がある)
			// 普通にテクスチャのポインタをスワップすると、再作成でポインタがわかってしまうため、インデックスで管理
			std::swap( mBackendTextureIndex, mFreezedTextureIndex );

			// 処理した。フラグOFF
			owner->mFreezeRequested = false;
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	virtual void preRender( ContextDrawParam* param )
	{
		// Factor が 1.0 の時は遷移先と同じであるため、処理を行う必要はない
		//if ( mTransitionFactor >= 1.0f ) return;

		Graphics::Texture** texs[2] = { &mPrimaryRenderTarget, &mBackendTextures[mBackendTextureIndex] };

		for ( int i = 0 ; i < 2; ++i )
		{
			// 再作成の必要はあるか？
			bool recreate = false;
			if ( *texs[i] == NULL )
			{
				recreate = true;	// 初回
			}
			else if ( (*texs[i])->getSize() != param->ViewSize )
			{
				recreate = true;	// ViewSize とサイズが違う
			}

			// ViewSize で再作成
			if ( recreate )
			{
				LN_SAFE_RELEASE(*texs[i]);
				ImageFilter* owner = static_cast<ImageFilter*>(mOwner);
				*texs[i] = owner->getManager()->getGraphicsManager()->getGraphicsDevice()->createRenderTarget(
					(lnU32)param->ViewSize.x, (lnU32)param->ViewSize.y, 1, Graphics::SurfaceFormat_X8R8G8B8);
			}
		}

		// レンダーターゲット交換
		mOldRenderTarget = param->Renderer->getRenderTarget( 0 );
		param->Renderer->setRenderTarget( 0, mPrimaryRenderTarget );
		param->Renderer->clear( true, false, LColorF::Transparency, 1.0f );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	virtual void postRender( ContextDrawParam* param )
	{ 
		// Factor が 1.0 の時は遷移先と同じであるため、処理を行う必要はない
		//if ( mTransitionFactor >= 1.0f ) return;

		// Freeze されたときはトランジション合成中の画像を固定したい。
		// そのため合成済みの画像を保持する必要があるため、その保存先としてまずは Baskend に描き込む
		param->Renderer->setRenderTarget( 0, mBackendTextures[mBackendTextureIndex] );
		param->Renderer->clear(true, false, LColorF::Transparency, 1.0f);

		// シェーダパラメータ設定 & テクニック選択
		Graphics::IShaderTechnique* tech;
        if ( mTransitionMaskTexture )
        {
			// マスクテクスチャ使用
            mTransitionWithMaskShader.varTexture->setTexture( mPrimaryRenderTarget );
            mTransitionWithMaskShader.varTransitionTime->setFloat( mTransitionFactor );
            mTransitionWithMaskShader.varTransitionFreezedTexture->setTexture( mBackendTextures[mFreezedTextureIndex] );
            mTransitionWithMaskShader.varTransitionVague->setFloat( mTransitionVague );
            mTransitionWithMaskShader.varTransitionMaskTexture->setTexture( mTransitionMaskTexture );
			tech = mTransitionWithMaskShader.techMainDraw;
        }
        else
        {
			// マスクテクスチャ非使用
			mTransitionShader.varTexture->setTexture( mPrimaryRenderTarget );
			mTransitionShader.varTransitionTime->setFloat( mTransitionFactor );
			mTransitionShader.varTransitionFreezedTexture->setTexture( mBackendTextures[mFreezedTextureIndex] );
			tech = mTransitionShader.techMainDraw;
        }
		//※もし起動直後に freeze されたときは、mBackendTextures[mFreezedTextureIndex] は透明のテクスチャである

		// 描画
		param->Renderer->setRenderState(Graphics::LRenderState::BUFFER_COPY_STATE_WITH_ALPHA);
		tech->getPassByIndex( 0 )->begin();
		param->GeometryRenderer->drawScreen( param->ViewSize );
		tech->getPassByIndex( 0 )->end();
	
		// レンダーターゲットを元に戻す
		param->Renderer->setRenderTarget( 0, mOldRenderTarget );

		// Backend を元のターゲットに書き込む
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

	// マスク有りトランジション用のシェーダ情報
	struct TransitionShader
    {
        Graphics::IShader*          Shader;
        Graphics::IShaderVariable*  varTransitionTime;
        Graphics::IShaderVariable*  varTexture;
        Graphics::IShaderVariable*  varTransitionFreezedTexture;
        Graphics::IShaderTechnique* techMainDraw;

    } mTransitionShader;

	// マスク有りトランジション用のシェーダ情報
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
                
            // あいまいさの範囲を 0.0～1.0 に変換する
            int v = LMath::limit( vague, 1, 255 );
			mTransitionVague = static_cast< lnFloat >( v ) / 255.f;

            mTransitionFactor.start( -mTransitionVague, 1.0f, duration );
            mTransitionFactor.setTime( 0.0 );

            /*
			// トランジション開始時、time = 0.0 の状態だと
			// あいまいさの範囲に入っている色が最初から透過されてしまう。
			// 開始時間はあいまいさの範囲だけ負の値にセットしておく。
			mTransitionTime = -mTransitionVague;
			mTransitionElapsedTime = 1.0 / duration_;
            mTransitionElapsedTime += mTransitionVague / duration_;

            // なお、トランジションは mTransitionTime が 1.0 以上になると終了
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
