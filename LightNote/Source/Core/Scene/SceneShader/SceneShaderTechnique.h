//==============================================================================
// SceneShaderTechnique 
//------------------------------------------------------------------------------
///**
//  @file       SceneShaderTechnique.h
//  @brief      SceneShaderTechnique
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../Graphics/Interface.h"
#include "../../Graphics/DeviceObjects.h"
#include "../DrawCommand.h"
#include "MMETypes.h"
#include "SceneShader.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// ■ SceneShaderTechnique
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class SceneShaderTechnique
{
public:
    SceneShaderTechnique();
    virtual ~SceneShaderTechnique();

public:

	/// 初期化
    void initialize( SceneShader* shader, Graphics::IShaderTechnique* tech );

	/// このテクニックが有効であるかを調べる
	bool isValid() const { return mValid; }

	/// 描画パスを取得する
    MMDPass getMMDPass() const { return mMMDPass; }

	/// MMETechEnableFlag の組み合わせ
	lnU32 getMMETechEnableFlags() const { return mFlags; }

	/// スクリプトで "ScriptExternal=Color" があるかどうかを判定する
    bool isPostProcess() const { return ( mCommandExistFlags & MME_SCEXIST_ScriptExternal_Color ); }

	/// 特定のコマンドの存在を確認するためのフラグ
    lnU32 getCommandExistFlags() const { return mCommandExistFlags; }

	/// 描画コマンドリストの取得
	DrawingCommandList* getDrawingCommandList() { return &mDrawingCommandList; }

private:

	/// スクリプト文字列の変数を受け取って mScriptCommandList にコマンドを追加していく
	//      pass_ : パス内のスクリプトを解析する場合、そのパスを渡す (Draw="Geometry" の実行に渡すpass)
    bool _lexScriptCommandString( Graphics::IShaderVariable* var, Graphics::IShaderPass* pass, DrawingCommandList* commandList );

private:

    SceneShader*						mSceneShader;
    Graphics::IShader*					mCoreShader;
    Graphics::IShaderTechnique*			mTechnique;
    MMDPass								mMMDPass;
    lnU32								mFlags;					///< MMETechEnableFlag の組み合わせ
	lnU32								mCommandExistFlags;     ///< MMEScriptCommandExistFlag の組み合わせ

    SubsetIndexArray					mSubsetIndexArray;
	DrawingCommandList					mDrawingCommandList;			
	std::vector<DrawingCommandList*>	mPassDrawingCommandListArray;	///< パス毎のコマンド (mDrawingCommandList の子扱い)
	bool								mValid;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================