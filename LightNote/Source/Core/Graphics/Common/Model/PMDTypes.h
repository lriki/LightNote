//==============================================================================
// PMDTypes 
//------------------------------------------------------------------------------
///**
//  @file       PMDTypes.h
//  @brief      PMDTypes
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../../Math/Vector2.h"
#include "../../../Math/Vector3.h"
#include "../../../Math/Vector4.h"
#include "../../../Math/Matrix.h"
#include "../../../Math/Quaternion.h"
#include "../Types.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

#pragma pack( push, 1 )

//-------------------------------------------------------------
// PMD

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
	short	nChildNo;			// 子ボーン番号

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
struct PMD_IK
{
	short			nTargetNo;	// IKターゲットボーン番号
	short			nEffNo;		// IK先端ボーン番号

	unsigned char	cbNumLink;	// IKを構成するボーンの数

	unsigned short	unCount;
	float			fFact;

	unsigned short	punLinkNo[1];// IKを構成するボーンの配列
};

// 表情頂点
struct PMD_FaceVtx
{
	unsigned long	ulIndex;
	LVector3	    vec3Pos;
};

// 表情データ (削除予定)
struct PMD_Face
{
	char			szName[20];		// 表情名 (0x00 終端，余白は 0xFD)

	unsigned long	ulNumVertices;	// 表情頂点数
	unsigned char	cbType;			// 分類

	PMD_FaceVtx		pVertices[1];	// 表情頂点データ
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

// コンストレイント(ジョイント)データ
struct PMD_Constraint
{
	char			szName[20];		// コンストレイント名

	unsigned long	ulRigidA;		// 剛体A
	unsigned long	ulRigidB;		// 剛体B

	LVector3	    vec3Position;	// 位置(モデル原点中心)
	LVector3	    vec3Rotation;	// 回転(radian)

	LVector3	    vec3PosLimitL;	// 移動制限1
	LVector3	    vec3PosLimitU;	// 移動制限2

	LVector3	    vec3RotLimitL;	// 回転制限1
	LVector3	    vec3RotLimitU;	// 回転制限2

	LVector3	    vec3SpringPos;	// ばね移動
	LVector3	    vec3SpringRot;	// ばね回転
};


//-------------------------------------------------------------
// VMD

// ファイルヘッダ
struct VMD_Header
{
	char	szHeader[30];			// "Vocaloid Motion Data 0002"
	char	szModelName[20];		// 対象モデル名
};

// モーションデータ
struct VMD_Motion
{
	char	szBoneName[15];			// ボーン名

	unsigned long	ulFrameNo;		// フレーム番号

	LVector3	vec3Position;			// 位置
	LQuaternion	vec4Rotation;			// 回転(クォータニオン)

	char	cInterpolationX[16];	// 補間情報 X軸移動
	char	cInterpolationY[16];	// 補間情報 Y軸移動
	char	cInterpolationZ[16];	// 補間情報 Z軸移動
	char	cInterpolationRot[16];	// 補間情報 回転
};

// 表情データ
struct VMD_Face
{
	char	szFaceName[15];		// 表情名

	unsigned long	ulFrameNo;	// フレーム番号

	float	fFactor;			// ブレンド率
};


//-------------------------------------------------------------
// PMX

enum PMX_Encode
{
	PMX_Encode_UTF16	= 0,
	PMX_Encode_UTF8		= 1,
};

struct PMX_Header
{
	char	Magic[4];		// "PMX " (PMX1.0は"Pmx ")
	float	Version;		// PMDバージョン番号
	lnU8	DataSize;		// 後続するデータ列のバイトサイズ  PMX2.0は 8 で固定
	lnByte	Data[8];		// [0] - エンコード方式  | 0:UTF16 1:UTF8
							// [1] - 追加UV数 	| 0～4 詳細は頂点参照
							// [2] - 頂点Indexサイズ | 1,2,4 のいずれか
							// [3] - テクスチャIndexサイズ | 1,2,4 のいずれか
							// [4] - 材質Indexサイズ | 1,2,4 のいずれか
							// [5] - ボーンIndexサイズ | 1,2,4 のいずれか
							// [6] - モーフIndexサイズ | 1,2,4 のいずれか
							// [7] - 剛体Indexサイズ | 1,2,4 のいずれか
};



struct PMX_Vertex
{
public:

