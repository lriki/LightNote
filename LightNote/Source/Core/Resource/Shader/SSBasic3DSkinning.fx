//=============================================================================
// SSBasic3DSkinning
//-----------------------------------------------------------------------------
//
//=============================================================================

//-------------------------------------------------------------------------
// lnote.h
//-------------------------------------------------------------------------
#if 0
// ボーンテクスチャサイズの逆数
float2 lnBoneTextureReciprocalSize = float2( 1.0/4.0, 1.0/256.0 );

// ボーン用頂点テクスチャサンプラー宣言
texture lnBoneTexture;
sampler lnBoneSampler = sampler_state
{
    Texture = (lnBoneTexture);
    MinFilter = Point;  // 殆どのGPUでは以下のようなステート設定にしないと
    MagFilter = Point;  // 頂点テクスチャのフェッチがうまくいかない
    MipFilter = None;
    AddressU = Clamp;
    AddressV = Clamp;
};

struct LN_VS_SKINNING_INPUT
{
    float4 Position     : POSITION;
    float4 BlendWeights : BLENDWEIGHT;
    float4 BlendIndices : BLENDINDICES;
    float3 Normal       : NORMAL;
    float4 Color        : COLOR0;
    float2 TexUV        : TEXCOORD0;
    //float4 AddUV1       : TEXCOORD1;
    //float4 AddUV2       : TEXCOORD2;
    //float4 AddUV3       : TEXCOORD3;
    //float4 AddUV4       : TEXCOORD4;
    //float4 SdefC        : TEXCOORD5;
    //float3 SdefR0       : TEXCOORD6;
    //float3 SdefR1       : TEXCOORD7;
    //float  EdgeWeight   : TEXCOORD8;
    //float  Index        : PSIZE15;
};

struct LN_SKINNING_OUTPUT
{
    float4 Position;    // スキニング後の座標
    float3 Normal;      // スキニング後の法線
};

LN_SKINNING_OUTPUT LN_SkinnedPositionNormal(
    float4 position,
    float3 normal,
    float4 weights,
    float4 indices )
{
    LN_SKINNING_OUTPUT o;
    o.Position = 0.0;
    o.Normal = 0.0;
    
    float2 uv = lnBoneTextureReciprocalSize;
    float  weights_array[4]  = (float[4])weights;
    int    index_array[4]    = (int[4])D3DCOLORtoUBYTE4(indices);
    float  last_weight = 0.0f;
    
    // 最後のひとつ以外
    for ( int iBone = 0; iBone < 1; ++iBone )
    {
        last_weight = last_weight + weights_array[iBone];
        
        float4 tc0 = float4( 0.5f * uv.x, ( index_array[iBone] + 0.5f ) * uv.y, 0, 1 );
    	float4 tc1 = float4( 1.5f * uv.x, ( index_array[iBone] + 0.5f ) * uv.y, 0, 1 );
    	float4 tc2 = float4( 2.5f * uv.x, ( index_array[iBone] + 0.5f ) * uv.y, 0, 1 );
    	float4 tc3 = float4( 3.5f * uv.x, ( index_array[iBone] + 0.5f ) * uv.y, 0, 1 );
        
        float4x4 bone = float4x4(
            tex2Dlod( lnBoneSampler, tc0 ),
            tex2Dlod( lnBoneSampler, tc1 ),
            tex2Dlod( lnBoneSampler, tc2 ),
            tex2Dlod( lnBoneSampler, tc3 ) );
        
        o.Position += mul( position , bone ) * weights_array[iBone];
        o.Normal   += (mul( float4( normal, 1.0f ), bone ) * weights_array[iBone]).xyz;
    }
    
    // 最後のひとつ
    last_weight = 1.0f - last_weight;
    
    float4 tc0 = float4( 0.5f * uv.x, ( index_array[1] + 0.5f ) * uv.y, 0, 1 );
	float4 tc1 = float4( 1.5f * uv.x, ( index_array[1] + 0.5f ) * uv.y, 0, 1 );
	float4 tc2 = float4( 2.5f * uv.x, ( index_array[1] + 0.5f ) * uv.y, 0, 1 );
	float4 tc3 = float4( 3.5f * uv.x, ( index_array[1] + 0.5f ) * uv.y, 0, 1 );
    
    float4x4 bone = float4x4(
        tex2Dlod( lnBoneSampler, tc0 ),
        tex2Dlod( lnBoneSampler, tc1 ),
        tex2Dlod( lnBoneSampler, tc2 ),
        tex2Dlod( lnBoneSampler, tc3 ) );

    o.Position += mul( position, bone ) * last_weight;
    o.Normal   += (mul( float4( normal, 1.0f ), bone ) * last_weight).xyz;
    
    return o;
}
#endif

//-------------------------------------------------------------------------
// Parameters
//-------------------------------------------------------------------------
float4x4        gWorldMatrix		: WORLD;
float4x4        gViewProjMatrix		: VIEWPROJECTION;

