//==============================================================================
// 画面全体へのエフェクト処理のサンプル (ポストエフェクト)
//------------------------------------------------------------------------------
/*  
	LightNote では「ビューフィルタ」と呼んでいます。
*/
//==============================================================================

#include <lnote.h>

int main()
{
	LNCore_Initialize();

	lnHandle texture;
	lnHandle sprite;
	LNTexture_CreateFromFile(&texture, "../Media/0129.png");
	LNSprite_Create(&sprite, texture);
	

	// TODO  タイムスケール

	// ViewPane は描画が行われる画面領域を表すオブジェクトです。
	lnHandle viewPane;
	LNViewPane_GetDefaultViewPane(&viewPane);

	// ViewPane 内のレイヤーのリストを取得します。
	lnHandle layerList;
	LNViewPane_GetLayers(viewPane, &layerList);

	// 色調を操作するフィルタを生成し、登録します。
	lnHandle toneFilter;
	LNToneFilter_Create(&toneFilter);
	LNObjectList_Add(layerList, toneFilter);

	// ブラー を適用するフィルタを生成し、登録します。
	lnHandle blurFilter;
	LNBlurFilter_Create(&blurFilter);
	LNObjectList_Add(layerList, blurFilter);

	// トランジションを適用するフィルタを生成し、登録します。
	lnHandle transitionFilter;
	LNTransitionFilter_Create(&transitionFilter);
	LNObjectList_Add(layerList, transitionFilter);


	// トランジション準備 (現在の何も描かれていない画面を遷移元とする)
	LNTransitionFilter_Freeze(transitionFilter);

	// トランジション実行
	lnHandle filterTexture;
	LNTexture_CreateFromFile(&filterTexture, "../Media/Transition1.png");
	LNTransitionFilter_Transition(transitionFilter, 2.0, filterTexture, 40);

	// メインループ
	lnBool endRequested;
	do
	{
		lnBool state;
		LNInput_IsTrigger(LN_BUTTON_0, &state);
		if (state)
		{
			// 1秒かけてセピア調に変更します。
			lnTone tone = { 0.0, -0.2, -0.5, 1.0 };
			LNToneFilter_SetTone(toneFilter, &tone, 2.0);
		}
		LNInput_IsTrigger(LN_BUTTON_1, &state);
		if (state)
		{
			// 白画面へフェードアウトします。
			lnTone tone = { 1.0, 1.0, 1.0, 0.0 };
			LNToneFilter_SetTone(toneFilter, &tone, 2.0);
		}
		LNInput_IsTrigger(LN_BUTTON_2, &state);
		if (state)
		{
			// 色調を元に戻します。
			lnTone tone = { 0.0, 0.0, 0.0, 0.0 };
			LNToneFilter_SetTone(toneFilter, &tone, 2.0);
		}

		LNInput_IsTrigger(LN_BUTTON_3, &state);
		if (state)
		{
			// 画面左上から x:100, y:100 を中心とした放射ブラーを適用します。
			lnHandle camera2D;
			LNCamera_GetDefault2DCamera(&camera2D);
			lnVector3 t = { 100, 100, 0 };
			LNBlurFilter_Blur(blurFilter, 0.5, 0.5, 1.05, &t, camera2D);
		}

		LNCore_Update();
		LNCore_IsEndRequested(&endRequested);
	} while (endRequested == LN_FALSE);

	LNCore_Terminate();
	return 0;
}