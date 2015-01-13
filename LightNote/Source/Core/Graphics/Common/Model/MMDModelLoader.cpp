//==============================================================================
// MMDModelLoader 
//==============================================================================

#pragma once

#include "stdafx.h"
#include "../../../FileIO/BinaryReader.h"
#include "ModelManager.h"
#include "Core/ModelCore.h"
#include "Core/ModelFrameCore.h"
#include "Core/ModelIKLimitter.h"
#include "Core/ModelIKCore.h"
#include "Core/ModelFaceCore.h"
#include "Core/ModelRigidBodyCore.h"
#include "Core/ModelConstraintCore.h"
#include "MMDModelLoader.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// MMDModelLoader
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ModelCore2* MMDModelLoader::load(ModelManager* manager, FileIO::Stream* stream, const FileIO::PathNameW& baseDir, bool isDynamic)
	{
		Graphics::IGraphicsDevice* device = manager->getGraphicsDevice();
		FileIO::BinaryReader reader( stream );

		mCenterBoneIndex = -1;
		mLeftKneeBoneIndex = -1;
		mRightKneeBoneIndex = -1;

		LRefPtr<ModelCore2> modelCore( LN_NEW ModelCore2( manager, ModelFormat_PMD ) );

		//-----------------------------------------------------
	    // �w�b�_�̃`�F�b�N
		PMD_Header pmdHeader;
		reader.read( &pmdHeader, sizeof(PMD_Header) );

	    if( pmdHeader.szMagic[0] != 'P' || pmdHeader.szMagic[1] != 'm' || pmdHeader.szMagic[2] != 'd' || pmdHeader.fVersion != 1.0f ) {
			return NULL;
		}

        // ���f����
        char modelName[21];
	    strncpy( modelName, pmdHeader.szName, 20 );
	    modelName[20] = '\0';
		modelCore->Name = modelName;

		//-----------------------------------------------------
	    // ���_
		lnU32 vertexCount = reader.readUInt32();

		// ���_�o�b�t�@�쐬
        modelCore->VertexBuffer = device->createVertexBuffer(
			Vertex::PMDVertex::elements(), vertexCount, NULL, true );

		// �f�[�^�𗬂�����
        Vertex::PMDVertex* vertices = (Vertex::PMDVertex*)modelCore->VertexBuffer->lock();
        for ( lnU32 i = 0 ; i < vertexCount; ++i )
	    {
			PMD_Vertex pmdVertex;
			reader.read( &pmdVertex, sizeof(PMD_Vertex) );

            Vertex::PMDVertex* v = &vertices[ i ];
            v->Position = pmdVertex.vec3Pos;
            v->Normal   = pmdVertex.vec3Normal;
            v->TexUV    = pmdVertex.uvTex;
            v->Color    = 0xffffffff;

            v->BlendIndices[ 0 ] = static_cast<lnFloat>( pmdVertex.unBoneNo[ 0 ] );
            v->BlendIndices[ 1 ] = static_cast<lnFloat>( pmdVertex.unBoneNo[ 1 ] );
            v->BlendIndices[ 2 ] = 0;
            v->BlendIndices[ 3 ] = 0;
            v->BlendWeights[ 0 ] = pmdVertex.cbWeight * 0.01f;
            v->BlendWeights[ 1 ] = 1.0f - v->BlendWeights[ 0 ];
            v->BlendWeights[ 2 ] = 0.0f;
            v->BlendWeights[ 3 ] = 0.0f;
	    }
        modelCore->VertexBuffer->unlock();

		//-----------------------------------------------------
	    // ���_�C���f�b�N�X
	    lnU32 indexCount = reader.readUInt32();

        // �C���f�b�N�X�o�b�t�@�쐬 (PMD �� 16bit)
        modelCore->IndexBuffer = device->createIndexBuffer( 
			indexCount, NULL, true, true );

	    // ���_�C���f�b�N�X�z����R�s�[
        lnU16* indices = (lnU16*)modelCore->IndexBuffer->lock();
		reader.read( indices, sizeof( lnU16 ) * indexCount );
        modelCore->IndexBuffer->unlock();

		//-----------------------------------------------------
	    // �}�e���A��
		lnU32 attrCount = reader.readUInt32();

		modelCore->Material.Materials.resize( attrCount );
		modelCore->Material.Attributes.resize( attrCount );
		int indexAttrOffset = 0;
		for ( lnU32 i = 0 ; i < attrCount ; ++i )
	    {
			PMD_Material pmdMaterial;
			reader.read( &pmdMaterial, sizeof(PMD_Material) );

			LMaterial* m = &modelCore->Material.Materials[i];

            m->Diffuse = pmdMaterial.col4Diffuse;
            m->Power = pmdMaterial.fShininess;

            m->Specular.R = pmdMaterial.col3Specular.x;
            m->Specular.G = pmdMaterial.col3Specular.y;
            m->Specular.B = pmdMaterial.col3Specular.z;
            m->Specular.A = 1.0f;

            m->Ambient.R = pmdMaterial.col3Ambient.x;
            m->Ambient.G = pmdMaterial.col3Ambient.y;
            m->Ambient.B = pmdMaterial.col3Ambient.z;

			// �e�N�X�`����������ꍇ�̓e�N�X�`���쐬
		    if ( pmdMaterial.szTextureFileName[0] != '\0' )
		    {
                char* sphereName = NULL;
                char* c = pmdMaterial.szTextureFileName;
                while ( *c ) {
                    if ( *c == '*' ) {
                        *c = '\0';
                        sphereName = ( c + 1 );
                        break;
                    }
                    ++c;
                }

				FileIO::PathNameW filePath(baseDir, pmdMaterial.szTextureFileName);
				Texture* tex = manager->createTexture(filePath, 0);
                m->setTexture( tex );
                LN_SAFE_RELEASE( tex );

                // �X�t�B�A�e�N�X�`��
                if ( sphereName )
                {
					tex = manager->createTexture(filePath, 0);
                    m->setSphereTexture( tex );
                    LN_SAFE_RELEASE( tex );
                }                
		    }

			// �����e�[�u���𖄂߂�
			LNMeshAttribute* attr = &modelCore->Material.Attributes[i];
            attr->MaterialIndex = i;
            attr->StartIndex = indexAttrOffset;
			attr->PrimitiveNum = pmdMaterial.ulNumIndices / 3;
			indexAttrOffset += pmdMaterial.ulNumIndices;
		}

		//-----------------------------------------------------
	    // �{�[��
	    lnU16 frameCount = reader.readUInt16();

		// �e�{�[�����C���f�b�N�X����E�����߁A�܂��͂��ׂăC���X�^���X��
		modelCore->FrameArray.resize( frameCount );
		for ( lnU16 i = 0 ; i < frameCount ; ++i ) {
			modelCore->FrameArray[i] = LN_NEW ModelFrameCore();
		}

		// �f�[�^�ǂݍ���
		for ( lnU16 i = 0 ; i < frameCount ; ++i )
	    {
			PMD_Bone pmdBone;
			reader.read( &pmdBone, sizeof(PMD_Bone) );

			ModelFrameCore* frameCore = modelCore->FrameArray[i];

			// �{�[����
			char name[ 21 ];
			strncpy( name, pmdBone.szName, 20 );
			name[ 20 ] = '\0';

			frameCore->initialize( 
				modelCore,
				i,
				pmdBone.nParentNo,
				lnRefString( name ),
				pmdBone.vec3Position );

			// �e��������΃��[�g�{�[������
			if ( frameCore->ParentFrameIndex == NULL ) {
				modelCore->RootFrames.push_back( frameCore );
			}

			// �Ђ����ɂ� IK���~�b�^��ݒ�
			trySetIKLimitter( name, frameCore );

			
			static const lnU8 LeftKnee[] =	{ 0x8D, 0xB6, 0x82, 0xD0, 0x82, 0xB4, 0x00 };	// SJIS "���Ђ�"
			static const lnU8 RightKnee[] =	{ 0x89, 0x45, 0x82, 0xD0, 0x82, 0xB4, 0x00 };	// SJIS "�E�Ђ�"
			if ( strcmp( name, (const char*)LeftKnee ) == 0 )
			{
				mLeftKneeBoneIndex = i;
			}
			else if ( strcmp( name, (const char*)RightKnee ) == 0 )
			{
				mRightKneeBoneIndex = i;
			}

			// "�Z���^�[" ��ێ�
			static const lnU8 SJISCenterName[] = { 0x83, 0x5A, 0x83, 0x93, 0x83, 0x5E, 0x81, 0x5B };	// SJIS "�Z���^�["
			if ( memcmp( name, SJISCenterName, 8 ) == 0 )
			{
				mCenterBoneIndex = i;
			}
		}

		//// �㏈��
		//for ( lnU16 i = 0 ; i < frameCount ; ++i ) {
		//	modelCore->FrameArray[i]->postInitialize();
		//}

		// �Z���^�[�{�[��������
        if ( mCenterBoneIndex == -1 ) return NULL;

		//-----------------------------------------------------
	    // IK
		lnU16 ikCount = reader.readUInt16();

	    // IK�z����쐬
	    if ( ikCount > 0 )
	    {
			modelCore->IKArray.resize( ikCount );
			for ( lnU16 i = 0 ; i < ikCount ; ++i ) 
			{
				PMD_IK2 pmdIK;
				reader.read( &pmdIK, sizeof(PMD_IK2) );

				ModelIKCore* ik = LN_NEW ModelIKCore();
				modelCore->IKArray[i] = ik;

				ik->OwnerModelCore	= modelCore;
				ik->TargetBoneIndex = pmdIK.nTargetNo;
				ik->EffBoneIndex	= pmdIK.nEffNo;
				ik->CalcCount		= pmdIK.unCount;
				ik->Fact			= pmdIK.fFact * 3.1415926f;

				// �V
				ModelIKCore2* ik2 = LN_NEW ModelIKCore2();
				modelCore->IKs.push_back( ik2 );
				ik2->TargetBoneIndex	= pmdIK.nTargetNo;
				ik2->EffectorBoneIndex	= pmdIK.nEffNo;							// ���ݏ������̃t���[���ԍ�
				ik2->LoopCount			= pmdIK.unCount;
				ik2->IKRotateLimit		= LMath::PI * pmdIK.fFact * (i + 1);


				// IK�����N�z��̍쐬 (�{�[���ԍ��͍~���Ŋi�[����Ă���)
				ik->IKBoneIndexArray.resize( pmdIK.cbNumLink );
				for( unsigned char j = 0 ; j < pmdIK.cbNumLink; ++j )
				{
					ik->IKBoneIndexArray[j] = reader.readUInt16();

					// �V
					ModelIKLink ikLink;
					ikLink.LinkBoneIndex = ik->IKBoneIndexArray[j];
					ikLink.IsRotateLimit = false;
					if ( ikLink.LinkBoneIndex == mLeftKneeBoneIndex || ikLink.LinkBoneIndex == mRightKneeBoneIndex )
					{
						// TODO: �����p�����[�^
						ikLink.IsRotateLimit = true;
						ikLink.MinLimit = LVector3( -3.14159f, 0, 0 );
						ikLink.MaxLimit = LVector3( -0.002f, 0, 0 );
					}
					ik2->IKLinks.push_back( ikLink );
				}

				ik->SortVal			= ik->IKBoneIndexArray[0];
			}

			modelCore->sortIKArray();
	    }

		//-----------------------------------------------------
	    // �\��
	    lnU16 faceCount = reader.readUInt16();

	    // �\��z����쐬
	    if ( faceCount > 0 )
	    {
			Base::TempBuffer tmpBuffer;

			modelCore->ModelMorphCores.resize( faceCount );
		    for ( lnU16 i = 0 ; i < faceCount; ++i )
		    {
				PMD_Face2 pmdFace;
				reader.read( &pmdFace, sizeof(PMD_Face2) );

				ModelMorphCore* morph = LN_NEW ModelMorphCore();
				modelCore->ModelMorphCores[i] = morph;

				// �\�
				char name[ 21 ];
				strncpy( name, pmdFace.szName, 20 );
				name[ 20 ] = '\0';
				morph->Name = name;

				// ���_��
				morph->VerticesCount = pmdFace.ulNumVertices;

				// ���_�f�[�^
				//morph->TargetVertices = LN_NEW PMD_FaceVtx[morph->VerticesCount];
				lnByte* buf = tmpBuffer.request( sizeof(PMD_FaceVtx) * morph->VerticesCount );
				reader.read( buf, sizeof(PMD_FaceVtx) * morph->VerticesCount );

				morph->TargetVertices = LN_NEW ModelMorphCore::MorphVertex[morph->VerticesCount];
				for ( int i = 0; i < morph->VerticesCount; i++ )
				{
					morph->TargetVertices[i].Position		= ((PMD_FaceVtx*)buf)[i].vec3Pos;
					morph->TargetVertices[i].TargetIndex	= ((PMD_FaceVtx*)buf)[i].ulIndex;
					morph->TargetVertices[i].TempBufferTargetIndex = 0;
				}

				// base�Ƃ̑��΃C���f�b�N�X���΃C���f�b�N�X�ɕϊ� (base ��[0] �� face)
				if ( i > 0 )
				{
					ModelMorphCore::MorphVertex* v = morph->TargetVertices;
					for (lnU32 iVertex = 0; iVertex < morph->VerticesCount; iVertex++, v++)
					{
						v->Position    = modelCore->ModelMorphCores[0]->TargetVertices[v->TargetIndex].Position + v->Position;
						v->TargetIndex = modelCore->ModelMorphCores[0]->TargetVertices[v->TargetIndex].TargetIndex;
					}
				}
		    }
	    }

		//-----------------------------------------------------
	    // �������獄�̏��܂œǂݔ�΂�

	    // �\��g�p�\�����X�g
		lnU8 faceDispCount = reader.readUInt8();
		reader.seek( sizeof(lnU16) * faceDispCount );

	    // �{�[���g�p�g�����X�g
		lnU8 boneDispCount = reader.readUInt8();
		reader.seek( sizeof(char) * 50 * boneDispCount );

	    // �{�[���g�p�\�����X�g
		lnU32 boneFrameDispCount = reader.readUInt32();
		reader.seek( 3 * boneFrameDispCount );

	    // �㑱�f�[�^�̗L�����`�F�b�N
		if ( reader.isEOF() ) {
			modelCore.addRef();
			return modelCore;
		}

	    // �p�ꖼ�Ή�
		lnU8 englishNameExist = reader.readUInt8();
	    if ( englishNameExist )
	    {
		    // ���f�����ƃR�����g(�p��)
			reader.seek( sizeof(char) * 20 );
			reader.seek( sizeof(char) * 256 );

		    // �{�[����(�p��)
            for ( lnU32 i = 0; i < frameCount; ++i ) {
				reader.seek( sizeof(char) * 20 );
            }

		    // �\�(�p��)
            for ( lnU16 i = 0; i < faceCount - 1; ++i )	{ // "base"�͉p�ꖼ���X�g�ɂ͊܂܂�Ȃ����� -1
		        reader.seek( sizeof(char) * 20 );
            }

		    // �{�[���g�p�g�����X�g(�p��)
			reader.seek( sizeof( char ) * 50 * boneDispCount );
	    }

	    // �㑱�f�[�^�̗L�����`�F�b�N(�����̃`�F�b�N�͕s�v����)
		if ( reader.isEOF() ) {
			modelCore.addRef();
			return modelCore;
		}

	    // �g�D�[���e�N�X�`�����X�g
        for ( int i = 0; i < 10; ++i )
        {
            //char* name = (char*)data;// �t�@�C���� 100Byte * 10��
			// toon01.bmp�`toon10.bmp �܂��� .bmp �t�@�C���܂ł̑��΃p�X���i�[����Ă���
			reader.seek( sizeof(char) * 100 );
        }	

		// �㑱�f�[�^�̗L�����`�F�b�N
	    if ( reader.isEOF() ) {
			modelCore.addRef();
			return modelCore;
		}

	    // �����܂œǂݔ�΂�

		//-----------------------------------------------------
	    // ����
		lnU32 rigidBodyCount = reader.readUInt32();

	    // ���̔z����쐬
	    if ( rigidBodyCount > 0 )
	    {
			modelCore->RigidBodys.resize( rigidBodyCount );
		    for ( lnU32 i = 0 ; i < rigidBodyCount ; ++i )
		    {
				PMD_RigidBody pmdBody;
				reader.read( &pmdBody, sizeof(PMD_RigidBody) );

				ModelRigidBodyCore* body = LN_NEW ModelRigidBodyCore();
				modelCore->RigidBodys[i] = body;

				// ���̂̎��
				switch ( pmdBody.cbRigidBodyType )
				{
					default:
					case 0: 
            			body->RigidBodyType = RigidBodyType_ControlledByBone; 
						break;
					case 1: 
            			body->RigidBodyType = RigidBodyType_Physics; 
						break;
					case 2: 
            			body->RigidBodyType = RigidBodyType_PhysicsAlignment; 
						break;
				}

				// �`��
				switch ( pmdBody.cbShapeType )
				{
					case 0:		// ��
						body->ColShapeData.Type = CollisionShapeType_Sphere;
						body->ColShapeData.Sphere.Radius = pmdBody.fWidth;
						break;
					case 1:		// ��
						body->ColShapeData.Type = CollisionShapeType_Box;
						body->ColShapeData.Box.Width  = pmdBody.fWidth;
						body->ColShapeData.Box.Height = pmdBody.fHeight;
						body->ColShapeData.Box.Depth  = pmdBody.fDepth;
						break;
					case 2:		// �J�v�Z��
						body->ColShapeData.Type = CollisionShapeType_Capsule;
						body->ColShapeData.Capsule.Radius = pmdBody.fWidth;
						body->ColShapeData.Capsule.Height = pmdBody.fHeight;
						break;
				}

				// �{�[���s�� �� ���̍s��ϊ��p
				LMatrix::rotationYawPitchRoll( 
					&body->BoneOffset, 
					pmdBody.vec3Rotation.y,
					pmdBody.vec3Rotation.x,
					pmdBody.vec3Rotation.z );
				body->BoneOffset.translation( pmdBody.vec3Position );
				//LMatrix bias;
				//bias.rotationZ( pmdBody.vec3Rotation.z );
				//bias.rotationX( pmdBody.vec3Rotation.x );
				//bias.rotationY( pmdBody.vec3Rotation.y );
				//bias.translation( pmdBody.vec3Position );
				//body->BoneOffset = bias;

				// ���̍s�� �� �{�[���s��ϊ��p
				LMatrix::inverse( &body->InvBoneOffset, body->BoneOffset );
				
				// ���̏��
				body->Mass = pmdBody.fMass;
				body->Group = 0x0001 << pmdBody.cbColGroupIndex;
				body->GroupMask = pmdBody.unColGroupMask;
				body->Restitution = pmdBody.fRestitution;
				body->Friction = pmdBody.fFriction;
				body->LinearDamping = pmdBody.fLinearDamping;
				body->AngularDamping = pmdBody.fAngularDamping;

				// �֘A�{�[��
				if ( pmdBody.unBoneIndex == 0xFFFF )
					body->RelatedFrameIndex = mCenterBoneIndex;
				else
					body->RelatedFrameIndex = pmdBody.unBoneIndex;
		    }
	    }

		//-----------------------------------------------------
	    // �R���X�g���C���g
		lnU32 constraintCount = reader.readUInt32();

	    // �R���X�g���C���g�z����쐬
	    if ( constraintCount > 0 )
	    {
			modelCore->Constraints.resize( constraintCount );
			for( lnU32 i = 0 ; i < constraintCount; ++i )
		    {
				PMD_Constraint pmdConstraint;
				reader.read( &pmdConstraint, sizeof(PMD_Constraint) );

				ModelConstraintCore* constraint = LN_NEW ModelConstraintCore();
				modelCore->Constraints[i] = constraint;

				char name[ 21 ];
				strncpy( name, pmdConstraint.szName, 20 );
				name[ 20 ] = '\0';
				constraint->Name = name;

				constraint->RigidBodyAIndex = pmdConstraint.ulRigidA;
				constraint->RigidBodyBIndex = pmdConstraint.ulRigidB;
				
				constraint->Position = pmdConstraint.vec3Position;
				constraint->Rotation = pmdConstraint.vec3Rotation;
				
				constraint->PositionLimitLower = pmdConstraint.vec3PosLimitL;
				constraint->PositionLimitUpper = pmdConstraint.vec3PosLimitU;
				
				constraint->RotationLimitLower = pmdConstraint.vec3RotLimitL;
				constraint->RotationLimitUpper = pmdConstraint.vec3RotLimitU;
				
				constraint->SpringPositionStiffness = pmdConstraint.vec3SpringPos;
#if 1
				constraint->SpringRotationStiffness = pmdConstraint.vec3SpringRot;
#else
				constraint->SpringRotationStiffness.x = LMath::DegToRad( pmdConstraint.vec3SpringRot.x );
				constraint->SpringRotationStiffness.y = LMath::DegToRad( pmdConstraint.vec3SpringRot.y );
				constraint->SpringRotationStiffness.z = LMath::DegToRad( pmdConstraint.vec3SpringRot.z );
#endif
			}
	    }
	    
		modelCore.addRef();
		return modelCore;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void MMDModelLoader::trySetIKLimitter( const char* sjisName, ModelFrameCore* frame )
	{
		// MMDX �X�^�C���� IK ���~�b�^
		static const lnU8 LeftKnee[] =	{ 0x8D, 0xB6, 0x82, 0xD0, 0x82, 0xB4 };	// SJIS "���Ђ�"
		static const lnU8 RightKnee[] =	{ 0x89, 0x45, 0x82, 0xD0, 0x82, 0xB4 };	// SJIS "�E�Ђ�"
		static const lnU8 LeftFoot[] =	{ 0x8D, 0xB6, 0x91, 0xAB };				// SJIS "����"
		static const lnU8 RightFoot[] =	{ 0x89, 0x45, 0x91, 0xAB };				// SJIS "����"

		// "���Ђ�" or "�E�Ђ�"
		if ( memcmp( sjisName, LeftKnee, 6 ) == 0
		||   memcmp( sjisName, RightKnee, 6 ) == 0
		){
			ModelIKLimitter2* limitter = LN_NEW ModelIKLimitter2();
			limitter->EnableRotationLimit = true;
			limitter->RotationMin = LVector3( LMath::DegToRad( 3.0f ), 0, 0 );	//3�x���炢������݂��Ă��Ə�肭�����B
			limitter->RotationMax = LVector3( LMath::PI, 0, 0 );
			limitter->Mirror[0] = true;
			limitter->Restitution = 0.99f;
			frame->IKLimitter = limitter;
		}
		// "����" or "����"
		else if ( memcmp( sjisName, LeftFoot, 4 ) == 0
		||		  memcmp( sjisName, RightFoot, 4 ) == 0
		){
			ModelIKLimitter2* limitter = LN_NEW ModelIKLimitter2();
			limitter->EnableAxisLimit = true;
			limitter->AxisLimits[0] = false;
            limitter->AxisLimits[1] = true;
            limitter->AxisLimits[2] = false;
			frame->IKLimitter = limitter;
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//void MMDModelLoader::trySetIKLimitter( const char* sjisName, ModelIKLink* ikLink )
	//{
	//	// MMDX �X�^�C���� IK ���~�b�^
	//	static const lnU8 LeftKnee[] =	{ 0x8D, 0xB6, 0x82, 0xD0, 0x82, 0xB4 };	// SJIS "���Ђ�"
	//	static const lnU8 RightKnee[] =	{ 0x89, 0x45, 0x82, 0xD0, 0x82, 0xB4 };	// SJIS "�E�Ђ�"
	//	static const lnU8 LeftFoot[] =	{ 0x8D, 0xB6, 0x91, 0xAB };				// SJIS "����"
	//	static const lnU8 RightFoot[] =	{ 0x89, 0x45, 0x91, 0xAB };				// SJIS "����"

	//	// "���Ђ�" or "�E�Ђ�"
	//	if ( memcmp( sjisName, LeftKnee, 6 ) == 0
	//	||   memcmp( sjisName, RightKnee, 6 ) == 0
	//	){
	//		ikLink->IsRotateLimit = true;
	//		ikLink->MinLimit = LVector3( LMath::DegToRad( 3.0f ), 0, 0 );	//3�x���炢������݂��Ă��Ə�肭�����B
	//		ikLink->MaxLimit = LVector3( LMath::PI, 0, 0 );

	//		// TOOD: ���̐�������Ȃ��݂���
	//	}
	//	// "����" or "����"
	//	else if ( memcmp( sjisName, LeftFoot, 4 ) == 0
	//	||		  memcmp( sjisName, RightFoot, 4 ) == 0
	//	){
	//		// MMDX �� Y ����]�݂̂ɐ������Ă��邪�A�Ƃ肠���������ŁB
	//	}
	//}

} // namespace Graphics
} // namespace Core
} // namespace LNote
