//==============================================================================
// Light 
//------------------------------------------------------------------------------
///**
//  @file       Light.h
//  @brief      Light
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Geometry/Geometry.h"
#include "../SceneGraph.h"
#include "SceneNode.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// ■ Light クラス
//------------------------------------------------------------------------------
///**
//  @brief      ライトを扱うクラス
//*/
//==============================================================================
class Light
    : public SceneNode//Base::ReferenceObject
{
public:
    Light( SceneGraph* scene_ );
    virtual ~Light();

public:

	/// 作成
	void create();

	/// 種類の取得
    LNLightType getType() const { return mType; }

	/// 種類の設定 (後で initlaize() に移動する)
    void setType( LNLightType type_ ) { mType = type_; }

	/// ライトの有効、無効を判定する
    bool isEnable() const { return mEnable; }

	/// ライトの有効、無効を設定する
    void setEnable( bool f_ ) { mEnable = f_; }

	/// 位置の設定
	//void setPosition( const LVector3& position_ ) { mPosition = position_; mNeedUpdateMatrices = true; }

	/// 位置の取得
    const LVector3& getPosition() const { return mPosition; }

	/// 向きの設定
    void setDirection( const LVector3& direction_ ) { mDirection = direction_; mNeedUpdateMatrices = true; }

	/// 向きの取得
    const LVector3& getDirection() const { return mDirection; }

	/// ディフューズカラーの設定
    void setDiffuseColor( const LColorF& color_ ) { mDiffuse = color_; }

	/// ディフューズカラーの取得
    const LColorF& getDiffuseColor() const { return mDiffuse; }

	/// アンビエントカラーの設定
    void setAmbientColor( const LColorF& color_ ) { mAmbient = color_; }

	/// アンビエントカラーの取得
    const LColorF& getAmbientColor() const { return mAmbient; }

	/// スペキュラカラーの設定
    void setSpecularColor( const LColorF& color_ ) { mSpecular = color_; }

	/// スペキュラカラーの取得
    const LColorF& getSpecularColor() const { return mSpecular; }

	/// ライティング優先度の設定 (適用するライトの列挙時、距離より優先する値 (大きい方が優先))
    void setLightingPriority( lnS32 priority_ ) { mLightingPriority = priority_; }

	/// ライティング優先度の取得
    lnU32 getLightingPriority() const { return mLightingPriority; }

	/// 有効距離の設定
    void setRange( lnFloat range_ ) { mRange = range_; }
    
	/// 有効距離の取得 (ライト列挙で、ポイント、スポットのときに視錐台に入るかの判定で使う)
    lnFloat getRange() const { return mRange; }


	/// スポットライトの内側コーン角度の設定 (ラジアン単位)
    void setSpotInAngle( lnFloat angle_ ) { mSpotInAngle = angle_; }

	/// スポットライトの内側コーン角度の取得 (ラジアン単位)
    lnFloat getSpotInAngle() const { return mSpotInAngle; }

	/// スポットライトの外側コーン角度の設定 (ラジアン単位)
    void setSpotOutAngle( lnFloat angle_ ) { mSpotOutAngle = angle_; }

	/// スポットライトの外側コーン角度の取得 (ラジアン単位)
    lnFloat getSpotOutAngle() const { return mSpotOutAngle; }

	/// シャドウの暗さの設定 (0.0～1.0)
	//void setShadowStrength( lnFloat strength_ ) { mShadowStrength = strength_; }

	/// シャドウの暗さの取得
	//lnFloat getShadowStrength() const { return mShadowStrength; }

	/// シャドウバッファを作成する
	//LNRESULT createShadowBuffer( lnU32 width_, lnU32 height_ );

	/// シャドウバッファとなるレンダーターゲットの取得
	//Graphics::ITexture* getShadowBuffer() const { return mShadowBuffer; }

	/// シャドウバッファとなるレンダーターゲットの設定 (参照カウントが変化します)
	//void setShadowBuffer( Core::Graphics::ITexture* tareget_ );

    
	/// ビュー行列の取得
    const LMatrix& getViewMatrix() const { _updateMatrices(); return mViewMatrix; }

	/// プロジェクション行列の取得
    const LMatrix& getProjectionMatrix() const { _updateMatrices(); return mProjectionMatrix; }

	/// ビュー行列とプロジェクション行列の積の取得
    const LMatrix& getViewProjectionMatrix() const { _updateMatrices(); return mViewProjMatrix; }

    void updateContext();

    LightContext* getLightContext() { return mLightContext; }

public:

	///// 現在の状態からビュー行列を作成して out_ に格納する
	//void getViewMatrix( LMatrix* out_, const LVector3& up_ = LVector3( 0, 1, 0 ) );

	///// 現在の状態からプロジェクション行列を作成して out_ に格納する
	//void getProjectionMatrix( LMatrix* out_, lnFloat fov_y_, lnFloat near_, lnFloat far_ );

	///// フレーム更新 (SceneBase::update() から呼ばれる)
	//void update();


public:

	/// ノードの種類の取得
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_LIGHT; }


	/// デバッグ用の描画
    virtual void drawDebug( lnU32 flags_ );

