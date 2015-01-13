//==============================================================================
// Mesh 
//------------------------------------------------------------------------------
///**
//  @file       Mesh.h
//  @brief      Mesh
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
// Mesh
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class Mesh
    : public VisualNode
{
public:
    Mesh( SceneGraph* scene );
    virtual ~Mesh();

public:

	/// �쐬 (�~���t�H���_�ɓ�����.fx�t�@�C��������ꍇ�͂����ǂݍ���Őݒ肷��)
	//		meshCreateFlags �� ModelCreateFlag �̑g�ݍ��킹
    void create( const lnChar* filePath, lnU32 meshCreateFlags = 0, bool asyncLoad = false );

	/// ���b�V���f�[�^�̎擾
    Graphics::Model* getMeshSource() { return mModel; }

public:
	// VisualNode
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_MODEL; }
    virtual void update( float elapsedTime );

	// FileIO::AsyncLoadObject
	virtual bool onLoadProc();
	virtual void onLoadProcFinished();

private:

	
	lnRefString			mModelFilePath;
	lnU32				mModelCreateFlags;

	Graphics::Model*    mModelForAsyncLoad;
	SceneShader*		mShaderForAsyncLoad;

    Graphics::Model*    mModel;
    Graphics::Texture* mSkinningBoneTexture;
    
    class DrawingContext;
    friend class DrawingContext;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================