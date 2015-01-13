//==============================================================================
// Camera 
//------------------------------------------------------------------------------
///**
//  @file       Camera.h
//  @brief      Camera
//*/
//==============================================================================

#pragma once

#include "../../Graphics/Common/Camera.h"
#include "../../Graphics/Common/ScreenEffect.h"
#include "../Common.h"
#include "SceneNode.h"

#define LNOTE_SCENE_CAMERA_SCREEN_EFFECT	0

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// ■ CameraContext
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class CameraContext
{
public:

    CameraContext();
    virtual ~CameraContext();
    
public:

    void initialize( Camera* camera_ );

    virtual void updateContext();

	void render( ContextDrawParam* param );
	
public:
	SceneNode*			getRenderRootNode() { return mRenderRootNode; }
	//Graphics::ITexture*	getRenderTarget() { return mRenderTarget; }

	const LVector2& getViewSize() const { return mViewSize; }
	const LMatrix&	getViewProjectionMatrix() const { return mViewProjMatrix; }
	bool			isProjection2D() const { return mProjection2D; }
	const LMatrix&	getProjectionMatrix() const { return mProjectionMatrix; }
	const LMatrix&	getViewMatrixInverse() const { return mCameraSceneParam.Mat_CameraView_Inverse; }
	lnFloat			getFovY() const { return mFovY; }
	lnFloat			getNearClip() const { return mNearClip; }
	lnFloat			getFarClip() const { return mFarClip; }
	const LMatrix&	getViewMatrix() const { return mViewMatrix; }
	const LNCameraSceneParam& getCameraSceneParam() const { return mCameraSceneParam; }









	const LVector4& getPosition() const { return mPosition; }

    const LVector3& getDirection() const { return mDirection; }

	

    

	// (ビルボード行列の計算で使用。setProjectionMatrix() 後でないと使えないので注意)
        

    

    LNZSortFunc getZSortType() const { return mZSortFunc; }


	/// SceneGraphContext::OnSetActivePaneContext() から設定される
    void setProjectionMatrix( const LMatrix& proj_ );

    

	/// WORLDVIEWPROJECTION セマンティクス等のために使う
    

private:

    Camera*             mCamera;
	SceneNode*			mRenderRootNode;
	//Graphics::ITexture*	mRenderTarget;

    lnFloat             mFovY;
    lnFloat             mNearClip;
    lnFloat             mFarClip;
	LVector2			mViewSize;
	LMatrix             mViewMatrix;
    LMatrix             mProjectionMatrix;
	LMatrix             mViewProjMatrix;
	bool                mProjection2D;





    LVector4	        mPosition;
    LVector3            mDirection;

	// fovY 等は、Pane によるプロジェクション行列作成のために必要
    
    
       ///< ビュー行列とプロジェクション行列の積

    LNZSortFunc         mZSortFunc;
    LNCameraSceneParam  mCameraSceneParam;  ///< カメラ単位でシェーダに送るデータ

#if LNOTE_SCENE_CAMERA_SCREEN_EFFECT
    Graphics::ScreenEffect* mScreenEffect;
    LColorF				    mBackgroundColor;
#endif
    bool					mOrthographic;

	
};


