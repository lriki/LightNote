//==============================================================================
// Pane 
//------------------------------------------------------------------------------
///**
//  @file       Pane.h
//  @brief      Pane
//*/
//==============================================================================

#pragma once

#include "../Base/Delegate.h"
#include "../Base/ReferenceList.h"
#include "../Graphics/Common/ScreenEffect.h"
#include "SceneNode/SceneNode.h"
#include "ImageFilter/ImageFilter.h"
#include "ImageFilter/ToneImageFilter.h"
#include "Layer.h"

namespace LNote
{
namespace Core
{
namespace Scene
{
//class Layer;
//typedef std::vector<Layer*>	LayerArray;


//==============================================================================
// LayerList
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class LayerList
	: public Base::ReferenceList<LayerBase*>
{
private:
	Pane*	mPane;

public:
	LayerList(Pane* pane) : mPane(pane) {}
	virtual ~LayerList() {}

public:
	Base::Event01<LayerBase*>	LayerAdded;
	Base::Event01<LayerBase*>	LayerRemoved;

protected:

	virtual void onAdding(LayerBase*& item)
	{
		LN_ERR2_ASSERT(item->getParentPane() == NULL);
	}

	virtual void onAdded(LayerBase*& item)
	{ 
		LayerAdded.raise(item);
	}

	virtual void onRemoving(LayerBase*& item)
	{
		//LN_ERR2_ASSERT(item->getParentPane() == mPane);
	}

	virtual void onRemoved(LayerBase*& item)
	{
		LayerRemoved.raise(item);
	}
};


//==============================================================================
// Pane
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class Pane
	: public Base::ReferenceObject
{
	LN_TYPE_INFO_ACCESS_DECL;
public:
    Pane( SceneGraph* scene );
    virtual ~Pane();

public:

	/// 作成
    void create( Graphics::Texture* renderTarget );

	/// 描画先レンダリングターゲットの設定 (現在設定されているターゲットのサイズが変わった時もこれを呼ぶこと)
	void setRenderTarget( Graphics::Texture* renderTarget );

	/// 描画先深度バッファの取得
//	Graphics::ITexture* getDepthBuffer() { return mScreenEffect->getPrimaryDepthBuffer(); }

	LayerList* getLayerList() { return &mLayerList; }

	/// 描画先ビューサイズ
	const LVector2& getViewSize() const { return mViewSize; }

	///// レイヤーを追加する
	//void addLayer(LayerBase* layer);

	///// レイヤーを取り外す
	//void removeLayer(LayerBase* layer);








	///// カメラレイヤーを追加する (layerLevel は大きい方が手前になる)
	//void addCameraLayer( Camera* camera, int layerLevel, bool screenEffect );

	///// カメラレイヤーを追加する (layerLevel は大きい方が手前になる)
	//void removeCameraLayer( Camera* camera );

	///// フィルタのアタッチ
	//void attachImageFilter( ImageFilter* filter, int priority ) { /*mImageFilterRenderer->attachImageFilter( filter, priority );*/ }

	///// フィルタのデタッチ
	//void detachImageFilter( ImageFilter* filter ) { /*mImageFilterRenderer->detachImageFilter( filter );*/ }

public:

	void LayerList_Added(LayerBase* item);
	void LayerList_Removed(LayerBase* item);

	/// スクリーン特殊効果の使用設定
    void setScreenEffectMode( LNScreenEffectMode mode );

	/// 背景色の設定 (特殊効果有効時に使用する。LColorF::TRANSPARENCY を設定するとクリアしない)
	void setBackgroundColor( const LColorF& color ) { mBackgroundColor = color; }

	///// 色調の設定
 //   void setTone( const LTone& tone, double duration ) { mScreenEffect->setTone( tone, duration ); }

	///// トランジションの準備として現在の画面を固定する
 //   void freeze() { mScreenEffect->freeze(); }

	///// トランジション実行
 //   void transition( double duration, Graphics::ITexture* mask, int vague = 40 ) { mScreenEffect->transition( duration, mask, vague ); }

	///// ブラー実行
 //   void blur( double duration, lnFloat power, lnFloat scale, const LVector3& center, Camera* baseCamera );

	///// ブラー適用中かを判定する
 //   bool isRunningBlurEffect() const { return mScreenEffect->isBlurEffect(); }

	///// エフェクト適用中かを判定する
 //   bool isRunningScreenEffect() const { return mScreenEffect->isEffect(); }

public:
	void	updateFrame(float elapsedTime);
    void	updateContext();
	void	render();

private:
	void _createScreenEffectResource();
	void _disposeScreenEffectResource();

private:

	typedef std::vector<LayerBase*>	LayerArray;

	SceneGraph*				mSceneGraph;
	PaneContext*			mPaneContext;
	Graphics::Texture*		mRenderTarget;
	LVector2				mViewSize;

	//LayerArray				mLayerArray;
	LayerList				mLayerList;

	//LayerArray				mLayerArrayInScreenEffect;
	//LayerArray				mLayerArrayOutScreenEffect;

	LNScreenEffectMode		mScreenEffectMode;
	//Graphics::ScreenEffect* mScreenEffect;
    LColorF				    mBackgroundColor;

	//ImageFilterRenderer*	mImageFilterRenderer;
	//ToneImageFilter*		mToneImageFilter;


	friend class PaneContext;
};

//==============================================================================
// ■ PaneContext
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class PaneContext
	: public Base::ReferenceObject
{
public:
    PaneContext();
    ~PaneContext();
    
public:
    void updateContext( Pane* pane );
	void render( ContextDrawParam* param );

private:
	Pane*					mOwnerPane;
	Graphics::Texture*		mRenderTarget;
	Pane::LayerArray		mLayerArray;
	//LayerArray				mLayerArrayInScreenEffect;
	//LayerArray				mLayerArrayOutScreenEffect;

    Graphics::ScreenEffect* mScreenEffect;
    LColorF				    mBackgroundColor;
};

} // namespace Scene
} // namespace Core
} // namespace LNote
