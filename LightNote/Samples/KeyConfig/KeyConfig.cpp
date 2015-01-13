//==============================================================================
// キーコンフィグのサンプル
//------------------------------------------------------------------------------
/*  
	アクションゲームを想定した、攻撃ボタンとジャンプボタンのコンフィグを行うサンプルです。
	基本的な入力についは 03.Tutorial_Input.cpp をご覧ください。

	・LNInput と LNVirtualPad について

		LNVirtualPad モジュールはハンドル1つで仮想コントローラ1つを表します。
		例えば家庭用ゲーム機ではコントローラを 1P～4P 接続することができますが、
		LightNote ではこれをイメージして、初期化時に最大4つの仮想コントローラが作られます。

		デバイスは以下のように割り当てられます。
		・1P	キーボード, マウス, 0番ジョイパッド
		・2P	1番ジョイパッド
		・3P	2番ジョイパッド
		・4P	3番ジョイパッド

		LNInput は、このうち 1P 用の仮想コントローラからの入力を受け取るユーティリティです。

	・キーコンフィグの流れ
	
		①
*/
//==============================================================================

#include <lnote.h>

#define PI 3.1415f

void MakeButtonName( char* outName, int deviceButton );

// LN_BUTTON_0 を攻撃ボタン、LN_BUTTON_1 をジャンプボタンとしています。
#define ATTACK_BUTTON	LN_BUTTON_0
#define JUMP_BUTTON		LN_BUTTON_1

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------
int main()
{
	LNCore_Initialize();

	// エフェクトを見易くするため背景を黒にする
	LNColorF color1 = { 0.0, 0.0, 0.0, 1.0 };
	LNHandle viewPane;
	LNViewPane_GetDefaultViewPane(&viewPane);
	LNViewPane_SetBackgroundColor(viewPane, &color1);

	// キャラクター画像を用意
	LNHandle texture1;
	LNHandle sprite1;
	LNTexture_CreateFromFile(&texture1, "../Media/Circle1.png");
	LNSprite_Create(&sprite1, texture1);
	LNSprite_SetSize(sprite1, 1, 1);
	// キャラクター用スプライトを 3D 空間に配置する
	LNHandle rootNode3D;
	LNSceneNode_GetDefault3DRootNode(&rootNode3D);
	LNSceneNode_AddChild(rootNode3D, sprite1);

	// 攻撃用に使用するエフェクトを用意 (エフェクトは 3D 空間に配置される)
	LNHandle effect1;
	LNEffect_Create(&effect1, "../Media/Laser01.efk");
	LNSceneNode_SetAngleXYZ(effect1, 0, PI / 2, 0);
	LNVisualNode_SetEnableDepthWrite(effect1, LN_FALSE);
	LNVisualNode_SetEnableDepthTest(effect1, LN_FALSE);

	// カメラを全体が見やすい位置に移動
	LNHandle camera;
	LNCamera_GetDefault3DCamera(&camera);
	LNCamera_SetCameraType(camera, LN_CAMERATYPE_POSITION_AND_ANGLE);
	LNCamera_SetPositionXYZ(camera, 5, 0, -30);

	// 文字列を描画するテクスチャを用意
	LNHandle infoTexture;
	LNHandle infoSprite;
	LNTexture_Create(&infoTexture, 640, 480);
	LNSprite_Create(&infoSprite, infoTexture);

	LNRect rect = { 0, 0, 640, 480 };
	LNTexture_DrawText( infoTexture, "各行動に割り当てるボタンを押してください。", &rect );
	rect.Y += 24;
	LNTexture_DrawText( infoTexture, "攻撃ボタン     > ", &rect );

	// 1P を示す仮想コントローラの取得
	LNHandle pad;
	LNVirtualPad_GetVirtualPad(0, &pad);

	int step = 0;
	float y = 0.0;
	float velocity = 0.0;

	// メインループ
	LNBool endRequested;
	do
	{
		switch ( step )
		{
			// キーコンフィグ中 (攻撃ボタン)
			case 0:
			{
				// 何らかのボタンが新しく押された瞬間を検出
				int deviceButton;
				LNVirtualPad_GetPressedDeviceButton(pad, &deviceButton);
				if ( deviceButton != 0 )
				{
					// 現在 ATTACK_BUTTON に割り当てられているデバイスボタンを全て解除し、
					// 新しく押されたデバイスボタンを登録する
					LNVirtualPad_DetachVirtualButton(pad, ATTACK_BUTTON, 0);
					LNVirtualPad_AttachVirtualButton(pad, ATTACK_BUTTON, deviceButton);

					// ボタン名を描画する
					char tmp[1000];
					MakeButtonName( tmp, deviceButton );
					rect.X = 160;
					rect.Y = 24;
					LNTexture_DrawText(infoTexture, tmp, &rect);
					
					// 次に入力する行動名を描画
					rect.X = 0;
					rect.Y = 48;
					LNTexture_DrawText( infoTexture, "ジャンプボタン > ", &rect );

					// 次のステップへ
					step = 1;
				}

				break;
			}
			// キーコンフィグ中 (ジャンプボタン)
			case 1:
			{
				// 何らかのボタンが新しく押された瞬間を検出
				int deviceButton;
				LNVirtualPad_GetPressedDeviceButton(pad, &deviceButton);
				if (deviceButton != 0)
				{
					// 現在 JUMP_BUTTON に割り当てられているデバイスボタンを全て解除し、
					// 新しく押されたデバイスボタンを登録する
					LNVirtualPad_DetachVirtualButton(pad, JUMP_BUTTON, 0);
					LNVirtualPad_AttachVirtualButton(pad, JUMP_BUTTON, deviceButton);

					// ボタン名を描画する
					char tmp[1000];
					MakeButtonName( tmp, deviceButton );
					rect.X = 160;
					rect.Y = 48;
					LNTexture_DrawText(infoTexture, tmp, &rect);

					// 次のステップへ
					step = 2;
				}
				break;
			}
			// 実行中
			case 2:
			{
				LNBool flag;
				// 攻撃ボタンが押された
				LNVirtualPad_IsTrigger(pad, ATTACK_BUTTON, &flag);
				if (flag)
				{
					LNEffect_Play( effect1, LN_TRUE );
				}
				// ジャンプボタンが押された
				LNVirtualPad_IsTrigger(pad, JUMP_BUTTON, &flag);
				if (flag)
				{
					velocity = 0.3f;
				}
				break;
			}
		}

		// 速度・位置計算
		velocity -= 0.03f;
		y += velocity;
		if ( y < 0.0 )
		{
			y = 0.0;
			velocity = 0.0;
		}

		LNSceneNode_SetPositionXYZ(sprite1, 0, y, 0);
		LNSceneNode_SetPositionXYZ(effect1, 1, y, 0);

		LNCore_Update();
		LNCore_IsEndRequested(&endRequested);
	} while (endRequested == LN_FALSE);

	LNCore_Terminate();
	return 0;
}

