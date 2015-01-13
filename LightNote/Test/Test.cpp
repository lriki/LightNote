#include "TestConfig.h"

class Test : public ::testing::Test {
protected:
	virtual void SetUp()
	{
		
		//LNote::fileio

		printf("◆SetUp\n");
	}

	virtual void TearDown()
	{
		printf("◆TearDown\n");
	}
};

// テストフィクスチャ クラスはテスト関数が1つ以上無いと Setup/TearDown が呼ばれないので、Dummy を作る
TEST_F(Test, Dummy)
{
	printf("Run LNote test.\n");
}


//#include <stdio.h>
//
//#include "gtest/gtest.h"

GTEST_API_ int main(int argc, char **argv) 
{
	printf("Running main()\n");

	LNConfig_SetExceptionMessageBoxEnabled(LN_FALSE);
	LNConfig_RegisterArchive(LN_FILE_PATH_CSTR("InArchiveTestText_txt.lna"), "test");
	LNCore_Initialize();

	testing::InitGoogleTest(&argc, argv);
	int r = RUN_ALL_TESTS();

	LNCore_Terminate();
	return r;
}