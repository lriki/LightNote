//==============================================================================
// BaseClasses 
//------------------------------------------------------------------------------
///**
//  @file       BaseClasses.h
//  @brief      BaseClasses
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../Base/BoolArray.h"
#include "../Common.h"
#include "MMETypes.h"

namespace LNote
{
namespace Core
{
namespace Scene
{
class MMELightBase;
class OffscreenScene;

typedef std::vector<MMELightBase*> MMELightArray;

struct OffscreenSceneFilterGroup
{
	class OffscreenScene*	OffscreenScene;
	lnS8					Group;
};

static const int MaxOffscreenScenes = 16;

//==============================================================================
// ■ MMEOffscreenSubsetInfo
//------------------------------------------------------------------------------
///**
//  @brief		シーンオブジェクトのベースクラス。オフスクリーンシーンごとに、各サブセット描画に使用するシェーダを管理する
//*/
//==============================================================================
class MMEOffscreenSubsetInfo
{
public:
	MMEOffscreenSubsetInfo();
	virtual ~MMEOffscreenSubsetInfo();

public:

	/// サブセット数 (setOffscreenSubsetShader() の前に確定させておくこと)
	virtual int getSubsetCount() const = 0;

	/// シェーダ設定
	void setOffscreenSubsetShader( OffscreenScene* scene, int subsetIndex, SceneShader* shader );

	/// シェーダ取得 (割り当てられていない場合は NULL)
	SceneShader* getOffscreenSubsetShader( OffscreenScene* scene, int subsetIndex );

	/// コピー
	void copyOffscreenSubsetInfo( const MMEOffscreenSubsetInfo& info );

private:

	void _deleteOffscreenSubsetArray( int index );

private:

	struct OffscreenSubset
	{
		SceneShader*	Shader;
	};

	typedef std::vector<OffscreenSubset>	OffscreenSubsetArray;

	OffscreenSubsetArray*	mOffscreenSubsets[MaxOffscreenScenes];
};
	
//==============================================================================
// ■ MMESceneObjectBase
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class MMESceneObjectBase
    : public Base::ReferenceObject
{
public:
	MMESceneObjectBase();
	virtual ~MMESceneObjectBase() {}

public:

	virtual const lnChar* getName() const = 0;

	// OffscreenScene の "self" の判別に使う (NULL 可)
	virtual SceneShader* getShader() = 0;
	virtual bool isVisible() const = 0;
	virtual const LMatrix& getMatrix() const = 0;
	virtual lnFloat getOpacity() const = 0;

	virtual const LMatrix& findFrameMarix( const lnChar* name ) const = 0;
	virtual lnFloat findMorphBlend( const lnChar* name ) const = 0;

	//virtual void setPriorityParameter( LNPriorityParameter* param ) = 0;
    virtual const MMELightArray&	getAffectLightArray() const = 0;
	virtual void render( ContextDrawParam& param ) = 0;

	// out に値を格納した場合は true を返す
    virtual bool getObjectMatrix( LMatrix* out, MMEVariableRequest req, int light_idx ) const = 0;

public:
	OffscreenSceneFilterGroup* getOffscreenSceneFilter( int id ) { return &mOffscreenSceneFilterGroup[id]; }

private:
	OffscreenSceneFilterGroup	mOffscreenSceneFilterGroup[MaxOffscreenScenes];
};

typedef std::vector<MMESceneObjectBase*> MMESceneObjectArray;



//==============================================================================
// ■ MMEViewport
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class MMEViewport
{
public:

	/// 何らかの Shader が削除されたときに呼ばれる
	virtual void removePostEffectShader( SceneShader* shader );
};

typedef std::vector<MMEViewport*> MMEViewportArray;


//==============================================================================
// ■ MMELightBase
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class MMELightBase
	//: public Base::ReferenceObject
{
public:
	virtual ~MMELightBase() {}

public:
	virtual const LMatrix& getViewMatrix() const = 0;   
    virtual const LMatrix& getProjectionMatrix() const = 0;
    virtual const LMatrix& getViewProjectionMatrix() const = 0;

	virtual const LVector3& getPosition() const = 0;
	virtual const LVector3& getDirection() const = 0;

	virtual const LColorF& getDiffuseColor() const = 0;
	virtual const LColorF& getAmbientColor() const = 0;
	virtual const LColorF& getSpecularColor() const = 0;
};

//==============================================================================
// ■ MMESceneBase
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class MMESceneBase
    : public Base::ReferenceObject
{
public:

	virtual MMELightBase* getDefaultMMELight() = 0;

	virtual MMESceneObjectArray& getMMESceneObjectArray() = 0;

	virtual MMESceneObjectBase* findMMESceneObject( const lnChar* name ) = 0;


	//// Shader 削除時に、ポストエフェクト系シェーダの参照を外すために使用する
	//virtual MMEViewportArray& getViewportList() = 0;

	/// テクスチャが作成されるときに参照されるベースサイズ
	//virtual const LVector2& getBaseViewSize() const = 0;
};

//==============================================================================
// ■ MMECameraBase
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class MMECameraBase
    : public Base::ReferenceObject
{
public:
	
	virtual const LNCameraSceneParam& getCameraSceneParam() = 0;// { return mLNCameraSceneParam; }

protected:
	//LNCameraSceneParam	mLNCameraSceneParam;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================