//=============================================================================
// GeometryRenderer
//-----------------------------------------------------------------------------
//  汎用の形状描画
//=============================================================================

float4x4        gWorldMatrix;
float4x4        gViewProjMatrix;

texture gTexture;
sampler2D gTexSampler = sampler_state
{
    texture = < gTexture >;
    MINFILTER = NONE;
    MAGFILTER = NONE;
};

struct VS_OUTPUT
{
    float4 Pos      : POSITION;
    float4 Color    : COLOR0;
    float2 TexUV    : TEXCOORD0;
};

//-------------------------------------------------------------------------
// vertex shader
//-------------------------------------------------------------------------
VS_OUTPUT vsBasic(
    float3 in_pos_      : POSITION,
    float4 in_color_    : COLOR0,
    float2 in_uv_       : TEXCOORD0 )
{
    VS_OUTPUT o;

    o.Pos   = mul( float4( in_pos_, 1.0f ), gWorldMatrix );
    o.Pos   = mul( o.Pos, gViewProjMatrix );
    o.Color = in_color_;
    o.TexUV = in_uv_;

    return o;
}

//-------------------------------------------------------------------------
// pixel shader
//-------------------------------------------------------------------------
float4 psBasic(
    float4 in_color_    : COLOR0,
    float2 in_uv_       : TEXCOORD0 ) : COLOR0
{
    return tex2D( gTexSampler, in_uv_ ) * in_color_;
}

//-------------------------------------------------------------------------
// technique
//-------------------------------------------------------------------------
technique MainDraw
{
    pass P0
    {
        VertexShader = compile vs_2_0 vsBasic();
        PixelShader	 = compile ps_2_0 psBasic();
    }
}

//=============================================================================
//
//=============================================================================
