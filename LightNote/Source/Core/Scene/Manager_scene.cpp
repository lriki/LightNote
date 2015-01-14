//==============================================================================
// Manager 
//------------------------------------------------------------------------------
/*
	�� �Q�[���p�̃V�[���ƌ�������ׂ����H
		���݂��� Scene �́ASceneNode ���p�����āASprite�AMesh ������������`��
		��������Ă��邪�A����� Unity �I�ȁA�R���|�[�l���g���܂���
		GameObject �I�ɂ��������悢�̂ł͂Ȃ����H
		Scene �� Game �Ŏ����悤�ȊK�w�\�����o���オ���Ă��܂����c�B

		�� Scene �� Scene�BGame �� Game�B

		   Scene �͒P���ɃO���t�B�J���ȊG����邱�Ƃ��ړI�BMMD �Ƃ��B
		   �c�[�����낤���Q�[�����낤���֌W�Ȃ��A�Ƃ������A
		   �����Ŏg����悤�ɂ���ׂ��B���ۂɃG�t�F�N�g�G�f�B�^�Ƃ��Ŏg�����B

		   Game �̓Q�[���p�̃V�[���J��(Title�AMenu��)��A3D�����̃G�~�b�^�A
		   ���̂�R���W�����I�u�W�F�N�g�������āA�ЂƂ̃V�[���B
		   GameScene �� Scene �̏�ʃ��C���[�B

	�� SceneShaderManager �̒u���ꏊ�� SceneGraph �� Manager ��
		Shader �����ł���΁A�S�Ẵp�����[�^�� SceneGraph �̕`�掞�ɍX�V����邽��
		Manager �ɒu���Ă����Ȃ��B
		
		���� �I�t�X�N���[���V�[����CONTROLOBJECT�B

		�I�t�X�N���[���V�[���́A����V�[���O���t��`�悷��O�ɑS�ĕ`�悷����́B
		Manager �ɒu�����ꍇ�A�c�[���Ȃǂŕ����̃V�[���O���t��������ꍇ�́A
		�ǂ̃V�[���O���t�����ɃI�t�X�N���[���V�[���������_�����O���邩���l���Ȃ���΂Ȃ�Ȃ��B
		�������Ȃ��ƁA�V�[���O���tA�p�̃I�t�X�N���[���ɁA�V�[���O���tB�̃I�u�W�F�N�g��
		�`����Ă��܂��B
		�܂�A�Œ���A�ǂ̃V�[���O���t�̒��O�Ń����_�����O���邩���o���Ă����Ȃ���΂Ȃ�Ȃ��B

		SceneGraph �Ɏ������Ă����΍��̂܂܂œ��ɖ��Ȃ����c

		���O���Ă�̂́A����������ʁB
		SceneGraph ���������ƁA�S������ SceneShader �ł�
		�����ϐ�����X�N���v�g����������Ă��܂��B
		���ł��A�Q�[���ł͂ǂ݂̂�1�����V�[�����Ȃ����A
		  �c�[���ŕ�����邱�Ƃ��l���Ă��A������PC�������ʂ��猩��΋C�ɂ���قǂ̗ʂł��Ȃ��B
		  
		  ������܂Œʂ�ASceneGraph �ɒu���B

	[2014/05/19] ���ԊǗ��ɂ���
		�EBlurFilter ��
			�t���[���� / ����  ��؂�ւ�����
		�EShader
			MME �d�l�Ƃ��āA���ԒP�ʁB
			�� update() �� elapsedTime �͎��ԒP�� (60fps�Ȃ�0.0016) �ł���K�v������B
		�EModel
			�����܂�ModelNode ����� update �������BTest �� advanceTime() ���ڌĂ�ł�B
			VMD �`���� ����(30fps) �ɒ����čX�V����K�v������B
			�� update() �� elapsedTime * 0.5 �Ƃ��B
			   (���̃^�C���X�P�[����AnimController �Ŏ����Ă��������ǂ�����)
			Model �� ���[�U�[�����͂���l�Ƃ��ẮA
			�E�A�j���[�V�����̑J�ڎ���
			���炢���Ȃ��E�E�E
		
*/
//==============================================================================

