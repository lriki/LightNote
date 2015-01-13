//==============================================================================
// サンプルゲーム - シューティング
//------------------------------------------------------------------------------
/*
	ルール
		自機を操作して弾を撃ち、より多く敵を落とす事が目標です。
		敵の弾に接触すると自機のHPが減少し、0 になるとゲームオーバーとなり、
		その時点のスコアが記録されます。

	操作方法
		キーボード
			方向キー          … 自機の移動
			Z キー            … 弾の発射
		ジョイパッド
			第1スティック/POV … 自機の移動
			0番ボタン         … 弾の発射
*/
//==============================================================================

#include <math.h>
#include <lnote.h>

#define DATA_PATH(name) "../Media/Game1_Shooting/"name
#define PI 3.14159265359

// 自機に関する情報をまとめた構造体
struct Player
{
	int			X;				// X座標
	int			Y;				// Y座標
	int			HP;				// ヒットポイント
	int			ShotInterval;	// 連射感覚 (次の発射までの待ちフレーム数)
	LNHandle	Sprite;			// スプライト
};

// 敵に関する情報をまとめた構造体
struct Enemy
{
	float		X;				// X座標
	float		Y;				// Y座標
	float		Velocity;		// 速度
	float		HP;				// ヒットポイント
	bool		DamageEffect;	// ダメージエフェクトとして、一瞬フラッシュさせる
	bool		Deleted;		// 消滅しているか
	int			ShotInterval;	// 連射感覚 (次の発射までの待ちフレーム数)
	LNHandle	Sprite;			// スプライト
};

// 弾を発射した人
enum BulletOwner
{
	BulletOwner_Player = 0,
	BulletOwner_Enemy,
};

// 弾に関する情報をまとめた構造体
struct Bullet
{
	float			X;			// X座標
	float			Y;			// Y座標
	float			Angle;		// 進行方向
	float			Velocity;	// 速度
	BulletOwner		Owner;		// 自機が発射した弾なのか、敵が発射した弾なのか
	bool			Deleted;	// 消滅しているか
	LNHandle		Sprite;		// スプライト
};

const int MAX_ENEMIES = 100;	// 敵の最大数
const int MAX_BULLETS = 1000;	// 弾の最大数

Player		g_Player;						// プレイヤー
Enemy		g_Enemies[MAX_ENEMIES] = { 0 };	// 全ての敵の管理配列
Bullet		g_Bullets[MAX_BULLETS] = { 0 };	// 全ての弾の管理配列
int			g_GameFrameCount = 0;			// ゲーム開始からの経過フレーム数
int			g_Score = 0;					// スコア (撃墜数)
int			g_HighScore = 0;				// ハイスコア
LNHandle	g_UITexture = NULL;				// UI 表示用テクスチャ
LNHandle	g_UISprite = NULL;				// UI 表示用スプライト

// 関数プロトタイプ
void Player_Init();
void Player_Update();
void Enemies_Init();
void Enemies_Update();
void Enemy_Create(int x, int y);
void Enemy_Delete(Enemy* enemy);
void Bullets_Init();
void Bullets_Update();
void Bullet_Create(int x, int y, float angle, float velocity, BulletOwner owner);
void Bullet_Delete(Bullet* bullet);
bool Collision_DetectCircles(float x1, float y1, float r1, float x2, float y2, float r2);
void Score_Init();
void Score_Add();
void Score_Save();

//---------------------------------------------------------------------
// 自機の初期化
//---------------------------------------------------------------------
void Player_Init()
{
	// プレイヤーの初期位置
	g_Player.X = 180;
	g_Player.Y = 400;

	g_Player.HP = 10;
	g_Player.ShotInterval = 0;

	// スプライトの作成
	LNHandle tex;
	LNTexture_CreateFromFile(&tex, DATA_PATH("Images1.png"));
	LNSprite_Create(&g_Player.Sprite, tex);
	LNSprite_SetSrcRectXYWH(g_Player.Sprite, 0, 0, 32, 32);
	LNSceneNode_SetCenterXYZ(g_Player.Sprite, 16, 16, 0);	// 32x32 画像の中心をスプライトの中心にする
}

