// Test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <time.h>
#include "../Core/Base/Encoding.h"
#include "../C_API/FuncLibManager.h"
#include "../Core/Graphics/Common/FormattedTextRenderer.h"
// ../../Samples/Media

#ifdef LNOTE_FUNCLIB_TEST

int mapdata[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 2, 3, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 4, 5, 6, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
};

int main()
{
	LNCore_Initialize();

	//for (size_t i = 0; i < 1000; i++)
	//{
	//	float f;
	//	LNRandom_GetFloatRange(NULL, 0, 100, &f);
	//	_p(f);
	//}

	

	LNHandle tex2;
	LNTexture_CreateFromFile(&tex2, "D:/Proj/lightnote/LightNote/Samples/Media/Spark1.png");
	LNTexture_ClearRGBA(tex2, 0, 0, 0, 0);

	Core::Manager* core = (Core::Manager*)LNCore_GetInternalObject();
	LRefPtr<Scene::PointParticle> particle1(LN_NEW Scene::PointParticle(core->getSceneManager()->getDefaultSceneGraph()));
	

	Graphics::PointParticleParameter param;
	param.ParticleNum = 100;
	param.GenerationInterval = 10;
	param.Size = 1;
	param.Life = 10 * 1000;
	param.FadeIn = 20;
	param.FadeOut = 20;
	param.PositionRand.set(0.2, 0.2, 0.2);
	param.AngleVelocity = 0.00001;
	param.Velocity.set(0, 0.0001, 0);
	particle1->create(param);
	particle1->setTexture((Graphics::Texture*)LNObject_GetInternalObject(tex2));
	particle1->setEnableDepthTest(false);
	particle1->setEnableDepthWrite(false);
	particle1->setBlendMode(LNote::Core::Graphics::BlendMode_Add);

	LNHandle viewPane;
	LNViewPane_GetDefaultViewPane(&viewPane);
	LNColorF bg1 = { 0, 0, 0, 1 };
	LNViewPane_SetBackgroundColor(viewPane, &bg1);

#if 0
	lnHandle tex1;
	LNTexture_Create(&tex1, 256, 256);
	lnHandle sprite1;
	LNSprite_Create(&sprite1, tex1);

	Graphics::FormattedTextRenderer tr;
	// A@S[5]B
	//uint32_t utf32Text[] = { 0x41, 0x40, 0x53, 0x5B, 0x35, 0x5D, 0x42, 0x00 };
	//lnString text1 = "A@S[5]B";
	//lnString text1 = "ASDFG\nHJKL";
	lnString text1 = "ゲームを始めます。\n鍵を探しながらステージを進み、\n最終ステージのボスを倒すと\nゲームクリアです。\n";
	Base::BasicString<uint32_t> text;// (text1.c_str(), Base::Encoding::GetSystemEncoding());
	text.ConvertFrom(text1.c_str(), text1.getByteCount(), Base::Encoding::GetSystemMultiByteEncoding());
	tr.setTypingSpeed(2);
	tr.setTargetTexture((Graphics::Texture*)LNObject_GetInternalObject(tex1));
	tr.setBaseFont(Function::Manager::GraphicsManager->getFontManager()->getDefaultFont());
	tr.setText(text.c_str());
	//text.ConvertFrom(L"てすとテキストASDF", Base::Encoding);
#endif


	LNHandle maptable;
	LNIntTable_CreateFromSrcData(&maptable, 8, 8, 1, mapdata);

	LNHandle tileset;
	LNTexture_CreateFromFile(&tileset, "D:/Documents/RPGXP/GoldenSunMapSystem/Graphics/Tilesets/002-Woods01EX.png");

	//lnHandle tilemap;
	//LNTilemap_Create(&tilemap);
	//LNTilemap_SetTilesetTexture(tilemap, tileset);
	//LNTilemap_SetMapData(tilemap, maptable);


	LNBool state;
	do
	{
		//tr.update();
		LNCore_Update();
		LNCore_IsEndRequested(&state);
	} while (!state);

	LNCore_Terminate();



#if 0
	LNCore_Initialize();

	


	//FileIO::PathName path(L"AAA");//(L"../");
	//FileIO::PathName path2 = path.canonicalizePath();
	//FileIO::PathName path3(L"../");
	//FileIO::PathName path4 = path3.canonicalizePath();

	//lnHandle font = LFont_Create();
	//const lnChar* name;
	//LFont_GetName(font, &name);
	//printf(name);

	//lnHandle texture = LTexture_CreateFromFile("../../Samples/Media/0129.png");
	lnHandle texture;
	LNTexture_CreateFromFile(&texture, "../../Samples/Media/0129.png");
	lnHandle sprite;
	LNSprite_Create(&sprite, texture);

	LNSceneNode_SetName(sprite, "my_sprite");

	const char* name;
	_p(LNSceneNode_GetName(sprite, &name));
	_p(name);

	lnVector3 vec0;
	lnVector3 vec1;
	lnVector3 vec2;
	LNVector3_Set(&vec1, 10.0, 20.0, 30.0);
	LNVector3_Set(&vec2, 0.0, 1.0, 0.0);
	LNVector3_RotateAxis(&vec0, &vec1, &vec2, 0.5);


	// TODO  タイムスケール

	// ViewPane は描画が行われる画面領域を表すオブジェクトです。
	// Filter
	lnHandle viewPane;
	LNViewPane_GetDefaultViewPane(&viewPane);

	lnHandle layerList;
	LNViewPane_GetLayers(viewPane, &layerList);

	// 色調を操作するフィルタを生成し、登録します。
	lnHandle toneFilter;
	LNToneFilter_Create(&toneFilter);
	//LObjectList_Add(layerList, toneFilter);
	//LViewPane_AttachLayer(viewPane, toneFilter);


	// ブラー を適用するフィルタを生成し、登録します。
	lnHandle blurFilter;
	LNBlurFilter_Create(&blurFilter);
	//LObjectList_Add(layerList, blurFilter);
	//LViewPane_AttachLayer(viewPane, blurFilter);

	// トランジションを適用するフィルタを生成し、登録します。
	lnHandle transition1;
	LNTransitionFilter_Create(&transition1);
	LNObjectList_Add(layerList, transition1);
	//LViewPane_AttachLayer(viewPane, transition1);



	// トランジション準備 (現在の何も描かれていない画面を遷移元とする)
	LNTransitionFilter_Freeze(transition1);

	// トランジション実行
	lnHandle filterTexture;
	LNTexture_CreateFromFile(&filterTexture, "../../Samples/Media/Transition1.png");
	LNTransitionFilter_Transition(transition1, 600, filterTexture, 40);

	lnBool state;
	do
	{
		lnHandle ctrl;
		LNVirtualPad_GetVirtualPad(0, &ctrl);

		LNInput_IsTrigger(LN_BUTTON_A, &state);
		if (state)
		{
			// 1秒かけてセピア調に変更します。
			lnTone tone = { 0.0, -0.2, -0.5, 1.0 };
			LNToneFilter_SetTone(toneFilter, &tone, 2.0);
		}
		LNInput_IsTrigger(LN_BUTTON_B, &state);
		if (state)
		{
			// 白画面へフェードアウトします。
			lnTone tone = { 1.0, 1.0, 1.0, 0.0 };
			LNToneFilter_SetTone(toneFilter, &tone, 2.0);
		}
		LNInput_IsTrigger(LN_BUTTON_C, &state);
		if (state)
		{
			// 色調を元に戻します。
			lnTone tone = { 0.0, 0.0, 0.0, 0.0 };
			LNToneFilter_SetTone(toneFilter, &tone, 2.0);
		}

		LNInput_IsTrigger(LN_BUTTON_X, &state);
		if (state)
		{
			// 画面左上から x:100, y:100 を中心とした放射ブラーを適用します。
			lnHandle camera2D;
			LNCamera_GetDefault2DCamera(&camera2D);
			lnVector3 t = { 100, 100, 0 };
			LNBlurFilter_Blur(blurFilter, 0.5, 0.5, 1.05, &t, camera2D);
		}

		LNCore_Update();
		LNCore_IsEndRequested(&state);
	} while (!state);

	LNCore_Terminate();
#endif
	return 0;
}



#endif // LNOTE_FUNCLIB_TEST

