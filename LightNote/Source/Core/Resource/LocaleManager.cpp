//==============================================================================
// LocaleManager 
//==============================================================================

#include "stdafx.h"
#include "LocaleManager.h"

namespace LNote
{
namespace Core
{
namespace Resource
{

	//static const lnChar* ERR_InvalidCallException = L"���\�b�h�̌Ăяo���������ł��B�����ɕs���Ȓl���n���ꂽ�\��������܂��B");
	//static const lnChar* ERR_OutOfMemoryException = L"�������������̂ɏ\���ȃ������̈悪����܂���B");
	//static const lnChar* ERR_FileNotFoundException = L"�t�@�C�������݂��Ȃ����A�A�N�Z�X�ł��Ȃ��t�@�C���ł��B");
	//static const lnChar* ERR_SystemCallException = L"�V�X�e���@�\�̌Ăяo���ŃG���[���������܂����B");
	//static const lnChar* ERR_FileFormatException = L"�Ή����Ă��Ȃ��t�@�C���t�H�[�}�b�g�ł��B");
	//static const lnChar* ERR_NotImplementedException = L"�������A�܂��͖��Ή��̋@�\���Ăяo����܂����B");
	//static const lnChar* ERR_InitializedException = L"�I�u�W�F�N�g�͊��ɏ���������Ă��܂��B");
	//static const lnChar* ERR_FormatException = L"�����ȃt�H�[�}�b�g�ł��B");
	//static const lnChar* ERR_ThreadException = L"�X���b�h���Ńn���h������Ȃ���O���������܂����B");
	//static const lnChar* ERR_FatalException = L"�v���I�ȃG���[���������܂����B");
	//static const lnChar* ERR_ComException = L"HRESULT �ɃG���[���Ԃ���܂����B");
	//static const lnChar* ERR_OpenGLException = L"OpenGL �ŃG���[���Ԃ���܂����B");

	//static const wchar_t* ERR_ThreadExceptionAbortW = L"���s���̃X���b�h�ŗ�O���������܂����B";

//==============================================================================
// LocaleManager
//==============================================================================

std::vector< std::wstring >	LocaleManager::mStringTable;

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void LocaleManager::initialize()
{
	mStringTable.resize(String_Max);
	mStringTable[String_ERR_InvalidOperationException] = L"�I�u�W�F�N�g�̏�Ԃɑ΂��鑀�삪�����ł��B";
	mStringTable[String_ERR_ArgumentException] = L"�s���Ȉ����ł��B";
	mStringTable[String_ERR_OutOfMemoryException] = L"�������������̂ɏ\���ȃ������̈悪����܂���B";
	mStringTable[String_ERR_FileNotFoundException] = L"�t�@�C�������݂��Ȃ����A�A�N�Z�X�ł��Ȃ��t�@�C���ł��B";
	mStringTable[String_ERR_SystemCallException] = L"�V�X�e���@�\�̌Ăяo���ŃG���[���������܂����B";
	mStringTable[String_ERR_NotImplementedException] = L"�������A�܂��͖��Ή��̋@�\���Ăяo����܂����B";
	mStringTable[String_ERR_InitializedException] = L"�I�u�W�F�N�g�͊��ɏ���������Ă��܂��B";
	mStringTable[String_ERR_InvalidFormatException] = L"�����ȃt�H�[�}�b�g�ł��B";
	mStringTable[String_ERR_ThreadException] = L"�X���b�h���Ńn���h������Ȃ���O���������܂����B";
	mStringTable[String_ERR_FatalException] = L"�v���I�ȃG���[���������܂����B";
	mStringTable[String_ERR_ComException] = L"HRESULT �ɃG���[���Ԃ���܂����B";
	mStringTable[String_ERR_OpenGLException] = L"OpenGL �ŃG���[���Ԃ���܂����B";
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
const wchar_t* LocaleManager::getString(int resourceID)
{
	return mStringTable[resourceID].c_str();
}

} // namespace Resource
} // namespace Core
} // namespace LNote
