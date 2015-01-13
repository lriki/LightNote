//==============================================================================
// Pane 
//------------------------------------------------------------------------------
/*
	�� �J�����Ƃ̊֌W
		�@�\�I�ɂ͂͂��ׂăJ�����ɓ��ꂷ�ׂ��ł́H
			�J�����̓����_�����O�^�[�Q�b�g�����B
			����́A���̃J�����̕`��Ώۂ��������ރ^�[�Q�b�g�ŁA
			�f�t�H���g�ł̓o�b�N�o�b�t�@�B�K�v�ɉ����āA���[�U�[���C�ӂ�
			�����_�����O�^�[�Q�b�g��ݒ肷�邱�Ƃ��ł���B

			�����ŃJ�����ɃG�t�F�N�g�n�@�\(�u���\�Ȃ�)�����������ꍇ�c
			�G�t�F�N�g���g�p����Ƃ��́A�v���C�}���E�Z�J���_����2����
			�����_�����O�^�[�Q�b�g������Ǘ�����K�v������B

			���̂܂܂��ƁA�ȉ��̂悤�Ȑ�����ۂ��K�v������B
			�E�G�t�F�N�g�g�p���̓��[�U�[�̓����_�����O�^�[�Q�b�g��ݒ�ł��Ȃ�
			�E�J�����̎������_�����O�^�[�Q�b�g���g�p����ꍇ��
			  ���t���[�� camera->getRenderTarget() ���Ăяo����
			  �v���C�}���̃����_�����O�^�[�Q�b�g���擾����K�v������

			�J�������������ɃG�t�F�N�g�L������ݒ肷��ȂǁA
			���悤�ɂ���Ă͉���ł��邪�c

			Pane ��p�ӂ��闘�_�́A�C�ӂ̃����_�����O�^�[�Q�b�g�ւ�
			�l�X�ȕ`����B���ł��邱�ƁB�Ⴆ�΁A
				�E�u���\��F�� (���̃G�t�F�N�g)
				�E���C���[�Ǘ� (3D���C���[�A2D���C���[����1�A2D���C���[����2�AGUI���C���[��)
				�E�r���[�|�[�g
			Selene�ɂ� View �Ƃ��������悤�ȊT�O�����邵�A��������b�`�ɂ��������ɂȂ�B

	���܂�i�j���m�H
		�ŏI�I�ɕ`�悪�s���郌���_�����O�^�[�Q�b�g�����b�v����N���X�B

	���u���\���T�|�[�g����̂͂ǂ��������H
		Viewport
			+ SceneNode �̈ꕔ�B�q�m�[�h�݂̂ɓK�p�ȂǁA�ׂ����ꍇ�������ł���B
			- �����̃J�����ɓ����Ƀu���\���f�����Ƃ��ł��Ȃ��B(�����ȊO�͕s���R�ɂȂ�)
		Scene
			+ ������Ԓ����I�B
			+ ��ʕ����Ή��\�B
			- �u���\�p��RT���� Camera �Ɏ������邱�ƂɂȂ�B(Viewport �Ɣ�ׂėB��ł��镪�Ë�������)
			- �������������G�B������ɂ��Ă���ʑS�́B(�����Ɠ����ɑS�J������ blar() ���ĂԊ����ɂȂ邩��)
		Camera
			+ ��ʕ����Ή��\�B
			- ���܂蒼�ϓI�ł͂Ȃ���������Ȃ��B(�L����A�𒆐S�Ƀu���\�o������ �� 4�J�����g���Ă��4���ꂼ���)
		Pane
			+ ��ʕ����Ή��\�B(�������A���̏ꍇ Pane �� Camera �� 1��1�ɂȂ��Ă��܂�)
			- �J�������l�̗��R�ł��܂蒼�ϓI�ł͂Ȃ���������Ȃ��B

	���܂�i�j���m�H����2
		���ǁA�G�t�F�N�g�n (�F���A�g�����W�V�����A�u���\) �̓J����(��Scene)�Ɏ����čs��������
		�����I & ��ʕ����Ή����e�ՂɂȂ�C������B
		�c���ĂȂ�ƁAPane �̖�ڂ�RT�̃��b�v�����B
		RT�Ƒ��Ɏ����́A�`���r���[�|�[�g�����������肷��B
		�J�����̋K�͂��኱�傫���Ȃ邯�ǁA�V�������Ă��� cpp �̕��͂���قǗʂȂ����A
		�G�t�F�N�g�̊̂�SceneEffect �N���X�����A
		�܂��Ƃ����� Camera �֑S���ړ����� Pane �͔p�~�ŁB

		�ƂȂ�ƁA�����I�ȕ`�惋�[�g�� Camera �ɂȂ�B
		�ЂƂ�RT�ɕ����J�������`�悷�邱�Ƃ��U���ɏo�Ă���̂�(3D��2D)
		����ς�D��x���Ǘ�������̂�����ƕ֗��Ȃ̂����c
		�Ƃ������A�K�{�BWPF�ł͂Ƃ������A�Q�[���ł͕`��͕ʃX���b�h�ɉB���Ă�̂ŕK�{�B

		�c�ł��AManager �őS�J�����ꊇ�`��݂����Ȋ֐��p�ӂ���ΕK�{���Ă킯�ł��Ȃ��B
		�J�������ƂɗD��x����������K�v�͂��邪�B

		���������ʓ|�����ǁA�ʓ|���K�v�Ȃ̂̓c�[�������Ƃ������B
		���̖ʓ|���A3D 2D��2�̃J����������ĕ`�悷�邾���B
		C#����SceneCanvas �Ȃ�R���g���[����邵�A�������Ƀ��b�v���Ă�������Ȃ��B

		���ƎQ�l�܂łɁA������ƑO�����ʂ�SceneGraph�ł́Adraw() �� Camera �ƃ��[�g�m�[�h��n����
		�`�悷��A�Ƃ�������Ŏ�������Ă����B

		�v���O���}�I�i���Ɂu���K�v�Ȃ����̂͂��ꂩ����K�v�Ȃ��v�I�Ȃ��̂���������A
		Pane �͔p�~�ł�����������Ȃ��B

	�����ǂ܂��K�v�ɂȂ���
		2D��Ԃ̏��3D��`�������ꍇ�́HRestoer�ł��K�v�������@�\�����A
		�^�i�Q�[��Ready...�ł��g���Ă��B
		������X���[�Y�Ɏ�������ɂ́A����ς�J�����̏�ɂ�����Ǘ��N���X���K�v�B
		
		�������J�����ɗD��x���������� SceneGraph::renderAllCameras() �Ń\�[�g����̂����肾���ǁA
		���Ⴀ�X�N���[���G�t�F�N�g�͂ǂ�����́H
		3D�A2D�AGUI���C���[���ׂĂɃu���\������������������Ȃ����A
		3D���C���[�ɂ����g�����W�V����������������������Ȃ��B
		���Ă������A���̕Ӊ����Q�[������邽�тɃ��C�u�������𒼐ڏ��������Ă��C������B

		�X�N���[���G�t�F�N�g�͐�΂ɕW�������������B
		���A�_����������邽�߂ɂ͂���ς� Pane �͕K�v�B

	��Unity��GamePlay�̓J�������|�X�g�G�t�F�N�g�����Ă��邯�ǁA����ɂ͍��킹�Ȃ��́H
		�J�����P�ʂ��ƁA2D�A3D�̕`���Ԃ��܂������G�t�F�N�g�̓K�p������ɂȂ�B
		
		�Ⴆ�΁A3D�A2D�AGUI���C���[�S�̂ɑ΂��ău���[��K�p�������ꍇ�ȂǁB

		���ɂ��A3D�A2D ��Ԃ����t�F�[�h�A�E�g�����āAGUI �͂��̂܂܂ɂ������ȂǁB
		���̎��͈ȉ��̂悤�Ƀp�X��g�ݗ��Ă�K�v������B
		preRender
		render3D
		render2D
		postRender
		renderGUI

		���ƁA�|�X�g�G�t�F�N�g�� preRender �Ŕw�i�̓h��Ԃ����K�v�Ȃ̂����邵�A
		����� 3D �� 2D �ƕ`�悵���Ƃ��� 3D �`�挋�ʂ��h��Ԃ���Ă͘b�ɂȂ�Ȃ��B

		�J�����P�ʂɂ����ꍇ�A�������������ɂ�3D�A2D�AGUI�ƍŏI�`��p�̌n4�̃J������p�ӂ��A
		3D��2D�𓯂�RT�ɏ������݁�����RT���ŏI�`��p�J����(�G�t�F�N�g�t��)�ŕ`�恨GUI�`��
		�Ƃ����菇���K�v�B
		���Ђ˂�΂�����ł������͂łĂ��邾�낤���ALightNote�I�ɂ���Ȃ��Ƃ�
		�v���O���}�ɂ�点��̂͂�낵���Ȃ��B
		�J�����ɑ΂��Ă��ꌩ�u���Ɏg���񂾁H�v�Ƃ����v���p�e�B���������������Ȃ��Ƃ����Ȃ����A
		�J�����̔�剻�Ƃ������������肵�Ȃ������B

		
		
*/
//==============================================================================

