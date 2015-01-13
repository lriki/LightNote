
#include "stdafx.h"
#include <map>
#pragma comment(lib, "../LNote/LNote.Debug.lib")


#include "../LNote/lnote.hpp"
using namespace LN;

int main()
{
	// 初期化
	Manager::Initialize();

	// 音声再生
	RefPtr<Sound> sound1(new Sound("ZIGG-ZAGG.mp3"));
	sound1->SetPitch(105);
	sound1->Play();

	// スプライト
	RefPtr<Texture> tex1(new Texture("20140816_1.png"));
	RefPtr<Sprite> sprite1(new Sprite(tex1));

	// レイヤーリストにトランジションフィルタを追加
	RefPtr<TransitionFilter> transition(new TransitionFilter());
	ViewPane* viewPane = ViewPane::GetDefaultViewPane();
	ObjectList<Layer*>* layerList = viewPane->GetLayers();
	layerList->Add(transition);

	// トランジション開始
	RefPtr<Texture> transitionTex(new Texture("Transition1.png"));
	transition->Freeze();
	transition->Transition(600, transitionTex, 40);

	// メインループ
	do
	{
		Manager::Update();
	} while (!Manager::IsEndRequested());

	Manager::Terminate();
	return 0;
}

////#include "Header.h"
//
////void Test1();
////
////int main()
////{
////	Test1();
////
////	printf("main:%d\n", Test1::Get());
////
////	return 0;
////}
////
//#if 1
//#include "../LNote/lnote.hpp"
//#include <crtdbg.h>
//#pragma comment(lib, "../LNote/LNote.lib")
//
//class A
//{
//public:
//	A()
//	{
//		//throw 10;
//	}
//};
//
//int main()
//{
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//
//
//	LManager_Initialize();
//
//
//	//A* a = NULL;
//	//try
//	//{
//	//	a = new A();
//	//}
//	//catch (...)
//	//{
//	//	
//	//}
//	//printf("%p\n", a);
//	//delete a;
//
//
//
//	lnBool result;
//	do
//	{
//		LManager_Update();
//
//		LManager_IsEndRequested(&result);
//	} while (!result);
//
//	LManager_Terminate();
//	return 0;
//}
//#endif
