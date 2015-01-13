//==============================================================================
// ModelMotion 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <stdlib.h>
#include <cmath>
#include <algorithm>
#include "ModelMotion.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Animation
{



	//----------------------------------------------------------------------
	// �� ������
	//----------------------------------------------------------------------
    void VMDBezier::initialize( float fPointX1, float fPointY1, float fPointX2, float fPointY2 )
    {
	    if ( fPointX1 == fPointY1 && fPointX2 == fPointY2 )
	    {
		    mIsLinear = true;
	    }
	    else
	    {
		    fPointX1 = (fPointX1 / 127.0f) * 3.0f;
		    fPointY1 = (fPointY1 / 127.0f) * 3.0f;

		    fPointX2 = (fPointX2 / 127.0f) * 3.0f;
		    fPointY2 = (fPointY2 / 127.0f) * 3.0f;

		    // �������̂��߃e�[�u�������Ă���
		    mYValue[0] = 0.0f;
		    mYValue[YVAL_DIV_NUM] = 1.0f;

		    float	fAddX = 1.0f / (float)YVAL_DIV_NUM;

		    for( int i = 1 ; i < YVAL_DIV_NUM ; i++ )
		    {
			    mYValue[i] = getYValue( fAddX * i, fPointX1, fPointY1, fPointX2, fPointY2 );
		    }

		    mIsLinear = false;
	    }
    }

	//----------------------------------------------------------------------
	// �� X�l�ɑΉ�����Y�l��Ԃ�
	//----------------------------------------------------------------------
    float VMDBezier::getYValue( float fX, float fX1, float fY1, float fX2, float fY2 )
    {
	    float	fT = fX;
	    float	fInvT = 1.0f - fT;

	    for( int i = 0 ; i < 32 ; i++ )
	    {
		    float	fTempX = fInvT*fInvT*fT*fX1 + fInvT*fT*fT*fX2 + fT*fT*fT;

		    fTempX -= fX;
		    if ( fabsf( fTempX ) < 0.0001f )
		    {
			    break;
		    }
		    else
		    {
			    fT -= fTempX * 0.5f;
			    fInvT = 1.0f - fT;
		    }
	    }

	    return fInvT*fInvT*fT*fY1 + fInvT*fT*fT*fY2 + fT*fT*fT;
    }

	//----------------------------------------------------------------------
	// �� �Ȑ���Ԓl��Ԃ�
	//----------------------------------------------------------------------
    float VMDBezier::getInterValue( float fX )
    {
	    if ( mIsLinear )	return fX;	// ���`���

	    // �e�[�u������`��Ԃ���
	    fX *= (float)YVAL_DIV_NUM;

	    int		iIdx = (int)fX;

	    fX -= iIdx;

	    return mYValue[iIdx] * (1.0f - fX) + mYValue[iIdx + 1] * fX;
    }

//==============================================================================
// �� ModelMotion �N���X
//==============================================================================

	/// �{�[���L�[�t���[���\�[�g�p��r�֐�
    static bool boneCompareFunc( const ModelMotion::KeyFrame& l_, const ModelMotion::KeyFrame& r_ )
    {
	    return l_.FrameNo < r_.FrameNo;
    }

	//----------------------------------------------------------------------
	// �� �R���X�g���N�^
	//----------------------------------------------------------------------
    ModelMotion::ModelMotion()
        : mType( LN_MODELANIM_LINEAR )
        , mTimeTick ( 60.0f )
    {
    }

	//----------------------------------------------------------------------
	// �� �L�[�t���[���ǉ�
	//----------------------------------------------------------------------
    void ModelMotion::addVMDKeyFrame(
        float frame_, const LVector3& pos_, const LQuaternion& rot_,
        char* interpolation_x,
	    char* interpolation_y,
	    char* interpolation_z,
	    char* interpolation_rot )
    {
        mKeyFrameList.push_back( KeyFrame() );
        KeyFrame& key = mKeyFrameList.back();

        key.FrameNo  = frame_;
        key.VMD.Position = pos_;
        key.VMD.Rotation = rot_;
        key.VMD.Rotation.getQuaternion().normalize();

        key.VMD.PosXInterBezier.initialize( interpolation_x[0], interpolation_x[4], interpolation_x[8], interpolation_x[12] );
		key.VMD.PosYInterBezier.initialize( interpolation_y[0], interpolation_y[4], interpolation_y[8], interpolation_y[12] );
		key.VMD.PosZInterBezier.initialize( interpolation_z[0], interpolation_z[4], interpolation_z[8], interpolation_z[12] );
		key.VMD.RotInterBezier.initialize( interpolation_rot[0], interpolation_rot[4], interpolation_rot[8], interpolation_rot[12] );
    }

	//----------------------------------------------------------------------
	// �� �L�[�t���[���̃\�[�g
	//----------------------------------------------------------------------
    void ModelMotion::sortKeyFrame()
    {
        std::sort( mKeyFrameList.begin(), mKeyFrameList.end(), boneCompareFunc );
    }

	/// ����t���[���ʒu���O�̃L�[�t���[�����������邽�߂̊֐��I�u�W�F�N�g
    class GreaterEqual
    {
        double frame;
    public:
        GreaterEqual( double frame_ ) : frame( frame_ ){}
        bool operator()( const ModelMotion::KeyFrame& key_ ) const
        {
            return frame <= key_.FrameNo;
        }
    };

	//----------------------------------------------------------------------
	// �� ���Ԃ̐ݒ�
	//----------------------------------------------------------------------
    void ModelMotion::setTime( double time_ )
    {
        double frame = time_ * mTimeTick;

        if ( mType == LN_MODELANIM_VMD )
        {
            // �t���[���� 1 ��
            if ( mKeyFrameList.size() == 1 )
            {
                mTransform.Rotation    = mKeyFrameList.front().VMD.Rotation.getQuaternion();
                mTransform.Translation = mKeyFrameList.front().VMD.Position.getVector3();
                return;
            }

            // �ŏ��̃t���[���ȑO
            if ( frame <= mKeyFrameList.front().FrameNo )
            {
                mTransform.Rotation    = mKeyFrameList.front().VMD.Rotation.getQuaternion();
                mTransform.Translation = mKeyFrameList.front().VMD.Position.getVector3();
                return;
            }

            // �Ō�̃t���[���ȍ~
            if ( frame >= mKeyFrameList.back().FrameNo )
            {
                mTransform.Rotation    = mKeyFrameList.back().VMD.Rotation.getQuaternion();
                mTransform.Translation = mKeyFrameList.back().VMD.Position.getVector3();
                return;
            }

            // �L�[����
            KeyFrameList::iterator itr = std::find_if( mKeyFrameList.begin(), mKeyFrameList.end(), GreaterEqual( frame ) );
            //if ( itr == mKeyFrameList.end() ) // ���肦�Ȃ��͂�
            //{
            //    mTransform = LSQTTransform::IDENTITY;
            //}

            //lnU32   k0idx = itr - mKeyFrameList.begin();
            //lnU32   k1idx = k0idx + 1;

            lnU32   k1idx = itr - mKeyFrameList.begin();
            lnU32   k0idx = k1idx - 1;
            

            KeyFrame& k0 = mKeyFrameList[ k0idx ];
            KeyFrame& k1 = mKeyFrameList[ k1idx ];

            float	t0 = k0.FrameNo;
	        float	t1 = k1.FrameNo;

            float rate = static_cast< float >( ( frame - t0 ) / ( t1 - t0 ) );
            float inter;


            inter = k1.VMD.PosXInterBezier.getInterValue( rate );
            mTransform.Translation.x = k0.VMD.Position.x * (1.0f - inter) + k1.VMD.Position.x * inter;

            inter = k1.VMD.PosYInterBezier.getInterValue( rate );
            mTransform.Translation.y = k0.VMD.Position.y * (1.0f - inter) + k1.VMD.Position.y * inter;

            inter = k1.VMD.PosZInterBezier.getInterValue( rate );
            mTransform.Translation.z = k0.VMD.Position.z * (1.0f - inter) + k1.VMD.Position.z * inter;

            inter = k1.VMD.RotInterBezier.getInterValue( rate );
            LQuaternion::slerp( &mTransform.Rotation, k0.VMD.Rotation.getQuaternion(), k1.VMD.Rotation.getQuaternion(), inter );

        }
        else
        {

        }
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Animation
} // namespace Core

} // namespace LNote

//==============================================================================
//
//==============================================================================