//---------------------------------------------------------------------
// 自機のフレーム更新
//---------------------------------------------------------------------
void Player_Update()
{
	// 方向キーの入力を受け取る
	LNBool state;
	LNInput_IsPress(LN_BUTTON_LEFT, &state);
	if (state)
		g_Player.X -= 2;
	LNInput_IsPress(LN_BUTTON_RIGHT, &state);
	if (state)
		g_Player.X += 2;
	LNInput_IsPress(LN_BUTTON_UP, &state);
	if (state)
		g_Player.Y -= 2;
	LNInput_IsPress(LN_BUTTON_DOWN, &state);
	if (state)
		g_Player.Y += 2;

	// 画面から出ないように移動範囲を制限
	if (g_Player.X < 0) g_Player.X = 0;
	if (g_Player.X > 328) g_Player.X = 328;
	if (g_Player.Y < 0) g_Player.Y = 0;
	if (g_Player.Y > 448) g_Player.Y = 448;

	// スプライトの位置を設定
	LNSceneNode_SetPositionXYZ(g_Player.Sprite, g_Player.X, g_Player.Y, 0);

	// 弾の発射
	LNInput_IsPress(LN_BUTTON_0, &state);
	if (state && g_Player.ShotInterval == 0)
	{
		Bullet_Create(g_Player.X, g_Player.Y, -PI * 0.6, 10, BulletOwner_Player);
		Bullet_Create(g_Player.X, g_Player.Y, -PI * 0.4, 10, BulletOwner_Player);
		Bullet_Create(g_Player.X, g_Player.Y, -PI * 0.5, 10, BulletOwner_Player);
		
		LNAudio_PlaySE(DATA_PATH("shot.wav"), 40, 150);

		g_Player.ShotInterval = 5;	// 次は5フレーム後に発射できる
	}
	if (g_Player.ShotInterval > 0)
		g_Player.ShotInterval--;
}

//---------------------------------------------------------------------
// 全ての敵の初期化
//---------------------------------------------------------------------
void Enemies_Init()
{
	// 最初はすべて消滅していることにする
	for (int i = 0; i < MAX_ENEMIES; i++)
		g_Enemies[i].Deleted = true;
}

//---------------------------------------------------------------------
// 全ての敵の更新
//---------------------------------------------------------------------
void Enemies_Update()
{
	// 30 フレームに1度、新しい敵を作る
	if (g_GameFrameCount % 30 == 0) {
		float x;
		LNRandom_GetFloatRange(NULL, 32, 328, &x);
		Enemy_Create(x, 0);
	}

	// 全ての敵の更新
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		// 生きている敵ならば
		if (g_Enemies[i].Deleted == false)
		{
			Enemy* enemy = &g_Enemies[i];

			// 下へ移動
			enemy->Y += enemy->Velocity;

			// スプライトの位置・角度を設定
			LNSceneNode_SetPositionXYZ(enemy->Sprite, enemy->X, enemy->Y, 0);

			// ダメージエフェクトの処理
			if (enemy->DamageEffect)
			{
				LNColorF c = { 1, 1, 1, 0.75 };
				LNVisualNode_SetBlendColor(enemy->Sprite, 0, &c);
			}
			else
			{
				LNColorF c = { 0, 0, 0, 0 };
				LNVisualNode_SetBlendColor(enemy->Sprite, 0, &c);
			}
			enemy->DamageEffect = false;

			// 画面から出た場合は消す
			if (enemy->X < -16 || enemy->X > 376 ||
				enemy->Y < -16 || enemy->Y > 496)
			{
				Enemy_Delete(enemy);
			}

			// 弾を撃つ
			if (enemy->ShotInterval == 0)
			{
				// 自機狙い
				float angle = atan2(g_Player.Y - enemy->Y, g_Player.X - enemy->X);
				Bullet_Create(enemy->X, enemy->Y, angle, 3, BulletOwner_Enemy);

				enemy->ShotInterval = 120;	// 次は2秒ごとに発射する
			}
			if (enemy->ShotInterval > 0)
				enemy->ShotInterval--;
		}
	}
}

//---------------------------------------------------------------------
// 新しい敵の作成
//---------------------------------------------------------------------
void Enemy_Create(int x, int y)
{
	// g_Bullets 内の空いている弾を探す
	Enemy* enemy = NULL;
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (g_Enemies[i].Deleted == true)
		{
			enemy = &g_Enemies[i];
			break;
		}
	}
	if (enemy != NULL)
	{
		enemy->X = x;
		enemy->Y = y;
		LNRandom_GetFloatRange(NULL, 1, 3, &enemy->Velocity);
		enemy->HP = 10;
		enemy->ShotInterval = 30;
		enemy->Deleted = false;

		LNHandle tex;
		LNTexture_CreateFromFile(&tex, DATA_PATH("Images1.png"));
		LNSprite_Create(&enemy->Sprite, tex);
		LNSprite_SetSrcRectXYWH(enemy->Sprite, 32, 0, 32, 32);
		LNSceneNode_SetCenterXYZ(enemy->Sprite, 16, 16, 0);	// 32x32 画像の中心をスプライトの中心にする
	}
}

