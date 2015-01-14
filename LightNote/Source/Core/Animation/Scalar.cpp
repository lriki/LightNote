//==============================================================================
// FixedLengthScalar 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <cmath>
#include <algorithm>
#include "Scalar.h"

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
// �� Scalar �N���X
//==============================================================================

    Scalar::Key::Key()
        : FramePos      ( 0.0f )
        , Value         ( 0.0f )
        , Velocity      ( 0.0f )
        , Accel         ( 0.0f )
        , LeftSlope     ( 0.0f )
        , RightSlope    ( 0.0f )
        , InterMode     ( LN_ANIM_INTER_LINEAR )
    {
    }

	/// �L�[�t���[���\�[�g�p��r�֐�
    static bool scalarCompareFunc( const Scalar::Key& l_, const Scalar::Key& r_ )
    {
	    return l_.FramePos < r_.FramePos;
    }

	/// ����t���[���ʒu���O�̃L�[�t���[�����������邽�߂̊֐��I�u�W�F�N�g
    class scalarGreaterEqual
    {
        double frame;
    public:
        scalarGreaterEqual( double frame_ ) : frame( frame_ ){}
        bool operator()( const Scalar::Key& key_ ) const
        {
            return key_.FramePos <= frame;
        }
    };

	//----------------------------------------------------------------------
	// �� �f�t�H���g�R���X�g���N�^
	//----------------------------------------------------------------------
    Scalar::Scalar()
        : mTime             ( 0.0 )
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
	// �� �L�[�̒ǉ�
	//----------------------------------------------------------------------
    void Scalar::addKeyFrame( double frame_pos_, lnFloat value_ )
    {
        Key key;
        key.FramePos = frame_pos_;
        key.Value    = value_;
        mKeyFrameList.push_back( key );
    }

	//----------------------------------------------------------------------
	// �� �L�[�̒ǉ�
	//----------------------------------------------------------------------
    void Scalar::addKeyFrame( const Key& key_ )
    {
        mKeyFrameList.push_back( key_ );
    }

	//----------------------------------------------------------------------
	// �� �L�[�t���[���̃\�[�g
	//----------------------------------------------------------------------
    void Scalar::sortKeyFrame()
    {
        std::sort( mKeyFrameList.begin(), mKeyFrameList.end(), scalarCompareFunc );
    }

	//----------------------------------------------------------------------
	// �� ���[�v�Đ��̗L���ݒ�
	//----------------------------------------------------------------------
    void Scalar::setEnableLoop( bool flag_ )
    {
        mEnableLoop = flag_;
    }

	//----------------------------------------------------------------------
	// �� ���[�v���Ԃ̐ݒ�
	//----------------------------------------------------------------------
    void Scalar::setLoopFrame( lnFloat begin_frame_, lnFloat end_frame_ )
    {
        mLoopBeginPos = begin_frame_;
        mLoopEndPos   = end_frame_;
    }

    

	//----------------------------------------------------------------------
	// �� ���Ԃ̐ݒ�
	//----------------------------------------------------------------------
    void Scalar::setTime( double time_ )
    {
        mTime = time_;

        time_ *= mTimeTick;

        
        if ( mEnableLoop )
        {
            // ���[�v�ʒu���ݒ�
            if ( mLoopEndPos == 0.0f && mLoopBeginPos == 0.0f )
            {
                // �I�[�̃L�[�t���[���̎��Ԃ��g���ă��[�v
                if ( mKeyFrameList.back().FramePos < time_ )
                {
                    time_ = fmod( time_, static_cast< double >( mKeyFrameList.back().FramePos ) );
                }
            }
            // ���[�v�ʒu�ݒ�ς�
            else 
            {
                // time_ �����[�v�̈�𒴂��Ă����ꍇ�͊J�n���Ԃ��烋�[�v
                if ( mLoopEndPos < time_ )
                {
                    time_ -= mLoopBeginPos;
                    time_ = fmod( time_, ( mLoopEndPos - mLoopBeginPos ) );
                    time_ = mLoopBeginPos + time_;
                }
            }
        }

        mCurrentFramePos = time_;

        if ( getKeyNum() )
        {
            // time_ ���ŏ��̃t���[���ʒu���O�̏ꍇ�̓f�t�H���g�l
            if ( time_ < mKeyFrameList.front().FramePos )
            {
                mValue = mDefaultValue;
            }
            // �L�[���ЂƂ����̏ꍇ�͂��̃L�[�̒l
            else if ( getKeyNum() == 1 )
            {
                mValue = mKeyFrameList.front().Value;
            }
            // time_ ���I�[�ȍ~�̏ꍇ�͏I�[�̒l
            else if ( time_ >= mKeyFrameList.back().FramePos )
            {
                mValue = mKeyFrameList.back().Value;
            }
            // �ȏ�ȊO�̏ꍇ�͕�Ԃ���
            else
            {
                // time_ �̌^�� Key::FramePos �̌^�͓����̂ɂ��Ă����Ȃ��Ɛ���Ɍ����ł��Ȃ�
                //key = (Key*)bsearch( &time_, mKeyFrames, mSize, sizeof( Key ), _cmpKey );

                // �L�[����
                /*
                KeyFrameList::iterator itr = std::find_if( mKeyFrameList.begin(), mKeyFrameList.end(), scalarGreaterEqual( mCurrentFramePos ) );
                const Key& key0 = *itr;
                const Key& key1 = *(itr + 1);
                */

                const Key* key0 = (Key*)bsearch( &mCurrentFramePos, &(mKeyFrameList[0]), mKeyFrameList.size(), sizeof( Key ), _cmpKey );
                const Key* key1 = key0 + 1;

                lnFloat p0 = key0->Value;
		        lnFloat p1 = key1->Value;
                lnFloat t0 = static_cast< lnFloat >( key0->FramePos );
		        lnFloat t1 = static_cast< lnFloat >( key1->FramePos );
                lnFloat t  = static_cast< lnFloat >( mCurrentFramePos - t0 ) / ( t1 - t0 );

                switch ( key0->InterMode )
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
                        mValue = LMath::QuadAccel( p0, key0->Velocity, key0->Accel, static_cast< lnFloat >( mCurrentFramePos - key0->FramePos )  );
                        break;
			        }
			        // �O�����
			        case LN_ANIM_INTER_CUBIC:
			        {
				        mValue = LMath::Hermite(
					        p0, p1, 
                            key0->RightSlope,
                            key1->LeftSlope,
					        t );
                        break;
			        }
			        // Catmull-Rom
			        case LN_ANIM_INTER_CATMULLROM:
			        {
                        // ���[�v�Đ��� time ���I�[�𒴂��Ă���ꍇ�A
                        // ���̎��_��key �̒l�� ���[�v�J�n�ʒu�̂ЂƂO�̃L�[���w���Ă���

                        const Key& begin = mKeyFrameList.front();
                        const Key& end   = mKeyFrameList.back();
                        
				        // ���̕�Ԃɂ́Abegin �̂ЂƂO�� end �̂ЂƂ�̒l���K�v�B
				        // ���ꂼ�ꂪ�n�_�A�I�_�̏ꍇ�̓��[�v����悤�ɕ�Ԃ���
				        mValue = LMath::CatmullRom(
                            ( ( key0->FramePos == begin.FramePos ) ? end.Value : (key0 - 1)->Value ),
					        p0,
					        p1,
					        ( ( key1->FramePos == end.FramePos ) ? begin.Value : (key0 + 2)->Value ),
					        t );
                        break;
			        }
		        }

#if 0
                lnFloat p0 = key0.Value;
		        lnFloat p1 = key1.Value;
                lnFloat t0 = static_cast< lnFloat >( key0.FramePos );
		        lnFloat t1 = static_cast< lnFloat >( key1.FramePos );
                lnFloat t  = static_cast< lnFloat >( time_ - t0 ) / ( t1 - t0 );

                switch ( key0.InterMode )
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
                        lnprintf("a %f %f %f %f\n", mValue, p0, p1, t);
                        break;
                    }
			        // �������x
			        case LN_ANIM_INTER_ACCEL:
			        {
                        mValue = LMath::accel( p0, key0.Velocity, key0.Accel, static_cast< lnFloat >( mCurrentFramePos - key0.FramePos )  );
                        break;
			        }
			        // �O�����
			        case LN_ANIM_INTER_CUBIC:
			        {
				        mValue = LMath::cubic(
					        p0, p1, 
                            key0.RightSlope,
                            key1.LeftSlope,
					        t );
                        break;
			        }
			        // Catmull-Rom
			        case LN_ANIM_INTER_CATMULLROM:
			        {
                        // ���[�v�Đ��� time ���I�[�𒴂��Ă���ꍇ�A
                        // ���̎��_��key �̒l�� ���[�v�J�n�ʒu�̂ЂƂO�̃L�[���w���Ă���

                        const Key& begin = mKeyFrameList.front();
                        const Key& end   = mKeyFrameList.back();
                        
				        // ���̕�Ԃɂ́Abegin �̂ЂƂO�� end �̂ЂƂ�̒l���K�v�B
				        // ���ꂼ�ꂪ�n�_�A�I�_�̏ꍇ�̓��[�v����悤�ɕ�Ԃ���
				        mValue = LMath::catmullRom(
                            ( ( key0.FramePos == begin.FramePos ) ? end.Value : (itr - 1)->Value ),
					        p0,
					        p1,
					        ( ( key1.FramePos == end.FramePos ) ? begin.Value : (itr + 2)->Value ),
					        t );
                        break;
			        }
		        }
