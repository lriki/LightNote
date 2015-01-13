//==============================================================================
// Types 
//------------------------------------------------------------------------------
///**
//  @file       Types.h
//  @brief      Types
//  @author     Riki
//  @note
//              Interface に混ぜてあったけど RenderState を別のヘッダに
//              分けたので、Include の問題を解決するために型定義もこっちに移動。
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <string>
#include <vector>

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
typedef std::vector< lnU32 > LNEffectedBoneIndices;
typedef std::vector< ln_std_tstring > LNFontNameList;

namespace Core
{
namespace Graphics
{
//class Bitmap;
}
}

/// グラフィックス API
enum LNGraphicsAPI
{
    LN_GRAPHICSAPI_DIRECTX9 = 0,    ///< DirectX9
    LN_GRAPHICSAPI_DIRECTX11,       ///< DirectX11
    LN_GRAPHICSAPI_OPENGL,          ///< OpenGL

	GraphicsAPI_Max,
};

/// テクスチャの種類
enum LNTextureType
{
    LN_TEXTURETYPE_UNKNOWN = 0,     
	LN_TEXTURETYPE_NORMAL,          ///< テクスチャ
    LN_TEXTURETYPE_RENDERTARGET,    ///< レンダーターゲット
    LN_TEXTURETYPE_DEPTH,           ///< 深度バッファ

	LN_TEXTURETYPE_INVALID = 99,
};



/// メンバのデータのサイズを定義するデータ型
enum LNDeclType
{
	LN_DECL_UNKNOWN = 0,
	LN_DECL_FLOAT1,
	LN_DECL_FLOAT2,
	LN_DECL_FLOAT3,
	LN_DECL_FLOAT4,
	LN_DECL_UBYTE4, 
	LN_DECL_COLOR4,     ///< 32ビット色コード (A8R8G8B8 の順。LN_DECL_UBYTE4 の場合、エンディアンは考慮されないので注意)
	LN_DECL_SHORT2,
	LN_DECL_SHORT4,

    LN_DECL_MAX
};

/// データの使用方法
enum LNDeclUsage
{
	LN_USAGE_UNKNOWN = 0,
	LN_USAGE_POSITION,
	LN_USAGE_NORMAL,
	LN_USAGE_COLOR,
	LN_USAGE_TEXCOORD,
	LN_USAGE_PSIZE,
    LN_USAGE_BLENDINDICES,
    LN_USAGE_BLENDWEIGHT,

	//TODO: Tangent 等、MME 頂点以外も

	LN_USAGE_MAX,
};

#define LN_DECL_END() { 0, LN_DECL_UNKNOWN, LN_USAGE_UNKNOWN, 0 }

/// グラフィックスパイプラインへの入力頂点データを扱う構造体
struct LNVertexElemenst
{
    lnU32             StreamIndex;    ///< ストリーム番号
	LNDeclType	    Type;
	LNDeclUsage	    Usage;
	lnU8			    UsageIndex;     ///< OpenGL で許可しているのは TEXCOORD0～8、PSIZE15 のみ。それ以外は 0
};

static const int LNUsageIndexMax = 16;

/// シェーダ変数のオブジェクトタイプ
enum LNShaderVariableClass
{
    LN_SVC_UNKNOWN = 0,
    LN_SVC_SCALAR,      ///< [GL] 
    LN_SVC_VECTOR,      ///< [GL] 
    LN_SVC_MATRIX,      ///< [GL] 
    LN_SVC_STRUCT,
    LN_SVC_ARRAY,       ///< TODO: DX9 ではどう認識するのかチェックしておく。今は未使用
    LN_SVC_SAMPLER,     ///< [GL] 
    LN_SVC_OBJECT,      ///< DX9のテクスチャ、サンプラ型はこれ
    LN_SVC_BUFFER,

	LN_SVC_MAX,
};

