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

	// ファイルヘッダ
	struct PMD_Header
	{
		char	szMagic[3];		// "Pmd"
		float	fVersion;		// PMDバージョン番号
		char	szName[20];		// モデル名
		char	szComment[256];	// コメント(著作権表示など)
	};

	// 頂点データ
	struct PMD_Vertex
	{
		LVector3    vec3Pos;	// 座標
		LVector3    vec3Normal;	// 法線ベクトル
		LVector2    uvTex;		// テクスチャ座標

		unsigned short	unBoneNo[2];	// ボーン番号
		unsigned char	cbWeight;		// ブレンドの重み (0～100％)
		unsigned char	cbEdge;			// エッジフラグ
	};

	// マテリアルデータ
	struct PMD_Material
	{
		LVector4    col4Diffuse;
		float		fShininess;
		LVector3    col3Specular,
					col3Ambient;

		//unsigned short	unknown;
		unsigned char	ToonIndex;
		unsigned char	EdgeFlag;	///< 0:通常、1:エッジ無効　輪郭線が有効の場合


		unsigned long	ulNumIndices;			// この材質に対応する頂点数
		char			szTextureFileName[20];	// テクスチャファイル名
	};

	// ボーンデータ
	struct PMD_Bone
	{
		char	szName[20];			// ボーン名 (0x00 終端，余白は 0xFD)
		short	nParentNo;			// 親ボーン番号 (なければ -1)
		short	nChildNo;			// 子ボーン番号 (LNote では使ってない)

		unsigned char	cbKind;		// ボーンの種類
		unsigned short	unIKTarget;	// IK時のターゲットボーン

		LVector3		vec3Position;	// モデル原点からの位置
		// ※回転情報は含まない
	};
	/*
	kind
		0x00　通常ボーン(回転のみ)
		0x01　通常ボーン(移動可能)
		0x02　IKボーン(移動可能)
		0x03　見えない(選択可)
		0x04　IKボーン(回転のみ)
		0x05　回転追従(回転のみ)
		0x06　IK接続先
		0x07　見えない(選択不可)
		0x08　ひねり
		0x09　回転運動
	*/
	
	// IKデータ
	struct PMD_IK2
	{
		short			nTargetNo;	// IKターゲットボーン番号
		short			nEffNo;		// IK先端ボーン番号

		unsigned char	cbNumLink;	// IKを構成するボーンの数

		unsigned short	unCount;
		float			fFact;
	};

	// 表情データ
	struct PMD_Face2
	{
		char			szName[20];		// 表情名 (0x00 終端，余白は 0xFD)

		unsigned long	ulNumVertices;	// 表情頂点数
		unsigned char	cbType;			// 分類
	};

	// 剛体データ
	struct PMD_RigidBody
	{
		char			szName[20];		// 剛体名

		unsigned short	unBoneIndex;	// 関連ボーン番号
		unsigned char	cbColGroupIndex;// 衝突グループ
		unsigned short	unColGroupMask;	// 衝突グループマスク

		unsigned char	cbShapeType;	// 形状  0:球 1:箱 2:カプセル

		float			fWidth;			// 半径(幅)
		float			fHeight;		// 高さ
		float			fDepth;			// 奥行

		LVector3	    vec3Position;	// 位置(左手座標系。ボーン相対)
		LVector3	    vec3Rotation;	// 回転(左手座標系。radian)

		float			fMass;			// 質量
		float			fLinearDamping;	// 移動減
		float			fAngularDamping;// 回転減
		float			fRestitution;	// 反発力
		float			fFriction;		// 摩擦力

		unsigned char	cbRigidBodyType;// タイプ 0:Bone追従 1:物理演算 2:物理演算(Bone位置合せ)
	};

#pragma pack( pop )

public:

	/// 読み込み
	ModelCore2* load(ModelManager* manager, FileIO::Stream* stream, const FileIO::PathNameW& baseDir, bool isDynamic);

private:
	// [削除予定]
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
