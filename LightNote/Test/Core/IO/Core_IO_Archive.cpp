#include "../../TestConfig.h"
#include "../../../Source/Core/FileIO/Archive.h"
#include "../../../Source/Core/FileIO/ArchiveMaker.h"

class Test_Core_IO_Archive : public ::testing::Test 
{
protected:
	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}
};

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
TEST_F(Test_Core_IO_Archive, Make)
{
	// "test" �Ƃ����f�B���N�g���̉��� "ArchiveTestText.txt" ������C���[�W
	Core::FileIO::ArchiveMaker maker;
	maker.open(LN_TEST_GET_FILE_PATH("test.lna"), "test");
	maker.addFile(LN_TEST_GET_FILE_PATH("ArchiveTestText.txt"), L"ArchiveTestText.txt");
	maker.close();

	// ������A�[�J�C�u���J��
	Core::FileIO::Archive archive;
	archive.initialize(LN_DIR_PATH_CSTR, "test.lna", "test");
	FileIO::Stream* stream = archive.createStream(LN_FILE_PATH_WSTR("test/ArchiveTestText.txt"), Core::FileIO::FileType_Binary);
	
	// �A�[�J�C�u���̃t�@�C�����J��
	Base::ReferenceBuffer* buf1 = LN_NEW Base::ReferenceBuffer();
	buf1->reserve(stream->getSize());
	stream->read(buf1->getPointer(), stream->getSize());

	// ���̃t�@�C�����J��
	Base::ReferenceBuffer* buf2 = Core::FileIO::FileUtils::readAllBytes(LN_FILE_PATH_CSTR("ArchiveTestText.txt"));

	// ���g��r
	EXPECT_EQ(buf1->getSize(), buf2->getSize());
	EXPECT_EQ(0, memcmp(buf1->getPointer(), buf2->getPointer(), buf1->getSize()));

	stream->release();
	buf1->release();
	buf2->release();
}