/// シェーダ変数の型
enum LNShaderVariableType	// InternalShaderVariableType とかにしておく。この enum は隠ぺいして、ShaderVariableType を使うようにする
{
    LN_SVT_UNKNOWN = 0,         ///<  [GL] 
    LN_SVT_ARRAY,
    LN_SVT_STRING,              ///< [DX9] [GL ※アノテーションのみ]
    LN_SVT_STRUCT,
    LN_SVT_TYPELESS_STRUCT,
    LN_SVT_BUFFER,
    LN_SVT_UNIFORMBUFFER,
    LN_SVT_ADDRESS,
    LN_SVT_PROGRAM_TYPE,
    LN_SVT_PIXELSHADER,         ///< [DX9]
    LN_SVT_VERTEXSHADER,        ///< [DX9]
    LN_SVT_TEXTURE,             ///< [DX9]
    LN_SVT_TEXTURE1D,           ///< [DX9]
    LN_SVT_TEXTURE2D,           ///< [DX9]
    LN_SVT_TEXTURE3D,           ///< [DX9]
    LN_SVT_TEXTURECUBE,         ///< [DX9]
    LN_SVT_SAMPLER,             ///< [DX9] 
    LN_SVT_SAMPLER1D,           ///< [DX9] [GL] DX9 に合わせて追加
    LN_SVT_SAMPLER2D,           ///< [DX9] [GL] 
    LN_SVT_SAMPLER3D,           ///< [DX9] [GL] 
    LN_SVT_SAMPLERCUBE,         ///< [DX9] [GL] 
    LN_SVT_ENUM,
    LN_SVT_VOID,                ///< [DX9] Cg にはないかも？
    LN_SVT_BOOL,                ///< [DX9] [GL]
    LN_SVT_CHAR,
    LN_SVT_DOUBLE,
    LN_SVT_FIXED,
    LN_SVT_FLOAT,               ///< [DX9] [GL]
    LN_SVT_HALF,
    LN_SVT_INT,                 ///< [DX9] [GL]
    LN_SVT_LONG,
    LN_SVT_SHORT,
    LN_SVT_UCHAR,
    LN_SVT_UINT,
    LN_SVT_ULONG,
    LN_SVT_USHORT,
	
	LN_SVT_MAX,

    LN_SVT_TEXTURE_BEGIN = LN_SVT_TEXTURE,
    LN_SVT_TEXTURE_END   = LN_SVT_TEXTURECUBE,


	// Texture 系は Cg では認識しないかったかも。
	// とりあえず DirectX9 に合わせておく。
	// 変更外必要な場合は Shader_dx.cpp の
	// ShaderVariable コンストラクタをいじる感じで。
	//

	// D3DXPT_PIXELFRAGMENT、D3DXPT_VERTEXFRAGMENT はカット

    /*
    LN_SVT_VOID,      ///< void ポインタ
    LN_SVT_BOOL,
    LN_SVT_INT,
    LN_SVT_FLOAT,
    LN_SVT_STRING,
    LN_SVT_TEXTURE,
    LN_SVT_TEXTURE1D,
    LN_SVT_TEXTURE2D,
    LN_SVT_TEXTURE3D,
    LN_SVT_TEXTURECUBE,
    */


    /*
    LN_SVT_SAMPLER,
    LN_SVT_SAMPLER1D,
    LN_SVT_SAMPLER2D,
    LN_SVT_SAMPLER3D,
    LN_SVT_SAMPLERCUBE,
    LN_SVT_PIXELSHADER,
    LN_SVT_VERTEXSHADER,
    LN_SVT_PIXELFRAGMENT,
    LN_SVT_VERTEXFRAGMENT,
    LN_SVT_UNSUPPORTED,
    */
	
};

/// シェーダプログラムのコンパイル結果
enum LNShaderCompileResult
{
    LN_SHADERRESULT_SUCCESS = 0,	///< 成功
    LN_SHADERRESULT_WARNING,		///< 警告が発生している (実行は可能)
    LN_SHADERRESULT_ERROR,			///< エラーが発生している

	// ※値の大きい方が重大
};

/// シェーダプログラムの変数の型情報 (DX9 では型の名前はとれないみたい)
struct LNShaderVariableTypeDesc
{
    LNShaderVariableClass   Class;
    LNShaderVariableType    Type;
    lnU32                   Rows;       ///< 行の数 (行列型以外は 1)
    lnU32                   Columns;    ///< 列の数
    lnU32                   Elements;   ///< 配列の要素数 (配列でない場合は 0) TODO:OpenGLでは1…調査必要
    bool                    Shared;     ///< shared キーワードがついている
};

/// シェーダプログラムの情報
struct LNShaderDesc
{
    lnU32           ParameterNum;       ///< パラメータの数
    lnU32           TechniqueNum;       ///< テクニックの数
};