#include "stdafx.h"
#include "../System/Manager.h"
#include "../FileIO/Interface.h"
#include "../Graphics/Interface.h"
#include "../Graphics/DeviceObjects.h"
#include "../Graphics/Manager.h"
#include "../Threading/Thread.h"
#include "SceneGraph.h"
#include "Pane.h"
#include "ImageFilter/ImageFilter.h"
#include "Manager.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// Manager
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Manager::Manager()
		: mGraphicsManager(NULL)
		, mEffectManager(NULL)
		, mDefaultScene(NULL)
		, mDefaultPane(NULL)
		, mDefault3DLayer(NULL)
		, mDefault2DLayer(NULL)
		, mAutoUpdateTimeScale(1.0)
		, mSystemManager(NULL)
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Manager::~Manager()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::initialize( const ConfigData& configData )
    {
        LN_LOG_INIT_BEGIN;
		
		LN_REFOBJ_SET( mSystemManager, configData.SystemManager );
        mGraphicsManager = configData.GraphicsManager;
        LN_SAFE_ADDREF( mGraphicsManager );
        mEffectManager = configData.EffectManager;

		mAutoUpdateTimeScale = configData.AutoUpdateTimeScale;

        // �f�t�H���g�̃V�[������� ( finalize() �ł̉���K�{ )
        SceneGraph::InitData data;
        data.FileManager        = configData.FileManager;
        data.GraphicsManager    = configData.GraphicsManager;
        data.EffectManager      = configData.EffectManager;
        data.PhysicsManager     = configData.PhysicsManager;
        data.VirtualViewSize    = configData.VirtualViewSize;
        data.ShaderCacheSize    = 32;
        //data.GUIManager            = init_data_.GUIManager; 
        //data.Criterion2DViewSize.x = init_data_.CriterionViewSize.x;
        //data.Criterion2DViewSize.y = init_data_.CriterionViewSize.y;
        //data.Default2DViewDepth    = 10000.0f;
        mDefaultScene = LN_NEW SceneGraph( this );
		mDefaultScene->initialize( data );



		// �f�t�H���g�� Pane
		mDefaultPane = LN_NEW Pane(mDefaultScene);
		mDefaultPane->create(mGraphicsManager->getGraphicsDevice()->getDefaultCanvas()->getBackbuffer());
		mDefaultPane->setScreenEffectMode(LN_SCREENEFFECTMODE_BUFFERINGEFFECT);

		mDefault3DLayer = LN_NEW DrawingLayer();
		mDefault3DLayer->create(mDefaultScene->getDefaultCamera(0));
		mDefault3DLayer->setPriority(0);
		//mDefaultPane->addLayer(mDefault3DLayer);
		mDefaultPane->getLayerList()->add(mDefault3DLayer);

		mDefault2DLayer = LN_NEW DrawingLayer();
		mDefault2DLayer->create(mDefaultScene->getDefaultCamera(1));
		mDefault2DLayer->setPriority(0);
		//mDefaultPane->addLayer(mDefault2DLayer);
		mDefaultPane->getLayerList()->add(mDefault2DLayer);


		mSystemManager->getMainWindow()->attachEventListener( this, 0 );

		mProfilerRenderer.create(mGraphicsManager, configData.Profiler);

        LN_LOG_INIT_END;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::finalize()
    {
        LN_LOG_FIN_BEGIN;

		// ImageFilter
		ln_foreach( ImageFilter* f, mImageFilterArray ) {
			f->release();
		}
		mImageFilterArray.clear();

		if ( mSystemManager ) {
			mSystemManager->getMainWindow()->detachEventListener( this );
		}
		
		//LN_SAFE_RELEASE( mDefaultPane );

		LN_SAFE_RELEASE(mDefault3DLayer);
		LN_SAFE_RELEASE(mDefault2DLayer);
		LN_SAFE_RELEASE(mDefaultPane);

        if ( mDefaultScene )
        {
            mDefaultScene->finalize();
            LN_SAFE_RELEASE( mDefaultScene );
        }

        LN_SAFE_RELEASE( mGraphicsManager );


        LN_SAFE_RELEASE( mSystemManager );

        LN_LOG_FIN_END;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::updateBeforePhysics()
	{
        mDefaultScene->updateBeforePhysics();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::updateDefaultScene( const Game::GameTime& gameTime )
	{
		mDefaultPane->updateFrame(gameTime.getElapsedTime() * mAutoUpdateTimeScale);
		mDefaultScene->updateFrame(gameTime.getElapsedTime() * mAutoUpdateTimeScale);
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::updateAfterPhysics()
	{
        mDefaultScene->updateAfterPhysics();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::updateDefaultSceneRenderContent()
	{
		mDefaultScene->updateAllContext();
		mDefaultPane->updateContext();
		//mDefaultPane->updateContext();

		mProfilerRenderer.updateRenderContents();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::renderDefaultScene(Graphics::Texture* renderTarget)
    {
        //Graphics::IRenderer* r = mGraphicsManager->getGraphicsDevice()->getRenderer();
        //Graphics::ICanvas* canvas = mGraphicsManager->getGraphicsDevice()->getDefaultCanvas();
        
        //LN_CALL_R( r->setRenderTarget( NULL, 0 ) );

		

		//canvas->activate();

		// �`��p�t�H�[�}���X������
		//{
		//	Base::ScopedProfilerSection section(
		//		Base::Profiler::Instance,
		//		Base::Profiler::RenderThread_ID,
		//		Base::Profiler::RenderThread_Section0);

			//r->beginScene();
			//r->clear( true, true, LColorF::WHITE, 1.0f );//0, 1.0f );//
			//mDefaultScene->render();
			//mDefaultRootPane->render( mDefaultScene );
			//mDefaultScene->renderByAllCameras();

			// Canvas �� RT ����蒼����邩������Ȃ��̂ŁA��ɊĎ��B
			// TODO: create�ŕ`����ݒ肷��̂͂���Ȃ������B
			mDefaultPane->setRenderTarget(renderTarget/*canvas->getBackbuffer()*/);
			mDefaultPane->render();
			// �Ƃ肠�����f�o�b�O�t�H���g
			//mGraphicsManager->getDebugText()->draw();
			/*r->clear(true,true,0xff0000ff,1.0f);*/

			// TODO: �Ƃ肠����
			LVector2 size(renderTarget->getSize().X, renderTarget->getSize().Y);
			mProfilerRenderer.render(size);



			//r->endScene();
		//}
		
		// present �p�t�H�[�}���X������
		//{
		//	Base::ScopedProfilerSection section(
		//		Base::Profiler::Instance,
		//		Base::Profiler::RenderThread_ID,
		//		Base::Profiler::RenderThread_Section_Present);

		//	canvas->present();
		//}

    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::executeGC()
	{
		// �Q�ƃJ�E���g�� 1 (���� Manager ���炵���Q�Ƃ���Ă��Ȃ�) ���폜����
		// TODO: StackArray
		ImageFilterArray::iterator itr = mImageFilterArray.begin();
		ImageFilterArray::iterator end = mImageFilterArray.end();
		for ( ; itr != end; )
		{
			LN_ERR2_ASSERT( (*itr)->getRefCount() > 0 );
			if ( (*itr)->getRefCount() == 1 ) {
				(*itr)->release();
				itr = mImageFilterArray.erase( itr );
			}
			else {
				++itr;
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool Manager::onEvent( const System::EventArgs& e )
    {
        return mDefaultScene->onEvent( e );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::postFrameUpdate()
	{
		mDefaultScene->postFrameUpdate();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::setBaseViewSize( const LVector2& size )
	{
		mDefaultScene->setBaseViewSize( size );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::addImageFilter( ImageFilter* filter )
	{
		filter->addRef();
		mImageFilterArray.push_back( filter );
	}

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================