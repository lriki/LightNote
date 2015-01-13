//==============================================================================
// �L�[�R���t�B�O�̃T���v��
//------------------------------------------------------------------------------
/*  
	�A�N�V�����Q�[����z�肵���A�U���{�^���ƃW�����v�{�^���̃R���t�B�O���s���T���v���ł��B
	��{�I�ȓ��͂ɂ��� 03.Tutorial_Input.cpp ���������������B

	�ELNInput �� LNVirtualPad �ɂ���

		LNVirtualPad ���W���[���̓n���h��1�ŉ��z�R���g���[��1��\���܂��B
		�Ⴆ�Ήƒ�p�Q�[���@�ł̓R���g���[���� 1P�`4P �ڑ����邱�Ƃ��ł��܂����A
		LightNote �ł͂�����C���[�W���āA���������ɍő�4�̉��z�R���g���[��������܂��B

		�f�o�C�X�͈ȉ��̂悤�Ɋ��蓖�Ă��܂��B
		�E1P	�L�[�{�[�h, �}�E�X, 0�ԃW���C�p�b�h
		�E2P	1�ԃW���C�p�b�h
		�E3P	2�ԃW���C�p�b�h
		�E4P	3�ԃW���C�p�b�h

		LNInput �́A���̂��� 1P �p�̉��z�R���g���[������̓��͂��󂯎�郆�[�e�B���e�B�ł��B

	�E�L�[�R���t�B�O�̗���
	
		�@
*/
//==============================================================================

#include <lnote.h>

#define PI 3.1415f

void MakeButtonName( char* outName, int deviceButton );

