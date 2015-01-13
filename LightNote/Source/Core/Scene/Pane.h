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

	/// �쐬
    void create( Graphics::Texture* renderTarget );

	/// �`��惌���_�����O�^�[�Q�b�g�̐ݒ� (���ݐݒ肳��Ă���^�[�Q�b�g�̃T�C�Y���ς��������������ĂԂ���)
	void setRenderTarget( Graphics::Texture* renderTarget );

	/// �`���[�x�o�b�t�@�̎擾
//	Graphics::ITexture* getDepthBuffer() { return mScreenEffect->getPrimaryDepthBuffer(); }

	LayerList* getLayerList() { return &mLayerList; }

	/// �`���r���[�T�C�Y
	const LVector2& getViewSize() const { return mViewSize; }

	///// ���C���[��ǉ�����
	//void addLayer(LayerBase* layer);

	///// ���C���[�����O��
	//void removeLayer(LayerBase* layer);








	///// �J�������C���[��ǉ����� (layerLevel �͑傫��������O�ɂȂ�)
	//void addCameraLayer( Camera* camera, int layerLevel, bool screenEffect );

	///// �J�������C���[��ǉ����� (layerLevel �͑傫��������O�ɂȂ�)
	//void removeCameraLayer( Camera* camera );

	///// �t�B���^�̃A�^�b�`
	//void attachImageFilter( ImageFilter* filter, int priority ) { /*mImageFilterRenderer->attachImageFilter( filter, priority );*/ }

	///// �t�B���^�̃f�^�b�`
	//void detachImageFilter( ImageFilter* filter ) { /*mImageFilterRenderer->detachImageFilter( filter );*/ }

public:

	void LayerList_Added(LayerBase* item);
	void LayerList_Removed(LayerBase* item);

	/// �X�N���[��������ʂ̎g�p�ݒ�
    void setScreenEffectMode( LNScreenEffectMode mode );

	/// �w�i�F�̐ݒ� (������ʗL�����Ɏg�p����BLColorF::TRANSPARENCY ��ݒ肷��ƃN���A���Ȃ�)
	void setBackgroundColor( const LColorF& color ) { mBackgroundColor = color; }

	///// �F���̐ݒ�
 //   void setTone( const LTone& tone, double duration ) { mScreenEffect->setTone( tone, duration ); }

	///// �g�����W�V�����̏����Ƃ��Č��݂̉�ʂ��Œ肷��
 //   void freeze() { mScreenEffect->freeze(); }

	///// �g�����W�V�������s
 //   void transition( double duration, Graphics::ITexture* mask, int vague = 40 ) { mScreenEffect->transition( duration, mask, vague ); }

	///// �u���[���s
 //   void blur( double duration, lnFloat power, lnFloat scale, const LVector3& center, Camera* baseCamera );

	///// �u���[�K�p�����𔻒肷��
 //   bool isRunningBlurEffect() const { return mScreenEffect->isBlurEffect(); }

	///// �G�t�F�N�g�K�p�����𔻒肷��
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
// �� PaneContext
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
