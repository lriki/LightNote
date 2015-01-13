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

	/// ������
    void initialize( const ConfigData& configData );

	/// �I������
    void finalize();

	/// �S�v�f�̃t���[���X�V (���C���X���b�h����ĂԂ���)
    void updateFrame( const Core::Game::GameTime& gameTime );

	/// �C�x���g����
    bool doEvent( const System::EventArgs& e );

	/// �`����̍X�V
    void updateRenderContents();

	/// �`��
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
