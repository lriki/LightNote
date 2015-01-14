//==============================================================================
// BezierSQTTransformAnimation 
//==============================================================================

#include "stdafx.h"
#include <algorithm>
#include <Dependencies/cpp-TimSort/timsort.hpp>
#include "../Math/LMath.h"
#include "SQTTransformAnimation.h"

namespace LNote
{
namespace Core
{
namespace Animation
{

//==============================================================================
// ■ BezierSQTTransformAnimation
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    BezierSQTTransformAnimation::BezierSQTTransformAnimation()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    BezierSQTTransformAnimation::~BezierSQTTransformAnimation()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void BezierSQTTransformAnimation::addKeyFrame(
        float framePos, const LVector3& pos, const LQuaternion& rot,
        char* interpolation_x,
	    char* interpolation_y,
	    char* interpolation_z,
	    char* interpolation_rot )
    {
        mKeyFrameArray.push_back( KeyFrame() );
        KeyFrame& key = mKeyFrameArray.back();

        key.FramePos = framePos;
        key.Position = pos;
        key.Rotation = rot;
        key.Rotation.Normalize();

		key.PosXInterBezier.initialize( interpolation_x[0], interpolation_x[4], interpolation_x[8], interpolation_x[12] );
		key.PosYInterBezier.initialize( interpolation_y[0], interpolation_y[4], interpolation_y[8], interpolation_y[12] );
		key.PosZInterBezier.initialize( interpolation_z[0], interpolation_z[4], interpolation_z[8], interpolation_z[12] );
		key.RotInterBezier.initialize( interpolation_rot[0], interpolation_rot[4], interpolation_rot[8], interpolation_rot[12] );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void BezierSQTTransformAnimation::sortKeyFrame()
    {
		struct 
		{
			bool operator()( const KeyFrame& l, const KeyFrame& r ) const
			{
				return l.FramePos < r.FramePos;
			}
		} compare;
		
		gfx::timsort( mKeyFrameArray.begin(), mKeyFrameArray.end(), compare );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void BezierSQTTransformAnimation::setTime( double time )
    {
		AnimationTimelineBase::setTime( time );

        // フレーム数 1 個
        if ( this->mKeyFrameArray.size() == 1 )
        {
			this->mTransform.Rotation = this->mKeyFrameArray.front().Rotation;
            this->mTransform.Translation = this->mKeyFrameArray.front().Position;
            return;
        }

        // 最初のフレーム以前
        if ( mCurrentFramePos <= this->mKeyFrameArray.front().FramePos )
        {
			this->mTransform.Rotation = this->mKeyFrameArray.front().Rotation;
            this->mTransform.Translation = this->mKeyFrameArray.front().Position;
            return;
        }

        // 最後のフレーム以降
        if ( mCurrentFramePos >= this->mKeyFrameArray.back().FramePos )
        {
			this->mTransform.Rotation = this->mKeyFrameArray.back().Rotation;
            this->mTransform.Translation = this->mKeyFrameArray.back().Position;
            return;
        }

		// あるフレーム位置直前のキーフレームを検索するための関数オブジェクト
		struct GreaterEqual
		{
			double FramePos;
			bool operator()( const KeyFrame& key ) const
			{
				return FramePos <= key.FramePos;
			}
		} compare;
		compare.FramePos = mCurrentFramePos;

        // キー検索
        KeyFrameArray::iterator itr = std::find_if( 
			this->mKeyFrameArray.begin(), this->mKeyFrameArray.end(), compare );
        //if ( itr == mKeyFrameList.end() ) // ありえないはず
        //{
        //    mTransform = LSQTTransform::IDENTITY;
        //}

        //lnU32   k0idx = itr - mKeyFrameList.begin();
        //lnU32   k1idx = k0idx + 1;

        lnU32   k1idx = itr - this->mKeyFrameArray.begin();
        lnU32   k0idx = k1idx - 1;
            

        KeyFrame& k0 = this->mKeyFrameArray[ k0idx ];
        KeyFrame& k1 = this->mKeyFrameArray[ k1idx ];

        float	t0 = k0.FramePos;
	    float	t1 = k1.FramePos;

        float rate = static_cast< float >( ( mCurrentFramePos - t0 ) / ( t1 - t0 ) );
        float inter;

		inter = k1.PosXInterBezier.getInterValue( rate );
        this->mTransform.Translation.X = k0.Position.X * (1.0f - inter) + k1.Position.X * inter;

        inter = k1.PosYInterBezier.getInterValue( rate );
        this->mTransform.Translation.Y = k0.Position.Y * (1.0f - inter) + k1.Position.Y * inter;

        inter = k1.PosZInterBezier.getInterValue( rate );
        this->mTransform.Translation.Z = k0.Position.Z * (1.0f - inter) + k1.Position.Z * inter;

        inter = k1.RotInterBezier.getInterValue( rate );
		this->mTransform.Rotation = LQuaternion::Slerp(k0.Rotation, k1.Rotation, inter);
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	double BezierSQTTransformAnimation::getLastFramePos()
	{
		if ( mKeyFrameArray.empty() ) return 0;
		return mKeyFrameArray.back().FramePos;
	}

} // namespace Animation
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================