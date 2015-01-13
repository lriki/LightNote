//==============================================================================
// �X�v���C�g�p�X�̃T���v��
//------------------------------------------------------------------------------
/*	
	�X�v���C�g�p�X�́A�X�v���C�g�� 2D �܂��� 3D ��Ԃɒ��ڕ`����s�����߂̋@�\�ł��B
	�ʏ�� Sprite �I�u�W�F�N�g�� 1 �ɂ� 1 �̃X�v���C�g�������`�悵�܂����A
	SpritePath �͕����̃X�v���C�g�𒼐ڕ`�悷�邱�Ƃ��ł��܂��B
	
	SpritePath �� Sprite �I�u�W�F�N�g�Ɣ�ׂČX�̃X�v���C�g�̐F���G�t�F�N�g��
	�e�q�֌W���\�z���邱�Ƃ͂ł��܂��񂪁A���ɍ����ȕ`����s�����Ƃ��ł��܂��B
*/
//==============================================================================

#include <lnote.h>
#include <math.h>

// �`�悷��X�v���C�g��
const int MaxBullets = 10000;

// �`��͈�
const float Left	= -32;
const float Top		= -32;
const float Right	= 672;
const float Bottom	= 512;

struct Bullet
{
	lnVector3	Pos;			// �ʒu
	lnVector3	Velocity;		// ���x
	float		Size;			// �傫��
	float		Angle;			// ��]�p�x
	float		AngleVelocity;	// �p���x
	lnColorF	Color;			// ��Z�F
	int			Pattern;		// �}�`�̎��
};

int main()
{
	LNConfig_SetFrameRate(30);
	LNCore_Initialize();

	lnHandle texture;
	LNTexture_CreateFromFile(&texture, "../Media/Bullets1.png");

	// SpritePath ���쐬����
	lnHandle spritePath;
	LNSpritePath_Create(&spritePath, MaxBullets);

	// �����ݒ�
	Bullet* buletts = new Bullet[MaxBullets];
	for (int i = 0; i < MaxBullets; i++)
	{
		LNRandom_GetFloatRange(NULL, 4.0, 16.0, &buletts[i].Size);
		LNRandom_GetFloatRange(NULL, Left, Right, &buletts[i].Pos.X);
		LNRandom_GetFloatRange(NULL, Top, Bottom, &buletts[i].Pos.Y);
		buletts[i].Pos.Z = buletts[i].Size;		// �T�C�Y�̑傫�����̂قǎ�O�ɕ\�����Ă݂�
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

	// ���C�����[�v
	lnBool endRequested;
	int frameCount = 0;
	do
	{
		for (int i = 0; i < MaxBullets; i++)
		{
			// �ړ�
			buletts[i].Pos.X += buletts[i].Velocity.X;
			buletts[i].Pos.Y += buletts[i].Velocity.Y;
			buletts[i].Angle += buletts[i].AngleVelocity;

			// (���܂��A�N�Z���g�B�������ŕ��������āA���������̂قǉe�����傫��)
			buletts[i].Pos.X += ((cos(0.02 * frameCount) + 1.0) * 1.0) * (24.0 - buletts[i].Size) / 8.0;

			// �ړ��͈͐���
			if (buletts[i].Pos.X < Left)	buletts[i].Pos.X = Right;
			if (Right < buletts[i].Pos.X)	buletts[i].Pos.X = Left;
			if (buletts[i].Pos.Y < Top)		buletts[i].Pos.Y = Bottom;
			if (Bottom < buletts[i].Pos.Y)	buletts[i].Pos.Y = Top;

			// ���W�ϊ��s����g���A���ɕ`�悷��X�v���C�g�̉�]�p�x��ݒ肷��
			lnMatrix rot;
			LNMatrix_Identity(&rot);
			LNMatrix_RotateZ(&rot, buletts[i].Angle);
			LNSpritePath_SetPathTransform(spritePath, &rot);

			// �`��
			lnVector3 center	= { buletts[i].Size / 2, buletts[i].Size / 2, 0 };	// �X�v���C�g�̒��S�����W�Ɖ�]�̒��S�ɂ���
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
