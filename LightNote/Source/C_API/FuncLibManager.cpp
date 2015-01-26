//==============================================================================
// FuncLibManager
/* 言語バインダ … コレクション

■C#
System.Collections.ObjectModel.Collection の実装ぽく。

class Collection<T>
{
IntPtr _handle;				// LCollection
Collection<T> _collection;	// C# インスタンスを保持しておく

void Add(T item)
{
LCollection_Add(_handle, item._handle);
_collection.Add(item);
}
}

問題はここでもデフォルトで作成されるオブジェクト。
→	LCollection 自体も、オブジェクトごとに作られるデフォルトオブジェクトである。
→	get したときに C# インスタンス作成で行けると思う。
→	これに限らず、継承されたクラスはどうするの？
SceneNode.Children は SceneNode だけど、デフォルトで作られる実体は Viewport とかあるし。
→	自動生成するなら、LNoteオブジェクトの継承の末端の型を知ることができないとならない。
併せて、そのIDからC#インスタンスを作るファクトリが必要になる。

もし LNote 内部から追加削除や順序変更が発生する場合、
ObservableCollection みたいに OnCollectionChanged を作る必要がある。



■ LuminoUI
・コレクションプロパティ
XAML でも、コレクションを表現する時は必ず IAddChild を実装していることが必要。
逆に使うとしたら静的なコレクションの表現だけ。
(Collection クラスとしては、プロパティは Count しかない)

…でも、継承して使うこともある。TriggersCollection とか。
他にも KeyedCollection とかあるし、派生先で新たにプロパティ定義しているのもある。

普通の Collection に対して UIエンジンの Collection は DependencyObject を継承する必要がすごくありそう。
→


*/

//==============================================================================

#include "stdafx.h"
#include "LFInternal.h"
#include "LFTypedef.h"
#include "../Core/Scene/Manager.h"
#include "../Core/Scene/SceneGraph.h"
#include "../Core/Scene/SceneNode/Camera.h"
#include "FuncLibManager.h"

namespace LNote
{
namespace Core
{
namespace Function
{

//==============================================================================
// Manager
//==============================================================================

	bool				Manager::IsSystemInitialized = false;

    ConfigData			Manager::ConfigData;
    Core::Manager*		Manager::CoreManager = NULL;
    FileIO::Manager*	Manager::FileIOManager = NULL;
	Input::Manager*		Manager::InputManager = NULL;
    Audio::Manager*		Manager::AudioManager = NULL;
	Graphics::Manager*	Manager::GraphicsManager = NULL;
	Scene::Manager*		Manager::SceneManager = NULL;

	LNHandle			Manager::MainWindow = NULL;
	LNHandle			Manager::DefaultViewPane = NULL;
	LNHandle			Manager::Default3DCamera = NULL;
	LNHandle			Manager::Default2DCamera = NULL;
	LNHandle			Manager::Default3DRootNode = NULL;
	LNHandle			Manager::Default2DRootNode = NULL;
	LNHandle			Manager::GameControllers[Core::Input::LN_MAX_GAME_CONTROLLERS] = { 0 };

	LNBool				Manager::ExceptionMessageBox = LN_TRUE;
	LNExceptionCallback	Manager::ExceptionCallback = NULL;

	Manager::ObjectEntryArray		Manager::mObjectEntryArray;
	std::stack<int>					Manager::mObjectIndexStack;
	
	//ReleaseList<FileIO::InFile>		Manager::InFileList;
	//ReleaseList<FileIO::OutFile>	Manager::OutFileList;
 //   ReleaseList<Audio::Sound>		Manager::SoundList;
	//ReleaseList<Graphics::ITexture>	Manager::TextureList;
	//ReleaseList<Scene::SceneNode>	Manager::SceneNodeList;

