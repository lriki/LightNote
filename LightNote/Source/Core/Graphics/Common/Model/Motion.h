//==============================================================================
// Motion 
//------------------------------------------------------------------------------
///**
//  @file       Motion.h
//  @brief      Motion
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <vector>
#include <map>
#include "../../../Base/RefString.h"
#include "../../../Math/LMath.h"
#include "../../../Animation/ModelMotion.h"
#include "../../../Animation/Scalar.h"
#include "../../../Animation/SQTTransformAnimation.h"
#include "../../../Animation/ScalarAnimation.h"
#include "PMDTypes.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ VMDLoader
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class VMDLoader
{
public:
	struct BoneAnimation
	{
		lnRefString TargetBoneName;
		Animation::BezierSQTTransformAnimation* Animation;
	};
	
	struct FaceAnimation
	{
		lnRefString TargetFaceName;
		Animation::ScalarAnimation* Animation;
	};

	typedef std::vector<BoneAnimation>	BoneAnimationArray;
	typedef std::vector<FaceAnimation>	FaceAnimationArray;

public:
	~VMDLoader();

public:

	/// 読み込み
	bool load( const void* data, lnU32 size );

	double getLastFramePosition() const { return mLastFramePosition; }
	BoneAnimationArray& getBoneAnimationArray() { return mBoneAnimationArray; }
	FaceAnimationArray& getFaceAnimationArray() { return mFaceAnimationArray; }

private:
    typedef std::map<LRefTString, lnU32>	BoneAnimationIndexMap;
    typedef std::pair<LRefTString, lnU32>	BoneAnimationIndexPair;

    typedef std::map<LRefTString, lnU32>	FaceAnimationIndexMap;
    typedef std::pair<LRefTString, lnU32>	FaceAnimationIndexPair;
	
	double					mLastFramePosition;

	BoneAnimationArray		mBoneAnimationArray;
	BoneAnimationIndexMap	mBoneAnimationIndexMap;
	
    FaceAnimationArray		mFaceAnimationArray;
	FaceAnimationIndexMap   mFaceAnimationIndexMap;
};



// ▼削除予定

















typedef Animation::ModelMotion  MotionAnimation;
typedef Animation::Scalar       FaceAnimation;

//==============================================================================
// ■ Motion クラス
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class Motion
    : public Base::ReferenceObject
{
public:
    Motion();
    LN_REFOBJ_METHODS;

protected:
    virtual ~Motion();

public:

	/// 初期化
    LNRESULT initializeVMD( const void* data_, lnU32 size_ );

	/// ボーン名に対応するアニメーションを返す
    MotionAnimation* findBoneAnimation( const lnChar* name_ );

	/// 表情名に対応するアニメーションを返す
    FaceAnimation* findFaceAnimation( const lnChar* name_ );

	/// ボーン名に対応するアニメーションの番号を返す (使わないかも)
    lnU32 findBoneAnimationIndex( const lnChar* name_ );

	/// 表情名に対応するアニメーションの番号を返す
    lnU32 findFaceAnimationIndex( const lnChar* name_ );

private:

    

    typedef std::vector< MotionAnimation* >     MotionAnimationList;
    typedef std::map< LRefTString, lnU32 >      AnimationIndexMap;
    typedef std::pair< LRefTString, lnU32 >     AnimationIndexPair;

    typedef std::vector< FaceAnimation* >       FaceAnimationList;
    typedef std::map< LRefTString, lnU32 >      FaceAnimationIndexMap;
    typedef std::pair< LRefTString, lnU32 >     FaceAnimationIndexPair;

private:

    double                  mMaxFrames;         ///< 最大フレーム数
    AnimationIndexMap       mAnimationIndexMap;
    MotionAnimationList     mMotionAnimationList;

    FaceAnimationIndexMap   mFaceAnimationIndexMap;
    FaceAnimationList       mFaceAnimationList;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core

} // namespace LNote

//==============================================================================
//
//==============================================================================