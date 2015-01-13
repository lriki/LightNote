//==============================================================================
// �����̍Đ��`���[���A��
//------------------------------------------------------------------------------
/*  
	�����̍Đ��� LNSound �܂��� LNAudio ���W���[���ōs���܂��B

	�ELNSound
		�����f�[�^���T�E���h�n���h����ʂ��đ��삵�܂��B
		�n���h���̐����E�j�����̊Ǘ��������ōs���K�v������܂����A
		�ЂƂ̉����ɑ΂��Ĉꎞ��~��Đ��ʒu�̎擾���A�ׂ������삪�\�ł��B

	�ELNAudio
		�Q�[�������̃��[�e�B���e�B�ł��B
		BGM�ASE ���̗p�r�ɍ��킹�āA�����t�@�C�����ȒP�ɍĐ����邱�Ƃ��ł��܂��B
		LNSound �Ƃ͈قȂ�A�T�E���h�n���h�������킸�A�t�@�C�����𒼐ڎw�肵�čĐ����܂��B
		(�C���^�[�t�F�C�X�� RGSS(RPG�c�N�[��)�� Audio ���W���[�����C���[�W���Ă��܂�)

	�Ή��t�@�C���t�H�[�}�b�g�� WAVE(.wav)�AMP3(.mp3)�AOgg Vorbis(.ogg)�AMIDI(.mid) �ł��B
	���̂��� Ogg Vorbis �� MIDI �͈ȉ��̃��[���Ń��[�v�ʒu��F�����܂��B
	�EOgg Vorbis
		�R�����g�Ƃ��� LOOPSTART�ALOOPLENGTH �Ƃ����l�����ߍ��܂�Ă���ƁA
		���̐��l�ɑΉ�����T���v���ʒu�����[�v�͈̔͂Ƃ��ĔF�����܂��B
	�EMIDI
		MIDI �f�[�^���ɃR���g���[���`�F���W�� 111 �Ԃ�����ƁA
		�Ȃ��Ō�܂ŉ��t�������Ƃ̃��[�v�ʒu�̖ڈ�Ƃ��ĔF�����܂��B
*/
//==============================================================================

#include <lnote.h>

int main()
{
	LNCore_Initialize();

	// �t�@�C�����特���f�[�^���쐬���A���̉����������T�E���h�n���h�����擾���܂��B
	LNHandle sound1;
	LNSound_Create(&sound1, "../Media/cursor1.wav");

	// ���C�����[�v
	LNBool flag;
	do
	{
		// �� �L�[ : �Đ� (LNSound)
		LNInput_IsTrigger(LN_BUTTON_LEFT, &flag);
		if (flag) {
			LNSound_Play(sound1);
		}
		// �� �L�[ : ��~ (LNSound)
		LNInput_IsTrigger(LN_BUTTON_RIGHT, &flag);
		if (flag) {
			LNSound_Stop(sound1);
		}

		// Z �L�[ : BGM �Đ� (LNAudio)
		LNInput_IsTrigger(LN_BUTTON_0, &flag);
		if (flag) {
			LNAudio_PlayBGM("../Media/ln22.ogg", 100, 100, 0);
		}
		// X �L�[ : BGM ��~ (�t�F�[�h�A�E�g) (LNAudio)
		LNInput_IsTrigger(LN_BUTTON_1, &flag);
		if (flag) {
			LNAudio_StopBGM(1000);
		}

		// A �L�[ : BGS �Đ� (LNAudio)
		LNInput_IsTrigger(LN_BUTTON_3, &flag);
		if (flag) {
			LNAudio_PlayBGS("../Media/water02.wav", 50, 100, 0);
		}
		// S �L�[ : BGS ��~ (�t�F�[�h�A�E�g) (LNAudio)
		LNInput_IsTrigger(LN_BUTTON_4, &flag);
		if (flag) {
			LNAudio_StopBGS(1000);
		}

		// Q �L�[ : SE �Đ� (LNAudio)
		LNInput_IsTrigger(LN_BUTTON_6, &flag);
		if (flag) {
			LNAudio_PlaySE("../Media/cursor1.wav", 50, 100);
		}
		// W �L�[ : SE ��~ (LNAudio)
		LNInput_IsTrigger(LN_BUTTON_7, &flag);
		if (flag) {
			LNAudio_StopSE();
		}

		LNCore_Update();
		LNCore_IsEndRequested(&flag);
	} while (flag == LN_FALSE);

	LNCore_Terminate();
	return 0;
}