//==============================================================================
// GLUtil 
//------------------------------------------------------------------------------
///**
//  @file       GLUtil.h
//  @brief      GLUtil
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../../Threading/EventFlag.h"
#include "../GraphicsResource.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ GLResource
//------------------------------------------------------------------------------
///**
//  @brief     リソースの遅延構築を補助するクラス
//*/
//==============================================================================
class GLResource
    : /*public Base::ReferenceObject
	, */public GraphicsResource
{
public:

    GLResource()
        : mInitialized( false )
    {}

    virtual ~GLResource()
    {
        // ダメ。サブクラスで呼ぶこと。
        //onDisposeGLResource();
    }

public:

	/// (called from GraphicsDevice::commitChangesGLResource())
    void commitChanges()
    {
        if (!mInitialized.isTrue())
        {
            onCommitGLResource();
            mInitialized.setTrue();
        }
    }

	/// 文字を描画した等リソースの更新が必要な時に呼び出す
    void requestCommitGLResource() { mInitialized.setFalse(); }

protected:
	// GraphicsResource
	virtual void onPauseDevice() { onDisposeGLResource(); }
	virtual void onResumeDevice() { mInitialized.setFalse(); }

protected:

	/// GLリソース作成
	///		Renderer::beginScene() からいろいろ経由で呼ばれる。
	///		リソースが未作成であれば作成。作成済みであれば更新を行う。
    virtual void onCommitGLResource() = 0;

	/// GLリソース解放
	///		あくまでリソースの解放。ヒープにあるオブジェクトを解放する必要はない。
    virtual void onDisposeGLResource() = 0;

private:
    Threading::EventFlag mInitialized;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================