//==============================================================================
// �V�[���O���t�̃T���v��
//------------------------------------------------------------------------------
/*  
	�V�[���O���t�Ƃ́A��ʂɕ\������I�u�W�F�N�g���K�w�\���ɂ܂Ƃ߂����̂ł��B
	�ȉ��̂悤�ȗv�f�ɂ���č\������܂��B

	�E�m�[�h
		�V�[���O���t�ɔz�u����S�ẴI�u�W�F�N�g�̃x�[�X�N���X�ł��B
		�ʒu���]�p�x���̎p�����������A�����̎q�m�[�h�������Ƃ��ł��܂��B

	�EVisual �m�[�h	(base:�m�[�h)
		Sprite ���A���ۂɉ�ʂɕ\�������I�u�W�F�N�g�̊�{�N���X�ł��B
		�F���̃}�e���A�����⍇�����@���A�`��ɕK�v�Ȋ�{�I�ȏ��������܂��B
		�m�[�h�̋@�\���p�����Ă���A�m�[�h�̎����ׂĂ̋@�\���g�p���邱�Ƃ��ł��܂��B

	�ESprite �m�[�h	(base:Visual�m�[�h)
		2D�摜��\�����邽�߂̃m�[�h�ł��B
		�T���v���u02.Tutorial_Sprite�v���Ŏg�p���Ă���X�v���C�g�͂���ɂ�����܂��B
		Visual�m�[�h�̋@�\���p�����Ă���AVisual�m�[�h�ƁA����ɂ��̃x�[�X�N���X�ł���
		�m�[�h�̋@�\���g�p���邱�Ƃ��ł��܂��B

	���ۂ̍\����͈ȉ��̂悤�ɂȂ�܂��B
	
	- ���[�g�m�[�h
		- 3D ���[�g�m�[�h
			- Sprite1
			- Effect1
		- 2D ���[�g�m�[�h
			- Sprite2
				- Sprite3
			- Effect2

	�e�I�u�W�F�N�g�͐������ɁA3D �܂��� 2D ���[�g�m�[�h�̎q�m�[�h�Ƃ���
	�����I�ɃV�[���O���t�ɒǉ�����܂��B
	�Ⴆ�΁ALSprite_Create() �ō쐬���ꂽ Sprite �m�[�h�� 2D ���[�g�m�[�h�A
	LSprite_Create3D() �ō쐬���ꂽ Sprite �m�[�h�� 3D ���[�g�m�[�h�ɒǉ�����܂��B

	�q�m�[�h�͐e�m�[�h�̎p�����������p���܂��B
	���̍\����ł́A�uSprite2�v�̍��W��ݒ肷��ƁA����ɒǐ����āuSprite3�v��
	�ړ�����悤�ɂȂ�܂��B
*/
//==============================================================================

#include <lnote.h>

int main()
{
	LNCore_Initialize();

	// �G�t�F�N�g�����Ղ����邽�ߔw�i�����ɂ���
	LNColorF color1 = { 0.0, 0.0, 0.0, 1.0 };
	LNHandle viewPane;
	LNViewPane_GetDefaultViewPane(&viewPane);
	LNViewPane_SetBackgroundColor(viewPane, &color1);

	// �m�[�h�@
	LNHandle sun;
	LNEffect_Create(&sun, "../Media/Sun1.efk");
	LNEffect_Play(sun, LN_TRUE);

	// �m�[�h�@�����񂷂�m�[�h�A 
	LNHandle ice;
	LNEffect_Create(&ice, "../Media/Ice1.efk");
	LNEffect_Play(ice, LN_TRUE);
	LNSceneNode_AddChild( sun, ice );	
	LNSceneNode_SetPositionXYZ(ice, 0, 0, 10);

	// �m�[�h�A�����񂷂�m�[�h�B
	LNHandle starTex1;
	LNHandle star1;
	LNTexture_CreateFromFile(&starTex1, "../Media/Spark1.png");
	LNSprite_Create(&star1, starTex1);
	LNSprite_SetSize( star1, 2, 2 );
	LNVisualNode_SetBlendMode(star1, LN_BLEND_ADD);
	LNVisualNode_SetEnableDepthWrite(star1, LN_FALSE);
	LNSceneNode_SetPositionXYZ(star1, 3, 0, 0);
	LNSceneNode_SetPlacementType(star1, LN_PLACEMENTTYPE_BILLBOARD);
	LNSceneNode_AddChild(ice, star1);

	// �J�����̈ʒu���������ꂽ�Ƃ���ɂ���
	LNHandle camera;
	LNCamera_GetDefault3DCamera(&camera);
	LNCamera_SetPositionXYZ(camera, 25, 12, -25);

	// ���C�����[�v
	LNBool endRequested;
	float angle = 0.0;
	do
	{
		// ���������炵�āA�@�ƇA�����ꂼ���]������
		LNSceneNode_SetAngleXYZ(sun, 0, angle, 0);
		LNSceneNode_SetAngleXYZ(ice, 0, angle * 2, 0);

		angle -= 0.01;

		LNCore_Update();
		LNCore_IsEndRequested(&endRequested);
	} while (endRequested == LN_FALSE);

	LNCore_Terminate();
	return 0;
}