//==============================================================================
// ■ Camera
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class Camera
    : public Base::ReferenceObject//SceneNode//
{
	LN_TYPE_INFO_ACCESS_DECL;
public:
    Camera( SceneGraph* scene );
    virtual ~Camera();

public:

	// 初期化
    void initialize( bool projection_2d_ );

	/// カメラタイプの設定
	void setCameraType( Graphics::CameraType type ) { mCoreCamera.setCameraType( type ); }

	/// 位置の設定
    void setPosition( const LVector3& pos ) { mCoreCamera.setPosition( pos ); }

	/// 位置の取得
	const LVector3& getPosition() const { return mCoreCamera.getPosition(); }

	/// 注視点の設定
	void setLookAt( const LVector3& at ) { mCoreCamera.setLookAt( at ); }

	/// 注視点の取得
	const LVector3& getLookAt() const { return mCoreCamera.getPosition(); }

	/// 上方向ベクトルの設定
	void setUpDirection( const LVector3& up ) { mCoreCamera.setUpDirection( up ); }

	/// 上方向ベクトルの取得
	const LVector3& getUpDirection() const { return mCoreCamera.getUpDirection(); }

	/// 向きの取得 (正規化済み)
    const LVector3& getDirection() const { return mCoreCamera.getDirection(); }

	void setAngle( const LVector3& angle ) { mCoreCamera.setAngle( angle ); }

	void setMatrix( const LMatrix& matrix ) { mCoreCamera.setMatrix( matrix ); }

	/// Y 方向視野角の設定
	void setFovY( lnFloat fov_y ) { mCoreCamera.setFovY( fov_y ); }

	/// Y 方向視野角の取得
	lnFloat getFovY() const { return mCoreCamera.getFovY(); }

	/// 最も近いビュープレーン位置の設定
	void setNearClip( lnFloat near_clip ) { mCoreCamera.setNearClip( near_clip ); }

	/// 最も近いビュープレーン位置の取得
	lnFloat getNearClip() const { return mCoreCamera.getNearClip(); }

	/// 最も遠いビュープレーン位置の設定
	void setFarClip( lnFloat far_clip ) { mCoreCamera.setFarClip( far_clip ); }

	/// 最も遠いビュープレーン位置の取得
	lnFloat getFarClip() const { return mCoreCamera.getFarClip(); }

	/// ビュー行列の取得
    const LMatrix& getViewMatrix() const { return mCoreCamera.getViewMatrix(); }

	/// プロジェクション行列の取得
	const LMatrix& getProjectionMatrix() const { return mCoreCamera.getProjectionMatrix(); }

	/// ビュー行列とプロジェクション行列の積の取得
	const LMatrix& getViewProjectionMatrix() const { return mCoreCamera.getViewProjectionMatrix(); }

	/// ビュー行列の逆行列の取得
	const LMatrix& getViewMatrixInverse() const { return mCoreCamera.getViewMatrixInverse(); }

	/// 視錐台の取得
    const LViewFrustum& getViewFrustum() const { return mCoreCamera.getViewFrustum(); }

#if LNOTE_SCENE_CAMERA_SCREEN_EFFECT
public:

	/// スクリーン特殊効果の使用設定
    void setScreenEffectMode( LNScreenEffectMode mode );

	/// 色調の設定
    void setTone( const LTone& tone, double duration ) { mScreenEffect->setTone( tone, duration ); }

	/// トランジションの準備として現在の画面を固定する
    void freeze() { mScreenEffect->freeze(); }

	/// トランジション実行
    void transition( double duration, Graphics::ITexture* mask, int vague = 40 ) { mScreenEffect->transition( duration, mask, vague ); }

	//----------------------------------------------------------------------
	///**
	//  @brief      ブラー実行
	//
	//  @param[in]  duration_     : 適用する時間 (秒)
	//  @param[in]  power_        : ブラーの強さ (ブラーイメージを描画する時のアルファ値。0.0～1.0 で指定)
	//  @param[in]  scale_        : ブラーの大きさ (長さ)
	//  @param[in]  center_       : ブラーの中心座標
	//  @param[in]  center_is_3d_ : center_ の座標情報が示す座標空間 (true の場合、3D 空間上の点)
	//  @par
	//              duration_ が 0 より大きい場合、値分の時間をかけて<br>
	//              徐々に power_ を 0 に戻します。
	//              <br>
	//              duration_ が 0 の場合は強さを自動で変化させません。<br>
	//              元に戻すためには、もう一度この関数を呼んで power_ を 0 にする必要があります。<br>
	//              <br>
	//              <br>
	//              scale_ は元のスクリーンイメージに対して、1フレーム当たりどの程度拡大するかを指定します。<br>
	//              1.0 でスクリーンと等倍 (変化なし)、2.0 でスクリーンの2倍の大きさとなります。<br>
	//              scale_ が 1.0 の場合、ブラーは放射状に拡散しません。<br>
	//              通常は 1.0～1.1 の範囲で指定するのが無難です。<br>
	//              また、1.0 より小さい値を指定すると内側向きのブラーになります。<br>
	//              <br>
	//              <br>
	//              center_is_3d_ は、center_ が 2D 空間上の座標か、3D 空間上の座標かを区別するために指定します。<br>
	//              <br>
	//              false が指定されている場合、center_ の値がそのままブラーの中心座標になります。<br>
	//              中心座標はレイヤーの左上を原点(0, 0)として、ピクセル単位で指定します。<br>
	//              <br>
	//              true が指定されている場合、center_ を3D空間上の点として、このレイヤーにセットされているカメラから見た
	//              スクリーン座標上の点を中心とします。<br>
	//              <br>    
	//*/
	//----------------------------------------------------------------------
    void blur(
        double duration,
        lnFloat power,
        lnFloat scale,
        const LVector3& center,
        bool center_is_3d );

	/// ブラー適用中かを判定する
    bool isRunningBlurEffect() const { return mScreenEffect->isBlurEffect(); }

	/// エフェクト適用中かを判定する
    bool isRunningScreenEffect() const { return mScreenEffect->isEffect(); }

	/// フレーム更新
	void updateFrame( const Game::GameTime& gameTime );

#endif

public:

	/// 描画ツリーのルートノードの設定
	void setRenderRootNode( SceneNode* node ) { mRenderRootNode = node; }

	/// 描画ツリーのルートノードの取得
	SceneNode* getRenderRootNode() const { return mRenderRootNode; }

	/// 描画先スクリーンとなるレンダリングターゲットの設定
	//void setRenderTarget( Graphics::ITexture* texture );

	/// 描画先ビューサイズ
	const LVector2& getViewSize() const;

	/// 2D カメラとして作成されているか
	bool isProjection2D() const { return mCoreCamera.isProjection2D(); }



	/// カメラ操作クラスの設定 (参照カウントはインクリメントしないが、デストラクト時に release())
    void setCameraOperator( Graphics::CameraOperator* camera_operator ) { mCoreCamera.setCameraOperator( camera_operator ); }
    
	/// Z ソート方法の設定
    void setZSortType( LNZSortFunc type_ ) { mZSortFunc = type_; }

	/// Z ソート方法の取得
    LNZSortFunc getZSortType() const { return mZSortFunc; }

	/// カメラから見た 3D 座標を 2D 座標に変換する
    void convertWorldPosToScreenPos( LVector3* out, const LVector3& world_pos ) const;


    LNDrawCoordSpace getCoordSpace() const { return ( isProjection2D() ) ? LN_DRAWCOORD_2D : LN_DRAWCOORD_3D; }


	
	void onUpdateViewSize( const LVector2& viewSize ) { mCoreCamera.setViewSize( viewSize ); }
	/// 
    void updateContext();

    CameraContext* getCameraContext() { return mContext; }

public:
	//void setParentPane( Pane* pane ) { mParentPane = pane; }
	//Pane* getParentPane() { return mParentPane; }
	void setParentLayer(DrawingLayer* pane) { mParentLayer = pane; }
	DrawingLayer* getParentLayer() { return mParentLayer; }

	/// カメラ単位でシェーダに送るデータの取得 (行列演算などいろいろやってるので 1 フレーム複数回呼ぶのは避けること)
    LNCameraSceneParam* getCameraSceneParam();

	/// (called from SceneGraph::onEvent)
    bool onEvent( const System::EventArgs& e );

#if LNOTE_SCENE_CAMERA_SCREEN_EFFECT
private:
	void _createScreenEffectResource();
	void _disposeScreenEffectResource();
#endif

protected:

    SceneGraph*             mScene;
	//Pane*					mParentPane;
	DrawingLayer*			mParentLayer;
    Graphics::Camera        mCoreCamera;
	//LVector2				mViewSize;
    lnU32					mClearFlag;         ///< LNClearFlag の組み合わせ
    bool                    mOrthographic;      ///< 遠近感シミュレートフラグ

    LNZSortFunc             mZSortFunc;
    LNCameraSceneParam      mCameraSceneParam;  ///< カメラ単位でシェーダに送るデータ
    CameraContext*          mContext;

    mutable bool            mMemberChanged;
     
	//bool                    mProjection2D;

#if LNOTE_SCENE_CAMERA_SCREEN_EFFECT
	LNScreenEffectMode		mScreenEffectMode;
	Graphics::ScreenEffect* mScreenEffect;
    LColorF				    mBackgroundColor;
#endif

	SceneNode*				mRenderRootNode;
	//Graphics::ITexture*		mRenderTarget;

    friend class CameraContext;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================