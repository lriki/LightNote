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

	// std::wstring resize �e�X�g
	//std::wstring s;// = L"����������";
	////s.reserve(15);
	//s.resize(15);
	////s = L"������������������������������";
	//wchar_t* p = &(s[0]);
	//memcpy(p, L"������������������������������", 15 * sizeof(wchar_t));
	//printf("%d\n", s.size());
	//printf("%d\n", s.capacity());
	//wprintf(L"%s\n", s.c_str());

	// �ŏ��o�C�g��
	{
		Base::Encoding* e = Base::Encoding::GetSystemMultiByteEncoding();
		EXPECT_EQ(1, e->GetMinByteCount());
	}

	// �ő�o�C�g��
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

	// ����G���R�[�f�B���O
	{
		// "���{��\��"
		byte_t str1[] = { 0x93, 0xFA, 0x96, 0x7B, 0x8C, 0xEA, 0x00 };

		lnStringA str2;
		str2.ConvertFrom(str1, 6, Base::Encoding::GetSystemMultiByteEncoding());

		EXPECT_EQ(6, str2.size());


		//std::string ws = "���{��\��";

		//CHLDStringA str;
		//str.ConvertFrom("���{��\��", 10, Base::Encoding::GetSystemEncoding());

		//printf(str.c_str());
	}

	// Multi �� Wide
	{
		// "���{��"
		byte_t str1[] = { 0x93, 0xFA, 0x96, 0x7B, 0x8C, 0xEA, 0x00 };

		lnStringW str2;
		str2.ConvertFrom(str1, 6, Base::Encoding::GetSystemMultiByteEncoding());

		EXPECT_EQ(3, str2.size());
		EXPECT_EQ(0x65E5, str2[0]);	// L'��'
		EXPECT_EQ(0x672C, str2[1]);	// L'�{'
		EXPECT_EQ(0x8A9e, str2[2]);	// L'��'
		EXPECT_EQ(0x0000, str2[3]);	// L'\0'
	}

	// Wide �� Multi 
	{
		std::string s = "���{��";

		// "���{��"
		wchar_t str1[] = { 0x65E5, 0x672C, 0x8A9e, 0x0000 };

		lnStringA str2;
		str2.ConvertFrom(str1, 3 * sizeof(wchar_t), Base::Encoding::GetWideCharEncoding());

		EXPECT_EQ(6, str2.size());
		EXPECT_EQ(0x93, (byte_t)str2[0]);	// '��'	�� unsingned char �Ŕ�r���Ȃ��ƈ�v�����Ȃ�
		EXPECT_EQ(0xFA, (byte_t)str2[1]);	// '��'
		EXPECT_EQ(0x96, (byte_t)str2[2]);	// '�{'
		EXPECT_EQ(0x7B, (byte_t)str2[3]);	// '�{'
		EXPECT_EQ(0x8C, (byte_t)str2[4]);	// '��'
		EXPECT_EQ(0xEA, (byte_t)str2[5]);	// '��'
		EXPECT_EQ(0x00, (byte_t)str2[6]);	// L'\0'
	}
}
