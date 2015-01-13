//=============================================================================
// PointParticle
//-----------------------------------------------------------------------------
//  ポイントスプライトによる GPU パーティクル
//=============================================================================

float4x4        gWorldMatrix		: WORLD;
float4x4        gViewProjMatrix		: VIEWPROJECTION;
float           gProj_11_Div_00;
//float4x4        gProjMatrix			: PROJECTION;
//static float    gProj_11_22         = gViewProjMatrix._22 / gViewProjMatrix._11;
float3          gCameraPosition;

float           gTime;              // 再生開始からの経過時間
//float           gTimeTickInv;       // 時間のスケール係数の逆数 (60だった場合は 0.0166)
float3          gGlobalAccel;       // 全体に適用する加速度
float           gGravityPower;      // (0,0,0)への引力の強さ (負の値で斥力)
bool            gIsSequential;      // ParticleCreateData.CreateInterval が 0 の場合は true
bool            gIsParticleLoop;    // 粒子単位でループ再生するフラグ
bool            gIsDynamic;         // 動的生成フラグ

// オブジェクトの色情報
/*
float           gOpacity            : OPACITY;
float4          gColorScale         : COLORSCALE;
float4          gBlendColor         : BLENDCOLOR;
float4          gTone               : TONE;
*/

// テスクチャ
texture gMaterialTexture	        : MATERIALTEXTURE;
sampler gTexSampler = sampler_state
{
    texture = < gMaterialTexture >;
    MINFILTER = NONE;
    MAGFILTER = NONE;
};

// 出力構造体
struct VS_OUTPUT
{
    float4 Pos      : POSITION;
	float  Size     : PSIZE;
	float4 Color    : COLOR0;
};

