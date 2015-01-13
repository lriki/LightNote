//==============================================================================
// FontManagerBase 
//------------------------------------------------------------------------------
///**
//  @file       FontManagerBase.h
//  @brief      FontManagerBase
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../Interface.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

// TODO: もう FreeType しか使う予定ないので、統合しておいてもいいと思う
//==============================================================================
// ■ FontManagerBase
//------------------------------------------------------------------------------
///**
//  @brief    
//*/
//==============================================================================
class FontManagerBase
    : public Base::ReferenceObject
{
public:
    FontManagerBase();
    virtual ~FontManagerBase();

public:

	/// 初期化
    void initialize( Font* defaultFont );

	/// 終了処理
    virtual void finalize();

	/// フォントの作成 (フォント名が NULL の場合はデフォルトの設定で作成する)
    virtual Font* createFont( const lnChar* fontFilePath );

	/// ビットマップフォントの作成
	virtual Font* createBitmapFont(Texture* texture) = 0;

	/// デフォルトのフォントをセットする
    void setDefaultFont( Font* font );

	/// デフォルトのフォントを取得する
    Font* getDefaultFont();

public:

	/// フォントファイルを追加する (ttf)
    virtual void addFontFile( const lnChar* fontname ) = 0;

	/// フォント作成
    virtual Font* createFont() = 0;

protected:

    Font*  mDefaultFont;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================