//==============================================================================
// SceneShader 
//------------------------------------------------------------------------------
///**
//  @file       SceneShader.h
//  @brief      SceneShader
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../Graphics/Interface.h"
#include "../../Graphics/DeviceObjects.h"
#include "../Common.h"
#include "MMETypes.h"
#include "SceneShaderManager.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// �� SceneShaderUtil
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class SceneShaderUtil
{
public:

	/// �e�N�X�`�����\�[�X�̐������
    struct TextureResourceData
    {
        LRefTString         ResourceName;	// ��{�I�̑��΃p�X
        LVector3            Dimensions;
        LVector2            ViewportRatio;
        int                 Miplevels;
        Graphics::SurfaceFormat		Format;

        // �A�j���[�V�����e�N�X�`���p
        lnFloat             Offset;
        lnFloat             Speed;
        LRefTString         SeekVariable;

        // �I�t�X�N���[��RT�p
        LVector4            ClearColor;
        lnFloat             ClearDepth;
        bool                AntiAlias;
        LRefTString         Description;
        LRefTString         DefaultEffect;
    };

public:

	//----------------------------------------------------------------------
	///**
	//  @brief      �V�F�[�_�ϐ��̃Z�}���e�B�N�X�𒲂ׂđΉ�����v�����ڂ�Ԃ�
	//
	//  @param[out] script_output_  : STANDARDSGLOBAL �� ScriptOutput �̒l���i�[����ϐ��̃A�h���X (��� "color")
	//  @param[out] script_class_   : �G�t�F�N�g�t�@�C���̎g�p�ړI (�f�t�H���g�� "object")
	//  @param[out] script_order_   : �G�t�F�N�g�t�@�C���̎��s�^�C�~���O (�f�t�H���g�� "standard")            
	//  @param[out] is_controllobject_ : �L���� CONTROLOBJECT �Z�}���e�B�N�X�̏ꍇ�Atrue ���i�[�����
	//
	//  @note
	//              script_output_�Ascript_class_�Ascript_order_ �� STANDARDSGLOBAL ��
	//              ����ꍇ�̂ݐݒ肳���B���̊֐��̌Ăяo������ sv_->Request == MME_VARREQ_STANDARDSGLOBAL ��
	//              �`�F�b�N��A�ϐ��̒��g���m�F���邱�ƁB
	//*/
	//----------------------------------------------------------------------
    static void checkVariableRequest(
        Graphics::IShaderVariable* var_,    // const��������
        const MMESemanticsMap& semantics_map_,
        MMEShaderVariable* sv_,
        MMEScriptOutput* script_output_,
        MMEScriptClass* script_class_,
        MMEScriptOrder* script_order_,
        bool* is_controllobject_ );

	/// �ϐ��� "Object" �A�m�e�[�V�����𒲂ׂāA"Light" �̏ꍇ�� fales�A����ȊO("Camera") �� true ��Ԃ�
    static bool checkAnnotationCameraOrLight( Graphics::IShaderVariable* var_ );

	/// �ϐ��� "Object" �A�m�e�[�V�����𒲂ׂāA"Geometry" �̏ꍇ�� 1�A"Light" �̏ꍇ�� 2�A����ȊO�� 0 ��Ԃ�
    static int checkAnnotationGeometryOrLight( Graphics::IShaderVariable* var_ );

	/// ���O���w�肵�ăA�m�e�[�V�������������� (�啶���������̋�ʖ����BIShaderVariable ����͑召��������ʂ���ł������Ȃ��̂ŗp��)
    static Graphics::IShaderVariable* getAnnotationByName( Graphics::IShaderVariable* var_, const lnChar* name_ );

	/// �v�����ڂ���A�v�Z����K�v�̂���s��}�X�N�̃r�b�g����쐬���ĕԂ�
    static lnU32 checkMatrixMask( MMEVariableRequest req_ );

	/// �e�N�X�`�����\�[�X���쐬����K�v�̂���v�����𔻒肷��
    static bool needCreateTexture( MMEVariableRequest req_ ) { return ( MME_VARREQ_TEXTURE <= req_ && req_ <= MME_VARREQ_OFFSCREENRENDERTARGET ); }

	//----------------------------------------------------------------------
	///**
	//  @brief      �e�N�X�`�����\�[�X�̏���Ԃ�
	//  
	//  @return     �e�N�X�`�������K�v������ꍇ (�ϐ��̃Z�}���e�B�N�X���e�N�X�`���n) true ��Ԃ�
	//*/
	//----------------------------------------------------------------------
    static bool checkTextureResourceData(
        MMEShaderVariable* sv_,  // ����� const �ɂ������c
        const MMEAnnotationMap& annotation_map_,
        TextureResourceData* data_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      �e�N�X�`�����\�[�X�̏�񂩂�A�e�N�X�`���T�C�Y��Ԃ�
	//
	//  @param[in]  viewport_size_ : �V�[���̉��z�X�N���[���T�C�Y
	//*/
	//----------------------------------------------------------------------
    static void checkTextureSize(
        int* width_,
        int* height_,
        const LVector3& dimensions_,
        const LVector2& viewport_ratio_,
        const LVector2& viewport_size_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      �`��T�u�Z�b�g�����������񂩂�A�T�u�Z�b�g�ԍ��̔z����쐬����
	//
	//  @note
	//              '-' �ɂ��͈͎w��Łux�Ԉȍ~���ׂāv��`�悷��ꍇ�A
	//              �z��̏I�[�ɂ� LN_INVALID_INDEX ���i�[�����B
	//*/
	//----------------------------------------------------------------------
    static void makeSubsetIndexArray( SubsetIndexArray* list_, const lnChar* subset_text_ );

	/// CONTROLLOBJECT �ݒ� (obj �� NULL �\)
	static void setControllObjectParam( MMEShaderVariable* sv, const MMESceneObjectBase* obj );

private:

    static const int MAX_SEMANTICS_NAME_LEN = 256;

    struct SubsetIndexOrder
    {
        lnU32   Index;
        int     RangeState;    ///< �͈͂̎w�� (0=�Ȃ� 1=����SubsetIndex�̒l�܂� 2=�T�u�Z�b�g���̍ő�܂�)
    };

    typedef std::list< SubsetIndexOrder > SubsetIndexOrderList;

};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================