/// ビットマップのピクセルフォーマット
enum LNBitmapFormat
{
    LN_BITMAPFORMAT_UNKNOWN = 0,
    LN_BITMAPFORMAT_A1,				///< モノクロ
    LN_BITMAPFORMAT_A8,				///< グレースケール
    LN_BITMAPFORMAT_A8R8G8B8,		///< AA RR GG BB (U32値。リトルエンディアン環境での実際のメモリ上は BGRA)
	LN_BITMAPFORMAT_A8B8G8R8,		///< AA BB GG RR (png からそのまま読んだ値など。)
};
#if 0
/// フォントグリフビットマップのピクセルフォーマット
enum GlyphPixelMode
{
    LN_GLYPHPIXELMODE_NONE = 0,
    LN_GLYPHPIXELMODE_MONO,         ///< 1 ピクセル = 1 ビット
    LN_GLYPHPIXELMODE_GRAY,         ///< 1 ピクセル = 8 ビット
};

/// フォントのグリフを表す構造体 (旧。削除予定)
struct LNGlyphOutlineData
{
    lnU8*   Bitmap;         ///< 1文字分のビットマップが格納されるバッファ
    lnU32   BitmapWidth;    ///< Bitmap の幅 (幅と高さは1文字分がぴっちり入るサイズ)
    lnU32   BitmapHeight;   ///< Bitmap の高さ

    lnU32   GlyphOffsetX;   ///< 描画領域の左上から、ビットマップを転送するべき座標へのXオフセット
    lnU32   GlyphOffsetY;   ///< 描画領域の左上から、ビットマップを転送するべき座標へのYオフセット
    lnU32   GlyphWidth;     ///< Bitmap 内の文字がある部分の幅
    lnU32   GlyphHeight;    ///< Bitmap 内の文字がある部分の高さ
    lnU32   CellWidth;      ///< 文字領域の幅 (次の文字を描画するときはこの値分だけピクセルを進めるようにする)

    lnU32   CharIdxOffset;  ///< IFont::getGlyphOutline() によって解析した文字がマルチバイト文字だった場合は 2、シングルバイトまたは Unicode(wchar_t)の場合は 1
    bool    BitmapIsARGB;   ///< true の場合は Bitmap を lnU32* にキャストして使うこと (true の場合、Bitmap は lnU8 ではなく lnU32 として確保されている)
};

/// フォントのグリフを表す構造体 (旧。削除予定)
struct LNFontGlyphData
{
    Core::Graphics::Bitmap* Bitmap;
    int             GlyphOffsetX;   ///< ビットマップを転送するべき座標へのXオフセット
    int             GlyphOffsetY;   ///< ビットマップを転送するべき座標へのYオフセット

	//int             BitmapRealDataSize;    ///< 内部データ
    int             NextBaseX;      ///< 内部データ
    int             NextBaseY;      ///< 内部データ
    lnUInt          Previous;       ///< 内部データ

    /*
    lnByte*         Bitmap;         ///< 1文字分のビットマップが格納されるバッファ
    int             Width;          ///< グリフの幅 (px)
    int             Height;         ///< グリフの高さ (px)
    int             Pitch;          ///< ビットマップの横幅 (バイト数。モノクロビットマップで使用する)
    LNBitmapFormat  Format;         ///< ピクセルフォーマット

    int             GlyphOffsetX;   ///< ビットマップを転送するべき座標へのXオフセット
    int             GlyphOffsetY;   ///< ビットマップを転送するべき座標へのYオフセット

    bool            UpFlow;         ///< ビットマップデータの上下反転フラグ

    int             NextBaseX;      ///< 内部データ
    int             NextBaseY;      ///< 内部データ
    void*           InternalGlyph;  ///< 内部データ FT_BitmapGlyph
    lnUInt          Previous;       ///< 内部データ
    bool            UsedCopy;       ///< 内部データ
    */
};
#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

/// 3D 空間での基準方向を表す値
enum LNAxisAlignedDir
{
    LN_AADIR_X = 0,   ///< X+ 方向 (右向き)
	LN_AADIR_Y,       ///< Y+ 方向 (上向き)
	LN_AADIR_Z,       ///< Z+ 方向 (奥向き)
    LN_AADIR_RX,      ///< X- 方向 (左向き)
	LN_AADIR_RY,      ///< Y- 方向 (下向き)
	LN_AADIR_RZ,      ///< Z- 方向 (手前向き)

	LN_AADIR_MAX,
};

/// スプライトのソート方法
enum LNSpriteSortMode
{
    LN_SPRITESORT_NONE                = 0x00,   ///< ソートしない
    LN_SPRITESORT_DEPTH_BACK_TO_FRONT = 0x01,   ///< Z値が大きいものが先に描画されるようにソートする (アルファブレンド使用時)
    LN_SPRITESORT_DEPTH_FRONT_TO_BACK = 0x02,   ///< Z値が小さいものが先に描画されるようにソートする (LN_SPRITESORT_DEPTH_BACK_TO_FRONT と同時には使えない)
    LN_SPRITESORT_TEXTURE             = 0x04    ///< テクスチャを優先してソートする (同じテクスチャを持つスプライトが多数あるとき用。ただし、アルファブレンドが有効な場合は描画が不自然になることがある)
};

