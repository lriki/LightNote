#pragma once

class TestUtils
{
public:
	static Core::FileIO::PathNameW getDirectoryPath(const char* baseSourceFilePath);
	static Core::FileIO::PathNameW getFilePath(const char* baseSourceFilePath, const char* fileName);

	static void SaveScreenCapture(const char* filePath);
};

// �\�[�X�t�@�C���Ɠ����t�H���_�̃t�@�C���p�X���擾����
#define LN_TEST_GET_DIR_PATH TestUtils::getDirectoryPath(__FILE__).getPath()

// �\�[�X�t�@�C���Ɠ����t�H���_�̃t�@�C���p�X���擾����
#define LN_TEST_GET_FILE_PATH(fileName) TestUtils::getFilePath(__FILE__, fileName).getPath()

// �\�[�X�t�@�C���Ɠ����t�H���_�̃p�X���擾����
#define LN_DIR_PATH_CSTR TestUtils::getDirectoryPath(__FILE__).toLocalChar().c_str()

// �\�[�X�t�@�C���Ɠ����t�H���_�̃p�X���擾����
#define LN_DIR_PATH_WSTR TestUtils::getDirectoryPath(__FILE__).getPath()

// �\�[�X�t�@�C���Ɠ����t�H���_�̃t�@�C���p�X���擾����
#define LN_FILE_PATH_CSTR(fileName) TestUtils::getFilePath(__FILE__, fileName).toLocalChar().c_str()

// �\�[�X�t�@�C���Ɠ����t�H���_�̃t�@�C���p�X���擾����
#define LN_FILE_PATH_WSTR(fileName) TestUtils::getFilePath(__FILE__, fileName).getPath()
