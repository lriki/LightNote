//==============================================================================
// FuncLibManager
//------------------------------------------------------------------------------
///**
//  @file       FuncLibManager.h
//  @brief      FuncLibManager
//*/
//==============================================================================

#pragma once

#include <stack>
#include <set>
#include "LFCommon.h"
#include "../Core/FileIO/InFile.h"
#include "../Core/FileIO/OutFile.h"
#include "../Core/Input/Common.h"
#include "../Core/Audio/Sound.h"
#include "../Core/Graphics/Interface.h"
#include "../Core/Graphics/DeviceObjects.h"
#include "../Core/Scene/SceneNode/SceneNode.h"
#include "../Core/Manager.h"
#include "LFTypedef.h"

namespace LNote
{
namespace Core
{
namespace Function
{

// Handle の管理リストの要素
struct ObjectEntry
{
	Base::ReferenceObject*	Object;
	void*					UserData;	///< バインダ側からセットするユーザーデータ。基本的に管理配列のインデックスとなる。
	//Base::Interface*		Interface;
	int						Index;
	int						RefCount;

	ObjectEntry()
	{
		Object = NULL;
		Index = 0;
		RefCount = 0;
	}
};

// Base::ReferenceObject のユーザーデータとして登録する
struct ObjectRegisterData
{
	int						Index;
	bool					IsInternalObject;	// true の場合は↓のManager 内で参照カウントを操作しない
};

//==============================================================================
// Manager
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//------------------------------------------------------------------------------
/*
	API 経由で公開される Intrface の UserData には必ず ObjectRegisterData が格納される。

	内部的にデフォルトで作成されるオブジェクトは、取得しようとしたときに必ず通過する LNHandle 化関数で、
	ObjectRegisterData が格納されているかチェックされる。
	この時格納されていなければ新しく格納し、この Manager の管理下に入る。
*/
//==============================================================================
class Manager
{
public:

	/// 初期化前処理
    static void preInitialize();

	/// 初期化前処理 (ユニットテスト用)
	static void preInitialize( Core::Manager* manager );

	/// ファイル入出力のみ初期化
	//static void initializeIO();

	/// 音声機能のみ初期化
	//static void initializeAudio();

	/// 初期化後処理
	static void postInitialize();

	/// 終了処理
    static void finalize();

	/// 例外処理
    static LNResult procException( Base::Exception* e );

	/// 最後に発生した例外コードの取得
    //static LNResult getLastErrorCode();

	/// Handleオブジェクト追加
	//static int pushObject( Base::ReferenceObject* obj, bool addRef = false );	// Obsolete
	//static int pushObject( Base::Interface* obj, bool addRef = false );			// Obsolete
	static int registerReferenceObject(Base::ReferenceObject* obj, bool isInternalObject = false);

	/// Handleオブジェクト release
	static void releaseObject(int index);

	/// Handleオブジェクト addRef
	static void addRefObject(int index);

	/// Handleオブジェクト addRef
	static int getRefCountRefObject(int index);

	/// Handleオブジェクト取得
	static Base::ReferenceObject*	getReferenceObject(int index);
	//static Base::Interface*			getInterface( int index );

	/// Handle が表す ObjectEntry の取得
	static ObjectEntry* getObjectEntry(LNHandle handle) { return &mObjectEntryArray[TO_INDEX(handle)]; }

	static LNHandle getInternalObjectHandle(Base::ReferenceObject* obj)
	{
		// 未登録なら登録する
		if (obj->getUserData() == NULL)
		{
			registerReferenceObject(obj, true);
			//pushObject(obj);
			//ObjectRegisterData* data = LN_NEW ObjectRegisterData();
			//data->
			//obj->
		}
		return ((ObjectRegisterData*)obj->getUserData())->Index;
	}

	static void onInterfaceDeleting(Base::Interface* obj)
	{
		ObjectRegisterData* data = (ObjectRegisterData*)obj->getUserData();
		if (data != NULL)
		{
			// 管理リストから削除
			ObjectEntryArray::iterator itr = mObjectEntryArray.begin();
			ObjectEntryArray::iterator end = mObjectEntryArray.end();
			for (; itr != end; itr++) {
				if ((*itr).Object == obj/* || (*itr).Interface == obj*/) {
					//mObjectEntryArray.erase(itr);
					(*itr).Object = NULL;
					(*itr).Index = 0;
					(*itr).RefCount = 0;
					break;
				}
			}

			// インデックスを戻す
			mObjectIndexStack.push(data->Index);

			// ユーザーデータ削除
			delete data;
			obj->setUserData(NULL);
		}
	}

public:

	static bool					IsSystemInitialized;

    static ConfigData			ConfigData;
    static Core::Manager*		CoreManager;
    static FileIO::Manager*		FileIOManager;
    static Input::Manager*		InputManager;
    static Audio::Manager*		AudioManager;
    static Graphics::Manager*	GraphicsManager;
    static Scene::Manager*		SceneManager;

	// デフォルトで作成されるオブジェクト
	static LNHandle				MainWindow;
	static LNHandle				DefaultViewPane;
	static LNHandle				Default3DCamera;
	static LNHandle				Default2DCamera;
	static LNHandle				Default3DRootNode;
	static LNHandle				Default2DRootNode;
	static LNHandle				GameControllers[Core::Input::LN_MAX_GAME_CONTROLLERS];

	static LNBool				ExceptionMessageBox;
	static LNExceptionCallback	ExceptionCallback;

private:
	typedef std::vector<ObjectEntry>	ObjectEntryArray;
	static ObjectEntryArray		mObjectEntryArray;
	static std::stack<int>		mObjectIndexStack;

public:
	
	//static ReleaseList<FileIO::InFile>		InFileList;
	//static ReleaseList<FileIO::OutFile>		OutFileList;
 //   static ReleaseList<Audio::Sound>		SoundList;
	//static ReleaseList<Graphics::ITexture>	TextureList;
	//static ReleaseList<Scene::SceneNode>	SceneNodeList;

private:

    static Base::ExceptionManager   mExceptionManager;
};

} // namespace Function
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================