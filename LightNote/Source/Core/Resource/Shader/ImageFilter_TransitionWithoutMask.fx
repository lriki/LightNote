//=============================================================================
// ImageFilter_TransitionWithoutMask
//-----------------------------------------------------------------------------
//  マスクテクスチャのない、全体のフェードイン/アウト トランジション
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
    Texture = <gTransitionFreezedTexture>;
    MinFilter = None;
    MagFilter = None;
};

//-------------------------------------------------------------------------
// vertex shader
//-------------------------------------------------------------------------
VS_OUTPUT vsBasic(
	float3	inPos	: POSITION,
	float2	inTexUV	: TEXCOORD0 )
{
	VS_OUTPUT o;
	o.Pos = float4( inPos, 1.0f );
	o.TexUV = inTexUV;
	return o;
}

//-------------------------------------------------------------------------
// pixel shader
//-------------------------------------------------------------------------
float4 psTransiton(
	float2	inTexUV	: TEXCOORD0 ) : COLOR
{
    float4 out_color = tex2D( gTexSampler, inTexUV );
    out_color.rgb = lerp(
        out_color.rgb,
        tex2D( gTransitionFreezedSampler, inTexUV ).rgb,
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
