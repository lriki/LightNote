//==============================================================================
// スプライトの特殊効果サンプル
//------------------------------------------------------------------------------
/*
	スプライトには以下の様々な特殊効果を設定することができます。
	・カラースケール (RGBA各色の強さ)
	・ブレンドカラー (塗りつぶし)
	・色調
	・合成方法
*/
//==============================================================================

#include <lnote.h>

int main()
{
	LNCore_Initialize();

	// 特殊効果の結果が見易いように背景を用意する
	lnHandle bgTex;
	lnHandle bgSprite;
	LNTexture_CreateFromFile(&bgTex, "../Media/DeskPad1.png");
	LNSprite_Create(&bgSprite, bgTex);

	// オリジナル画像

	int y = 60;

	//------------------------------------------------------------
	//  特殊効果を適用する前のオリジナル
	lnHandle tex1;
	lnHandle sprite1_1;
	LNTexture_CreateFromFile(&tex1, "../Media/Crystal2.png");
	LNSprite_Create(&sprite1_1, tex1);
	LNSceneNode_SetPositionXYZ(sprite1_1, 0, y, 0);

	y += 80;

	//------------------------------------------------------------
	// カラースケール
	//		カラースケールは描画時に RGBA 各色成分に乗算される値で、
	//		色の強さを決めます。

	// R=100%, G=0%, B=0%, A=100%
	lnColorF color4 = { 1.0, 0, 0, 1.0 };
	lnHandle sprite4;
	LNSprite_Create(&sprite4, tex1);
	LNSceneNode_SetPositionXYZ(sprite4, 0, y, 0);
	LNVisualNode_SetColorScale(sprite4, 0, &color4);

	// R=25%, G=100%, B=50%, A=75%
	lnColorF color5 = { 0.25, 1.0, 0.5, 0.75 };
	lnHandle sprite5;
	LNSprite_Create(&sprite5, tex1);
	LNSceneNode_SetPositionXYZ(sprite5, 50, y, 0);
	LNVisualNode_SetColorScale(sprite5, 0, &color5);

	y += 80;

	//------------------------------------------------------------
	// ブレンドカラー
	//		ブレンドカラーは描画時にイメージを塗りつぶす色です。
	//		ブレンドの割合にはアルファ値が使用されます。

	// 白50%
	lnColorF color3_1 = { 1.0, 1.0, 1.0, 0.5 };
	lnHandle sprite3_1;
	LNSprite_Create(&sprite3_1, tex1);
	LNSceneNode_SetPositionXYZ(sprite3_1, 0, y, 0);
	LNVisualNode_SetBlendColor(sprite3_1, 0, &color3_1);

	// 白75%
	lnColorF color3_2 = { 1.0, 1.0, 1.0, 0.75 };
	lnHandle sprite3_2;
	LNSprite_Create(&sprite3_2, tex1);
	LNSceneNode_SetPositionXYZ(sprite3_2, 50, y, 0);
	LNVisualNode_SetBlendColor(sprite3_2, 0, &color3_2);

	// 白100%
	lnColorF color3_3 = { 1.0, 1.0, 1.0, 1.0 };
	lnHandle sprite3_3;
	LNSprite_Create(&sprite3_3, tex1);
	LNSceneNode_SetPositionXYZ(sprite3_3, 100, y, 0);
	LNVisualNode_SetBlendColor(sprite3_3, 0, &color3_3);

	// 赤50%
	lnColorF color3_4 = { 1.0, 0.0, 0.0, 0.5 };
	lnHandle sprite3_4;
	LNSprite_Create(&sprite3_4, tex1);
	LNSceneNode_SetPositionXYZ(sprite3_4, 150, y, 0);
	LNVisualNode_SetBlendColor(sprite3_4, 0, &color3_4);

	// 緑75%
	lnHandle sprite3_5;
	lnColorF color3_5 = { 0.0, 1.0, 0.0, 0.75 };
	LNSprite_Create(&sprite3_5, tex1);
	LNSceneNode_SetPositionXYZ(sprite3_5, 200, y, 0);
	LNVisualNode_SetBlendColor(sprite3_5, 0, &color3_5);

	// 青100%
	lnColorF color3_6 = { 0.0, 0.0, 1.0, 1.0 };
	lnHandle sprite3_6;
	LNSprite_Create(&sprite3_6, tex1);
	LNSceneNode_SetPositionXYZ(sprite3_6, 250, y, 0);
	LNVisualNode_SetBlendColor(sprite3_6, 0, &color3_6);

	y += 80;

	//------------------------------------------------------------
	// 色調
	//		色調はRGB各色を調整する値です。(不透明度には影響しません)
	//		-1.0～1.0の範囲で指定します。

	// 赤
	lnTone tone4_1 = { 1.0, 0.0, 0.0, 0.0 };
	lnHandle sprite4_1;
	LNSprite_Create(&sprite4_1, tex1);
	LNSceneNode_SetPositionXYZ(sprite4_1, 0, y, 0);
	LNVisualNode_SetTone(sprite4_1, 0, &tone4_1);

	// 青
	lnTone tone4_2 = { 0.0, 1.0, 0.0, 0.0 };
	lnHandle sprite4_2;
	LNSprite_Create(&sprite4_2, tex1);
	LNSceneNode_SetPositionXYZ(sprite4_2, 50, y, 0);
	LNVisualNode_SetTone(sprite4_2, 0, &tone4_2);

	// 緑
	lnTone tone4_3 = { 0.0, 0.0, 1.0, 0.0 };
	lnHandle sprite4_3;
	LNSprite_Create(&sprite4_3, tex1);
	LNSceneNode_SetPositionXYZ(sprite4_3, 100, y, 0);
	LNVisualNode_SetTone(sprite4_3, 0, &tone4_3);

	// モノクロ
	lnTone tone4_4 = { 0.0, 0.0, 0.0, 1.0 };
	lnHandle sprite4_4;
	LNSprite_Create(&sprite4_4, tex1);
	LNSceneNode_SetPositionXYZ(sprite4_4, 150, y, 0);
	LNVisualNode_SetTone(sprite4_4, 0, &tone4_4);

	// セピア調
	lnTone tone4_5 = { 0.0, -0.5, -0.9, 1.0 };
	lnHandle sprite4_5;
	LNSprite_Create(&sprite4_5, tex1);
	LNSceneNode_SetPositionXYZ(sprite4_5, 200, y, 0);
	LNVisualNode_SetTone(sprite4_5, 0, &tone4_5);

	y += 80;

	//------------------------------------------------------------
	// 合成方法
	//		描画先イメージとの合成方法です。

	// 加算
	lnHandle sprite5_1;
	LNSprite_Create(&sprite5_1, tex1);
	LNSceneNode_SetPositionXYZ(sprite5_1, 0, y, 0);
	LNVisualNode_SetBlendMode(sprite5_1, LN_BLEND_ADD);

	// 減算
	lnHandle sprite5_2;
	LNSprite_Create(&sprite5_2, tex1);
	LNSceneNode_SetPositionXYZ(sprite5_2, 50, y, 0);
	LNVisualNode_SetBlendMode(sprite5_2, LN_BLEND_SUB);

	// 乗算
	lnHandle sprite5_3;
	LNSprite_Create(&sprite5_3, tex1);
	LNSceneNode_SetPositionXYZ(sprite5_3, 100, y, 0);
	LNVisualNode_SetBlendMode(sprite5_3, LN_BLEND_MUL);

	// リバース
	lnHandle sprite5_4;
	LNSprite_Create(&sprite5_4, tex1);
	LNSceneNode_SetPositionXYZ(sprite5_4, 150, y, 0);
	LNVisualNode_SetBlendMode(sprite5_4, LN_BLEND_REVERSE);


	//------------------------------------------------------------
	// 以下、説明文
	lnColor fontColor = { 255, 255, 255, 255 };
	lnHandle font1;
	LNFont_Create(&font1);
	LNFont_SetColor(font1, &fontColor);
	LNFont_SetBold(font1, LN_TRUE);

	lnHandle textTexture;
	lnHandle textSprite;
	LNTexture_Create(&textTexture, 640, 480);
	LNSprite_Create(&textSprite, textTexture);

	lnRect rect = { 0, 40, 640, 480 };
	LNTexture_DrawText(textTexture, "オリジナルのイメージ", &rect, font1);
	rect.Y += 80;
	LNTexture_DrawText(textTexture, "カラースケール (RGBA 各色成分の強さ)", &rect, font1);
	rect.Y += 80;
	LNTexture_DrawText(textTexture, "ブレンドカラー (塗りつぶし色とその強さ)", &rect, font1);
	rect.Y += 80;
	LNTexture_DrawText(textTexture, "色調", &rect, font1);
	rect.Y += 80;
	LNTexture_DrawText(textTexture, "合成方法", &rect, font1);

	// メインループ
	lnBool endRequested;
	{
		LNCore_Update();
		LNCore_IsEndRequested(&endRequested);
	} while (endRequested == LN_FALSE);

	LNCore_Terminate();
	return 0;
}