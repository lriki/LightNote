//==============================================================================
// �X�v���C�g�̃`���[�g���A��
//------------------------------------------------------------------------------
/* 
	�X�v���C�g�͉�ʏ��2D�摜��\�����邽�߂̎d�g�݂ł��B

	�X�v���C�g��p�����摜�\���́A�u�G��`���v�Ƃ�������
	�u�G��u���v�ƍl���������C���[�W���₷���ł��B

	2D�摜��\�����邽�߂ɂ͈ȉ���2�̃I�u�W�F�N�g���K�v�ɂȂ�܂��B
	�E�e�N�X�`��	�c �摜���̂��̂�\���܂��B
	�E�X�v���C�g	�c �e�N�X�`�����ǂ̈ʒu�ɕ\�����邩���̏��������܂��B

	("�X�v���C�g"�Ƃ��������ȔɃe�N�X�`���𒣂�t����悤�ȃC���[�W��
	 �z������Ƃ킩��₷��������܂���)

	 ���X�v���C�g�̓V�[���O���t�̋@�\�̈ꕔ�ł��B
	   �����摜��\�����邾���ł���Γ��Ɉӎ�����K�v�͂���܂��񂪁A
	   �V�[���O���t�ɂ��Ă͕ʂ̃T���v�����������������B(SceneGraph)
*/
//==============================================================================

#include <lnote.h>

int main()
{
	LNCore_Initialize();

	// �摜�t�@�C������e�N�X�`�����쐬���܂��B
	// ��������ƁA��1�����̃A�h���X�Ƀe�N�X�`���n���h�����i�[����܂��B
	// �n���h���Ƃ́A�쐬���ꂽ�I�u�W�F�N�g�̎��ʔԍ��ł��B
	LNHandle texture;
	LNTexture_CreateFromFile(&texture, "../Media/20140309.png");

	// �쐬�����e�N�X�`����\�����邽�߂̃X�v���C�g���쐬���܂��B
	// ��2�����ɕ\���������e�N�X�`�����w�肵�܂��B
	// ��������ƁA��1�����̃A�h���X�ɃX�v���C�g�n���h�����i�[����܂��B
	LNHandle sprite;
	LNSprite_Create(&sprite, texture);

	// �X�v���C�g�̃T�C�Y��ݒ肵�܂��B
	// ��1�����ɂ͐ݒ肵�����X�v���C�g�̃n���h�����w�肵�A�ȍ~�͏��ɉ����A�c��(px)���w�肵�܂��B
	// �T�C�Y�̃f�t�H���g�̓e�N�X�`���̃T�C�Y�ł��B
	LNSprite_SetSize(sprite, 320, 420);

	// �X�v���C�g�̈ʒu��ݒ肵�܂��B
	// �����͏��� x, y, z ���W���w�肵�܂��B
	// 2D��Ԃɔz�u�����X�v���C�g�� Z ���W�́A���̃X�v���C�g�Ƃ̑O��֌W��\���܂��B
	// ���̂ق���]���̎p�����̐ݒ�ɂ� LNSceneNode_Set�` ���g�p���܂��B
	LNSceneNode_SetPositionXYZ(sprite, 160, 30, 0);

	// �X�v���C�g�̕s�����x��ݒ肵�܂��B
	// ���̂ق��J���[�X�P�[����F���̐ݒ�ɂ� LNVisualNode_Set�` ���g�p���܂��B
	LNVisualNode_SetOpacity(sprite, 0.75);

	// �X�v���C�g�̓V�[���m�[�h(SceneNode)�ƃr�W���A���m�[�h(VisualNode)�Ƃ����I�u�W�F�N�g���p�����Ă���A
	// �����̎��@�\��S�Ďg�p���邱�Ƃ��ł��܂��B
	// SceneNode �͈ʒu���]�p�x�A�g�嗦���̎p������̋@�\�������܂��B
	// VisualNode �͕s�����x��F�����̎��o���ʂɊ֌W����@�\�������܂��B
	
	// ���C�����[�v
	LNBool flag;
	do
	{
		LNCore_Update();	// �`��͂����Ŏ����I�ɍs���܂�
		LNCore_IsEndRequested(&flag);
	} while (flag == LN_FALSE);	

	// LNHandle �ŕ\�����e�I�u�W�F�N�g�́ALNObject_Release() �ō폜�ł��܂��B
	// LNObject_Release() �ō폜���Ȃ��Ă��ALNCore_Terminate() �ł��ׂĎ����I�ɍ폜����܂��B
	LNObject_Release(sprite);
	LNObject_Release(texture);

	LNCore_Terminate();
	return 0;
}