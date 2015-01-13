//==============================================================================
// Common 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Common.h"
#include "../Interface.h"
#include "../DeviceObjects.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ColorF
//==============================================================================

	const ColorF ColorF::Transparency = ColorF(0.0, 0.0, 0.0, 0.0);
	const ColorF ColorF::TransparencyWhite = ColorF(1.0, 1.0, 1.0, 0.0);
	const ColorF ColorF::Black = ColorF(0.0, 0.0, 0.0, 1.0);
	const ColorF ColorF::White = ColorF(1.0, 1.0, 1.0, 1.0);
	const ColorF ColorF::Gray = ColorF(0.5, 0.5, 0.5, 1.0);
	const ColorF ColorF::Red = ColorF(1.0, 0.0, 0.0, 1.0);
	const ColorF ColorF::Green = ColorF(0.0, 1.0, 0.0, 1.0);
	const ColorF ColorF::Blue = ColorF(0.0, 0.0, 1.0, 1.0);


	////----------------------------------------------------------------------
	//// ● コンストラクタ
	////----------------------------------------------------------------------
 //   ColorT::ColorT( lnFloat r_, lnFloat g_, lnFloat b_, lnFloat a_ )
 //   {
 //       red		= ( r_ < 0.0f ) ? 0.0f : ( ( r_ > 1.0f ) ? 1.0f : r_ );
	//	green	= ( g_ < 0.0f ) ? 0.0f : ( ( g_ > 1.0f ) ? 1.0f : g_ );
	//	blue	= ( b_ < 0.0f ) ? 0.0f : ( ( b_ > 1.0f ) ? 1.0f : b_ );
	//	alpha	= ( a_ < 0.0f ) ? 0.0f : ( ( a_ > 1.0f ) ? 1.0f : a_ );
 //   }

	////----------------------------------------------------------------------
	//// ● 新しい値の設定
	////----------------------------------------------------------------------
 //   void ColorT::set( lnFloat r_, lnFloat g_, lnFloat b_, lnFloat a_ )
 //   {
 //       red		= ( r_ < 0.0f ) ? 0.0f : ( ( r_ > 1.0f ) ? 1.0f : r_ );
	//	green	= ( g_ < 0.0f ) ? 0.0f : ( ( g_ > 1.0f ) ? 1.0f : g_ );
	//	blue	= ( b_ < 0.0f ) ? 0.0f : ( ( b_ > 1.0f ) ? 1.0f : b_ );
	//	alpha	= ( a_ < 0.0f ) ? 0.0f : ( ( a_ > 1.0f ) ? 1.0f : a_ );
 //   }

	//----------------------------------------------------------------------
	// ● 32 ビットの色コードから色を作成
	//----------------------------------------------------------------------
	void ColorF::setFrom32Bit(lnU32 code_, LNGraphicsAPI api_)
    {
        static const lnFloat d = 0.003921568627450980392156862745098f;
        switch ( api_ )
        {
            case LN_GRAPHICSAPI_DIRECTX9:
            case LN_GRAPHICSAPI_DIRECTX11:
                R	    = static_cast< lnFloat >( code_ & 0x000000ff ) * d;
		        code_ >>= 8;
		        G	= static_cast< lnFloat >( code_ & 0x000000ff ) * d;
		        code_ >>= 8;
		        B    = static_cast< lnFloat >( code_ & 0x000000ff ) * d;
		        code_ >>= 8;
		        A	= static_cast< lnFloat >( code_ & 0x000000ff ) * d;
                break;
            case LN_GRAPHICSAPI_OPENGL:
                B	= static_cast< lnFloat >( code_ & 0x000000ff ) * d;
		        code_ >>= 8;
		        G	= static_cast< lnFloat >( code_ & 0x000000ff ) * d;
		        code_ >>= 8;
		        R		= static_cast< lnFloat >( code_ & 0x000000ff ) * d;
		        code_ >>= 8;
		        A	= static_cast< lnFloat >( code_ & 0x000000ff ) * d;
                break;
            default:
                LN_THROW_NotImpl( 0 );
                break;
        }
    }

	//----------------------------------------------------------------------
	// ● 32 ビットの色コードに変換
	//----------------------------------------------------------------------
	lnU32 ColorF::to32Bit(LNGraphicsAPI api_) const
    {
        lnU32 r = static_cast< lnU32 >( R   * 255.0f );
		lnU32 g = static_cast< lnU32 >( G * 255.0f );
		lnU32 b = static_cast< lnU32 >( B  * 255.0f );
		lnU32 a = static_cast< lnU32 >( A * 255.0f );
        switch ( api_ )
        {
            case LN_GRAPHICSAPI_DIRECTX9:
            case LN_GRAPHICSAPI_DIRECTX11:
                return ( ( ( a & 0xff ) << 24 ) | ( ( r & 0xff ) << 16 ) | ( ( g & 0xff ) << 8 ) | ( b & 0xff ) );
            case LN_GRAPHICSAPI_OPENGL:
		        return ( ( ( a & 0xff ) << 24 ) | ( ( b & 0xff ) << 16 ) | ( ( g & 0xff ) << 8 ) | ( r & 0xff ) );
            default:
                LN_THROW_NotImpl( 0 );
                return 0;
        }
    }
	lnU32 ColorF::to32Bit() const
    {
		return
			(((lnU32)(A * 255.0f)) & 0xff) << 24 |
			(((lnU32)(R   * 255.0f)) & 0xff) << 16 |
			(((lnU32)(G * 255.0f)) & 0xff) << 8  |
			(((lnU32)(B  * 255.0f)) & 0xff) ;
	}

	//----------------------------------------------------------------------
	// ● 要素の内容をコンソールに出力する
	//----------------------------------------------------------------------
	void ColorF::dump(const char* str_) const
    {
        if ( str_ )
		{
			printf( "%s", str_ );
		}
		else
		{
			printf( "◆ ColorT " );
		}
		printf( "( %f, %f, %f, %f )\n", R, G, B, A );
    }

 //   bool ColorT::operator == ( const ColorT& color )
 //   {
 //       return ( memcmp( this, &color, sizeof( ColorT ) ) == 0 );
 //  //     return (
	//		//red == color_.red && green == color_.green &&
	//		//blue == color_.blue && alpha == color_.alpha );
 //   }

 //   bool ColorT::operator != ( const ColorT& color )
 //   {
 //       return ( memcmp( this, &color, sizeof( ColorT ) ) != 0 );
	//}

