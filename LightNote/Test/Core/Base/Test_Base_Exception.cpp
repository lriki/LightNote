#include "../../TestConfig.h"

class Core_Base_Exception : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
TEST_F(Core_Base_Exception, FileNotFound)
{
	try
	{
		LN_THROW_FileNotFound(0, "filename.txt");
	}
	catch (Base::Exception& e)
	{
		EXPECT_EQ(0, wcscmp(e.getMessage(), L"ファイルが存在しないか、アクセスできないファイルです。\nfilename.txt"));
	}
}
