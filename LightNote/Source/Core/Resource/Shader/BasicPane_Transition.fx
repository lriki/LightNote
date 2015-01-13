//=============================================================================
// BasicPane_Transition
//-----------------------------------------------------------------------------
//  マスクテクスチャのない、単純なトランジション
//=============================================================================

float   gTransitionTime;

struct VS_OUTPUT 
{
    float4 Pos      : POSITION;
    float2 TexUV    : TEXCOORD0;
};

// LN_LAYERTYPE_BUFFERINGDRAW フラグが指定されているときに作成される内部 RT (mPrimaryRenderTarget)
texture gTexture;
sampler gTexSampler = sampler_state
{
    texture = < gTexture >;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;
    AddressU  = CLAMP;
    AddressV  = CLAMP;
};

// トランジション用のフリーズされたスクリーンのイメージ
texture gTransitionFreezedTexture;
sampler gTransitionFreezedSampler = sampler_state
{
    Texture = < gTransitionFreezedTexture >;
    MinFilter = None;
    MagFilter = None;
};

//-------------------------------------------------------------------------
// vertex shader
//-------------------------------------------------------------------------
VS_OUTPUT vsBasic(
    float3 in_pos_  : POSITION,
    float2 in_uv_   : TEXCOORD0 )
{
    VS_OUTPUT o;
    
    o.Pos = float4( in_pos_, 1.0f );
    o.TexUV = in_uv_;

    return o;
}

//-------------------------------------------------------------------------
// pixel shader
//-------------------------------------------------------------------------
float4 psTransiton(
    float2  in_uv_   : TEXCOORD0 ) : COLOR
{
    float4 out_color = tex2D( gTexSampler, in_uv_ );
    
    out_color.rgb = lerp(
        out_color.rgb,
        tex2D( gTransitionFreezedSampler, in_uv_ ).rgb,
        (1.0 - gTransitionTime) );
    
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
        PixelShader	 = compile ps_2_0 psTransiton();
    }
}

//=============================================================================
//
//=============================================================================
