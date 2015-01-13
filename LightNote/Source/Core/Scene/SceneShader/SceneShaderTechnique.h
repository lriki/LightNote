//==============================================================================
// SceneShaderTechnique 
//------------------------------------------------------------------------------
///**
//  @file       SceneShaderTechnique.h
//  @brief      SceneShaderTechnique
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../Graphics/Interface.h"
#include "../../Graphics/DeviceObjects.h"
#include "../DrawCommand.h"
#include "MMETypes.h"
#include "SceneShader.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// �� SceneShaderTechnique
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class SceneShaderTechnique
{
public:
    SceneShaderTechnique();
    virtual ~SceneShaderTechnique();

public:

	/// ������
    void initialize( SceneShader* shader, Graphics::IShaderTechnique* tech );

	/// ���̃e�N�j�b�N���L���ł��邩�𒲂ׂ�
	bool isValid() const { return mValid; }

	/// �`��p�X���擾����
    MMDPass getMMDPass() const { return mMMDPass; }

	/// MMETechEnableFlag �̑g�ݍ��킹
	lnU32 getMMETechEnableFlags() const { return mFlags; }

	/// �X�N���v�g�� "ScriptExternal=Color" �����邩�ǂ����𔻒肷��
    bool isPostProcess() const { return ( mCommandExistFlags & MME_SCEXIST_ScriptExternal_Color ); }

	/// ����̃R�}���h�̑��݂��m�F���邽�߂̃t���O
    lnU32 getCommandExistFlags() const { return mCommandExistFlags; }

	/// �`��R�}���h���X�g�̎擾
	DrawingCommandList* getDrawingCommandList() { return &mDrawingCommandList; }

private:

	/// �X�N���v�g������̕ϐ����󂯎���� mScriptCommandList �ɃR�}���h��ǉ����Ă���
	//      pass_ : �p�X���̃X�N���v�g����͂���ꍇ�A���̃p�X��n�� (Draw="Geometry" �̎��s�ɓn��pass)
    bool _lexScriptCommandString( Graphics::IShaderVariable* var, Graphics::IShaderPass* pass, DrawingCommandList* commandList );

private:

    SceneShader*						mSceneShader;
    Graphics::IShader*					mCoreShader;
    Graphics::IShaderTechnique*			mTechnique;
    MMDPass								mMMDPass;
    lnU32								mFlags;					///< MMETechEnableFlag �̑g�ݍ��킹
	lnU32								mCommandExistFlags;     ///< MMEScriptCommandExistFlag �̑g�ݍ��킹

    SubsetIndexArray					mSubsetIndexArray;
	DrawingCommandList					mDrawingCommandList;			
	std::vector<DrawingCommandList*>	mPassDrawingCommandListArray;	///< �p�X���̃R�}���h (mDrawingCommandList �̎q����)
	bool								mValid;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================