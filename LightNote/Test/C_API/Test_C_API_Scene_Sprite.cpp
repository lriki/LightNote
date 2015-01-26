#include <TestConfig.h>

class Test_C_API_Scene_Sprite : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_C_API_Scene_Sprite, Create)
{
	LNHandle tex1;
	LNHandle sprite1;
	LNTexture_CreateFromFile(&tex1, LN_FILE_PATH_CSTR("TestData/TestImg1.png"));
	LNSprite_Create(&sprite1, tex1);


	LNCore_Update();
	LNCore_Update();
	LNCore_Update();

	TestUtils::SaveScreenCapture(LN_FILE_PATH_CSTR("ScreenCapture.png"));

	LNObject_Release(tex1);
	LNObject_Release(sprite1);
}