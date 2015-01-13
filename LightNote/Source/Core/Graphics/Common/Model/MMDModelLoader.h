//==============================================================================
// MMDModelLoader 
//------------------------------------------------------------------------------
///**
//  @file       MMDModelLoader.h
//  @brief      MMDModelLoader
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../../FileIO/Interface.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{
class ModelCore2;
class ModelFrameCore;

//==============================================================================
// MMDModelLoader
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class MMDModelLoader
{
public:

#pragma pack( push, 1 )

	// �t�@�C���w�b�_
	struct PMD_Header
	{
		char	szMagic[3];		// "Pmd"
		float	fVersion;		// PMD�o�[�W�����ԍ�
		char	szName[20];		// ���f����
		char	szComment[256];	// �R�����g(���쌠�\���Ȃ�)
	};

	// ���_�f�[�^
	struct PMD_Vertex
	{
		LVector3    vec3Pos;	// ���W
		LVector3    vec3Normal;	// �@���x�N�g��
		LVector2    uvTex;		// �e�N�X�`�����W

		unsigned short	unBoneNo[2];	// �{�[���ԍ�
		unsigned char	cbWeight;		// �u�����h�̏d�� (0�`100��)
		unsigned char	cbEdge;			// �G�b�W�t���O
	};

	// �}�e���A���f�[�^
	struct PMD_Material
	{
		LVector4    col4Diffuse;
		float		fShininess;
		LVector3    col3Specular,
					col3Ambient;

		//unsigned short	unknown;
		unsigned char	ToonIndex;
		unsigned char	EdgeFlag;	///< 0:�ʏ�A1:�G�b�W�����@�֊s�����L���̏ꍇ


		unsigned long	ulNumIndices;			// ���̍ގ��ɑΉ����钸�_��
		char			szTextureFileName[20];	// �e�N�X�`���t�@�C����
	};

	// �{�[���f�[�^
	struct PMD_Bone
	{
		char	szName[20];			// �{�[���� (0x00 �I�[�C�]���� 0xFD)
		short	nParentNo;			// �e�{�[���ԍ� (�Ȃ���� -1)
		short	nChildNo;			// �q�{�[���ԍ� (LNote �ł͎g���ĂȂ�)

		unsigned char	cbKind;		// �{�[���̎��
		unsigned short	unIKTarget;	// IK���̃^�[�Q�b�g�{�[��

		LVector3		vec3Position;	// ���f�����_����̈ʒu
		// ����]���͊܂܂Ȃ�
	};
	/*
	kind
		0x00�@�ʏ�{�[��(��]�̂�)
		0x01�@�ʏ�{�[��(�ړ��\)
		0x02�@IK�{�[��(�ړ��\)
		0x03�@�����Ȃ�(�I����)
		0x04�@IK�{�[��(��]�̂�)
		0x05�@��]�Ǐ](��]�̂�)
		0x06�@IK�ڑ���
		0x07�@�����Ȃ�(�I��s��)
		0x08�@�Ђ˂�
		0x09�@��]�^��
	*/
	
	// IK�f�[�^
	struct PMD_IK2
	{
		short			nTargetNo;	// IK�^�[�Q�b�g�{�[���ԍ�
		short			nEffNo;		// IK��[�{�[���ԍ�

		unsigned char	cbNumLink;	// IK���\������{�[���̐�

		unsigned short	unCount;
		float			fFact;
	};

	// �\��f�[�^
	struct PMD_Face2
	{
		char			szName[20];		// �\� (0x00 �I�[�C�]���� 0xFD)

		unsigned long	ulNumVertices;	// �\��_��
		unsigned char	cbType;			// ����
	};

	// ���̃f�[�^
	struct PMD_RigidBody
	{
		char			szName[20];		// ���̖�

		unsigned short	unBoneIndex;	// �֘A�{�[���ԍ�
		unsigned char	cbColGroupIndex;// �Փ˃O���[�v
		unsigned short	unColGroupMask;	// �Փ˃O���[�v�}�X�N

		unsigned char	cbShapeType;	// �`��  0:�� 1:�� 2:�J�v�Z��

		float			fWidth;			// ���a(��)
		float			fHeight;		// ����
		float			fDepth;			// ���s

		LVector3	    vec3Position;	// �ʒu(������W�n�B�{�[������)
		LVector3	    vec3Rotation;	// ��](������W�n�Bradian)

		float			fMass;			// ����
		float			fLinearDamping;	// �ړ���
		float			fAngularDamping;// ��]��
		float			fRestitution;	// ������
		float			fFriction;		// ���C��

		unsigned char	cbRigidBodyType;// �^�C�v 0:Bone�Ǐ] 1:�������Z 2:�������Z(Bone�ʒu����)
	};

#pragma pack( pop )

public:

	/// �ǂݍ���
	ModelCore2* load(ModelManager* manager, FileIO::Stream* stream, const FileIO::PathNameW& baseDir, bool isDynamic);

private:
	// [�폜�\��]
	void trySetIKLimitter( const char* sjisName, ModelFrameCore* frame );

	
	void trySetIKLimitter( const char* sjisName, ModelIKLink* ikLink );

	//ModelFrameCore* createModelFrameCore( const PMD_Bone& pmdBone );

private:

	//std::vector<ModelFrameCore*>	mModelFrameCoreArray;
	int		mCenterBoneIndex;

	int		mLeftKneeBoneIndex;
	int		mRightKneeBoneIndex;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote
