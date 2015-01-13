//=============================================================================
// SSBasic2D
//-----------------------------------------------------------------------------
//  2D描画基本
//=============================================================================

float4x4        gWorldMatrix    : WORLD;
float4x4        g2DProjMatrix   : VIEWPROJECTION; // スクリーンの左上を原点にとし、-0.5 のピクセルオフセットを含めてスクリーン座標空間上に座標変換する行列

float2          g_ViewportSize   : VIEWPORTPIXELSIZE;
static float2   gViewportOffset = ( float2( 0.5, 0.5 ) / g_ViewportSize );

float           gOpacity        : OPACITY;
float4          gColorScale     : COLORSCALE;
float4          gBlendColor     : BLENDCOLOR;
float4          gTone           : TONE;
static float4   gColor          = float4( gColorScale.xyz, gColorScale.a * gOpacity );


float			g_Time1			: TIME;		// 経過時間 (秒)
static float			g_Time = g_Time1 / 100;		// 経過時間 (秒)


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




//-------------------------------------------------------------
// distance functions
// http://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
//-------------------------------------------------------------
float disp(float3 p, float w)      { return sin(w * p.x) * sin(w * p.y) * sin(w * p.z); }
float disp2(float3 p, float w)     { return sin(w * p.x * 0.79) * sin(w * p.y * 0.56) * sin(w * p.z * 0.91); }
float plane(float3 p, float4 n)    { return dot(p, normalize(n.xyz))    + n.w; }
float sphere(float3 p, float s)    { return length(p)         - s;             }
float cyl(float3 p, float3 s)      { return length(p.xz)      - s.xz;          }
float box(float3 p, float3 s)      { return length(max(abs(p) - s.xz,  0));    }
float ibox(float3 p, float3 scale) { return length(max(abs(p) - scale, 0).xz) - scale.xz * 0.2;}

//rotate
float2 rot(float2 d, float t) { return float2(cos(t) * d.x - sin(t) * d.y, sin(t) * d.x + cos(t) * d.y); }

//増やすマクロ
#define dup(name, prep, map, ite, ret) float name(float3 p) { map; } float4 p##name(float2 vp : texcoord0) : color {  float2 uv = -1 + 2 * vp;  if(abs(uv.y) > 0.75) return 0; float3 dir = normalize(float3(uv * float2(1.25, 1), 1));   float3 start = float3(0, 0, g_Time); prep; float3 pos = start; for(int i = 0 ; i < ite; i++) { pos += 0.75*name(pos) * dir;  } float2 k = float2(0.01, 0); float3 N = normalize(float3( name(pos + k.xyy), name(pos + k.yxy), name(pos + k.yyx)) - name(pos));ret;}


//-------------------------------------------------------------
//func pd
//-------------------------------------------------------------
dup(
  d,
  //レイ前処理
  float t = g_Time * 0.2;
  start.z += t*2;
  dir.yz = rot(dir.zy, t*0.3);
  dir.xy = rot(dir.xy, t*0.7),
  
  //オブジェクト距離計算
	/*
  if( (g_Time % 60) > 45) {
    p.x += -cos(p.y+g_Time*9) * 0.3;
    p.y +=  sin(p.z+g_Time*7) * 0.3;
    p.z += -cos(p.y+g_Time*7) * 0.3;
  }
	*/
  float3 tp = float3(p.x + sin(p.z * 0.3) * 1.3, p.y + cos(p.z * 0.2) * 1.2, p.z);
  float   k = dot(2-p, float3(0, 1, 0));
  
  //fragment
  k = max(k, -(length(abs(p.xz   % 0.20)-0.10)-0.08));
  k = max(k, -(length(abs(p.xz   % 1.00)-0.50)-0.45));
  k = min(k,  ibox(   abs(tp     % 4.00)-2.00, 0.2));
  k = max(k, -(length(abs(tp.yx  % 5.00)-3.00)-2.0));
  k = max(k, -(length(abs(p.xy   % 0.20)-0.10)-0.08));
  k = max(k, -(length(abs(p.xy   % 0.04)-0.02)-0.008));
  k = max(k, -(length(abs(p.xz   % 0.04)-0.02)-0.008));
  k = max(k, -(length(abs(p.zy   % 0.04)-0.02)-0.008));
  return k,
  
  //重いのでもう少し小さくしても良いかも
  70,
  
  //色作る
  float  d = length(pos - start);
  float  D = dot(N, normalize(float3(1, 1, 1)));
  return float4((pow(D,32)+D*(0.4*float3(1, 3, 5))) + d * 0.04, 1);
)



















float4 psBasic(
    float2 inUV   : TEXCOORD0  ) : COLOR
{
	return pd(inUV);//float4(inUV.x, 1, 0, 1);
}



float hex( float2 p, float2 h )
{
	float2 q = abs(p);
	return max(q.x-h.y,max(q.x+q.y*0.57735,q.y*1.1547)-h.x);
}


