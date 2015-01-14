//==============================================================================
// Manager 
//------------------------------------------------------------------------------
/*
	■ ゲーム用のシーンと結合するべきか？
		現在この Scene は、SceneNode を継承して、Sprite、Mesh 等特化させる形で
		実装されているが、これを Unity 的な、コンポーネントを包含する
		GameObject 的にした方がよいのではないか？
		Scene と Game で似たような階層構造が出来上がってしまうし…。

		→ Scene は Scene。Game は Game。

		   Scene は単純にグラフィカルな絵を作ることが目的。MMD とか。
		   ツールだろうがゲームだろうが関係ない、というか、
		   両方で使えるようにするべき。実際にエフェクトエディタとかで使うし。

		   Game はゲーム用のシーン遷移(Title、Menu等)や、3D音声のエミッタ、
		   剛体やコリジョンオブジェクトを持って、ひとつのシーン。
		   GameScene は Scene の上位レイヤー。

	■ SceneShaderManager の置き場所は SceneGraph か Manager か
		Shader だけであれば、全てのパラメータは SceneGraph の描画時に更新されるため
		Manager に置いても問題ない。
		
		問題は オフスクリーンシーンとCONTROLOBJECT。

		オフスクリーンシーンは、あるシーングラフを描画する前に全て描画するもの。
		Manager に置いた場合、ツールなどで複数のシーングラフを作った場合は、
		どのシーングラフを元にオフスクリーンシーンをレンダリングするかを考えなければならない。
		そうしないと、シーングラフA用のオフスクリーンに、シーングラフBのオブジェクトが
		描かれてしまう。
		つまり、最低限、どのシーングラフの直前でレンダリングするかを覚えておかなければならない。

		SceneGraph に持たせておけば今のままで特に問題ないが…

		懸念してるのは、メモリ消費量。
		SceneGraph が分かれると、全く同じ SceneShader でも
		同じ変数情報やスクリプトが生成されてしまう。
		→でも、ゲームではどのみち1つしかシーン作らないし、
		  ツールで複数作ることを考えても、昨今のPCメモリ量から見れば気にするほどの量でもない。
		  
		  →これまで通り、SceneGraph に置く。

	[2014/05/19] 時間管理について
		・BlurFilter 等
			フレーム数 / 時間  を切り替えたい
		・Shader
			MME 仕様として、時間単位。
			→ update() の elapsedTime は時間単位 (60fpsなら0.0016) である必要がある。
		・Model
			※いまはModelNode からの update 未実装。Test で advanceTime() 直接呼んでる。
			VMD 形式は 時間(30fps) に直して更新する必要がある。
			→ update() で elapsedTime * 0.5 とか。
			   (このタイムスケールはAnimController で持っておく方が良いかも)
			Model で ユーザーが入力する値としては、
			・アニメーションの遷移時間
			くらいかなぁ・・・
		
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

        // デフォルトのシーンを作る ( finalize() での解放必須 )
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



		// デフォルトの Pane
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

		// 描画パフォーマンス測定区間
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

			// Canvas で RT が作り直されるかもしれないので、常に監視。
			// TODO: createで描画先を設定するのはいらないかも。
			mDefaultPane->setRenderTarget(renderTarget/*canvas->getBackbuffer()*/);
			mDefaultPane->render();
			// とりあえずデバッグフォント
			//mGraphicsManager->getDebugText()->draw();
			/*r->clear(true,true,0xff0000ff,1.0f);*/

			// TODO: とりあえず
			LVector2 size(renderTarget->getSize().X, renderTarget->getSize().Y);
			mProfilerRenderer.render(size);



			//r->endScene();
		//}
		
		// present パフォーマンス測定区間
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
		// 参照カウントが 1 (この Manager からしか参照されていない) を削除する
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