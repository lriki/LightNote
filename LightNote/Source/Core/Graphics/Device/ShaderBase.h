//=============================================================================
//【 ShaderBase 】
//-----------------------------------------------------------------------------
///**
//  @file       ShaderBase.h
//  @brief      ShaderBase
//  @author     Riki
//*/
//=============================================================================

#pragma once

#include "../../Base/Cache.h"
#include "../../Base/ReferenceBuffer.h"
#include "../Interface.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//=============================================================================
// ■ ShaderBase
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class ShaderBase
    : public IShader
	, public Base::ICacheObject
{
	LN_CACHE_OBJECT_DECL;
public:
	ShaderBase();
	virtual ~ShaderBase();
};

//=============================================================================
// ■ ShaderErrorInfo
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class ShaderErrorInfo
    //: public Base::CacheObject
{
public:
	struct Info
	{
		lnRefString	Message;
		lnRefString	FilePath;
		int			Line;
		int			Column;
		bool		IsError;	///< true=エラー false=警告
	};

	std::vector<Info>	InfoArray;

public:
	ShaderErrorInfo() {}
	virtual ~ShaderErrorInfo() {}

public:

	void addError( const lnRefString& message, const lnRefString& file, int line, int column )
	{
		Info info;
		info.Message	= message;
		info.FilePath	= file;
		info.Line		= line;
		info.Column		= column;
		info.IsError	= true;
		InfoArray.push_back( info );
	}

	void addWarning( const lnRefString& message, const lnRefString& file, int line, int column )
	{
		Info info;
		info.Message	= message;
		info.FilePath	= file;
		info.Line		= line;
		info.Column		= column;
		info.IsError	= false;
		InfoArray.push_back( info );
	}

private:
};

//=============================================================================
// ■ HSLSAnalyzer
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class HSLSAnalyzer
{
public:
	HSLSAnalyzer( FileIO::Manager* ioManager );
	~HSLSAnalyzer();

public:

	/// 解析 (ASCIIと被らない文字コードは可能。改行コードはなんでもOK。wchar_t 不可)
	void analyze( const char* data, int size );

	/// 結合済み文字列バッファの取得
	lnString& getOutput() { return mOutput; }

	/// 物理行を論理行に変換する
	int getLogicalLine( int physicalLine );

private:

	void _addIncludeBuffer( const char* filePath );

private:

	struct PhysicalLineBlock
	{
		int		StartPhysicalLine;
	};

	typedef std::map<lnString, Base::ReferenceBuffer*>	IncludeBufferMap;
	typedef std::pair<lnString, Base::ReferenceBuffer*>	IncludeBufferPair;
	
	FileIO::Manager*		mIOManager;
	IncludeBufferMap	mIncludeBufferMap;
	lnString			mOutput;
	ShaderErrorInfo		mErrorInfo;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//
//=============================================================================