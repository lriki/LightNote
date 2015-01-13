//=============================================================================
//【 CanvasBase 】
//=============================================================================

#include "stdafx.h"
#include "../../Base/Parser.h"
#include "../../FileIO/Manager.h"
#include "ShaderBase.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//=============================================================================
// ■ ShaderBase
//=============================================================================

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	ShaderBase::ShaderBase()
	{
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	ShaderBase::~ShaderBase() 
	{
		
	}

//=============================================================================
// ■ HSLSAnalyzer
//=============================================================================

	class IncludeLineFinder
		: public Base::TokenAnalyzer
	{
	public:
		/// #include ひとつ分
		struct IncludeLineInfo
		{
			int			LineCount;
			lnRefString	FilePath;
			lnRefString	LineStr;	// # から 終端の " または > まで
		};

	public:
		int								LineCount;	///< 行数 (1スタート)
		std::vector<IncludeLineInfo>	IncludeLineInfoArray;

	private:

		enum OperatorType 
		{
			OT_Sharp,		// #
			OT_DoubleQuote,	// "
			OT_LeftAngle,
			OT_RightAngle,
		};

		enum KeywordType 
		{
			KT_Include,
		};

		enum PreProSeq
		{
			PreProSeq_Idle,
			PreProSeq_FoundSharp,
			PreProSeq_FoundInclude,
			PreProSeq_FilePath,
		};

	public:
		IncludeLineFinder(/* FileIO::Manager* ioManager */)
		{
			//mIOManager = ioManager;
			LineCount = 1;
			mPreProSeq = PreProSeq_Idle;
			mFilePathBegin = NULL;
		}

	public:
		virtual const Base::OperatorCode* getOperatorList()
		{
			static const Base::OperatorCode codes[] = 
			{
				{ _T("#"),  1, OT_Sharp },
				{ _T("\""),	1, OT_DoubleQuote },
				{ _T("<"),	1, OT_LeftAngle },
				{ _T(">"),	1, OT_RightAngle },
				{ NULL,     0, 0 }
			};
			return codes;
		}

		virtual const Base::KeywordCode* getKeywordList()
		{
			static const Base::KeywordCode codes[] = 
			{
				{ _T("include"),  7, KT_Include },
				{ NULL,     0, 0 }
			};
			return codes;
		}


		virtual void analyzeToken( const Base::TokenCode& token )
		{
			// TODO: コメント未考慮

			// ファイルパス解析中
			if ( mFilePathBegin )
			{
				if ( token.Type == Base::TOKEN_TYPE_OPERATOR ) {
					if (token.UserType == OT_DoubleQuote ||
						token.UserType == OT_RightAngle ) {

						IncludeLineInfo info;
						info.LineCount = LineCount;
						info.FilePath = lnRefString( 
							(mFilePathBegin + 1), 
							((token.End - 1) - (mFilePathBegin + 1)));
						info.LineStr = lnRefString( 
							mLastSharpPos,
							(token.End - mLastSharpPos));

						// 再帰


						IncludeLineInfoArray.push_back( info );

						mFilePathBegin = NULL;
						mLastSharpPos = NULL;
						mPreProSeq = PreProSeq_Idle;
					}
				}
				return;
			}

			// 改行
			if ( token.Type == Base::TokenType_NewlLine ) {
				++LineCount;
				mPreProSeq = PreProSeq_Idle;
				return;
			}

			// # を見つけに行く
			if ( mPreProSeq == PreProSeq_Idle ) {
				if ( token.Type == Base::TOKEN_TYPE_OPERATOR ) {
					if ( token.UserType == OT_Sharp ) {
						mPreProSeq = PreProSeq_FoundSharp;
						mLastSharpPos = token.Begin;
					}
				}
				return;
			}
			// include を見つけに行く
			if ( mPreProSeq == PreProSeq_FoundSharp ) {
				if ( token.Type == Base::TokenType_Keyword ) {
					if ( token.UserType == KT_Include ) {
						mPreProSeq = PreProSeq_FoundInclude;
					}
				}
				return;
			}
			// " か < を見つけに行く
			if ( mPreProSeq == PreProSeq_FoundInclude ) {
				if ( token.Type == Base::TOKEN_TYPE_OPERATOR ) {
					if ( token.UserType == OT_DoubleQuote ||
						 token.UserType == OT_LeftAngle ) {
						mPreProSeq = PreProSeq_FilePath;
						mFilePathBegin = token.Begin;
					}
				}
				return;
			}
		}

	private:
		//FileIO::Manager*	mIOManager;
		PreProSeq		mPreProSeq;
		const lnChar*	mLastSharpPos;
		const lnChar*	mFilePathBegin;
	};

	/// シェーダファイルひとつ分
	class FileInfo
	{
	public:
		FileInfo( FileIO::Manager* ioManager, ShaderErrorInfo* errorInfo )
		{
			mIOManager = ioManager;
			mErrorInfo = errorInfo;
		}

	public:

		/// 解析スタート
		void analyze( const char* data, int size )
		{
			mSourceBuffer.reserve( (const lnByte*)data, size );
			_analyze();
		};

		/// 結合後内容
		lnString& getOutput() { return mOutput; };

	private:
		void _analyze()
		{
			// まず最初に include 行と必要なファイルパスを見つける
			// (置換と同時にでもできるけど、ちょっと複雑になるので)
			Base::Parser parser;
			IncludeLineFinder includeLineFinder;
			parser.analyze( (const char*)mSourceBuffer.getPointer(), &includeLineFinder );

			// とりあえず全部置換
			mOutput = lnString( (const char*)mSourceBuffer.getPointer(), mSourceBuffer.getSize() );
			ln_foreach( IncludeLineFinder::IncludeLineInfo& info, includeLineFinder.IncludeLineInfoArray )
			{

				//LRefPtr<FileInfo> childFile( LN_NEW FileInfo( mIOManager, mErrorInfo ) );
				FileInfo childFile( mIOManager, mErrorInfo );
				try
				{
					// ファイルを開いて子FileInfoに全て読み込む
					LRefPtr<FileIO::InFile> file( mIOManager->createInFile( info.FilePath.c_str() ) );
					childFile.mSourceBuffer.reserve( file->getSize() );
					file->read( childFile.mSourceBuffer.getPointer(), childFile.mSourceBuffer.getSize() );
				}
				catch (Base::Exception& e)
				{
					lnString err = _T("not found include file : ");
					err += info.FilePath.c_str();
					mErrorInfo->addError( err.c_str(), _T(""), 0, 0 );
				}

				// ちゃんと読みこめていれば、解析を行う
				if ( childFile.mSourceBuffer.getSize() != 0 ) {
					
					// include の相対パスは、ルートのファイルの存在するディレクトリを基準とする
					// (HLSL の仕様っぽい)
#if 0
					lnRefString dir = FileIO::Path::getDirectoryPath( info.FilePath.c_str() );
					if ( !dir.empty() ) {
						mIOManager->moveCurrentDirectory( dir.c_str() );
					}
#endif
					childFile._analyze();
#if 0
					if ( !dir.empty() ) {
						mIOManager->returnCurrentDirectory();
					}
#endif
					// # ～　" or > までを、子ファイル内容で置換する
					Base::StringUtils::replaceString( &mOutput, info.LineStr.c_str(), childFile.mOutput.c_str() );
				}

			}
		}

	private:
		FileIO::Manager*			mIOManager;
		ShaderErrorInfo*		mErrorInfo;
		Base::ReferenceBuffer	mSourceBuffer;	///< ソースファイル
		lnString				mOutput;
	};

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	HSLSAnalyzer::HSLSAnalyzer( FileIO::Manager* ioManager )
	{
		mIOManager = ioManager;
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	HSLSAnalyzer::~HSLSAnalyzer()
	{
	}

	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void HSLSAnalyzer::analyze( const char* data, int size )
	{
		// まず最初に include 行と必要なファイルパスを見つける
		// (置換と同時にでもできるけど、ちょっと複雑になるので)
		Base::Parser parser;
		IncludeLineFinder includeLineFinder;
		parser.analyze( data, &includeLineFinder );

		FileInfo fileInfo( mIOManager, &mErrorInfo );
		fileInfo.analyze( data, size );
		mOutput = fileInfo.getOutput();

#if 0
		// とりあえず全部置換
		mOutput = lnString( data, size );
		ln_foreach( IncludeLineFinder::IncludeLineInfo& info, includeLineFinder.IncludeLineInfoArray )
		{
			try
			{
				LRefPtr<FileIO::InFile> file( mIOManager->createInFile( info.FilePath.c_str() ) );
				file->load();

				Base::StringUtil::replaceString( &mOutput, info.LineStr.c_str(), (char*)file->getData() );
			}
			catch (Base::Exception& e)
			{
				mErrorInfo.addError( _T("not found include file"), _T(""), 0, 0 );
			}
#if 0
			if ( mIOManager->existsFile( info.FilePath.c_str() ) )
			{
				LRefPtr<FileIO::InFile> file( mIOManager->createInFile( info.FilePath.c_str() ) );
				file->load();

				Base::StringUtil::replaceString( &mOutput, info.LineStr.c_str(), (char*)file->getData() );
			}
			else
			{
				mErrorInfo.addError( _T("not found include file"), _T(""), 0, 0 );
			}
#endif

		}
#endif
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//
//=============================================================================