float4 psDemo12(
    float2 inUV   : TEXCOORD0  ) : COLOR
{
	float aspect = g_ViewportSize.x / g_ViewportSize.y;
	float2 pos = ( inUV.xy / g_ViewportSize.xy ) * 2.0 - 1.0;
	pos.x *= aspect;
	
	float4 c = float4(0, 0, 0, 0);
	
	float radius = 0.22;
	float d = hex(pos, float2(radius, radius));
	
	if (d > 0.0)
		c = float4(1, 0, 1, 1);
	//c = float4(d>0.0 ? 0.0 : 1.0);
	
	return c;
}

float4 psDemo1(
    float2 inUV   : TEXCOORD0  ) : COLOR
{
	// 画面の左下が(-w, -1), 右上が(w, 1)となるような座標系を作る。
	// pos は、今回の pixel はこの座標系上のどの位置かを示す。
	float aspect = g_ViewportSize.x / g_ViewportSize.y;
	float2 pos = float2(inUV.x * aspect, 1.0-inUV.y) * 2.0 - 1.0;	// DirectX の TexUV なので、上下を逆転させている
	
	// カメラの定義 (左手座標系としている。手前から奥を見ている)
	float3 cameraPos	= float3(0.0, 0.0, 3.0);
	float3 cameraFront	= float3(0.0, 0.0, 1.0);
	float3 cameraUp		= float3(0.0, 1.0, 0.0);
	float3 cameraRight	= cross(cameraFront, cameraUp);
	float focus = 1.8;	// カメラと描画平面の距離
	
	float3 rayDir = normalize(cameraRight*pos.x + cameraUp*pos.y + cameraFront*focus);
	
	//float2 pos = ( inUV.xy / g_ViewportSize.xy ) * 2.0 - 1.0;
	//pos.x *= aspect;
	
	
	
	return float4(rayDir, 1);
}

// カメラからのレイマップ
float4 psDemo2(
    float2 inUV   : TEXCOORD0  ) : COLOR
{
	float2 resolution = g_ViewportSize / g_ViewportSize.y;//float2(g_ViewportSize.x / g_ViewportSize.y, g_ViewportSize.y / g_ViewportSize.y);	// (w, 1.0) へ (wはアス比としても使える)
	
	// 画面の左下が(-w, -1), 右上が(w, 1)となるような座標系を作る。(w は、画面が (640, 480)なら1.33になっている)
	// inUV は 0.0～1.0
	// pos は、今回の pixel はこの座標系上のどの位置かを示す。
	inUV.x *= resolution.x;
	inUV.y = 1.0 - inUV.y;	// DirectX の TexUV なので、上下を逆転させている
	float2 pos = (inUV.xy*2.0 - resolution) / resolution.y;
	
	// カメラの定義 (左手座標系としている。手前から奥を見ている)
	float3 cameraPos	= float3(0.0, 0.0, 3.0);
	float3 cameraFront	= float3(0.0, 0.0, 1.0);
	float3 cameraUp		= float3(0.0, 1.0, 0.0);
	float3 cameraRight	= cross(cameraUp, cameraFront);// float3(1.0, 0.0, 0.0);//
	float focus = 1.8;	// カメラと描画平面の距離
	
	float3 rayDir = normalize(cameraRight*pos.x + cameraUp*pos.y + cameraFront*focus);
	
	return float4((rayDir.xy + 1.0) /  2.0, rayDir.z, 1.0);	// 画像として見やすいように、-1.0～1.0 のベクトルサイズを 0.0～1.0 に変換している
}


float sdSphere( float3 p, float s )
{
  return length(p)-s;
}
float udBox( float3 p, float3 b )
{
  return length(max(abs(p)-b,0.0));
}
// カメラからのレイマップ
float4 psDemo3(
    float2 inUV   : TEXCOORD0  ) : COLOR
{
	float2 resolution = g_ViewportSize / g_ViewportSize.y;//float2(g_ViewportSize.x / g_ViewportSize.y, g_ViewportSize.y / g_ViewportSize.y);	// (w, 1.0) へ (wはアス比としても使える)
	
	// 画面の左下が(-w, -1), 右上が(w, 1)となるような座標系を作る。(w は、画面が (640, 480)なら1.33になっている)
	// inUV は 0.0～1.0
	// pos は、今回の pixel はこの座標系上のどの位置かを示す。
	inUV.x *= resolution.x;
	inUV.y = 1.0 - inUV.y;	// DirectX の TexUV なので、上下を逆転させている
	float2 pos = (inUV.xy*2.0 - resolution) / resolution.y;
	
	// カメラの定義 (左手座標系としている。手前から奥を見ている)
	float3 cameraPos	= float3(0.0, 0.0, -3.0);
	float3 cameraFront	= float3(0.0, 0.0, 1.0);
	float3 cameraUp		= float3(0.0, 1.0, 0.0);
	float3 cameraRight	= cross(cameraUp, cameraFront);// float3(1.0, 0.0, 0.0);//
	float focus = 1.8;	// カメラと描画平面の距離
	
	float3 rayDir = normalize(cameraRight*pos.x + cameraUp*pos.y + cameraFront*focus);
	
	
	
	// ↓
	float t = 0.0, d;
	float3 posOnRay = cameraPos;	// レイの始点

	for(int i=0; i<16; ++i)
	{
		d = sdSphere(posOnRay, 1.0);//udBox(posOnRay, float3(1, 1, 1));//
		t += d;
		posOnRay = cameraPos + t*rayDir;
	}

	float4 c;
	if(abs(d) < 0.001)
	{
		c = float4(1, 1, 1, 1);
	}else
	{
		c = float4(0, 0, 0, 0);
	}
	
	return c;
}




