//==============================================================================
// SceneShader 
//------------------------------------------------------------------------------
///**
//  @file       SceneShader.h
//  @brief      SceneShader
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../Graphics/Interface.h"
#include "../../Graphics/DeviceObjects.h"
#include "../Common.h"
#include "MMETypes.h"
#include "SceneShaderManager.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// ■ SceneShaderUtil
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class SceneShaderUtil
{
public:

	/// テクスチャリソースの生成情報
    struct TextureResourceData
    {
        LRefTString         ResourceName;	// 基本的の相対パス
        LVector3            Dimensions;
        LVector2            ViewportRatio;
        int                 Miplevels;
        Graphics::SurfaceFormat		Format;

        // アニメーションテクスチャ用
        lnFloat             Offset;
        lnFloat             Speed;
        LRefTString         SeekVariable;

        // オフスクリーンRT用
        LVector4            ClearColor;
        lnFloat             ClearDepth;
        bool                AntiAlias;
        LRefTString         Description;
        LRefTString         DefaultEffect;
    };

public:

	//----------------------------------------------------------------------
	///**
	//  @brief      シェーダ変数のセマンティクスを調べて対応する要求項目を返す
	//
	//  @param[out] script_output_  : STANDARDSGLOBAL の ScriptOutput の値を格納する変数のアドレス (常に "color")
	//  @param[out] script_class_   : エフェクトファイルの使用目的 (デフォルトで "object")
	//  @param[out] script_order_   : エフェクトファイルの実行タイミング (デフォルトは "standard")            
	//  @param[out] is_controllobject_ : 有効な CONTROLOBJECT セマンティクスの場合、true が格納される
	//
	//  @note
	//              script_output_、script_class_、script_order_ は STANDARDSGLOBAL が
	//              ある場合のみ設定される。この関数の呼び出し側で sv_->Request == MME_VARREQ_STANDARDSGLOBAL と
	//              チェック後、変数の中身を確認すること。
	//*/
	//----------------------------------------------------------------------
    static void checkVariableRequest(
        Graphics::IShaderVariable* var_,    // const化したい
        const MMESemanticsMap& semantics_map_,
        MMEShaderVariable* sv_,
        MMEScriptOutput* script_output_,
        MMEScriptClass* script_class_,
        MMEScriptOrder* script_order_,
        bool* is_controllobject_ );

	/// 変数の "Object" アノテーションを調べて、"Light" の場合は fales、それ以外("Camera") は true を返す
    static bool checkAnnotationCameraOrLight( Graphics::IShaderVariable* var_ );

	/// 変数の "Object" アノテーションを調べて、"Geometry" の場合は 1、"Light" の場合は 2、それ以外は 0 を返す
    static int checkAnnotationGeometryOrLight( Graphics::IShaderVariable* var_ );

	/// 名前を指定してアノテーションを検索する (大文字小文字の区別無し。IShaderVariable からは大小小文字区別ありでしか取れないので用意)
    static Graphics::IShaderVariable* getAnnotationByName( Graphics::IShaderVariable* var_, const lnChar* name_ );

	/// 要求項目から、計算する必要のある行列マスクのビット列を作成して返す
    static lnU32 checkMatrixMask( MMEVariableRequest req_ );

	/// テクスチャリソースを作成する必要のある要求かを判定する
    static bool needCreateTexture( MMEVariableRequest req_ ) { return ( MME_VARREQ_TEXTURE <= req_ && req_ <= MME_VARREQ_OFFSCREENRENDERTARGET ); }

	//----------------------------------------------------------------------
	///**
	//  @brief      テクスチャリソースの情報を返す
	//  
	//  @return     テクスチャを作る必要がある場合 (変数のセマンティクスがテクスチャ系) true を返す
	//*/
	//----------------------------------------------------------------------
    static bool checkTextureResourceData(
        MMEShaderVariable* sv_,  // これも const にしたい…
        const MMEAnnotationMap& annotation_map_,
        TextureResourceData* data_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      テクスチャリソースの情報から、テクスチャサイズを返す
	//
	//  @param[in]  viewport_size_ : シーンの仮想スクリーンサイズ
	//*/
	//----------------------------------------------------------------------
    static void checkTextureSize(
        int* width_,
        int* height_,
        const LVector3& dimensions_,
        const LVector2& viewport_ratio_,
        const LVector2& viewport_size_ );

	//----------------------------------------------------------------------
	///**
	//  @brief      描画サブセットを示す文字列から、サブセット番号の配列を作成する
	//
	//  @note
	//              '-' による範囲指定で「x番以降すべて」を描画する場合、
	//              配列の終端には LN_INVALID_INDEX が格納される。
	//*/
	//----------------------------------------------------------------------
    static void makeSubsetIndexArray( SubsetIndexArray* list_, const lnChar* subset_text_ );

	/// CONTROLLOBJECT 設定 (obj は NULL 可能)
	static void setControllObjectParam( MMEShaderVariable* sv, const MMESceneObjectBase* obj );

private:

    static const int MAX_SEMANTICS_NAME_LEN = 256;

    struct SubsetIndexOrder
    {
        lnU32   Index;
        int     RangeState;    ///< 範囲の指定 (0=なし 1=つぎのSubsetIndexの値まで 2=サブセット数の最大まで)
    };

    typedef std::list< SubsetIndexOrder > SubsetIndexOrderList;

};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================