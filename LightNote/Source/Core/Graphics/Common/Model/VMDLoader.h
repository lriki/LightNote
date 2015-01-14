//==============================================================================
// VMDLoader 
//------------------------------------------------------------------------------
///**
//  @file       VMDLoader.h
//  @brief      VMDLoader
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
// VMDLoader
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class VMDLoader2
{
public:
	struct BoneAnimation
	{
		lnRefStringW TargetBoneName;
		Animation::BezierSQTTransformAnimation* Animation;
	};
	
	struct FaceAnimation
	{
		lnRefStringW TargetFaceName;
		Animation::ScalarAnimation* Animation;
	};

	typedef std::vector<BoneAnimation>	BoneAnimationArray;
	typedef std::vector<FaceAnimation>	FaceAnimationArray;

public:
	VMDLoader2() {}
	~VMDLoader2();

public:

	/// 読み込み
	bool load( FileIO::Stream* stream );

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

} // namespace Graphics
} // namespace Core
} // namespace LNote
