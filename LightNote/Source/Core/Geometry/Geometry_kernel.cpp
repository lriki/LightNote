//==============================================================================
// Geometry 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Math/Math.h"
#include "Geometry.h"


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{


//==============================================================================
// �� Square �N���X
//==============================================================================


//==============================================================================
// �� LBox �N���X
//==============================================================================

	// �C�ӂ�2�_����{�b�N�X�����
    void LBox::create( const LVector3& p0_, const LVector3& p1_ )
    {
        if ( p0_.x < p1_.x )
        {
            Min.x = p0_.x;
            Max.x = p1_.x;
        }
        else
        {
            Min.x = p1_.x;
            Max.x = p0_.x;
        }

        if ( p0_.y < p1_.y )
        {
            Min.y = p0_.y;
            Max.y = p1_.y;
        }
        else
        {
            Min.y = p1_.y;
            Max.y = p0_.y;
        }

        if ( p0_.z < p1_.z )
        {
            Min.z = p0_.z;
            Max.z = p1_.z;
        }
        else
        {
            Min.z = p1_.z;
            Max.z = p0_.z;
        }
    }



//==============================================================================
// �� LPlane �N���X
//==============================================================================

	// �f�t�H���g�R���X�g���N�^
    LPlane::LPlane()
        : a ( 0.0f )
        , b ( 0.0f )
        , c ( 0.0f )
        , d ( 0.0f )
    {
    }

	// �R���X�g���N�^
    LPlane::LPlane( lnFloat a_, lnFloat b_, lnFloat c_, lnFloat d_ )
        : a ( a_ )
        , b ( b_ )
        , c ( c_ )
        , d ( d_ )
    {
    }

	// �R���X�g���N�^ (3�_����쐬����)
    LPlane::LPlane( const LVector3& v0_, const LVector3& v1_, const LVector3& v2_ )
    {
        create( v0_, v1_, v2_ );
    }

	// 3�_����쐬����
    void LPlane::create( const LVector3& p0_, const LVector3& p1_, const LVector3& p2_ )
    {
        LVector3 n;
        LVector3::cross( &n, ( p1_ - p0_ ), ( p2_ - p0_ ) );
        n.normalize();

        a = n.x;
        b = n.y;
        c = n.z;
        d = -LVector3::dot( p0_, n ); 
    }

	// ���ʂ� 3D �x�N�g���̓��ς��v�Z���� (w = 1 �Ƃ��Čv�Z����)
    lnFloat LPlane::dotCoord( const LVector3& v0_ ) const
    {
        return ( a * v0_.x ) + ( b * v0_.y ) + ( c * v0_.z ) + d;//*1
    }

	// �_�����ʂ̓����ɂ��邩�𔻒肷��
    bool LPlane::checkInside( const LVector3& point_ ) const
    {
	    return ( dotCoord( point_ ) >= 0.0f );
    }

	// �������ʂ̓����ɂ��邩�𔻒肷��
    bool LPlane::checkInside( const LVector3& point_, lnFloat radius_ ) const
    {
	    return ( dotCoord( point_ ) >= -radius_ );
    }

	// �R���\�[���ɏo�͂���
	void LPlane::dump( const char* str_ ) const
	{
		if ( str_ )
		{
			printf( "%s", str_ );
		}
		else
		{
			printf( "�� Plane " );
		}
		printf( "( %f, %f, %f, %f )\n", a, b, c, d );
	}

//==============================================================================
// �� LViewFrustum �N���X
//==============================================================================

	// �f�t�H���g�R���X�g���N�^
    LViewFrustum::LViewFrustum()
        : Right     ( 1.0f, 0.0f, 0.0f )
        , Up        ( 0.0f, 1.0f, 0.0f )
        , Forward   ( 0.0f, 0.0f, 1.0f )
    {
        Fov    = LMath::PI / 4.0f;
	    Aspect = 1.0f;
	    Near   = 1.0f;
	    Far    = 1000.0f;
    }

	// �R���X�g���N�^
    LViewFrustum::LViewFrustum( lnFloat fov_, lnFloat aspect_, lnFloat near_clip_, lnFloat far_clip_ )
    {
        create( fov_, aspect_, near_clip_, far_clip_ );
    }

	// �쐬
    void LViewFrustum::create( lnFloat fov_, lnFloat aspect_, lnFloat near_clip_, lnFloat far_clip_ )
    {
        Fov    = fov_;
	    Aspect = aspect_;
	    Near   = near_clip_;
	    Far    = far_clip_;

        lnFloat fov_over2 = ::tan( Fov / 2.0f );
	    LVector3 near_right = Right * Aspect * ( Near * fov_over2 ); // ��O�E
	    LVector3 far_right  = Right * Aspect * ( Far  * fov_over2 ); // ���E
	    LVector3 near_up    = Up * ( Near * fov_over2 );             // ��O��
	    LVector3 far_up     = Up * ( Far  * fov_over2 );             // ����

        // �E�ォ�玞�v���Ɏ�O�A���̎l�p�`������Ă���
	    NearClip[ 0 ] = ( Forward * Near ) - near_right + near_up;
	    NearClip[ 1 ] = ( Forward * Near ) + near_right + near_up;
	    NearClip[ 2 ] = ( Forward * Near ) + near_right - near_up;
	    NearClip[ 3 ] = ( Forward * Near ) - near_right - near_up;

	    FarClip[ 0 ] = ( Forward * Far ) - far_right + far_up;
	    FarClip[ 1 ] = ( Forward * Far ) + far_right + far_up;
	    FarClip[ 2 ] = ( Forward * Far ) + far_right - far_up;
	    FarClip[ 3 ] = ( Forward * Far ) - far_right - far_up;

        // �����������v���ɒ�`����ꍇ�A�@���͎������痣���������w���Ă���

	    LVector3 origin( 0.0f, 0.0f, 0.0f );
	    Planes[ LN_FRUSTUM_NEAR ].create(   NearClip[ 2 ], NearClip[ 1 ], NearClip[ 0 ] );
	    Planes[ LN_FRUSTUM_FAR ].create(    FarClip[ 0 ],  FarClip[ 1 ],  FarClip[ 2 ] );
	    Planes[ LN_FRUSTUM_RIGHT ].create(  FarClip[ 2 ],  FarClip[ 1 ],  origin);
	    Planes[ LN_FRUSTUM_TOP ].create(    FarClip[ 1 ],  FarClip[ 0 ],  origin);
	    Planes[ LN_FRUSTUM_LEFT ].create(   FarClip[ 0 ],  FarClip[ 3 ],  origin);
	    Planes[ LN_FRUSTUM_BOTTOM ].create( FarClip[ 3 ],  FarClip[ 2 ],  origin);
    }

	// �쐬
    void LViewFrustum::create2DProjection( lnFloat width_, lnFloat height_, lnFloat near_clip_, lnFloat far_clip_ )
    {
        Aspect = 0;//width_ / height_;  // ���ێg��Ȃ��̂Łc
	    Near   = near_clip_;
	    Far    = far_clip_;

        // �E�ォ�玞�v���Ɏ�O�A���̎l�p�`������Ă���
        NearClip[ 0 ].set( 0, 0, Near );
	    NearClip[ 1 ].set( width_, 0, Near );
	    NearClip[ 2 ].set( 0, height_, Near );
	    NearClip[ 3 ].set( width_, height_, Near );

        FarClip[ 0 ].set( 0, 0, Far );
	    FarClip[ 1 ].set( width_, 0, Far );
	    FarClip[ 2 ].set( 0, height_, Far );
	    FarClip[ 3 ].set( width_, height_, Far );

        // Planes
        LVector3 origin( 0.0f, 0.0f, 0.0f );
	    Planes[ LN_FRUSTUM_NEAR ].create(   NearClip[ 2 ], NearClip[ 1 ], NearClip[ 0 ] );
	    Planes[ LN_FRUSTUM_FAR ].create(    FarClip[ 0 ],  FarClip[ 1 ],  FarClip[ 2 ] );
	    Planes[ LN_FRUSTUM_RIGHT ].create(  FarClip[ 2 ],  FarClip[ 1 ],  origin);
	    Planes[ LN_FRUSTUM_TOP ].create(    FarClip[ 1 ],  FarClip[ 0 ],  origin);
	    Planes[ LN_FRUSTUM_LEFT ].create(   FarClip[ 0 ],  FarClip[ 3 ],  origin);
	    Planes[ LN_FRUSTUM_BOTTOM ].create( FarClip[ 3 ],  FarClip[ 2 ],  origin);
    }

	/// �ݒ肳��Ă���f�[�^�����ƂɎ�������쐬���� (transform_ �� NULL �w��\ ���̏ꍇ�AZ+ ������)
    void LViewFrustum::update( const LMatrix* transform_ )
    {
        lnFloat fov_over2 = ::tan( Fov / 2.0f );
	    LVector3 near_right = Right * Aspect * ( Near * fov_over2 ); // ��O�E
	    LVector3 far_right  = Right * Aspect * ( Far  * fov_over2 ); // ���E
	    LVector3 near_up    = Up * ( Near * fov_over2 );             // ��O��
	    LVector3 far_up     = Up * ( Far  * fov_over2 );             // ����

        // �E�ォ�玞�v���Ɏ�O�A���̎l�p�`������Ă���
	    NearClip[ 0 ] = ( Forward * Near ) - near_right + near_up;
	    NearClip[ 1 ] = ( Forward * Near ) + near_right + near_up;
	    NearClip[ 2 ] = ( Forward * Near ) + near_right - near_up;
	    NearClip[ 3 ] = ( Forward * Near ) - near_right - near_up;

	    FarClip[ 0 ] = ( Forward * Far ) - far_right + far_up;
	    FarClip[ 1 ] = ( Forward * Far ) + far_right + far_up;
	    FarClip[ 2 ] = ( Forward * Far ) + far_right - far_up;
	    FarClip[ 3 ] = ( Forward * Far ) - far_right - far_up;

        if ( transform_ )
        {
            for ( int i = 0; i < 4; ++i )
            {
                NearClip[ i ].transform( *transform_ );
                FarClip[ i ].transform( *transform_ );
            } 
        }

        // �����������v���ɒ�`����ꍇ�A�@���͎������痣���������w���Ă���

	    LVector3 origin( 0.0f, 0.0f, 0.0f );
	    Planes[ LN_FRUSTUM_NEAR ].create(   NearClip[ 2 ], NearClip[ 1 ], NearClip[ 0 ] );
	    Planes[ LN_FRUSTUM_FAR ].create(    FarClip[ 0 ],  FarClip[ 1 ],  FarClip[ 2 ] );
	    Planes[ LN_FRUSTUM_RIGHT ].create(  FarClip[ 2 ],  FarClip[ 1 ],  origin);
	    Planes[ LN_FRUSTUM_TOP ].create(    FarClip[ 1 ],  FarClip[ 0 ],  origin);
	    Planes[ LN_FRUSTUM_LEFT ].create(   FarClip[ 0 ],  FarClip[ 3 ],  origin);
	    Planes[ LN_FRUSTUM_BOTTOM ].create( FarClip[ 3 ],  FarClip[ 2 ],  origin);
    }

	// �_������̓����ɂ��邩�𔻒肷��
    bool LViewFrustum::checkInside( const LVector3& point_ ) const
    {
        for ( int i = 0; i < LN_FRUSTUM_MAX_PANELS; ++i )
	    {
		    if ( !Planes[ i ].checkInside( point_ ) )
            {
			    return false;
            }
	    }
	    return true;
    }

	// ��������̓����ɂ��邩�𔻒肷��
	bool LViewFrustum::checkInside( const LVector3& point_, lnFloat radius_ ) const
    {
        for ( int i = 0; i < LN_FRUSTUM_MAX_PANELS; ++i )
	    {
		    if ( !Planes[ i ].checkInside( point_, radius_ ) )
            {
			    return false;
            }
	    }
	    return true;
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Core

} // namespace LNote

//==============================================================================
//
//==============================================================================