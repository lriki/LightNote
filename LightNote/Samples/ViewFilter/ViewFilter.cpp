//==============================================================================
// ��ʑS�̂ւ̃G�t�F�N�g�����̃T���v�� (�|�X�g�G�t�F�N�g)
//------------------------------------------------------------------------------
/*  
	LightNote �ł́u�r���[�t�B���^�v�ƌĂ�ł��܂��B
*/
//==============================================================================

#include <lnote.h>

int main()
{
	LNCore_Initialize();

	lnHandle texture;
	lnHandle sprite;
	LNTexture_CreateFromFile(&texture, "../Media/0129.png");
	LNSprite_Create(&sprite, texture);
	

	// TODO  �^�C���X�P�[��

	// ViewPane �͕`�悪�s�����ʗ̈��\���I�u�W�F�N�g�ł��B
	lnHandle viewPane;
	LNViewPane_GetDefaultViewPane(&viewPane);

	// ViewPane ���̃��C���[�̃��X�g���擾���܂��B
	lnHandle layerList;
	LNViewPane_GetLayers(viewPane, &layerList);

	// �F���𑀍삷��t�B���^�𐶐����A�o�^���܂��B
	lnHandle toneFilter;
	LNToneFilter_Create(&toneFilter);
	LNObjectList_Add(layerList, toneFilter);

	// �u���[ ��K�p����t�B���^�𐶐����A�o�^���܂��B
	lnHandle blurFilter;
	LNBlurFilter_Create(&blurFilter);
	LNObjectList_Add(layerList, blurFilter);

	// �g�����W�V������K�p����t�B���^�𐶐����A�o�^���܂��B
	lnHandle transitionFilter;
	LNTransitionFilter_Create(&transitionFilter);
	LNObjectList_Add(layerList, transitionFilter);


	// �g�����W�V�������� (���݂̉����`����Ă��Ȃ���ʂ�J�ڌ��Ƃ���)
	LNTransitionFilter_Freeze(transitionFilter);

	// �g�����W�V�������s
	lnHandle filterTexture;
	LNTexture_CreateFromFile(&filterTexture, "../Media/Transition1.png");
	LNTransitionFilter_Transition(transitionFilter, 2.0, filterTexture, 40);

	// ���C�����[�v
	lnBool endRequested;
	do
	{
		lnBool state;
		LNInput_IsTrigger(LN_BUTTON_0, &state);
		if (state)
		{
			// 1�b�����ăZ�s�A���ɕύX���܂��B
			lnTone tone = { 0.0, -0.2, -0.5, 1.0 };
			LNToneFilter_SetTone(toneFilter, &tone, 2.0);
		}
		LNInput_IsTrigger(LN_BUTTON_1, &state);
		if (state)
		{
			// ����ʂփt�F�[�h�A�E�g���܂��B
			lnTone tone = { 1.0, 1.0, 1.0, 0.0 };
			LNToneFilter_SetTone(toneFilter, &tone, 2.0);
		}
		LNInput_IsTrigger(LN_BUTTON_2, &state);
		if (state)
		{
			// �F�������ɖ߂��܂��B
			lnTone tone = { 0.0, 0.0, 0.0, 0.0 };
			LNToneFilter_SetTone(toneFilter, &tone, 2.0);
		}

		LNInput_IsTrigger(LN_BUTTON_3, &state);
		if (state)
		{
			// ��ʍ��ォ�� x:100, y:100 �𒆐S�Ƃ������˃u���[��K�p���܂��B
			lnHandle camera2D;
			LNCamera_GetDefault2DCamera(&camera2D);
			lnVector3 t = { 100, 100, 0 };
			LNBlurFilter_Blur(blurFilter, 0.5, 0.5, 1.05, &t, camera2D);
		}

		LNCore_Update();
		LNCore_IsEndRequested(&endRequested);
	} while (endRequested == LN_FALSE);

	LNCore_Terminate();
	return 0;
}