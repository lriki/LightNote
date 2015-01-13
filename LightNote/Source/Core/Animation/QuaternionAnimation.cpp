//==============================================================================
// QuaternionAnimation 
//==============================================================================

#include "stdafx.h"
#include <algorithm>
#include <Dependencies/cpp-TimSort/timsort.hpp>
#include "../Math/Math.h"
#include "QuaternionAnimation.h"

namespace LNote
{
namespace Core
{
namespace Animation
{

//==============================================================================
// ■ BezierAnimation
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    BezierAnimation::BezierAnimation()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    BezierAnimation::~BezierAnimation()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void BezierAnimation::addKeyFrame( float framePos, const LQuaternion& rot, char* interpolation )
    {
        mKeyFrameArray.push_back( KeyFrame() );
        KeyFrame& key = mKeyFrameArray.back();

        key.FramePos = framePos;
        key.Rotation = rot;
        key.Rotation.normalize();
		key.RotInterBezier.initialize( interpolation[0], interpolation[4], interpolation[8], interpolation[12] );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void BezierAnimation::sortKeyFrame()
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
    void BezierAnimation::setTime( double time )
    {
		AnimationTimelineBase::setTime( time );

        // フレーム数 1 個
        if ( this->mKeyFrameArray.size() == 1 )
        {
            this->mQuaternion = this->mKeyFrameArray.front().Rotation;
            return;
        }

        // 最初のフレーム以前
        if ( mCurrentFramePos <= this->mKeyFrameArray.front().FramePos )
        {
            this->mQuaternion = this->mKeyFrameArray.front().Rotation;
            return;
        }

        // 最後のフレーム以降
        if ( mCurrentFramePos >= this->mKeyFrameArray.back().FramePos )
        {
            this->mQuaternion = this->mKeyFrameArray.back().Rotation;
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

        inter = k1.RotInterBezier.getInterValue( rate );
        LQuaternion::slerp( &mQuaternion, k0.Rotation, k1.Rotation, inter );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	double BezierAnimation::getLastFramePos()
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