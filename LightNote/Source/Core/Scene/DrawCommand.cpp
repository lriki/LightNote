//==============================================================================
// DrawCommand 
//==============================================================================

#include "stdafx.h"
#include "../Graphics/Common/GeometryRenderer.h"
#include "SceneNode/SceneNode.h"
#include "SceneNode/Camera.h"
#include "SceneNode/Sprite.h"
#include "SceneShader/SceneShaderManager.h"
#include "SceneShader/SceneShader.h"
#include "SceneGraph.h"
#include "DrawCommand.h"
#include "Manager.h"

#include "SceneShader/MMETextureObject.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// ■ DrawingCommandList
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	DrawingCommandList::DrawingCommandList()
		: mOwnerShader					( NULL )
		, mOwnerVisualNodeContext		( NULL )
		, mParentList					( NULL )
		, mScriptExternalColorPosition	( -1 )
		, mOldDepthBuffer				( NULL )
		, mOldClearDepth				( 0 )
	{
		memset( mOldRenderTarget, 0, sizeof(mOldRenderTarget) );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	DrawingCommandList::~DrawingCommandList()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::initialize( SceneShader* ownerShader )
	{
		mOwnerShader = ownerShader;
		//Manager* manager = mOwnerShader->getSceneShaderManager()->getScene()->getManager();

		//mRenderer = manager->getGraphicsManager()->getGraphicsDevice()->getRenderer();
		//mGeometryRenderer = manager->getGraphicsManager()->getGeometryRenderer();
		
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::setOwnerVisualNodeContext( VisualNodeContext* context )
	{
		mOwnerVisualNodeContext = context;

		ln_foreach( DrawingCommandList* c, mChildCommandListArray )
		{
			c->setOwnerVisualNodeContext( mOwnerVisualNodeContext );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::setDrawingSubsetNumbers( const std::vector<int>& numbers )
	{
		mDrawingSubsetNumbers = numbers;

		ln_foreach( DrawingCommandList* c, mChildCommandListArray )
		{
			c->setDrawingSubsetNumbers( numbers );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::execute( ContextDrawParam& param )
	{
		_pushCurrentState( param );
		_executeInternal( param, 0, 0 );
		_popCurrentState( param );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::executePreScriptExternal_Color( ContextDrawParam& param )
	{
		LN_ERR2_ASSERT( mScriptExternalColorPosition >= 0 );
		_pushCurrentState( param );
		_executeInternal( param, 0, 0 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::executePostScriptExternal_Color( ContextDrawParam& param )
	{
		LN_ERR2_ASSERT( mScriptExternalColorPosition >= 0 );
		_executeInternal( param, mScriptExternalColorPosition + 1, 0 );
		_popCurrentState( param );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool DrawingCommandList::checkValid( MMEScriptClass scriptClass, MMEScriptOrder scriptOrder, bool onTech )
	{
		// TODO: エラーメッセージ

		int loopNest = 0;
		bool foundScriptExternalColor = false;
		int size = mCommandArray.size();
		for ( int i = 0; i < size; ++i )
		{
			Command* cmd = &mCommandArray[i];
			switch ( cmd->Type )
			{
				case COMMAND_RenderColorTarget:
				{
					if ( cmd->RenderColorTarget.Index < 0 || cmd->RenderColorTarget.Index >= Graphics::MaxMultiRenderTargets )
					{
						// レンダリングターゲットのインデックスが不正
						return false;
					}
					break;
				}
				case COMMAND_ScriptExternal_Color:
				{
					if ( scriptOrder != MME_SCRORDER_postprocess )
					{
						// ポストエフェクト（STANDARDSGLOBALパラメータのScriptOrderアノテーションに
						// "postprocess"が指定されたエフェクトファイル）でのみ実行できる。
						return false;
					}
					if ( foundScriptExternalColor )
					{
						// ScriptExternal=Color は1つだけ
						return false;
					}
					if ( loopNest > 0 )
					{
						// ScriptExternal=Color はループ中に記述することはできない
						return false;
					}
					foundScriptExternalColor = true;
					break;
				}
				case COMMAND_Pass:
				{
					if ( onTech == false )
					{
						// テクニックのスクリプト上でしか使用できない。
						return false;
					}
					break;
				}
				case COMMAND_LoopByCount:
				{
					if ( onTech == false )
					{
						// テクニックのスクリプト上でしか使用できない。
						return false;
					}
					++loopNest;
					break;
				}
				case COMMAND_LoopEnd:
				{
					if ( onTech == false )
					{
						// テクニックのスクリプト上でしか使用できない。
						return false;
					}
					--loopNest;
					if ( loopNest < 0 )
					{
						// LoopEnd が多い
						return false;
					}
					break;
				}
				case COMMAND_LoopGetIndex:
				{
					if ( onTech == false )
					{
						// テクニックのスクリプト上でしか使用できない。
						return false;
					}
					break;
				}
				case COMMAND_DrawGeometry:
				{
					if ( onTech )
					{
						// パスのスクリプト上でしか使用できない。
						return false;
					}
					if ( scriptClass == MME_SCRCLS_scene )
					{
						// STANDARDSGLOBALのScriptClassに"scene"を指定している場合、
						// このコマンドを実行してはならない。
						return false;
					}
					break;
				}
				case COMMAND_DrawBuffer:
				{
					if ( onTech )
					{
						// パスのスクリプト上でしか使用できない。
						return false;
					}
					if ( scriptClass == MME_SCRCLS_object )
					{
						// STANDARDSGLOBALのScriptClassに"object"を指定している場合、
						// このコマンドを実行してはならない。
						return false;
					}
					break;
				}
			}
		}

		if ( loopNest > 0 )
		{
			// ループが閉じていない
			return false;
		}

		if ( scriptOrder == MME_SCRORDER_postprocess && foundScriptExternalColor )
		{
			// ポストエフェクトだが、ScriptExternal=Color が見つからなかった
			return false;
		}

		return true;
	}

#if 1
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::add_RenderColorTarget( int index, Graphics::IShaderVariable* textureVariable )
	{
		Command c;
		c.Type = COMMAND_RenderColorTarget;
		c.RenderColorTarget.Index = index;
		c.RenderColorTarget.TextureVariable = textureVariable;
		mCommandArray.push_back( c );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::add_RenderDepthStencilTarget( Graphics::IShaderVariable* textureVariable )
	{
		Command c;
		c.Type = COMMAND_RenderDepthStencilTarget;
		c.RenderDepthStencilTarget.TextureVariable = textureVariable;
		mCommandArray.push_back( c );
	}
#else
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::add_RenderColorTarget( int index, Graphics::ITexture* texture )
	{
		Command c;
		c.Type = COMMAND_RenderColorTarget;
		c.RenderColorTarget.Index = index;
		c.RenderColorTarget.Texture = texture;
		mCommandArray.push_back( c );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::add_RenderDepthStencilTarget( Graphics::ITexture* texture )
	{
		Command c;
		c.Type = COMMAND_RenderDepthStencilTarget;
		c.RenderDepthStencilTarget.Texture = texture;
		mCommandArray.push_back( c );
	}
#endif

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::add_ClearSetColor( const LVector4& color )
	{
		Command c;
		c.Type = COMMAND_ClearSetColor;
		c.ClearSetColor.Color[0] = color.x;
		c.ClearSetColor.Color[1] = color.y;
		c.ClearSetColor.Color[2] = color.z;
		c.ClearSetColor.Color[3] = color.w;
		mCommandArray.push_back( c );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::add_ClearSetDepth( lnFloat depth )
	{
		Command c;
		c.Type = COMMAND_ClearSetDepth;
		c.ClearSetDepth.Depth = depth;
		mCommandArray.push_back( c );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::add_ClearColor()
	{
		Command c;
		c.Type = COMMAND_ClearColor;
		mCommandArray.push_back( c );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::add_ClearDepth()
	{
		Command c;
		c.Type = COMMAND_ClearDepth;
		mCommandArray.push_back( c );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::add_ScriptExternal_Color()
	{
		Command c;
		c.Type = COMMAND_ScriptExternal_Color;
		mCommandArray.push_back( c );

		// この位置を覚えておく
		mScriptExternalColorPosition = mCommandArray.size() - 1;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::add_Pass( Graphics::IShaderPass* pass, DrawingCommandList* commandList )
	{
		if ( commandList )
		{
			LN_ERR2_ASSERT( commandList->mParentList == NULL );
			mChildCommandListArray.push_back( commandList );
			commandList->mParentList = this;
		}

		Command c;
		c.Type = COMMAND_Pass;
		c.Pass.Pass = pass;
		c.Pass.CommandList = commandList;
		mCommandArray.push_back( c );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::add_LoopByCount( int count )
	{
		Command c;
		c.Type = COMMAND_LoopByCount;
		c.LoopByCount.Count = count;
		mCommandArray.push_back( c );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::add_LoopEnd()
	{
		Command c;
		c.Type = COMMAND_LoopEnd;
		mCommandArray.push_back( c );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::add_LoopGetIndex( Graphics::IShaderVariable* variable )
	{
		Command c;
		c.Type = COMMAND_LoopGetIndex;
		c.LoopGetIndex.Variable = variable;
		mCommandArray.push_back( c );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::add_DrawGeometry( Graphics::IShaderPass* pass )
	{
		Command c;
		c.Type = COMMAND_DrawGeometry;
		c.DrawGeometry.Pass = pass;
		mCommandArray.push_back( c );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::add_DrawBuffer( Graphics::IShaderPass* pass )
	{
		Command c;
		c.Type = COMMAND_DrawBuffer;
		c.DrawBuffer.Pass = pass;
		mCommandArray.push_back( c );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	int DrawingCommandList::_executeInternal( ContextDrawParam& param, int pc, int currentLoopCount )
	{
		int size = mCommandArray.size();
		for ( ;pc < size ; ++pc )
		{
			Command* cmd = &mCommandArray[pc];
			switch ( cmd->Type )
			{
				case COMMAND_RenderColorTarget:
				{
					if ( cmd->RenderColorTarget.TextureVariable )
					{
						param.Renderer->setRenderTarget( 
							cmd->RenderColorTarget.Index, cmd->RenderColorTarget.TextureVariable->getTexture() );
					}
					else
					{
						param.Renderer->setRenderTarget( cmd->RenderColorTarget.Index, mOldRenderTarget[cmd->RenderColorTarget.Index] );
					}
					break;
				}
				case COMMAND_RenderDepthStencilTarget:
				{
					if ( cmd->RenderDepthStencilTarget.TextureVariable )
					{
						param.Renderer->setDepthBuffer( cmd->RenderDepthStencilTarget.TextureVariable->getTexture() );
					}
					else
					{
						param.Renderer->setDepthBuffer( mOldDepthBuffer );
					}
					break;
				}
				case COMMAND_ClearSetColor:
				{
					mClearColor.set(
						cmd->ClearSetColor.Color[0],
						cmd->ClearSetColor.Color[1],
						cmd->ClearSetColor.Color[2],
						cmd->ClearSetColor.Color[3] );
					break;
				}
				case COMMAND_ClearSetDepth:
				{
					mClearDepth = cmd->ClearSetDepth.Depth;
					break;
				}
				case COMMAND_ClearColor:
				{
					param.Renderer->clear( true, false, mClearColor, 0 );
					break;
				}
				case COMMAND_ClearDepth:
				{
					param.Renderer->clear(false, true, LColorF::Transparency, mClearDepth);
					break;
				}
				case COMMAND_ScriptExternal_Color:
				{
					// 終了
					return pc;
				}
				case COMMAND_Pass:
				{
					if ( cmd->Pass.CommandList )
					{
						cmd->Pass.CommandList->execute( param );
					}
					// コマンドリストが省略されている場合は直接 Draw=Geometry
					/* Tech 側で作ってるのでここでは行わない
					else
					{
						_drawGeometry( cmd->Pass.Pass );
					}
					*/
					break;
				}
				case COMMAND_LoopByCount:
				{
					int lastPC = pc;
					for ( int i = 0 ; i < cmd->LoopByCount.Count; ++i )
					{
						lastPC = _executeInternal( param, pc + 1, i );
					}
					pc = lastPC;
					break;
				}
				case COMMAND_LoopEnd:
				{
					// 呼び出し側へ戻る
					return pc;
				}
				case COMMAND_LoopGetIndex:
				{
					cmd->LoopGetIndex.Variable->setInt( currentLoopCount );
					break;
				}
				case COMMAND_DrawGeometry:
				{
					_drawGeometry( param, cmd->DrawGeometry.Pass );
					break;
				}
				case COMMAND_DrawBuffer:
				{
					Graphics::LRenderState state = param.Renderer->getRenderState();
                    state.DepthTest = false;
                    state.DepthWrite = false;
                    param.Renderer->setRenderState( state );

					if ( cmd->DrawBuffer.Pass ) cmd->DrawBuffer.Pass->begin();

                    

                    param.GeometryRenderer->drawSquare(
                        -1.0f,  1.0f, 0xffffffff, 0.0f, 0.0f,   // 左上
                         1.0f,  1.0f, 0xffffffff, 1.0f, 0.0f,   // 右上
                        -1.0f, -1.0f, 0xffffffff, 0.0f, 1.0f,
                         1.0f, -1.0f, 0xffffffff, 1.0f, 1.0f,
						 0.0f );

					if ( cmd->DrawBuffer.Pass ) cmd->DrawBuffer.Pass->end();
					break;
				}
			}
		}
		return pc - 1;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::_drawGeometry( ContextDrawParam& param, Graphics::IShaderPass* pass )
	{
		param.Renderer->setRenderState(
			mOwnerVisualNodeContext->getRenderState() );

		// サブセットの指定がなければすべて描画する
		if ( mDrawingSubsetNumbers.empty() )
        {
			int subsetCount = mOwnerVisualNodeContext->getSubsetCount();
            for ( int i = 0; i < subsetCount; ++i )
            {
				mOwnerShader->updateSubsetParam(
					mOwnerVisualNodeContext->getSubsetRenderParam( i ) );

                // 描画
                if ( pass ) pass->begin();
                mOwnerVisualNodeContext->drawSubset( param, i );
                if ( pass ) pass->end();
            }
        }
		// サブセット範囲の指定がある
		else
		{
			int count = mDrawingSubsetNumbers.size();
			for ( int i = 0 ; i < count; ++i )
			{
				if ( mDrawingSubsetNumbers[i] < mOwnerVisualNodeContext->getSubsetCount() )
				{
					mOwnerShader->updateSubsetParam(
						mOwnerVisualNodeContext->getSubsetRenderParam( mDrawingSubsetNumbers[i] ) );

					// 描画
					if ( pass ) pass->begin();
					mOwnerVisualNodeContext->drawSubset( param, mDrawingSubsetNumbers[i] );
					if ( pass ) pass->end();
				}
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::_pushCurrentState( ContextDrawParam& param )
	{
		// 現在のレンダリングターゲット&深度バッファを記憶
		for ( int i = 0; i < Graphics::MaxMultiRenderTargets; ++i )
		{
			mOldRenderTarget[i] = param.Renderer->getRenderTarget( i );
			LN_SAFE_ADDREF( mOldRenderTarget[i] );
		}
		mOldDepthBuffer = param.Renderer->getDepthBuffer();
		LN_SAFE_ADDREF( mOldDepthBuffer );

		// 親の状態を記憶
		if ( mParentList )
		{
			mOldClearColor = mParentList->mClearColor;
			mOldClearDepth = mParentList->mClearDepth;
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DrawingCommandList::_popCurrentState( ContextDrawParam& param )
	{
		// レンダリングターゲット&深度バッファを元に戻す
		for ( int i = 0; i < Graphics::MaxMultiRenderTargets; ++i )
		{
			param.Renderer->setRenderTarget( i, mOldRenderTarget[i] );
			LN_SAFE_RELEASE( mOldRenderTarget[i] );
		}
		param.Renderer->setDepthBuffer( mOldDepthBuffer );
		LN_SAFE_RELEASE( mOldDepthBuffer );

		// 親の状態を元に戻す
		if ( mParentList )
		{
			mParentList->mClearColor = mOldClearColor;
			mParentList->mClearDepth = mOldClearDepth;
		}
	}


} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================