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
	// CurDir を記憶して、このソースファイルの Dir をカレントにする
	wchar_t curDir[LN_MAX_PATH] = { 0 };
	System::Environment::getCurrentDirectory(curDir);
	System::Environment::setCurrentDirectory(LN_DIR_PATH_WSTR);

	// ファイルは存在する？
	{
		LNBool result;
		LNResult lr = LNFile_Exists("ArchiveTestText.txt", &result);
		ASSERT_EQ(LN_TRUE, result);
		ASSERT_EQ(::LN_OK, lr);
	}

	// ファイルは開ける？
	{
		LNHandle file;
		LNResult lr = LNFile_Create(&file, "ArchiveTestText.txt");
		ASSERT_NE(NULL, file);
		ASSERT_EQ(::LN_OK, lr);
		LNFile_Close(file);
	}

	// ファイルは存在する？(ファイル無し)
	{
		LNBool result;
		LNResult lr = LNFile_Exists("FFFF.txt", &result);
		ASSERT_EQ(LN_FALSE, result);
		ASSERT_EQ(::LN_OK, lr);
	}

	// ファイルは開ける？(ファイル無し)
	{
		LNHandle file;
		LNResult lr = LNFile_Create(&file, "FFFF.txt");
		ASSERT_EQ(::LN_ERR_FILE_NOT_FOUND, lr);
		LNFile_Close(file);
	}

	// main() で登録したアーカイブ内のファイルは存在チェックできる？
	{
		LNBool result;
		LNResult lr = LNFile_Exists("../../InArchiveTestText_txt/ArchiveTestText.txt", &result);
		ASSERT_EQ(LN_TRUE, result);
		ASSERT_EQ(::LN_OK, lr);
	}

	// main() で登録したアーカイブ内のファイルは開ける？
	{
		LNHandle file;
		LNResult lr = LNFile_Create(&file, "../../InArchiveTestText_txt/ArchiveTestText.txt");
		ASSERT_NE(NULL, file);
		ASSERT_EQ(::LN_OK, lr);
		LNFile_Close(file);
	}

	// main() で登録したアーカイブ内のファイルは存在チェックできる？(ファイル無し)
	{
		LNBool result;
		LNResult lr = LNFile_Exists("../../InArchiveTestText_txt/FFFF.txt", &result);
		ASSERT_EQ(LN_FALSE, result);
		ASSERT_EQ(::LN_OK, lr);
	}

	// main() で登録したアーカイブ内のファイルは開ける？(ファイル無し)
	{
		LNHandle file;
		LNResult lr = LNFile_Create(&file, "../../InArchiveTestText_txt/FFFF.txt");
		ASSERT_EQ(::LN_ERR_FILE_NOT_FOUND, lr);
		LNFile_Close(file);
	}

	// カレントディレクトリを戻す
	System::Environment::setCurrentDirectory(curDir);
}
