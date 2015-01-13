#include <TestConfig.h>

class Test_Base_String : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST_F(Test_Base_String, Basic)
{
	// uint32_t ���^�����ɂł����肷��
	{	
		Base::BasicString<uint32_t> str1;
	}
	// char�Awchar_t �̑��ݕϊ�
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
	// ���ʂ̕���
	{
		Base::String str = _T("a,b,c");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_None, &lines);
		EXPECT_EQ(3, lines.size());
		EXPECT_EQ(_T("a"), lines[0]);
		EXPECT_EQ(_T("b"), lines[1]);
		EXPECT_EQ(_T("c"), lines[2]);
	}

	// �󕶎�
	{
		Base::String str = _T("");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_None, &lines);
		EXPECT_EQ(1, lines.size());
	}

	// �󕶎��� RemoveEmptyEntries
	{
		Base::String str = _T("");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_RemoveEmptyEntries, &lines);
		EXPECT_EQ(0, lines.size());
	}

	// ��؂薳��
	{
		Base::String str = _T("abc");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_None, &lines);
		EXPECT_EQ(1, lines.size());
	}

	// ��؂薳�� RemoveEmptyEntries
	{
		Base::String str = _T("abc");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_RemoveEmptyEntries, &lines);
		EXPECT_EQ(1, lines.size());
	}

	// �g�[�N������
	{
		Base::String str = _T(",");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_None, &lines);
		EXPECT_EQ(2, lines.size());
	}

	// �g�[�N������ RemoveEmptyEntries
	{
		Base::String str = _T(",");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_RemoveEmptyEntries, &lines);
		EXPECT_EQ(0, lines.size());
	}

	// �g�[�N������ 2
	{
		Base::String str = _T(",,");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_None, &lines);
		EXPECT_EQ(3, lines.size());
	}

	// �g�[�N������ 2 RemoveEmptyEntries
	{
		Base::String str = _T(",,");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_RemoveEmptyEntries, &lines);
		EXPECT_EQ(0, lines.size());
	}

	// ��v�f�t��
	{
		Base::String str = _T("a,,c");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_None, &lines);
		EXPECT_EQ(3, lines.size());
		EXPECT_EQ(_T("a"), lines[0]);
		EXPECT_EQ(_T(""), lines[1]);
		EXPECT_EQ(_T("c"), lines[2]);
	}

	// ��v�f�t�� RemoveEmptyEntries
	{
		Base::String str = _T("a,,c");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_RemoveEmptyEntries, &lines);
		EXPECT_EQ(2, lines.size());
		EXPECT_EQ(_T("a"), lines[0]);
		EXPECT_EQ(_T("c"), lines[1]);
	}

	// �擪��v�f�t��
	{
		Base::String str = _T(",b,c");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_None, &lines);
		EXPECT_EQ(3, lines.size());
		EXPECT_EQ(_T(""), lines[0]);
		EXPECT_EQ(_T("b"), lines[1]);
		EXPECT_EQ(_T("c"), lines[2]);
	}

	// �擪��v�f�t�� RemoveEmptyEntries
	{
		Base::String str = _T(",b,c");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_RemoveEmptyEntries, &lines);
		EXPECT_EQ(2, lines.size());
		EXPECT_EQ(_T("b"), lines[0]);
		EXPECT_EQ(_T("c"), lines[1]);
	}

	// �I�[��v�f�t��
	{
		Base::String str = _T("a,b,");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_None, &lines);
		EXPECT_EQ(3, lines.size());
		EXPECT_EQ(_T("a"), lines[0]);
		EXPECT_EQ(_T("b"), lines[1]);
		EXPECT_EQ(_T(""), lines[2]);
	}

	// �I�[��v�f�t�� RemoveEmptyEntries
	{
		Base::String str = _T("a,b,");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_RemoveEmptyEntries, &lines);
		EXPECT_EQ(2, lines.size());
		EXPECT_EQ(_T("a"), lines[0]);
		EXPECT_EQ(_T("b"), lines[1]);
	}

	// ���[��v�f�t��
	{
		Base::String str = _T(",b,");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_None, &lines);
		EXPECT_EQ(3, lines.size());
		EXPECT_EQ(_T(""), lines[0]);
		EXPECT_EQ(_T("b"), lines[1]);
		EXPECT_EQ(_T(""), lines[2]);
	}

	// ���[��v�f�t�� RemoveEmptyEntries
	{
		Base::String str = _T(",b,");
		std::vector<Base::String> lines;
		str.Split(_T(","), Base::StringSplitOptions_RemoveEmptyEntries, &lines);
		EXPECT_EQ(1, lines.size());
		EXPECT_EQ(_T("b"), lines[0]);
	}
}


