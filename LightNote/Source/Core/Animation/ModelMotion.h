//==============================================================================
// ModelMotion 
//------------------------------------------------------------------------------
///**
//  @file       ModelMotion.h
//  @brief      ModelMotion
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include "../Math/LMath.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{

enum LNModelAnimationType
{
    LN_MODELANIM_LINEAR = 0,
    LN_MODELANIM_VMD,
};

namespace Animation
{

//==============================================================================
// �� VMDBezier �N���X
//==============================================================================
class VMDBezier
{
public:

	void initialize( float pt_x1_, float pt_y1_, float pt_x2_, float pt_y2_ );

	/// t = 0.0�`1.0
	float getInterValue( float t_ );

private:

	float getYValue( float x_, float x1_, float y1_, float x2_, float y2_ );

private:

	static const int	YVAL_DIV_NUM = 16;
	float	            mYValue[ YVAL_DIV_NUM + 1 ];
	bool	            mIsLinear;
};

// �� union ���ł̓R�s�[�R���X�g���N�^�����N���X�͎g���Ȃ��̂ŋً}�p
#pragma pack( push, 1 )

struct KeyVector3
{
    lnFloat x, y, z;

    KeyVector3& operator = ( const LVector3& v_ )
    {
        x = v_.X; y = v_.Y; z = v_.Z;
        return *this;
    }

    LVector3& getVector3() { return (LVector3&)*this; }
    LVector3* getVector3Ptr() { return (LVector3*)this; }
};

/*
struct KeyVector4
{
    lnFloat x, y, z, w;

    KeyVector4& operator = ( const LVector4& v_ )
    {
        x = v_.x; y = v_.y; z = v_.z; w = v_.w;
    }

    LVector4& getVector4() { return (LVector4&)*this; }
    LVector4* getVector4Ptr() { return (LVector4*)this; }
};
*/

struct KeyQuaternion
{
    lnFloat x, y, z, w;

    KeyQuaternion& operator = ( const LQuaternion& v_ )
    {
        x = v_.X; y = v_.Y; z = v_.Z; w = v_.W;
        return *this;
    }

    LQuaternion& getQuaternion() { return (LQuaternion&)*this; }
};

#pragma pack( pop )

//==============================================================================
// �� ModelMotion �N���X
//==============================================================================
class ModelMotion
{
public:

	/// �L�[�t���[���f�[�^
    struct KeyFrame
    {
	    float	    FrameNo;		// �t���[���ԍ�

        union
        {
            /// ���`��� (X �t�@�C���p)
            struct
            {
                KeyVector3	    Position;	    ///< �ʒu
	            KeyQuaternion	Rotation;	    ///< ��]
                KeyVector3      Scale;          ///< �g��
            } Linear;

            /// VMD �p
            struct
            {
	            KeyVector3	    Position;	    // �ʒu
	            KeyQuaternion	Rotation;	    // ��](�N�H�[�^�j�I��)

	            VMDBezier	    PosXInterBezier;	// X���ړ����
	            VMDBezier	    PosYInterBezier;	// Y���ړ����
	            VMDBezier	    PosZInterBezier;	// Z���ړ����
	            VMDBezier	    RotInterBezier;	    // ��]���
            } VMD;
        };
    };

public:

    ModelMotion();

public:

	/// �L�[�t���[���^�C�v�̐ݒ�
    void setAnimType( LNModelAnimationType type_ ) { mType = type_; }

	/// �L�[�t���[���ǉ� (VMD �p)
    void addVMDKeyFrame(
        float frame_, const LVector3& pos_, const LQuaternion& rot_,
        char* interpolation_x,
	    char* interpolation_y,
	    char* interpolation_z,
	    char* interpolation_rot );

	/// �L�[�t���[���̃\�[�g
    void sortKeyFrame();

	/// ���Ԃ̃X�P�[���W���̐ݒ� (��{�I�Ɍ��݂̃t���[�����[�g��n������)
    void setTimeTickPerSec( lnFloat tick_ ) { mTimeTick = tick_; }

	/// ���Ԃ̐ݒ� (tick_ �ɂ�
    void setTime( double time_ );

	/// �l�̎擾 (setTime() ��ɌĂԂ���)
    const LSQTTransform& getTransform() const { return mTransform; }

private:

    typedef std::vector< KeyFrame >     KeyFrameList;

private:

    LNModelAnimationType    mType;
    KeyFrameList            mKeyFrameList;
    LSQTTransform           mTransform;
    lnFloat                 mTimeTick;

};



//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Animation
} // namespace Core

} // namespace LNote

//==============================================================================
//
//==============================================================================