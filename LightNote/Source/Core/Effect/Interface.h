//==============================================================================
// Interface 
//------------------------------------------------------------------------------
///**
//  @file       Interface.h
//  @brief      Interface
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../System/Interface.h"
#include "../Math/Matrix.h"

namespace LNote
{
namespace Core
{
namespace Effect
{

//==============================================================================
// Effect
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class Effect
	: public Base::ReferenceObject
{
public:
	virtual ~Effect() {};

public:

	/// �G�t�F�N�g�̍��W�ϊ��s���ݒ肷��
	virtual void setMatrix( const LMatrix& matrix ) = 0;

	/// �G�t�F�N�g���Đ�����
	///		overlap=true  : �Đ����̃G�t�F�N�g���~�����ɐV�����G�t�F�N�g�̍Đ����J�n����
	///		overlap=false : �Đ����̃G�t�F�N�g���~���Ă���V�����G�t�F�N�g���Đ�����
	///		�Â��G�t�F�N�g�́A�ȍ~���W�ϊ����X�V���Ȃ��B
	virtual void play( bool overlap ) = 0;

	/// �G�t�F�N�g���~����
	virtual void stop() = 0;

	/// �Đ��������m�F
	virtual bool isPlaying() = 0;

	/// ���Ԃ�i�߂�
	///		���ԒP�ʂ� Manager �ɐݒ肵���t���[�����[�g�ɂ��B
	///		���Ԃ�i�߂邾���ŁA���ۂ̍X�V�����͕ʃX���b�h�ōs����B
	virtual void advanceTime( lnFloat deltaTime ) = 0;

	/// �G�t�F�N�g��`�悷��
	virtual void draw() = 0;
};

#if 0

//==============================================================================
// �� IEffect
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class IEffect
    : public Base::Interface
{
public:

	/// �G�t�F�N�g�̍��W�ϊ��s���ݒ肷��
	virtual void setMatrix( const LMatrix& matrix ) = 0;

	/// �G�t�F�N�g���Đ�����
	///		overlap=true  : �Đ����̃G�t�F�N�g���~�����ɐV�����G�t�F�N�g�̍Đ����J�n����
	///		overlap=false : �Đ����̃G�t�F�N�g���~���Ă���V�����G�t�F�N�g���Đ�����
	///		�Â��G�t�F�N�g�́A�ȍ~���W�ϊ����X�V���Ȃ��B
	virtual void play( bool overlap ) = 0;

	/// �G�t�F�N�g���~����
	virtual void stop() = 0;

	/// �Đ��������m�F
	virtual bool isPlaying() = 0;

	/// ���Ԃ�i�߂�
	///		���ԒP�ʂ� Manager �ɐݒ肵���t���[�����[�g�ɂ��B
	///		���Ԃ�i�߂邾���ŁA���ۂ̍X�V�����͕ʃX���b�h�ōs����B
	virtual void advanceTime( lnFloat deltaTime ) = 0;

	/// �G�t�F�N�g��`�悷��
	virtual void draw() = 0;

protected:
    virtual ~IEffect() {};
};
#endif

} // namespace Effect
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================