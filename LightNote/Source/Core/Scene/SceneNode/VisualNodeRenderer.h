//=============================================================================
//【 VisualNodeRenderer 】
//-----------------------------------------------------------------------------
///**
//  @file       VisualNodeRenderer.h
//  @brief      VisualNodeRenderer
//  @author     Riki
//*/
//=============================================================================

#pragma once

#include "NodeRenderParam.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

////=============================================================================
//// ■ VisualNodeRenderer
////-----------------------------------------------------------------------------
/////**
////  @brief
////*/
////=============================================================================
//class VisualNodeRenderer
//    : public Base::ReferenceObject
//{
//public:
//    VisualNodeRenderer();
//    virtual ~VisualNodeRenderer();
//
//public:
//
//	/// 作成
//    void create( VisualNode* ownerNode, int subsetParamCount, VisualNodeRendererContext* rendererContext = NULL );
//	
//	/// この Renderer を保持する SceneNode の取得
////	SceneNode* getOwnerNode() { return mOwnerNode; }
//
//public:
//
//	///// 可視状態の設定
// //   void setVisible( bool enabled )										{ mIsVisible = enabled; }
//
// //   /// 可視状態の判定
// //   bool isVisible() const												{ return mIsVisible; }
//
//	/// 不透明度の設定
//    void setOpacity( lnFloat opacity, lnU32 subsetIndex = 0 )			{ mNodeRenderParam.Subset[ subsetIndex ].Opacity = opacity; }
//
//	/// 不透明度の取得
//    lnFloat getOpacity( lnU32 subsetIndex = 0 ) const					{ return mNodeRenderParam.Subset[ subsetIndex ].Opacity; }
//
//	/// 乗算色の設定
//    void setColorScale( const LColorF& color, lnU32 subsetIndex = 0 )	{ mNodeRenderParam.Subset[ subsetIndex ].ColorScale = color; }          
//
//	/// 乗算色の取得
//    const LColorF& getColorScale( lnU32 subsetIndex = 0 ) const			{ return mNodeRenderParam.Subset[ subsetIndex ].ColorScale; }
//
//	/// ブレンドカラーの設定
//    void setBlendColor( const LColorF& color, lnU32 subsetIndex = 0 )	{ mNodeRenderParam.Subset[ subsetIndex ].BlendColor = color; }
//
//	/// ブレンドカラーの取得
//    const LColorF& getBlendColor( lnU32 subsetIndex = 0 ) const			{ return mNodeRenderParam.Subset[ subsetIndex ].BlendColor; }
//
//	/// 色調の設定
//    void setTone( const LTone& tone, lnU32 subsetIndex = 0 )			{ mNodeRenderParam.Subset[ subsetIndex ].Tone = tone; }
//
//	/// 色調の取得
//    const LTone& getTone( lnU32 subsetIndex = 0 ) const					{ return mNodeRenderParam.Subset[ subsetIndex ].Tone; }
//
//	/// UV 変換行列の設定
//	void setUVTransform( const LMatrix& matrix, lnU32 subsetIndex = 0 )	{ mNodeRenderParam.Subset[ subsetIndex ].UVTransform = matrix; }
//
//	/// UV 変換行列の設定
//    const LMatrix& getUVTransform( lnU32 subsetIndex = 0 )  const		{ return mNodeRenderParam.Subset[ subsetIndex ].UVTransform; }
//
//	/// 合成方法の設定
//    void setBlendMode( LNBlendMode mode )				{ mNodeRenderParam.RenderState.BlendMode = mode; }
//
//	/// 合成方法の取得
//    LNBlendMode getBlendMode() const					{ return mNodeRenderParam.RenderState.BlendMode; }
//
//	/// 深度テストの有効設定
//    void setEnableDepthTest( bool enabled )				{ mNodeRenderParam.RenderState.DepthTest = enabled; }
//
//	/// 深度テストの有効判定
//    bool isEnableDepthTest() const						{ return mNodeRenderParam.RenderState.DepthTest; }
//
//	/// 深度Writeの有効設定
//    void setEnableDepthWrite( bool enabled )			{ mNodeRenderParam.RenderState.DepthWrite = enabled; }
//
//	/// 深度書き込みの有効判定
//    bool isEnableDepthWritet() const					{ return mNodeRenderParam.RenderState.DepthWrite; }
//
//	/// カリング方法の設定
//    void setCullingMode( LNCullingMode mode )			{ mNodeRenderParam.RenderState.Culling = mode; }
//
//	/// カリング方法の取得
//    LNCullingMode getCullingMode() const				{ return mNodeRenderParam.RenderState.Culling; }
//
//	///// 回転順序の設定
//	//void setRotateOrder( Math::RotationOrder order )	{ mRotOrder = order; }
//
//	///// 回転順序の取得
//	//Math::RotationOrder getRotateOrder() const			{ return mRotOrder; }
//
//	///// 優先度の設定 (高い方から先に描画される)
//	//void setPriority( lnS32 priority )					{ mPriority = priority; }
//
//	///// 優先度の取得
//	//lnS32 getPriority() const							{ return mPriority; }
//  
//	///// ビルボード状態の設定
//	//void setBillboardType( LNBillboardType type )		{ mBillboardType = type; }
//
//	///// ビルボード状態の取得
//	//LNBillboardType getBillboardType() const			{ return mBillboardType; }
//
//	/// シェーダの設定
//	//void setShader( SceneShader* shader );
//
//	///// シェーダの取得
//	//SceneShader* getShader()							{ return mShader; }
//
//public:
//
//	
//
//protected:
//	VisualNode*					mOwnerNode;
//	VisualNodeRendererContext*	mRendererContext;
//	NodeRenderParam				mNodeRenderParam;
//	//Math::RotationOrder			mRotOrder;
// //   lnS32						mPriority;
// //   LNBillboardType				mBillboardType;
//	//SceneShader*				mShader;
//	//bool						mIsVisible;
//	
//    LMatrix						mCombinedWorldMatrix;
//
//	friend class VisualNodeRendererContext;
//
//	/*	■ mShader はここに置くべきか？
//	 *		「描画に必要な情報」ってことであればここでOK。
//	 *		ただし、SceneNode 側に「ポストエフェクトの場合は専用のリストに入れられる」という
//	 *		処理があるため、ここにおいても SceneNode に依存する必要がある。
//	 *		⇒でも、逆に言えば特殊な処理をしているのは今のところこれだけ。
//	 *		  とりあえずはいいかも。
//	 */
//	/*	■ 描画用のバックエンドデータ(Context) は flip? copy?
//	 *		flip の場合、クリティカルな処理はポインタの入れ替えだけにできる。
//	 *		ただし、ユーザー側が初期設定として設定したプロパティはそのままだと
//	 *		プライマリにしか反映されない。flip 時に更新しなおすとか、
//	 *		いずれにしても +αの処理が発生するし、そのあたりのケアが少し複雑になる。
//	 *		copy の場合は必要なプロパティを全てコピーする必要がある。
//	 *		それだけでも若干パフォーマンスは落ちるが、割と致命的なのは
//	 *		スキンメッシュのボーンや、頂点バッファのコピー。
//	 *		これは行列をひとつの配列にして memcpy か、最悪これだけ flip にするなど
//	 *		何かしらの対策が必要になる。
//	 *		ただ、それ以外は基本的に copy の方が単純なのでこっちベースで作る。
//	 */
//};
//
////=============================================================================
//// ■ VisualNodeRendererContext
////-----------------------------------------------------------------------------
/////**
////  @brief
////*/
////=============================================================================
//class VisualNodeRendererContext
//    : public MMESceneObjectBase
//{
//public:
//    VisualNodeRendererContext();
//    virtual ~VisualNodeRendererContext();
//
//public:
//
//	/// 作成
//    void create( VisualNodeRenderer* ownerRenderer );
//
//	/// [同期フェーズ] (とりあえず updateContext() から呼んでいる)
//	virtual void releaseAllResource();
//
//	/// [同期フェーズ]
//    virtual void updateContext( VisualNodeRendererContext* parentNodeContext );
//
//	/// [描画スレッド] 描画前処理
//    virtual void preRender( ContextDrawParam& param );
//
//	/// [描画スレッド] Node 単位のパラメータをシェーダに設定した後呼び出される (追加で設定したいパラメータがあるときにオーバーライドする)
//    virtual void onSetNodeParameter( SceneShader* shader ) {}
//
//	/// サブセット数の取得
//	virtual int getSubsetCount();
//
//	/// サブセットを描画する
//	virtual void drawSubset( int index ) {};
//
//public:
//	/// MMESceneObjectBase
//	virtual const lnChar*			getName() const { return mName.c_str(); }
//    virtual SceneShader*			getShader() { return mShader; }
//	virtual const LMatrix&			getMatrix() const { return mCombinedWorldMatrix; }
//	virtual bool					isVisible() const { return mIsVisible; }
//	virtual lnFloat					getOpacity() const { return mSubsetRenderParamArray[0].Opacity; }
//	//virtual void					setPriorityParameter( LNPriorityParameter* param ) { mPriorityParameter = param; }
//    virtual const MMELightArray&	getAffectLightArray() const { return mAffectLightContextArray; }
//    virtual bool					getObjectMatrix( LMatrix* outMat, MMEVariableRequest req, int lightIndex ) const;
//	virtual const LMatrix&			findFrameMarix( const lnChar* name ) const { return LMatrix::IDENTITY; }
//	virtual lnFloat					findMorphBlend( const lnChar* name ) const { return 0.0f; }
//	virtual void					render( ContextDrawParam& param );
//
//protected:
//	typedef std::vector<LNSubsetRenderParam>				SubsetRenderParamArray;
//	//typedef Base::ReferenceObjectArray<VisualNodeRenderer*>	VisualNodeRendererRefArray;
//
//	VisualNodeRenderer*				mOwnerRenderer;
//	//VisualNodeRendererContextArray	mChildContextArray;
//	MMELightArray					mAffectLightContextArray;	///< 描画に使うライト
//	CameraContext*					mAffectCameraContext;		///< 今回の描画に使うカメラ (preRender() でセット)
//
//	SceneShader*			mShader;
//    SubsetRenderParamArray	mSubsetRenderParamArray;
//	lnRefString				mName;
//    LMatrix                 mCombinedWorldMatrix;		///< 行列の階層更新で作成される結合済みワールド行列
//    LRenderState            mRenderState;
//	//LNPriorityParameter*    mPriorityParameter;
//    lnS32					mPriority;
//	bool                    mIsVisible;
//	bool					mEnableDrawing;				///< 視錐台カリング等で評価した後、描画するかどうか
//
//	lnFloat                 mZDistance;
//	LNBillboardType         mBillboardType;
//	//VisualNodeRendererRefArray	mVisualNodeRendererRefArray;
//};

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================