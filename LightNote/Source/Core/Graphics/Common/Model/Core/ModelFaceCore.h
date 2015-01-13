//==============================================================================
// ModelFaceCore 
//------------------------------------------------------------------------------
///**
//  @file       ModelFaceCore.h
//  @brief      ModelFaceCore
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../PMDTypes.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{
enum ModelMorphType
{
    ModelMorphType_Vertex = 0,
	ModelMorphType_UV,
	ModelMorphType_AdditionalUV1,
	ModelMorphType_AdditionalUV2,
	ModelMorphType_AdditionalUV3,
	ModelMorphType_AdditionalUV4,
	ModelMorphType_Bone,
	ModelMorphType_Matrial,
	ModelMorphType_Group,
	ModelMorphType_Flip,
	ModelMorphType_Impulse,
};

//==============================================================================
// ModelMorphCore
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ModelMorphCore
	: public Base::ReferenceObject
{
public:
	struct MorphVertex
	{
		LVector3	Position;
		lnU32		TargetIndex;
		lnU32		TempBufferTargetIndex;
	};

public:
    ModelMorphCore();
    virtual ~ModelMorphCore();

public:

	///// 適用
 //   void apply( Vertex::PMDVertex* vertices ) const;

 //   /// 適用
 //   void blend( Vertex::PMDVertex* vertices, float rate ) const;

public:

    lnRefStringW    Name;				///< 表情名
    lnU32	        VerticesCount;		///< 表情頂点数
    MorphVertex*    TargetVertices;		///< 表情頂点データ


	// PMX用一時
	int				OperationPanel;		///< 操作パネル (PMD:カテゴリ) 1:眉(左下) 2:目(左上) 3:口(右上) 4:その他(右下)  | 0:システム予約
	ModelMorphType	MorphType;

	union MorphOffset
	{
		struct 
		{
			int			MorphIndex;			///< モーフIndex
			float		MorphRatio;			///< モーフ率 : グループモーフのモーフ値 * モーフ率 = 対象モーフのモーフ値
		} GroupMorphOffset;
		struct 
		{
			int			VertexIndex;		///< 頂点Index
			float		PositionOffset[3];	///< 座標オフセット量(x,y,z)
		} VertexMorphOffset;
		struct 
		{
			int			BoneIndex;			///< ボーンIndex
			float		Moving[3];			///< 移動量(x,y,z)
			float		Rotating[4];		///< 回転量-クォータニオン(x,y,z,w)
		} BoneMorphOffset;
		struct 
		{
			int			VertexIndex;		///< 頂点Index
			float		UVOffset[4];		///< UVオフセット量(x,y,z,w) ※通常UVはz,wが不要項目になるがモーフとしてのデータ値は記録しておく
		} UVMorphOffset;
		struct 
		{
			int			MaterialIndex;		///< 材質Index -> -1:全材質対象
			lnU8		OffsetCalcType;		///< オフセット演算形式 | 0:乗算, 1:加算 - 詳細は後述
			float		Diffuse[4];			///< Diffuse (R,G,B,A) - 乗算:1.0／加算:0.0 が初期値となる(同以下)
			float		Specular[3];		///< Specular (R,G,B)
			float		SpecularCoe;		///< Specular係数
			float		Ambient[3];			///< Ambient (R,G,B)
			float		EdgeColor[4];		///< エッジ色 (R,G,B,A)
			float		EdgeSize;			///< エッジサイズ
			float		TextureCoe[4];		///< テクスチャ係数 (R,G,B,A)
			float		SphereTextureCoe[4];///< スフィアテクスチャ係数 (R,G,B,A)
			float		ToonTextureCoe[4];	///< Toonテクスチャ係数 (R,G,B,A)
		} MaterialMorphOffset;
		struct 
		{
			int			MorphIndex;			///< モーフIndex
			float		MorphValue;
		} FlipMorphOffset;
		struct 
		{
			int			RigidIndex;
			lnU8		LocalFlag;
			float		Moving[3];
			float		Rotating[3];
		} ImpulseMorphOffset;
	};
	std::vector<MorphOffset>	MorphOffsets;
};

//==============================================================================
// ■ ModelFaceCore
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ModelFaceCore
	: public Base::ReferenceObject
{
public:
    ModelFaceCore();
    virtual ~ModelFaceCore();

public:

	/// 初期化
    void initializePMD( const PMD_Face* faceData, const ModelFaceCore* baseFace );

	/// 名前の取得
    const lnRefString& getName() const { return mName; }

	/// 適用
    void applyFace( Vertex::PMDVertex* vertices ) const;

	/// 適用
    void blendFace( Vertex::PMDVertex* vertices, float rate ) const;

private:

	/// 解放
    void _release();

private:

    lnRefString     mName;              ///< 表情名
    lnU32	        mVerticesCount;		///< 表情頂点数
    PMD_FaceVtx*    mFaceVertices;	    ///< 表情頂点データ
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================