/// ビルボードの計算方法
enum LNBillboardType
{
    LN_BILLBOARD_NOT = 0,   ///< ビルボードの計算を行わない
    LN_BILLBOARD_FRONT,     ///< カメラ (ビュー行列) に対して正面を向く
    LN_BILLBOARD_ROT_Y,     ///< Y 軸回転のみ行う

	LN_BILLBOARD_MAX,
};

/// メッシュの属性
struct LNMeshAttribute
{
    lnU32     MaterialIndex;  ///< 対応するマテリアル番号
    lnU32     StartIndex;     ///< 開始インデックス
    lnU32     PrimitiveNum;   ///< 描画プリミティブ数

    LNEffectedBoneIndices   BoneIndices;    ///< (いらないかも)GPU スキニングにおいて、この属性のサブセットが影響を受けるボーン番号の配列
};

/// グリッドを表示するかどうかの指定
enum LNGridVisible
{
    LN_GRID_XZ      = 0x01,     ///< XZ 平面に描画する
    LN_GRID_XY      = 0x02,     ///< XY 平面に描画する
    LN_GRID_YZ      = 0x04,     ///< YZ 平面に描画する
    LN_GRID_AXIS    = 0x08      ///< 軸を描画する
};

/// グリッドの描画範囲
enum LNGridPlaceRange
{
    LN_GRIDPLACE_PLUS = 0,      ///< +方向にだけ描画する
    LN_GRIDPLACE_MINUS,         ///< -方向にだけ描画する
    LN_GRIDPLACE_PLUS_MINUS     ///< +-両方に描画する
};





//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

namespace LNote
{
namespace Core
{
namespace Graphics
{
class Bitmap;

/// プリミティブ
enum PrimitiveType
{
	PrimitiveType_TriangleList = 1,
	PrimitiveType_TriangleStrip,
	PrimitiveType_LineList,
	PrimitiveType_LineStrip,
	PrimitiveType_PointList,
};

/// 合成方法
enum BlendMode
{
	BlendMode_Normal = 0,        ///< 通常 ( アルファ無視 )
	BlendMode_Alpha,             ///< アルファブレンド
	BlendMode_Add,               ///< 加算
	BlendMode_AddAlphaDisable,	///< 加算（アルファチャンネル無視）
	BlendMode_Sub,				///< 減算
	BlendMode_SubAlphaDisable,	///< 減算（アルファチャンネル無視）
	BlendMode_Mul,				///< 乗算
	BlendMode_Screen,			///< スクリーン
	BlendMode_Reverse,           ///< リバース

	BlendMode_Max,
};

/// カリング方法
enum CullingMode
{
	CullingMode_Front = 0,          ///< 前面表示  (D3DCULL_CCW)
	CullingMode_Back,               ///< 裏面表示
	CullingMode_Double,             ///< 両面表示

	CullingMode_Max,
};

/// カリング方法
/* 手右座標系スタイル (右手系の場合はD3DCULL_CWがデフォ)
enum LNCullingMode
{
LN_CULL_BACK = 0,           ///< 裏面表示
LN_CULL_FRONT,              ///< 前面表示
LN_CULL_DOUBLE              ///< 両面表示
};
*/

/// 塗りつぶし方法
enum FillMode
{
	FillMode_Solid = 0,	    ///< 面を塗りつぶす
	FillMode_Wire,	        ///< ワイヤーフレーム
	FillMode_Point,	        ///< 頂点
};

/// グラフィックスパイプラインへの入力頂点データを扱う構造体
struct GraphicsCache
{
	lnU32			StreamIndex;    ///< ストリーム番号
	LNDeclType		Type;
	LNDeclUsage		Usage;
	lnU8			UsageIndex;
};

/// テクスチャフィルタ
enum TextureFilter
{
	TextureFilter_None = 0,
	TextureFilter_Point,
	TextureFilter_Linear,
};

/// テクスチャアドレッシング
enum TextureAddress
{
	TextureAddress_Repeat = 0,    ///< 繰り返し
	TextureAddress_Clamp,         ///< 境界色が引き延ばされる
};

/// デバイスの状態
enum DeviceState
{
	DeviceState_Enabled = 0,	///< 描画可能
	DeviceState_Lost,			///< 消失中。直ちに resetDevice() の必要がある
	DeviceState_Pausing,		///< アプリが非アクティブになった等 (直ちに resetDevice() は必要ない)
};

/// サーフェイスフォーマット
enum SurfaceFormat
{
    SurfaceFormat_Unknown = 0,
    SurfaceFormat_A8R8G8B8,            ///< 32 ビットのアルファ付きフォーマット (lnU32 アクセス時の表現。lnByte[4] にすると、ABGR)
    SurfaceFormat_X8R8G8B8,            ///< 32 ビットのアルファ無しフォーマット
    SurfaceFormat_A16B16G16R16F,       ///< 64 ビットの浮動小数点フォーマット
    SurfaceFormat_A32B32G32R32F,       ///< 128 ビットの浮動小数点フォーマット
    SurfaceFormat_D24S8,               ///< 32 ビットの Z バッファフォーマット