    Base::ExceptionManager  Manager::mExceptionManager;
	
#if 0
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::initialize()
    {
		checkCommonDefinition();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::initializeIO()
	{
		checkCommonDefinition();
        CoreManager = LN_NEW Core::Manager();
		CoreManager->initializeFileManager( ConfigData );
		IOManager		= CoreManager->getFileManager();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::initializeAudio()
    {
		checkCommonDefinition();
        CoreManager = LN_NEW Core::Manager();
        CoreManager->initializeForAudioLibrary( ConfigData );
		IOManager		= CoreManager->getFileManager();
        AudioManager	= CoreManager->getAudioManager();
    }
#endif

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::preInitialize()
    {
		if ( CoreManager == NULL )
		{
			checkCommonDefinition();
			CoreManager = LN_NEW Core::Manager();
			IsSystemInitialized = true;
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::preInitialize( Core::Manager* manager )
	{
		CoreManager = manager;
		CoreManager->addRef();
		IsSystemInitialized = true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::postInitialize()
	{

		// オブジェクト管理配列
		for (int i = 0; i < 512; ++i)
		{
			ObjectEntry e;
			e.Object = NULL;
			//e.Interface = NULL;
			e.Index = i;
			mObjectEntryArray.push_back(e);

			if (i > 0) {  // 0 は NULL 扱い。欠番にする。
				mObjectIndexStack.push(i);
			}
		}

		// デフォルトオブジェクトの登録
		if ( CoreManager != NULL ) 
		{
			FileIOManager	= CoreManager->getFileManager();
			InputManager	= CoreManager->getInputManager();
			AudioManager	= CoreManager->getAudioManager();	
			GraphicsManager	= CoreManager->getGraphicsManager();
			SceneManager	= CoreManager->getSceneManager();

			MainWindow		= registerReferenceObject(CoreManager->getSystemManager()->getMainWindow(), true);

			if ( SceneManager != NULL )
			{
				DefaultViewPane = registerReferenceObject(SceneManager->getDefaultPane(), true);
				Default3DCamera = registerReferenceObject(SceneManager->getDefaultSceneGraph()->getDefaultCamera(0), true);
				Default2DCamera = registerReferenceObject(SceneManager->getDefaultSceneGraph()->getDefaultCamera(1), true);
				Default3DRootNode = registerReferenceObject(SceneManager->getDefaultSceneGraph()->getDefaultNode(LN_DEFAULT_SCENENODE_3D), true);
				Default2DRootNode = registerReferenceObject(SceneManager->getDefaultSceneGraph()->getDefaultNode(LN_DEFAULT_SCENENODE_2D), true);
			}

			if (InputManager != NULL)
			{
				GameControllers[0] = registerReferenceObject(InputManager->getGameController(0), true);
				GameControllers[1] = registerReferenceObject(InputManager->getGameController(1), true);
				GameControllers[2] = registerReferenceObject(InputManager->getGameController(2), true);
				GameControllers[3] = registerReferenceObject(InputManager->getGameController(3), true);
			}
		}

		// interface 削除コールバック
		Base::Interface::mInterfaceDeleting = onInterfaceDeleting;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::finalize()
    {
		//InFileList.clear();
		//OutFileList.clear();
		//SoundList.clear();
		//TextureList.clear();
		//SceneNodeList.clear();

		// Interface 削除コールバックから vector の内容が変わるため一時リスト化
		ObjectEntryArray deleteArray = mObjectEntryArray;

		// 残っているオブジェクトを全て削除
		ln_foreach(ObjectEntry& e, deleteArray)
		{
			if (e.Object != NULL && !((ObjectRegisterData*)e.Object->getUserData())->IsInternalObject) {
				LN_SAFE_RELEASE(e.Object);
			}
			//if (e.Interface != NULL && !((ObjectRegisterData*)e.Interface->getUserData())->IsInternalObject) {
			//	LN_SAFE_RELEASE(e.Interface);
			//}
		}
		// C# とかはこの関数の後で release されてくることがある
		//mObjectEntryArray.clear();
		//while ( !mObjectIndexStack.empty() ) {
		//	mObjectIndexStack.pop();
		//}

        if ( CoreManager )
        {
            CoreManager->finalize();
            LN_SAFE_RELEASE( CoreManager );
        }

		IsSystemInitialized = false;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    LNResult Manager::procException( Base::Exception* e )
    {
		if (ExceptionMessageBox != LN_FALSE) {
			mExceptionManager.procException(*e);
		}
		return (LNResult)e->getErrorCode();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    //LNResult Manager::getLastErrorCode()
    //{
    //    return mExceptionManager.getLastException().getErrorCode();
    //}

	////----------------------------------------------------------------------
	////
	////----------------------------------------------------------------------
	//int Manager::pushObject( Base::ReferenceObject* obj, bool addRef )
	//{
	//	LN_ERR2_ASSERT(obj);
	//	LN_THROW_InvalidCall(obj->getUserData() == NULL);

	//	if ( addRef ) {
	//		obj->addRef();
	//	}

	//	// 管理配列がすべて埋まっている場合
	//	if ( mObjectIndexStack.empty() ) 
	//	{
	//		// 末尾に追加する
	//		ObjectEntry e;
	//		e.Object	= obj;
	//		//e.Interface	= NULL;
	//		e.Index		= mObjectEntryArray.size();
	//		mObjectEntryArray.push_back(e);

	//		// ユーザーデータ登録
	//		ObjectRegisterData* data = LN_NEW ObjectRegisterData();
	//		data->Index = e.Index;
	//		data->IsInternalObject = false;
	//		obj->setUserData(data);

	//		return e.Index;
	//	}
	//	else 
	//	{
	//		// 空き場所を取得
	//		int newPos = mObjectIndexStack.top();
	//		mObjectIndexStack.pop();

	//		// 格納
	//		ObjectEntry& e = mObjectEntryArray[newPos];
	//		LN_ERR2_ASSERT( e.Object == NULL );
	//		//LN_ERR2_ASSERT( e.Interface == NULL );

	//		e.Object	= obj;
	//		//e.Interface	= NULL;
	//		e.Index		= newPos;

	//		// ユーザーデータ登録
	//		ObjectRegisterData* data = LN_NEW ObjectRegisterData();
	//		data->Index = e.Index;
	//		data->IsInternalObject = false;
	//		obj->setUserData(data);

	//		return e.Index;
	//	}
	//}

	////----------------------------------------------------------------------
	////
	////----------------------------------------------------------------------
	//int Manager::pushObject( Base::Interface* obj, bool addRef )
	//{
	//	LN_ERR2_ASSERT( obj );
	//	LN_THROW_InvalidCall(obj->getUserData() == NULL);

	//	if ( addRef ) {
	//		obj->addRef();
	//	}

	//	// 管理配列がすべて埋まっている場合
	//	if ( mObjectIndexStack.empty() ) 
	//	{
	//		// 末尾に追加する
	//		ObjectEntry e;
	//		e.Object	= NULL;
	//		//e.Interface	= obj;
	//		e.Index		= mObjectEntryArray.size();
	//		mObjectEntryArray.push_back( e );

	//		// ユーザーデータ登録
	//		ObjectRegisterData* data = LN_NEW ObjectRegisterData();
	//		data->Index = e.Index;
	//		obj->setUserData(data);
	//		data->IsInternalObject = false;

	//		return e.Index;
	//	}
	//	else 
	//	{
	//		// 空き場所を取得
	//		int newPos = mObjectIndexStack.top();
	//		mObjectIndexStack.pop();

	//		// 格納
	//		ObjectEntry& e = mObjectEntryArray[newPos];
	//		LN_ERR2_ASSERT( e.Object == NULL );
	//		//LN_ERR2_ASSERT( e.Interface == NULL );

	//		e.Object	= NULL;
	//		//e.Interface	= obj;
	//		e.Index		= newPos;

	//		// ユーザーデータ登録
	//		ObjectRegisterData* data = LN_NEW ObjectRegisterData();
	//		data->Index = e.Index;
	//		data->IsInternalObject = false;
	//		obj->setUserData(data);

	//		return e.Index;
	//	}
	//}


	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	int Manager::registerReferenceObject(Base::ReferenceObject* obj, bool isInternalObject)
	{
		LN_ERR2_ASSERT(obj);
		LN_THROW_Argument(obj->getUserData() == NULL);

		//if (!isInternalObject) {
		//	obj->addRef();
		//}

		// 管理配列がすべて埋まっている場合
		if (mObjectIndexStack.empty())
		{
			// 末尾に追加する
			ObjectEntry e;
			e.Object = obj;
			//e.Interface = obj;
			e.Index = mObjectEntryArray.size();
			e.RefCount = 1;
			mObjectEntryArray.push_back(e);

			// ユーザーデータ登録
			ObjectRegisterData* data = LN_NEW ObjectRegisterData();
			data->Index = e.Index;
			obj->setUserData(data);
			data->IsInternalObject = isInternalObject;

			return e.Index;
		}
		else
		{
			// 空き場所を取得
			int newPos = mObjectIndexStack.top();
			mObjectIndexStack.pop();

			// 格納
			ObjectEntry& e = mObjectEntryArray[newPos];
			LN_ERR2_ASSERT(e.Object == NULL);
			//LN_ERR2_ASSERT(e.Interface == NULL);

			e.Object = obj;
			//e.Interface = obj;
			e.Index = newPos;
			e.RefCount = 1;

			// ユーザーデータ登録
			ObjectRegisterData* data = LN_NEW ObjectRegisterData();
			data->Index = e.Index;
			data->IsInternalObject = isInternalObject;
			obj->setUserData(data);

			return e.Index;
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Manager::releaseObject(int index)
	{
		if (!IsSystemInitialized)
			return;		// すべて解放済み

		ObjectEntry& e = mObjectEntryArray[index];
		//LN_ERR2_ASSERT( e.Object || e.Interface );	// 同じindexをスタックに戻さないように
		//if (e.Object == NULL && e.Interface == NULL)					// finalize 後に呼ばれることがあるので、assert にはしたくない
		//{
		//	return;
		//}

		if (e.Object != NULL &&
			!((ObjectRegisterData*)e.Object->getUserData())->IsInternalObject) {
			e.RefCount--;
			if (e.RefCount <= 0) {
				LN_SAFE_RELEASE(e.Object);
			}
		}
		//if (e.Interface != NULL &&
		//	!((ObjectRegisterData*)e.Interface->getUserData())->IsInternalObject) {
		//	LN_SAFE_RELEASE(e.Interface);
		//}
		// インデックスを戻すのはコールバック側で行う
		//mObjectIndexStack.push( index );
	}

	void Manager::addRefObject(int index)
	{
		ObjectEntry& e = mObjectEntryArray[index];
		if (e.Object != NULL) {
			e.RefCount++;
		}
	}

	int Manager::getRefCountRefObject(int index)
	{
		ObjectEntry& e = mObjectEntryArray[index];
		if (e.Object != NULL) {
			return e.RefCount;
		}
		return 0;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Base::ReferenceObject* Manager::getReferenceObject(int index)
	{
		return mObjectEntryArray[index].Object;
	}


	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ObjectEntry* Manager::getObjectEntry(LNHandle handle)
	{ 
		return &mObjectEntryArray[TO_INDEX(handle)];
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//Base::Interface* Manager::getInterface( int index )
	//{
	//	return mObjectEntryArray[index].Interface;
	//}

} // namespace Function
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================