#include "stdafx.h"
#include "../Base/STLUtils.h"
#include "../Resource/LNResource.h"
#include "SceneGraph.h"
#include "SceneShader/SceneShader.h"
#include "SceneNode/Camera.h"
#include "Manager.h"
#include "Layer.h"
#include "Pane.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

#if 0
//==============================================================================
// �� Layer
//==============================================================================
class Layer
{
public:
	virtual ~Layer() {}
	virtual void onUpdateViewSize( const LVector2& viewSize ) = 0;
	virtual void render( ContextDrawParam* param ) = 0;
	virtual const void* getDeleteKey() const = 0;

public:
	int		LayerLevel;
};

class CameraLayer
	: public Layer
{
public:
	CameraLayer( Camera* camera )
		: mCamera( NULL )
	{
		LN_REFOBJ_SET( mCamera, camera );
	}

	virtual ~CameraLayer()
	{
		LN_SAFE_RELEASE( mCamera );
	}

public:
	virtual void onUpdateViewSize( const LVector2& viewSize )
	{
		mCamera->onUpdateViewSize( viewSize );
	}

	virtual void render( ContextDrawParam* param )
	{
		mCamera->getCameraContext()->render( param );
	}
	
	virtual const void* getDeleteKey() const
	{ 
		return mCamera; 
	}

	Camera* getCamera() { return mCamera; }

private:
	Camera*	mCamera;
};
#endif