//---------------------------------------------------------------------
// 敵を削除する
//---------------------------------------------------------------------
void Enemy_Delete(Enemy* enemy)
{
	enemy->Deleted = true;
	LNObject_Release(enemy->Sprite);
}

//---------------------------------------------------------------------
// 全ての弾の初期化
//---------------------------------------------------------------------
void Bullets_Init()
{
	// 最初はすべて消滅していることにする
	for (int i = 0; i < MAX_BULLETS; i++)
		g_Bullets[i].Deleted = true;
}

//---------------------------------------------------------------------
// 全ての弾の更新
//---------------------------------------------------------------------
void Bullets_Update()
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		Bullet* bullet = &g_Bullets[i];
		if (!bullet->Deleted)
		{
			// 進行方向へ移動
			float vx = bullet->Velocity * cos(bullet->Angle);
			float vy = bullet->Velocity * sin(bullet->Angle);
			bullet->X += vx;
			bullet->Y += vy;

			// スプライトの位置・角度を設定
			LNSceneNode_SetPositionXYZ(bullet->Sprite, bullet->X, bullet->Y, 0);
			LNSceneNode_SetAngleXYZ(bullet->Sprite, 0, 0, bullet->Angle + PI * 0.5);

			// 画面から出た場合は消す
			if (bullet->X < -16 || bullet->X > 376 ||
				bullet->Y < -16 || bullet->Y > 496)
			{
				Bullet_Delete(bullet);
			}

			// 自機が撃った弾は敵と衝突判定する
			if (bullet->Owner == BulletOwner_Player)
			{
				for (int i = 0; i < MAX_ENEMIES; i++)
				{
					Enemy* enemy = &g_Enemies[i];
					if (!enemy->Deleted)
					{
						// 弾の半径は3px、敵の半径は12pxとする
						if (Collision_DetectCircles(bullet->X, bullet->Y, 3, enemy->X, enemy->Y, 12)) 
						{
							// 弾を削除
							Bullet_Delete(bullet);

							// 敵にダメージを与え、倒したらスコアを増やす
							enemy->HP--;
							enemy->DamageEffect = true;		// ヒットエフェクトを表示する
							if (enemy->HP == 0) {
								Enemy_Delete(enemy);
								Score_Add();
							}
							break;
						}
					}
				}
			}
			// 敵が撃った弾は自機と衝突判定する
			else if (bullet->Owner == BulletOwner_Enemy)
			{
				// 弾の半径は3px、自機の半径は5pxとする
				if (Collision_DetectCircles(bullet->X, bullet->Y, 3, g_Player.X, g_Player.Y, 5))
				{
					// 弾を削除
					Bullet_Delete(bullet);

					// 自機にダメージを与える
					g_Player.HP--;
				}
			}
		}
	}
}

//---------------------------------------------------------------------
// 新しい弾の作成
//---------------------------------------------------------------------
void Bullet_Create(int x, int y, float angle, float velocity, BulletOwner owner)
{
	// g_Bullets 内の空いている弾を探す
	Bullet* bullet = NULL;
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (g_Bullets[i].Deleted == true)
		{
			bullet = &g_Bullets[i];
			break;
		}
	}
	if (bullet != NULL)
	{
		bullet->X = x;
		bullet->Y = y;
		bullet->Angle = angle;
		bullet->Velocity = velocity;
		bullet->Owner = owner;
		bullet->Deleted = false;
		LNHandle tex;
		LNTexture_CreateFromFile(&tex, DATA_PATH("Images1.png"));
		LNSprite_Create(&bullet->Sprite, tex);
		LNSceneNode_SetCenterXYZ(bullet->Sprite, 16, 16, 0);	// 32x32 画像の中心をスプライトの中心にする

		// 自機・敵のどちらが発射したかによって表示画像を変える
		if (owner == BulletOwner_Player)
			LNSprite_SetSrcRectXYWH(bullet->Sprite, 64, 0, 32, 32);
		else
			LNSprite_SetSrcRectXYWH(bullet->Sprite, 96, 0, 32, 32);
	}
	else {
		// 見つからなかった。弾は発射できない
	}
}

//---------------------------------------------------------------------
// 弾を削除する
//---------------------------------------------------------------------
void Bullet_Delete(Bullet* bullet)
{
	bullet->Deleted = true;
	LNObject_Release(bullet->Sprite);
}