///////////////////////////////////////////////////////////////////////////////
float3 trans(float3 p)
{
  //return fmod(p, 8.0)-4.0;
	//return (p % 8.0) - 4.0;
	return ((abs(p) % 4.0)) - 2.0;	// ここに abs が無いと、第1象限にしか表示されなくなる
}
 
 float sdBox2( float3 p, float3 b )
{
  float3 d = abs(p) - b;
  return min(max(d.x,max(d.y,d.z)),0.0) +
         length(max(d,0.0));
}
 
float distanceFunction(float3 pos)
{
	//return length(trans(pos)) - 1.0;
	return sdBox2(trans(pos), float3(1, 1, 1));
}
 
float3 getNormal(float3 p)
{
  const float d = 0.0001;
  return
    normalize
    (
      float3
      (
        distanceFunction(p+float3(d,0.0,0.0))-distanceFunction(p+float3(-d,0.0,0.0)),
        distanceFunction(p+float3(0.0,d,0.0))-distanceFunction(p+float3(0.0,-d,0.0)),
        distanceFunction(p+float3(0.0,0.0,d))-distanceFunction(p+float3(0.0,0.0,-d))
      )
    );
}
float4 psDemo4(
    float2 inUV   : TEXCOORD0  ) : COLOR
{
	float2 resolution = g_ViewportSize / g_ViewportSize.y;//float2(g_ViewportSize.x / g_ViewportSize.y, g_ViewportSize.y / g_ViewportSize.y);	// (w, 1.0) へ (wはアス比としても使える)
	
	// 画面の左下が(-w, -1), 右上が(w, 1)となるような座標系を作る。(w は、画面が (640, 480)なら1.33になっている)
	// inUV は 0.0～1.0
	// pos は、今回の pixel はこの座標系上のどの位置かを示す。
	inUV.x *= resolution.x;
	inUV.y = 1.0 - inUV.y;	// DirectX の TexUV なので、上下を逆転させている
	float2 pos = (inUV.xy*2.0 - resolution) / resolution.y;
	
	// カメラの定義 (左手座標系としている。手前から奥を見ている)
	float3 cameraPos	= float3(1.0, 0.0, -50.0);
	float3 cameraFront	= float3(0.0, 0.0, 1.0);
	float3 cameraUp		= normalize(float3(0.2, 1.0, 0.0));
	float3 cameraRight	= cross(cameraUp, cameraFront);// float3(1.0, 0.0, 0.0);//
	float focus = 1.8;	// カメラと描画平面の距離
	
	float3 rayDir = normalize(cameraRight*pos.x + cameraUp*pos.y + cameraFront*focus);
	
	
	
	// ↓
	float t = 0.0, d;
	float3 posOnRay = cameraPos;	// レイの始点

	for(int i=0; i<32; ++i)
	{
		d = distanceFunction(posOnRay);//udBox(posOnRay, float3(1, 1, 1));//
		t += d;
		posOnRay = cameraPos + t*rayDir;
	}

	float4 c;
	if(abs(d) < 0.001){
		float3 normal = getNormal(posOnRay);
		d = dot(normalize(float3(1.0, 1.0, -0.5)), normal) + 0.5;	// 光源向を仮に1,1,1としている
		c = float4(1.0, 0.9, 0.3, 1.0);
		
		t = (g_Time % 4.0);
		float localY = posOnRay.y % 2.0;
		if (t <= localY && localY < t + 0.5)
			c = float4(0, 0, 0, 1.0);
		c.xyz *= d;
	}
	else{
		c = float4(1, 1, 1, 1);
	}
	
	return c;
}

//-------------------------------------------------------------------------
// technique
//-------------------------------------------------------------------------
technique MainDraw
{
    pass P0
    {
        VertexShader = compile vs_3_0 vsBasic();
        //PixelShader	 = compile ps_3_0 psBasic();
    	PixelShader	 = compile ps_3_0 psDemo4();
    }
}
