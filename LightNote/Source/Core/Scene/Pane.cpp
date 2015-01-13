//==============================================================================
// Pane 
//------------------------------------------------------------------------------
/*
	■ カメラとの関係
		機能的にははすべてカメラに統一すべきでは？
			カメラはレンダリングターゲットを持つ。
			これは、そのカメラの描画対象を書き込むターゲットで、
			デフォルトではバックバッファ。必要に応じて、ユーザーが任意の
			レンダリングターゲットを設定することができる。

			ここでカメラにエフェクト系機能(ブラ―など)を持たせた場合…
			エフェクトを使用するときは、プライマリ・セカンダリの2枚の
			レンダリングターゲットを内部管理する必要がある。

			このままだと、以下のような制約を課す必要がある。
			・エフェクト使用時はユーザーはレンダリングターゲットを設定できない
			・カメラの持つレンダリングターゲットを使用する場合は
			  毎フレーム camera->getRenderTarget() を呼び出して
			  プライマリのレンダリングターゲットを取得する必要がある

			カメラ初期化時にエフェクト有効かを設定するなど、
			やりようによっては回避できるが…

			Pane を用意する利点は、任意のレンダリングターゲットへの
			様々な描画を隠蔽できること。例えば、
				・ブラ―や色調 (今のエフェクト)
				・レイヤー管理 (3Dレイヤー、2Dレイヤーその1、2Dレイヤーその2、GUIレイヤー等)
				・ビューポート
			Seleneにも View という似たような概念があるし、これをリッチにした感じになる。

	■つまりナニモノ？
		最終的に描画が行われるレンダリングターゲットをラップするクラス。

	■ブラ―をサポートするのはどこがいい？
		Viewport
			+ SceneNode の一部。子ノードのみに適用など、細かい場合分けができる。
			- 複数のカメラに同時にブラ―を映すことができない。(ある一つ以外は不自然になる)
		Scene
			+ 多分一番直感的。
			+ 画面分割対応可能。
			- ブラ―用のRT等は Camera に持たせることになる。(Viewport と比べて唯一である分妥協が利く)
			- 内部が少し複雑。いずれにしても画面全体。(発生と同時に全カメラの blar() を呼ぶ感じになるかも)
		Camera
			+ 画面分割対応可能。
			- あまり直観的ではないかもしれない。(キャラAを中心にブラ―出したい → 4カメラ使ってれば4つそれぞれで)
		Pane
			+ 画面分割対応可能。(ただし、その場合 Pane と Camera が 1対1になってしまう)
			- カメラ同様の理由であまり直観的ではないかもしれない。

	■つまりナニモノ？その2
		結局、エフェクト系 (色調、トランジション、ブラ―) はカメラ(とScene)に持って行った方が
		直感的 & 画面分割対応が容易になる気がする。
		…ってなると、Pane の役目はRTのラップだけ。
		RTと他に持つ情報は、描画先ビューポートだけだったりする。
		カメラの規模が若干大きくなるけど、新しくしてから cpp の方はそれほど量ないし、
		エフェクトの肝はSceneEffect クラスだし、
		まぁともあれ Camera へ全部移動して Pane は廃止で。

		となると、内部的な描画ルートは Camera になる。
		ひとつのRTに複数カメラが描画することがザラに出てくるので(3D→2D)
		やっぱり優先度を管理するものがあると便利なのだが…
		というか、必須。WPFではともかく、ゲームでは描画は別スレッドに隠してるので必須。

		…でも、Manager で全カメラ一括描画みたいな関数用意すれば必須ってわけでもない。
		カメラごとに優先度を持たせる必要はあるが。

		少しだけ面倒だけど、面倒が必要なのはツールを作るときだけ。
		その面倒も、3D 2Dの2つのカメラを作って描画するだけ。
		C#側でSceneCanvas なるコントロール作るし、そっちにラップしても何ら問題ない。

		あと参考までに、ちょっと前見た別のSceneGraphでは、draw() に Camera とルートノードを渡して
		描画する、という流れで実装されていた。

		プログラマ的格言に「今必要ないものはこれからも必要ない」的なものもある昨今、
		Pane は廃止でいいかもしれない。

	■結局また必要になった
		2D空間の上に3Dを描きたい場合は？Restoerでも必要だった機能だし、
		某格ゲーのReady...でも使ってた。
		これをスムーズに実装するには、やっぱりカメラの上にもう一つ管理クラスが必要。
		
		もちろんカメラに優先度を持たせて SceneGraph::renderAllCameras() でソートするのもありだけど、
		じゃあスクリーンエフェクトはどうするの？
		3D、2D、GUIレイヤーすべてにブラ―をかけたいかもしれないし、
		3Dレイヤーにだけトランジションをかけたいかもしれない。
		っていうか、その辺何かゲームを作るたびにライブラリ側を直接書き換えてた気がする。

		スクリーンエフェクトは絶対に標準実装したい。
		かつ、柔軟性を持たせるためにはやっぱり Pane は必要。

	■UnityやGamePlayはカメラがポストエフェクト持っているけど、それには合わせないの？
		カメラ単位だと、2D、3Dの描画空間をまたいだエフェクトの適用が困難になる。
		
		例えば、3D、2D、GUIレイヤー全体に対してブラーを適用したい場合など。

		他にも、3D、2D 空間だけフェードアウトさせて、GUI はそのままにしたいなど。
		この時は以下のようにパスを組み立てる必要がある。
		preRender
		render3D
		render2D
		postRender
		renderGUI

		あと、ポストエフェクトは preRender で背景の塗りつぶしが必要なのもあるし、
		それで 3D → 2D と描画したときに 3D 描画結果が塗りつぶされては話にならない。

		カメラ単位にした場合、これを解決するには3D、2D、GUIと最終描画用の系4つのカメラを用意し、
		3Dと2Dを同じRTに書き込み→このRTを最終描画用カメラ(エフェクト付き)で描画→GUI描画
		という手順が必要。
		頭ひねればいくらでも回避策はでてくるだろうが、LightNote的にこんなことを
		プログラマにやらせるのはよろしくない。
		カメラに対しても一見「何に使うんだ？」というプロパティをいくつか持たせないといけないし、
		カメラの肥大化というかすっきりしなさそう。

		
		
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
// ■ Layer
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

		// ビューサイズの変更監視
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

			// デフォルトの Pane であれば Scene 全体のベースサイズとして設定する
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

		// ソート用比較関数
		struct
		{
			bool operator()(const LayerBase* l, const LayerBase* r) const {
				return (l->getPriority() < r->getPriority());
			}
		} compare;

		// ソート
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

		// ソート用比較関数
		struct
		{
			bool operator()(const Layer* l, const Layer* r) const {
				return ( l->LayerLevel < r->LayerLevel );
			}
		} compare;

		// レイヤー作成
		CameraLayer* layer = LN_NEW CameraLayer( camera );
		layer->LayerLevel = layerLevel;

		// リストに追加
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

		// 検索用比較関数
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
		layer->addRef();	// TOOD: これはリスト側で管理するべきな気がする
		layer->setParentPane(this);
		layer->onUpdateViewSize(getViewSize());
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Pane::LayerList_Removed(LayerBase* layer)
	{
		layer->setParentPane(NULL);
		layer->release();	// TOOD: これはリスト側で管理するべきな気がする
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
	//	// baseCamera が無い場合は最下位レイヤーのカメラを探して代用
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
// ■ PaneContext
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

		// 外部でステートが変更されたままになっている対策のため、一度リセットをかける
		param->Renderer->setRenderState(Graphics::LRenderState(), true);

		param->Renderer->setRenderTarget( 0, mRenderTarget );

		// スクリーンエフェクトの前処理
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

		// ポストエフェクトは基本的に X8R8G8B8 で作らないとうまく描画できない。
		// 透過されないため、前処理後にクリアする。
		param->Renderer->clear((mBackgroundColor.A != 0), true, mBackgroundColor, 1.0f);
		//param->Renderer->clear(true, true, LColorF::GREEN, 1.0f);

		// スクリーンエフェクトの前処理 (TODO:削除予定)
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

		// エフェクト適用描画
		//ln_foreach( Layer* layer, mLayerArrayInScreenEffect )
		//{
		//	layer->render( param );
		//}


		ln_foreach(LayerBase* layer, mLayerArray)
		{
			layer->getContext()->postRender(param);
		}


		// スクリーンエフェクトの後処理 (TODO:削除予定)
		//if ( mScreenEffect )
		//{
		//	mScreenEffect->renderBlurAndTransition( param->Renderer, param->GeometryRenderer );

		//	param->Renderer->setRenderTarget( 0, oldRenderTarget, true );
		//	param->Renderer->setDepthBuffer( oldDepthBuffer );

		//	mScreenEffect->renderScreen( param->Renderer, param->GeometryRenderer );
		//	mScreenEffect->swapBuffers();
		//}

		// スクリーンエフェクトの後処理
		//mOwnerPane->mImageFilterRenderer->getDrawContext()->postRender(param);


		// エフェクト外描画
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
// ■ Pane
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

		// ビューサイズの更新
		printf("_updateViewSize() は updateContext() でしないと、リソース再構築後に不正アクセス\n");
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
// ■ PaneContext
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
// ■ PanePostDrawContext
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