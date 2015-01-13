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
	    // ヘッダのチェック
		PMD_Header pmdHeader;
		reader.read( &pmdHeader, sizeof(PMD_Header) );

	    if( pmdHeader.szMagic[0] != 'P' || pmdHeader.szMagic[1] != 'm' || pmdHeader.szMagic[2] != 'd' || pmdHeader.fVersion != 1.0f ) {
			return NULL;
		}

        // モデル名
        char modelName[21];
	    strncpy( modelName, pmdHeader.szName, 20 );
	    modelName[20] = '\0';
		modelCore->Name = modelName;

		//-----------------------------------------------------
	    // 頂点
		lnU32 vertexCount = reader.readUInt32();

		// 頂点バッファ作成
        modelCore->VertexBuffer = device->createVertexBuffer(
			Vertex::PMDVertex::elements(), vertexCount, NULL, true );

		// データを流し込む
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
	    // 頂点インデックス
	    lnU32 indexCount = reader.readUInt32();

        // インデックスバッファ作成 (PMD は 16bit)
        modelCore->IndexBuffer = device->createIndexBuffer( 
			indexCount, NULL, true, true );

	    // 頂点インデックス配列をコピー
        lnU16* indices = (lnU16*)modelCore->IndexBuffer->lock();
		reader.read( indices, sizeof( lnU16 ) * indexCount );
        modelCore->IndexBuffer->unlock();

		//-----------------------------------------------------
	    // マテリアル
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

			// テクスチャ名がある場合はテクスチャ作成
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

                // スフィアテクスチャ
                if ( sphereName )
                {
					tex = manager->createTexture(filePath, 0);
                    m->setSphereTexture( tex );
                    LN_SAFE_RELEASE( tex );
                }                
		    }

			// 属性テーブルを埋める
			LNMeshAttribute* attr = &modelCore->Material.Attributes[i];
            attr->MaterialIndex = i;
            attr->StartIndex = indexAttrOffset;
			attr->PrimitiveNum = pmdMaterial.ulNumIndices / 3;
			indexAttrOffset += pmdMaterial.ulNumIndices;
		}

		//-----------------------------------------------------
	    // ボーン
	    lnU16 frameCount = reader.readUInt16();

		// 親ボーンをインデックスから拾うため、まずはすべてインスタンス化
		modelCore->FrameArray.resize( frameCount );
		for ( lnU16 i = 0 ; i < frameCount ; ++i ) {
			modelCore->FrameArray[i] = LN_NEW ModelFrameCore();
		}

		// データ読み込み
		for ( lnU16 i = 0 ; i < frameCount ; ++i )
	    {
			PMD_Bone pmdBone;
			reader.read( &pmdBone, sizeof(PMD_Bone) );

			ModelFrameCore* frameCore = modelCore->FrameArray[i];

			// ボーン名
			char name[ 21 ];
			strncpy( name, pmdBone.szName, 20 );
			name[ 20 ] = '\0';

			frameCore->initialize( 
				modelCore,
				i,
				pmdBone.nParentNo,
				lnRefString( name ),
				pmdBone.vec3Position );

			// 親が無ければルートボーン扱い
			if ( frameCore->ParentFrameIndex == NULL ) {
				modelCore->RootFrames.push_back( frameCore );
			}

			// ひざ等には IKリミッタを設定
			trySetIKLimitter( name, frameCore );

			
			static const lnU8 LeftKnee[] =	{ 0x8D, 0xB6, 0x82, 0xD0, 0x82, 0xB4, 0x00 };	// SJIS "左ひざ"
			static const lnU8 RightKnee[] =	{ 0x89, 0x45, 0x82, 0xD0, 0x82, 0xB4, 0x00 };	// SJIS "右ひざ"
			if ( strcmp( name, (const char*)LeftKnee ) == 0 )
			{
				mLeftKneeBoneIndex = i;
			}
			else if ( strcmp( name, (const char*)RightKnee ) == 0 )
			{
				mRightKneeBoneIndex = i;
			}

			// "センター" を保持
			static const lnU8 SJISCenterName[] = { 0x83, 0x5A, 0x83, 0x93, 0x83, 0x5E, 0x81, 0x5B };	// SJIS "センター"
			if ( memcmp( name, SJISCenterName, 8 ) == 0 )
			{
				mCenterBoneIndex = i;
			}
		}

		//// 後処理
		//for ( lnU16 i = 0 ; i < frameCount ; ++i ) {
		//	modelCore->FrameArray[i]->postInitialize();
		//}

		// センターボーンが無い
        if ( mCenterBoneIndex == -1 ) return NULL;

		//-----------------------------------------------------
	    // IK
		lnU16 ikCount = reader.readUInt16();

	    // IK配列を作成
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

				// 新
				ModelIKCore2* ik2 = LN_NEW ModelIKCore2();
				modelCore->IKs.push_back( ik2 );
				ik2->TargetBoneIndex	= pmdIK.nTargetNo;
				ik2->EffectorBoneIndex	= pmdIK.nEffNo;							// 現在処理中のフレーム番号
				ik2->LoopCount			= pmdIK.unCount;
				ik2->IKRotateLimit		= LMath::PI * pmdIK.fFact * (i + 1);


				// IKリンク配列の作成 (ボーン番号は降順で格納されている)
				ik->IKBoneIndexArray.resize( pmdIK.cbNumLink );
				for( unsigned char j = 0 ; j < pmdIK.cbNumLink; ++j )
				{
					ik->IKBoneIndexArray[j] = reader.readUInt16();

					// 新
					ModelIKLink ikLink;
					ikLink.LinkBoneIndex = ik->IKBoneIndexArray[j];
					ikLink.IsRotateLimit = false;
					if ( ikLink.LinkBoneIndex == mLeftKneeBoneIndex || ikLink.LinkBoneIndex == mRightKneeBoneIndex )
					{
						// TODO: 制限パラメータ
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
	    // 表情
	    lnU16 faceCount = reader.readUInt16();

	    // 表情配列を作成
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

				// 表情名
				char name[ 21 ];
				strncpy( name, pmdFace.szName, 20 );
				name[ 20 ] = '\0';
				morph->Name = name;

				// 頂点数
				morph->VerticesCount = pmdFace.ulNumVertices;

				// 頂点データ
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

				// baseとの相対インデックスを絶対インデックスに変換 (base は[0] の face)
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
	    // ここから剛体情報まで読み飛ばし

	    // 表情枠用表示リスト
		lnU8 faceDispCount = reader.readUInt8();
		reader.seek( sizeof(lnU16) * faceDispCount );

	    // ボーン枠用枠名リスト
		lnU8 boneDispCount = reader.readUInt8();
		reader.seek( sizeof(char) * 50 * boneDispCount );

	    // ボーン枠用表示リスト
		lnU32 boneFrameDispCount = reader.readUInt32();
		reader.seek( 3 * boneFrameDispCount );

	    // 後続データの有無をチェック
		if ( reader.isEOF() ) {
			modelCore.addRef();
			return modelCore;
		}

	    // 英語名対応
		lnU8 englishNameExist = reader.readUInt8();
	    if ( englishNameExist )
	    {
		    // モデル名とコメント(英語)
			reader.seek( sizeof(char) * 20 );
			reader.seek( sizeof(char) * 256 );

		    // ボーン名(英語)
            for ( lnU32 i = 0; i < frameCount; ++i ) {
				reader.seek( sizeof(char) * 20 );
            }

		    // 表情名(英語)
            for ( lnU16 i = 0; i < faceCount - 1; ++i )	{ // "base"は英語名リストには含まれないため -1
		        reader.seek( sizeof(char) * 20 );
            }

		    // ボーン枠用枠名リスト(英語)
			reader.seek( sizeof( char ) * 50 * boneDispCount );
	    }

	    // 後続データの有無をチェック(ここのチェックは不要かも)
		if ( reader.isEOF() ) {
			modelCore.addRef();
			return modelCore;
		}

	    // トゥーンテクスチャリスト
        for ( int i = 0; i < 10; ++i )
        {
            //char* name = (char*)data;// ファイル名 100Byte * 10個
			// toon01.bmp～toon10.bmp または .bmp ファイルまでの相対パスが格納されている
			reader.seek( sizeof(char) * 100 );
        }	

		// 後続データの有無をチェック
	    if ( reader.isEOF() ) {
			modelCore.addRef();
			return modelCore;
		}

	    // ここまで読み飛ばし

		//-----------------------------------------------------
	    // 剛体
		lnU32 rigidBodyCount = reader.readUInt32();

	    // 剛体配列を作成
	    if ( rigidBodyCount > 0 )
	    {
			modelCore->RigidBodys.resize( rigidBodyCount );
		    for ( lnU32 i = 0 ; i < rigidBodyCount ; ++i )
		    {
				PMD_RigidBody pmdBody;
				reader.read( &pmdBody, sizeof(PMD_RigidBody) );

				ModelRigidBodyCore* body = LN_NEW ModelRigidBodyCore();
				modelCore->RigidBodys[i] = body;

				// 剛体の種類
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

				// 形状
				switch ( pmdBody.cbShapeType )
				{
					case 0:		// 球
						body->ColShapeData.Type = CollisionShapeType_Sphere;
						body->ColShapeData.Sphere.Radius = pmdBody.fWidth;
						break;
					case 1:		// 箱
						body->ColShapeData.Type = CollisionShapeType_Box;
						body->ColShapeData.Box.Width  = pmdBody.fWidth;
						body->ColShapeData.Box.Height = pmdBody.fHeight;
						body->ColShapeData.Box.Depth  = pmdBody.fDepth;
						break;
					case 2:		// カプセル
						body->ColShapeData.Type = CollisionShapeType_Capsule;
						body->ColShapeData.Capsule.Radius = pmdBody.fWidth;
						body->ColShapeData.Capsule.Height = pmdBody.fHeight;
						break;
				}

				// ボーン行列 → 剛体行列変換用
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

				// 剛体行列 → ボーン行列変換用
				LMatrix::inverse( &body->InvBoneOffset, body->BoneOffset );
				
				// 剛体情報
				body->Mass = pmdBody.fMass;
				body->Group = 0x0001 << pmdBody.cbColGroupIndex;
				body->GroupMask = pmdBody.unColGroupMask;
				body->Restitution = pmdBody.fRestitution;
				body->Friction = pmdBody.fFriction;
				body->LinearDamping = pmdBody.fLinearDamping;
				body->AngularDamping = pmdBody.fAngularDamping;

				// 関連ボーン
				if ( pmdBody.unBoneIndex == 0xFFFF )
					body->RelatedFrameIndex = mCenterBoneIndex;
				else
					body->RelatedFrameIndex = pmdBody.unBoneIndex;
		    }
	    }

		//-----------------------------------------------------
	    // コンストレイント
		lnU32 constraintCount = reader.readUInt32();

	    // コンストレイント配列を作成
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
		// MMDX スタイルの IK リミッタ
		static const lnU8 LeftKnee[] =	{ 0x8D, 0xB6, 0x82, 0xD0, 0x82, 0xB4 };	// SJIS "左ひざ"
		static const lnU8 RightKnee[] =	{ 0x89, 0x45, 0x82, 0xD0, 0x82, 0xB4 };	// SJIS "右ひざ"
		static const lnU8 LeftFoot[] =	{ 0x8D, 0xB6, 0x91, 0xAB };				// SJIS "左足"
		static const lnU8 RightFoot[] =	{ 0x89, 0x45, 0x91, 0xAB };				// SJIS "左足"

		// "左ひざ" or "右ひざ"
		if ( memcmp( sjisName, LeftKnee, 6 ) == 0
		||   memcmp( sjisName, RightKnee, 6 ) == 0
		){
			ModelIKLimitter2* limitter = LN_NEW ModelIKLimitter2();
			limitter->EnableRotationLimit = true;
			limitter->RotationMin = LVector3( LMath::DegToRad( 3.0f ), 0, 0 );	//3度ぐらい制限を設けてやると上手くいく。
			limitter->RotationMax = LVector3( LMath::PI, 0, 0 );
			limitter->Mirror[0] = true;
			limitter->Restitution = 0.99f;
			frame->IKLimitter = limitter;
		}
		// "左足" or "左足"
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
	//	// MMDX スタイルの IK リミッタ
	//	static const lnU8 LeftKnee[] =	{ 0x8D, 0xB6, 0x82, 0xD0, 0x82, 0xB4 };	// SJIS "左ひざ"
	//	static const lnU8 RightKnee[] =	{ 0x89, 0x45, 0x82, 0xD0, 0x82, 0xB4 };	// SJIS "右ひざ"
	//	static const lnU8 LeftFoot[] =	{ 0x8D, 0xB6, 0x91, 0xAB };				// SJIS "左足"
	//	static const lnU8 RightFoot[] =	{ 0x89, 0x45, 0x91, 0xAB };				// SJIS "左足"

	//	// "左ひざ" or "右ひざ"
	//	if ( memcmp( sjisName, LeftKnee, 6 ) == 0
	//	||   memcmp( sjisName, RightKnee, 6 ) == 0
	//	){
	//		ikLink->IsRotateLimit = true;
	//		ikLink->MinLimit = LVector3( LMath::DegToRad( 3.0f ), 0, 0 );	//3度ぐらい制限を設けてやると上手くいく。
	//		ikLink->MaxLimit = LVector3( LMath::PI, 0, 0 );

	//		// TOOD: ↑の制限じゃないみたい
	//	}
	//	// "左足" or "左足"
	//	else if ( memcmp( sjisName, LeftFoot, 4 ) == 0
	//	||		  memcmp( sjisName, RightFoot, 4 ) == 0
	//	){
	//		// MMDX は Y 軸回転のみに制限しているが、とりあえず無しで。
	//	}
	//}

} // namespace Graphics
} // namespace Core
} // namespace LNote
