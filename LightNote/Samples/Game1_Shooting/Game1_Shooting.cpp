//==============================================================================
// �T���v���Q�[�� - �V���[�e�B���O
//------------------------------------------------------------------------------
/*
	���[��
		���@�𑀍삵�Ēe�������A��葽���G�𗎂Ƃ������ڕW�ł��B
		�G�̒e�ɐڐG����Ǝ��@��HP���������A0 �ɂȂ�ƃQ�[���I�[�o�[�ƂȂ�A
		���̎��_�̃X�R�A���L�^����܂��B

	������@
		�L�[�{�[�h
			�����L�[          �c ���@�̈ړ�
			Z �L�[            �c �e�̔���
		�W���C�p�b�h
			��1�X�e�B�b�N/POV �c ���@�̈ړ�
			0�ԃ{�^��         �c �e�̔���
*/
//==============================================================================

#include <math.h>
#include <lnote.h>

#define DATA_PATH(name) "../Media/Game1_Shooting/"name
#define PI 3.14159265359

// ���@�Ɋւ�������܂Ƃ߂��\����
struct Player
{
	int			X;				// X���W
	int			Y;				// Y���W
	int			HP;				// �q�b�g�|�C���g
	int			ShotInterval;	// �A�ˊ��o (���̔��˂܂ł̑҂��t���[����)
	LNHandle	Sprite;			// �X�v���C�g
};

// �G�Ɋւ�������܂Ƃ߂��\����
struct Enemy
{
	float		X;				// X���W
	float		Y;				// Y���W
	float		Velocity;		// ���x
	float		HP;				// �q�b�g�|�C���g
	bool		DamageEffect;	// �_���[�W�G�t�F�N�g�Ƃ��āA��u�t���b�V��������
	bool		Deleted;		// ���ł��Ă��邩
	int			ShotInterval;	// �A�ˊ��o (���̔��˂܂ł̑҂��t���[����)
	LNHandle	Sprite;			// �X�v���C�g
};

// �e�𔭎˂����l
enum BulletOwner
{
	BulletOwner_Player = 0,
	BulletOwner_Enemy,
};

// �e�Ɋւ�������܂Ƃ߂��\����
struct Bullet
{
	float			X;			// X���W
	float			Y;			// Y���W
	float			Angle;		// �i�s����
	float			Velocity;	// ���x
	BulletOwner		Owner;		// ���@�����˂����e�Ȃ̂��A�G�����˂����e�Ȃ̂�
	bool			Deleted;	// ���ł��Ă��邩
	LNHandle		Sprite;		// �X�v���C�g
};

const int MAX_ENEMIES = 100;	// �G�̍ő吔
const int MAX_BULLETS = 1000;	// �e�̍ő吔

Player		g_Player;						// �v���C���[
Enemy		g_Enemies[MAX_ENEMIES] = { 0 };	// �S�Ă̓G�̊Ǘ��z��
Bullet		g_Bullets[MAX_BULLETS] = { 0 };	// �S�Ă̒e�̊Ǘ��z��
int			g_GameFrameCount = 0;			// �Q�[���J�n����̌o�߃t���[����
int			g_Score = 0;					// �X�R�A (���Đ�)
int			g_HighScore = 0;				// �n�C�X�R�A
LNHandle	g_UITexture = NULL;				// UI �\���p�e�N�X�`��
LNHandle	g_UISprite = NULL;				// UI �\���p�X�v���C�g

// �֐��v���g�^�C�v
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
// ���@�̏�����
//---------------------------------------------------------------------
void Player_Init()
{
	// �v���C���[�̏����ʒu
	g_Player.X = 180;
	g_Player.Y = 400;

	g_Player.HP = 10;
	g_Player.ShotInterval = 0;

	// �X�v���C�g�̍쐬
	LNHandle tex;
	LNTexture_CreateFromFile(&tex, DATA_PATH("Images1.png"));
	LNSprite_Create(&g_Player.Sprite, tex);
	LNSprite_SetSrcRectXYWH(g_Player.Sprite, 0, 0, 32, 32);
	LNSceneNode_SetCenterXYZ(g_Player.Sprite, 16, 16, 0);	// 32x32 �摜�̒��S���X�v���C�g�̒��S�ɂ���
}

