//==============================================================================
// Resource 
//==============================================================================

#pragma once

#include "../FileIO/StreamObject.h"

namespace LNote
{
namespace Core
{
namespace Resource
{

//==============================================================================
// ■ StaticResourceManager
//------------------------------------------------------------------------------
///**
//	@brief      
//*/
//==============================================================================
class StaticResourceManager
{
public:

	/// 文字列リソースの取得
	static const lnChar* getString( int resourceID );
};

/// 文字列リソース
namespace String
{

enum
{
	WARNING_Audio_DirectXIsNotInstalled = 1,	/// XAudio の初期化に失敗しました。\nDirectX がインストールされていない可能性があります。\n音声無しで続行します。

	String_MAX,
};



    

    static const lnChar* ERR_FileArchive1           = _T( "#アーカイブファイルを開くことができませんでした。" );
    static const lnChar* ERR_FileArchive2           = _T( "#不正なアーカイブファイルを開こうとしました。" );
    static const lnChar* ERR_FileArchive3           = _T( "#アーカイブにを開くためのキーが不正です。" );

    static const lnChar* ERR_SceneViewport          = _T( "指定されたシェーダはプリプロセスまたはポストプロセス用ではありません。" );
    static const lnChar* ERR_SceneShader_onSettingShader    = _T( "共有できないシェーダが設定されようとしました。" );
    static const lnChar* ERR_SceneShader_build1     = _T( "オブジェクトを描画するエフェクトに \"Draw=Buffer\" コマンドが見つかりました。" );
    static const lnChar* ERR_SceneShader_build2     = _T( "スクリーンバッファを描画するエフェクトに \"Draw=Geometry\" コマンドが見つかりました。" );
    static const lnChar* ERR_SceneShader_build3     = _T( "ポストエフェクト用のエフェクトに \"ScriptExternal=Color\" コマンドのないテクニックが見つかりました。" );
    static const lnChar* ERR_SceneShader_build4     = _T( "ポストエフェクト用ではないエフェクトに \"ScriptExternal=Color\" コマンドのあるテクニックが見つかりました。" );

	static const lnChar* ERR_DirectMusicNotInit		= _T( "DirectMusic が初期化されていません。" );

	static const lnChar* ERR_FailedDirect3DLoadLibs = _T("DirectX の DLL の読み込みに失敗しました。\nDirectX が正しくインストールされていない可能性があります。");
	static const lnChar* ERR_FailedDirect3DInit		= _T("Direct3D の初期化に失敗しました。\nDirectX が正しくインストールされていないか、\nお使いのビデオカードのドライバーが対応していない可能性があります。");
	static const lnChar* ERR_NotFoundValidGraphicsDevice	= _T("使用できるグラフィックデバイスが見つかりませんでした。");
	static const lnChar* ERR_InvalidVertexShaderVersion		= _T("お使いの環境は頂点シェーダ2.0に対応していません。");
	static const lnChar* ERR_InvalidPixelShaderVersion		= _T("お使いの環境はピクセルシェーダ2.0に対応していません。");

	static const lnChar* ERR_FailedDXInputInit		= _T("XInput の DLL の読み込みに失敗しました。\nDirectX が正しくインストールされていないか、\n古いバージョンである可能性があります。");

}

/// シェーダプログラムリソース
//namespace Shader
//{
//    /// 基本形状描画用
//    extern const unsigned char GeometryRenderer[];
//    extern const unsigned int  GeometryRenderer_Size;
//
//    /// スプライト描画用
//    extern const unsigned char SpriteRenderer[];
//    extern const unsigned int  SpriteRenderer_Size;
//
//    /// SceneShader 2D 基本
//    extern const unsigned char SSBasic2D[];
//    extern const unsigned int  SSBasic2D_Size;
//
//    /// SceneShader 3D 基本
//    extern const unsigned char SSBasic3D[];
//    extern const unsigned int  SSBasic3D_Size;
//
//    /// SceneShader ライティングなし
//    extern const unsigned char SSNoLighting[];
//    extern const unsigned int  SSNoLighting_Size;
//
//    
//}

///// OpenGL シェーダ
//namespace GLSL
//{
//    /// 基本形状描画用
//    extern const unsigned char GeometryRenderer[];
//    extern const unsigned int  GeometryRenderer_Size;
//}

/// 画像リソース
namespace Image
{
   
}

//#define LN_RESOURCE_ROOT_PATH_W			L"LNResource"
//#define LN_RESOURCE_ROOT_PATH_LENGTH	10
//
///// 埋め込みリソースをストリームとして返すクラス
//class HardCodeResourceArchive
//    //: public Base::ReferenceObject
//    //, public FileIO::IArchive
//{
//public:
//    //HardCodeResourceArchive();
//    //~HardCodeResourceArchive();
//   // LN_REFOBJ_METHODS;
//
//public:
//
//    /// 入力ストリームの作成
//    static FileIO::IInStream* createStream( const wchar_t* filePath, FileIO::FileOpenMode mode );
//
//	/// パスの先頭がリソースディレクトリ(LNResource)かをチェック
//	static bool checkResourceRootPath( const wchar_t* path );
//};



enum DataResourceID
{
	ID_Unknown = 0,

	ID_Shader_MMM_EffectHeader,
	ID_Shader_GeometryRenderer,
	ID_Shader_SpriteRenderer,
	ID_Shader_SSBasic2D,
	ID_Shader_SSBasic3D,
	ID_Shader_SSBasic3DSkinning,
	ID_Shader_SSNoLighting,
	ID_Shader_ImageFilter_Tone,
	ID_Shader_ImageFilter_TransitionWithoutMask,
	ID_Shader_ImageFilter_TransitionWithMask,
	ID_Shader_ImageFilter_BufferingBlar,

	ID_Shader_BasicPane_Transition,
	ID_Shader_BasicPane_TransitionWithMask,
	ID_Shader_BasicPane_BufferingBlar,
	ID_Shader_BasicPane_PostDraw,
	ID_Shader_PointParticle,
	ID_Shader_Tilemap,

	ID_Image_DebugFont,
	ID_Image_GUISkin,
	ID_Image_toon01,
	ID_Image_toon02,
	ID_Image_toon03,
	ID_Image_toon04,
	ID_Image_toon05,
	ID_Image_toon06,
	ID_Image_toon07,
	ID_Image_toon08,
	ID_Image_toon09,
	ID_Image_toon10,
	ID_Image_BitmapFont1_h7_6x15,

	ID_GUI_BasicTheme,
	ID_GUI_Window,
};

//==============================================================================
// StaticResource
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class StaticResource	// TODO: Core::Manager を再配置可能とかにするなら、シングルトンにするなり対応が必要
{
public:
	/// 初期化
	static void initialize();

	/// 終了処理
	static void finalize();

	/// データへのポインタと、データサイズを取得する
	static void loadData( int dataResourceID, const void** data, size_t* byteCount );

	/// (ユーティリティ)loadData() で取得したのデータポインタをストリーム化して返す
	static FileIO::Stream* createStream( int dataResourceID );
};



} // namespace Resource
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================