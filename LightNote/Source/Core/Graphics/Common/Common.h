//==============================================================================
// Common 
//------------------------------------------------------------------------------
///**
//  @file       Common.h
//  @brief      Common
//*/
//==============================================================================

#pragma once

#include "../Common.h"
#include "Types.h"
#include "../../Math/LMath.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{ 
//class ITexture;
//class IShader;

//==============================================================================
// ColorF
//------------------------------------------------------------------------------
///**
//  @brief      �F�̃N���X (�e�v�f 0.0�`1.0)
//*/
//==============================================================================
class ColorF
{
public:
	lnFloat R;
	lnFloat G;
	lnFloat B;
	lnFloat A;

public:
	static const ColorF Transparency;			///< ���� (0.0, 0.0, 0.0, 0.0)
	static const ColorF TransparencyWhite;		///< ������ (1.0, 1.0, 1.0, 0.0)
	static const ColorF White;					///< �� (1.0, 1.0, 1.0, 1.0)
	static const ColorF Black;					///< �� (0.0, 0.0, 0.0, 1.0)
	static const ColorF Gray;					///< �D�F (0.5, 0.5, 0.5, 1.0)
	static const ColorF Red;					///< �� (1.0, 0.0, 0.0, 1.0)
	static const ColorF Green;					///< �� (0.0, 1.0, 0.0, 1.0)
	static const ColorF Blue;					///< �� (0.0, 0.0, 1.0, 1.0)

public:

	/// ��(0.0, 0.0, 0.0, 1.0)�ŏ�����
	ColorF() { R = 0.0; G = 0.0; B = 0.0; A = 1.0; }

	/// �e�v�f���w�肵�ď�����
	ColorF(lnFloat r, lnFloat g, lnFloat b, lnFloat a = 1.0f) { R = r; G = g; B = b; A = a; }

	//----------------------------------------------------------------------
	///**
	//  @brief      �R���X�g���N�^
	//
	//  @par
	//              32 �r�b�g�̐F�R�[�h (ARGB) ����쐬���܂��B
	//*/
	//----------------------------------------------------------------------
	ColorF(lnU32 color_code_, LNGraphicsAPI api_) { setFrom32Bit(color_code_, api_); }

	/// �R�s�[
	ColorF(const ColorF& src) { R = src.R; G = src.G; B = src.B; A = src.A; }

	/// �R�s�[
	ColorF& operator = (const ColorF& src) { R = src.R; G = src.G; B = src.B; A = src.A; return *this; }

public:

	//----------------------------------------------------------------------
	///**
	//  @brief      �V�����l�̐ݒ�
	//
	//  @par
	//              �l���͈͊O�̏ꍇ�� 0.0 �` 1.0 �̊ԂɃN���b�s���O����܂��B
	//*/
	//----------------------------------------------------------------------
	void set(lnFloat r, lnFloat g, lnFloat b, lnFloat a = 1.0f) { R = r; G = g; B = b; A = a; }