//==============================================================================
// Color
//==============================================================================

	const Color Color::Transparency = Color(0, 0, 0, 0);
	const Color Color::TransparencyWhite = Color(255, 255, 255, 0);
	const Color Color::Black = Color(0, 0, 0, 255);
	const Color Color::White = Color(255, 255, 255, 255);
	const Color Color::Gray = Color(127, 127, 127, 255);
	const Color Color::Red = Color(255, 0, 0, 255);
	const Color Color::Green = Color(0, 255, 0, 255);
	const Color Color::Blue = Color(0, 0.0, 255, 255);

//==============================================================================
// ■ Tone クラス
//==============================================================================

	// コンソールに出力する
	void Tone::dump( const char* str_ ) const
	{
		if ( str_ )
		{
			printf( "%s", str_ );
		}
		else
		{
			printf( "◆ Tone " );
		}
		printf( "( %f, %f, %f, %f )\n", red, green, blue, gray );
	}

//==============================================================================
// ■ Material クラス
//==============================================================================

	/// デストラクタ
    Material::~Material()
    {
        LN_SAFE_RELEASE( _Texture );
		LN_SAFE_RELEASE( _ToonTexture );
        LN_SAFE_RELEASE( _SphereTexture );
        LN_SAFE_RELEASE( _Shader );
    }

	/// テクスチャの取得
    Graphics::Texture* Material::getTexture() const
    {
        return _Texture;
    }

	/// テクスチャの設定
    void Material::setTexture( Graphics::Texture* texture_ )
    {
        LN_SAFE_ADDREF( texture_ );
        LN_SAFE_RELEASE( _Texture );
        _Texture = texture_;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Graphics::Texture* Material::getToonTexture() const
	{
		return _ToonTexture;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Material::setToonTexture( Graphics::Texture* texture )
	{
		LN_REFOBJ_SET( _ToonTexture, texture );
	}

	/// スフィアテクスチャの取得
    Graphics::Texture* Material::getSphereTexture() const
    {
        return _SphereTexture;
    }

	/// スフィアテクスチャの設定
    void Material::setSphereTexture( Graphics::Texture* texture_ )
    {
        LN_SAFE_ADDREF( texture_ );
        LN_SAFE_RELEASE( _SphereTexture );
        _SphereTexture = texture_;    
    }

	/// シェーダの取得
    Graphics::IShader* Material::getShader() const
    {
        return _Shader;
    }

	/// シェーダの設定
    void Material::setShader( Graphics::IShader* shader_ )
    {
        LN_SAFE_ADDREF( shader_ );
        LN_SAFE_RELEASE( _Shader );
        _Shader = shader_;
    }

	/// コピーコンストラクタ
    Material::Material( const Material& material_ )
    {
        Diffuse = material_.Diffuse;
		Ambient = material_.Ambient;
		Specular = material_.Specular;
		Emissive = material_.Emissive;
		Power = material_.Power;
		
        _Texture = NULL;
		_ToonTexture = NULL;
        _SphereTexture = NULL;
        _Shader = NULL;
        setTexture( material_._Texture );
		setToonTexture( material_._ToonTexture );
        setSphereTexture( material_._SphereTexture );
        setShader( material_._Shader );

		EdgeColor = material_.EdgeColor;
		EdgeSize = material_.EdgeSize;
		SphereMode = material_.SphereMode;
		DrawingFlags = material_.DrawingFlags;
    }

	/// 代入
    Material& Material::operator = ( const Material& material_ )
    {
        Diffuse = material_.Diffuse;
		Ambient = material_.Ambient;
		Specular = material_.Specular;
		Emissive = material_.Emissive;
		Power = material_.Power;
		
  //      _Texture = NULL;
		//_ToonTexture = NULL;
  //      _SphereTexture = NULL;
  //      _Shader = NULL;
        setTexture( material_._Texture );
		setToonTexture( material_._ToonTexture );
        setSphereTexture( material_._SphereTexture );
        setShader( material_._Shader );

		EdgeColor = material_.EdgeColor;
		EdgeSize = material_.EdgeSize;
		SphereMode = material_.SphereMode;
		DrawingFlags = material_.DrawingFlags;
        return *this;
    }

	
//==============================================================================
// ■ LRenderState クラス
//==============================================================================

    const LRenderState LRenderState::BUFFER_COPY_STATE = LRenderState(
		BlendMode_Normal,
		CullingMode_Front,
		FillMode_Solid,
        false, false, false, false );

	const LRenderState LRenderState::BUFFER_COPY_STATE_WITH_ALPHA = LRenderState(
		BlendMode_Alpha,
		CullingMode_Front,
		FillMode_Solid,
        false, false, false, false );

    const LRenderState LRenderState::SMOOTH_ALPHA = LRenderState(
		BlendMode_Alpha,
		CullingMode_Front,
		FillMode_Solid,
        false, false, false, false );

} // namespace Graphics
    
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------


} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================