//---------------------------------------------------------------------
// ���@�̃t���[���X�V
//---------------------------------------------------------------------
void Player_Update()
{
	// �����L�[�̓��͂��󂯎��
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

	// ��ʂ���o�Ȃ��悤�Ɉړ��͈͂𐧌�
	if (g_Player.X < 0) g_Player.X = 0;
	if (g_Player.X > 328) g_Player.X = 328;
	if (g_Player.Y < 0) g_Player.Y = 0;
	if (g_Player.Y > 448) g_Player.Y = 448;

	// �X�v���C�g�̈ʒu��ݒ�
	LNSceneNode_SetPositionXYZ(g_Player.Sprite, g_Player.X, g_Player.Y, 0);

	// �e�̔���
	LNInput_IsPress(LN_BUTTON_0, &state);
	if (state && g_Player.ShotInterval == 0)
	{
		Bullet_Create(g_Player.X, g_Player.Y, -PI * 0.6, 10, BulletOwner_Player);
		Bullet_Create(g_Player.X, g_Player.Y, -PI * 0.4, 10, BulletOwner_Player);
		Bullet_Create(g_Player.X, g_Player.Y, -PI * 0.5, 10, BulletOwner_Player);
		
		LNAudio_PlaySE(DATA_PATH("shot.wav"), 40, 150);

		g_Player.ShotInterval = 5;	// ����5�t���[����ɔ��˂ł���
	}
	if (g_Player.ShotInterval > 0)
		g_Player.ShotInterval--;
}

//---------------------------------------------------------------------
// �S�Ă̓G�̏�����
//---------------------------------------------------------------------
void Enemies_Init()
{
	// �ŏ��͂��ׂď��ł��Ă��邱�Ƃɂ���
	for (int i = 0; i < MAX_ENEMIES; i++)
		g_Enemies[i].Deleted = true;
}

//---------------------------------------------------------------------
// �S�Ă̓G�̍X�V
//---------------------------------------------------------------------
void Enemies_Update()
{
	// 30 �t���[����1�x�A�V�����G�����
	if (g_GameFrameCount % 30 == 0) {
		float x;
		LNRandom_GetFloatRange(NULL, 32, 328, &x);
		Enemy_Create(x, 0);
	}

	// �S�Ă̓G�̍X�V
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		// �����Ă���G�Ȃ��
		if (g_Enemies[i].Deleted == false)
		{
			Enemy* enemy = &g_Enemies[i];

			// ���ֈړ�
			enemy->Y += enemy->Velocity;

			// �X�v���C�g�̈ʒu�E�p�x��ݒ�
			LNSceneNode_SetPositionXYZ(enemy->Sprite, enemy->X, enemy->Y, 0);

			// �_���[�W�G�t�F�N�g�̏���
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

			// ��ʂ���o���ꍇ�͏���
			if (enemy->X < -16 || enemy->X > 376 ||
				enemy->Y < -16 || enemy->Y > 496)
			{
				Enemy_Delete(enemy);
			}

			// �e������
			if (enemy->ShotInterval == 0)
			{
				// ���@�_��
				float angle = atan2(g_Player.Y - enemy->Y, g_Player.X - enemy->X);
				Bullet_Create(enemy->X, enemy->Y, angle, 3, BulletOwner_Enemy);

				enemy->ShotInterval = 120;	// ����2�b���Ƃɔ��˂���
			}
			if (enemy->ShotInterval > 0)
				enemy->ShotInterval--;
		}
	}
}

//---------------------------------------------------------------------
// �V�����G�̍쐬
//---------------------------------------------------------------------
void Enemy_Create(int x, int y)
{
	// g_Bullets ���̋󂢂Ă���e��T��
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
		LNSceneNode_SetCenterXYZ(enemy->Sprite, 16, 16, 0);	// 32x32 �摜�̒��S���X�v���C�g�̒��S�ɂ���
	}
}