	//----------------------------------------------------------------------
	///**
	//  @brief      32 �r�b�g�̐F�R�[�h����F���쐬
	//
	//  @note
	//              ARGB �̏��Ŋe 8 �r�b�g
	//*/
	//----------------------------------------------------------------------
    void setFrom32Bit( lnU32 code_, LNGraphicsAPI api_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      32 �r�b�g�̐F�R�[�h�ɕϊ�
	//*/
	//----------------------------------------------------------------------
    lnU32 to32Bit( LNGraphicsAPI api_ ) const;	// �폜��Ă�
	lnU32 to32Bit() const;	// A R G B

	//----------------------------------------------------------------------
	///**
	//  @brief      Vector4 �Ƃ��Ă̎Q�Ƃ�Ԃ�
	//*/
	//----------------------------------------------------------------------
    const LVector4& toVector4() const { return *( (LVector4*)this ); }

	//----------------------------------------------------------------------
	///**
	//	@brief		�v�f�̓��e���R���\�[���ɏo�͂���
	// 
	//	@param[in]	str_ : �w�b�_������
	//*/
	//----------------------------------------------------------------------
	void dump( const char* str_ = NULL ) const;


public:
	bool operator == (const ColorF& color) { return (memcmp(this, &color, sizeof(ColorF)) == 0); }
	bool operator != (const ColorF& color) { return (memcmp(this, &color, sizeof(ColorF)) != 0); }

	ColorF& operator = (const LVector4& vec) { R = vec.X; G = vec.Y; B = vec.Z; A = vec.W; return *this; }

	operator const LVector4&() { return *reinterpret_cast<LVector4*>(this); }
	operator const LVector4&() const { return *reinterpret_cast<const LVector4*>(this); }
};

//==============================================================================
// Color
//------------------------------------------------------------------------------
///**
//  @brief      �F (�e�v�f 0�`255)
//*/
//==============================================================================
class Color
{
public:
	static const Color Transparency;			///< ����	(0, 0, 0, 0)
	static const Color TransparencyWhite;		///< ������	(255, 255, 255, 0)
	static const Color Black;					///< ��		(0, 0, 0, 255)
	static const Color White;					///< ��		(255, 255, 255, 255)
	static const Color Gray;					///< �D�F	(127, 127, 127, 255)
	static const Color Red;						///< ��		(255, 0, 0, 255)
	static const Color Green;					///< ��		(0, 255, 0, 255)
	static const Color Blue;					///< ��		(0, 0.0, 255, 255)

public:
	lnU8	R;
	lnU8	G;
	lnU8	B;
	lnU8	A;

public:

	/// ��(0, 0, 0, 255)�ŏ�����
	Color() { R = 0; G = 0; B = 0; A = 255; }

	/// �e�v�f���w�肵�ď�����
	Color(lnU8 r, lnU8 g, lnU8 b, lnU8 a) { R = r; G = g; B = b; A = a; }

public:

	/// �v�f�̈ꊇ�ݒ�
	void set(lnU8 r, lnU8 g, lnU8 b, lnU8 a) { R = r; G = g; B = b; A = a; }

	/// 32bit �R�[�h�֕ϊ����� (�G���f�B�A���ɂ���ăo�C�g�V�[�P���X���ς�邽�߁AByte[4] �Ƃ��ɕϊ����Ȃ�����)
	lnU32 toU32ARGB() const { return (A << 24) | (R << 16) | (G << 8) | (B); }

public:
	bool operator == (const Color& color) { return (memcmp(this, &color, sizeof(Color)) == 0); }
	bool operator != (const Color& color) { return (memcmp(this, &color, sizeof(Color)) != 0); }

};

//==============================================================================
// HSVColor
//------------------------------------------------------------------------------
///**
//  @brief		HSV �J���[
//*/
//==============================================================================
class HSVColor
{
public:
	lnU32	H;	///< �F�� 0�`360
	lnU32	S;	///< �ʓx 0�`255
	lnU32	V;	///< ���x 0�`255
	lnU32	A;	///< �s�����x 0�`255

public:

	/// ��(0, 0, 0, 255)�ŏ�����
	HSVColor() { H = 0; S = 0; V = 0; A = 255; }

	/// �e�v�f���w�肵�ď�����
	HSVColor(lnU32 h, lnU32 s, lnU32 v, lnU32 a) { H = h; S = s; V = v; A = a; }

public:

	/// HSV����RGB�ɕϊ�
	Color toRGBAColor() const 
	{
		unsigned char c1, c2, c3;
		if (S == 0) 
		{
			return Color(V, V, V, A);
		}
		else 
		{
			int t = (H * 6) % 360;
			c1 = (V*(255 - S)) / 255;
			c2 = (V*(255 - (S*t) / 360)) / 255;
			c3 = (V*(255 - (S*(360 - t)) / 360)) / 255;
			switch (H / 60)
			{
				case 0: return Color(V, c3, c1, A);
				case 1: return Color(c2, V, c1, A);
				case 2: return Color(c1, V, c3, A);
				case 3: return Color(c1, c2, V, A);
				case 4: return Color(c3, c1, V, A);
				case 5: return Color(V, c1, c2, A);
			}
		}
		return Color();
	}
};

////==============================================================================
//// ColorF
////------------------------------------------------------------------------------
/////**
////  @brief      �F�̎����\�� (�e�v�f 0.0�`1.0)
////*/
////==============================================================================
//class ColorF
//{
//public:
//	static const ColorF Transparency;			///< ���� (0.0, 0.0, 0.0, 0.0)
//	static const ColorF TransparencyWhite;		///< ������ (1.0, 1.0, 1.0, 0.0)
//	static const ColorF White;					///< �� (1.0, 1.0, 1.0, 1.0)
//	static const ColorF Black;					///< �� (0.0, 0.0, 0.0, 1.0)
//	static const ColorF Gray;					///< �D�F (0.5, 0.5, 0.5, 1.0)
//	static const ColorF Red;					///< �� (1.0, 0.0, 0.0, 1.0)
//	static const ColorF Green;					///< �� (0.0, 1.0, 0.0, 1.0)
//	static const ColorF Blue;					///< �� (0.0, 0.0, 1.0, 1.0)
//
//public:
//	lnFloat	R;
//	lnFloat	G;
//	lnFloat	B;
//	lnFloat	A;
//
//public:
//
//	/// ��(0.0, 0.0, 0.0, 1.0)�ŏ�����
//	ColorF() { R = 0.0; G = 0.0; B = 0.0; A = 1.0; }
//
//	/// �e�v�f���w�肵�ď�����
//	ColorF(lnFloat r, lnFloat g, lnFloat b, lnFloat a) { R = r; G = g; B = b; A = a; }
//
//public:
//
//	/// �v�f�̈ꊇ�ݒ�
//	void set(lnFloat r, lnFloat g, lnFloat b, lnFloat a) { R = r; G = g; B = b; A = a; }
//
//public:
//	bool operator == (const ColorF& color) { return (memcmp(this, &color, sizeof(ColorF)) == 0); }
//	bool operator != (const ColorF& color) { return (memcmp(this, &color, sizeof(ColorF)) != 0); }
//
//	ColorF& operator = (const LVector4& vec) { R = vec.x; G = vec.y; B = vec.z; A = vec.w; return *this; }
//
//	operator const LVector4&() { return *reinterpret_cast<LVector4*>(this); }
//	operator const LVector4&() const { return *reinterpret_cast<const LVector4*>(this); }
//};

//==============================================================================
// �� Tone �N���X
//------------------------------------------------------------------------------
///**
//  @brief		�F���̃N���X
//*/
//==============================================================================
class Tone
{
public:

	lnFloat red;
	lnFloat green;
	lnFloat blue;
	lnFloat gray;

public:

	//----------------------------------------------------------------------
	///**
	//  @brief      �f�t�H���g�R���X�g���N�^
	//*/
	//----------------------------------------------------------------------
	Tone() : red( 0.0f ), green( 0.0f ), blue( 0.0f ), gray( 0.0f )
	{}

	//----------------------------------------------------------------------
	///**
	//  @brief      �R���X�g���N�^
	//*/
	//----------------------------------------------------------------------
	Tone( lnFloat red_, lnFloat green_, lnFloat blue_, lnFloat gray_ = 0.0f )
	{
		set( red_, green_, blue_, gray_ );
	}

	//----------------------------------------------------------------------
	///**
	//  @brief      �V�����l�̐ݒ�
	//*/
	//----------------------------------------------------------------------
	void set( lnFloat red_, lnFloat green_, lnFloat blue_, lnFloat gray_ = 0.0f )
	{
		red		= ( red_   < -1.0f ) ? -1.0f : ( ( red_   > 1.0f ) ? 1.0f : red_ );
		green	= ( green_ < -1.0f ) ? -1.0f : ( ( green_ > 1.0f ) ? 1.0f : green_ );
		blue	= ( blue_  < -1.0f ) ? -1.0f : ( ( blue_  > 1.0f ) ? 1.0f : blue_ );
		gray	= ( gray_  < 0.0f ) ? 0.0f : ( ( gray_ > 1.0f ) ? 1.0f : gray_ );
	}

	//----------------------------------------------------------------------
	///**
	//	@brief		�v�f�̓��e���R���\�[���ɏo�͂���
	// 
	//	@param[in]	str_ : �w�b�_������
	//*/
	//----------------------------------------------------------------------
	void dump( const char* str_ = NULL ) const;

	/// LVector4 �ւ̌^�ϊ�
    operator LVector4() { return *reinterpret_cast< LVector4* >( this ); }
    operator const LVector4() const { return *reinterpret_cast< const LVector4* >( this ); }
};


//==============================================================================
// �� Material �N���X
//------------------------------------------------------------------------------
///**
//  @brief  �}�e���A���̃N���X
//*/
//==============================================================================
class Material
{
public:

	/// �`��I�v�V����
	enum DrawingFlags
	{
		DrawingFlag_CullingDouble	= 0x01,	///< ���ʕ`��
		DrawingFlag_GroundShadow	= 0x02,	///< �n�ʉe
		DrawingFlag_SelfShadowMap	= 0x04,	///< �Z���t�V���h�E�}�b�v�ւ̕`��
		DrawingFlag_SelfShadow		= 0x08,	///< �Z���t�V���h�E�̕`��
		DrawingFlag_Edge			= 0x10,	///< �G�b�W�`��
	};

	/// �X�t�B�A�e�N�X�`���̍������[�h
	enum SphereMode
	{
		SphereMode_Disable			= 0,	///< ����
		SphereMode_Mul				= 1,	///< ��Z(sph)
		SphereMode_Add				= 2,	///< ���Z(spa)
		SphereMode_SubTexture		= 3,	///< �T�u�e�N�X�`��(�ǉ�UV1��x,y��UV�Q�Ƃ��Ēʏ�e�N�X�`���`����s��)
	};

public:
    
    ColorF					Diffuse;		///< ���̂̐F
	ColorF					Ambient;		///< ����
	ColorF					Specular;		///< ����
	ColorF					Emissive;		///< ���̂̔����F ( �����̉e�����󂯂Ȃ��F )
    lnFloat					Power;			///< ����̋���
	ColorF					EdgeColor;		///< �G�b�W�J���[
	lnFloat					EdgeSize;		///< �G�b�W�T�C�Y
	SphereMode				SphereMode;		///< SphereMode
	lnU8					DrawingFlags;	///< �`��I�v�V����

	ColorF					TextureCoe;			///< �e�N�X�`���W��
	ColorF					SphereTextureCoe;	///< �X�t�B�A�e�N�X�`���W��
	ColorF					ToonTextureCoe;		///< Toon�e�N�X�`���W��

private:

    Graphics::Texture*		_Texture;		///< �e�N�X�`�� (getTexture() setTexture()�ŃA�N�Z�X����)
	Graphics::Texture*		_ToonTexture;	///< �g�D�[���e�N�X�`�� (getToonTexture() setToonTexture()�ŃA�N�Z�X����)
    Graphics::Texture*		_SphereTexture;	///< �X�t�B�A�e�N�X�`�� (getSphereTexture() setSphereTexture()�ŃA�N�Z�X����)
    Graphics::IShader*		_Shader;		///< �V�F�[�_ (getShader() setShader()�ŃA�N�Z�X����)

public:

	//----------------------------------------------------------------------
	///**
	//  @brief      �R���X�g���N�^
	//*/
	//----------------------------------------------------------------------
	Material()
	{
		Diffuse.set( 1.0f, 1.0f, 1.0f, 1.0f );
		Ambient.set( 0.0f, 0.0f, 0.0f, 0.0f );
		Specular.set( 0.5f, 0.5f, 0.5f, 0.5f );
		Emissive.set( 0.0f, 0.0f, 0.0f, 0.0f );
		Power = 50.0f;
        _Texture = NULL;
		_ToonTexture = NULL;
        _SphereTexture = NULL;
        _Shader = NULL;
		EdgeColor.set( 0.0f, 0.0f, 0.0f, 1.0f );
		EdgeSize = 0.0f;
		SphereMode = SphereMode_Disable;
		DrawingFlags = 0;
	}

    ~Material();

	//----------------------------------------------------------------------
	///**
	//  @brief      �������v�f�����邩�ǂ����𔻒肷��
	//*/
	//----------------------------------------------------------------------
    bool isSemiTransState() const
    {
        return ( Diffuse.A < 1.0f );
    }

	/// �e�N�X�`���̎擾
    Graphics::Texture* getTexture() const;

	/// �e�N�X�`���̐ݒ�
    void setTexture( Graphics::Texture* texture_ );

	/// �g�D�[���e�N�X�`���̎擾
    Graphics::Texture* getToonTexture() const;

	/// �g�D�[���e�N�X�`���̐ݒ�
    void setToonTexture( Graphics::Texture* texture );

	/// �X�t�B�A�e�N�X�`���̎擾
    Graphics::Texture* getSphereTexture() const;

	/// �X�t�B�A�e�N�X�`���̐ݒ�
    void setSphereTexture( Graphics::Texture* texture_ );

	/// �V�F�[�_�̎擾
    Graphics::IShader* getShader() const;

	/// �V�F�[�_�̐ݒ�
    void setShader( Graphics::IShader* texture_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      �R���\�[���ɕ\��
	//*/
	//----------------------------------------------------------------------
	void dump( const char* str_ = NULL ) const
	{
        if ( str_ )
		{
			printf( "%s\n", str_ );
		}
		else
		{
			printf( "�� LMaterial " );
		}
		printf( "Diffuse  : %f, %f, %f, %f\n", Diffuse.R, Diffuse.G, Diffuse.B, Diffuse.A );
		printf( "Ambient  : %f, %f, %f, %f\n", Ambient.R, Ambient.G, Ambient.B, Ambient.A );
		printf( "Specular : %f, %f, %f, %f\n", Specular.R, Specular.G, Specular.B, Specular.A );
		printf( "Emissive : %f, %f, %f, %f\n", Emissive.R, Emissive.G, Emissive.B, Emissive.A );
		printf( "Power    : %f\n", Power );
	}

	//----------------------------------------------------------------------
	///**
	//  @brief      ��r
	//*/
	//----------------------------------------------------------------------
	bool operator == ( const Material& material_ )
	{
		return (
			Diffuse  == material_.Diffuse  && Ambient  == material_.Ambient &&
			Specular == material_.Specular && Emissive == material_.Emissive &&
            Power    == material_.Power );
	}

	/// �R�s�[�R���X�g���N�^
    Material( const Material& material_ );

	/// ���
    Material& operator = ( const Material& material_ );

#ifdef LNOTE_INCLUDED_DIRECTX_HEADER

	//----------------------------------------------------------------------
	///**
	//  @brief      D3DMATERIAL9 ����ݒ�
	//*/
	//----------------------------------------------------------------------
	void setFromDxMaterial( const D3DMATERIAL9& dx_material_ )
	{
		Diffuse.R	   = dx_material_.Diffuse.r;
		Diffuse.G  = dx_material_.Diffuse.g;
		Diffuse.B   = dx_material_.Diffuse.b;
		Diffuse.A  = dx_material_.Diffuse.a;
		Ambient.R	   = dx_material_.Ambient.r;
		Ambient.G  = dx_material_.Ambient.g;
		Ambient.B   = dx_material_.Ambient.b;
		Ambient.A  = dx_material_.Ambient.a;
		Specular.R   = dx_material_.Specular.r;
		Specular.G = dx_material_.Specular.g;
		Specular.B  = dx_material_.Specular.b;
		Specular.A = dx_material_.Specular.a;
		Emissive.A   = dx_material_.Emissive.r;
		Emissive.G = dx_material_.Emissive.g;
		Emissive.B  = dx_material_.Emissive.b;
		Emissive.A = dx_material_.Emissive.a;
		Power = dx_material_.Power;
	}

	//----------------------------------------------------------------------
	///**
	//  @brief      D3DMATERIAL9 �ɕϊ�
	//*/
	//----------------------------------------------------------------------
	D3DMATERIAL9 toDxMaterial() const
	{
		D3DMATERIAL9 dx_material;
		dx_material.Diffuse.r  = Diffuse.R;
		dx_material.Diffuse.g  = Diffuse.G;
		dx_material.Diffuse.b  = Diffuse.B;
		dx_material.Diffuse.a  = Diffuse.A;
		dx_material.Ambient.r  = Ambient.R;
		dx_material.Ambient.g  = Ambient.G;
		dx_material.Ambient.b  = Ambient.B;
		dx_material.Ambient.a  = Ambient.A;
		dx_material.Specular.r = Specular.R;
		dx_material.Specular.g = Specular.G;
		dx_material.Specular.b = Specular.B;
		dx_material.Specular.a = Specular.A;
		dx_material.Emissive.r = Emissive.R;
		dx_material.Emissive.g = Emissive.G;
		dx_material.Emissive.b = Emissive.B;
		dx_material.Emissive.a = Emissive.A;
		dx_material.Power = Power;
		return dx_material;
	}

	//----------------------------------------------------------------------
	///**
	//  @brief      D3DMATERIAL9 ���R���\�[���ɕ\��
	//*/
	//----------------------------------------------------------------------
	static void dumpDxMaterial( const D3DMATERIAL9& dx_material_ )
	{
		printf( "D3DMATERIAL9----------------------------\n" );
		printf( "diffuse  : %f, %f, %f, %f\n", dx_material_.Diffuse.r, dx_material_.Diffuse.g, dx_material_.Diffuse.b, dx_material_.Diffuse.a );
		printf( "ambient  : %f, %f, %f, %f\n", dx_material_.Ambient.r, dx_material_.Ambient.g, dx_material_.Ambient.b, dx_material_.Ambient.a );
		printf( "specular : %f, %f, %f, %f\n", dx_material_.Specular.r, dx_material_.Specular.g, dx_material_.Specular.b, dx_material_.Specular.a );
		printf( "emissive : %f, %f, %f, %f\n", dx_material_.Emissive.r, dx_material_.Emissive.g, dx_material_.Emissive.b, dx_material_.Emissive.a );
		printf( "power    : %f\n", dx_material_.Power );
		printf( "----------------------------------------\n" );
	}

#endif // LNOTE_DIRECTX

private:
};

} // namespace Graphics
} // namespace Core

// ��
typedef Core::Graphics::ColorF		LColorF;
typedef Core::Graphics::Color		LColor;
//typedef Core::Graphics::ColorF		LColorF;
typedef Core::Graphics::Tone		LTone;
typedef Core::Graphics::Material	LMaterial;

} // namespace LNote