// マテリアル
float4          gMaterialDiffuse    : DIFFUSE  < string Object = "Geometry"; >;
float3          gMaterialAmbient    : AMBIENT  < string Object = "Geometry"; >;
float3          gMaterialEmisive    : EMISSIVE < string Object = "Geometry"; >;
float3          gMaterialSpecular   : SPECULAR < string Object = "Geometry"; >;
float           gSpecularPower      : SPECULARPOWER < string Object = "Geometry"; >;
float3          gMaterialToon       : TOONCOLOR;

// ライト
float3          gLightDirection     : DIRECTION < string Object = "Light"; >;
float3          gLightDiffuse       : DIFFUSE   < string Object = "Light"; >;
float3          gLightAmbient       : AMBIENT   < string Object = "Light"; >;
float3          gLightSpecular      : SPECULAR  < string Object = "Light"; >;
static float4   gDiffuseColor       = gMaterialDiffuse  * float4( gLightDiffuse, 1.0f );
static float3   gAmbientColor       = saturate( gMaterialAmbient  * gLightAmbient + gMaterialEmisive );
static float3   gSpecularColor      = gMaterialSpecular * gLightSpecular;

// オブジェクトの色情報
float           gOpacity            : OPACITY;
float4          gColorScale         : COLORSCALE;
float4          gBlendColor         : BLENDCOLOR;
float4          gTone               : TONE;
static float4   gColor              = float4( gColorScale.xyz, gColorScale.a * gOpacity );


texture gMaterialTexture	        : MATERIALTEXTURE;
sampler gTexSampler = sampler_state
{
    texture = < gMaterialTexture >;
    MINFILTER = NONE;
    MAGFILTER = NONE;
    
    //ADDRESSU = MIRRORONCE;
    //ADDRESSV = MIRRORONCE;
};



//-------------------------------------------------------------------------
// struct
//-------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos      : POSITION;
    float4 Color    : COLOR0;
    float2 TexUV    : TEXCOORD0;
};

//-------------------------------------------------------------------------
// vertex shader
//-------------------------------------------------------------------------
//VS_OUTPUT vsBasic( LN_VS_SKINNING_INPUT IN )
VS_OUTPUT vsBasic( MMM_SKINNING_INPUT IN )
{
    VS_OUTPUT o;
	
    //LN_SKINNING_OUTPUT so = LN_SkinnedPositionNormal(
    //    IN.Position, IN.Normal, IN.BlendWeights, IN.BlendIndices );
    
	MMM_SKINNING_OUTPUT skinOut = 
		MMM_SkinnedPositionNormal(IN.Pos, IN.Normal, IN.BlendWeight, IN.BlendIndices, IN.SdefC, IN.SdefR0, IN.SdefR1);
	
    // 座標変換
    o.Pos = mul( skinOut.Position, gWorldMatrix );
	o.Pos = mul( o.Pos, gViewProjMatrix );
    
	// ディフューズ色＋アンビエント色 計算
    float3 global_normal = normalize( mul( skinOut.Normal, (float3x3)gWorldMatrix ) );
    o.Color = IN.Color;
    o.Color.rgb = saturate( max( 0, gDiffuseColor.rgb * dot( global_normal, -gLightDirection ) + gAmbientColor ) );//dot( Out.Normal, -LightDirection ))
    o.Color.a = gDiffuseColor.a;
    
	
	// テクスチャ座標
	o.TexUV = IN.Tex;

    return o;
}

//-------------------------------------------------------------------------
// pixel shader
//-------------------------------------------------------------------------
float4 psBasic(
    float4 in_color_    : COLOR0,
    float2 in_uv_       : TEXCOORD0  ) : COLOR
{
    float4 out_color = tex2D( gTexSampler, in_uv_ ) * in_color_ * gColor;
    
    // ブレンドの計算
    //out_color.rgb = ( gBlendColor.rgb * gBlendColor.a ) + ( out_color.rgb * ( 1.0 - gBlendColor.a ) );
    out_color.rgb = lerp( out_color.rgb, gBlendColor.rgb, gBlendColor.a );
    
    // 色調の計算。(NTSC 系加重平均法というらしい。緑だけがなんとなく明るく感じるとかを防ぐ)
	float y = ( 0.208012 * out_color.r + 0.586611 * out_color.g + 0.114478 * out_color.b ) * gTone.w;
    out_color.rgb = ( out_color.rgb * ( 1.0 - gTone.w ) ) + y + gTone.rgb;

    return out_color;//float4( 1, 0, 0, 1 );//
}

//-------------------------------------------------------------------------
// technique
//-------------------------------------------------------------------------
technique MainDraw
{
    pass P0
    {
        VertexShader = compile vs_3_0 vsBasic();
        PixelShader	 = compile ps_2_0 psBasic();
    }
}

//=============================================================================
//                              end of file
//=============================================================================