	// 以下は今のところDirectX限定
	SurfaceFormat_R16F,
	SurfaceFormat_R32F,
};

/// Canvas のリサイズモード
enum BackbufferResizeMode
{
	BackbufferResizeMode_Scaling = 0,				///< バックバッファは初期化時のサイズのまま、ウィンドウサイズに合わせて拡縮を行う (ぼやける)
	BackbufferResizeMode_ScalingWithLetterBox,		///< バックバッファは初期化時のサイズのまま、レターボックスを表示してアスペクト比を保ち拡縮を行う
	BackbufferResizeMode_Resize,					///< バックバッファをウィンドウサイズに合わせてリサイズする
	//BackbufferResizeMode_ResizeWithLetterBox,		///< バックバッファをウィンドウサイズに合わせて、レターボックスを表示してアスペクト比を保つようにリサイズする

	BackbufferResizeMode_Max,
};

/// テキストアラインの記述
enum TextAlign
{
	TextAlign_Left = 0,		///< 左揃え
	TextAlign_Center,		///< 中央揃え
	TextAlign_Right,		///< 右揃え
};

/// シェーダ変数の型
enum ShaderVariableType
{
	SVT_UNKNOWN = 0,
	SVT_BOOL,
	SVT_INT,
	SVT_FLOAT,
	SVT_VECTOR,
	SVT_VECTORARRAY,
	SVT_MATRIX,
	SVT_MATRIXARRAY,
	SVT_TEXTURE,
	//SVT_SAMPLER,
	SVT_STRING,
};



/// モデル生成オプション
enum ModelCreateFlag
{
    ModelCreateFlag_SoftwareSkinning 		= 0x01,		///< ソフトウェアスキニング用のモデル (動的な頂点、インデックスバッファを作成)
    ModelCreateFlag_CollisionMesh			= 0x02,		///< 物理演算のメッシュボディとして扱うメッシュを生成する
	ModelCreateFlag_IgnoreTextureNotFound	= 0x04,		///< マテリアルに含まれるテクスチャファイルが見つからなくても無視する
	ModelCreateFlag_CopyMaterials			= 0x08,		///< マテリアルを共有せずにコピーする

	ModelCreateFlag_Max,
};

/// カメラの座標変換方法
enum CameraType
{
	CameraType_PositionAndTarget	= 0,	///< (2Dカメラでは無効)
	CameraType_PositionAndAngle,			///< (2D カメラのデフォルト)
	CameraType_TransformMatrix,				/// ()

	CameraType_Max,
};

struct FontGlyphData
{
	Bitmap*			GlyphBitmap;
	Bitmap*			OutlineBitmap;	///< 縁どり線用 (NULL でなければ先にこれをbltし、その上に GlyphBitmap を blt する)
	int				GlyphOffsetX;   ///< ビットマップを転送するべき座標へのXオフセット
	int				GlyphOffsetY;   ///< ビットマップを転送するべき座標へのYオフセット
	int				OutlineOffset;	///< 縁どり時は、線の分だけサイズが大きくなる。

	int				MetricsHeight;	///< 行高さ

	/* 描画先座標は GlyphOffsetX, GlyphOffsetY に格納されている。
	* これは makeGlyphData() を呼ぶたびに送り方向へ加算されていくため、
	* 描画側は座標を加算し続ける必要はない。
	* 常に 描画領域の左上 + GlyphOffsetX(Y) でOK。
	*
	* GlyphOffsetX, GlyphOffsetY は、アウトラインの有無にかかわらず常に同じ位置を指す。
	* アウトラインがある場合は この位置を起点に書いた後、OutlineOffset を加算した場所に 通常グリフを blt する。
	*/

	
};

} // namespace Graphics
} // namespace Core
} // namespace LNote
