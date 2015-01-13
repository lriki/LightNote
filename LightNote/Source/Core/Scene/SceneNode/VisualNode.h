//==============================================================================
// VisualNode 
//------------------------------------------------------------------------------
///**
//  @file       VisualNode.h
//  @brief      VisualNode
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "SceneNode.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// ■ VisualNode
//------------------------------------------------------------------------------
///**
//  @brief	
//*/
//==============================================================================
class VisualNode
    : public SceneNode
{
	LN_TYPE_INFO_ACCESS_DECL;
public:
    VisualNode( SceneGraph* scene );
    virtual ~VisualNode();

public:

	/// 作成
    void create( int subsetCount, LNDrawCoordSpace cs, SceneNodeContext* context );

public:
#if 0

	/// 不透明度の設定
	void setOpacity( lnFloat opacity, lnU32 subsetIndex = 0 )			{ mRenderer->setOpacity( opacity, subsetIndex ); }

	/// 不透明度の取得
	lnFloat getOpacity( lnU32 subsetIndex = 0 ) const					{ return mRenderer->getOpacity( subsetIndex ); }

	/// 乗算色の設定
    void setColorScale( const LColorF& color, lnU32 subsetIndex = 0 )	{ mRenderer->setColorScale( color, subsetIndex ); }

	/// 乗算色の取得
    const LColorF& getColorScale( lnU32 subsetIndex = 0 ) const			{ return mRenderer->getColorScale( subsetIndex ); }

	/// ブレンドカラーの設定
    void setBlendColor( const LColorF& color, lnU32 subsetIndex = 0 )	{ mRenderer->setBlendColor( color, subsetIndex ); }

	/// ブレンドカラーの取得
    const LColorF& getBlendColor( lnU32 subsetIndex = 0 ) const			{ return mRenderer->getBlendColor( subsetIndex ); }

	/// 色調の設定
    void setTone( const LTone& tone, lnU32 subsetIndex = 0 )			{ mRenderer->setTone( tone, subsetIndex ); }

	/// 色調の取得
    const LTone& getTone( lnU32 subsetIndex = 0 ) const					{ return mRenderer->getTone( subsetIndex ); }

	/// UV 変換行列の設定
	void setUVTransform( const LMatrix& matrix, lnU32 subsetIndex = 0 )	{ mRenderer->setUVTransform( matrix, subsetIndex ); }

	/// UV 変換行列の設定
    const LMatrix& getUVTransform( lnU32 subsetIndex = 0 )  const		{ return mRenderer->getUVTransform( subsetIndex ); }

	/// 合成方法の設定
	void setBlendMode( LNBlendMode mode )				{ mRenderer->setBlendMode( mode ); }

	/// 合成方法の取得
	LNBlendMode getBlendMode() const					{ return mRenderer->getBlendMode(); }

	/// 深度テストの有効設定
    void setEnableDepthTest( bool enabled )				{ mRenderer->setEnableDepthTest( enabled ); }

	/// 深度テストの有効判定
    bool isEnableDepthTest() const						{ return mNodeRenderParam.RenderState.DepthTest; }

	/// 深度Writeの有効設定
    void setEnableDepthWrite( bool enabled )			{ mRenderer->setEnableDepthWrite( enabled ); }

	/// 深度書き込みの有効判定
    bool isEnableDepthWritet() const					{ return mNodeRenderParam.RenderState.DepthWrite; }

	/// カリング方法の設定
    void setCullingMode( LNCullingMode mode )			{ mRenderer->setCullingMode( mode ); }

	/// カリング方法の取得
    LNCullingMode getCullingMode() const				{ return mNodeRenderParam.RenderState.Culling; }

	/// シェーダの設定
    void setShader( SceneShader* shader );

	/// シェーダの取得
	SceneShader* getShader()							{ return mShader; }

//////
#else
	/// 可視状態の設定
    void setVisible( bool flag_ )                   { mIsVisible = flag_; }

	/// 可視状態の判定
    bool isVisible() const                          { return mIsVisible; }

	/// 不透明度の設定
    void setOpacity( lnFloat opacity_, lnU32 subset_idx_ = 0 )             { mNodeRenderParam.Subset[ subset_idx_ ].Opacity = opacity_; }//mOpacity = opacity_; }

	/// 不透明度の取得
    lnFloat getOpacity( lnU32 subset_idx_ = 0 ) const                      { return mNodeRenderParam.Subset[ subset_idx_ ].Opacity; }//return mOpacity; }

	/// 乗算色の設定
    void setColorScale( const LColorF& color_, lnU32 subset_idx_ = 0 )      { mNodeRenderParam.Subset[ subset_idx_ ].ColorScale = color_; }          

	/// 乗算色の取得
    const LColorF& getColorScale( lnU32 subset_idx_ = 0 ) const             { return mNodeRenderParam.Subset[ subset_idx_ ].ColorScale; }

	/// ブレンドカラーの設定
    void setBlendColor( const LColorF& color_, lnU32 subset_idx_ = 0 )      { mNodeRenderParam.Subset[ subset_idx_ ].BlendColor = color_; }

	/// ブレンドカラーの取得
    const LColorF& getBlendColor( lnU32 subset_idx_ = 0 ) const             { return mNodeRenderParam.Subset[ subset_idx_ ].BlendColor; }

	/// 色調の設定
    void setTone( const LTone& tone_, lnU32 subset_idx_ = 0 )              { mNodeRenderParam.Subset[ subset_idx_ ].Tone = tone_; }

	/// 色調の取得
    const LTone& getTone( lnU32 subset_idx_ = 0 ) const                     { return mNodeRenderParam.Subset[ subset_idx_ ].Tone; }

	/// UV 変換行列の設定
	void setUVTransform( const LMatrix& matrix_, lnU32 subset_idx_ = 0 )    { mNodeRenderParam.Subset[ subset_idx_ ].UVTransform = matrix_; }

	/// UV 変換行列の設定
    const LMatrix& getUVTransform( lnU32 subset_idx_ = 0 )  const           { return mNodeRenderParam.Subset[ subset_idx_ ].UVTransform; }

	/// 合成方法の設定
	void setBlendMode(Graphics::BlendMode mode_)          { mNodeRenderParam.RenderState.Blend = mode_; }

	/// 合成方法の取得
	Graphics::BlendMode getBlendMode() const                { return mNodeRenderParam.RenderState.Blend; }

	/// 深度テストの有効設定
    void setEnableDepthTest( bool flag_ )           { mNodeRenderParam.RenderState.DepthTest = flag_; }

	/// 深度テストの有効判定
    bool isEnableDepthTest() const                  { return mNodeRenderParam.RenderState.DepthTest; }

	/// 深度Writeの有効設定
    void setEnableDepthWrite( bool flag_ )          { mNodeRenderParam.RenderState.DepthWrite = flag_; }

	/// 深度書き込みの有効判定
    bool isEnableDepthWritet() const                { return mNodeRenderParam.RenderState.DepthWrite; }

	/// カリング方法の設定
	void setCullingMode(Graphics::CullingMode mode_)      { mNodeRenderParam.RenderState.Culling = mode_; }

	/// カリング方法の取得
	Graphics::CullingMode getCullingMode() const            { return mNodeRenderParam.RenderState.Culling; }

	///// 回転順序の設定
	//void setRotateOrder( Math::RotationOrder order_ )	{ mRotOrder = order_; }

	///// 回転順序の取得
	//Math::RotationOrder getRotateOrder() const			{ return mRotOrder; }

	///// 優先度の設定 (高い方から先に描画される)
	//void setPriority( lnS32 priority_ )               { mPriority = priority_; }

	///// 優先度の取得
	//lnS32 getPriority() const                         { return mPriority; }
  
	///// ビルボード状態の設定
	//void setBillboardType( LNBillboardType type_ )  { mBillboardType = type_; }

	///// ビルボード状態の取得
	//LNBillboardType getBillboardType() const        { return mBillboardType; }

	///// 自動更新の有効設定
	//void setEnableAutoUpdate( bool flag_ )          { mIsAutoUpdate = flag_; }

	///// 自動更新の有効判定
	//bool isEnableAutoUpdate()                       { return mIsAutoUpdate; }

	/// シェーダの設定
    void setShader( SceneShader* shader_ );

	/// シェーダの取得
    SceneShader* getShader()                        { return mShader; }
#endif



public:
	virtual SceneNodeType	getNodeType() const { return SceneNodeType_VisualNode; }
    virtual void			updateContext();

protected:

	VisualNodeRenderer*	mRenderer;
};