//---------------------------------------------------------------------
// �G���폜����
//---------------------------------------------------------------------
void Enemy_Delete(Enemy* enemy)
{
	enemy->Deleted = true;
	LNObject_Release(enemy->Sprite);
}

//---------------------------------------------------------------------
// �S�Ă̒e�̏�����
//---------------------------------------------------------------------
void Bullets_Init()
{
	// �ŏ��͂��ׂď��ł��Ă��邱�Ƃɂ���
	for (int i = 0; i < MAX_BULLETS; i++)
		g_Bullets[i].Deleted = true;
}

//---------------------------------------------------------------------
// �S�Ă̒e�̍X�V
//---------------------------------------------------------------------
void Bullets_Update()
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		Bullet* bullet = &g_Bullets[i];
		if (!bullet->Deleted)
		{
			// �i�s�����ֈړ�
			float vx = bullet->Velocity * cos(bullet->Angle);
			float vy = bullet->Velocity * sin(bullet->Angle);
			bullet->X += vx;
			bullet->Y += vy;

			// �X�v���C�g�̈ʒu�E�p�x��ݒ�
			LNSceneNode_SetPositionXYZ(bullet->Sprite, bullet->X, bullet->Y, 0);
			LNSceneNode_SetAngleXYZ(bullet->Sprite, 0, 0, bullet->Angle + PI * 0.5);

			// ��ʂ���o���ꍇ�͏���
			if (bullet->X < -16 || bullet->X > 376 ||
				bullet->Y < -16 || bullet->Y > 496)
			{
				Bullet_Delete(bullet);
			}

			// ���@���������e�͓G�ƏՓ˔��肷��
			if (bullet->Owner == BulletOwner_Player)
			{
				for (int i = 0; i < MAX_ENEMIES; i++)
				{
					Enemy* enemy = &g_Enemies[i];
					if (!enemy->Deleted)
					{
						// �e�̔��a��3px�A�G�̔��a��12px�Ƃ���
						if (Collision_DetectCircles(bullet->X, bullet->Y, 3, enemy->X, enemy->Y, 12)) 
						{
							// �e���폜
							Bullet_Delete(bullet);

							// �G�Ƀ_���[�W��^���A�|������X�R�A�𑝂₷
							enemy->HP--;
							enemy->DamageEffect = true;		// �q�b�g�G�t�F�N�g��\������
							if (enemy->HP == 0) {
								Enemy_Delete(enemy);
								Score_Add();
							}
							break;
						}
					}
				}
			}
			// �G���������e�͎��@�ƏՓ˔��肷��
			else if (bullet->Owner == BulletOwner_Enemy)
			{
				// �e�̔��a��3px�A���@�̔��a��5px�Ƃ���
				if (Collision_DetectCircles(bullet->X, bullet->Y, 3, g_Player.X, g_Player.Y, 5))
				{
					// �e���폜
					Bullet_Delete(bullet);

					// ���@�Ƀ_���[�W��^����
					g_Player.HP--;
				}
			}
		}
	}
}

//---------------------------------------------------------------------
// �V�����e�̍쐬
//---------------------------------------------------------------------
void Bullet_Create(int x, int y, float angle, float velocity, BulletOwner owner)
{
	// g_Bullets ���̋󂢂Ă���e��T��
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
		LNSceneNode_SetCenterXYZ(bullet->Sprite, 16, 16, 0);	// 32x32 �摜�̒��S���X�v���C�g�̒��S�ɂ���

		// ���@�E�G�̂ǂ��炪���˂������ɂ���ĕ\���摜��ς���
		if (owner == BulletOwner_Player)
			LNSprite_SetSrcRectXYWH(bullet->Sprite, 64, 0, 32, 32);
		else
			LNSprite_SetSrcRectXYWH(bullet->Sprite, 96, 0, 32, 32);
	}
	else {
		// ������Ȃ������B�e�͔��˂ł��Ȃ�
	}
}

//---------------------------------------------------------------------
// �e���폜����
//---------------------------------------------------------------------
void Bullet_Delete(Bullet* bullet)
{
	bullet->Deleted = true;
	LNObject_Release(bullet->Sprite);
}

