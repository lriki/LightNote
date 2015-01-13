//==============================================================================
// Viewport 
//------------------------------------------------------------------------------
///**
//  @file       Viewport.h
//  @brief      Viewport
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Animation/FadeValue.h"
//#include "../../Graphics/Common/Model/Model.h"
#include "VisualNode.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{
class ViewportContext;

//==============================================================================
// ■ Viewport クラス
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class Viewport
    : public VisualNode
{
public:

	/// コンストラクタ
    Viewport( SceneGraph* scene_ );

	/// デストラクタ
    virtual ~Viewport();

public:

	/// 作成
    void create( const Geometry::Size& size_, Graphics::SpriteRenderer* spriteRenderer );

    

	/// プリプロセス・ポストプロセス用のシェーダを追加する (priority_ は値が大きい方が先に処理される。ScriptOrder に preprocess または postprocess が指定されているシェーダのみ追加可能)
    void addPostEffectShader( SceneShader* shader_, int priority_ );

	/// プリプロセス・ポストプロセス用のシェーダを外す
    void removePostEffectShader( SceneShader* shader_ );

	/// 色調の設定
    void setTone( const LTone& tone_, double duration_ );

	/// エフェクト適用中かを判定する
    bool isEffect() const { return mTone.isFinished(); }

	/// この Viewport 以下の Sprite 描画に使用する SpriteRenderer
	Graphics::SpriteRenderer* getSpriteRenderer() { return mSpriteRenderer; }

public:

	/// 描画コマンドを階層的に作成する
	//virtual void makeDrawCommand( DrawCommandContainer* container_, MMDPass pass_ );

	/// ノードの種類の取得
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_VIEWPORT; }

	/// フレーム更新
    virtual void update( float elapsedTime );

private:

    

private:

    struct PPShader
    {
        int          Priority;
        SceneShader* Shader;

        bool operator() ( const PPShader& e_ ) const { return e_.Shader == Shader; }
    };

    typedef std::vector< PPShader >    PPShaderArray;

	/// ソート用の比較関数 (降順)
    static bool _cmpPPShader( const PPShader& l_, const PPShader& r_ ) { return l_.Priority >= r_.Priority; }

private:
	friend class ViewportContext;
    PPShaderArray                       mPPShaderArray;
    Animation::FadeValue< LVector4, lnFloat >    mTone;
    LVector2                            mViewSize;
	Graphics::SpriteRenderer*	mSpriteRenderer;

    

};

//==============================================================================
// ■ ViewportContext
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ViewportContext
	: public VisualNodeContext
{
public:
    ViewportContext();
    virtual ~ViewportContext();
    
public:
	void initialize( Viewport* viewport, Graphics::SpriteRenderer* spriteRenderer );
	virtual void render( ContextDrawParam& param );

private:
	Viewport*					mViewport;
	Graphics::SpriteRenderer*	mSpriteRenderer;
	//Viewport::PPShaderArray	mPPShaderArray;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================