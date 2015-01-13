//==============================================================================
// DrawCommand 
//------------------------------------------------------------------------------
///**
//  @file       DrawCommand.h
//  @brief      DrawCommand
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <stack>
#include <vector>
#include "Common.h"
#include "SceneShader/MMETypes.h"
#include "../Graphics/Interface.h"
#include "../Graphics/DeviceObjects.h"


namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// ■ DrawingCommandList
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class DrawingCommandList
{
public:
	DrawingCommandList();
	~DrawingCommandList();

public:

	/// 初期化
	void initialize( SceneShader* ownerShader );

	/// Draw=Geometry にて描画される SceneNode を設定する
	/// (Pass= によって設定されているコマンドリストに対しても設定される)
	void setOwnerVisualNodeContext( VisualNodeContext* context );

	/// 描画するサブセット番号の配列を設定する
	/// (Pass= によって設定されているコマンドリストに対しても設定される)
	void setDrawingSubsetNumbers( const std::vector<int>& numbers );

	/// このコマンドリストを実行する
	void execute( ContextDrawParam& param );

	/// ScriptExternal=Color まで実行する
	void executePreScriptExternal_Color( ContextDrawParam& param );

	/// ScriptExternal=Color から実行する
	void executePostScriptExternal_Color( ContextDrawParam& param );

	/// スクリプトが正しいかを確認する
	bool checkValid( MMEScriptClass scriptClass, MMEScriptOrder scriptOrder, bool onTech );

public:

	/// レンダリングターゲットを設定する
	/// (texture を NULL にすると、デフォルト(コマンド開始時)にリセット)
	//void add_RenderColorTarget( int index, Graphics::ITexture* texture );

	/// レンダリングターゲットを設定する
	/// (textureVariable を NULL にすると、デフォルト(コマンド開始時)にリセット)
	void add_RenderColorTarget( int index, Graphics::IShaderVariable* textureVariable );

	/// 深度バッファを設定する
	/// (texture を NULL にすると、デフォルト(コマンド開始時)にリセット)
	//void add_RenderDepthStencilTarget( Graphics::ITexture* texture );

	/// レンダリングターゲットを設定する
	/// (textureVariable を NULL にすると、デフォルト(コマンド開始時)にリセット)
	void add_RenderDepthStencilTarget( Graphics::IShaderVariable* textureVariable );

	/// レンダリングターゲットをクリアする色を設定する
	void add_ClearSetColor( const LVector4& color );		// TODO: このあたり、Graphics::IShaderVariable から getVector するように変更

	/// 深度バッファをクリアするZ値を設定する
	void add_ClearSetDepth( lnFloat depth );

	/// レンダリングターゲットをクリアする
	void add_ClearColor();

	/// 深度ステンシルサーフェイスをクリアする
	void add_ClearDepth();

	/// ScriptExternal=Color
	void add_ScriptExternal_Color();

	/// 指定したパスを実行する 
	/// (他の描画コマンドを実行する。commandList を NULL にすると、Draw=Geometry のみを実行する)
	void add_Pass( Graphics::IShaderPass* pass, DrawingCommandList* commandList );

	/// 指定した回数だけ、スクリプトの一部をループする
	void add_LoopByCount( int count );

	/// ループ終端
	void add_LoopEnd();

	/// ループ中のループカウンタの値を、指定したパラメータに設定する
	void add_LoopGetIndex( Graphics::IShaderVariable* variable );

	/// オブジェクトを描画する
	void add_DrawGeometry( Graphics::IShaderPass* pass );

	/// レンダリングターゲットのスクリーンと一致するサイズの、長方形のポリゴンを描画する
	void add_DrawBuffer( Graphics::IShaderPass* pass );

private:

	/// このコマンドリストを実行する (戻り値は最後に実行したコマンドを指すプログラムカウンタ)
	int _executeInternal( ContextDrawParam& param, int programCounter, int currentLoopCount );

	/// Draw=Geometry
	void _drawGeometry( ContextDrawParam& param, Graphics::IShaderPass* pass );

	/// 現在のレンダリングターゲット等の状態を記憶
	void _pushCurrentState( ContextDrawParam& param );

	/// 記憶した状態を復帰
	void _popCurrentState( ContextDrawParam& param );

private:

	/// 描画コマンドの種類
	enum CommandType
	{
		COMMAND_Unknown = 0,
		COMMAND_RenderColorTarget,
		COMMAND_RenderDepthStencilTarget,
		COMMAND_ClearSetColor,
		COMMAND_ClearSetDepth,
		COMMAND_ClearColor,
		COMMAND_ClearDepth,
		COMMAND_ScriptExternal_Color,
		COMMAND_Pass,
		COMMAND_LoopByCount,
		COMMAND_LoopEnd,
		COMMAND_LoopGetIndex,
		COMMAND_DrawGeometry,
		COMMAND_DrawBuffer,
	};

	/// 描画コマンド
	struct Command
	{
		CommandType		Type;
		union
		{
			struct
			{
				int							Index;
				Graphics::IShaderVariable*	TextureVariable;
				//Graphics::ITexture*		Texture;
			} RenderColorTarget;

			struct
			{
				Graphics::IShaderVariable*	TextureVariable;
				//Graphics::ITexture*		Texture;
			} RenderDepthStencilTarget;

			struct
			{
				lnFloat						Color[4];
			} ClearSetColor;

			struct
			{
				lnFloat						Depth;
			} ClearSetDepth;

			struct
			{
				Graphics::IShaderPass*		Pass;
				DrawingCommandList*			CommandList;
			} Pass;

			struct
			{
				int							Count;
			} LoopByCount;

			struct
			{
				Graphics::IShaderVariable*	Variable;
			} LoopGetIndex;

			struct
			{
				Graphics::IShaderPass*		Pass;
			} DrawGeometry;

			struct
			{
				Graphics::IShaderPass*		Pass;
			} DrawBuffer;
		};
	};

	typedef std::vector<Command>				CommandArray;
	typedef std::vector<DrawingCommandList*>	ChildCommandListArray;

private:

	SceneShader*				mOwnerShader;
	CommandArray				mCommandArray;
	ChildCommandListArray		mChildCommandListArray;
	VisualNodeContext*			mOwnerVisualNodeContext;
	std::vector<int>			mDrawingSubsetNumbers;
	DrawingCommandList*			mParentList;
	int							mScriptExternalColorPosition;

	LColorF						mClearColor;
	lnFloat						mClearDepth;

	//Graphics::IRenderer*        mRenderer;
 //   Graphics::GeometryRenderer* mGeometryRenderer;
	Graphics::Texture*			mOldRenderTarget[Graphics::MaxMultiRenderTargets];
	Graphics::Texture*			mOldDepthBuffer;
	LColorF						mOldClearColor;
	lnFloat						mOldClearDepth;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================