//-------------------------------------------------------------------------
// vertex shader
//-------------------------------------------------------------------------
VS_OUTPUT vsBasic(
    float3 in_pos_          : POSITION,     // 初期座標
    float3 in_velocity_     : TEXCOORD0,    // 初速度
    float3 in_accel_        : TEXCOORD1,    // 加速度
    float3 in_axis_         : TEXCOORD2,    // 回転軸
    float3 in_angle_data_   : TEXCOORD3,    // [ 初期角度, 角速度, 角加速度 ]
    float3 in_size_data_    : TEXCOORD4,    // 大きさ [ 初期, 速度, 加速度 ]
    float4 in_time_data_    : TEXCOORD5,    // [ 開始時間, 存在時間, フェードイン時間, フェードアウト時間 ] ※全てフレーム数
    float4 in_fade_data_    : TEXCOORD6,    // 
    uniform bool sequential_,               // 指定された間隔で、順に表示していくフラグ (false の場合、全ての粒子が最初から表示されている)
    uniform bool loop_,
    uniform bool is_dynamic_ )   
{
    VS_OUTPUT o;
    
    //---------------------------------------------------------
    // time に粒子ごとのローカルの時間を作る
    
    // フレーム数を時間に変換
    //in_time_data_ *= gTimeTickInv;
    
    float time = ( gTime - in_time_data_.x );
    
    
    // 順に表示していく場合
    if ( sequential_ )//|| in_fade_data_.w != 0 ) w は残像時間オフセット
    {
        //time = 
        
        // 時間外の粒子は表示しない
        if ( time < 0 )
        {
            o.Pos   = float4( 0, 0, 0, 0 );
            o.Size  = 0;
            o.Color = float4( 0, 0, 0, 0 );
            return o;
        }
        
        
    }
#if 0
    else if ( time < 0.0 )
    {
        time = fmod( gTime + in_time_data_.y - fmod( in_time_data_.x , in_time_data_.y ), in_time_data_.y );
    }
#endif
    
    // ループ再生する場合は、time をアニメーションの繰返しを考慮した時間に変換
    if ( loop_ )
    {
        time = fmod( time, in_time_data_.y );
    }
    
    
    //---------------------------------------------------------
    // フェード処理
    
    o.Color.rgba = 1.0;
    
    // フェードイン
    // ( in_time_data_.z までは 0.0～1.0 で変化して、それ以上は saturate() によって 1.0 に丸められる )
    o.Color.a *= saturate( time / in_time_data_.z );

    // フェードアウト
    o.Color.a *= saturate( ( in_time_data_.y - time ) / in_time_data_.w );

	// アルファ乗算 (残像の場合、in_fade_data_.x には 1.0f 以外がセットされてる)
	o.Color.a *= in_fade_data_.x;
	

    //---------------------------------------------------------
    // 位置

    float t2 = 0.5 * time * time;
    
    // 位置
	float3 pos = in_pos_ + ( in_velocity_ * time ) + ( ( in_accel_ ) * t2 );
    
    // 角度
    float angle = in_angle_data_.x + ( in_angle_data_.y * time ) + ( in_angle_data_.z * t2 );

    // in_axis_ を軸として ang ラジアン回転させる行列
    float s = sin( angle );
    float c = cos( angle );
    float4x4 ang_mat = float4x4(
        (in_axis_.x*in_axis_.x*(1-c)+c), (in_axis_.x*in_axis_.y*(1-c)+in_axis_.z*s), (in_axis_.z*in_axis_.x*(1-c)-in_axis_.y*s), 0,
        (in_axis_.x*in_axis_.y*(1-c)-in_axis_.z*s), (in_axis_.y*in_axis_.y*(1-c)+c), (in_axis_.y*in_axis_.z*(1-c)+in_axis_.x*s), 0,
        (in_axis_.z*in_axis_.x*(1-c)+in_axis_.y*s), (in_axis_.y*in_axis_.z*(1-c)-in_axis_.x*s), (in_axis_.z*in_axis_.z*(1-c)+c), 0,
        0, 0, 0, 1 );

    // ワールド座標に変換
    o.Pos = mul( float4( pos, 1.0f ), ang_mat );
    if ( !is_dynamic_ )
    {
        o.Pos = mul( o.Pos, gWorldMatrix );
    }
    
    // 全体に適用する加速度
    o.Pos.xyz += gGlobalAccel * t2;

    // 引力・斥力
    if ( length( o.Pos.xyz ) != 0.0 && gGravityPower != 0.0 )
    {
        o.Pos.xyz += ( normalize( o.Pos.xyz ) * -gGravityPower ) * t2;
    }
    
    
    
    
    //---------------------------------------------------------
    // 大きさの計算
    float size = in_size_data_.x + ( in_size_data_.y * time ) + ( ( in_size_data_.z ) * t2 );
    float dist = length( gCameraPosition.xyz - o.Pos.xyz );
    o.Size = 1000.0 * size * gProj_11_Div_00 / dist;
    
    //float size = in_size_data_.x + ( in_size_data_.y * time ) + ( ( in_size_data_.z ) * t2 );
    //float4 ss = float4(size, 0, 0, 0);
    //o.Size = mul( ss, gViewProjMatrix ).x * 640;
    //o.Size = size * 20;
    
    // ビュー、プロジェクション変換
    o.Pos = mul( o.Pos, gViewProjMatrix );
    
    /*
    size *= 0.5;
    size =
        size * gViewProjMatrix._11 +
        size * gViewProjMatrix._12 + 
        size * gViewProjMatrix._13 +
        size * gViewProjMatrix._14;
        */
    //o.Size = size;// * 2;
    //float dist = length( o.Pos );
    //o.Size = size * ( gViewProjMatrix._11 ) * ( 1.0 / ( dist ) );
    
    return o;
}

//-------------------------------------------------------------------------
// pixel shader
//-------------------------------------------------------------------------
float4 psBasic(
    float4 in_color_    : COLOR0,
    float2 in_uv_       : TEXCOORD0  ) : COLOR0
{
	/*
    float4 out_color = tex2D( gTexSampler, in_uv_ ) * in_color_ * gColor;
    
    // ブレンドの計算
    //out_color.rgb = ( gBlendColor.rgb * gBlendColor.a ) + ( out_color.rgb * ( 1.0 - gBlendColor.a ) );
    out_color.rgb = lerp( out_color.rgb, gBlendColor.rgb, gBlendColor.a );
    
    // 色調の計算。(NTSC 系加重平均法というらしい。緑だけがなんとなく明るく感じるとかを防ぐ)
    float y = ( 0.208012 * out_color.r + 0.586611 * out_color.g + 0.114478 * out_color.b ) * gTone.w;
    out_color.rgb = ( out_color.rgb * ( 1.0 - gTone.w ) ) + y + gTone.rgb;
    
    return out_color;
    */

    return tex2D( gTexSampler, in_uv_ ) * in_color_;// * gColor;
}

//-------------------------------------------------------------------------
// technique
//-------------------------------------------------------------------------
technique MainDraw
{
    pass P0
    {
    	//PointSpriteEnable = true;	// ポイントスプライトを使う
    	
        VertexShader = compile vs_2_0 vsBasic( gIsSequential, gIsParticleLoop, gIsDynamic );
        PixelShader	 = compile ps_2_0 psBasic();
    }
}

//=============================================================================
//                              end of file
//=============================================================================
