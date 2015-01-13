#include <TestConfig.h>
#include <Core/Base/Encoding.h>

class Test_Base_Encoding : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
TEST_F(Test_Base_Encoding, SystemEncodingTest)
{
	setlocale(LC_ALL, "");

	// std::wstring resize テスト
	//std::wstring s;// = L"あいうえお";
	////s.reserve(15);
	//s.resize(15);
	////s = L"あいうえおあいうえおあいうえお";
	//wchar_t* p = &(s[0]);
	//memcpy(p, L"あいうえおあいうえおあいうえお", 15 * sizeof(wchar_t));
	//printf("%d\n", s.size());
	//printf("%d\n", s.capacity());
	//wprintf(L"%s\n", s.c_str());

	// 最小バイト数
	{
		Base::Encoding* e = Base::Encoding::GetSystemMultiByteEncoding();
		EXPECT_EQ(1, e->GetMinByteCount());
	}

	// 最大バイト数
	{
		//setlocale(LC_ALL, "en-US");
		//char* a = setlocale(LC_ALL, ".65001");
		//char* a = setlocale(LC_ALL, "ko-KR.949");
		//char* a = setlocale(LC_ALL, ".949");
		//printf(a);
		//printf("%d\n", _getmbcp());
		Base::Encoding* e = Base::Encoding::GetSystemMultiByteEncoding();
		EXPECT_EQ(6, e->GetMaxByteCount());
	}

	// 同一エンコーディング
	{
		// "日本語表示"
		byte_t str1[] = { 0x93, 0xFA, 0x96, 0x7B, 0x8C, 0xEA, 0x00 };

		lnStringA str2;
		str2.ConvertFrom(str1, 6, Base::Encoding::GetSystemMultiByteEncoding());

		EXPECT_EQ(6, str2.size());


		//std::string ws = "日本語表示";

		//CHLDStringA str;
		//str.ConvertFrom("日本語表示", 10, Base::Encoding::GetSystemEncoding());

		//printf(str.c_str());
	}

	// Multi → Wide
	{
		// "日本語"
		byte_t str1[] = { 0x93, 0xFA, 0x96, 0x7B, 0x8C, 0xEA, 0x00 };

		lnStringW str2;
		str2.ConvertFrom(str1, 6, Base::Encoding::GetSystemMultiByteEncoding());

		EXPECT_EQ(3, str2.size());
		EXPECT_EQ(0x65E5, str2[0]);	// L'日'
		EXPECT_EQ(0x672C, str2[1]);	// L'本'
		EXPECT_EQ(0x8A9e, str2[2]);	// L'語'
		EXPECT_EQ(0x0000, str2[3]);	// L'\0'
	}

	// Wide → Multi 
	{
		std::string s = "日本語";

		// "日本語"
		wchar_t str1[] = { 0x65E5, 0x672C, 0x8A9e, 0x0000 };

		lnStringA str2;
		str2.ConvertFrom(str1, 3 * sizeof(wchar_t), Base::Encoding::GetWideCharEncoding());

		EXPECT_EQ(6, str2.size());
		EXPECT_EQ(0x93, (byte_t)str2[0]);	// '日'	※ unsingned char で比較しないと一致が取れない
		EXPECT_EQ(0xFA, (byte_t)str2[1]);	// '日'
		EXPECT_EQ(0x96, (byte_t)str2[2]);	// '本'
		EXPECT_EQ(0x7B, (byte_t)str2[3]);	// '本'
		EXPECT_EQ(0x8C, (byte_t)str2[4]);	// '語'
		EXPECT_EQ(0xEA, (byte_t)str2[5]);	// '語'
		EXPECT_EQ(0x00, (byte_t)str2[6]);	// L'\0'
	}
}
