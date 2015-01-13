//==============================================================================
// Scalar 
//------------------------------------------------------------------------------
///**
//  @file       Scalar.h
//  @brief      Scalar
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include "../Math/Math.h"
#include "Types.h"

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
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class Scalar
{
public:

	// �L�[�t���[��
    struct Key
    {
        double      FramePos;       ///< �t���[���ʒu
        lnFloat     Value;          ///< �l
        lnFloat     Velocity;       ///< ���x
        lnFloat     Accel;          ///< �����x
        lnFloat     LeftSlope;      ///< ���� (�I��) ���x
        lnFloat     RightSlope;     ///< �E�� (�J�n) ���x
        lnU8          InterMode;      ///< ���̃L�[�Ƃ̕�ԕ��@ ( LNAnimInterMode �̒l )
    
        /// ������ ( InterMode �� LN_ANIM_INTER_LINEAR�A����ȊO�� 0.0 �ŏ����� )
        Key();
    };

public:

    Scalar();

public:

	/// ���Ԃ̃X�P�[���W���̐ݒ� (�ʏ�̓t���[�����[�g�̒l�����̂܂ܓn���Ă����B�f�t�H���g�l�� 60�BsetTime() �Ŏ��Ԃ̒l�ɏ�Z�����)
    void setTimeTickPerSec( lnFloat tick_ ) { mTimeTick = tick_; }

	/// �L�[���ЂƂ��Ȃ����A���Ԃ��ŏ��̃t���[���ʒu���O�̂Ƃ��ɕԂ��l�̐ݒ�
    void setDefaultValue( lnFloat value_ ) { mDefaultValue = value_; }

	/// �L�[�̒ǉ� (�\�[�g�͍s��Ȃ�)
    void addKeyFrame( double frame_pos_, lnFloat value_ );

	/// �L�[�̒ǉ� (�\�[�g�͍s��Ȃ�)
    void addKeyFrame( const Key& key_ );

	/// �L�[�t���[���̃\�[�g (�t���[���ʒu�̏���)
    void sortKeyFrame();

	/// �L�[�̐��̎擾
    int getKeyNum() const { return mKeyFrameList.size(); }

	/// ���[�v�Đ��̗L���ݒ�
    void setEnableLoop( bool flag_ );

	/// ���[�v�̈�̐ݒ�
    void setLoopFrame( lnFloat begin_frame_, lnFloat end_frame_ );

	/// ���Ԃ̐ݒ� (�ʏ�A�b�Ŏw�肷��)
    void setTime( double time_ );

	/// ���Ԃ�i�߂� (�i�߂����Ԃ� setTime() ���Ă΂��)
    void advanceTime( lnFloat delta_time_ );  

	/// �Đ����I�����Ă��邩�𔻒肷�� (�L�[�� 0 �̎��ƁA���Ԃ��Ō�̃L�[�̃t���[���ʒu�ȍ~�̏ꍇ true�B���[�v�Đ����͏�� false)
    bool isFinished() const;

	/// �l�̎擾
    lnFloat getValue() const { return mValue; }

	/// �L�[��S�č폜����
    void clear();

	/// �L�[�ɃA�N�Z�X����
    Key* getKey( int idx_ );

	/// �L�[�ɃA�N�Z�X����
    Key& operator [] ( int idx_ );

private:

    static int _cmpKey( const void* a_, const void* b_ );

private:

    typedef std::vector< Key >   KeyFrameList;  

private:

    KeyFrameList    mKeyFrameList;
    double          mTime;
    lnFloat         mTimeTick;
    double          mCurrentFramePos;   ///< setTime() �Őݒ肳�ꂽ���Ԃ̃t���[���ʒu (���[�v�͈͓�)
    double          mLoopBeginPos;
    double          mLoopEndPos;
    lnFloat         mDefaultValue;
    lnFloat         mValue;
    bool            mEnableLoop;
};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Animation
} // namespace Core
} // namespace LNote




// �g�p�@�ɂ���Ă͑S���g��Ȃ������o�ϐ�������B
// �����������̂͂Ƃ肠�����|�C���^�ɂ��Ă����āA�K�v�Ȃ��̂��� new �Ŋm�ۂ���悤�ɂ��悤�Ƃ��l�������ǁA
// new �̐���������ƍ쐬�Ɏ��Ԃ��������Ă���B���̕ӂ͑O�� Effect �N���X�Ŗ��ɂȂ����Ƃ���B
// ���̂��߁A���Ƃ����ʂȗ̈悪�����Ă��A�������̂��߂Ƀ����o�͑S�����Ԃō���Ă����B

// �����_���l�t���̂��̂͂������ɃT�C�Y�傫���Ȃ邵�A�G�t�F�N�g�ł����g��Ȃ�����A
// ����͌p�����Ď������Ă����B�@�� �ۗ�

// ���Ȃ݂� Effect �͂��ׂăX�J���[�̃L�[�ɂ���B�N�H�[�^�ɃI���͎g��Ȃ��B
// ( �N�H�[�^�ɃI�����g���ꍇ�͉�]�� 4 �̃g���b�N�ɑS�ăL�[�����悤�ɂ��Ȃ��ƃ_���Ƃ��A
//   ���낢�땡�G�ɂȂ�̂� )

//==============================================================================
//
//==============================================================================