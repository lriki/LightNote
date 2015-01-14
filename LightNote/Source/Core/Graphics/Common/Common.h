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
//  @brief      色のクラス (各要素 0.0～1.0)
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
	static const ColorF Transparency;			///< 透明 (0.0, 0.0, 0.0, 0.0)
	static const ColorF TransparencyWhite;		///< 白透明 (1.0, 1.0, 1.0, 0.0)
	static const ColorF White;					///< 白 (1.0, 1.0, 1.0, 1.0)
	static const ColorF Black;					///< 黒 (0.0, 0.0, 0.0, 1.0)
	static const ColorF Gray;					///< 灰色 (0.5, 0.5, 0.5, 1.0)
	static const ColorF Red;					///< 赤 (1.0, 0.0, 0.0, 1.0)
	static const ColorF Green;					///< 緑 (0.0, 1.0, 0.0, 1.0)
	static const ColorF Blue;					///< 青 (0.0, 0.0, 1.0, 1.0)

public:

	/// 黒(0.0, 0.0, 0.0, 1.0)で初期化
	ColorF() { R = 0.0; G = 0.0; B = 0.0; A = 1.0; }

	/// 各要素を指定して初期化
	ColorF(lnFloat r, lnFloat g, lnFloat b, lnFloat a = 1.0f) { R = r; G = g; B = b; A = a; }

	//----------------------------------------------------------------------
	///**
	//  @brief      コンストラクタ
	//
	//  @par
	//              32 ビットの色コード (ARGB) から作成します。
	//*/
	//----------------------------------------------------------------------
	ColorF(lnU32 color_code_, LNGraphicsAPI api_) { setFrom32Bit(color_code_, api_); }

	/// コピー
	ColorF(const ColorF& src) { R = src.R; G = src.G; B = src.B; A = src.A; }

	/// コピー
	ColorF& operator = (const ColorF& src) { R = src.R; G = src.G; B = src.B; A = src.A; return *this; }

