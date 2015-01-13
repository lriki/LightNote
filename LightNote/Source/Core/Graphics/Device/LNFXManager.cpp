//==============================================================================
// LNFXManager 
//==============================================================================

#include "stdafx.h"
#include <tinyxml2/tinyxml2.h>
#include "../../Resource/LNResource.h"
#include "LNFXManager.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ LNFXManager
//==============================================================================

	const lnChar* LNFXManager::Magic = _T("lnfx");

	const char*		LNFXManager::LNoteHLDLMacro = "LNOTE_HLSL";

	//const lnChar* LNFXFile::VariableElementName = _T("Variable");
	//const lnChar* LNFXFile::AnnotationElementName = _T("Annotation");
	//const lnChar* LNFXFile::TechniqueElementName = _T("Technique");
	//const lnChar* LNFXFile::PassElementName = _T("Pass");
	//const lnChar* LNFXFile::VertexShaderElementName = _T("VertexShader");
	//const lnChar* LNFXFile::PixelShaderElementName = _T("PixelShader");
	//const lnChar* LNFXFile::RenderStateElementName = _T("RenderState");

	//const lnChar* LNFXFile::NameAttributeName = _T("Name");
	//const lnChar* LNFXFile::SemanticAttributeName = _T("Semantic");
	//const lnChar* LNFXFile::SharedAttributeName = _T("Shared");
	//const lnChar* LNFXFile::TextureAttributeName = _T("Texture");
	//const lnChar* LNFXFile::ValueAttributeName = _T("Value");
	//const lnChar* LNFXFile::EntryPointAttributeName = _T("Shader");

	const lnChar*	LNFXManager::BeginHLSLTag = "@BEGINHLSL";
	const lnChar*	LNFXManager::EndHLSLTag = "@ENDHLSL";
	const lnChar*	LNFXManager::BeginGLSLTag = "@BEGINGLSL[";
	const lnChar*	LNFXManager::EndGLSLTag = "@ENDGLSL";
	const lnChar*	LNFXManager::BeginInfoTag	= "@BEGININFO";
	const lnChar*	LNFXManager::EndInfoTag	= "@ENDINFO";

	const lnChar*	LNFXManager::VariableElementName = _T("Variable");
	const lnChar*	LNFXManager::AnnotationElementName = _T("Annotation");
	const lnChar*	LNFXManager::TechniqueElementName = _T("Technique");
	const lnChar*	LNFXManager::PassElementName = _T("Pass");
	const lnChar*	LNFXManager::VertexShaderElementName = _T("VertexShader");
	const lnChar*	LNFXManager::PixelShaderElementName = _T("PixelShader");
	const lnChar*	LNFXManager::RenderStateElementName = _T("RenderState");

	const lnChar*	LNFXManager::NameAttributeName = _T("Name");
	const lnChar*	LNFXManager::SemanticAttributeName = _T("Semantic");
	const lnChar*	LNFXManager::SharedAttributeName = _T("Shared");
	const lnChar*	LNFXManager::TextureAttributeName = _T("Texture");
	const lnChar*	LNFXManager::ValueAttributeName = _T("Value");
	const lnChar*	LNFXManager::EntryPointAttributeName = _T("EntryPoint");

	const char*		LNFXManager::BeginHLSLTag_ReplaceTarget = "#ifdef LNOTE_HLSL";
	const char*		LNFXManager::EndHLSLTag_ReplaceTarget = "#endif";

	const char*		LNFXManager::BeginGLSLTag_ReplaceTarget = "#ifdef LNOTE_GLSL_";
	const char*		LNFXManager::EndGLSLTag_ReplaceTarget = "#endif";

	const char*		LNFXManager::BeginInfoTag_ReplaceTarget = "#ifdef LNOTE_INFO";
	const char*		LNFXManager::EndInfoTag_ReplaceTarget = "#endif";

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    LNFXManager::LNFXManager()
		: mOutStream	( NULL )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    LNFXManager::~LNFXManager()
    {
		closeOutFile();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNFXManager::open( FileIO::Stream* stream, LNGraphicsAPI api, bool addDepsInclude, IGLShaderBuilder* glShaderBuilder )
	{
		char magic[6] = { 0 };	// "/*lnfx"
		int lnfxVer = 0;
		stream->read( magic, 6 );
		if ( strncmp( magic, "/*lnfx", 6 ) == 0 ) {
			stream->read( magic, 1 );	// バッファ使い回し
			lnfxVer = magic[0] - '0';
		}
		stream->seek( SEEK_SET, 0 );

		// 一時バッファに読む
		LRefPtr<Base::ReferenceBuffer> tmpBuffer( LN_NEW Base::ReferenceBuffer() );
		tmpBuffer->reserve( stream->getSize() );
		stream->read( tmpBuffer->getPointer(), tmpBuffer->getSize() );
		lnString code = lnString( (const char*)tmpBuffer->getPointer(), tmpBuffer->getSize() );
		
		// lnfx ファイル。色々なタグを置換していく
		if (lnfxVer > 0)
		{
			// 置換 "@BEGINHLSL" → "#ifdef LNOTE_HLSL"
			Base::StringUtils::replaceString( &code, BeginHLSLTag, BeginHLSLTag_ReplaceTarget );
			// 置換 "@ENDHLSL"   → "#endif"
			Base::StringUtils::replaceString( &code, EndHLSLTag, EndHLSLTag_ReplaceTarget );
		
			// 置換 "@BEGINGLSL[xxxx]" → #ifdef LNOTE_GLSL_xxxx
			const int BeginGLSLTagLength = strlen(BeginGLSLTag);
			std::string::size_type beginPos = 0;
			while (true)
			{
				// 開始タグ検索
				beginPos = code.find( BeginGLSLTag, beginPos );
				if ( beginPos != std::string::npos ) 
				{
					// タグ名終端
					std::string::size_type tagEndBracket = code.find( ']', beginPos );
					if ( tagEndBracket != std::string::npos ) 
					{
						// タグ名
						lnString tagName( code.c_str(), beginPos + BeginGLSLTagLength, tagEndBracket - beginPos - BeginGLSLTagLength );
					
						// 置換対象名
						lnString from = BeginGLSLTag + tagName + ']';

						// 置換後
						lnString to = BeginGLSLTag_ReplaceTarget;
						to += tagName;

						// 置換
						Base::StringUtils::replaceString( &code, from.c_str(), to.c_str() );
					}
				}
				else {
					// 見つからなかった
					break;
				}
				beginPos++;
			}
			// 置換 "@ENDHLSL"   → "#endif"
			Base::StringUtils::replaceString( &code, EndGLSLTag, EndGLSLTag_ReplaceTarget );

			// 置換 "@BEGININFO" → "#ifdef LNOTE_INFO"
			Base::StringUtils::replaceString( &code, BeginInfoTag, BeginInfoTag_ReplaceTarget );
			// 置換 "@ENDINFO"   → "#endif"
			Base::StringUtils::replaceString( &code, EndInfoTag, EndInfoTag_ReplaceTarget );

			// Info 部分は取りだしておく
			beginPos = code.find( BeginInfoTag, 0 );
			if ( beginPos != std::string::npos ) {
				std::string::size_type endPos = code.find( EndInfoTag, beginPos );
				if ( endPos != std::string::npos ) {
					mInfoCode = lnString( 
						code, 
						beginPos + strlen(BeginInfoTag), 
						endPos - beginPos - strlen(BeginInfoTag) );
				}
			}
		}
		else
		{
			// lnfx でないファイルはそのまま
		}
		
		mHLSLCode = code;
#if 0
		//------------------------------------------------------
		// DirectX9
		if ( api == LN_GRAPHICSAPI_DIRECTX9 )
		{
			// ".lnfx" 形式のファイル。
			if ( lnfxVer > 0 )
			{
				//mHLSLCode = code;
				// 置換 "@BEGINHLSL" → "#ifdef LNOTE_HLSL"
				Base::StringUtil::replaceString( &mHLSLCode, BeginHLSLTag, BeginHLSLTag_ReplaceTarget );
				// 置換 "@ENDHLSL"   → "#endif"
				Base::StringUtil::replaceString( &mHLSLCode, EndHLSLTag, EndHLSLTag_ReplaceTarget );
				
				
				/*
				mHLSLCode.clear();
				std::string::size_type beginPos = 0;
				while (true)
				{
					// "@BEGINHLSL" を探す
					beginPos = code.find( BeginHLSLTag, beginPos );
					if ( beginPos != std::string::npos ) {
						// "@ENDHLSL" を探す
						std::string::size_type endPos = code.find( EndHLSLTag, beginPos );
						if ( endPos != std::string::npos ) {
							// 末尾に追加していく
							mHLSLCode += lnString( 
								code, 
								beginPos + strlen(BeginHLSLTag), 
								endPos - beginPos - strlen(BeginHLSLTag) );
						}
					}
					else
					{
						// "@BEGINHLSL" が見つからなかった。終了
						break;
					}
					beginPos++;
				}
				*/
			}
			// ".fx"
			else
			{
				

				//mHLSLCode = code;//lnString( (const char*)tmpBuffer->getPointer(), tmpBuffer->getSize() );

				// 自動 include
				if ( addDepsInclude ) 
				{
					const char* data;
					size_t size;
					Resource::StaticResource::loadData( Resource::ID_Shader_MMM_EffectHeader, (const void**)&data, &size );
					
					mHLSLCode  = lnString(data);	// リソースには終端 NULL が付いてくるのでコレで
					mHLSLCode += "\n#line 1\n";
					mHLSLCode += code;

					//mHLSLCode = lnString(data, size) + lnString("\n#line 1\n") + mHLSLCode;

					
					//_p(mHLSLCode.c_str());
					//_p(code.c_str());
					//	static const char MME_MMM_EffectHeader[] = 
							//"";
					//		"#include <LNResource\\Shader\\MMM_EffectHeader.h>\n#line 2";

					//mHLSLCode = lnString(MME_MMM_EffectHeader) + mHLSLCode;
				}
				else {
					mHLSLCode = code;
				}
			}
		}
		//------------------------------------------------------
		// OpenGL
		else if ( api == LN_GRAPHICSAPI_OPENGL )
		{
			const int BeginGLSLTagLength = strlen(BeginGLSLTag);

			// TODO: エラー行のための #line

			// GLSL コード抽出
			std::string::size_type beginPos = 0;
			while (true)
			{
				// 開始タグ検索
				beginPos = code.find( BeginGLSLTag, beginPos );
				if ( beginPos != std::string::npos ) 
				{
					// タグ名終端
					std::string::size_type tagEndBracket = code.find( ']', beginPos );
					if ( tagEndBracket != std::string::npos ) 
					{
						// タグ名
						lnRefString tagName( code.c_str(), beginPos + BeginGLSLTagLength, tagEndBracket - beginPos - BeginGLSLTagLength );

						// 終了タグ検索
						std::string::size_type endPos = code.find( EndGLSLTag, tagEndBracket );
						if ( endPos != std::string::npos ) {
							GLSLCode c;
							c.TagName = tagName;
							c.Code = lnRefString( code.c_str(), tagEndBracket + 1, endPos - tagEndBracket - 1 );
							mGLSLCodeArray.push_back( c );
						}
					}
					else {
						// TODO: タグがマッチしていない
					}
				}
				else {
					// 見つからなかった
					break;
				}
				beginPos++;
			}

			// Info
			beginPos = code.find( BeginInfoTag, 0 );
			if ( beginPos != std::string::npos ) {
				std::string::size_type endPos = code.find( EndInfoTag, beginPos );
				if ( endPos != std::string::npos ) {
					mInfoCode = lnString( 
						code, 
						beginPos + strlen(BeginInfoTag), 
						endPos - beginPos - strlen(BeginInfoTag) );
				}
			}
		}
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNFXManager::buildShaderClasses( IGLShaderBuilder* builder )
	{
		mBuilder = builder;
		if ( !mInfoCode.empty() ) {
			_analyzeXML( mInfoCode.c_str(), mInfoCode.size() );
		}
	}



	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNFXManager::GLSLCode* LNFXManager::_findShaderCode( const char* programName )
	{
		ln_foreach( GLSLCode& code, mGLSLCodeArray )
		{
			if ( code.TagName == programName ) {
				return &code;
			}
		}
		return NULL;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNFXManager::_analyzeXML( const char* xml, size_t size )
	{
		XML::Document xmlDoc;
		xmlDoc.Parse( xml, size );

		// まずは Technique → Pass と作成して、コンパイルする。この中で Variable も作る
		XML::Element* xmlElement = xmlDoc.FirstChildElement();
		while ( xmlElement != NULL )
		{
			// <Technique>
			if ( strcmp( xmlElement->Name(), TechniqueElementName ) == 0 ) {
				_analyzeXMLTechniqueElement( xmlElement );
			}

			xmlElement = xmlElement->NextSiblingElement();
		}

		// 次に、作成済みの Variable にアノテーションを割り振る
		xmlElement = xmlDoc.FirstChildElement();
		while ( xmlElement != NULL )
		{
			// <Variable>
			if ( strcmp( xmlElement->Name(), VariableElementName ) == 0 ) {
				_analyzeXMLVariableElement( xmlElement );
			}

			xmlElement = xmlElement->NextSiblingElement();
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNFXManager::_analyzeXMLVariableElement( XML::Element* xmlElement )
	{
		// 属性
		const char* attrName		= xmlElement->Attribute( NameAttributeName );
		const char* attrSemantic	= xmlElement->Attribute( SemanticAttributeName );
		const bool attrShared		= xmlElement->BoolAttribute( SharedAttributeName );
		const char* attrTexture		= xmlElement->Attribute(TextureAttributeName);
		LN_THROW_InvalidFormat(attrName);

		// 適用試行
		IAnnotatedObject* var = mBuilder->OnTrySetVariableParam( attrName, attrSemantic, attrShared, attrTexture );
		if ( var != NULL )
		{
			// 子要素
			XML::Element* xmlChild = xmlElement->FirstChildElement();
			while ( xmlChild != NULL )
			{
				// <Annotation>
				if ( strcmp( xmlChild->Name(), AnnotationElementName ) == 0 ) {
					_analyzeXMLAnnotationElement( xmlChild, var );
				}

				xmlChild = xmlChild->NextSiblingElement();
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNFXManager::_analyzeXMLAnnotationElement( XML::Element* xmlElement, IAnnotatedObject* obj )
	{
		// 属性
		const char* attrName	= xmlElement->Attribute( NameAttributeName );
		const char* attrValue	= xmlElement->Attribute( ValueAttributeName );
		LN_THROW_InvalidFormat(attrName);

		obj->addAnnotation( attrName, attrValue );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNFXManager::_analyzeXMLTechniqueElement( XML::Element* xmlElement )
	{
		// 属性
		const char* attrName = xmlElement->Attribute( NameAttributeName );
		LN_THROW_InvalidFormat(attrName);

		IAnnotatedObject* tech = mBuilder->OnCreateTechnique( attrName );

		// 子要素
		XML::Element* xmlChild = xmlElement->FirstChildElement();
		while ( xmlChild != NULL )
		{
			// <Pass>
			if ( strcmp( xmlChild->Name(), PassElementName ) == 0 ) {
				_analyzeXMLPassElement( xmlChild, tech );
			}
			// <Annotation>
			else if ( strcmp( xmlChild->Name(), AnnotationElementName ) == 0 ) {
				_analyzeXMLAnnotationElement( xmlChild, tech );
			}

			xmlChild = xmlChild->NextSiblingElement();
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNFXManager::_analyzeXMLPassElement( XML::Element* xmlElement, IAnnotatedObject* parentTech )
	{
		// 属性
		const char* attrName = xmlElement->Attribute(NameAttributeName);
		LN_THROW_InvalidFormat(attrName);

		IAnnotatedObject* pass = mBuilder->OnCreatePass( parentTech, attrName );

		// 子要素
		XML::Element* xmlChild = xmlElement->FirstChildElement();
		while ( xmlChild != NULL )
		{
			// <VertexShader>
			if ( strcmp( xmlChild->Name(), VertexShaderElementName ) == 0 ) {
				attrName = xmlChild->Attribute( EntryPointAttributeName );
				GLSLCode* code = _findShaderCode(attrName);
				LN_THROW_InvalidFormat(code);
				mBuilder->OnCreateVertexShader( pass, code->Code.c_str(), code->Code.size() );
			}
			// <PixelShader>
			else if ( strcmp( xmlChild->Name(), PixelShaderElementName ) == 0 ) {
				attrName = xmlChild->Attribute( EntryPointAttributeName );
				GLSLCode* code = _findShaderCode(attrName);
				LN_THROW_InvalidFormat(code);
				mBuilder->OnCreatePixelShader( pass, code->Code.c_str(), code->Code.size() );
			}
			// <RenderState>
			else if ( strcmp( xmlChild->Name(), RenderStateElementName ) == 0 ) {
				attrName = xmlChild->Attribute(NameAttributeName);
				LN_THROW_InvalidFormat(attrName);
				const char* value = xmlChild->Attribute( ValueAttributeName );
				mBuilder->OnSetRenderState( pass, attrName, value );
			}
			// <Annotation>
			else if ( strcmp( xmlChild->Name(), AnnotationElementName ) == 0 ) {
				_analyzeXMLAnnotationElement( xmlChild, pass );
			}

			xmlChild = xmlChild->NextSiblingElement();
		}

		// link
		mBuilder->OnEndPass( pass );
	}


	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool LNFXManager::openOutFile( const lnChar* filePath )
	{
		LN_ERR2_ASSERT( mOutStream == NULL );

		mOutFilePath = filePath;
		mOutStream = fopen( filePath, "w" );
		if ( !mOutStream ) return false;

		fprintf( mOutStream, "/*%s%d*/\n", Magic, 1 );

		return false;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNFXManager::closeOutFile()
	{
		if ( mOutStream ) {
			fclose( mOutStream );
			mOutStream = NULL;
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//void LNFXManager::writeIndexXML( const lnChar* xmlFilePath )
	//{
	//	FILE* fp = fopen( xmlFilePath, "rb" );
	//	LN_THROW_FileNotFound( fp, xmlFilePath );

	//	lnU32 size = Core::FileIO::File::getFileSize( fp );
	//	fseek( fp, 0, SEEK_SET );

	//	lnByte* buf = new lnByte[size];
	//	fread( buf, 1, size, fp );

	//	fwrite( &size, 1, 4, mOutStream );
	//	fwrite( buf, 1, size, mOutStream );

	//	delete[] buf;
	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNFXManager::writeHLSL( const std::string& code )
	{
		fprintf( mOutStream, "%s\n", BeginHLSLTag );
		fprintf( mOutStream, "%s\n", code.c_str() );
		fprintf( mOutStream, "%s\n", EndHLSLTag );
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================