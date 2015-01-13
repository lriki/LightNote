//==============================================================================
// ���[�U�[���͂̃`���[�g���A��
//------------------------------------------------------------------------------
/*  
	�L�[�{�[�h�E�}�E�X�E�W���C�p�b�h����̓��͂��󂯎��܂��B

	������@
	�E�����L�[	: �O�i�E���
	�E�����L�[	: ���������E�E������
	�EZ �L�[	: �������тɉ��� ON/OFF ��؂�ւ���

	LightNote �́u���z�R���g���[���v�Ƃ����d�g�݂œ��͏����Ǘ����Ă��܂��B
	PC�ɂ͗l�X�ȓ��̓f�o�C�X��ڑ����邱�Ƃ��ł��܂����A
	���z�R���g���[���͂����̍����ӎ����邱�ƂȂ����͂��󂯎��V�X�e���ł��B

	���O�͑傰���ł��������ōs���Ă��邱�Ƃ͒P���ŁA
	�Ⴆ�΁u���{�^���v��������Ă��邩���`�F�b�N�������Ƃ��́A
	LNInput_IsPress(LN_BUTTON_LEFT) �Ƃ������悤�ɋL�q���邾����
	�ڑ�����Ă��� �L�[�{�[�h�A�Q�[���p�b�h�A�}�E�X�̑S�ẴL�[�E�{�^�����`�F�b�N����A
	�u���{�^���v�ɑ���������̂���������Ă��邩���m�F�ł��܂��B
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

	// �摜��\�����鏀��
	LNHandle texture;
	LNHandle sprite;
	LNTexture_CreateFromFile(&texture, "../Media/Arrow1.png");
	LNSprite_Create(&sprite, texture);

	// �������ł��邩
	bool isBoost = false;
	
	// ���C�����[�v
	do
	{
		float velocity = 2.0;
		if ( isBoost )
			velocity *= 2.0;

		// LNInput_IsPress ���g�p���āA����{�^����������Ă��邩���`�F�b�N���܂��B
		// ���̊֐��̓L�[�{�[�h/�}�E�X/�W���C�p�b�h���l�X�ȓ��̓f�o�C�X�̏�Ԃ���A�{�^����������Ă��邩���m�F���܂��B
		// �Ⴆ�΁A�u���{�^���v���m�F����萔 LN_BUTTON_LEFT ��n���ƁA�ȉ��̂����ꂩ�̓��͂�����ꍇ�� LN_TRUE ���o�͂��܂��B
		// �E�L�[�{�[�h�� ���L�[ ����������Ă���
		// �E�W���C�p�b�h�� POV ���������ɓ��͂���Ă���
		// �E�W���C�p�b�h�̑�1�X�e�B�b�N�����ɓ��͂���Ă���

		// �������� ('��' �L�[)
		LNInput_IsPress(LN_BUTTON_LEFT, &flag);
		if (flag == LN_TRUE)
		{
			dirAngle -= 0.1;
		}
		// �E������ ('��' �L�[)
		LNInput_IsPress(LN_BUTTON_RIGHT, &flag);
		if (flag == LN_TRUE)
		{
			dirAngle += 0.1;
		}
		// �O�i ('��' �L�[)
		LNInput_IsPress(LN_BUTTON_UP, &flag);
		if (flag == LN_TRUE)
		{
			x += cosf(dirAngle) * velocity;
			y += sinf(dirAngle) * velocity;
		}
		// ��� ('��' �L�[)
		LNInput_IsPress(LN_BUTTON_DOWN, &flag);
		if (flag == LN_TRUE)
		{
			x -= cosf(dirAngle) * velocity;
			y -= sinf(dirAngle) * velocity;
		}
		// �{�^�����������тɉ��� ON/OFF ��؂�ւ��� ('Z' �L�[)
		LNInput_IsTrigger(LN_BUTTON_0, &flag);
		if (flag == LN_TRUE)
		{
			isBoost = !isBoost;
		}

		// �{�^���萔�ƃf�o�C�X�̃{�^���́A�f�t�H���g�ł͈ȉ��̂悤�Ƀ}�b�s���O����Ă��܂��B
		// �ύX���������́uKeyConfig�v�T���v�����Q�l�ɂ��Ă��������B
		/*
		���z�{�^����	�萔��			�L�[�{�[�h	�}�E�X	�W���C�p�b�h
		0�� �{�^��		LN_BUTTON_0		'Z' �L�[	-		0 �ԃ{�^��
		1�� �{�^��		LN_BUTTON_1		'X' �L�[	-		1 �ԃ{�^��
		2�� �{�^��		LN_BUTTON_2		'C' �L�[	-		2 �ԃ{�^��
		3�� �{�^��		LN_BUTTON_3		'A' �L�[	-		3 �ԃ{�^��
		4�� �{�^��		LN_BUTTON_4		'S' �L�[	-		4 �ԃ{�^��
		5�� �{�^��		LN_BUTTON_5		'D' �L�[	-		5 �ԃ{�^��
		6�� �{�^��		LN_BUTTON_6		'Q' �L�[	-		6 �ԃ{�^��
		7�� �{�^��		LN_BUTTON_7		'W' �L�[	-		7 �ԃ{�^��
		8�� �{�^��		LN_BUTTON_8		-			-		8 �ԃ{�^��
		9�� �{�^��		LN_BUTTON_9		-			-		9 �ԃ{�^��
		10�� �{�^��		LN_BUTTON_10	-			-		10 �ԃ{�^��
		11�� �{�^��		LN_BUTTON_11	-			-		11 �ԃ{�^��
		12�� �{�^��		LN_BUTTON_12	-			-		12 �ԃ{�^��
		13�� �{�^��		LN_BUTTON_13	-			-		13 �ԃ{�^��
		14�� �{�^��		LN_BUTTON_14	-			-		14 �ԃ{�^��
		15�� �{�^��		LN_BUTTON_15	-			-		15 �ԃ{�^��
		�� �{�^��		LN_BUTTON_LEFT	'��' �L�[	-		POV�� / ��1��-
		�E �{�^��		LN_BUTTON_RIGHT	'��' �L�[	-		POV�E / ��1��+
		�� �{�^��		LN_BUTTON_UP	'��' �L�[	-		POV�� / ��2��-
		�� �{�^��		LN_BUTTON_DOWN	'��' �L�[	-		POV�� / ��2��+
		*/

		// �������͐Ԃ����摜�̕�����]������
		if ( isBoost )
			LNSprite_SetSrcRectXYWH( sprite, 64, 0, 64, 64 );
		else
			LNSprite_SetSrcRectXYWH(sprite, 0, 0, 64, 64);

		// �X�v���C�g�̎p����ݒ�
		LNSceneNode_SetCenterXYZ( sprite, 32, 32, 0 );	// 64x64px �̒��S���ړ��Ɖ�]�̒��S�ɂ���
		LNSceneNode_SetPositionXYZ(sprite, x, y, 0);
		LNSceneNode_SetAngleXYZ(sprite, 0, 0, dirAngle);

		LNCore_Update();	// ���͏��̍X�V�͂����Ŏ����I�ɍs���܂�
		LNCore_IsEndRequested(&flag);
	} while (flag == LN_FALSE);

	LNCore_Terminate();
	return 0;
}