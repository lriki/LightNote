//==============================================================================
// Common 
//------------------------------------------------------------------------------
///**
//  @file       Common.h
//  @brief      Common
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include <list>
#include <map>

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{

class SceneNode;
class SceneShaderManager;
class MMETextureObject;

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

/// MME �Z�}���e�B�N�X�ꗗ
enum MMESemantics
{
    MME_SEMANTIC_NONE = 0,

    MME_SEMANTIC_WORLD,
    MME_SEMANTIC_VIEW,
    MME_SEMANTIC_PROJECTION,
    MME_SEMANTIC_WORLDVIEW,
    MME_SEMANTIC_VIEWPROJECTION,
    MME_SEMANTIC_WORLDVIEWPROJECTION,

    MME_SEMANTIC_WORLD_I,
    MME_SEMANTIC_VIEW_I,
    MME_SEMANTIC_PROJECTION_I,
    MME_SEMANTIC_WORLDVIEW_I,
    MME_SEMANTIC_VIEWPROJECTION_I,
    MME_SEMANTIC_WORLDVIEWPROJECTION_I,

    MME_SEMANTIC_WORLD_T,
    MME_SEMANTIC_VIEW_T,
    MME_SEMANTIC_PROJECTION_T,
    MME_SEMANTIC_WORLDVIEW_T,
    MME_SEMANTIC_VIEWPROJECTION_T,
    MME_SEMANTIC_WORLDVIEWPROJECTION_T,

    MME_SEMANTIC_WORLD_IT,
    MME_SEMANTIC_VIEW_IT,
    MME_SEMANTIC_PROJECTION_IT,
    MME_SEMANTIC_WORLDVIEW_IT,
    MME_SEMANTIC_VIEWPROJECTION_IT,
    MME_SEMANTIC_WORLDVIEWPROJECTION_IT,

    MME_SEMANTIC_DIFFUSE,
    MME_SEMANTIC_AMBIENT,
    MME_SEMANTIC_EMISSIVE,
    MME_SEMANTIC_SPECULAR,
    MME_SEMANTIC_SPECULARPOWER,
    MME_SEMANTIC_TOONCOLOR,
    MME_SEMANTIC_EDGECOLOR,

    MME_SEMANTIC_POSITION,          ///< ���C�g�܂��̓J�����́A���[���h��ԓ��ɂ�����ʒu
    MME_SEMANTIC_DIRECTION,         ///< ���C�g�܂��̓J�����́A���[���h��ԓ��ɂ��������

    MME_SEMANTIC_MATERIALTEXTURE,       ///< �}�e���A���ɐݒ肳��Ă���e�N�X�`��
    MME_SEMANTIC_MATERIALSPHEREMAP,     ///< �}�e���A���ɐݒ肳��Ă���X�t�B�A�}�b�v�e�N�X�`��

    MME_SEMANTIC_VIEWPORTPIXELSIZE,     ///< �����_�����O�^�[�Q�b�g�̃X�N���[���̃T�C�Y (Scene �̕`���RT�������̓I�t�X�N���[��RT�̃T�C�Y�BScripts��"RenderColorTarget"�R�}���h�ŁART��ύX���Ă��A���̒l�͕ω����Ȃ�)

    MME_SEMANTIC_TIME,                  ///< 0�t���[���ڂ���̍Đ����� (�b�B���f���P�ʁB�A�j���Đ����A�܂��͎��Ԑݒ莞�Ɍ��ݎ��Ԃ���������)
    MME_SEMANTIC_ELAPSEDTIME,           ///< �O��̕`�悩��̌o�ߎ��� (�b)

    MME_SEMANTIC_MOUSEPOSITION,         ///< [float2] �}�E�X�̌��݈ʒu (�X�N���[�����S��(0,0)�A��������(-1,-1)�A�E�����(1,1)�ɂȂ�)
    MME_SEMANTIC_LEFTMOUSEDOWN,         ///< [float4] �Ō�Ƀ{�^���������ꂽ�Ƃ��̃}�E�X�̍��W(x, y)�A���݃{�^����������Ă��邩�i0 or 1�j�A�Ō�Ƀ{�^���������ꂽ���_��TIME�l(�b)
    MME_SEMANTIC_MIDDLEMOUSEDOWN,   
    MME_SEMANTIC_RIGHTMOUSEDOWN,

    MME_SEMANTIC_CONTROLOBJECT,         ///< �w�肵���I�u�W�F�N�g�́A���W�⃏�[���h�ϊ��s����擾����(�^�ɂ���ď��̎�ނ��قȂ�)

    MME_SEMANTIC_RENDERCOLORTARGET,
    MME_SEMANTIC_RENDERDEPTHSTENCILTARGET,
    MME_SEMANTIC_ANIMATEDTEXTURE,
    MME_SEMANTIC_OFFSCREENRENDERTARGET,
    MME_SEMANTIC_TEXTUREVALUE,

    MME_SEMANTIC_STANDARDSGLOBAL,


	// �ȉ��͓Ǝ��d�l
	//LN_SEMANTIC_ORTHOGRAPHIC,       ///< �J�����r���[�T�C�Y����쐬�������ˉe�s�� (���얢�m�F)
    LN_SEMANTIC_PROJECTION2D,       ///< �������g��Ȃ��B�����Ă���

    LN_SEMANTIC_OPACITY,            ///< �s�����x
    LN_SEMANTIC_COLOR_SCALE,        ///< ��Z����F
    LN_SEMANTIC_BLEND_COLOR,        ///< �u�����h����F
    LN_SEMANTIC_TONE,               ///< �F��
    LN_SEMANTIC_UVTRANSFORM,        ///< �e�N�X�`�����W�ϊ��s��


    MME_MAX_SEMANTICS       ///< �Z�}���e�B�N�X�̍ő吔
};

/// MME �A�m�e�[�V�����ꗗ
enum MMEAnnotation
{
    MME_ANNOTATION_NONE = 0,

    MME_ANNOTATION_Object,          ///< �W�I���g���ϊ��A���C�g�A�}�e���A���A�ʒu�A�����̑Ώ�
    MME_ANNOTATION_SyncInEditMode,  ///< ���ԏ��̃t���[���A���t���O

    MME_ANNOTATION_Name,            ///< CONTROLOBJECT �̑ΏۃI�u�W�F�N�g��
    MME_ANNOTATION_Item,            ///< CONTROLOBJECT �̓���l��

    MME_ANNOTATION_ResourceType,    ///< �e�N�X�`�����\�[�X�̎��
    MME_ANNOTATION_ResourceName,    ///< �e�N�X�`�����\�[�X�̃t�@�C����
    MME_ANNOTATION_Width,           ///< �e�N�X�`���̕�
    MME_ANNOTATION_Height,          ///< �e�N�X�`���̍���
    MME_ANNOTATION_Depth,           ///< �e�N�X�`���̐[��
    MME_ANNOTATION_Dimensions,      ///< �e�N�X�`���̃f�B�����W����
    MME_ANNOTATION_ViewportRatio,   ///< �e�N�X�`���̃r���[�ɑ΂���䗦
    MME_ANNOTATION_Format,          ///< �e�N�X�`���̃T�[�t�F�C�X�t�H�[�}�b�g
    MME_ANNOTATION_Miplevels,       ///< �e�N�X�`���̃~�b�v���x��
    MME_ANNOTATION_Levels,          ///< �e�N�X�`���̃~�b�v���x��

    MME_ANNOTATION_Offset,          ///< �A�j���[�V�����e�N�X�`���̊J�n���Ԃ̂��炵��
    MME_ANNOTATION_Speed,           ///< �A�j���[�V�����e�N�X�`���̍Đ��X�s�[�h
    MME_ANNOTATION_SeekVariable,    ///< �A�j���[�V�����e�N�X�`���̃V�[�N������@

    MME_ANNOTATION_ClearColor,      ///< �I�t�X�N���[��RT�̃N���A�F
    MME_ANNOTATION_ClearDepth,      ///< �I�t�X�N���[��RT�̐[�x�N���A�l
    MME_ANNOTATION_AntiAlias,       ///< �I�t�X�N���[��RT�̃A���`�G�C���A�X�L���t���O�@
    MME_ANNOTATION_Description,     ///< �I�t�X�N���[��RT�̐�����
    MME_ANNOTATION_DefaultEffect,   ///< �I�t�X�N���[��RT�̃f�t�H���g�G�t�F�N�g�t�@�C����

    MME_ANNOTATION_TextureName,     ///< TEXTUREVALUE �̑Ώۃe�N�X�`���ϐ���

    MME_ANNOTATION_ScriptOutput,    ///< "color" �ŌŒ�
    MME_ANNOTATION_ScriptClass,     ///< STANDARDSGLOBAL �Ŏw�肳���G�t�F�N�g�t�@�C���̖ړI
    MME_ANNOTATION_ScriptOrder,     ///< STANDARDSGLOBAL �Ŏw�肳���G�t�F�N�g�t�@�C���̎��s�^�C�~���O
    
    MME_ANNOTATION_Script,          ///< �e�X�N���v�g���

    MME_MAX_ANNOTATIONS
};

/// �V�F�[�_�ϐ����Z�}���e�B�N�X�E�A�m�e�[�V�����ɂ���ėv�����Ă�����̂̎��
enum MMEVariableRequest
{
    MME_VARREQ_NONE = 0,

    MME_VARREQ_MATRIX_World,                ///< [ object ] �I�u�W�F�N�g�̃��[���h�ϊ��s��
    MME_VARREQ_MATRIX_CAMERA_WorldView,     ///< [ object ] �I�u�W�F�N�g�̃��[���h�ϊ��s��ƃJ�����̃r���[�ϊ��s��
    MME_VARREQ_MATRIX_CAMERA_WorldViewProj, ///< [ object ] �I�u�W�F�N�g�̃��[���h�ϊ��s��ƃJ�����̃r���[�A�v���W�F�N�V�����ϊ��s��
    MME_VARREQ_MATRIX_LIGHT_WorldView,      ///< [ object ] �I�u�W�F�N�g�̃��[���h�ϊ��s��ƃf�t�H���g���C�g�̃r���[�ϊ��s��
    MME_VARREQ_MATRIX_LIGHT_WorldViewProj,  ///< [ object ] �I�u�W�F�N�g�̃��[���h�ϊ��s��ƃf�t�H���g���C�g�̃r���[�A�v���W�F�N�V�����ϊ��s��

    MME_VARREQ_MATRIX_CAMERA_View,          ///< [ camera scene ] �J�����̃r���[�ϊ��s��
    MME_VARREQ_MATRIX_CAMERA_Proj,          ///< [ camera scene ] �J�����̃v���W�F�N�V�����ϊ��s��
    MME_VARREQ_MATRIX_CAMERA_ViewProj,      ///< [ camera scene ] �J�����̃r���[�~�v���W�F�N�V�����ϊ��s��

    MME_VARREQ_MATRIX_LIGHT_View,           ///< [ scene ] �f�t�H���g���C�g�̃r���[�ϊ��s��
    MME_VARREQ_MATRIX_LIGHT_Proj,           ///< [ scene ] �f�t�H���g���C�g�̃v���W�F�N�V�����ϊ��s��
    MME_VARREQ_MATRIX_LIGHT_ViewProj,       ///< [ scene ] �f�t�H���g���C�g�̃r���[�~�v���W�F�N�V�����ϊ��s��

	// �t�s��
    MME_VARREQ_MATRIX_World_I,
    MME_VARREQ_MATRIX_CAMERA_WorldView_I,
    MME_VARREQ_MATRIX_CAMERA_WorldViewProj_I,
    MME_VARREQ_MATRIX_LIGHT_WorldView_I,
    MME_VARREQ_MATRIX_LIGHT_WorldViewProj_I,
    MME_VARREQ_MATRIX_CAMERA_View_I,
    MME_VARREQ_MATRIX_CAMERA_Proj_I,
    MME_VARREQ_MATRIX_CAMERA_ViewProj_I,
    MME_VARREQ_MATRIX_LIGHT_View_I,
    MME_VARREQ_MATRIX_LIGHT_Proj_I,
    MME_VARREQ_MATRIX_LIGHT_ViewProj_I,

	// �]�u�s��
    MME_VARREQ_MATRIX_World_T,
    MME_VARREQ_MATRIX_CAMERA_WorldView_T,
    MME_VARREQ_MATRIX_CAMERA_WorldViewProj_T,
    MME_VARREQ_MATRIX_LIGHT_WorldView_T,
    MME_VARREQ_MATRIX_LIGHT_WorldViewProj_T,
    MME_VARREQ_MATRIX_CAMERA_View_T,
    MME_VARREQ_MATRIX_CAMERA_Proj_T,
    MME_VARREQ_MATRIX_CAMERA_ViewProj_T,
    MME_VARREQ_MATRIX_LIGHT_View_T,
    MME_VARREQ_MATRIX_LIGHT_Proj_T,
    MME_VARREQ_MATRIX_LIGHT_ViewProj_T,

	// �t�s��~�]�u�s��
    MME_VARREQ_MATRIX_World_IT,
    MME_VARREQ_MATRIX_CAMERA_WorldView_IT,
    MME_VARREQ_MATRIX_CAMERA_WorldViewProj_IT,
    MME_VARREQ_MATRIX_LIGHT_WorldView_IT,
    MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT,
    MME_VARREQ_MATRIX_CAMERA_View_IT,
    MME_VARREQ_MATRIX_CAMERA_Proj_IT,
    MME_VARREQ_MATRIX_CAMERA_ViewProj_IT,
    MME_VARREQ_MATRIX_LIGHT_View_IT,
    MME_VARREQ_MATRIX_LIGHT_Proj_IT,
    MME_VARREQ_MATRIX_LIGHT_ViewProj_IT,


    MME_VARREQ_OBJECT_DIFFUSE,              ///< [ object ] �f�B�t���[�Y�F�i�g�U���j
    MME_VARREQ_OBJECT_AMBIENT,              ///< [ object ] �A���r�G���g�F�i�����j
    MME_VARREQ_OBJECT_EMISSIVE,             ///< [ object ] �G�~�b�V�����F�i���ˌ��j
    MME_VARREQ_OBJECT_SPECULAR,             ///< [ object ] �X�y�L�����F�i���ʌ��j
    MME_VARREQ_OBJECT_SPECULARPOWER,        ///< [ object ] �X�y�L�����̋��x
    MME_VARREQ_OBJECT_TOONCOLOR,            ///< [ object ] �g�D�[���F
    MME_VARREQ_OBJECT_EDGECOLOR,            ///< [ object ] �֊s�F

    MME_VARREQ_LIGHT_DIFFUSE,               ///< [ scene ] �f�t�H���g���C�g�̃f�B�t���[�Y�F�i�g�U���j
    MME_VARREQ_LIGHT_AMBIENT,               ///< [ scene ] �f�t�H���g���C�g�̃A���r�G���g�F�i�����j
    MME_VARREQ_LIGHT_SPECULAR,              ///< [ scene ] �f�t�H���g���C�g�̃X�y�L�����F�i���ʌ��j

    MME_VARREQ_CAMERA_POSITION,             ///< [ camera scene ] �J�����̈ʒu
    MME_VARREQ_CAMERA_DIRECTION,            ///< [ camera scene ] �J�����̌���

    MME_VARREQ_LIGHT_POSITION,              ///< [ scene ] �f�t�H���g���C�g�̈ʒu
    MME_VARREQ_LIGHT_DIRECTION,             ///< [ scene ] �f�t�H���g���C�g�̌���

    MME_VARREQ_OBJECT_MATERIALTEXTURE,      ///< [ object ] �}�e���A���ɐݒ肳��Ă���e�N�X�`��
    MME_VARREQ_OBJECT_MATERIALSPHEREMAP,    ///< [ object ] �}�e���A���ɐݒ肳��Ă���A�X�t�B�A�}�b�v�e�N�X�`��

    MME_VARREQ_VIEWPORTPIXELSIZE,           ///< [ camera scene / offscr scene ] �X�N���[���������̓I�t�X�N���[�������_�����O�^�[�Q�b�g�̃T�C�Y (Scripts��"RenderColorTarget"�R�}���h�ŁA�����_�����O�^�[�Q�b�g���ύX���Ă��A���̒l�͕ω����Ȃ�)

    MME_VARREQ_TIME,                        ///< [ scene ] �o�ߎ��� (�b)
    MME_VARREQ_ELAPSEDTIME,                 ///< [ scene ] �O��̃t���[������̌o�ߎ��� (�b)

    MME_VARREQ_MOUSEPOSITION,               ///< �}�E�X���W (x, y)
    MME_VARREQ_LEFTMOUSEDOWN,               ///< �}�E�X�̍��{�^�����
    MME_VARREQ_MIDDLEMOUSEDOWN,             ///< �}�E�X�̒��{�^�����
    MME_VARREQ_RIGHTMOUSEDOWN,              ///< �}�E�X�̉E�{�^�����


    MME_VARREQ_CONTROLOBJECT_Visible,          ///< �w�肵���I�u�W�F�N�g���\������Ă��邩�ۂ� (bool)
    MME_VARREQ_CONTROLOBJECT_Scale,         ///< �w�肵���I�u�W�F�N�g�̃X�P�[�����O�l (float)
    MME_VARREQ_CONTROLOBJECT_Position,      ///< �w�肵���I�u�W�F�N�g�̍��W�i�I�t�Z�b�g�j(float3 float4)
    MME_VARREQ_CONTROLOBJECT_World,         ///< �w�肵���I�u�W�F�N�g�̃��[���h�ϊ��s�� (float4x4)

    MME_VARREQ_CONTROLOBJECT_BoneOffset,    ///< �w��{�[���̍��W (float3,float4)           [�{�[����]
    MME_VARREQ_CONTROLOBJECT_BoneMatrix,    ///< �w��{�[���̃��[���h�ϊ��s�� (float4x4)    [�{�[����]
    MME_VARREQ_CONTROLOBJECT_MorphBlend,    ///< �w�肵���\��̒l (float)                   [�\�]

    MME_VARREQ_CONTROLOBJECT_X,             ///< �A�N�Z�T���̈ʒuX(�A�N�Z�T���p�l����X)�B�^��float�B
    MME_VARREQ_CONTROLOBJECT_Y,             ///< �A�N�Z�T���̈ʒuY(�A�N�Z�T���p�l����Y)�B�^��float�B
    MME_VARREQ_CONTROLOBJECT_Z,             ///< �A�N�Z�T���̈ʒuZ(�A�N�Z�T���p�l����Z)�B�^��float�B
    MME_VARREQ_CONTROLOBJECT_XYZ,           ///< �A�N�Z�T���̈ʒu(�A�N�Z�T���p�l����X,Y,Z)�B�^��float3�B
    MME_VARREQ_CONTROLOBJECT_Rx,            ///< �A�N�Z�T���̉�]X(�A�N�Z�T���p�l����Rx)�B�^��float�B
    MME_VARREQ_CONTROLOBJECT_Ry,            ///< �A�N�Z�T���̉�]Y(�A�N�Z�T���p�l����Ry)�B�^��float�B
    MME_VARREQ_CONTROLOBJECT_Rz,            ///< �A�N�Z�T���̉�]Z(�A�N�Z�T���p�l����Rz)�B�^��float�B
    MME_VARREQ_CONTROLOBJECT_Rxyz,          ///< �A�N�Z�T���̉�](�A�N�Z�T���p�l����Rx,Ry,Rz)�B�^��float3�B
    MME_VARREQ_CONTROLOBJECT_Si,            ///< �A�N�Z�T���̃T�C�Y(�A�N�Z�T���p�l����Si)�B�^��float�B
    MME_VARREQ_CONTROLOBJECT_Tr,            ///< �A�N�Z�T���̓����x(�A�N�Z�T���p�l����Tr)�B�^��float�B



    
	// ���\�[�X���쐬�������
    MME_VARREQ_TEXTURE,                     ///< MME �̎d�l�ł͂Ȃ����ǁA�e�N�X�`���^���ʗp�ɗp�� (texture, texture2D, texture3D, textureCUBE�̂��������ꂩ)
    MME_VARREQ_RENDERCOLORTARGET,
    MME_VARREQ_RENDERDEPTHSTENCILTARGET,
    MME_VARREQ_ANIMATEDTEXTURE,
    MME_VARREQ_OFFSCREENRENDERTARGET,

    MME_VARREQ_TEXTUREVALUE,

    MME_VARREQ_STANDARDSGLOBAL,


	// �ȉ��͓Ǝ��d�l
	//LN_VARREQ_MATRIX_CAMERA_ORTHOGRAPHIC,
	//LN_VARREQ_MATRIX_LIGHT_ORTHOGRAPHIC,
    LN_VARREQ_MATRIX_PROJECTION2D,

    LN_VARREQ_OPACITY,            ///< �s�����x
    LN_VARREQ_COLOR_SCALE,        ///< ��Z����F
    LN_VARREQ_BLEND_COLOR,        ///< �u�����h����F
    LN_VARREQ_TONE,               ///< �F��
    LN_VARREQ_UVTRANSFORM,        ///< �e�N�X�`�����W�ϊ��s��


	// �����ł���������̉񐔂����炷���߂̍�
    MME_VARREQ_GEOMETRY_MATRIX_BEGIN    = MME_VARREQ_MATRIX_World,
    MME_VARREQ_GEOMETRY_MATRIX_END      = MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT,
	
	MME_VARREQ_CONTROLOBJECT_BEGIN		= MME_VARREQ_CONTROLOBJECT_Visible,
	MME_VARREQ_CONTROLOBJECT_END		= MME_VARREQ_CONTROLOBJECT_Tr,
};

/// �K�v�̂Ȃ��s��v�Z���}�X�N���邽�߂̃t���O
enum MMEWorldMatrixCalcFlags
{
    MME_WMCF_NONE = 0,

	//MME_WMCF_WORLD                      = 0x00000000,
    MME_WMCF_WORLD_VIEW_CAMERA          = 0x00000001,   ///< ���[���h�s�� * �J�����̃r���[�s��
    MME_WMCF_WORLD_VIEW_PROJ_CAMERA     = 0x00000004,   ///< ���[���h�s�� * �J�����̃r���[�s�� * �J�����̃v���W�F�N�V�����s��
    MME_WMCF_WORLD_VIEW_LIGHT           = 0x00000008,   ///< ���[���h�s�� * ���C�g�̃r���[�s��
    MME_WMCF_WORLD_VIEW_PROJ_LIGHT      = 0x00000020,   ///< ���[���h�s�� * ���C�g�̃r���[�s�� * ���C�g�̃v���W�F�N�V�����s��

    MME_WMCF_WORLD_I                    = 0x00000002,   ///< (�t�s��) ���[���h�s��
    MME_WMCF_WORLD_VIEW_CAMERA_I        = 0x00000040,   ///< (�t�s��) ���[���h�s�� * �J�����̃r���[�s��
    MME_WMCF_WORLD_VIEW_PROJ_CAMERA_I   = 0x00000100,   ///< (�t�s��) ���[���h�s�� * �J�����̃r���[�s�� * �J�����̃v���W�F�N�V�����s��
    MME_WMCF_WORLD_VIEW_LIGHT_I         = 0x00000200,   ///< (�t�s��) ���[���h�s�� * ���C�g�̃r���[�s��    
    MME_WMCF_WORLD_VIEW_PROJ_LIGHT_I    = 0x00000800,   ///< (�t�s��) ���[���h�s�� * ���C�g�̃r���[�s�� * ���C�g�̃v���W�F�N�V�����s��

    MME_WMCF_WORLD_T                    = 0x00002000,   ///< (�]�u�s��) ���[���h�s��
    MME_WMCF_WORLD_VIEW_CAMERA_T        = 0x00001000,   ///< (�]�u�s��) ���[���h�s�� * �J�����̃r���[�s��
    MME_WMCF_WORLD_VIEW_PROJ_CAMERA_T   = 0x00004000,   ///< (�]�u�s��) ���[���h�s�� * �J�����̃r���[�s�� * �J�����̃v���W�F�N�V�����s��
    MME_WMCF_WORLD_VIEW_LIGHT_T         = 0x00008000,   ///< (�]�u�s��) ���[���h�s�� * ���C�g�̃r���[�s��
    MME_WMCF_WORLD_VIEW_PROJ_LIGHT_T    = 0x00020000,   ///< (�]�u�s��) ���[���h�s�� * ���C�g�̃r���[�s�� * ���C�g�̃v���W�F�N�V�����s��

    MME_WMCF_WORLD_IT                   = 0x00400000,   ///< (�t�s�� * �]�u�s��) ���[���h�s��
    MME_WMCF_WORLD_VIEW_CAMERA_IT       = 0x00040000,   ///< (�t�s�� * �]�u�s��) ���[���h�s�� * �J�����̃r���[�s��
    MME_WMCF_WORLD_VIEW_PROJ_CAMERA_IT  = 0x00100000,   ///< (�t�s�� * �]�u�s��) ���[���h�s�� * �J�����̃r���[�s�� * �J�����̃v���W�F�N�V�����s��
    MME_WMCF_WORLD_VIEW_LIGHT_IT        = 0x00200000,   ///< (�t�s�� * �]�u�s��) ���[���h�s�� * ���C�g�̃r���[�s��
    MME_WMCF_WORLD_VIEW_PROJ_LIGHT_IT   = 0x00800000,   ///< (�t�s�� * �]�u�s��) ���[���h�s�� * ���C�g�̃r���[�s�� * ���C�g�̃v���W�F�N�V�����s��

	// �����Ŏw�肷��̂̓��[���h�s��ɂ��������̂����B
	// ���C�g��J�����͂��ׂẴV�F�[�_�ŋ��L����̂ŁA���炩���ߑS���v�Z���Ă����B

	// �ȉ��͂悭������K�v�Ȃ���������

	//MME_WMCF_WORLD_PROJ_CAMERA          = 0x00000002,   ///< ���[���h�s�� * �J�����̃v���W�F�N�V�����s��
	//MME_WMCF_WORLD_PROJ_LIGHT           = 0x00000010,   ///< ���[���h�s�� * ���C�g�̃v���W�F�N�V�����s��

	//MME_WMCF_WORLD_PROJ_CAMERA_I        = 0x00000080,   ///< (�t�s��) ���[���h�s�� * �J�����̃v���W�F�N�V�����s��
	//MME_WMCF_WORLD_PROJ_LIGHT_I         = 0x00000400,   ///< (�t�s��) ���[���h�s�� * ���C�g�̃v���W�F�N�V�����s��

	//MME_WMCF_WORLD_PROJ_CAMERA_T        = 0x00002000,   ///< (�]�u�s��) ���[���h�s�� * �J�����̃v���W�F�N�V�����s��
	//MME_WMCF_WORLD_PROJ_LIGHT_T         = 0x00010000,   ///< (�]�u�s��) ���[���h�s�� * ���C�g�̃v���W�F�N�V�����s��

	//MME_WMCF_WORLD_PROJ_CAMERA_IT       = 0x00080000,   ///< (�t�s�� * �]�u�s��) ���[���h�s�� * �J�����̃v���W�F�N�V�����s��
	//MME_WMCF_WORLD_PROJ_LIGHT_IT        = 0x00400000,   ///< (�t�s�� * �]�u�s��) ���[���h�s�� * ���C�g�̃v���W�F�N�V�����s��

};

/// (�Œ�l)
enum MMEScriptOutput
{
    MME_SCROUT_color = 0,          ///< �f�t�H���g�l
};

/// �G�t�F�N�g�t�@�C���̖ړI (����`�悷��G�t�F�N�g��)
enum MMEScriptClass
{
    MME_SCRCLS_object = 0,         ///< �I�u�W�F�N�g��`�悷��B�i�f�t�H���g�j
    MME_SCRCLS_scene,              ///< �X�N���[���o�b�t�@��`�悷��B
    MME_SCRCLS_sceneorobject,      ///< ��L�̗����B
};

/// �G�t�F�N�g�t�@�C���̎��s�^�C�~���O
enum MMEScriptOrder
{
    MME_SCRORDER_standard = 0,      ///< �I�u�W�F�N�g��`�悷��B�i�f�t�H���g�j
    MME_SCRORDER_preprocess,        ///< �I�u�W�F�N�g�̕`�������ɕ`�悷��B�v���G�t�F�N�g�p�B
    MME_SCRORDER_postprocess,       ///< �I�u�W�F�N�g�̕`��̌�ŕ`�悷��B�|�X�g�G�t�F�N�g�p�B
};

/// MMD �̕`��p�X
enum MMDPass
{
    MMD_PASS_object = 0,            ///< �I�u�W�F�N�g�{�́i�Z���t�V���h�EOFF�j
    MMD_PASS_zplot,                 ///< �Z���t�V���h�E�pZ�l�v���b�g
    MMD_PASS_object_ss,             ///< �I�u�W�F�N�g�{�́i�Z���t�V���h�EON�j
    MMD_PASS_shadow,                ///< �e�i�Z���t�V���h�E�ł͂Ȃ��P���ȉe�j�� �g�p���Ȃ�
    MMD_PASS_edge,                  ///< �֊s�iPMD���f���̂݁j���g�p���Ȃ�
};

/// �e�N�j�b�N�̃A�m�e�[�V�����Ŏw�肳��Ă��� bool �l
enum MMETechEnableFlag
{
    MME_TECHSTATE_Disable       = 0x00,

    MME_TECHSTATE_UseTexture    = 0x01,     ///< �e�N�X�`���g�p�̗L��
    MME_TECHSTATE_UseSphereMap  = 0x02,     ///< �X�t�B�A�}�b�v�g�p�̗L��
    MME_TECHSTATE_UseToon       = 0x04,     ///< �g�D�[�������_�����O�g�p�̗L��

    MME_TECHSTATE_OMITTED_Texture    = 0x10, ///< UseTexture �t���O�̗L�� (�ȗ�����Ă���)
    MME_TECHSTATE_OMITTED_SphereMap  = 0x20, ///< UseSphereMap �t���O�̗L�� (�ȗ�����Ă���)
    MME_TECHSTATE_OMITTED_Toon       = 0x40, ///< EnableToon �t���O�̗L�� (�ȗ�����Ă���)

	//MME_TECHSTATE_ALL =                     ///< ���ׂẴt���O�̑g�ݍ��킹
	//    MME_TECHSTATE_UseTexture ||
	//    MME_TECHSTATE_UseSphereMap ||
	//    MME_TECHSTATE_UseToon
};

/// ����R�}���h���e�N�j�b�N�̃X�N���v�g���ɑ��݂��Ă��邩�������t���O
enum MMEScriptCommandExistFlag
{
    MME_SCEXIST_ScriptExternal_Color = 0x01,    ///< ScriptExternal=Color
    MME_SCEXIST_Draw_Geometry        = 0x02,    ///< Draw=Geometry
    MME_SCEXIST_Draw_Buffer          = 0x04,    ///< Draw=Buffer
};

/// �V�F�[�_�v���O�������̕ϐ��ЂƂ���\���\����
struct MMEShaderVariable
{
    Graphics::IShaderVariable*  Variable;           ///< �V�F�[�_�ϐ�
    MMEVariableRequest          Request;            ///< �ϐ����v�����Ă�����̂̎��
	//SceneNode*                  ControlObject;      ///< CONTROLOBJECT �̎Q�ƑΏ� (�V�����I�u�W�F�N�g�� Scene �ɒǉ����ꂽ�Ƃ��ɍĎ擾����)
    LRefTString                 ObjectName;         ///< CONTROLOBJECT �� name �A�m�e�[�V�����̒l
    LRefTString                 ItemName;           ///< CONTROLOBJECT �� item �A�m�e�[�V�����̒l
    MMETextureObject*			TextureObject;		///< OFFSCREENRENDERTARGET ���ō쐬���ꂽ�e�N�X�`�� (�Z�}���e�B�N�X���Ȃ��Ă� ResourceName �A�m�e�[�V����������΍��)
    lnU32                       LightNum;           ///< �K�v�ȃ��C�g��
	bool						LightParamIsArray;	///< ���C�g�v�f���i�[����ϐ����z��̏ꍇ
    bool                        LightParamIsMatrix; ///< ���C�g��񂪍s��̏ꍇ�� true�Bfalse �̏ꍇ�� Vector4 (setMatrix() ���������AsetVector() ���g�����̎w��)
};

typedef std::map< LRefTString, MMESemantics >			MMESemanticsMap;
typedef std::pair< LRefTString, MMESemantics >			MMESemanticsPair;
typedef std::map< LRefTString, MMEAnnotation >			MMEAnnotationMap;
typedef std::pair< LRefTString, MMEAnnotation >			MMEAnnotationPair;
typedef std::map< LRefTString, MMEShaderVariable* >		MMEShaderVariableMap;
typedef std::pair< LRefTString, MMEShaderVariable* >	MMEShaderVariablePair;
typedef std::vector< MMEShaderVariable* >				MMEShaderVariableArray;
typedef std::vector< MMETextureObject* >				MMETextureObjectArray;




//-------------------------------------------------------------------------
//

/// Scene �P�ʂ̃V�F�[�_�ݒ�p�����[�^ (1�t���[����1�x�����ݒ肷��΂����p�����[�^)
struct MMESceneParam
{
    const LMatrix*   Mat_LightView;         ///< �f�t�H���g���C�g�̃r���[�s��
    const LMatrix*   Mat_LightProj;         ///< �f�t�H���g���C�g�̃v���W�F�N�V�����s��
    const LMatrix*   Mat_LightViewProj;     ///< �f�t�H���g���C�g�̃r���[�~�v���W�F�N�V�����s��
	//const LMatrix*   Mat_LightOrtho;        ///< (������)
    LVector4	    LightPosition;			///< ���C�g�̈ʒu
    LVector4	    LightDirection;			///< ���C�g�̌���
    const LVector4*	LightDiffuse;		    ///< ���C�g�̃f�B�t���[�Y�F�i�g�U���j
	const LVector4*	LightAmbient;			///< ���C�g�̃A���r�G���g�F�i�����j
	const LVector4*	LightSpecular;          ///< ���C�g�̃X�y�L�����F�i���ʌ��j

    lnFloat         Time;                   ///< �o�ߎ��� (�b) ���Q�[���J�n���炸���Ɖ��Z�Ƃ����ƃ��o���̂ŉ����Ή����Ă���
    lnFloat         ElapsedTime;            ///< �O��̃t���[������̌o�ߎ��� (�b)
    LVector4        MousePosition;          ///< �}�E�X���W (x, y)
    LVector4        LeftMouseDown;          ///< �}�E�X�̍��{�^�����
    LVector4        MiddleMouseDown;        ///< �}�E�X�̒��{�^�����
    LVector4        RightMouseDown;         ///< �}�E�X�̉E�{�^�����

	// �ȉ��� calcMatrices() �Őݒ肷��
    LMatrix  Mat_LightView_Inverse;
    LMatrix  Mat_LightProj_Inverse;
    LMatrix  Mat_LightViewProj_Inverse;
    LMatrix  Mat_LightView_Transpose;
    LMatrix  Mat_LightProj_Transpose;
    LMatrix  Mat_LightViewProj_Transpose;
    LMatrix  Mat_LightView_InverseTranspose;
    LMatrix  Mat_LightProj_InverseTranspose;
    LMatrix  Mat_LightViewProj_InverseTranspose;

    void calcMatrices()
    {
        Mat_LightView_Inverse = LMatrix::Inverse(*Mat_LightView );
		Mat_LightProj_Inverse = LMatrix::Inverse(*Mat_LightProj);
		Mat_LightViewProj_Inverse = LMatrix::Inverse(*Mat_LightViewProj);
		Mat_LightView_Transpose = LMatrix::Transpose(*Mat_LightView);
		Mat_LightProj_Transpose = LMatrix::Transpose(*Mat_LightProj);
		Mat_LightViewProj_Transpose = LMatrix::Transpose(*Mat_LightViewProj);
        Mat_LightView_InverseTranspose = Mat_LightView_Inverse * Mat_LightView_Transpose;
        Mat_LightProj_InverseTranspose = Mat_LightProj_Inverse * Mat_LightProj_Transpose;
        Mat_LightViewProj_InverseTranspose = Mat_LightViewProj_Inverse * Mat_LightViewProj_Transpose;
    }
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================