//==============================================================================
// �X�v���C�g�̓�����ʃT���v��
//------------------------------------------------------------------------------
/*
	�X�v���C�g�ɂ͈ȉ��̗l�X�ȓ�����ʂ�ݒ肷�邱�Ƃ��ł��܂��B
	�E�J���[�X�P�[�� (RGBA�e�F�̋���)
	�E�u�����h�J���[ (�h��Ԃ�)
	�E�F��
	�E�������@
*/
//==============================================================================

#include <lnote.h>

int main()
{
	LNCore_Initialize();

	// ������ʂ̌��ʂ����Ղ��悤�ɔw�i��p�ӂ���
	lnHandle bgTex;
	lnHandle bgSprite;
	LNTexture_CreateFromFile(&bgTex, "../Media/DeskPad1.png");
	LNSprite_Create(&bgSprite, bgTex);

	// �I���W�i���摜

	int y = 60;

	//------------------------------------------------------------
	//  ������ʂ�K�p����O�̃I���W�i��
	lnHandle tex1;
	lnHandle sprite1_1;
	LNTexture_CreateFromFile(&tex1, "../Media/Crystal2.png");
	LNSprite_Create(&sprite1_1, tex1);
	LNSceneNode_SetPositionXYZ(sprite1_1, 0, y, 0);

	y += 80;

	//------------------------------------------------------------
	// �J���[�X�P�[��
	//		�J���[�X�P�[���͕`�掞�� RGBA �e�F�����ɏ�Z�����l�ŁA
	//		�F�̋��������߂܂��B

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
	// �u�����h�J���[
	//		�u�����h�J���[�͕`�掞�ɃC���[�W��h��Ԃ��F�ł��B
	//		�u�����h�̊����ɂ̓A���t�@�l���g�p����܂��B

	// ��50%
	lnColorF color3_1 = { 1.0, 1.0, 1.0, 0.5 };
	lnHandle sprite3_1;
	LNSprite_Create(&sprite3_1, tex1);
	LNSceneNode_SetPositionXYZ(sprite3_1, 0, y, 0);
	LNVisualNode_SetBlendColor(sprite3_1, 0, &color3_1);

	// ��75%
	lnColorF color3_2 = { 1.0, 1.0, 1.0, 0.75 };
	lnHandle sprite3_2;
	LNSprite_Create(&sprite3_2, tex1);
	LNSceneNode_SetPositionXYZ(sprite3_2, 50, y, 0);
	LNVisualNode_SetBlendColor(sprite3_2, 0, &color3_2);

	// ��100%
	lnColorF color3_3 = { 1.0, 1.0, 1.0, 1.0 };
	lnHandle sprite3_3;
	LNSprite_Create(&sprite3_3, tex1);
	LNSceneNode_SetPositionXYZ(sprite3_3, 100, y, 0);
	LNVisualNode_SetBlendColor(sprite3_3, 0, &color3_3);

	// ��50%
	lnColorF color3_4 = { 1.0, 0.0, 0.0, 0.5 };
	lnHandle sprite3_4;
	LNSprite_Create(&sprite3_4, tex1);
	LNSceneNode_SetPositionXYZ(sprite3_4, 150, y, 0);
	LNVisualNode_SetBlendColor(sprite3_4, 0, &color3_4);

	// ��75%
	lnHandle sprite3_5;
	lnColorF color3_5 = { 0.0, 1.0, 0.0, 0.75 };
	LNSprite_Create(&sprite3_5, tex1);
	LNSceneNode_SetPositionXYZ(sprite3_5, 200, y, 0);
	LNVisualNode_SetBlendColor(sprite3_5, 0, &color3_5);

	// ��100%
	lnColorF color3_6 = { 0.0, 0.0, 1.0, 1.0 };
	lnHandle sprite3_6;
	LNSprite_Create(&sprite3_6, tex1);
	LNSceneNode_SetPositionXYZ(sprite3_6, 250, y, 0);
	LNVisualNode_SetBlendColor(sprite3_6, 0, &color3_6);

	y += 80;

	//------------------------------------------------------------
	// �F��
	//		�F����RGB�e�F�𒲐�����l�ł��B(�s�����x�ɂ͉e�����܂���)
	//		-1.0�`1.0�͈̔͂Ŏw�肵�܂��B

	// ��
	lnTone tone4_1 = { 1.0, 0.0, 0.0, 0.0 };
	lnHandle sprite4_1;
	LNSprite_Create(&sprite4_1, tex1);
	LNSceneNode_SetPositionXYZ(sprite4_1, 0, y, 0);
	LNVisualNode_SetTone(sprite4_1, 0, &tone4_1);

	// ��
	lnTone tone4_2 = { 0.0, 1.0, 0.0, 0.0 };
	lnHandle sprite4_2;
	LNSprite_Create(&sprite4_2, tex1);
	LNSceneNode_SetPositionXYZ(sprite4_2, 50, y, 0);
	LNVisualNode_SetTone(sprite4_2, 0, &tone4_2);

	// ��
	lnTone tone4_3 = { 0.0, 0.0, 1.0, 0.0 };
	lnHandle sprite4_3;
	LNSprite_Create(&sprite4_3, tex1);
	LNSceneNode_SetPositionXYZ(sprite4_3, 100, y, 0);
	LNVisualNode_SetTone(sprite4_3, 0, &tone4_3);

	// ���m�N��
	lnTone tone4_4 = { 0.0, 0.0, 0.0, 1.0 };
	lnHandle sprite4_4;
	LNSprite_Create(&sprite4_4, tex1);
	LNSceneNode_SetPositionXYZ(sprite4_4, 150, y, 0);
	LNVisualNode_SetTone(sprite4_4, 0, &tone4_4);

	// �Z�s�A��
	lnTone tone4_5 = { 0.0, -0.5, -0.9, 1.0 };
	lnHandle sprite4_5;
	LNSprite_Create(&sprite4_5, tex1);
	LNSceneNode_SetPositionXYZ(sprite4_5, 200, y, 0);
	LNVisualNode_SetTone(sprite4_5, 0, &tone4_5);

	y += 80;

	//------------------------------------------------------------
	// �������@
	//		�`���C���[�W�Ƃ̍������@�ł��B

	// ���Z
	lnHandle sprite5_1;
	LNSprite_Create(&sprite5_1, tex1);
	LNSceneNode_SetPositionXYZ(sprite5_1, 0, y, 0);
	LNVisualNode_SetBlendMode(sprite5_1, LN_BLEND_ADD);

	// ���Z
	lnHandle sprite5_2;
	LNSprite_Create(&sprite5_2, tex1);
	LNSceneNode_SetPositionXYZ(sprite5_2, 50, y, 0);
	LNVisualNode_SetBlendMode(sprite5_2, LN_BLEND_SUB);

	// ��Z
	lnHandle sprite5_3;
	LNSprite_Create(&sprite5_3, tex1);
	LNSceneNode_SetPositionXYZ(sprite5_3, 100, y, 0);
	LNVisualNode_SetBlendMode(sprite5_3, LN_BLEND_MUL);

	// ���o�[�X
	lnHandle sprite5_4;
	LNSprite_Create(&sprite5_4, tex1);
	LNSceneNode_SetPositionXYZ(sprite5_4, 150, y, 0);
	LNVisualNode_SetBlendMode(sprite5_4, LN_BLEND_REVERSE);


	//------------------------------------------------------------
	// �ȉ��A������
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
	LNTexture_DrawText(textTexture, "�I���W�i���̃C���[�W", &rect, font1);
	rect.Y += 80;
	LNTexture_DrawText(textTexture, "�J���[�X�P�[�� (RGBA �e�F�����̋���)", &rect, font1);
	rect.Y += 80;
	LNTexture_DrawText(textTexture, "�u�����h�J���[ (�h��Ԃ��F�Ƃ��̋���)", &rect, font1);
	rect.Y += 80;
	LNTexture_DrawText(textTexture, "�F��", &rect, font1);
	rect.Y += 80;
	LNTexture_DrawText(textTexture, "�������@", &rect, font1);

	// ���C�����[�v
	lnBool endRequested;
	{
		LNCore_Update();
		LNCore_IsEndRequested(&endRequested);
	} while (endRequested == LN_FALSE);

	LNCore_Terminate();
	return 0;
}