	LVector3	Position;
	//LVector4	BlendWeight;
	//LVector4	BlendIndices;
    lnFloat     BlendWeights[4];	///< ボーンブレンド率
	lnFloat		BlendIndices[4];	///< ボーンインデックス
	LVector3	Normal;
	LVector2	TexUV;

	//LVector4	AddUV1;
	//LVector4	AddUV2;
	//LVector4	AddUV3;
	//LVector4	AddUV4;
	LVector4	AdditionalUV[4];
	
	LVector4	SdefC;				///< Sdef - C値
	LVector3	SdefR0;				///< Sdef - R0値
	LVector3	SdefR1;				///< Sdef - R1値

	float		EdgeWeight;			///< エッジウェイト
	float		Index;				///< 頂点インデックス値

	/// 頂点レイアウト
    static LNVertexElemenst* elements()
    {
	    static LNVertexElemenst elements[] = {
		    { 0, LN_DECL_FLOAT3, LN_USAGE_POSITION     , 0 },
            { 0, LN_DECL_FLOAT4, LN_USAGE_BLENDWEIGHT  , 0 },
            { 0, LN_DECL_FLOAT4, LN_USAGE_BLENDINDICES , 0 },
            { 0, LN_DECL_FLOAT3, LN_USAGE_NORMAL       , 0 },
            { 0, LN_DECL_FLOAT2, LN_USAGE_TEXCOORD     , 0 },
			
            { 0, LN_DECL_FLOAT4, LN_USAGE_TEXCOORD     , 1 },
            { 0, LN_DECL_FLOAT4, LN_USAGE_TEXCOORD     , 2 },
            { 0, LN_DECL_FLOAT4, LN_USAGE_TEXCOORD     , 3 },
            { 0, LN_DECL_FLOAT4, LN_USAGE_TEXCOORD     , 4 },

            { 0, LN_DECL_FLOAT4, LN_USAGE_TEXCOORD     , 5 },
            { 0, LN_DECL_FLOAT3, LN_USAGE_TEXCOORD     , 6 },
            { 0, LN_DECL_FLOAT3, LN_USAGE_TEXCOORD     , 7 },

            { 0, LN_DECL_FLOAT1, LN_USAGE_TEXCOORD     , 8 },
            { 0, LN_DECL_FLOAT1, LN_USAGE_PSIZE        , 15 },
		      LN_DECL_END() };
	    return elements;
    }
};


namespace Vertex
{

class PMDVertex
{
public:

    LVector3	Position;           ///< 座標
    LVector3	Normal;             ///< 法線
    LVector2    TexUV;                 ///< テクスチャ座標
    lnU32       Color;              ///< 頂点カラー (ライブラリの仕様に合わせるために用意。すべて 0xffffffff)
    /*lnU8*/lnFloat        BlendIndices[ 4 ];  ///< ボーンインデックス (PMD は 2 つだけだけど、とりあえず 4 バイトにまとめるため)
    lnFloat     BlendWeights[ 4 ];  ///< ボーンブレンド率 (0.0～1.0。ファイルから取得するのは [0] だけで、[2]と[3]は使わない (常に0) )
    
	/// 頂点レイアウト
    static LNVertexElemenst* elements()
    {
	    static LNVertexElemenst elements[] = {
		    { 0, LN_DECL_FLOAT3, LN_USAGE_POSITION     , 0 },
            { 0, LN_DECL_FLOAT3, LN_USAGE_NORMAL       , 0 },
            { 0, LN_DECL_FLOAT2, LN_USAGE_TEXCOORD     , 0 },
            { 0, LN_DECL_COLOR4, LN_USAGE_COLOR        , 0 },
            //{ 0, LN_DECL_COLOR4, LN_USAGE_BLENDINDICES , 0 },
			{ 0, LN_DECL_FLOAT4, LN_USAGE_BLENDINDICES , 0 },
            { 0, LN_DECL_FLOAT4, LN_USAGE_BLENDWEIGHT  , 0 },
		      LN_DECL_END() };
	    return elements;
    }
};

class BasicXFileVertex
{
public:

    LVector3	Position;           ///< 座標
    LVector3	Normal;             ///< 法線
    lnU32       Color;              ///< 頂点カラー (ライブラリの仕様に合わせるために用意。すべて 0xffffffff)
    LVector2    TexUV;              ///< テクスチャ座標
};

}

#pragma pack( pop )

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================