//==============================================================================
// ■ VisualNodeContext
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class VisualNodeContext
	: public SceneNodeContext
{
public:
	VisualNodeContext();
	virtual ~VisualNodeContext() {}

public:
	const Graphics::LRenderState&			getRenderState() const { return mRenderState; }
    int							getSubsetCount() const { return mSubsetRenderParamArray.size(); }
	const LNSubsetRenderParam&  getSubsetRenderParam( int index ) const { return mSubsetRenderParamArray[index]; }
	
public:
	// サブセットがある場合に render() から呼ばれる (ステートは設定済み。頂点の描画だけ行う)
	virtual void drawSubset( ContextDrawParam& param, int index ) {}

protected:
	// SceneNodeContext
	virtual SceneNodeType	getNodeType() const { return SceneNodeType_VisualNode; }
    virtual void			updateContext( SceneNodeContext* parentContext );
    virtual void			releaseAllResource();
	virtual void			render( ContextDrawParam& param );

protected:
	// MMESceneObjectBase
	virtual SceneShader*	getShader() { return mShader; }
	virtual lnFloat			getOpacity() const { return (mSubsetRenderParamArray.empty()) ? 1.0f : mSubsetRenderParamArray[0].Opacity; }
	
protected:
	typedef std::vector<LNSubsetRenderParam>				SubsetRenderParamArray;

	Graphics::LRenderState			mRenderState;
    SubsetRenderParamArray	mSubsetRenderParamArray;
    SceneShader*            mShader;


#if 0
public:

    void initialize( SceneNode* node );

	/// [同期フェーズ]
    virtual void updateContext( VisualNodeContext* parent_context_ );

	/// [同期フェーズ] (とりあえず updateContext() から呼んでいる)
    virtual void releaseAllResource();

	/// [描画スレッド]
	//void setPriorityParameter( LNPriorityParameter* param_ ) { mPriorityParameter = param_; }

	/// [描画スレッド]
    virtual void preRender( ContextDrawParam& param );

	/// [描画スレッド] Node 単位のパラメータをシェーダに設定する時に呼び出される
    virtual void onSetNodeParameter( SceneShader* shader_ ) {}

	/// [描画スレッド] (今は Model のみで使用中。ほかのも順に、これで描画するようにしたい)
	virtual void drawSubset( lnU32 index ) { mSceneNode->drawSubset( index ); }

	/// 描画
	virtual void render( ContextDrawParam& param );

public:

    int getSubsetCount() const { return mSubsetNum; }

    virtual SceneShader* getShader() { return mShader; }

    const LMatrix& getCombinedWorldMatrix() const { return mCombinedWorldMatrix; }

    

    

	// SceneShader::updateSubsetParam() 用
    const LNSubsetRenderParam&  getSubsetRenderParam( int index_ ) const { return mSubsetRenderParams[index_]; }

    const LRenderState&         getRenderState() const { return mRenderState; }

    CameraContext* getCameraContext() { return mAffectCameraContext; }

protected:
	virtual const lnChar*			getName() const { return mName.c_str(); }
	virtual const LMatrix&			getMatrix() const { return mCombinedWorldMatrix; }
	virtual bool					isVisible() const { return mIsVisible; }
	virtual lnFloat					getOpacity() const { return mSubsetRenderParams[0].Opacity; }
	virtual void					setPriorityParameter( LNPriorityParameter* param ) { mPriorityParameter = param; }
    virtual const MMELightArray&	getAffectLightArray() const { return mAffectLightContextArray; }
    virtual bool					getObjectMatrix( LMatrix* out, MMEVariableRequest req, int light_idx ) const;
	virtual const LMatrix&			findFrameMarix( const lnChar* name ) const { return LMatrix::IDENTITY; }
	virtual lnFloat					findMorphBlend( const lnChar* name ) const { return 0.0f; }
protected:

    static bool _cmpZAndPrioritySort( const VisualNodeContext* l_, const VisualNodeContext* r_ );

protected:

    SceneNode*              mSceneNode;
	lnRefString				mName;
    VisualNodeContextArray   mChildSceneNodeContextArray;
    lnU32                   mSubsetNum;
    LNSubsetRenderParam*    mSubsetRenderParams;	///< 要素数は mSubsetNum
    LRenderState            mRenderState;
    LNPriorityParameter*    mPriorityParameter;
    LMatrix                 mCombinedWorldMatrix;   ///< 行列の階層更新で作成される結合済みワールド行列
    lnS32                   mPriority;
    LNBillboardType         mBillboardType;
    SceneShader*            mShader;
    lnFloat                 mZDistance;
    MMELightArray       mAffectLightContextArray;           ///< 描画に使うライト
    CameraContext*          mAffectCameraContext;
    bool                    mIsVisible;
	bool					mEnableDrawing;

public:

	SceneNodeArray			mChildNodeList;
#endif
};
} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================