//==============================================================================
// �G���[�����̃T���v��
//------------------------------------------------------------------------------
/*
	LightNote �̊֐��̓G���[����������ƁA
	�f�t�H���g�ł̓��b�Z�[�W�{�b�N�X��\��������A�G���[�R�[�h��Ԃ��܂��B
	
	����͊J�����̗��֐����l����������ŁA
	���t�ɊJ�����s�����͂قƂ�ǂ̏ꍇ�f�t�H���g�̏����Ŗ��Ȃ��Ǝv���܂����A
	�G���[���b�Z�[�W���J�X�^�}�C�Y������A���b�Z�[�W�{�b�N�X���o�͂�����
	���O�ŃG���[�������s�������ꍇ�́A�f�t�H���g�̃G���[�������J�X�^�}�C�Y���邱�Ƃ��ł��܂��B

	�J�X�^�}�C�Y�́ALNException_SetHandler() �œo�^����R�[���o�b�N�֐���
	�������邱�Ƃōs���܂��B
	������͖{�v���O������ ExceptionCallback() ���������������B
*/
//==============================================================================

#include <lnote.h>

void ExceptionCallback( lnErrorCode errCode );

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------
int main()
{
	// �G���[�������̏������J�X�^�}�C�Y�����߂̊֐���o�^����
	// (�R�����g�A�E�g���铙���āA�G���[���̓��삪�ς�邱�Ƃ��m�F���Ă�������)
	LNException_SetHandler( ExceptionCallback );

	if ( LNCore_Initialize() != LN_OK )
		return -1;

	// ���݂��Ȃ��t�@�C�����J�����Ƃ��Ă݂�
	lnResult r = LNAudio_PlayBGM( "xxxx.wav", 100, 100, 0 );
	if ( r != LN_OK )
		goto Exit;

	// ���C�����[�v
	lnBool endRequested;
	do
	{
		LNCore_Update();
		LNCore_IsEndRequested(&endRequested);
	} while (endRequested == LN_FALSE);
Exit:
	LNCore_Terminate();
	return 0;
}

//----------------------------------------------------------------------
// �J�X�^���̃G���[�R�[���o�b�N
//----------------------------------------------------------------------
void ExceptionCallback( lnErrorCode errCode )
{
	switch ( errCode )
	{
	// �t�@�C����������Ȃ������Ƃ��̓R���\�[���ɏo�͂��邾���ɂ���
	case LN_ERR_FILE_NOT_FOUND:
		printf( "�T���v���G���[�o�� : �t�@�C����������܂���ł����B\n" );
		break;
	// ����ȊO�̃G���[�̓f�t�H���g�̏��� (���b�Z�[�W�{�b�N�X�o��) �ɔC����
	default:
		LNException_ProcDefault();
		break;
	}
}
