#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
//
//   fxc /Zpc /Tvs_2_0 /EVS /Fh Shader/EffekseerRenderer.Standard_VS.h
//    Shader/standard_renderer_VS.fx
//
//
// Parameters:
//
//   float4x4 mCameraProj;
//
//
// Registers:
//
//   Name         Reg   Size
//   ------------ ----- ----
//   mCameraProj  c0       4
//

    vs_2_0
    dcl_position v0
    dcl_normal v1
    dcl_texcoord v2
    mul r0, v0.y, c1
    mad r0, c0, v0.x, r0
    mad r0, c2, v0.z, r0
    add oPos, r0, c3
    mov oD0, v1
    mov oT0.xy, v2

// approximately 6 instruction slots used
#endif

const BYTE g_vs20_VS[] =
{
      0,   2, 254, 255, 254, 255, 
     34,   0,  67,  84,  65,  66, 
     28,   0,   0,   0,  83,   0, 
      0,   0,   0,   2, 254, 255, 
      1,   0,   0,   0,  28,   0, 
      0,   0,  16,   1,   0,   0, 
     76,   0,   0,   0,  48,   0, 
      0,   0,   2,   0,   0,   0, 
      4,   0,   2,   0,  60,   0, 
      0,   0,   0,   0,   0,   0, 
    109,  67,  97, 109, 101, 114, 
     97,  80, 114, 111, 106,   0, 
      3,   0,   3,   0,   4,   0, 
      4,   0,   1,   0,   0,   0, 
      0,   0,   0,   0, 118, 115, 
     95,  50,  95,  48,   0,  77, 
    105,  99, 114, 111, 115, 111, 
    102, 116,  32,  40,  82,  41, 
     32,  72,  76,  83,  76,  32, 
     83, 104,  97, 100, 101, 114, 
     32,  67, 111, 109, 112, 105, 
    108, 101, 114,  32,  57,  46, 
     50,  57,  46,  57,  53,  50, 
     46,  51,  49,  49,  49,   0, 
     31,   0,   0,   2,   0,   0, 
      0, 128,   0,   0,  15, 144, 
     31,   0,   0,   2,   3,   0, 
      0, 128,   1,   0,  15, 144, 
     31,   0,   0,   2,   5,   0, 
      0, 128,   2,   0,  15, 144, 
      5,   0,   0,   3,   0,   0, 
     15, 128,   0,   0,  85, 144, 
      1,   0, 228, 160,   4,   0, 
      0,   4,   0,   0,  15, 128, 
      0,   0, 228, 160,   0,   0, 
      0, 144,   0,   0, 228, 128, 
      4,   0,   0,   4,   0,   0, 
     15, 128,   2,   0, 228, 160, 
      0,   0, 170, 144,   0,   0, 
    228, 128,   2,   0,   0,   3, 
      0,   0,  15, 192,   0,   0, 
    228, 128,   3,   0, 228, 160, 
      1,   0,   0,   2,   0,   0, 
     15, 208,   1,   0, 228, 144, 
      1,   0,   0,   2,   0,   0, 
      3, 224,   2,   0, 228, 144, 
    255, 255,   0,   0
};
