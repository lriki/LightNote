#include <TestConfig.h>

class Test_Base_String : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST_F(Test_Base_String, Basic)
{
	// uint32_t を型引数にできたりする
	{	
		Base::BasicString<uint32_t> str1;
	}
	// char、wchar_t の相互変換
	{	
		Base::StringA str1;
		str1.assignNativeString("test");
		Base::StringA str2;
		str2.assignNativeString(L"test");
		Base::StringW wstr1;
		wstr1.assignNativeString("test");
		Base::StringW wstr2;
		wstr2.assignNativeString(L"test");

		EXPECT_EQ("test", str1);
		EXPECT_EQ("test", str2);
		EXPECT_EQ(L"test", wstr1);
		EXPECT_EQ(L"test", wstr2);
	}
}

TEST_F(Test_Base_String, Split)
{
	// 普通の分割
	{
		Base::String str = _T("a,b,c");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_None, &lines);
		EXPECT_EQ(3, lines.size());
		EXPECT_EQ(_T("a"), lines[0]);
		EXPECT_EQ(_T("b"), lines[1]);
		EXPECT_EQ(_T("c"), lines[2]);
	}

	// 空文字
	{
		Base::String str = _T("");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_None, &lines);
		EXPECT_EQ(1, lines.size());
	}

	// 空文字の RemoveEmptyEntries
	{
		Base::String str = _T("");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_RemoveEmptyEntries, &lines);
		EXPECT_EQ(0, lines.size());
	}

	// 区切り無し
	{
		Base::String str = _T("abc");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_None, &lines);
		EXPECT_EQ(1, lines.size());
	}

	// 区切り無し RemoveEmptyEntries
	{
		Base::String str = _T("abc");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_RemoveEmptyEntries, &lines);
		EXPECT_EQ(1, lines.size());
	}

	// トークン無し
	{
		Base::String str = _T(",");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_None, &lines);
		EXPECT_EQ(2, lines.size());
	}

	// トークン無し RemoveEmptyEntries
	{
		Base::String str = _T(",");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_RemoveEmptyEntries, &lines);
		EXPECT_EQ(0, lines.size());
	}

	// トークン無し 2
	{
		Base::String str = _T(",,");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_None, &lines);
		EXPECT_EQ(3, lines.size());
	}

	// トークン無し 2 RemoveEmptyEntries
	{
		Base::String str = _T(",,");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_RemoveEmptyEntries, &lines);
		EXPECT_EQ(0, lines.size());
	}

	// 空要素付き
	{
		Base::String str = _T("a,,c");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_None, &lines);
		EXPECT_EQ(3, lines.size());
		EXPECT_EQ(_T("a"), lines[0]);
		EXPECT_EQ(_T(""), lines[1]);
		EXPECT_EQ(_T("c"), lines[2]);
	}

	// 空要素付き RemoveEmptyEntries
	{
		Base::String str = _T("a,,c");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_RemoveEmptyEntries, &lines);
		EXPECT_EQ(2, lines.size());
		EXPECT_EQ(_T("a"), lines[0]);
		EXPECT_EQ(_T("c"), lines[1]);
	}

	// 先頭空要素付き
	{
		Base::String str = _T(",b,c");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_None, &lines);
		EXPECT_EQ(3, lines.size());
		EXPECT_EQ(_T(""), lines[0]);
		EXPECT_EQ(_T("b"), lines[1]);
		EXPECT_EQ(_T("c"), lines[2]);
	}

	// 先頭空要素付き RemoveEmptyEntries
	{
		Base::String str = _T(",b,c");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_RemoveEmptyEntries, &lines);
		EXPECT_EQ(2, lines.size());
		EXPECT_EQ(_T("b"), lines[0]);
		EXPECT_EQ(_T("c"), lines[1]);
	}

	// 終端空要素付き
	{
		Base::String str = _T("a,b,");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_None, &lines);
		EXPECT_EQ(3, lines.size());
		EXPECT_EQ(_T("a"), lines[0]);
		EXPECT_EQ(_T("b"), lines[1]);
		EXPECT_EQ(_T(""), lines[2]);
	}

	// 終端空要素付き RemoveEmptyEntries
	{
		Base::String str = _T("a,b,");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_RemoveEmptyEntries, &lines);
		EXPECT_EQ(2, lines.size());
		EXPECT_EQ(_T("a"), lines[0]);
		EXPECT_EQ(_T("b"), lines[1]);
	}

	// 両端空要素付き
	{
		Base::String str = _T(",b,");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_None, &lines);
		EXPECT_EQ(3, lines.size());
		EXPECT_EQ(_T(""), lines[0]);
		EXPECT_EQ(_T("b"), lines[1]);
		EXPECT_EQ(_T(""), lines[2]);
	}

	// 両端空要素付き RemoveEmptyEntries
	{
		Base::String str = _T(",b,");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_RemoveEmptyEntries, &lines);
		EXPECT_EQ(1, lines.size());
		EXPECT_EQ(_T("b"), lines[0]);
	}
}