public:

	//----------------------------------------------------------------------
	///**
	//  @brief      新しい値の設定
	//
	//  @par
	//              値が範囲外の場合は 0.0 ～ 1.0 の間にクリッピングされます。
	//*/
	//----------------------------------------------------------------------
	void set(lnFloat r, lnFloat g, lnFloat b, lnFloat a = 1.0f) { R = r; G = g; B = b; A = a; }

	//----------------------------------------------------------------------
	///**
	//  @brief      32 ビットの色コードから色を作成
	//
	//  @note
	//              ARGB の順で各 8 ビット
	//*/
	//----------------------------------------------------------------------
    void setFrom32Bit( lnU32 code_, LNGraphicsAPI api_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      32 ビットの色コードに変換
	//*/
	//----------------------------------------------------------------------
    lnU32 to32Bit( LNGraphicsAPI api_ ) const;	// 削除よてい
	lnU32 to32Bit() const;	// A R G B

	//----------------------------------------------------------------------
	///**
	//  @brief      Vector4 としての参照を返す
	//*/
	//----------------------------------------------------------------------
    const LVector4& toVector4() const { return *( (LVector4*)this ); }

	//----------------------------------------------------------------------
	///**
	//	@brief		要素の内容をコンソールに出力する
	// 
	//	@param[in]	str_ : ヘッダ文字列
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
//  @brief      色 (各要素 0～255)
//*/
//==============================================================================
class Color
{
public:
	static const Color Transparency;			///< 透明	(0, 0, 0, 0)
	static const Color TransparencyWhite;		///< 白透明	(255, 255, 255, 0)
	static const Color Black;					///< 黒		(0, 0, 0, 255)
	static const Color White;					///< 白		(255, 255, 255, 255)
	static const Color Gray;					///< 灰色	(127, 127, 127, 255)
	static const Color Red;						///< 赤		(255, 0, 0, 255)
	static const Color Green;					///< 緑		(0, 255, 0, 255)
	static const Color Blue;					///< 青		(0, 0.0, 255, 255)

public:
	lnU8	R;
	lnU8	G;
	lnU8	B;
	lnU8	A;

public:

	/// 黒(0, 0, 0, 255)で初期化
	Color() { R = 0; G = 0; B = 0; A = 255; }

	/// 各要素を指定して初期化
	Color(lnU8 r, lnU8 g, lnU8 b, lnU8 a) { R = r; G = g; B = b; A = a; }

public:

	/// 要素の一括設定
	void set(lnU8 r, lnU8 g, lnU8 b, lnU8 a) { R = r; G = g; B = b; A = a; }

	/// 32bit コードへ変換する (エンディアンによってバイトシーケンスが変わるため、Byte[4] とかに変換しないこと)
	lnU32 toU32ARGB() const { return (A << 24) | (R << 16) | (G << 8) | (B); }

public:
	bool operator == (const Color& color) { return (memcmp(this, &color, sizeof(Color)) == 0); }
	bool operator != (const Color& color) { return (memcmp(this, &color, sizeof(Color)) != 0); }

};

//==============================================================================
// HSVColor
//------------------------------------------------------------------------------
///**
//  @brief		HSV カラー
//*/
//==============================================================================
class HSVColor
{
public:
	lnU32	H;	///< 色相 0～360
	lnU32	S;	///< 彩度 0～255
	lnU32	V;	///< 明度 0～255
	lnU32	A;	///< 不透明度 0～255

public:

	/// 黒(0, 0, 0, 255)で初期化
	HSVColor() { H = 0; S = 0; V = 0; A = 255; }

	/// 各要素を指定して初期化
	HSVColor(lnU32 h, lnU32 s, lnU32 v, lnU32 a) { H = h; S = s; V = v; A = a; }

public:

	/// HSVからRGBに変換
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
////  @brief      色の実数表現 (各要素 0.0～1.0)
////*/
////==============================================================================
//class ColorF
//{
//public:
//	static const ColorF Transparency;			///< 透明 (0.0, 0.0, 0.0, 0.0)
//	static const ColorF TransparencyWhite;		///< 白透明 (1.0, 1.0, 1.0, 0.0)
//	static const ColorF White;					///< 白 (1.0, 1.0, 1.0, 1.0)
//	static const ColorF Black;					///< 黒 (0.0, 0.0, 0.0, 1.0)
//	static const ColorF Gray;					///< 灰色 (0.5, 0.5, 0.5, 1.0)
//	static const ColorF Red;					///< 赤 (1.0, 0.0, 0.0, 1.0)
//	static const ColorF Green;					///< 緑 (0.0, 1.0, 0.0, 1.0)
//	static const ColorF Blue;					///< 青 (0.0, 0.0, 1.0, 1.0)
//
//public:
//	lnFloat	R;
//	lnFloat	G;
//	lnFloat	B;
//	lnFloat	A;
//
//public:
//
//	/// 黒(0.0, 0.0, 0.0, 1.0)で初期化
//	ColorF() { R = 0.0; G = 0.0; B = 0.0; A = 1.0; }
//
//	/// 各要素を指定して初期化
//	ColorF(lnFloat r, lnFloat g, lnFloat b, lnFloat a) { R = r; G = g; B = b; A = a; }
//
//public:
//
//	/// 要素の一括設定
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
// ■ Tone クラス
//------------------------------------------------------------------------------
///**
//  @brief		色調のクラス
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
	//  @brief      デフォルトコンストラクタ
	//*/
	//----------------------------------------------------------------------
	Tone() : red( 0.0f ), green( 0.0f ), blue( 0.0f ), gray( 0.0f )
	{}

	//----------------------------------------------------------------------
	///**
	//  @brief      コンストラクタ
	//*/
	//----------------------------------------------------------------------
	Tone( lnFloat red_, lnFloat green_, lnFloat blue_, lnFloat gray_ = 0.0f )
	{
		set( red_, green_, blue_, gray_ );
	}

	//----------------------------------------------------------------------
	///**
	//  @brief      新しい値の設定
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
	//	@brief		要素の内容をコンソールに出力する
	// 
	//	@param[in]	str_ : ヘッダ文字列
	//*/
	//----------------------------------------------------------------------
	void dump( const char* str_ = NULL ) const;

	/// LVector4 への型変換
    operator LVector4() { return *reinterpret_cast< LVector4* >( this ); }
    operator const LVector4() const { return *reinterpret_cast< const LVector4* >( this ); }
};


//==============================================================================
// ■ Material クラス
//------------------------------------------------------------------------------
///**
//  @brief  マテリアルのクラス
//*/
//==============================================================================
class Material
{
public:

	/// 描画オプション
	enum DrawingFlags
	{
		DrawingFlag_CullingDouble	= 0x01,	///< 両面描画
		DrawingFlag_GroundShadow	= 0x02,	///< 地面影
		DrawingFlag_SelfShadowMap	= 0x04,	///< セルフシャドウマップへの描画
		DrawingFlag_SelfShadow		= 0x08,	///< セルフシャドウの描画
		DrawingFlag_Edge			= 0x10,	///< エッジ描画
	};

	/// スフィアテクスチャの合成モード
	enum SphereMode
	{
		SphereMode_Disable			= 0,	///< 無効
		SphereMode_Mul				= 1,	///< 乗算(sph)
		SphereMode_Add				= 2,	///< 加算(spa)
		SphereMode_SubTexture		= 3,	///< サブテクスチャ(追加UV1のx,yをUV参照して通常テクスチャ描画を行う)
	};

public:
    
    ColorF					Diffuse;		///< 物体の色
	ColorF					Ambient;		///< 環境光
	ColorF					Specular;		///< 光沢
	ColorF					Emissive;		///< 物体の発光色 ( 光源の影響を受けない色 )
    lnFloat					Power;			///< 光沢の強さ
	ColorF					EdgeColor;		///< エッジカラー
	lnFloat					EdgeSize;		///< エッジサイズ
	SphereMode				SphereMode;		///< SphereMode
	lnU8					DrawingFlags;	///< 描画オプション

	ColorF					TextureCoe;			///< テクスチャ係数
	ColorF					SphereTextureCoe;	///< スフィアテクスチャ係数
	ColorF					ToonTextureCoe;		///< Toonテクスチャ係数

private:

    Graphics::Texture*		_Texture;		///< テクスチャ (getTexture() setTexture()でアクセスする)
	Graphics::Texture*		_ToonTexture;	///< トゥーンテクスチャ (getToonTexture() setToonTexture()でアクセスする)
    Graphics::Texture*		_SphereTexture;	///< スフィアテクスチャ (getSphereTexture() setSphereTexture()でアクセスする)
    Graphics::IShader*		_Shader;		///< シェーダ (getShader() setShader()でアクセスする)

public:

	//----------------------------------------------------------------------
	///**
	//  @brief      コンストラクタ
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
	//  @brief      半透明要素があるかどうかを判定する
	//*/
	//----------------------------------------------------------------------
    bool isSemiTransState() const
    {
        return ( Diffuse.A < 1.0f );
    }

	/// テクスチャの取得
    Graphics::Texture* getTexture() const;

	/// テクスチャの設定
    void setTexture( Graphics::Texture* texture_ );

	/// トゥーンテクスチャの取得
    Graphics::Texture* getToonTexture() const;

	/// トゥーンテクスチャの設定
    void setToonTexture( Graphics::Texture* texture );

	/// スフィアテクスチャの取得
    Graphics::Texture* getSphereTexture() const;

	/// スフィアテクスチャの設定
    void setSphereTexture( Graphics::Texture* texture_ );

	/// シェーダの取得
    Graphics::IShader* getShader() const;

	/// シェーダの設定
    void setShader( Graphics::IShader* texture_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      コンソールに表示
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
			printf( "◆ LMaterial " );
		}
		printf( "Diffuse  : %f, %f, %f, %f\n", Diffuse.R, Diffuse.G, Diffuse.B, Diffuse.A );
		printf( "Ambient  : %f, %f, %f, %f\n", Ambient.R, Ambient.G, Ambient.B, Ambient.A );
		printf( "Specular : %f, %f, %f, %f\n", Specular.R, Specular.G, Specular.B, Specular.A );
		printf( "Emissive : %f, %f, %f, %f\n", Emissive.R, Emissive.G, Emissive.B, Emissive.A );
		printf( "Power    : %f\n", Power );
	}

	//----------------------------------------------------------------------
	///**
	//  @brief      比較
	//*/
	//----------------------------------------------------------------------
	bool operator == ( const Material& material_ )
	{
		return (
			Diffuse  == material_.Diffuse  && Ambient  == material_.Ambient &&
			Specular == material_.Specular && Emissive == material_.Emissive &&
            Power    == material_.Power );
	}

	/// コピーコンストラクタ
    Material( const Material& material_ );

	/// 代入
    Material& operator = ( const Material& material_ );

#ifdef LNOTE_INCLUDED_DIRECTX_HEADER

	//----------------------------------------------------------------------
	///**
	//  @brief      D3DMATERIAL9 から設定
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
	//  @brief      D3DMATERIAL9 に変換
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
	//  @brief      D3DMATERIAL9 をコンソールに表示
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

// 仮
typedef Core::Graphics::ColorF		LColorF;
typedef Core::Graphics::Color		LColor;
//typedef Core::Graphics::ColorF		LColorF;
typedef Core::Graphics::Tone		LTone;
typedef Core::Graphics::Material	LMaterial;

} // namespace LNote
