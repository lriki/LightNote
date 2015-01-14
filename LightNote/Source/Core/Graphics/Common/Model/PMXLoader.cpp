//==============================================================================
// PMXLoader 
//==============================================================================

#include "stdafx.h"
#include "../../../Base/Unicode.h"
#include "Core/ModelCore.h"
#include "Core/ModelFrameCore.h"
#include "Core/ModelRigidBodyCore.h"
#include "Core/ModelConstraintCore.h"
#include "Core/ModelIKCore.h"
#include "ModelManager.h"
#include "PMXLoader.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// PMXLoader
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	PMXLoader::~PMXLoader()
	{
		ln_foreach( Texture* tex, mTextureTable )
		{
			LN_SAFE_RELEASE( tex );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ModelCore2* PMXLoader::load(ModelManager* manager, FileIO::Stream* stream, const FileIO::PathNameW& baseDir, bool isDynamic)
	{
		mManager = manager;
		mGraphicsDevice = manager->getGraphicsDevice();
		mIsDynamic = isDynamic;
		mFlags = 0;

		FileIO::BinaryReader reader( stream );
		LRefPtr<ModelCore2> modelCore( LN_NEW ModelCore2( manager, ModelFormat_PMX ) );
		mModelCore = modelCore;
	
		//-----------------------------------------------------
	    // �w�b�_
		reader.read( &mPMXHeader, sizeof(PMX_Header) );

		if ( mPMXHeader.Magic[0] != 'P' ||
			 mPMXHeader.Magic[1] != 'M' ||
			 mPMXHeader.Magic[2] != 'X' ||
			 mPMXHeader.Magic[3] != ' ')
		{
			return NULL;
		}

		if (mPMXHeader.Version < 2.0f) return NULL;
#if 0
		_p( pmxHeader.Version );
		printf( "DataSize              : %d\n", pmxHeader.DataSize );
		printf( "�G���R�[�h����        : %d\n", pmxHeader.Data[0]);
		printf( "�ǉ�UV��              : %d\n", pmxHeader.Data[1]);
		printf( "���_Index�T�C�Y       : %d\n", pmxHeader.Data[2]);
		printf( "�e�N�X�`��Index�T�C�Y : %d\n", pmxHeader.Data[3]);
		printf( "�ގ�Index�T�C�Y       : %d\n", pmxHeader.Data[4]);
		printf( "�{�[��Index�T�C�Y     : %d\n", pmxHeader.Data[5]);
		printf( "���[�tIndex�T�C�Y     : %d\n", pmxHeader.Data[6]);
		printf( "����Index�T�C�Y       : %d\n", pmxHeader.Data[7]);
#endif
		
	    // ���f�����
		loadModelInfo( &reader );

	    // ���_
		loadVertices( &reader );

		// ���_�C���f�b�N�X
		loadIndices( &reader );

		// �e�N�X�`���e�[�u��
		loadTextureTable( &reader, baseDir );

		// �}�e���A��
		loadMaterials( &reader );

		// �{�[��
		loadBones( &reader );

		// ���[�t
		loadMorphs( &reader );

		// �\���g
		loadDisplayFrame( &reader );

		// ����
		loadRigidBodys( &reader );

		// �W���C���g
		loadJoints( &reader );

		modelCore.addRef();
		return modelCore;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void PMXLoader::loadModelInfo( FileIO::BinaryReader* reader )
	{
		//-----------------------------------------------------
	    // ���f�����

		// ���f����
		lnS32 byteSize = reader->readInt32();
		mModelCore->Name = readString( reader, byteSize );

		// ���f�����p
		byteSize = reader->readInt32();
		reader->seek( byteSize );

		// �R�����g
		byteSize = reader->readInt32();
		reader->seek( byteSize );

		// �R�����g�p
		byteSize = reader->readInt32();
		reader->seek( byteSize );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void PMXLoader::loadVertices( FileIO::BinaryReader* reader )
	{
		//-----------------------------------------------------
	    // ���_

		// ���_��
		int vertexCount = reader->readInt32();

		// ���_�o�b�t�@�쐬
		mModelCore->VertexBuffer = mGraphicsDevice->createVertexBuffer( 
			PMX_Vertex::elements(), vertexCount, NULL, true );

	    // �f�[�^�𗬂�����
		struct BaseVertex
		{
			LVector3	Position;
			LVector3	Normal;
			LVector2	TexUV;
		} baseVertex;
        PMX_Vertex* vertices = (PMX_Vertex*)mModelCore->VertexBuffer->lock();
        PMX_Vertex* v;
        for ( lnU32 i = 0 ; i < vertexCount; ++i )
	    {
			v = &vertices[i];

			// ���_�A�@���A�e�N�X�`��UV
			reader->read( &baseVertex, sizeof(BaseVertex) );
			v->Position = baseVertex.Position;
			v->Normal = baseVertex.Normal;
			v->TexUV = baseVertex.TexUV;

			// �ǉ�UV
			for ( int j = 0; j < getAdditionalUVCount(); i++)
			{
				v->AdditionalUV[i].Set(
					reader->readFloat(),
					reader->readFloat(),
					reader->readFloat(),
					reader->readFloat() );
			}

			// �u�����h�E�F�C�g
			int defType = reader->readInt8();
			switch ( defType )
			{
				case 0:	// BDEF1
					v->BlendIndices[0] = reader->readInt( getBoneIndexSize() );
					v->BlendIndices[1] = 0.0f;
					v->BlendIndices[2] = 0.0f;
					v->BlendIndices[3] = 0.0f;
					v->BlendWeights[0] = 1.0f;
					v->BlendWeights[1] = 0.0f;
					v->BlendWeights[2] = 0.0f;
					v->BlendWeights[3] = 0.0f;
					break;
				case 1:	// BDEF2
					v->BlendIndices[0] = reader->readInt( getBoneIndexSize() );
					v->BlendIndices[1] = reader->readInt( getBoneIndexSize() );
					v->BlendIndices[2] = 0.0f;
					v->BlendIndices[3] = 0.0f;
					v->BlendWeights[0] = reader->readFloat();
					v->BlendWeights[1] = 1.0f - v->BlendWeights[0];
					v->BlendWeights[2] = 0.0f;
					v->BlendWeights[3] = 0.0f;
					break;
				case 2:	// BDEF4
					v->BlendIndices[0] = reader->readInt( getBoneIndexSize() );
					v->BlendIndices[1] = reader->readInt( getBoneIndexSize() );
					v->BlendIndices[2] = reader->readInt( getBoneIndexSize() );
					v->BlendIndices[3] = reader->readInt( getBoneIndexSize() );
					v->BlendWeights[0] = reader->readFloat();
					v->BlendWeights[1] = reader->readFloat();
					v->BlendWeights[2] = reader->readFloat();
					v->BlendWeights[3] = reader->readFloat();
					break;
				case 3:	// SDEF
					v->BlendIndices[0] = reader->readInt( getBoneIndexSize() );
					v->BlendIndices[1] = reader->readInt( getBoneIndexSize() );
					v->BlendIndices[2] = 0.0f;
					v->BlendIndices[3] = 0.0f;
					v->BlendWeights[0] = reader->readFloat();
					v->BlendWeights[1] = 1.0f - v->BlendWeights[0];
					v->BlendWeights[2] = 0.0f;
					v->BlendWeights[3] = 0.0f;
					reader->read( &v->SdefC, sizeof(lnFloat) * 3 );
					reader->read( &v->SdefR0, sizeof(lnFloat) * 3 );
					reader->read( &v->SdefR1, sizeof(lnFloat) * 3 );	// TODO:���C���l��v�v�Z
					break;
			}

			// �G�b�W�{��
			v->EdgeWeight = reader->readFloat();
	    }

        mModelCore->VertexBuffer->unlock();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void PMXLoader::loadIndices( FileIO::BinaryReader* reader )
	{
		//-----------------------------------------------------
	    // ���_�C���f�b�N�X

		// �C���f�b�N�X��
		int indexCount = reader->readInt32();

		// �C���f�b�N�X�o�b�t�@�쐬
		mModelCore->IndexBuffer = mGraphicsDevice->createIndexBuffer( 
			indexCount, NULL, true, (getVertexIndexSize() <= 2) );

		// 1 �o�C�g�C���f�b�N�X
		if ( getVertexIndexSize() == 1 )
		{
			//TODO:���Ή�
			LN_ERR2_ASSERT( 0 );
		}
		// 2 or 4 �o�C�g�C���f�b�N�X
		else
		{
			// ���̂܂܃R�s�[
			lnByte* indices = (lnByte*)mModelCore->IndexBuffer->lock();
			reader->read( indices, getVertexIndexSize() * indexCount );
			mModelCore->IndexBuffer->unlock();
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void PMXLoader::loadTextureTable(FileIO::BinaryReader* reader, const FileIO::PathNameW& baseDir)
	{
		//-----------------------------------------------------
	    // �e�N�X�`��

		// �e�N�X�`����
		int textureCount = reader->readInt32();

		// �e�N�X�`���e�[�u��
		mTextureTable.reserve( textureCount );

		for (int i = 0 ; i < textureCount; ++i )
		{
			// �e�N�X�`����
			lnS32 byteSize = reader->readInt32();
			lnRefStringW name = readString( reader, byteSize );

			// �쐬
			FileIO::PathNameW filePath(baseDir, name.c_str());
			mTextureTable.push_back(mManager->createTexture(filePath, mFlags));
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void PMXLoader::loadMaterials( FileIO::BinaryReader* reader )
	{
		//-----------------------------------------------------
	    // �}�e���A���E�����e�[�u��

		// �}�e���A����
		int materialCount = reader->readInt32();

		// �������m��
		mModelCore->Material.Materials.resize( materialCount );
		mModelCore->Material.Attributes.resize( materialCount );

		int indexAttrOffset = 0;
		for ( int i = 0 ; i < materialCount; ++i )
		{
			LMaterial* m = &mModelCore->Material.Materials[i];

			// �ގ���
			lnS32 byteSize = reader->readInt32();
			reader->seek( byteSize );

			// �ގ��p��
			byteSize = reader->readInt32();
			reader->seek( byteSize );

			// Diffuse
			LVector4 color;
			reader->read( &color, sizeof(lnFloat) * 4 );
			m->Diffuse.set( color.X, color.Y, color.Z, color.W );

			// Specular
			reader->read( &color, sizeof(lnFloat) * 3 );
			m->Specular.R = color.X;
            m->Specular.G = color.Y;
            m->Specular.B = color.Z;
            m->Specular.A = 1.0f;

			// Specular�W��
			m->Power = reader->readFloat();

			// Ambient
			reader->read( &color, sizeof(lnFloat) * 3 );
			m->Ambient.R = color.X;
            m->Ambient.G = color.Y;
            m->Ambient.B = color.Z;
            m->Ambient.A = 1.0f;

			// �`��t���O
			lnU8 bitFlag = reader->readInt8();
			m->DrawingFlags = bitFlag;

			// �G�b�W�F
			reader->read( &m->EdgeColor, sizeof(lnFloat) * 4 );

			// �G�b�W�T�C�Y
			m->EdgeSize = reader->readFloat();

			// �e�N�X�`��
			int normalTexture = reader->readInt( getTextureIndexSize() );
			if ( normalTexture >= 0 )
				m->setTexture( mTextureTable[normalTexture] );

			// �X�t�B�A�e�N�X�`��
			int sphereTexture = reader->readInt( getTextureIndexSize() );
			m->SphereMode = (enum Material::SphereMode)reader->readInt8();
			
			// �g�D�[���e�N�X�`��
			int shareToon = reader->readInt8();
			if ( shareToon == 0 )
			{
				int index = reader->readInt( getTextureIndexSize() );
				if ( index >= 0 )
					m->setToonTexture( mTextureTable[index] );
			}
			// ���LToon�e�N�X�`�� (toon01.bmp�`toon10.bmp)
			else
			{
				int index = reader->readInt8();
				m->setToonTexture( mManager->getMMDDefaultToonTexture( index ) );
			}

			// ���R��
			byteSize = reader->readInt32();
			reader->seek( byteSize );

			// �}�e���A���ɑΉ����钸�_��
			int vc = reader->readInt32();

			// �����e�[�u���𖄂߂�
			LNMeshAttribute* attr = &mModelCore->Material.Attributes[i];
            attr->MaterialIndex = i;
            attr->StartIndex = indexAttrOffset;
			attr->PrimitiveNum = vc / 3;
			indexAttrOffset += vc;
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void PMXLoader::loadBones( FileIO::BinaryReader* reader )
	{
		// �{�[����
		int boneCount = reader->readInt32();

		// �e�{�[�����C���f�b�N�X����E�����߁A�܂��͂��ׂăC���X�^���X��
		mModelCore->FrameArray.resize( boneCount );
		for ( lnU16 i = 0 ; i < boneCount; ++i ) {
			mModelCore->FrameArray[i] = LN_NEW ModelFrameCore();
		}

		// �f�[�^�ǂݍ���
		for ( lnU16 i = 0 ; i < boneCount; ++i )
	    {
			ModelFrameCore* frameCore = mModelCore->FrameArray[i];
			frameCore->OwnerModelCore = mModelCore;
			frameCore->FrameIndex = i;

			// �{�[����
			frameCore->Name = readTextBuf( reader );

			// �{�[���p��
			/*�g��Ȃ�*/ readTextBuf( reader );

			// �����ʒu
			reader->read( &frameCore->OrgPosition, sizeof(lnFloat) * 3 );

			// �e�{�[���̃{�[��Index
			frameCore->ParentFrameIndex = reader->readInt( getBoneIndexSize() );

			// �ό`�K�w
			frameCore->TransformLevel = reader->readInt32();

			// �{�[���t���O
			lnU32 flag = reader->readUInt16();
			frameCore->BoneConnect			= (flag & 0x0001) != 0 ? BoneConnectType_Bone : BoneConnectType_PositionOffset;
			frameCore->CanRotate			= (flag & 0x0002) != 0;
			frameCore->CanMove				= (flag & 0x0004) != 0;
			frameCore->IsVisible			= (flag & 0x0008) != 0;
			frameCore->CanOperate			= (flag & 0x0010) != 0;

			frameCore->IsIK					= (flag & 0x0020) != 0;
			frameCore->LocalProvide			= (flag & 0x0080) != 0 ? LocalProvideType_ParentLocalTransformValue : LocalProvideType_UserTransformValue;

			frameCore->IsRotateProvided		= (flag & 0x0100) != 0;
			frameCore->IsMoveProvided		= (flag & 0x0200) != 0;

			frameCore->IsFixAxis			= (flag & 0x0400) != 0;
			frameCore->IsLocalAxis			= (flag & 0x0800) != 0;

			frameCore->TransformAfterPhysics= (flag & 0x1000) != 0;
			frameCore->ParentTransform		= (flag & 0x2000) != 0;

			if ( frameCore->BoneConnect == BoneConnectType_PositionOffset )
            {
				// ���W�I�t�Z�b�g, �{�[���ʒu����̑��Ε�
				reader->read( &frameCore->PositionOffset, sizeof(lnFloat) * 3 );
            }
            else
            {
				// �ڑ���{�[���̃{�[��Index
                frameCore->ConnectedBoneIndex = reader->readInt( getBoneIndexSize() );
            }
			
			// ��]�t�^:1 �܂��� �ړ��t�^:1 �̏ꍇ
			if ( frameCore->IsRotateProvided || frameCore->IsMoveProvided )
            {
				// �t�^�e�{�[���̃{�[��Index
                frameCore->ProvidedParentBoneIndex = reader->readInt( getBoneIndexSize() );
				frameCore->ProvidedRatio = reader->readFloat();
            }

			// ���Œ�:1 �̏ꍇ
            if ( frameCore->IsFixAxis ) {
				reader->read( &frameCore->AxisDirectionVector, sizeof(lnFloat) * 3 );
			}

			//  ���[�J����:1 �̏ꍇ
            if ( frameCore->IsLocalAxis )
            {
				reader->read( &frameCore->DimentionXDirectionVector, sizeof(lnFloat) * 3 );
				reader->read( &frameCore->DimentionZDirectionVector, sizeof(lnFloat) * 3 );
            }

			// �O���e�ό`:1 �̏ꍇ
            if ( frameCore->ParentTransform ) {
				frameCore->KeyValue = reader->readInt32();
			}

			// IK:1 �̏ꍇ
            if ( frameCore->IsIK )
            {
#if 1	// �폜�\��
	//            frameCore->IKTargetBoneIndex = reader->readInt( getBoneIndexSize() );
	//            frameCore->IKLoopCount = reader->readInt32();
				//frameCore->IKRotateLimit = reader->readFloat();

				// �V
				ModelIKCore2* ik = LN_NEW ModelIKCore2();
				mModelCore->IKs.push_back( ik );
				ik->TargetBoneIndex		= i;							// ���ݏ������̃t���[���ԍ�
				ik->EffectorBoneIndex	= reader->readInt( getBoneIndexSize() );
				ik->LoopCount			= reader->readInt32();
				ik->IKRotateLimit		= reader->readFloat();

                int ikLinkCount = reader->readInt32();
                for ( int i = 0; i < ikLinkCount; i++ )
                {
					ModelIKLink ikLink;
					ikLink.LinkBoneIndex = reader->readInt( getBoneIndexSize() );
					ikLink.IsRotateLimit = (reader->readInt8() != 0);
					if ( ikLink.IsRotateLimit )
					{
						reader->read( &ikLink.MinLimit, sizeof(lnFloat) * 3 );
						reader->read( &ikLink.MaxLimit, sizeof(lnFloat) * 3 );
					}
					//frameCore->IKLinks.push_back( ikLink );

					// �V
					ik->IKLinks.push_back( ikLink );
                }
#endif
            }
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void PMXLoader::loadMorphs( FileIO::BinaryReader* reader )
	{
		// ���[�t��
		int boneCount = reader->readInt32();
		mModelCore->ModelMorphCores.resize( boneCount );

		// �f�[�^�ǂݍ���
		for ( lnU16 i = 0 ; i < boneCount; ++i )
	    {
			ModelMorphCore* morphCore = LN_NEW ModelMorphCore();
			mModelCore->ModelMorphCores[i] = morphCore;

			// ���[�t��
			morphCore->Name = readTextBuf( reader );

			// ���[�t�p��
			/*�g��Ȃ�*/ readTextBuf( reader );

			// ����p�l��
			morphCore->OperationPanel = reader->readInt8();

			// ���[�t���
			int morphType = reader->readInt8();

			// ���[�t�I�t�Z�b�g
			morphCore->MorphOffsets.resize( reader->readInt32() );
			for ( int i = 0; i < morphCore->MorphOffsets.size(); i++ )
			{
				ModelMorphCore::MorphOffset* mo = &morphCore->MorphOffsets[i];
				switch ( morphType )
				{
					case 0:		// �O���[�v���[�t
						morphCore->MorphType = ModelMorphType_Group;
						mo->GroupMorphOffset.MorphIndex = reader->readInt( getMorphIndexSize() );
						mo->GroupMorphOffset.MorphRatio = reader->readFloat();
                        break;
                    case 1:		// ���_���[�t
						// VertexIndex �̓��f���{�̂̒��_�C���f�b�N�X
						// PositionOffset �͌��̈ʒu����̑��Έʒu
						morphCore->MorphType = ModelMorphType_Vertex;
						mo->VertexMorphOffset.VertexIndex = reader->readInt( getVertexIndexSize() );
						reader->read( &mo->VertexMorphOffset.PositionOffset, sizeof(float) * 3 );
                        break;
                    case 2:		// �{�[�����[�t
						morphCore->MorphType = ModelMorphType_Bone;
						mo->BoneMorphOffset.BoneIndex = reader->readInt( getVertexIndexSize() );
						reader->read( &mo->BoneMorphOffset.Moving, sizeof(float) * 3 );
						reader->read( &mo->BoneMorphOffset.Rotating, sizeof(float) * 4 );
                        break;
                    case 3:		// UV���[�t
						morphCore->MorphType = ModelMorphType_UV;
						mo->UVMorphOffset.VertexIndex = reader->readInt( getVertexIndexSize() );
						reader->read( &mo->UVMorphOffset.UVOffset, sizeof(float) * 4 );
                        break;
                    case 4:		// �ǉ�UV���[�t1
						morphCore->MorphType = ModelMorphType_AdditionalUV1;
						mo->UVMorphOffset.VertexIndex = reader->readInt( getVertexIndexSize() );
						reader->read( &mo->UVMorphOffset.UVOffset, sizeof(float) * 4 );
                        break;
                    case 5:		// �ǉ�UV���[�t2
						morphCore->MorphType = ModelMorphType_AdditionalUV2;
						mo->UVMorphOffset.VertexIndex = reader->readInt( getVertexIndexSize() );
						reader->read( &mo->UVMorphOffset.UVOffset, sizeof(float) * 4 );
                        break;
                    case 6:		// �ǉ�UV���[�t3
						morphCore->MorphType = ModelMorphType_AdditionalUV3;
						mo->UVMorphOffset.VertexIndex = reader->readInt( getVertexIndexSize() );
						reader->read( &mo->UVMorphOffset.UVOffset, sizeof(float) * 4 );
                        break;
                    case 7:		// �ǉ�UV���[�t4
						morphCore->MorphType = ModelMorphType_AdditionalUV4;
						mo->UVMorphOffset.VertexIndex = reader->readInt( getVertexIndexSize() );
						reader->read( &mo->UVMorphOffset.UVOffset, sizeof(float) * 4 );
                        break;
                    case 8:		// �ގ����[�t
						morphCore->MorphType = ModelMorphType_Matrial;
						mo->MaterialMorphOffset.MaterialIndex = reader->readInt( getMaterialIndexSize() );
						mo->MaterialMorphOffset.OffsetCalcType = reader->readUInt8();
						reader->read( &mo->MaterialMorphOffset.Diffuse, sizeof(float) * 4 );
						reader->read( &mo->MaterialMorphOffset.Specular, sizeof(float) * 3 );
						reader->read( &mo->MaterialMorphOffset.SpecularCoe, sizeof(float) * 1 );
						reader->read( &mo->MaterialMorphOffset.Ambient, sizeof(float) * 3 );
						reader->read( &mo->MaterialMorphOffset.EdgeColor, sizeof(float) * 4 );
						reader->read( &mo->MaterialMorphOffset.EdgeSize, sizeof(float) * 1 );
						reader->read( &mo->MaterialMorphOffset.TextureCoe, sizeof(float) * 4 );
						reader->read( &mo->MaterialMorphOffset.SphereTextureCoe, sizeof(float) * 4 );
						reader->read( &mo->MaterialMorphOffset.ToonTextureCoe, sizeof(float) * 4 );
                        break;
                    case 9:		// Flip���[�t
						morphCore->MorphType = ModelMorphType_Flip;
						mo->FlipMorphOffset.MorphIndex = reader->readInt( getMorphIndexSize() );
						mo->FlipMorphOffset.MorphValue = reader->readFloat();
                        break;
                    case 10:	// Impulse���[�t
						morphCore->MorphType = ModelMorphType_Impulse;
						mo->ImpulseMorphOffset.RigidIndex = reader->readInt( getMorphIndexSize() );
						mo->ImpulseMorphOffset.LocalFlag = reader->readUInt8();
						reader->read( &mo->ImpulseMorphOffset.Moving, sizeof(float) * 3 );
						reader->read( &mo->ImpulseMorphOffset.Rotating, sizeof(float) * 3 );
                        break;
				}
			}
		}
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void PMXLoader::loadDisplayFrame( FileIO::BinaryReader* reader )
	{
		// �\���g�͂��ׂēǂݔ�΂�
		int displayFrameCount = reader->readInt32();
		for ( int i = 0; i < displayFrameCount; i++ )
		{
			// �g��
			lnS32 byteSize = reader->readInt32();
			reader->seek( byteSize );

			// �g���p
			byteSize = reader->readInt32();
			reader->seek( byteSize );

			// ����g�t���O
			reader->seek( sizeof(lnU8) );

			// �g���v�f��
			int frameElementCount = reader->readInt32();
			for ( int j = 0; j < frameElementCount; j++ )
			{
				// �v�f�Ώ�
				lnU8 type = reader->readUInt8();
				switch( type )
				{
				case 0:	// �{�[�����^�[�Q�b�g�̏ꍇ
					reader->readInt( getBoneIndexSize() );
					break;
				case 1:	// ���[�t���^�[�Q�b�g�̏ꍇ
					reader->readInt( getMorphIndexSize() );
					break ;
				}
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void PMXLoader::loadRigidBodys( FileIO::BinaryReader* reader )
	{
		// ���̐�
		int bodyCount = reader->readInt32();
		mModelCore->RigidBodys.resize( bodyCount );

		// �f�[�^�ǂݍ���
		for ( lnU16 i = 0 ; i < bodyCount; ++i )
	    {
			ModelRigidBodyCore* bodyCore = LN_NEW ModelRigidBodyCore();
			mModelCore->RigidBodys[i] = bodyCore;

			// ���̖�
			bodyCore->Name = readTextBuf( reader );

			// ���̉p��
			/*�g��Ȃ�*/ readTextBuf( reader );

			// �֘A�{�[��Index - �֘A�Ȃ��̏ꍇ��-1
			bodyCore->RelatedFrameIndex = reader->readInt( getBoneIndexSize() );

			// �O���[�v
			bodyCore->Group = (1 << reader->readUInt8());

			// ��Փ˃O���[�v�t���O
			bodyCore->GroupMask = reader->readUInt16();

			// �`�� - 0:�� 1:�� 2:�J�v�Z��
			lnU8 type = reader->readUInt8();

			// �T�C�Y(x,y,z)
			LVector3 size;
			reader->read( &size, sizeof(float) * 3 );

			switch ( type )
			{
				case 0:
					bodyCore->ColShapeData.Type = CollisionShapeType_Sphere;
					bodyCore->ColShapeData.Sphere.Radius = size.X;
					break;
				case 1:
					bodyCore->ColShapeData.Type = CollisionShapeType_Box;
					bodyCore->ColShapeData.Box.Width = size.X;
					bodyCore->ColShapeData.Box.Height = size.Y;
					bodyCore->ColShapeData.Box.Depth = size.Z;
					break;
				case 2:
					bodyCore->ColShapeData.Type = CollisionShapeType_Capsule;
					bodyCore->ColShapeData.Capsule.Radius = size.X;
					bodyCore->ColShapeData.Capsule.Height = size.Y;
					break;
			}
			
			// �ʒu(x,y,z)
			LVector3 Position;
			reader->read( &Position, sizeof(float) * 3 );

			// ��](x,y,z) -> ���W�A���p
			LVector3 Rotation;
			reader->read( &Rotation, sizeof(float) * 3 );
			if (LMath::IsNaN(Rotation.X))
			{
				Rotation.X = 0;
			}
			if (LMath::IsNaN(Rotation.Y))
			{
				Rotation.Y = 0;
			}
			if (LMath::IsNaN(Rotation.Z))
			{
				Rotation.Z = 0;
			}

			// �I�t�Z�b�g�s��
			LMatrix bias;
			bias.RotationZ( Rotation.Z );
			bias.RotationX( Rotation.X );
			bias.RotationY( Rotation.Y );
			bias.Translation( Position - mModelCore->FrameArray[bodyCore->RelatedFrameIndex]->OrgPosition );
			bodyCore->BoneOffset = bias;
			bodyCore->InvBoneOffset = LMatrix::Inverse(bodyCore->BoneOffset);

			// ���̊�{���
			bodyCore->Mass				= reader->readFloat();
			bodyCore->LinearDamping		= reader->readFloat();
			bodyCore->AngularDamping	= reader->readFloat();
			bodyCore->Restitution		= reader->readFloat();
			bodyCore->Friction			= reader->readFloat();

			// ���̂̕������Z - 0:�{�[���Ǐ](static) 1:�������Z(dynamic) 2:�������Z + Bone�ʒu���킹
			lnU8 physicsType = reader->readUInt8();
			switch ( physicsType )
			{
				case 0:
					bodyCore->RigidBodyType = RigidBodyType_ControlledByBone;
					break;
				case 1:
					bodyCore->RigidBodyType = RigidBodyType_Physics;
					break;
				case 2:
					bodyCore->RigidBodyType = RigidBodyType_PhysicsAlignment;
					break;
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void PMXLoader::loadJoints( FileIO::BinaryReader* reader )
	{
		// �W���C���g��
		int jointCount = reader->readInt32();
		mModelCore->Constraints.resize( jointCount );

		// �f�[�^�ǂݍ���
		for ( lnU16 i = 0 ; i < jointCount; ++i )
	    {
			ModelConstraintCore* jointCore = LN_NEW ModelConstraintCore();
			mModelCore->Constraints[i] = jointCore;

			// Joint��
			jointCore->Name = readTextBuf( reader );

			// Joint���p
			/*�g��Ȃ�*/ readTextBuf( reader );

			// Joint��� - 0:�X�v�����O6DOF   | PMX2.0�ł� 0 �̂�(�g���p)
			int type = reader->readUInt8();
			LN_THROW_InvalidFormat(type == 0);

			// ��� PMD �Ɠ���
			jointCore->RigidBodyAIndex = reader->readInt( getRigidBodyIndexSize() );
			jointCore->RigidBodyBIndex = reader->readInt( getRigidBodyIndexSize() );
			reader->read( &jointCore->Position, sizeof(LVector3) );
			reader->read( &jointCore->Rotation, sizeof(LVector3) );
			reader->read( &jointCore->PositionLimitLower, sizeof(LVector3) );
			reader->read( &jointCore->PositionLimitUpper, sizeof(LVector3) );
			reader->read( &jointCore->RotationLimitLower, sizeof(LVector3) );
			reader->read( &jointCore->RotationLimitUpper, sizeof(LVector3) );
			reader->read( &jointCore->SpringPositionStiffness, sizeof(LVector3) );
			reader->read( &jointCore->SpringRotationStiffness, sizeof(LVector3) );
			
			jointCore->SpringRotationStiffness.X = LMath::ToRadians(jointCore->SpringRotationStiffness.X);
			jointCore->SpringRotationStiffness.Y = LMath::ToRadians(jointCore->SpringRotationStiffness.Y);
			jointCore->SpringRotationStiffness.Z = LMath::ToRadians(jointCore->SpringRotationStiffness.Z);
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	lnRefStringW PMXLoader::readTextBuf( FileIO::BinaryReader* reader )
	{
		lnS32 byteSize = reader->readInt32();
		return readString( reader, byteSize );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	lnRefStringW PMXLoader::readString( FileIO::BinaryReader* reader, int byteSize )
	{
		reader->read( mTempBuffer.request( byteSize ), byteSize );

		Base::UnicodeString str;
		if ( getEncode() == PMX_Encode_UTF16 )
			str.setDecodeBytes( mTempBuffer.getBuffer(), byteSize, Base::UnicodeString::getUTF16Codec() );
		else
			str.setDecodeBytes( mTempBuffer.getBuffer(), byteSize, Base::UnicodeString::getUTF8Codec() );

		return lnRefStringW( str.getString() );
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote
