//=============================================================================
// SSBasic2D
//-----------------------------------------------------------------------------
//  2D�`���{
//=============================================================================

float4x4        gWorldMatrix    : WORLD;
float4x4        g2DProjMatrix   : VIEWPROJECTION; // �X�N���[���̍�������_�ɂƂ��A-0.5 �̃s�N�Z���I�t�Z�b�g���܂߂ăX�N���[�����W��ԏ�ɍ��W�ϊ�����s��

float2          gViewportSize   : VIEWPORTPIXELSIZE;
static float2   gViewportOffset = ( float2( 0.5, 0.5 ) / gViewportSize );

float           gOpacity        : OPACITY;
float4          gColorScale     : COLORSCALE;
float4          gBlendColor     : BLENDCOLOR;
float4          gTone           : TONE;
static float4   gColor          = float4( gColorScale.xyz, gColorScale.a * gOpacity );

texture gMaterialTexture	    : MATERIALTEXTURE;
sampler gTexSampler = sampler_state
{
    texture = < gMaterialTexture >;
    MINFILTER = NONE;
    MAGFILTER = NONE;
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
    float3 inPos  : POSITION,
    float2 inUV   : TEXCOORD0 )
{
    VS_OUTPUT o;
    
    o.Pos = mul( float4( inPos, 1.0 ), gWorldMatrix );
    o.Pos = mul( o.Pos, g2DProjMatrix );
    
    o.TexUV = inUV + gViewportOffset;

    return o;
}

//-------------------------------------------------------------------------
// pixel shader
//-------------------------------------------------------------------------
float4 psBasic(
    float2 in_uv_   : TEXCOORD0  ) : COLOR
{
    float4 out_color = tex2D( gTexSampler, in_uv_ ) * gColor;
    
    // �u�����h�̌v�Z
    //out_color.rgb = ( gBlendColor.rgb * gBlendColor.a ) + ( out_color.rgb * ( 1.0 - gBlendColor.a ) );
    out_color.rgb = lerp( out_color.rgb, gBlendColor.rgb, gBlendColor.a );
    
    // �F���̌v�Z�B(NTSC �n���d���ϖ@�Ƃ����炵���B�΂������Ȃ�ƂȂ����邭������Ƃ���h��)
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
//                              end of file
//=============================================================================
