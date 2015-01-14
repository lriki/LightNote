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
	    // ヘッダ
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
		printf( "エンコード方式        : %d\n", pmxHeader.Data[0]);
		printf( "追加UV数              : %d\n", pmxHeader.Data[1]);
		printf( "頂点Indexサイズ       : %d\n", pmxHeader.Data[2]);
		printf( "テクスチャIndexサイズ : %d\n", pmxHeader.Data[3]);
		printf( "材質Indexサイズ       : %d\n", pmxHeader.Data[4]);
		printf( "ボーンIndexサイズ     : %d\n", pmxHeader.Data[5]);
		printf( "モーフIndexサイズ     : %d\n", pmxHeader.Data[6]);
		printf( "剛体Indexサイズ       : %d\n", pmxHeader.Data[7]);
#endif
		
	    // モデル情報
		loadModelInfo( &reader );

	    // 頂点
		loadVertices( &reader );

		// 頂点インデックス
		loadIndices( &reader );

		// テクスチャテーブル
		loadTextureTable( &reader, baseDir );

		// マテリアル
		loadMaterials( &reader );

		// ボーン
		loadBones( &reader );

		// モーフ
		loadMorphs( &reader );

		// 表示枠
		loadDisplayFrame( &reader );

		// 剛体
		loadRigidBodys( &reader );

		// ジョイント
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
	    // モデル情報

		// モデル名
		lnS32 byteSize = reader->readInt32();
		mModelCore->Name = readString( reader, byteSize );

		// モデル名英
		byteSize = reader->readInt32();
		reader->seek( byteSize );

		// コメント
		byteSize = reader->readInt32();
		reader->seek( byteSize );

		// コメント英
		byteSize = reader->readInt32();
		reader->seek( byteSize );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void PMXLoader::loadVertices( FileIO::BinaryReader* reader )
	{
		//-----------------------------------------------------
	    // 頂点

		// 頂点数
		int vertexCount = reader->readInt32();

		// 頂点バッファ作成
		mModelCore->VertexBuffer = mGraphicsDevice->createVertexBuffer( 
			PMX_Vertex::elements(), vertexCount, NULL, true );

	    // データを流し込む
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

			// 頂点、法線、テクスチャUV
			reader->read( &baseVertex, sizeof(BaseVertex) );
			v->Position = baseVertex.Position;
			v->Normal = baseVertex.Normal;
			v->TexUV = baseVertex.TexUV;

			// 追加UV
			for ( int j = 0; j < getAdditionalUVCount(); i++)
			{
				v->AdditionalUV[i].Set(
					reader->readFloat(),
					reader->readFloat(),
					reader->readFloat(),
					reader->readFloat() );
			}

			// ブレンドウェイト
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
					reader->read( &v->SdefR1, sizeof(lnFloat) * 3 );	// TODO:※修正値を要計算
					break;
			}

			// エッジ倍率
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
	    // 頂点インデックス

		// インデックス数
		int indexCount = reader->readInt32();

		// インデックスバッファ作成
		mModelCore->IndexBuffer = mGraphicsDevice->createIndexBuffer( 
			indexCount, NULL, true, (getVertexIndexSize() <= 2) );

		// 1 バイトインデックス
		if ( getVertexIndexSize() == 1 )
		{
			//TODO:未対応
			LN_ERR2_ASSERT( 0 );
		}
		// 2 or 4 バイトインデックス
		else
		{
			// そのままコピー
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
	    // テクスチャ

		// テクスチャ数
		int textureCount = reader->readInt32();

		// テクスチャテーブル
		mTextureTable.reserve( textureCount );

		for (int i = 0 ; i < textureCount; ++i )
		{
			// テクスチャ名
			lnS32 byteSize = reader->readInt32();
			lnRefStringW name = readString( reader, byteSize );

			// 作成
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
	    // マテリアル・属性テーブル

		// マテリアル数
		int materialCount = reader->readInt32();

		// メモリ確保
		mModelCore->Material.Materials.resize( materialCount );
		mModelCore->Material.Attributes.resize( materialCount );

		int indexAttrOffset = 0;
		for ( int i = 0 ; i < materialCount; ++i )
		{
			LMaterial* m = &mModelCore->Material.Materials[i];

			// 材質名
			lnS32 byteSize = reader->readInt32();
			reader->seek( byteSize );

			// 材質英名
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

			// Specular係数
			m->Power = reader->readFloat();

			// Ambient
			reader->read( &color, sizeof(lnFloat) * 3 );
			m->Ambient.R = color.X;
            m->Ambient.G = color.Y;
            m->Ambient.B = color.Z;
            m->Ambient.A = 1.0f;

			// 描画フラグ
			lnU8 bitFlag = reader->readInt8();
			m->DrawingFlags = bitFlag;

			// エッジ色
			reader->read( &m->EdgeColor, sizeof(lnFloat) * 4 );

			// エッジサイズ
			m->EdgeSize = reader->readFloat();

			// テクスチャ
			int normalTexture = reader->readInt( getTextureIndexSize() );
			if ( normalTexture >= 0 )
				m->setTexture( mTextureTable[normalTexture] );

			// スフィアテクスチャ
			int sphereTexture = reader->readInt( getTextureIndexSize() );
			m->SphereMode = (enum Material::SphereMode)reader->readInt8();
			
			// トゥーンテクスチャ
			int shareToon = reader->readInt8();
			if ( shareToon == 0 )
			{
				int index = reader->readInt( getTextureIndexSize() );
				if ( index >= 0 )
					m->setToonTexture( mTextureTable[index] );
			}
			// 共有Toonテクスチャ (toon01.bmp～toon10.bmp)
			else
			{
				int index = reader->readInt8();
				m->setToonTexture( mManager->getMMDDefaultToonTexture( index ) );
			}

			// 自由欄
			byteSize = reader->readInt32();
			reader->seek( byteSize );

			// マテリアルに対応する頂点数
			int vc = reader->readInt32();

			// 属性テーブルを埋める
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
		// ボーン数
		int boneCount = reader->readInt32();

		// 親ボーンをインデックスから拾うため、まずはすべてインスタンス化
		mModelCore->FrameArray.resize( boneCount );
		for ( lnU16 i = 0 ; i < boneCount; ++i ) {
			mModelCore->FrameArray[i] = LN_NEW ModelFrameCore();
		}

		// データ読み込み
		for ( lnU16 i = 0 ; i < boneCount; ++i )
	    {
			ModelFrameCore* frameCore = mModelCore->FrameArray[i];
			frameCore->OwnerModelCore = mModelCore;
			frameCore->FrameIndex = i;

			// ボーン名
			frameCore->Name = readTextBuf( reader );

			// ボーン英名
			/*使わない*/ readTextBuf( reader );

			// 初期位置
			reader->read( &frameCore->OrgPosition, sizeof(lnFloat) * 3 );

			// 親ボーンのボーンIndex
			frameCore->ParentFrameIndex = reader->readInt( getBoneIndexSize() );

			// 変形階層
			frameCore->TransformLevel = reader->readInt32();

			// ボーンフラグ
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
				// 座標オフセット, ボーン位置からの相対分
				reader->read( &frameCore->PositionOffset, sizeof(lnFloat) * 3 );
            }
            else
            {
				// 接続先ボーンのボーンIndex
                frameCore->ConnectedBoneIndex = reader->readInt( getBoneIndexSize() );
            }
			
			// 回転付与:1 または 移動付与:1 の場合
			if ( frameCore->IsRotateProvided || frameCore->IsMoveProvided )
            {
				// 付与親ボーンのボーンIndex
                frameCore->ProvidedParentBoneIndex = reader->readInt( getBoneIndexSize() );
				frameCore->ProvidedRatio = reader->readFloat();
            }

			// 軸固定:1 の場合
            if ( frameCore->IsFixAxis ) {
				reader->read( &frameCore->AxisDirectionVector, sizeof(lnFloat) * 3 );
			}

			//  ローカル軸:1 の場合
            if ( frameCore->IsLocalAxis )
            {
				reader->read( &frameCore->DimentionXDirectionVector, sizeof(lnFloat) * 3 );
				reader->read( &frameCore->DimentionZDirectionVector, sizeof(lnFloat) * 3 );
            }

			// 外部親変形:1 の場合
            if ( frameCore->ParentTransform ) {
				frameCore->KeyValue = reader->readInt32();
			}

			// IK:1 の場合
            if ( frameCore->IsIK )
            {
#if 1	// 削除予定
	//            frameCore->IKTargetBoneIndex = reader->readInt( getBoneIndexSize() );
	//            frameCore->IKLoopCount = reader->readInt32();
				//frameCore->IKRotateLimit = reader->readFloat();

				// 新
				ModelIKCore2* ik = LN_NEW ModelIKCore2();
				mModelCore->IKs.push_back( ik );
				ik->TargetBoneIndex		= i;							// 現在処理中のフレーム番号
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

					// 新
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
		// モーフ数
		int boneCount = reader->readInt32();
		mModelCore->ModelMorphCores.resize( boneCount );

		// データ読み込み
		for ( lnU16 i = 0 ; i < boneCount; ++i )
	    {
			ModelMorphCore* morphCore = LN_NEW ModelMorphCore();
			mModelCore->ModelMorphCores[i] = morphCore;

			// モーフ名
			morphCore->Name = readTextBuf( reader );

			// モーフ英名
			/*使わない*/ readTextBuf( reader );

			// 操作パネル
			morphCore->OperationPanel = reader->readInt8();

			// モーフ種類
			int morphType = reader->readInt8();

			// モーフオフセット
			morphCore->MorphOffsets.resize( reader->readInt32() );
			for ( int i = 0; i < morphCore->MorphOffsets.size(); i++ )
			{
				ModelMorphCore::MorphOffset* mo = &morphCore->MorphOffsets[i];
				switch ( morphType )
				{
					case 0:		// グループモーフ
						morphCore->MorphType = ModelMorphType_Group;
						mo->GroupMorphOffset.MorphIndex = reader->readInt( getMorphIndexSize() );
						mo->GroupMorphOffset.MorphRatio = reader->readFloat();
                        break;
                    case 1:		// 頂点モーフ
						// VertexIndex はモデル本体の頂点インデックス
						// PositionOffset は元の位置からの相対位置
						morphCore->MorphType = ModelMorphType_Vertex;
						mo->VertexMorphOffset.VertexIndex = reader->readInt( getVertexIndexSize() );
						reader->read( &mo->VertexMorphOffset.PositionOffset, sizeof(float) * 3 );
                        break;
                    case 2:		// ボーンモーフ
						morphCore->MorphType = ModelMorphType_Bone;
						mo->BoneMorphOffset.BoneIndex = reader->readInt( getVertexIndexSize() );
						reader->read( &mo->BoneMorphOffset.Moving, sizeof(float) * 3 );
						reader->read( &mo->BoneMorphOffset.Rotating, sizeof(float) * 4 );
                        break;
                    case 3:		// UVモーフ
						morphCore->MorphType = ModelMorphType_UV;
						mo->UVMorphOffset.VertexIndex = reader->readInt( getVertexIndexSize() );
						reader->read( &mo->UVMorphOffset.UVOffset, sizeof(float) * 4 );
                        break;
                    case 4:		// 追加UVモーフ1
						morphCore->MorphType = ModelMorphType_AdditionalUV1;
						mo->UVMorphOffset.VertexIndex = reader->readInt( getVertexIndexSize() );
						reader->read( &mo->UVMorphOffset.UVOffset, sizeof(float) * 4 );
                        break;
                    case 5:		// 追加UVモーフ2
						morphCore->MorphType = ModelMorphType_AdditionalUV2;
						mo->UVMorphOffset.VertexIndex = reader->readInt( getVertexIndexSize() );
						reader->read( &mo->UVMorphOffset.UVOffset, sizeof(float) * 4 );
                        break;
                    case 6:		// 追加UVモーフ3
						morphCore->MorphType = ModelMorphType_AdditionalUV3;
						mo->UVMorphOffset.VertexIndex = reader->readInt( getVertexIndexSize() );
						reader->read( &mo->UVMorphOffset.UVOffset, sizeof(float) * 4 );
                        break;
                    case 7:		// 追加UVモーフ4
						morphCore->MorphType = ModelMorphType_AdditionalUV4;
						mo->UVMorphOffset.VertexIndex = reader->readInt( getVertexIndexSize() );
						reader->read( &mo->UVMorphOffset.UVOffset, sizeof(float) * 4 );
                        break;
                    case 8:		// 材質モーフ
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
                    case 9:		// Flipモーフ
						morphCore->MorphType = ModelMorphType_Flip;
						mo->FlipMorphOffset.MorphIndex = reader->readInt( getMorphIndexSize() );
						mo->FlipMorphOffset.MorphValue = reader->readFloat();
                        break;
                    case 10:	// Impulseモーフ
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
		// 表示枠はすべて読み飛ばす
		int displayFrameCount = reader->readInt32();
		for ( int i = 0; i < displayFrameCount; i++ )
		{
			// 枠名
			lnS32 byteSize = reader->readInt32();
			reader->seek( byteSize );

			// 枠名英
			byteSize = reader->readInt32();
			reader->seek( byteSize );

			// 特殊枠フラグ
			reader->seek( sizeof(lnU8) );

			// 枠内要素数
			int frameElementCount = reader->readInt32();
			for ( int j = 0; j < frameElementCount; j++ )
			{
				// 要素対象
				lnU8 type = reader->readUInt8();
				switch( type )
				{
				case 0:	// ボーンがターゲットの場合
					reader->readInt( getBoneIndexSize() );
					break;
				case 1:	// モーフがターゲットの場合
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
		// 剛体数
		int bodyCount = reader->readInt32();
		mModelCore->RigidBodys.resize( bodyCount );

		// データ読み込み
		for ( lnU16 i = 0 ; i < bodyCount; ++i )
	    {
			ModelRigidBodyCore* bodyCore = LN_NEW ModelRigidBodyCore();
			mModelCore->RigidBodys[i] = bodyCore;

			// 剛体名
			bodyCore->Name = readTextBuf( reader );

			// 剛体英名
			/*使わない*/ readTextBuf( reader );

			// 関連ボーンIndex - 関連なしの場合は-1
			bodyCore->RelatedFrameIndex = reader->readInt( getBoneIndexSize() );

			// グループ
			bodyCore->Group = (1 << reader->readUInt8());

			// 非衝突グループフラグ
			bodyCore->GroupMask = reader->readUInt16();

			// 形状 - 0:球 1:箱 2:カプセル
			lnU8 type = reader->readUInt8();

			// サイズ(x,y,z)
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
			
			// 位置(x,y,z)
			LVector3 Position;
			reader->read( &Position, sizeof(float) * 3 );

			// 回転(x,y,z) -> ラジアン角
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

			// オフセット行列化
			LMatrix bias;
			bias.RotationZ( Rotation.Z );
			bias.RotationX( Rotation.X );
			bias.RotationY( Rotation.Y );
			bias.Translation( Position - mModelCore->FrameArray[bodyCore->RelatedFrameIndex]->OrgPosition );
			bodyCore->BoneOffset = bias;
			bodyCore->InvBoneOffset = LMatrix::Inverse(bodyCore->BoneOffset);

			// 剛体基本情報
			bodyCore->Mass				= reader->readFloat();
			bodyCore->LinearDamping		= reader->readFloat();
			bodyCore->AngularDamping	= reader->readFloat();
			bodyCore->Restitution		= reader->readFloat();
			bodyCore->Friction			= reader->readFloat();

			// 剛体の物理演算 - 0:ボーン追従(static) 1:物理演算(dynamic) 2:物理演算 + Bone位置合わせ
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
		// ジョイント数
		int jointCount = reader->readInt32();
		mModelCore->Constraints.resize( jointCount );

		// データ読み込み
		for ( lnU16 i = 0 ; i < jointCount; ++i )
	    {
			ModelConstraintCore* jointCore = LN_NEW ModelConstraintCore();
			mModelCore->Constraints[i] = jointCore;

			// Joint名
			jointCore->Name = readTextBuf( reader );

			// Joint名英
			/*使わない*/ readTextBuf( reader );

			// Joint種類 - 0:スプリング6DOF   | PMX2.0では 0 のみ(拡張用)
			int type = reader->readUInt8();
			LN_THROW_InvalidFormat(type == 0);

			// 後は PMD と同じ
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