//---------------------------------------------------------------------
// 2つの円の衝突判定を行う
//---------------------------------------------------------------------
bool Collision_DetectCircles(float x1, float y1, float r1, float x2, float y2, float r2)
{
	LNVector2 d = { x1 - x2, y1 - y2 };
	float length;
	LNVector2_GetLength(&d, &length);
	return (length <= (r1 + r2));
}

//---------------------------------------------------------------------
// UIパーツ (スコア/HP表示) の初期化
//---------------------------------------------------------------------
void UI_Init()
{
	// 画面全体に覆いかぶせるようにスプライトを作成する。このテクスチャに文字を書き込んでいく
	LNTexture_Create(&g_UITexture, 360, 480);
	LNSprite_Create(&g_UISprite, g_UITexture);

	// Z 値を指定することで、他のスプライトよりも手前に表示する
	LNSceneNode_SetPositionXYZ(g_UISprite, 0, 0, 100);
}

//---------------------------------------------------------------------
// UIパーツ (スコア/HP表示) の更新
//---------------------------------------------------------------------
void UI_Update()
{
	LNTexture_ClearRGBA(g_UITexture, 0, 0, 0, 0);

	// 自機のHPを描画
	char text[256];
	sprintf(text, "HP : %d", g_Player.HP);
	LNTexture_DrawTextXY(g_UITexture, text, 8, 8);

	// スコア・ハイスコアを描画 (右揃え)
	sprintf(text, "SCORE : %4d / %4d", g_Score, g_HighScore);
	LNRect drawArea = { 0, 8, 352, 100 };
	LNTexture_DrawText(g_UITexture, text, &drawArea, NULL, -1, LN_TEXTALIGN_RIGHT);
}

//---------------------------------------------------------------------
// スコアの初期化
//---------------------------------------------------------------------
void Score_Init()
{
	g_Score = 0;
	g_HighScore = 10;

	// score ファイルが存在すればハイスコアを読み取る
	LNBool exists;
	LNFile_Exists("score.dat", &exists);
	if (exists) {
		LNHandle file;
		LNFile_Create(&file, "score.dat", LN_FILEMODE_OPEN, LN_FILEACCESS_READ);
		LNFile_Read(file, &g_HighScore, sizeof(int), NULL);
		LNFile_Close(file);
	}
}

//---------------------------------------------------------------------
// スコアを増やす
//---------------------------------------------------------------------
void Score_Add()
{
	g_Score++;

	// ハイスコア更新
	if (g_Score > g_HighScore)
		g_HighScore = g_Score;
}

//---------------------------------------------------------------------
// ハイスコアの保存
//---------------------------------------------------------------------
void Score_Save()
{
	LNHandle file;
	LNFile_Create(&file, "score.dat", LN_FILEMODE_CREATE, LN_FILEACCESS_WRITE);
	LNFile_Write(file, &g_HighScore, sizeof(int));
	LNFile_Close(file);
}

//---------------------------------------------------------------------
// main
//---------------------------------------------------------------------
int main()
{
	LNConfig_SetWindowTitle("サンプルゲーム");
	LNConfig_SetWindowSize(360, 480);
	LNConfig_SetDirectMusicInitMode(LN_DMINITMODE_NORMAL);
	LNConfig_SetAcceleratorKey(LN_ACCELERATORKEY_SHOW_DEBUG_STATISTICS, LN_KEY_F3, LN_FALSE, LN_FALSE, LN_FALSE);

	if (LNCore_Initialize() != LN_OK)
		return -1;

	Player_Init();				// 自機の初期化
	Enemies_Init();				// 全ての敵の初期化
	Bullets_Init();				// 全ての弾の初期化
	UI_Init();					// UI の初期化
	Score_Init();				// スコアの初期化

	// BGM 再生
	LNAudio_PlayBGM(DATA_PATH("ln29_SampleBGM.mid"), 100, 100);

	// メインループ
	LNBool endRequested;
	do
	{
		Player_Update();		// 自機の更新
		Enemies_Update();		// 全ての敵の更新
		Bullets_Update();		// 全ての弾の更新
		UI_Update();			// UI の更新

		g_GameFrameCount++;

		LNCore_Update();
		LNCore_IsEndRequested(&endRequested);
	} while (!endRequested && g_Player.HP > 0);	// 自機が落とされたら終了

	Score_Save();				// ハイスコアの保存

	LNCore_Terminate();
	return 0;
}
