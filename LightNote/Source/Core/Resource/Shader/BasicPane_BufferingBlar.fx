//=============================================================================
// BasicPane_BufferingBlar
//-----------------------------------------------------------------------------
//  ブラ―処理の実装
//=============================================================================

float       gBlurPower;         // ブラーの強さ
float4      gBlurColor;         // ブラーの色
float4x4    gBlurMatrix;        // ブラーイメージの座標変換行列

struct VS_OUTPUT 
{
    float4 Pos      : POSITION;
    float2 TexUV    : TEXCOORD0;
};

// ブラー用の 1フレーム前のイメージ
texture gSecondaryTexture;
sampler gSecondarySampler = sampler_state
{
    Texture = < gSecondaryTexture >;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;
    AddressU  = CLAMP;
    AddressV  = CLAMP;
};

//-------------------------------------------------------------------------
// vertex shader
//-------------------------------------------------------------------------
VS_OUTPUT vsBufferingBulr(
    float3 in_pos_  : POSITION,
    float2 in_uv_   : TEXCOORD0 )
{
    VS_OUTPUT o;
    o.Pos = mul( float4( in_pos_, 1.0f ), gBlurMatrix );
    o.TexUV = in_uv_;
    return o;
}

//-------------------------------------------------------------------------
// pixel shader
//-------------------------------------------------------------------------
float4 psBufferingBulr(
    float2  in_uv_   : TEXCOORD0 ) : COLOR
{
    float4 out_color = tex2D( gSecondarySampler, in_uv_ );
    out_color.a *= gBlurPower;
    out_color *= gBlurColor;
    
    // ブラーを青っぽくする
    //out_color.rgb *= float3(0, 0, 1);
    
    // 外側ほど透明になるようにする
    //float len = length( in_uv_ - 0.5 );
    //color.a *= 1.0 - len;
    
    return out_color;
}

//-------------------------------------------------------------------------
// technique
//-------------------------------------------------------------------------
technique MainDraw
{
    pass P0
    {
        VertexShader = compile vs_2_0 vsBufferingBulr();
        PixelShader	 = compile ps_2_0 psBufferingBulr();
    }
}

//=============================================================================
//
//=============================================================================