protected:

	/// mNeedUpdateMatrices が true の場合、行列計算を行う
    void _updateMatrices() const;

protected:

	//SceneGraph*             mScene;

    LNLightType             mType;              ///< ライトの種類
	//LVector3                    mPosition;          ///< 位置
    LVector3                mDirection;         ///< 向き

    LColorF                  mDiffuse;           ///< ディフューズカラー
    LColorF                  mAmbient;           ///< アンビエントカラー
    LColorF                  mSpecular;          ///< スペキュラカラー

    lnS32                   mLightingPriority;

    lnFloat                 mRange;             ///< 有効距離
    lnFloat                 mSpotInAngle;       ///< 内側のコーン角度 (ラジアン単位)
    lnFloat                 mSpotOutAngle;      ///< 外側のコーン角度 (ラジアン単位)

	//lnFloat                 mShadowStrength;    ///< シャドウの暗さ (0.0～1.0)
	//Graphics::ITexture*     mShadowBuffer;

    mutable LMatrix         mViewMatrix;        ///< ビュー行列
    mutable LMatrix         mProjectionMatrix;  ///< プロジェクション行列
    mutable LMatrix         mViewProjMatrix;    ///< ビュー行列とプロジェクション行列の積

    mutable LViewFrustum    mViewFrustum;       ///< シャドウバッファを使うときの視錐台
    
    LightContext*           mLightContext;

    bool                    mEnable;            ///< ライトが有効かどうかのフラグ
    mutable bool            mNeedUpdateMatrices;


    /*
    Intensity
	    光の明るさ。スポット/ポイントでは 1、ディレクショナルでは 0.5 デフォ。(unity)
    */

    friend class LightContext;
};

//==============================================================================
// ■ LightContext
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class LightContext
	: public MMELightBase
{
public:

    static LightContext DUMMY;

public:

    LightContext();

public:

    void initialize( Light* light_ );

    void updateContext();

public:

    bool isEnable() const { return mEnable; }

    LNLightType getType() const { return mType; }

    

    lnS32 getLightingPriority() const { return mLightingPriority; }

    lnFloat getRange() const { return mRange; }

    

	// ソート用の一時データ
    lnFloat DistanceForSort;

public:
	virtual const LVector3& getPosition() const { return mPosition; }
    virtual const LVector3& getDirection() const { return mDirection; }

protected:
	virtual const LMatrix& getViewMatrix() const { return mViewMatrix; }
    virtual const LMatrix& getProjectionMatrix() const { return mProjectionMatrix; }
    virtual const LMatrix& getViewProjectionMatrix() const { return mViewProjMatrix; }

    virtual const LColorF&   getDiffuseColor() const { return mDiffuse; }
    virtual const LColorF&   getAmbientColor() const { return mAmbient; }
    virtual const LColorF&   getSpecularColor() const { return mSpecular; }

private:

    Light*          mLight;
    LNLightType     mType;
    LVector3        mPosition;
    LVector3        mDirection;
    LColorF          mDiffuse;
    LColorF          mAmbient;
    LColorF          mSpecular;
    lnS32           mLightingPriority;
    lnFloat         mRange;
    lnFloat         mSpotInAngle;
    lnFloat         mSpotOutAngle;
    LMatrix         mViewMatrix;
    LMatrix         mProjectionMatrix;
    LMatrix         mViewProjMatrix;
    bool            mEnable;
};

} // namespace Scene
} // namespace Core
} // namespace LNote
