//==============================================================================
// FixedLengthScalar 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <stdlib.h>
#include <cmath>
#include "FixedLengthScalar.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Animation
{

//==============================================================================
// �� FixedLengthScalar �N���X
//==============================================================================

    FixedLengthScalar::Key::Key()
        : FramePos      ( 0.0f )
        , Value         ( 0.0f )
        , Velocity      ( 0.0f )
        , Accel         ( 0.0f )
        , LeftSlope     ( 0.0f )
        , RightSlope    ( 0.0f )
        , InterMode     ( LN_ANIM_INTER_LINEAR )
    {}

	//----------------------------------------------------------------------
	// �� �f�t�H���g�R���X�g���N�^
	//----------------------------------------------------------------------
    FixedLengthScalar::FixedLengthScalar()
        : mCapacity         ( 0 )
        , mKeyFrames        ( NULL )
        , mSize             ( 0 )
        , mTime             ( 0.0 )
        , mTimeTick         ( 60.0f )
        , mCurrentFramePos  ( 0.0f )
        , mLoopBeginPos     ( 0.0f )
        , mLoopEndPos       ( 0.0f )
        , mDefaultValue     ( 0.0f )
        , mValue            ( 0.0f )
        , mEnableLoop       ( false )
    {
    }

	//----------------------------------------------------------------------
	// �� �R���X�g���N�^
	//----------------------------------------------------------------------
    FixedLengthScalar::FixedLengthScalar( int capacity_ )
        : mCapacity         ( 0 )
        , mKeyFrames        ( NULL )
        , mSize             ( 0 )
        , mTimeTick         ( 60.0f )
        , mCurrentFramePos  ( 0.0f )
        , mLoopBeginPos     ( 0.0f )
        , mLoopEndPos       ( 0.0f )
        , mDefaultValue     ( 0.0f )
        , mValue            ( 0.0f )
        , mEnableLoop       ( false )
    {
        setCapacity( capacity_ );
    }

	//----------------------------------------------------------------------
	// �� �f�X�g���N�^
	//----------------------------------------------------------------------
    FixedLengthScalar::~FixedLengthScalar()
    {
        SAFE_DELETE_ARRAY( mKeyFrames );
    }

	//----------------------------------------------------------------------
	// �� �L�[�̍ő吔�̐ݒ� (���݂̃L�[�͑S�Ĕj�������)
	//----------------------------------------------------------------------
    void FixedLengthScalar::setCapacity( int capacity_ )
    {
        clear();
        SAFE_DELETE_ARRAY( mKeyFrames );
        mKeyFrames = LN_NEW Key[ capacity_ ];
        mCapacity = capacity_;
    }

	//----------------------------------------------------------------------
	// �� �L�[���ЂƂ��Ȃ��Ƃ��ɕԂ��l�̐ݒ�
	//----------------------------------------------------------------------
    void FixedLengthScalar::setDefaultValue( lnFloat value_ )
    {
        mDefaultValue = value_;
    }

	//----------------------------------------------------------------------
	// �� �L�[�̒ǉ�
	//----------------------------------------------------------------------
    bool FixedLengthScalar::addKeyFrame( lnFloat frame_pos_, lnFloat value_ )
    {
        if ( mSize >= mCapacity ) { return false; }

        mKeyFrames[ mSize ].FramePos = frame_pos_;
        mKeyFrames[ mSize ].Value = value_;
        ++mSize;

        return true;
    }

	//----------------------------------------------------------------------
	// �� ���Ԃ̐ݒ�
	//----------------------------------------------------------------------
    void FixedLengthScalar::setTime( double time_ )
    {
        mTime = time_;


        time_ *= mTimeTick;

        
        if ( mEnableLoop )
        {
            // ���[�v�ʒu���ݒ�
            if ( mLoopEndPos == 0.0f && mLoopBeginPos == 0.0f )
            {
                if ( mKeyFrames[ mSize - 1 ].FramePos < time_ )
                {
                    time_ = fmod( time_, static_cast< double >( mKeyFrames[ mSize - 1 ].FramePos ) );
                }
            }
            // ���[�v�ʒu�ݒ�ς�
            else 
            {
                // time_ �����[�v�̈�𒴂��Ă����ꍇ�͊J�n���Ԃ���
                if ( mLoopEndPos < time_ )
                {
                    time_ -= mLoopBeginPos;
                    time_ = fmod( time_, ( mLoopEndPos - mLoopBeginPos ) );
                    time_ = mLoopBeginPos + time_;
                }
            }
        }

        mCurrentFramePos = time_;

        Key* key;

        if ( mSize )
        {
            // time_ ���ŏ��̃t���[���ʒu���O�̏ꍇ�̓f�t�H���g�l
            if ( time_ < mKeyFrames[ 0 ].FramePos )
            {
                mValue = mDefaultValue;
            }
            // �L�[���ЂƂ����̏ꍇ�͂��̒l
            else if ( mSize == 1 )
            {
                mValue = mKeyFrames[ 0 ].Value;
            }
            // time_ ���I�[�ȍ~�̏ꍇ�͏I�[�̒l ( _cmpKey() �ŏI�[�̂ЂƂ��� NULL �Q�Ƃ���΍� )
            else if ( time_ >= mKeyFrames[ mSize - 1 ].FramePos )
            {
                mValue = mKeyFrames[ mSize - 1 ].Value;
            }
            // �ȏ�ȊO�̏ꍇ�͕�Ԃ���
            else
            {
                // time_ �̌^�� Key::FramePos �̌^�͓�����̂ɂ��Ă����Ȃ��Ɛ���Ɍ����ł��Ȃ�
                key = (Key*)bsearch( &time_, mKeyFrames, mSize, sizeof( Key ), _cmpKey );

                lnFloat p0 = key->Value;
		        lnFloat p1 = ( key + 1 )->Value;
                lnFloat t0 = static_cast< lnFloat >( key->FramePos );
		        lnFloat t1 = static_cast< lnFloat >( ( key + 1 )->FramePos );
                lnFloat t  = static_cast< lnFloat >( time_ - t0 ) / ( t1 - t0 );

                switch ( key->InterMode )
		        {
			        // ��Ԗ���
			        case LN_ANIM_INTER_NONE:
                    {
				        mValue = p0;
                        break;
                    }
			        // ���`
			        case LN_ANIM_INTER_LINEAR:
                    {
				        mValue = p0 + ( p1 - p0 ) * t;
                        break;
                    }
			        // �������x
			        case LN_ANIM_INTER_ACCEL:
			        {
                        mValue = LMath::QuadAccel( p0, key->Velocity, key->Accel, static_cast< lnFloat >( mCurrentFramePos - key->FramePos )  );
                        break;
			        }
			        // ���x�J�[�u
			        case LN_ANIM_INTER_CUBIC:
			        {
				        mValue = LMath::Hermite(
					        p0, p1, 
                            ( key + 1 )->RightSlope,
                            ( key + 1 )->LeftSlope,
					        t );
                        break;
			        }
			        // Catmull-Rom
			        case LN_ANIM_INTER_CATMULLROM:
			        {
                        // ���[�v�Đ��� time ���I�[�𒴂��Ă���ꍇ�A
                        // ���̎��_��key �̒l�� ���[�v�J�n�ʒu�̂ЂƂO�̃L�[���w���Ă���

                        Key* begin = mKeyFrames;
                        Key* end   = &mKeyFrames[ mSize - 1 ];
                        
				        // ���̕�Ԃɂ́Abegin �̂ЂƂO�� end �̂ЂƂ�̒l���K�v�B
				        // ���ꂼ�ꂪ�n�_�A�I�_�̏ꍇ�̓��[�v����悤�ɕ�Ԃ���
				        mValue = LMath::CatmullRom(
                            ( ( key == begin ) ? end->Value : ( key - 1 )->Value ),
					        p0,
					        p1,
					        ( ( ( key + 1 ) == end ) ? begin->Value : ( key + 2 )->Value ),
					        t );
                        break;
			        }
		        }
            }
        }
        else
        {
            mValue = mDefaultValue;
        }
    }

	//----------------------------------------------------------------------
	// �� ���Ԃ�i�߂�
	//----------------------------------------------------------------------
    void FixedLengthScalar::advanceTime( lnFloat delta_time_ )
    {
        setTime( mTime + static_cast< double >( delta_time_ ) );
    }

	//----------------------------------------------------------------------
	// �� ���[�v�Đ��̗L���ݒ�
	//----------------------------------------------------------------------
    void FixedLengthScalar::setEnableLoop( bool flag_ )
    {
        mEnableLoop = flag_;
    }

	//----------------------------------------------------------------------
	// �� ���[�v�̈�̐ݒ�
	//----------------------------------------------------------------------
    void FixedLengthScalar::setLoopRange( lnFloat begin_frame_, lnFloat frame_length_ )
    {
        mLoopBeginPos = begin_frame_;
        mLoopEndPos   = ( begin_frame_ + frame_length_ );
    }

	//----------------------------------------------------------------------
	// �� ���[�v���Ԃ̐ݒ�
	//----------------------------------------------------------------------
    void FixedLengthScalar::setLoopFrame( lnFloat begin_frame_, lnFloat end_frame_ )
    {
        mLoopBeginPos = begin_frame_;
        mLoopEndPos   = end_frame_;
    }

	//----------------------------------------------------------------------
	// �� �Đ����I�����Ă��邩�𔻒肷�� (���[�v�Đ����͏�� false)
	//----------------------------------------------------------------------
    bool FixedLengthScalar::isFinished() const
    {
        
        if ( mEnableLoop ) { return false; }
        if ( mSize == 0 ) { return true; }
        
        if ( mCurrentFramePos < mKeyFrames[ mSize - 1 ].FramePos ) { return false; }

        return true;
    }

	//----------------------------------------------------------------------
	// �� �L�[��S�č폜����
	//----------------------------------------------------------------------
    void FixedLengthScalar::clear()
    {
        mSize = 0;
    }

	//----------------------------------------------------------------------
	// �� �L�[�ɃA�N�Z�X����
	//----------------------------------------------------------------------
    FixedLengthScalar::Key& FixedLengthScalar::operator [] ( int idx_ )
    {
        if ( idx_ >= mSize )
        {
            // �x���Ƃ�
            *reinterpret_cast< int* >( 0 ) = 0;
        }
        return mKeyFrames[ idx_ ];
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    const FixedLengthScalar::Key& FixedLengthScalar::operator [] ( int idx ) const
    {
        return mKeyFrames[idx];
    }

	//----------------------------------------------------------------------
	// �� ��r�֐�
	//----------------------------------------------------------------------
    int FixedLengthScalar::_cmpKey( const void* a_, const void* b_ )
    {
        if ( ((Key*)a_)->FramePos < ((Key*)b_)->FramePos )
            return -1;
        else if ( ((Key*)a_)->FramePos >= ((Key*)b_)->FramePos && ((Key*)a_)->FramePos < (((Key*)b_+1))->FramePos )
            return 0;
        else
            return 1;
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