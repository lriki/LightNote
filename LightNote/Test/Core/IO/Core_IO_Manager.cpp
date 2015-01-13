#include "../../TestConfig.h"

class Core_IO_Manager : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
TEST_F(Core_IO_Manager, ChangeCurrentDirectory)
{
	// CurDir ���L�����āA���̃\�[�X�t�@�C���� Dir ���J�����g�ɂ���
	wchar_t curDir[LN_MAX_PATH] = { 0 };
	System::Environment::getCurrentDirectory(curDir);
	System::Environment::setCurrentDirectory(LN_DIR_PATH_WSTR);

	// �t�@�C���͑��݂���H
	{
		LNBool result;
		LNResult lr = LNFile_Exists("ArchiveTestText.txt", &result);
		ASSERT_EQ(LN_TRUE, result);
		ASSERT_EQ(::LN_OK, lr);
	}

	// �t�@�C���͊J����H
	{
		LNHandle file;
		LNResult lr = LNFile_Create(&file, "ArchiveTestText.txt");
		ASSERT_NE(NULL, file);
		ASSERT_EQ(::LN_OK, lr);
		LNFile_Close(file);
	}

	// �t�@�C���͑��݂���H(�t�@�C������)
	{
		LNBool result;
		LNResult lr = LNFile_Exists("FFFF.txt", &result);
		ASSERT_EQ(LN_FALSE, result);
		ASSERT_EQ(::LN_OK, lr);
	}

	// �t�@�C���͊J����H(�t�@�C������)
	{
		LNHandle file;
		LNResult lr = LNFile_Create(&file, "FFFF.txt");
		ASSERT_EQ(::LN_ERR_FILE_NOT_FOUND, lr);
		LNFile_Close(file);
	}

	// main() �œo�^�����A�[�J�C�u���̃t�@�C���͑��݃`�F�b�N�ł���H
	{
		LNBool result;
		LNResult lr = LNFile_Exists("../../InArchiveTestText_txt/ArchiveTestText.txt", &result);
		ASSERT_EQ(LN_TRUE, result);
		ASSERT_EQ(::LN_OK, lr);
	}

	// main() �œo�^�����A�[�J�C�u���̃t�@�C���͊J����H
	{
		LNHandle file;
		LNResult lr = LNFile_Create(&file, "../../InArchiveTestText_txt/ArchiveTestText.txt");
		ASSERT_NE(NULL, file);
		ASSERT_EQ(::LN_OK, lr);
		LNFile_Close(file);
	}

	// main() �œo�^�����A�[�J�C�u���̃t�@�C���͑��݃`�F�b�N�ł���H(�t�@�C������)
	{
		LNBool result;
		LNResult lr = LNFile_Exists("../../InArchiveTestText_txt/FFFF.txt", &result);
		ASSERT_EQ(LN_FALSE, result);
		ASSERT_EQ(::LN_OK, lr);
	}

	// main() �œo�^�����A�[�J�C�u���̃t�@�C���͊J����H(�t�@�C������)
	{
		LNHandle file;
		LNResult lr = LNFile_Create(&file, "../../InArchiveTestText_txt/FFFF.txt");
		ASSERT_EQ(::LN_ERR_FILE_NOT_FOUND, lr);
		LNFile_Close(file);
	}

	// �J�����g�f�B���N�g����߂�
	System::Environment::setCurrentDirectory(curDir);
}
