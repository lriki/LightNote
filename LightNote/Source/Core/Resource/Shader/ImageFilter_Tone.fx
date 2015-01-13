//==============================================================================
// ImageFilter_Tone
//------------------------------------------------------------------------------
//==============================================================================

// 色調
float4 gTone;

// スクリーン
texture gScreenTexture;
sampler gScreenSampler = sampler_state
{ 
	Texture		= <gScreenTexture>;
	MinFilter	= POINT;
	MagFilter	= POINT;
	MipFilter	= NONE;
	AddressU	= CLAMP;
	AddressV	= CLAMP;
};

// 出力構造体
struct VS_OUTPUT
{ 
	float4 Pos		: POSITION;
	float2 TexUV	: TEXCOORD0;
};

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
VS_OUTPUT vsBasic(
	float3 inPos	: POSITION,
	float2 inTexUV	: TEXCOORD0 )
{
	VS_OUTPUT o;
	o.Pos	= float4( inPos, 1.0f );
	o.TexUV	= inTexUV;
	return o;
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
float4 psBasic(
	float2 inTexUV	: TEXCOORD0 ) : COLOR
{
	float4 outColor = tex2D( gScreenSampler, inTexUV );
	float y = ( 0.208012 * outColor.r + 0.586611 * outColor.g + 0.114478 * outColor.b ) * gTone.w;
	outColor.rgb = ( outColor.rgb * (1.0 - gTone.w) ) + y + gTone.rgb;
	return outColor;
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
technique MainDraw
{
    pass P0
    {
        VertexShader	= compile vs_2_0 vsBasic();
        PixelShader		= compile ps_2_0 psBasic();
    }
}
