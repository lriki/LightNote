//==============================================================================
// LocaleManager 
//==============================================================================

#pragma once

#include <vector>

namespace LNote
{
namespace Core
{
namespace Resource
{

/// 文字列リソース ID
enum Strings
{
	String_None = 0,
	String_ERR_InvalidOperationException,
	String_ERR_ArgumentException,
	String_ERR_OutOfMemoryException,
	String_ERR_FileNotFoundException,
	String_ERR_SystemCallException,
	String_ERR_NotImplementedException,
	String_ERR_InitializedException,
	String_ERR_InvalidFormatException,
	String_ERR_ThreadException,
	String_ERR_FatalException,
	String_ERR_ComException,
	String_ERR_OpenGLException,
	
	String_Max,
};


//==============================================================================
// LocaleManager
//------------------------------------------------------------------------------
///**
//	@brief      
//*/
//==============================================================================
class LocaleManager
{
public:

	/// 初期化
	static void initialize();

	/// 文字列リソースの取得
	static const wchar_t* getString(int resourceID);

private:
	static std::vector< std::wstring >	mStringTable;
};

} // namespace Resource
} // namespace Core
} // namespace LNote
