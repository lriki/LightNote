//==============================================================================
// Manager 
//------------------------------------------------------------------------------
///**
//  @file       Manager.h
//  @brief      Manager
//*/
//==============================================================================

#pragma once

#include "Common.h"
#include "../Game/GameTime.h"
#include "../System/Event.h"

namespace LNote
{
namespace Core
{
namespace GUI
{

//==============================================================================
// Manager
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class Manager
    : public Base::ReferenceObject
{
public:

    struct ConfigData
    {
        System::Window*		MainWindow;
        FileIO::Manager*	FileManager;
        Graphics::Manager*	GraphicsManager;
    };

public:
    Manager();
    ~Manager();

public:

	/// 初期化
    void initialize( const ConfigData& configData );

	/// 終了処理
    void finalize();

	/// 全要素のフレーム更新 (メインスレッドから呼ぶこと)
    void updateFrame( const Core::Game::GameTime& gameTime );

	/// イベント処理
    bool doEvent( const System::EventArgs& e );

	/// 描画情報の更新
    void updateRenderContents();

	/// 描画
    void render();

public:
	void registerType(ClassDefinition* type);
	DependencyObject* newInstance(const lnRefString& typeName);

public:
    FileIO::Manager* getFileManager() { return mFileManager; }
    Graphics::Manager* getGraphicsManager() { return mGraphicsManager; }

private:

    System::Window*		mSystemMainWindow;
    FileIO::Manager*	mFileManager;
    Graphics::Manager*	mGraphicsManager;
};

} // namespace GUI
} // namespace Core
} // namespace LNote
