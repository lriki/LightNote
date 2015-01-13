//==============================================================================
// �t�@�C�����o�͂̃`���[�g���A��
//------------------------------------------------------------------------------
/*  
	�P���ȃt�@�C���̓ǂݍ��݁E�������݂ł���΁A
	fopen() ���̕W���@�\�Ɣ�ׂĂł��邱�Ƃɑ卷�͂���܂���B

	LightNote �̃t�@�C������n�@�\���g�p����ƁALightNote �Ǝ���
	�Í����A�[�J�C�u����t�@�C����ǂݍ��ނ��Ƃ��ł��܂��B

	�E�Í����A�[�J�C�u�Ƃ́H
		�����[�X�����Q�[����i�Ɋ܂܂��摜�≹���f�[�^�����O����ǂ߂Ȃ��悤�ɁA
		�ЂƂ̃��\�[�X�t�@�C���Ƃ��Ă܂Ƃ߁A�Í�������@�\�ł��B

		�쐬�ɂ͕t���̃c�[�����g�p���A�p�X���[�h�Ō����|���܂��B
		�g�p����Ƃ��� LNConfig_RegisterArchive �Ƀt�@�C�����ƃp�X���[�h���Z�b�g���܂��B
		
		�Í����A�[�J�C�u�̍쐬�菇�c
		�@ Tools/ArchiveMaker.exe �ɁA�A�[�J�C�u���������t�H���_��D&D���܂��B
		�A �p�X���[�h�̓��͂����߂���̂ŁA���͂���Enter�������܂��B
		�B D&D�����t�H���_�Ɠ����ꏊ�ɃA�[�J�C�u�t�@�C������������܂� (�g���q .lna)
		���ۂ̎g�p���@�͖{�v���O�������������������B
*/
//==============================================================================

#include <lnote.h>

int main()
{
	// �Í����A�[�J�C�u��o�^���܂��B
	// ���̏ꏊ�� SampleArchive �Ƃ����t�H���_�����邩�̂悤�ɁA
	// �A�[�J�C�u���̃t�@�C���ɃA�N�Z�X�ł���悤�ɂȂ�܂��B
	LNConfig_RegisterArchive( "../Media/SampleArchive.lna", "test" );

	// LNote ������
	LNCore_Initialize();

	// �A�[�J�C�u���̃t�@�C�����J���܂��B
	LNHandle file1;
	LNFile_Create(&file1, "../Media/SampleArchive/TextFile.txt", LN_FILEMODE_OPEN, LN_FILEACCESS_READ);

	// �t�@�C���T�C�Y���擾���܂��B
	int size;
	LNFile_GetSize(file1, &size);

	// �t�@�C���̓��e��S�� data �ɓǂݍ��݂܂��B
	char data[1000] = { 0 };
	int validSize;
	LNFile_Read(file1, (lnByte*)data, size, &validSize);

	// �t�@�C������܂��B
	LNFile_Close( file1 );

	// �ǂݍ��񂾓��e���e�N�X�`���ɕ`�悵�܂��B
	LNHandle texture1;
	LNRect rect1 = { 0, 0, 640, 480 };
	LNTexture_Create(&texture1, 640, 480);
	LNTexture_DrawText(texture1, data, &rect1);



	// �A�[�J�C�u���̉摜�t�@�C����ǂݍ��݁A�e�N�X�`�����쐬���܂��B
	// �e�N�X�`���Ɍ��炸���������A�t�@�C���A�N�Z�X�𔺂��֐��͂��ׂ�
	// �o�^���ꂽ�A�[�J�C�u�̒���T���ɍs���܂��B
	LNHandle texture2;
	LNTexture_CreateFromFile(&texture2, "../Media/SampleArchive/20140309.png");

	// �e�N�X�`����\�����邽�߂̃X�v���C�g�̗p��
	LNHandle sprite1;
	LNHandle sprite2;
	LNSprite_Create(&sprite2, texture2);
	LNSprite_Create(&sprite1, texture1);
	LNVisualNode_SetOpacity( sprite2, 0.15 );

	// ���C�����[�v
	LNBool flag;
	do
	{
		LNCore_Update();
		LNCore_IsEndRequested(&flag);
	} while (flag == LN_FALSE);

	LNCore_Terminate();
	return 0;
}