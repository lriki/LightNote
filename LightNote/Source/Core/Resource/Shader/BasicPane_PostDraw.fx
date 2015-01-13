//=============================================================================
// BasicPane_PostDraw
//-----------------------------------------------------------------------------
//  Pane の最終描画
//=============================================================================

float4      gTone;

texture gPrimaryTexture;
sampler gPrimarySampler = sampler_state
{
    Texture = < gPrimaryTexture >;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;
    AddressU  = CLAMP;
    AddressV  = CLAMP;
};

struct VS_OUTPUT 
{
    float4 Pos      : POSITION;
    float2 TexUV    : TEXCOORD0;
};

//-------------------------------------------------------------------------
// vertex shader
//-------------------------------------------------------------------------
VS_OUTPUT vsBasic(
    float3 in_pos_  : POSITION,
    float2 in_uv_   : TEXCOORD0 )
{
    VS_OUTPUT o;
    o.Pos   = float4( in_pos_, 1.0f );
    o.TexUV = in_uv_;
    return o;
}

//-------------------------------------------------------------------------
// pixel shader
//-------------------------------------------------------------------------
float4 psBasic(
    float2  in_uv_   : TEXCOORD0 ) : COLOR
{
    float4 out_color = tex2D( gPrimarySampler, in_uv_ );
    
    float y = ( 0.208012 * out_color.r + 0.586611 * out_color.g + 0.114478 * out_color.b ) * gTone.w;
    out_color.rgb = ( out_color.rgb * ( 1.0 - gTone.w ) ) + y + gTone.rgb;
    
    return out_color;
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