//----------------------------------------------------------------------
// デバイスボタン番号から種類と番号の文字列を作成する
//----------------------------------------------------------------------
void MakeButtonName( char* outName, int deviceButton )
{
	LNDeviceButtonType type;
	LNVirtualPad_GetDeviceButtonType(deviceButton, &type);
	switch ( type )
	{
		// キーボード
		case LN_DEVICEBUTTONTYPE_KEYBOARD:		
			sprintf( outName, "キーボード %d", deviceButton );	// (deviceButton は lnKeyCode である)
			break;
		// マウスボタン
		case LN_DEVICEBUTTONTYPE_MOUSE:			
			sprintf( outName, "マウスボタン %d", deviceButton - LN_DEVICEBUTTON_MOUSE_1 );
			break;
		// ジョイパッドのボタン
		case LN_DEVICEBUTTONTYPE_JOYPAD_BUTTON:	
			sprintf( outName, "ジョイパッドのボタン %d", deviceButton - LN_DEVICEBUTTON_JOYPAD_BUTTON_1 );
			break;
		// ジョイパッドのPOV
		case LN_DEVICEBUTTONTYPE_JOYPAD_POV:	
			sprintf( outName, "ジョイパッドのPOV %d", deviceButton - LN_DEVICEBUTTON_JOYPAD_POV_UP );
			break;
		// ジョイパッドの軸
		case LN_DEVICEBUTTONTYPE_JOYPAD_AXIS:	
			sprintf( outName, "ジョイパッドの軸 %d", deviceButton - LN_DEVICEBUTTON_JOYPAD_AXIS_1_MINUIS );
			break;
		// 不明なボタン
		default:
			sprintf( outName, "不明なボタン %d", deviceButton );
			break;
	}
}
