//==============================================================================
// スプライトパスのサンプル
//------------------------------------------------------------------------------
/*	
	スプライトパスは、スプライトを 2D または 3D 空間に直接描画を行うための機能です。
	通常の Sprite オブジェクトは 1 つにつき 1 つのスプライトを自動描画しますが、
	SpritePath は複数のスプライトを直接描画することができます。
	
	SpritePath は Sprite オブジェクトと比べて個々のスプライトの色調エフェクトや
	親子関係を構築することはできませんが、非常に高速な描画を行うことができます。
*/
//==============================================================================

#include <lnote.h>
#include <math.h>

// 描画するスプライト数
const int MaxBullets = 10000;

// 描画範囲
const float Left	= -32;
const float Top		= -32;
const float Right	= 672;
const float Bottom	= 512;

struct Bullet
{
	lnVector3	Pos;			// 位置
	lnVector3	Velocity;		// 速度
	float		Size;			// 大きさ
	float		Angle;			// 回転角度
	float		AngleVelocity;	// 角速度
	lnColorF	Color;			// 乗算色
	int			Pattern;		// 図形の種類
};

int main()
{
	LNConfig_SetFrameRate(30);
	LNCore_Initialize();

	lnHandle texture;
	LNTexture_CreateFromFile(&texture, "../Media/Bullets1.png");

	// SpritePath を作成する
	lnHandle spritePath;
	LNSpritePath_Create(&spritePath, MaxBullets);

	// 初期設定
	Bullet* buletts = new Bullet[MaxBullets];
	for (int i = 0; i < MaxBullets; i++)
	{
		LNRandom_GetFloatRange(NULL, 4.0, 16.0, &buletts[i].Size);
		LNRandom_GetFloatRange(NULL, Left, Right, &buletts[i].Pos.X);
		LNRandom_GetFloatRange(NULL, Top, Bottom, &buletts[i].Pos.Y);
		buletts[i].Pos.Z = buletts[i].Size;		// サイズの大きいものほど手前に表示してみる
		LNRandom_GetFloatRange(NULL, 0.1, 1.0, &buletts[i].Velocity.X);
		LNRandom_GetFloatRange(NULL, -2.5, -0.5, &buletts[i].Velocity.Y);
		buletts[i].Velocity.Z = 0;
		buletts[i].Angle = 0.0;
		LNRandom_GetFloatRange(NULL, 0, 0.25, &buletts[i].AngleVelocity);
		LNRandom_GetFloatRange(NULL, 0.0, 1.0, &buletts[i].Color.R);
		LNRandom_GetFloatRange(NULL, 0.0, 1.0, &buletts[i].Color.G);
		LNRandom_GetFloatRange(NULL, 0.0, 1.0, &buletts[i].Color.B);
		LNRandom_GetFloatRange(NULL, 0.0, 1.0, &buletts[i].Color.A);
		LNRandom_GetIntRange(NULL, 0, 4, &buletts[i].Pattern);
	}

	// メインループ
	lnBool endRequested;
	int frameCount = 0;
	do
	{
		for (int i = 0; i < MaxBullets; i++)
		{
			// 移動
			buletts[i].Pos.X += buletts[i].Velocity.X;
			buletts[i].Pos.Y += buletts[i].Velocity.Y;
			buletts[i].Angle += buletts[i].AngleVelocity;

			// (おまけアクセント。一定周期で風が吹いて、小さいものほど影響が大きい)
			buletts[i].Pos.X += ((cos(0.02 * frameCount) + 1.0) * 1.0) * (24.0 - buletts[i].Size) / 8.0;

			// 移動範囲制限
			if (buletts[i].Pos.X < Left)	buletts[i].Pos.X = Right;
			if (Right < buletts[i].Pos.X)	buletts[i].Pos.X = Left;
			if (buletts[i].Pos.Y < Top)		buletts[i].Pos.Y = Bottom;
			if (Bottom < buletts[i].Pos.Y)	buletts[i].Pos.Y = Top;

			// 座標変換行列を使い、次に描画するスプライトの回転角度を設定する
			lnMatrix rot;
			LNMatrix_Identity(&rot);
			LNMatrix_RotateZ(&rot, buletts[i].Angle);
			LNSpritePath_SetPathTransform(spritePath, &rot);

			// 描画
			lnVector3 center	= { buletts[i].Size / 2, buletts[i].Size / 2, 0 };	// スプライトの中心を座標と回転の中心にする
			lnVector2 size		= { buletts[i].Size, buletts[i].Size };
			lnRect srcRect		= { buletts[i].Pattern * 32, 0, 32, 32 };
			lnColorF colors[4]	= { buletts[i].Color, buletts[i].Color, buletts[i].Color, buletts[i].Color };
			LNSpritePath_DrawSprite(spritePath, &buletts[i].Pos, &center, &size, texture, &srcRect, colors, LN_FRONTAXIS_RZ);
		}

		frameCount++;

		LNCore_Update();
		LNCore_IsEndRequested(&endRequested);
	} while (endRequested == LN_FALSE);

	delete[] buletts;
	LNCore_Terminate();
	return 0;
}