#endif
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
    void Scalar::advanceTime( lnFloat delta_time_ )
    {
        setTime( mTime + static_cast< double >( delta_time_ ) );
    }

	//----------------------------------------------------------------------
	// �� �Đ����I�����Ă��邩�𔻒肷�� (���[�v�Đ����͏�� false)
	//----------------------------------------------------------------------
    bool Scalar::isFinished() const
    {
        if ( mEnableLoop ) return false;

        if ( getKeyNum() == 0 ) { 
        	return true; }
        
        if ( mCurrentFramePos < mKeyFrameList.back().FramePos ) { 
        	return false; }

        return true;
    }

	//----------------------------------------------------------------------
	// �� �L�[��S�č폜����
	//----------------------------------------------------------------------
    void Scalar::clear()
    {
        mKeyFrameList.clear();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
     Scalar::Key* Scalar::getKey( int idx_ )
    {
        if ( idx_ >= getKeyNum() )
        {
            // �x���Ƃ�
            //printf( __FUNCTION__" index error[%d]\n", idx_ );
            *reinterpret_cast< int* >( 0 ) = 0;
        }
        return &mKeyFrameList[ idx_ ];
    }

	//----------------------------------------------------------------------
	// �� �L�[�ɃA�N�Z�X����
	//----------------------------------------------------------------------
    Scalar::Key& Scalar::operator [] ( int idx_ )
    {
        if ( idx_ >= getKeyNum() )
        {
            // �x���Ƃ�
            //printf( __FUNCTION__" index error[%d]\n", idx_ );
            *reinterpret_cast< int* >( 0 ) = 0;
        }
        return mKeyFrameList[ idx_ ];
    }

	//----------------------------------------------------------------------
	// �� ��r�֐�
	//----------------------------------------------------------------------
    int Scalar::_cmpKey( const void* a_, const void* b_ )
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