//==============================================================================
// Pane
//==============================================================================

	LN_TYPE_INFO_ACCESS_IMPL(Pane);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Pane::Pane( SceneGraph* scene )
		: mSceneGraph			( NULL )
		, mPaneContext			( NULL )
		, mRenderTarget			( NULL )
		, mLayerList(this)
        , mScreenEffectMode		( LN_SCREENEFFECTMODE_NONE )
		//, mScreenEffect			( NULL )
		, mBackgroundColor		( LColorF::White )
		//, mImageFilterRenderer	( NULL )
		//, mToneImageFilter		( NULL )
	{
		LN_REFOBJ_SET( mSceneGraph, scene );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Pane::~Pane()
	{
		_disposeScreenEffectResource();

		ln_foreach(LayerBase* layer, mLayerList)
		{
			//LN_SAFE_RELEASE(layer);
		}
		mLayerList.clear();

		//ln_foreach( Layer* layer, mLayerArrayInScreenEffect )
		//{
		//	SAFE_DELETE( layer );
		//}
		//ln_foreach( Layer* layer, mLayerArrayOutScreenEffect )
		//{
		//	SAFE_DELETE( layer );
		//}

//		LN_SAFE_RELEASE( mImageFilterRenderer );
		SAFE_DELETE( mPaneContext );
		LN_SAFE_RELEASE( mRenderTarget );
		LN_SAFE_RELEASE( mSceneGraph );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Pane::create( Graphics::Texture* renderTarget )
	{
		setRenderTarget( renderTarget );
		mPaneContext = LN_NEW PaneContext();

		//ImageFilterRenderer::create( mPaneContext );
		//mImageFilterRenderer = LN_NEW ImageFilterRenderer();
		//mImageFilterRenderer->create();

		mLayerList.LayerAdded.add(Base::createDelegate(this, &Pane::LayerList_Added));
		mLayerList.LayerRemoved.add(Base::createDelegate(this, &Pane::LayerList_Removed));
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Pane::setRenderTarget( Graphics::Texture* renderTarget )
	{
		LN_REFOBJ_SET( mRenderTarget, renderTarget );

		// �r���[�T�C�Y�̕ύX�Ď�
		const LVector2& size = mRenderTarget->getSize();
		if ( mViewSize != size ) 
		{
			mViewSize = size;

			ln_foreach(LayerBase* layer, mLayerList)
			{
				layer->onUpdateViewSize(mViewSize);
			}

			//ln_foreach( Layer* layer, mLayerArrayInScreenEffect )
			//{
			//	layer->onUpdateViewSize( mViewSize );
			//}
			//ln_foreach( Layer* layer, mLayerArrayOutScreenEffect )
			//{
			//	layer->onUpdateViewSize( mViewSize );
			//}

			// �f�t�H���g�� Pane �ł���� Scene �S�̂̃x�[�X�T�C�Y�Ƃ��Đݒ肷��
			if ( this == mSceneGraph->getManager()->getDefaultPane() )
			{
				mSceneGraph->setBaseViewSize( mViewSize );
			}
		}
	}

#if 0
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Pane::addLayer(LayerBase* layer)
	{
		LN_ERR2_ASSERT(layer->getParentPane() == NULL);

		mLayerArray.push_back(layer);

		// �\�[�g�p��r�֐�
		struct
		{
			bool operator()(const LayerBase* l, const LayerBase* r) const {
				return (l->getPriority() < r->getPriority());
			}
		} compare;

		// �\�[�g
		std::stable_sort(mLayerArray.begin(), mLayerArray.end(), compare);

		layer->addRef();
		layer->setParentPane(this);
		layer->onUpdateViewSize(getViewSize());
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Pane::removeLayer(LayerBase* layer)
	{
		LN_ERR2_ASSERT(layer->getParentPane() == this);

		Base::STLUtils::remove(mLayerArray, layer);
		layer->setParentPane(NULL);
		layer->release();
	}
#endif

#if 0
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Pane::addCameraLayer( Camera* camera, int layerLevel, bool screenEffect )
	{
		LN_ERR2_ASSERT( camera->getParentPane() == NULL );

		// �\�[�g�p��r�֐�
		struct
		{
			bool operator()(const Layer* l, const Layer* r) const {
				return ( l->LayerLevel < r->LayerLevel );
			}
		} compare;

		// ���C���[�쐬
		CameraLayer* layer = LN_NEW CameraLayer( camera );
		layer->LayerLevel = layerLevel;

		// ���X�g�ɒǉ�
		if ( screenEffect ) {
			mLayerArrayInScreenEffect.push_back( layer );
			std::stable_sort( mLayerArrayInScreenEffect.begin(), mLayerArrayInScreenEffect.end(), compare );
		}
		else {
			mLayerArrayOutScreenEffect.push_back( layer );
			std::stable_sort( mLayerArrayInScreenEffect.begin(), mLayerArrayInScreenEffect.end(), compare );
		}

		camera->setParentPane( this );
		layer->onUpdateViewSize( getViewSize() );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Pane::removeCameraLayer( Camera* camera )
	{
		LN_ERR2_ASSERT( camera->getParentPane() == this );

		// �����p��r�֐�
		struct
		{
			void* Key;
			bool operator() ( const Layer* layer ) const { return (layer->getDeleteKey() == Key); }
		} compare;

		compare.Key = camera;
		Base::STLUtils::remove_if( mLayerArrayInScreenEffect, compare );
		Base::STLUtils::remove_if( mLayerArrayOutScreenEffect, compare );

		camera->setParentPane( NULL );
	}
#endif


	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Pane::LayerList_Added(LayerBase* layer)
	{
		layer->addRef();	// TOOD: ����̓��X�g���ŊǗ�����ׂ��ȋC������
		layer->setParentPane(this);
		layer->onUpdateViewSize(getViewSize());
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Pane::LayerList_Removed(LayerBase* layer)
	{
		layer->setParentPane(NULL);
		layer->release();	// TOOD: ����̓��X�g���ŊǗ�����ׂ��ȋC������
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Pane::setScreenEffectMode( LNScreenEffectMode mode )
    {
        if ( mode != mScreenEffectMode )
        {
            _disposeScreenEffectResource();

            if ( mode == LN_SCREENEFFECTMODE_BUFFERINGEFFECT )
            {
				_createScreenEffectResource();
            }

            mScreenEffectMode = mode;
        }
    }

	////----------------------------------------------------------------------
	////
	////----------------------------------------------------------------------
	//void Pane::blur( double duration, lnFloat power, lnFloat scale, const LVector3& center, Camera* baseCamera ) 
	//{ 
	//	// baseCamera �������ꍇ�͍ŉ��ʃ��C���[�̃J������T���đ�p
	//	if ( baseCamera == NULL ) 
	//	{
	//		LayerArray::iterator itr = mLayerArrayInScreenEffect.begin();
	//		LayerArray::iterator end = mLayerArrayInScreenEffect.end();
	//		for ( ; itr != end; ++itr )
	//		{
	//			CameraLayer* layer = dynamic_cast<CameraLayer*>( *itr );
	//			if ( layer ) {
	//				baseCamera = layer->getCamera();
	//				break;
	//			}
	//		}
	//		if ( baseCamera == NULL )
	//		{
	//			itr = mLayerArrayOutScreenEffect.begin();
	//			end = mLayerArrayOutScreenEffect.end();
	//			for ( ; itr != end; ++itr )
	//			{
	//				CameraLayer* layer = dynamic_cast<CameraLayer*>( *itr );
	//				if ( layer ) {
	//					baseCamera = layer->getCamera();
	//					break;
	//				}
	//			}
	//		}
	//	}

	//	LVector3 pos;

	//	if ( baseCamera )
	//	{
	//		if ( baseCamera->isProjection2D() )
	//		{
	//			LVector3::transformCoord( &pos, center, baseCamera->getProjectionMatrix() );
	//		}
	//		else
	//		{
	//			LVector3::transformCoord( &pos, center, baseCamera->getViewProjectionMatrix() );
	//		}
	//	}
	//	else
	//	{
	//		pos = center;
	//	}

 //       mScreenEffect->blur( duration, power, scale, pos );
	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Pane::updateFrame(float elapsedTime)
	{
//		if ( mScreenEffectMode != LN_SCREENEFFECTMODE_BUFFERINGEFFECT )
//		{
//#if 0
//			if ( mRootNode3D->getSceneNodeType() == LN_SCENENODE_VIEWPORT )
//			{
//				((Viewport*)mRootNode3D)->setTone( (const LTone&)mTone.getValue(), 0 );
//			}
//			if ( mRootNode2D->getSceneNodeType() == LN_SCENENODE_VIEWPORT )
//			{
//				((Viewport*)mRootNode2D)->setTone( (const LTone&)mTone.getValue(), 0 );
//			}
//#endif
//		}
//        else
//        {
//			if ( mScreenEffect )
//			{
//				mScreenEffect->update( gameTime.getElapsedTime() );
//			}
//        }
//
//		mImageFilterRenderer->update( gameTime.getElapsedTime() );

		ln_foreach(LayerBase* layer, mLayerList)
		{
			layer->updateFrame(elapsedTime);
		}

	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Pane::updateContext()
	{
		//if ( mScreenEffect ) mScreenEffect->updateDrawContent();
		//mImageFilterRenderer->updateContext();
		ln_foreach(LayerBase* layer, mLayerList)
		{
			layer->updateContext();
		}
		mPaneContext->updateContext( this );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Pane::render()
	{
		ContextDrawParam param;
		param.Renderer = mSceneGraph->getGraphicsManager()->getGraphicsDevice()->getRenderer();
		param.GeometryRenderer = mSceneGraph->getGraphicsManager()->getGeometryRenderer();
		param.SpriteRenderer = NULL;
		param.CameraContext = NULL;
		param.SceneGraphContext = NULL;
		param.RenderRootNode = NULL;
		param.OffscreenScene = NULL;

		mPaneContext->render( &param );
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Pane::_createScreenEffectResource()
	{
        //mScreenEffect = LN_NEW Graphics::ScreenEffect();
        //mScreenEffect->initialize( mSceneGraph->getGraphicsManager(), getViewSize() );

		//mToneImageFilter = LN_NEW ToneImageFilter();
		//mToneImageFilter->create( mSceneGraph->getManager() );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Pane::_disposeScreenEffectResource()
    {
        //LN_SAFE_RELEASE( mScreenEffect );
		//LN_SAFE_RELEASE( mToneImageFilter );
    }

//==============================================================================
// �� PaneContext
//==============================================================================

	///---------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    PaneContext::PaneContext()
		: mOwnerPane	( NULL )
		, mRenderTarget	( NULL )
		, mScreenEffect	( NULL )
	{
	}

	///---------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    PaneContext::~PaneContext()
	{
		LN_SAFE_RELEASE( mRenderTarget );
		LN_SAFE_RELEASE( mScreenEffect );
	}

	///---------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void PaneContext::updateContext( Pane* pane )
	{
		mOwnerPane = pane;

		LN_REFOBJ_SET( mRenderTarget, pane->mRenderTarget );
		mLayerArray = pane->mLayerList;
		//mLayerArrayInScreenEffect	= pane->mLayerArrayInScreenEffect;
		//mLayerArrayOutScreenEffect	= pane->mLayerArrayOutScreenEffect;

//		LN_REFOBJ_SET( this->mScreenEffect, pane->mScreenEffect );
		this->mBackgroundColor = pane->mBackgroundColor;
	}

	///---------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void PaneContext::render( ContextDrawParam* param )
	{
		param->ViewSize = mRenderTarget->getSize();

		// �O���ŃX�e�[�g���ύX���ꂽ�܂܂ɂȂ��Ă���΍�̂��߁A��x���Z�b�g��������
		param->Renderer->setRenderState(Graphics::LRenderState(), true);

		param->Renderer->setRenderTarget( 0, mRenderTarget );

		// �X�N���[���G�t�F�N�g�̑O����
		//mOwnerPane->mImageFilterRenderer->getDrawContext()->preRender( param );

		//ln_foreach(LayerBase* layer, mLayerArray)
		//{
		//	layer->getContext()->preRender(param);
		//}
		Pane::LayerArray::reverse_iterator itr = mLayerArray.rbegin();
		Pane::LayerArray::reverse_iterator end = mLayerArray.rend();
		for (; itr != end; ++itr)
		{
			(*itr)->getContext()->preRender(param);
		}

		// �|�X�g�G�t�F�N�g�͊�{�I�� X8R8G8B8 �ō��Ȃ��Ƃ��܂��`��ł��Ȃ��B
		// ���߂���Ȃ����߁A�O������ɃN���A����B
		param->Renderer->clear((mBackgroundColor.A != 0), true, mBackgroundColor, 1.0f);
		//param->Renderer->clear(true, true, LColorF::GREEN, 1.0f);

		// �X�N���[���G�t�F�N�g�̑O���� (TODO:�폜�\��)
		//Graphics::ITexture* oldRenderTarget = NULL;
		//Graphics::ITexture* oldDepthBuffer = NULL;
		//if ( mScreenEffect )
		//{
		//	oldRenderTarget = param->Renderer->getRenderTarget( 0 );
		//	oldDepthBuffer  = param->Renderer->getDepthBuffer();

		//	param->Renderer->setRenderTarget( 0, mScreenEffect->getPrimaryRenderTarget(), true );
		//	param->Renderer->setDepthBuffer( mScreenEffect->getPrimaryDepthBuffer() );
		//	param->Renderer->clear( ( mBackgroundColor.alpha != 0 ), true, mBackgroundColor, 1.0f );
		//}

		// �G�t�F�N�g�K�p�`��
		//ln_foreach( Layer* layer, mLayerArrayInScreenEffect )
		//{
		//	layer->render( param );
		//}


		ln_foreach(LayerBase* layer, mLayerArray)
		{
			layer->getContext()->postRender(param);
		}


		// �X�N���[���G�t�F�N�g�̌㏈�� (TODO:�폜�\��)
		//if ( mScreenEffect )
		//{
		//	mScreenEffect->renderBlurAndTransition( param->Renderer, param->GeometryRenderer );

		//	param->Renderer->setRenderTarget( 0, oldRenderTarget, true );
		//	param->Renderer->setDepthBuffer( oldDepthBuffer );

		//	mScreenEffect->renderScreen( param->Renderer, param->GeometryRenderer );
		//	mScreenEffect->swapBuffers();
		//}

		// �X�N���[���G�t�F�N�g�̌㏈��
		//mOwnerPane->mImageFilterRenderer->getDrawContext()->postRender(param);


		// �G�t�F�N�g�O�`��
		//ln_foreach( Layer* layer, mLayerArrayOutScreenEffect )
		//{
		//	layer->render( param );
		//}
	}


} // namespace Scene
} // namespace Core
} // namespace LNote

#if 0

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// �� Pane
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Pane::Pane( Manager* manager )
        : mManager		( manager )
		//, mSceneGraph   ( NULL )
        , mCamera3D     ( NULL )
        , mCamera2D     ( NULL )
        , mRootNode3D   ( NULL )
        , mRootNode2D   ( NULL )
        , mGUIDesktop   ( NULL )
        , mPaneContext  ( NULL )
		, mPanePostDrawContext	( NULL )
        , mEffectMode               ( LN_SCREENEFFECTMODE_NONE )
        , mScreenEffect             ( NULL )
		, mBackgroundColor			( 1.0f, 1.0f, 1.0f, 1.0f )
    {
//        mManager->addPane( this );
        LN_SAFE_ADDREF( mManager );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Pane::~Pane()
    {
        _disposeEffectResource();

        ln_foreach( Pane* pane, mChildPaneArray )
        {
            LN_SAFE_RELEASE( pane );
        }
        mChildPaneArray.clear();

        LN_SAFE_RELEASE( mCamera3D );
        LN_SAFE_RELEASE( mCamera2D );
        LN_SAFE_RELEASE( mRootNode3D );
        LN_SAFE_RELEASE( mRootNode2D );
        LN_SAFE_RELEASE( mGUIDesktop );
        SAFE_DELETE( mPaneContext );
        if ( mManager )
        {
            //mManager->removePane( this );
			LN_SAFE_RELEASE( mManager );
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Pane::initialize( const Geometry::Size& size_ )
    {
        //mViewSize.set(  );
		_updateViewSize( LVector2( (lnFloat)size_.w, (lnFloat)size_.h ) );

        mPaneContext = LN_NEW PaneContext();
        mPaneContext->initialize( this );
    }

	

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Pane::setCamera( Camera* camera_3d_, Camera* camera_2d_ )
    {
        LN_SAFE_ADDREF( camera_3d_ );
        LN_SAFE_RELEASE( mCamera3D );
        mCamera3D = camera_3d_;
		if (mCamera3D) mCamera3D->setHolderPane( this );

        LN_SAFE_ADDREF( camera_2d_ );
        LN_SAFE_RELEASE( mCamera2D );
        mCamera2D = camera_2d_;
		if (mCamera2D) mCamera2D->setHolderPane( this );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Pane::setRootNode( SceneNode* root_3d_, SceneNode* root_2d_ )
    {
        LN_SAFE_ADDREF( root_3d_ );
        LN_SAFE_RELEASE( mRootNode3D );
        mRootNode3D = root_3d_;

        LN_SAFE_ADDREF( root_2d_ );
        LN_SAFE_RELEASE( mRootNode2D );
        mRootNode2D = root_2d_;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Pane::getRootNode( SceneNode** root_3d_, SceneNode** root_2d_ )
    {
        *root_3d_ = mRootNode3D;
        *root_2d_ = mRootNode2D;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//void Pane::setSceneGraph( SceneGraph* scene )
	//{

	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Pane::setGUIDesktop( GUI::GUIDesktop* desktop_ )
    {
        LN_SAFE_ADDREF( desktop_ );
        LN_SAFE_RELEASE( mGUIDesktop );
        mGUIDesktop = desktop_;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Pane::setEffectMode( LNScreenEffectMode mode_ )
    {
        if ( mode_ != mEffectMode )
        {
            _disposeEffectResource();

            if ( mode_ == LN_SCREENEFFECTMODE_BUFFERINGEFFECT )
            {
				_createEffectResource();
            }

            mEffectMode = mode_;
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Pane::addChildPane( Pane* pane_ )
    {
        mChildPaneArray.push_back( pane_ );
        LN_SAFE_ADDREF( pane_ );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Pane::removeChildPane( Pane* pane_ )
    {
        if ( Base::STLUtil::remove( mChildPaneArray, pane_ ) )
        {
            LN_SAFE_RELEASE( pane_ );
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Pane::blur(
        double duration_,
        lnFloat power_,
        lnFloat scale_,
        const LVector3& center_,
        bool center_is_3d_ )
    {
        LVector3 pos;

        if ( center_is_3d_ )
        {
            LVector3::transformCoord( &pos, center_, mCamera3D->getViewProjectionMatrix() );
        }
        else
        {
            LVector3::transformCoord( &pos, center_, mCamera2D->getProjectionMatrix() );
        }

        mScreenEffect->blur( duration_, power_, scale_, pos );
    }


	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Pane::update( const Game::GameTime& game_time )
    {
		if ( mEffectMode != LN_SCREENEFFECTMODE_BUFFERINGEFFECT )
		{
#if 0
			if ( mRootNode3D->getSceneNodeType() == LN_SCENENODE_VIEWPORT )
			{
				((Viewport*)mRootNode3D)->setTone( (const LTone&)mTone.getValue(), 0 );
			}
			if ( mRootNode2D->getSceneNodeType() == LN_SCENENODE_VIEWPORT )
			{
				((Viewport*)mRootNode2D)->setTone( (const LTone&)mTone.getValue(), 0 );
			}
#endif
		}
        else
        {
            mScreenEffect->update( game_time.getElapsedTime() );
        }

        // GUI
        if ( mGUIDesktop )
        {
            mGUIDesktop->updateCoordinate( LRect( 0, 0, (int)mViewSize.x, (int)mViewSize.y ) );
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Pane::makeDrawCommand( DrawCommandContainer* c_ )
    {
#if 0
		Graphics::IGraphicsDevice* device = mSceneGraph->getGraphicsManager()->getGraphicsDevice();
		Graphics::IRenderer* renderer = device->getRenderer();

        if ( mEffectMode == LN_SCREENEFFECTMODE_BUFFERINGEFFECT )
        {
            c_->add_SetRT( 0, mScreenEffect->getPrimaryRenderTarget(), true );
            c_->add_SetDepth(  mScreenEffect->getPrimaryDepthBuffer(), true );

			c_->add_ClearColor( mBackgroundColor.to32Bit( device->getGraphicsAPI() ) );
            c_->add_ClearDepth( 1.0f );
        }

        mRootNode3D->updateParamFromCameraAndLight( mCamera3D );
        mRootNode2D->updateParamFromCameraAndLight( mCamera2D );

        c_->add_SetActivePaneContext( mPaneContext, true );
        mRootNode3D->makeDrawCommand( c_, MMD_PASS_object );

        c_->add_SetActivePaneContext( mPaneContext, false );
        mRootNode2D->makeDrawCommand( c_, MMD_PASS_object );

        if ( mEffectMode == LN_SCREENEFFECTMODE_BUFFERINGEFFECT )
        {
            c_->add_SetRT( 0, NULL, true );
            c_->add_SetDepth( NULL, true );

            c_->add_DrawContext( mPanePostDrawContext );

            mScreenEffect->swapBuffers();
        }
#endif
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Pane::updateContext()
    {
        mPaneContext->updateContext();
		if ( mPanePostDrawContext ) mPanePostDrawContext->updateContext();

        if ( mScreenEffect ) mScreenEffect->updateDrawContent();

        // GUI
        if ( mGUIDesktop )
        {
            mGUIDesktop->updateDrawContents();
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Pane::render( SceneGraph* scene )
    {
		Graphics::IGraphicsDevice* device = mManager->getGraphicsManager()->getGraphicsDevice();
		Graphics::IRenderer* renderer = device->getRenderer();

		// �r���[�T�C�Y�̍X�V
		printf("_updateViewSize() �� updateContext() �ł��Ȃ��ƁA���\�[�X�č\�z��ɕs���A�N�Z�X\n");
		Graphics::ITexture* rt = renderer->getRenderTarget( 0 );
		_updateViewSize( rt->getSize() );

		Graphics::ITexture* oldRenderTarget = NULL;
		Graphics::ITexture* oldDepthBuffer = NULL;

        if ( mEffectMode == LN_SCREENEFFECTMODE_BUFFERINGEFFECT )
        {
			oldRenderTarget = renderer->getRenderTarget( 0 );
			oldDepthBuffer  = renderer->getDepthBuffer();

			renderer->setRenderTarget( 0, mScreenEffect->getPrimaryRenderTarget(), true );
			renderer->setDepthBuffer( mScreenEffect->getPrimaryDepthBuffer() );
			renderer->clear( true, true, mBackgroundColor, 1.0f );
        }

		scene->render2( this );

		if ( mEffectMode == LN_SCREENEFFECTMODE_BUFFERINGEFFECT )
        {
			mScreenEffect->renderBlurAndTransition( renderer, mManager->getGraphicsManager()->getGeometryRenderer() );

			if ( mGUIDesktop ) mGUIDesktop->draw();

            renderer->setRenderTarget( 0, oldRenderTarget, true );
			renderer->setDepthBuffer( oldDepthBuffer );


            mScreenEffect->renderScreen( renderer, mManager->getGraphicsManager()->getGeometryRenderer() );
            mScreenEffect->swapBuffers();
        }
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Pane::onEvent( const System::EventArgs& e )
	{
		return false;
	}
    
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Pane::_updateViewSize( const LVector2& size )
	{
		if ( mViewSize != size )
		{
			mViewSize = size;

			if ( mEffectMode == LN_SCREENEFFECTMODE_BUFFERINGEFFECT )
			{
				_disposeEffectResource();
				_createEffectResource();
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Pane::_createEffectResource()
	{
        mScreenEffect = LN_NEW Graphics::ScreenEffect();
        mScreenEffect->initialize( mManager->getGraphicsManager()->getGraphicsDevice(), mViewSize );

		mPanePostDrawContext = LN_NEW PanePostDrawContext();
		mPanePostDrawContext->initialize( this );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Pane::_disposeEffectResource()
    {
        SAFE_DELETE( mScreenEffect );
		LN_SAFE_RELEASE( mPanePostDrawContext );
    }

//==============================================================================
// �� PaneContext
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    PaneContext::PaneContext()
        : mCamera3D     ( NULL )
        , mCamera2D     ( NULL )
        , mGUIDesktop   ( NULL )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    PaneContext::~PaneContext()
    {
        LN_SAFE_RELEASE( mCamera3D );
        LN_SAFE_RELEASE( mCamera2D );
        LN_SAFE_RELEASE( mGUIDesktop );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void PaneContext::initialize( Pane* pane_ )
    {
        mPane = pane_;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void PaneContext::updateContext()
    {
        mViewSize = mPane->mViewSize;
        LN_REFOBJ_SET( mCamera3D, mPane->mCamera3D );
        LN_REFOBJ_SET( mCamera2D, mPane->mCamera2D );
        LN_REFOBJ_SET( mGUIDesktop, mPane->mGUIDesktop );
        mEffectMode = mPane->mEffectMode;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void PaneContext::getCameraContext( CameraContext** camera_context_3d_, CameraContext** camera_context_2d_ )
    {
        *camera_context_3d_ = mCamera3D->getCameraContext();
        *camera_context_2d_ = mCamera2D->getCameraContext();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void PaneContext::draw(
        Graphics::IRenderer* renderer_,
        Graphics::GeometryRenderer* geometry_renderer_ )
    {
        if ( mEffectMode == LN_SCREENEFFECTMODE_BUFFERINGEFFECT )
        {
            mPane->getScreenEffectRenderer()->renderBlurAndTransition( renderer_, geometry_renderer_ );
        }

        // GUI
        if ( mGUIDesktop )
        {
            mGUIDesktop->draw();
        }
    }

//==============================================================================
// �� PanePostDrawContext
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    PanePostDrawContext::PanePostDrawContext()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    PanePostDrawContext::~PanePostDrawContext()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void PanePostDrawContext::initialize( Pane* pane_ )
	{
		mPane = pane_;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void PanePostDrawContext::updateContext()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void PanePostDrawContext::draw(
        Graphics::IRenderer* renderer_,
        Graphics::GeometryRenderer* geometry_renderer_ )
	{
        mPane->getScreenEffectRenderer()->renderScreen( renderer_, geometry_renderer_ );        
	}


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

#endif

//==============================================================================
//
//==============================================================================