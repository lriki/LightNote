//==============================================================================
// Util 
//------------------------------------------------------------------------------
///**
//  @file       Util.h
//  @brief      Util
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// �� Util �N���X
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class Util
{
public:

	/// �s�N�Z���P�ʂ̓]����`���e�N�X�`��UV�̋�` (0.0�`1.0) �ɕϊ�����
    static void normalizeSrcRect(
        lnFloat* l_, lnFloat* t_, lnFloat* r_, lnFloat* b_,
        const Geometry::Rect& rect_, const LVector2& tex_size_ )
    {
        lnFloat x = static_cast< lnFloat >( rect_.x );
        lnFloat y = static_cast< lnFloat >( rect_.y );
        lnFloat w = static_cast< lnFloat >( rect_.w );
        lnFloat h = static_cast< lnFloat >( rect_.h );
        lnFloat tex_rw = 1.f / tex_size_.X;
        lnFloat tex_rh = 1.f / tex_size_.Y;

        *l_ = x * tex_rw;
        *t_ = y * tex_rh;
        *r_ = ( x + w ) * tex_rw;
        *b_ = ( y + h ) * tex_rh;

#if 0 // �ȉ��łȂ��ƌv�Z�덷���o�邪�A���ی��Ă݂�ƂقƂ�Ǖς��Ȃ������̂łƂ肠������̂܂�
        lnFloat tex_rw = 1.f / tex_size_.x;
        lnFloat tex_rh = 1.f / tex_size_.y;

        *l_ = x / tex_size_.x;
        *t_ = y / tex_size_.y;
        *r_ = ( x + w ) / tex_size_.x;
        *b_ = ( y + h ) / tex_size_.y;
#endif
    }

	/// �s�N�Z���P�ʂ̓]����`���e�N�X�`��UV�̋�` (0.0�`1.0) �ɕϊ�����
    static void normalizeSrcRect(
        lnFloat* l_, lnFloat* t_, lnFloat* r_, lnFloat* b_,
        const LRectF& rect_, const LVector2& tex_size_ )
    {
        lnFloat tex_rw = 1.f / tex_size_.X;
        lnFloat tex_rh = 1.f / tex_size_.Y;

        *l_ = rect_.x * tex_rw;
        *t_ = rect_.y * tex_rh;
        *r_ = ( rect_.x + rect_.w ) * tex_rw;
        *b_ = ( rect_.y + rect_.h ) * tex_rh;
	}
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core

} // namespace LNote

//==============================================================================
//
//==============================================================================