// LN_BUTTON_0 ���U���{�^���ALN_BUTTON_1 ���W�����v�{�^���Ƃ��Ă��܂��B
#define ATTACK_BUTTON	LN_BUTTON_0
#define JUMP_BUTTON		LN_BUTTON_1

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------
int main()
{
	LNCore_Initialize();

	// �G�t�F�N�g�����Ղ����邽�ߔw�i�����ɂ���
	LNColorF color1 = { 0.0, 0.0, 0.0, 1.0 };
	LNHandle viewPane;
	LNViewPane_GetDefaultViewPane(&viewPane);
	LNViewPane_SetBackgroundColor(viewPane, &color1);

	// �L�����N�^�[�摜��p��
	LNHandle texture1;
	LNHandle sprite1;
	LNTexture_CreateFromFile(&texture1, "../Media/Circle1.png");
	LNSprite_Create(&sprite1, texture1);
	LNSprite_SetSize(sprite1, 1, 1);
	// �L�����N�^�[�p�X�v���C�g�� 3D ��Ԃɔz�u����
	LNHandle rootNode3D;
	LNSceneNode_GetDefault3DRootNode(&rootNode3D);
	LNSceneNode_AddChild(rootNode3D, sprite1);

	// �U���p�Ɏg�p����G�t�F�N�g��p�� (�G�t�F�N�g�� 3D ��Ԃɔz�u�����)
	LNHandle effect1;
	LNEffect_Create(&effect1, "../Media/Laser01.efk");
	LNSceneNode_SetAngleXYZ(effect1, 0, PI / 2, 0);
	LNVisualNode_SetEnableDepthWrite(effect1, LN_FALSE);
	LNVisualNode_SetEnableDepthTest(effect1, LN_FALSE);

	// �J������S�̂����₷���ʒu�Ɉړ�
	LNHandle camera;
	LNCamera_GetDefault3DCamera(&camera);
	LNCamera_SetCameraType(camera, LN_CAMERATYPE_POSITION_AND_ANGLE);
	LNCamera_SetPositionXYZ(camera, 5, 0, -30);

	// �������`�悷��e�N�X�`����p��
	LNHandle infoTexture;
	LNHandle infoSprite;
	LNTexture_Create(&infoTexture, 640, 480);
	LNSprite_Create(&infoSprite, infoTexture);

	LNRect rect = { 0, 0, 640, 480 };
	LNTexture_DrawText( infoTexture, "�e�s���Ɋ��蓖�Ă�{�^���������Ă��������B", &rect );
	rect.Y += 24;
	LNTexture_DrawText( infoTexture, "�U���{�^��     > ", &rect );

	// 1P ���������z�R���g���[���̎擾
	LNHandle pad;
	LNVirtualPad_GetVirtualPad(0, &pad);

	int step = 0;
	float y = 0.0;
	float velocity = 0.0;

	// ���C�����[�v
	LNBool endRequested;
	do
	{
		switch ( step )
		{
			// �L�[�R���t�B�O�� (�U���{�^��)
			case 0:
			{
				// ���炩�̃{�^�����V���������ꂽ�u�Ԃ����o
				int deviceButton;
				LNVirtualPad_GetPressedDeviceButton(pad, &deviceButton);
				if ( deviceButton != 0 )
				{
					// ���� ATTACK_BUTTON �Ɋ��蓖�Ă��Ă���f�o�C�X�{�^����S�ĉ������A
					// �V���������ꂽ�f�o�C�X�{�^����o�^����
					LNVirtualPad_DetachVirtualButton(pad, ATTACK_BUTTON, 0);
					LNVirtualPad_AttachVirtualButton(pad, ATTACK_BUTTON, deviceButton);

					// �{�^������`�悷��
					char tmp[1000];
					MakeButtonName( tmp, deviceButton );
					rect.X = 160;
					rect.Y = 24;
					LNTexture_DrawText(infoTexture, tmp, &rect);
					
					// ���ɓ��͂���s������`��
					rect.X = 0;
					rect.Y = 48;
					LNTexture_DrawText( infoTexture, "�W�����v�{�^�� > ", &rect );

					// ���̃X�e�b�v��
					step = 1;
				}

				break;
			}
			// �L�[�R���t�B�O�� (�W�����v�{�^��)
			case 1:
			{
				// ���炩�̃{�^�����V���������ꂽ�u�Ԃ����o
				int deviceButton;
				LNVirtualPad_GetPressedDeviceButton(pad, &deviceButton);
				if (deviceButton != 0)
				{
					// ���� JUMP_BUTTON �Ɋ��蓖�Ă��Ă���f�o�C�X�{�^����S�ĉ������A
					// �V���������ꂽ�f�o�C�X�{�^����o�^����
					LNVirtualPad_DetachVirtualButton(pad, JUMP_BUTTON, 0);
					LNVirtualPad_AttachVirtualButton(pad, JUMP_BUTTON, deviceButton);

					// �{�^������`�悷��
					char tmp[1000];
					MakeButtonName( tmp, deviceButton );
					rect.X = 160;
					rect.Y = 48;
					LNTexture_DrawText(infoTexture, tmp, &rect);

					// ���̃X�e�b�v��
					step = 2;
				}
				break;
			}
			// ���s��
			case 2:
			{
				LNBool flag;
				// �U���{�^���������ꂽ
				LNVirtualPad_IsTrigger(pad, ATTACK_BUTTON, &flag);
				if (flag)
				{
					LNEffect_Play( effect1, LN_TRUE );
				}
				// �W�����v�{�^���������ꂽ
				LNVirtualPad_IsTrigger(pad, JUMP_BUTTON, &flag);
				if (flag)
				{
					velocity = 0.3f;
				}
				break;
			}
		}

		// ���x�E�ʒu�v�Z
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
// �f�o�C�X�{�^���ԍ������ނƔԍ��̕�������쐬����
//----------------------------------------------------------------------
void MakeButtonName( char* outName, int deviceButton )
{
	LNDeviceButtonType type;
	LNVirtualPad_GetDeviceButtonType(deviceButton, &type);
	switch ( type )
	{
		// �L�[�{�[�h
		case LN_DEVICEBUTTONTYPE_KEYBOARD:		
			sprintf( outName, "�L�[�{�[�h %d", deviceButton );	// (deviceButton �� lnKeyCode �ł���)
			break;
		// �}�E�X�{�^��
		case LN_DEVICEBUTTONTYPE_MOUSE:			
			sprintf( outName, "�}�E�X�{�^�� %d", deviceButton - LN_DEVICEBUTTON_MOUSE_1 );
			break;
		// �W���C�p�b�h�̃{�^��
		case LN_DEVICEBUTTONTYPE_JOYPAD_BUTTON:	
			sprintf( outName, "�W���C�p�b�h�̃{�^�� %d", deviceButton - LN_DEVICEBUTTON_JOYPAD_BUTTON_1 );
			break;
		// �W���C�p�b�h��POV
		case LN_DEVICEBUTTONTYPE_JOYPAD_POV:	
			sprintf( outName, "�W���C�p�b�h��POV %d", deviceButton - LN_DEVICEBUTTON_JOYPAD_POV_UP );
			break;
		// �W���C�p�b�h�̎�
		case LN_DEVICEBUTTONTYPE_JOYPAD_AXIS:	
			sprintf( outName, "�W���C�p�b�h�̎� %d", deviceButton - LN_DEVICEBUTTON_JOYPAD_AXIS_1_MINUIS );
			break;
		// �s���ȃ{�^��
		default:
			sprintf( outName, "�s���ȃ{�^�� %d", deviceButton );
			break;
	}
}
