#include "TestConfig.h"

class Test : public ::testing::Test {
protected:
	virtual void SetUp()
	{
		
		//LNote::fileio

		printf("��SetUp\n");
	}

	virtual void TearDown()
	{
		printf("��TearDown\n");
	}
};

// �e�X�g�t�B�N�X�`�� �N���X�̓e�X�g�֐���1�ȏ㖳���� Setup/TearDown ���Ă΂�Ȃ��̂ŁADummy �����
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