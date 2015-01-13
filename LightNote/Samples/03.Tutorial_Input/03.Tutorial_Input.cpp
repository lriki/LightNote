//==============================================================================
// ユーザー入力のチュートリアル
//------------------------------------------------------------------------------
/*  
	キーボード・マウス・ジョイパッドからの入力を受け取ります。

	操作方法
	・↑↓キー	: 前進・後退
	・←→キー	: 左を向く・右を向く
	・Z キー	: 押すたびに加速 ON/OFF を切り替える

	LightNote は「仮想コントローラ」という仕組みで入力情報を管理しています。
	PCには様々な入力デバイスを接続することができますが、
	仮想コントローラはそれらの差を意識することなく入力を受け取るシステムです。

	名前は大げさですが内部で行っていることは単純で、
	例えば「左ボタン」が押されているかをチェックしたいときは、
	LNInput_IsPress(LN_BUTTON_LEFT) といったように記述するだけで
	接続されている キーボード、ゲームパッド、マウスの全てのキー・ボタンがチェックされ、
	「左ボタン」に相当するものが押下されているかを確認できます。
*/
//==============================================================================

#include <lnote.h>
#include <math.h>

int main()
{
	LNBool flag;

	LNCore_Initialize();

	float x = 320.0;
	float y = 240.0;
	float dirAngle = -3.1415 / 2;

	// 画像を表示する準備
	LNHandle texture;
	LNHandle sprite;
	LNTexture_CreateFromFile(&texture, "../Media/Arrow1.png");
	LNSprite_Create(&sprite, texture);

	// 加速中であるか
	bool isBoost = false;
	
	// メインループ
	do
	{
		float velocity = 2.0;
		if ( isBoost )
			velocity *= 2.0;

		// LNInput_IsPress を使用して、あるボタンが押されているかをチェックします。
		// この関数はキーボード/マウス/ジョイパッド等様々な入力デバイスの状態から、ボタンが押されているかを確認します。
		// 例えば、「左ボタン」を確認する定数 LN_BUTTON_LEFT を渡すと、以下のいずれかの入力がある場合に LN_TRUE を出力します。
		// ・キーボードの ←キー が押下されている
		// ・ジョイパッドの POV が左方向に入力されている
		// ・ジョイパッドの第1スティックが左に入力されている

		// 左を向く ('←' キー)
		LNInput_IsPress(LN_BUTTON_LEFT, &flag);
		if (flag == LN_TRUE)
		{
			dirAngle -= 0.1;
		}
		// 右を向く ('→' キー)
		LNInput_IsPress(LN_BUTTON_RIGHT, &flag);
		if (flag == LN_TRUE)
		{
			dirAngle += 0.1;
		}
		// 前進 ('↑' キー)
		LNInput_IsPress(LN_BUTTON_UP, &flag);
		if (flag == LN_TRUE)
		{
			x += cosf(dirAngle) * velocity;
			y += sinf(dirAngle) * velocity;
		}
		// 後退 ('↓' キー)
		LNInput_IsPress(LN_BUTTON_DOWN, &flag);
		if (flag == LN_TRUE)
		{
			x -= cosf(dirAngle) * velocity;
			y -= sinf(dirAngle) * velocity;
		}
		// ボタンを押すたびに加速 ON/OFF を切り替える ('Z' キー)
		LNInput_IsTrigger(LN_BUTTON_0, &flag);
		if (flag == LN_TRUE)
		{
			isBoost = !isBoost;
		}

		// ボタン定数とデバイスのボタンは、デフォルトでは以下のようにマッピングされています。
		// 変更したい時は「KeyConfig」サンプルを参考にしてください。
		/*
		仮想ボタン名	定数名			キーボード	マウス	ジョイパッド
		0番 ボタン		LN_BUTTON_0		'Z' キー	-		0 番ボタン
		1番 ボタン		LN_BUTTON_1		'X' キー	-		1 番ボタン
		2番 ボタン		LN_BUTTON_2		'C' キー	-		2 番ボタン
		3番 ボタン		LN_BUTTON_3		'A' キー	-		3 番ボタン
		4番 ボタン		LN_BUTTON_4		'S' キー	-		4 番ボタン
		5番 ボタン		LN_BUTTON_5		'D' キー	-		5 番ボタン
		6番 ボタン		LN_BUTTON_6		'Q' キー	-		6 番ボタン
		7番 ボタン		LN_BUTTON_7		'W' キー	-		7 番ボタン
		8番 ボタン		LN_BUTTON_8		-			-		8 番ボタン
		9番 ボタン		LN_BUTTON_9		-			-		9 番ボタン
		10番 ボタン		LN_BUTTON_10	-			-		10 番ボタン
		11番 ボタン		LN_BUTTON_11	-			-		11 番ボタン
		12番 ボタン		LN_BUTTON_12	-			-		12 番ボタン
		13番 ボタン		LN_BUTTON_13	-			-		13 番ボタン
		14番 ボタン		LN_BUTTON_14	-			-		14 番ボタン
		15番 ボタン		LN_BUTTON_15	-			-		15 番ボタン
		左 ボタン		LN_BUTTON_LEFT	'←' キー	-		POV左 / 第1軸-
		右 ボタン		LN_BUTTON_RIGHT	'→' キー	-		POV右 / 第1軸+
		上 ボタン		LN_BUTTON_UP	'↑' キー	-		POV上 / 第2軸-
		下 ボタン		LN_BUTTON_DOWN	'↓' キー	-		POV下 / 第2軸+
		*/

		// 加速中は赤い矢印画像の部分を転送する
		if ( isBoost )
			LNSprite_SetSrcRectXYWH( sprite, 64, 0, 64, 64 );
		else
			LNSprite_SetSrcRectXYWH(sprite, 0, 0, 64, 64);

		// スプライトの姿勢を設定
		LNSceneNode_SetCenterXYZ( sprite, 32, 32, 0 );	// 64x64px の中心を移動と回転の中心にする
		LNSceneNode_SetPositionXYZ(sprite, x, y, 0);
		LNSceneNode_SetAngleXYZ(sprite, 0, 0, dirAngle);

		LNCore_Update();	// 入力情報の更新はここで自動的に行われます
		LNCore_IsEndRequested(&flag);
	} while (flag == LN_FALSE);

	LNCore_Terminate();
	return 0;
}