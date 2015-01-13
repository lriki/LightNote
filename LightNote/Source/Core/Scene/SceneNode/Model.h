//==============================================================================
// Model 
//------------------------------------------------------------------------------
///**
//  @file       Model.h
//  @brief      Model
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../Graphics/Common/Model/Model.h"
#include "SceneNode.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// Model
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class Model
    : public VisualNode
{
	LN_TYPE_INFO_ACCESS_DECL;
public:
    Model( SceneGraph* scene );
    virtual ~Model();

public:

	/// çÏê¨
    void create( const lnChar* filePath, lnU32 flags = 0, bool asyncLoad = false );

	Graphics::Model2* getModelSource() { return mModel; }

public:
	// VisualNode
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_MODEL; }
	virtual void updateBeforePhysics();
    virtual void update( float elapsedTime );
	virtual void updateAfterPhysics();
	
	// FileIO::AsyncLoadObject
	virtual bool onLoadProc();
	virtual void onLoadProcFinished();

private:
	
	// îÒìØä˙ì«Ç›çûÇ›àÍéûïœêî
	lnRefString			mModelFilePath;
	lnU32				mModelCreateFlags;
	Graphics::Model2*	mModelForAsyncLoad;
	SceneShader*		mShaderForAsyncLoad;



    Graphics::Model2*	mModel;
    Graphics::Texture*	mSkinningBoneTexture;
    
    class DrawingContext;
    friend class DrawingContext;
};

} // namespace Scene
} // namespace Core
} // namespace LNote