//---------------------------------------------------------------------
// 2�̉~�̏Փ˔�����s��
//---------------------------------------------------------------------
bool Collision_DetectCircles(float x1, float y1, float r1, float x2, float y2, float r2)
{
	LNVector2 d = { x1 - x2, y1 - y2 };
	float length;
	LNVector2_GetLength(&d, &length);
	return (length <= (r1 + r2));
}

//---------------------------------------------------------------------
// UI�p�[�c (�X�R�A/HP�\��) �̏�����
//---------------------------------------------------------------------
void UI_Init()
{
	// ��ʑS�̂ɕ������Ԃ���悤�ɃX�v���C�g���쐬����B���̃e�N�X�`���ɕ�������������ł���
	LNTexture_Create(&g_UITexture, 360, 480);
	LNSprite_Create(&g_UISprite, g_UITexture);

	// Z �l���w�肷�邱�ƂŁA���̃X�v���C�g������O�ɕ\������
	LNSceneNode_SetPositionXYZ(g_UISprite, 0, 0, 100);
}

//---------------------------------------------------------------------
// UI�p�[�c (�X�R�A/HP�\��) �̍X�V
//---------------------------------------------------------------------
void UI_Update()
{
	LNTexture_ClearRGBA(g_UITexture, 0, 0, 0, 0);

	// ���@��HP��`��
	char text[256];
	sprintf(text, "HP : %d", g_Player.HP);
	LNTexture_DrawTextXY(g_UITexture, text, 8, 8);

	// �X�R�A�E�n�C�X�R�A��`�� (�E����)
	sprintf(text, "SCORE : %4d / %4d", g_Score, g_HighScore);
	LNRect drawArea = { 0, 8, 352, 100 };
	LNTexture_DrawText(g_UITexture, text, &drawArea, NULL, -1, LN_TEXTALIGN_RIGHT);
}

//---------------------------------------------------------------------
// �X�R�A�̏�����
//---------------------------------------------------------------------
void Score_Init()
{
	g_Score = 0;
	g_HighScore = 10;

	// score �t�@�C�������݂���΃n�C�X�R�A��ǂݎ��
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
// �X�R�A�𑝂₷
//---------------------------------------------------------------------
void Score_Add()
{
	g_Score++;

	// �n�C�X�R�A�X�V
	if (g_Score > g_HighScore)
		g_HighScore = g_Score;
}

//---------------------------------------------------------------------
// �n�C�X�R�A�̕ۑ�
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
	LNConfig_SetWindowTitle("�T���v���Q�[��");
	LNConfig_SetWindowSize(360, 480);
	LNConfig_SetDirectMusicInitMode(LN_DMINITMODE_NORMAL);
	LNConfig_SetAcceleratorKey(LN_ACCELERATORKEY_SHOW_DEBUG_STATISTICS, LN_KEY_F3, LN_FALSE, LN_FALSE, LN_FALSE);

	if (LNCore_Initialize() != LN_OK)
		return -1;

	Player_Init();				// ���@�̏�����
	Enemies_Init();				// �S�Ă̓G�̏�����
	Bullets_Init();				// �S�Ă̒e�̏�����
	UI_Init();					// UI �̏�����
	Score_Init();				// �X�R�A�̏�����

	// BGM �Đ�
	LNAudio_PlayBGM(DATA_PATH("ln29_SampleBGM.mid"), 100, 100);

	// ���C�����[�v
	LNBool endRequested;
	do
	{
		Player_Update();		// ���@�̍X�V
		Enemies_Update();		// �S�Ă̓G�̍X�V
		Bullets_Update();		// �S�Ă̒e�̍X�V
		UI_Update();			// UI �̍X�V

		g_GameFrameCount++;

		LNCore_Update();
		LNCore_IsEndRequested(&endRequested);
	} while (!endRequested && g_Player.HP > 0);	// ���@�����Ƃ��ꂽ��I��

	Score_Save();				// �n�C�X�R�A�̕ۑ�

	LNCore_Terminate();
	return 0;
}
