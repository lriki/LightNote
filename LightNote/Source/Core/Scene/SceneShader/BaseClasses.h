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
// �� MMEOffscreenSubsetInfo
//------------------------------------------------------------------------------
///**
//  @brief		�V�[���I�u�W�F�N�g�̃x�[�X�N���X�B�I�t�X�N���[���V�[�����ƂɁA�e�T�u�Z�b�g�`��Ɏg�p����V�F�[�_���Ǘ�����
//*/
//==============================================================================
class MMEOffscreenSubsetInfo
{
public:
	MMEOffscreenSubsetInfo();
	virtual ~MMEOffscreenSubsetInfo();

public:

	/// �T�u�Z�b�g�� (setOffscreenSubsetShader() �̑O�Ɋm�肳���Ă�������)
	virtual int getSubsetCount() const = 0;

	/// �V�F�[�_�ݒ�
	void setOffscreenSubsetShader( OffscreenScene* scene, int subsetIndex, SceneShader* shader );

	/// �V�F�[�_�擾 (���蓖�Ă��Ă��Ȃ��ꍇ�� NULL)
	SceneShader* getOffscreenSubsetShader( OffscreenScene* scene, int subsetIndex );

	/// �R�s�[
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
// �� MMESceneObjectBase
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

	// OffscreenScene �� "self" �̔��ʂɎg�� (NULL ��)
	virtual SceneShader* getShader() = 0;
	virtual bool isVisible() const = 0;
	virtual const LMatrix& getMatrix() const = 0;
	virtual lnFloat getOpacity() const = 0;

	virtual const LMatrix& findFrameMarix( const lnChar* name ) const = 0;
	virtual lnFloat findMorphBlend( const lnChar* name ) const = 0;

	//virtual void setPriorityParameter( LNPriorityParameter* param ) = 0;
    virtual const MMELightArray&	getAffectLightArray() const = 0;
	virtual void render( ContextDrawParam& param ) = 0;

	// out �ɒl���i�[�����ꍇ�� true ��Ԃ�
    virtual bool getObjectMatrix( LMatrix* out, MMEVariableRequest req, int light_idx ) const = 0;

public:
	OffscreenSceneFilterGroup* getOffscreenSceneFilter( int id ) { return &mOffscreenSceneFilterGroup[id]; }

private:
	OffscreenSceneFilterGroup	mOffscreenSceneFilterGroup[MaxOffscreenScenes];
};

typedef std::vector<MMESceneObjectBase*> MMESceneObjectArray;



//==============================================================================
// �� MMEViewport
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class MMEViewport
{
public:

	/// ���炩�� Shader ���폜���ꂽ�Ƃ��ɌĂ΂��
	virtual void removePostEffectShader( SceneShader* shader );
};

typedef std::vector<MMEViewport*> MMEViewportArray;


//==============================================================================
// �� MMELightBase
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
// �� MMESceneBase
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


	//// Shader �폜���ɁA�|�X�g�G�t�F�N�g�n�V�F�[�_�̎Q�Ƃ��O�����߂Ɏg�p����
	//virtual MMEViewportArray& getViewportList() = 0;

	/// �e�N�X�`�����쐬�����Ƃ��ɎQ�Ƃ����x�[�X�T�C�Y
	//virtual const LVector2& getBaseViewSize() const = 0;
};

//==============================================================================
// �� MMECameraBase
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