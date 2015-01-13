//==============================================================================
// GLShader 
//==============================================================================

#include "stdafx.h"

#ifdef LNOTE_FOR_200
#include <algorithm>
#include <tinyxml2/tinyxml2.h>
#include "../../../FileIO/Manager.h"
#include "GLUtil.h"
#include "GLGraphicsDevice.h"
#include "GLShader.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// GLAnnotatedObjectImpl
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	GLAnnotatedObjectImpl::~GLAnnotatedObjectImpl()
	{
		ln_foreach( GLShaderAnnotation* anno, mShaderAnnotationArray ) {
			anno->release();
		}
		mShaderAnnotationArray.clear();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLAnnotatedObjectImpl::addAnnotation( const char* name, const char* value )
	{
		GLShaderAnnotation* anno = LN_NEW GLShaderAnnotation();
		mShaderAnnotationArray.push_back( anno );
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IShaderVariable* GLAnnotatedObjectImpl::getAnnotationByIndex( lnU32 index )
	{
		return mShaderAnnotationArray[index];
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IShaderVariable* GLAnnotatedObjectImpl::getAnnotationByName( const char* name )
	{
		ln_foreach( IShaderVariable* v, mShaderAnnotationArray ) {
			if ( _tcscmp( v->getName(), name ) == 0 ) {
				return v;
			}
		}
		return NULL;
	}

//==============================================================================
// ■ GLShader
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    GLShader::GLShader()
		: mGraphicsDevice	( NULL )
        , mCompileResult    ( LN_SHADERRESULT_ERROR )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    GLShader::~GLShader()
    {
		_dispose();
		LN_SAFE_RELEASE( mGraphicsDevice );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLShader::create( GLGraphicsDevice* device, FileIO::Stream* stream )
	{
		LN_REFOBJ_SET( mGraphicsDevice, device );
		//mFilePath = filePath;
		//_create();


		//LRefPtr<FileIO::InFile> file(
		//	mGraphicsDevice->getIOManager()->createInFile( mFilePath.c_str(), FileIO::FileOpenMode_Binary ) );

		//LNFXFile lnglFile;
		//lnglFile.create( stream, this );

		
		mLNFXManager.open( stream, LN_GRAPHICSAPI_OPENGL, true, NULL );
		mLNFXManager.buildShaderClasses( this );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLShader::addLeveledLogs( LNShaderCompileResult result, const lnChar* log )
	{
		// 一度エラーが設定されたら低いレベルには戻らない
		if ( result > mCompileResult ) {
			mCompileResult = result;
		}

		mCompileErrors += log;
		mCompileErrors += "\n";
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IShaderVariable* GLShader::getVariableByIndex( lnU32 index )
	{
		return mShaderVariableArray[index];
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IShaderVariable* GLShader::getVariableByName( const char* name )
	{
		ln_foreach( GLShaderVariable* v, mShaderVariableArray ) {
			if ( _tcscmp( v->getName(), name ) == 0 ) {
				return v;
			}
		}
		return NULL;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IShaderTechnique* GLShader::getTechniqueByIndex( lnU32 index )
	{
		return mShaderTechniqueArray[index];
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IShaderTechnique* GLShader::getTechniqueByName( const char* name )
	{
		ln_foreach( GLShaderTechnique* tech, mShaderTechniqueArray ) {
			if ( _tcscmp( tech->getName(), name ) == 0 ) {
				return tech;
			}
		}
		return NULL;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLShader::onPauseDevice()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLShader::onResumeDevice()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLShader::_create()
	{
		
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLShader::_dispose()
	{
		// Variable
		ln_foreach( GLShaderVariable* v, mShaderVariableArray ) {
			v->release();
		}
		mShaderVariableArray.clear();
		
		// Technique
		ln_foreach( GLShaderTechnique* tech, mShaderTechniqueArray ) {
			tech->release();
		}
		mShaderTechniqueArray.clear();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IAnnotatedObject* GLShader::OnCreateTechnique( const char* name )
	{
		GLShaderTechnique* tech = LN_NEW GLShaderTechnique();
		mShaderTechniqueArray.push_back( tech );
		return tech;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IAnnotatedObject* GLShader::OnCreatePass( IAnnotatedObject* parentTech, const char* name )
	{
		GLShaderPass* pass = LN_NEW GLShaderPass();
		pass->create( this );
		static_cast<GLShaderTechnique*>( parentTech )->addPass( pass );
		return pass;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLShader::OnCreateVertexShader( IAnnotatedObject* parentPass, const char* code, size_t size )
	{
		static_cast<GLShaderPass*>( parentPass )->createVertexShader( code, size );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLShader::OnCreatePixelShader( IAnnotatedObject* parentPass, const char* code, size_t size )
	{
		static_cast<GLShaderPass*>( parentPass )->createPixelShader( code, size );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLShader::OnSetRenderState( IAnnotatedObject* parentPass, const char* name, const char* value )
	{
		LN_THROW_NotImpl(0);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLShader::OnEndPass( IAnnotatedObject* pass )
	{
		static_cast<GLShaderPass*>( pass )->initialLinkShader();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IAnnotatedObject* GLShader::OnTrySetVariableParam( const char* targetName, const char* semantic, bool shared, const char* attachedTextureVarName )
	{
		LN_THROW_NotImpl(0);
	}

//==============================================================================
// ■ GLShaderVariable
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    GLShaderVariable::GLShaderVariable()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    GLShaderVariable::~GLShaderVariable()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLShaderVariable::create( const char* name, const LNShaderVariableTypeDesc& desc, ShaderVariableType lnType )
	{
		ShaderVariableBase::initialize(
			lnType,
			name,
			lnRefString(),
			desc );
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IShaderVariable* GLShaderVariable::getAnnotationByIndex( lnU32 index )
	{
		return mShaderAnnotationArray[index];
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IShaderVariable* GLShaderVariable::getAnnotationByName( const char* name )
	{
		ln_foreach( IShaderVariable* v, mShaderAnnotationArray ) {
			if ( _tcscmp( v->getName(), name ) == 0 ) {
				return v;
			}
		}
		return NULL;
	}


//==============================================================================
// ■ GLShaderAnnotation
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    GLShaderAnnotation::GLShaderAnnotation()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    GLShaderAnnotation::~GLShaderAnnotation()
    {
    }

//==============================================================================
// ■ GLShaderTechnique
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    GLShaderTechnique::GLShaderTechnique()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    GLShaderTechnique::~GLShaderTechnique()
    {
		ln_foreach( GLShaderPass* pass, mShaderPassArray ) {
			pass->release();
		}
		mShaderPassArray.clear();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLShaderTechnique::addPass( GLShaderPass* pass )
	{
		mShaderPassArray.push_back( pass );
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IShaderPass* GLShaderTechnique::getPassByIndex( lnU32 index )
	{
		return mShaderPassArray[index];
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IShaderPass* GLShaderTechnique::getPassByName( const char* name )
	{
		ln_foreach( IShaderPass* pass, mShaderPassArray ) {
			if ( _tcscmp( pass->getName(), name ) == 0 ) {
				return pass;
			}
		}
		return NULL;
	}

//==============================================================================
// ■ GLShaderPass
//==============================================================================

	struct GLSL_LNAttrInfo
	{
		const char*	Name;
		int			Length;
	};

	static GLSL_LNAttrInfo gGLSL_LNAttributeName[LN_USAGE_MAX] =
    {
        //"",                 // LN_USAGE_UNKNOWN ,
        //"lnPosition",       // LN_VERTEXDECLUSAGE_POSITION,        ///< 頂点位置
        //"lnBlendWeight",    // LN_VERTEXDECLUSAGE_BLENDWEIGHT,     ///< ブレンドウェイト値
        //"lnBlendIndices",   // LN_VERTEXDECLUSAGE_BLENDINDICES,    ///< ブレンドインデックス値
        //"lnNormal",         // LN_VERTEXDECLUSAGE_NORMAL,          ///< 法線
        //"lnTexCoord0",      // LN_VERTEXDECLUSAGE_TEXCOORD0,       ///< UV
        //"lnTexCoord1",      // LN_VERTEXDECLUSAGE_TEXCOORD1,       ///< 追加UV1
        //"lnTexCoord2",      // LN_VERTEXDECLUSAGE_TEXCOORD2,       ///< 追加UV2
        //"lnTexCoord3",      // LN_VERTEXDECLUSAGE_TEXCOORD3,       ///< 追加UV3
        //"lnTexCoord4",      // LN_VERTEXDECLUSAGE_TEXCOORD4,       ///< 追加UV4
        //"lnTexCoord5",      // LN_VERTEXDECLUSAGE_TEXCOORD5,       ///< Sdef - C値
        //"lnTexCoord6",      // LN_VERTEXDECLUSAGE_TEXCOORD6,       ///< Sdef - R0値
        //"lnTexCoord7",      // LN_VERTEXDECLUSAGE_TEXCOORD7,       ///< Sdef - R1値
        //"lnTexCoord8",      // LN_VERTEXDECLUSAGE_TEXCOORD8,       ///< エッジウェイト
        //"lnPSize15",        // LN_VERTEXDECLUSAGE_PSIZE15,         ///< 頂点インデックス値
        //"lnColor",          // LN_VERTEXDECLUSAGE_COLOR,

		{ "",					0, },	// LN_USAGE_UNKNOWN,
		{ "ln_Position",		11, },	// LN_USAGE_POSITION,
		{ "ln_Normal",			9, },	// LN_USAGE_NORMAL,
		{ "ln_Color",			8, },	// LN_USAGE_COLOR,
		{ "ln_TexCoord",		11, },	// LN_USAGE_TEXCOORD,
		{ "ln_PSize",			8, },	// LN_USAGE_PSIZE,
		{ "ln_BlendIndices",	15, },	// LN_USAGE_BLENDINDICES,
		{ "ln_BlendWeight",		14, },	// LN_USAGE_BLENDWEIGHT,
    };

	

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    GLShaderPass::GLShaderPass()
		: mShader				( NULL )
		, mProgram				( NULL )
		, mVertexShaderCode		( NULL )
		, mVertexShaderCodeSize	( 0 )
		, mPixelShaderCode		( NULL )
		, mPixelShaderCodeSize	( 0 )
		, mNextTextureUnitIndex	( 0 )
		//, mVertexShader	( NULL ) 
		//, mPixelShader	( NULL )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    GLShaderPass::~GLShaderPass()
    {
        if ( mProgram ) {
			glDeleteProgram( mProgram );
            mProgram = NULL;
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLShaderPass::create( GLShader* shader )
	{
		mShader = shader;
		// プログラムオブジェクトの作成 (これは LN_CHECK_GLERROR() は不定の値を返すらしい)
		mProgram = glCreateProgram();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLShaderPass::createVertexShader( const char* code, size_t size )
	{
		mVertexShaderCode = code;
		mVertexShaderCodeSize = size;
		//LN_ERR2_ASSERT( mVertexShader == NULL );
		//mVertexShader = glCreateShader( GL_VERTEX_SHADER );// エラーは NULL で LN_CHECK_GLERROR();
		//_compileShader( mVertexShader, (const char*)code, size );
		//glAttachShader( mProgram, mVertexShader );// LN_CHECK_GLERROR();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLShaderPass::createPixelShader( const char* code, size_t size )
	{
		mPixelShaderCode = code;
		mPixelShaderCodeSize = size;
		//LN_ERR2_ASSERT( mPixelShader == NULL );
		//mPixelShader = glCreateShader( GL_FRAGMENT_SHADER );// エラーは NULL で  LN_CHECK_GLERROR();
		//_compileShader( mPixelShader, (const char*)code, size );
		//glAttachShader( mProgram, mPixelShader );// LN_CHECK_GLERROR();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLShaderPass::initialLinkShader()
	{
		// リンク
		_linkShader();

        // すべての変数を列挙 (uniform 変数の数を調べて、その数分 ShaderVariable 作成)
        GLint params;
        glGetProgramiv( mProgram, GL_ACTIVE_UNIFORMS, &params );	LN_CHECK_GLERROR();
        for ( int i = 0; i < params; ++i )
        {
			_tryCreateVariable( i );
        }

		// attribute と Usage の対応テーブル作成
		_lookupAttribLocation();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLShaderPass::begin()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLShaderPass::end()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLShaderPass::commit()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLShaderPass::_linkShader()
	{
		// 頂点シェーダコンパイル
		GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
		_compileShader( vertexShader, mVertexShaderCode, mVertexShaderCodeSize );
		glAttachShader( mProgram, vertexShader );

		// フラグメントシェーダコンパイル
		GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
		_compileShader( fragmentShader, mPixelShaderCode, mPixelShaderCodeSize );
		glAttachShader( mProgram, fragmentShader );

		// リンク
		GLint linked;
        glLinkProgram( mProgram );// LN_CHECK_GLERROR();
		glGetProgramiv( mProgram, GL_LINK_STATUS, &linked );// LN_CHECK_GLERROR();

		// ログがあるかチェック
		//LNShaderCompileResult result;
        int log_size;
	    int length;
	    glGetProgramiv( mProgram, GL_INFO_LOG_LENGTH, &log_size );// LN_CHECK_GLERROR();
	    if ( log_size > 1 )
	    {
            char* buf = LN_NEW char[log_size];
            glGetProgramInfoLog( mProgram, log_size, &length, buf );// LN_CHECK_GLERROR();

            mShader->addLeveledLogs(
				(linked != GL_FALSE) ? LN_SHADERRESULT_WARNING : LN_SHADERRESULT_ERROR,
				buf );

            SAFE_DELETE_ARRAY( buf );

	    }

		// もう必要ない
		if ( vertexShader ) {
			glDetachShader( mProgram, vertexShader );
			glDeleteShader( vertexShader );
        }
		if ( fragmentShader ) {
			glDetachShader( mProgram, fragmentShader );
			glDeleteShader( fragmentShader );
        }
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void GLShaderPass::_compileShader( GLuint shader, const GLchar* code, GLint size )
    {
		//int lene = strlen(code);
		//code = "void main() { }";
        // シェーダオブジェクトにソースプログラムをセット
        glShaderSource( shader, 1, (const GLchar **)&code, NULL );// LN_CHECK_GLERROR();
      
        // シェーダのコンパイル
        GLint compiled;
        glCompileShader( shader );// LN_CHECK_GLERROR();
        glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );// LN_CHECK_GLERROR();
        
        // ログがあるかチェック (ログの長さは、最後のNULL文字も含む)
        int log_size;
        int length;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH , &log_size );// LN_CHECK_GLERROR();
        if ( log_size > 1 )
        {
            char* buf = LN_NEW char[log_size];
            glGetShaderInfoLog( shader, log_size, &length, buf );// LN_CHECK_GLERROR();

			mShader->addLeveledLogs(
				(compiled != GL_FALSE) ? LN_SHADERRESULT_WARNING : LN_SHADERRESULT_ERROR,
				buf );

            SAFE_DELETE_ARRAY( buf );
        }
    }

	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
	void GLShaderPass::_tryCreateVariable( int varIndex )
    {
		VariableInfo info;
		info.VarIndex = varIndex;

        GLsizei name_len = 0;
        GLsizei var_size = 0;   // 配列サイズっぽい
        GLenum  var_type = 0;
        GLchar  name[256] = { 0 };
        glGetActiveUniform( mProgram, varIndex, 256, &name_len, &var_size, &var_type, name ); LN_CHECK_GLERROR();

		info.ShaderVariable = static_cast<GLShaderVariable*>( mShader->getVariableByName( name ) );
		if ( info.ShaderVariable == NULL )
		{
			// 見つからなかったので新規登録
			LNShaderVariableTypeDesc desc;
			ShaderVariableType type;
			Utils::convertShaderVariableTypeGLToLN( var_type, var_size, &desc, &type );

			info.ShaderVariable = LN_NEW GLShaderVariable();
			info.ShaderVariable->create( name, desc, type );
			mShader->addShaderVariable( info.ShaderVariable );
		}

		// サンプラの場合は割り当てるべきテクスチャユニット番号を取得する
		if ( info.ShaderVariable->getType() == SVT_TEXTURE )
		{
			info.TextureUnitIndex = mNextTextureUnitIndex;
			++mNextTextureUnitIndex;
		}

		info.Location = glGetUniformLocation( mProgram, name ); LN_CHECK_GLERROR();

		mVariableInfoArray.push_back( info );
	}

	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
	void GLShaderPass::_lookupAttribLocation()
	{
		// 初期化
        for ( int i = 0; i < LN_USAGE_MAX; ++i ) {
			for ( int j = 0; j < LNUsageIndexMax; ++j ) {
				mAttribLocationTable[i][j] = -1;
			}
        }

		// 各 attribute 変数が、どの Usage に対応するかのテーブルを作る
		GLint params;
        glGetProgramiv( mProgram, GL_ACTIVE_ATTRIBUTES, &params ); LN_CHECK_GLERROR();
		for ( int i = 0; i < params; ++i )
        {
            GLsizei name_len = 0;
            GLsizei var_size = 0;
            GLenum  var_type = 0;
            GLchar  name[256] = { 0 };
			glGetActiveAttrib( mProgram, i, 256, &name_len, &var_size, &var_type, name );	LN_CHECK_GLERROR();

			// 全体長さと、インデックス部分以外の長さ
			int allLen = strlen( name );
			int len = allLen;
			for ( ; len > 0 && isdigit( name[len - 1] ); --len );	// ln_Color0 等の後ろの数字部分をカット

			// 総当たりで調査
            for ( int ln_attr_i = 0; ln_attr_i < LN_USAGE_MAX; ++ln_attr_i )
            {
				if ( len == gGLSL_LNAttributeName[ln_attr_i].Length
				&&	 Base::StringUtils::strcmpNI( name, gGLSL_LNAttributeName[ln_attr_i].Name, gGLSL_LNAttributeName[ln_attr_i].Length ) 
				){
					int udageIndex = 0;
					if ( len < allLen ) {
						udageIndex = atoi( &name[len] );
					}

					int loc = glGetAttribLocation( mProgram, name );	LN_CHECK_GLERROR();
					LN_ERR2_ASSERT_S( loc <= 255 );
					mAttribLocationTable[ln_attr_i][udageIndex] = loc;
                    //printf( "%s: idx=%d loc:%d\n", name, i, loc );
                    break;
                }
            }
        }
	}

#if 0
//==============================================================================
// ■ LNFXFile
//==============================================================================

	const lnChar* LNFXFile::Magic = _T("lnfx");

	const lnChar* LNFXFile::VariableElementName = _T("Variable");
	const lnChar* LNFXFile::AnnotationElementName = _T("Annotation");
	const lnChar* LNFXFile::TechniqueElementName = _T("Technique");
	const lnChar* LNFXFile::PassElementName = _T("Pass");
	const lnChar* LNFXFile::VertexShaderElementName = _T("VertexShader");
	const lnChar* LNFXFile::PixelShaderElementName = _T("PixelShader");
	const lnChar* LNFXFile::RenderStateElementName = _T("RenderState");

	const lnChar* LNFXFile::NameAttributeName = _T("Name");
	const lnChar* LNFXFile::SemanticAttributeName = _T("Semantic");
	const lnChar* LNFXFile::SharedAttributeName = _T("Shared");
	const lnChar* LNFXFile::TextureAttributeName = _T("Texture");
	const lnChar* LNFXFile::ValueAttributeName = _T("Value");
	const lnChar* LNFXFile::EntryPointAttributeName = _T("Shader");

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNFXFile::~LNFXFile()
	{
		ln_foreach( ShaderProgram* sp , mShaderProgramArray )
		{
			SAFE_DELETE( sp );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNFXFile::create( FileIO::IInStream* stream, GLShader* shader )
	{
		mStream = stream;
		mShader = shader;
		FileIO::BinaryReader reader( stream );

		// マジックナンバー
		lnByte magic[4] = { 0 };
		reader.read( magic, 4 );
		if ( memcmp( magic, Magic, 4 ) != 0 ) {
			LN_THROW_FileFormat( 0 );
		}

		// ファイルバージョン
		lnU8 version = reader.readUInt8();

		// 先頭のHLSLは飛ばす
		lnU32 size = reader.readUInt32();
		reader.seek( size );

		// XML
		lnU32 xmlSize = reader.readUInt32();
		mTempBuffer.request( xmlSize );
		reader.read( mTempBuffer.getBuffer(), xmlSize );

		// シェーダプログラム
		Base::TempBuffer buf;
		while ( stream->getPosition() < stream->getSize() )
		{
			ShaderProgram* sp = LN_NEW ShaderProgram;

			// 名前
			lnU32 size = reader.readUInt32();
			buf.request( size );
			reader.read( buf.getBuffer(), size );
			sp->Name = lnString( (const char*)buf.getBuffer(), size );

			// サイズ
			sp->Size = reader.readUInt32();

			// シーク位置
			sp->SeekPos = stream->getPosition();

			reader.seek( sp->Size );
			mShaderProgramArray.push_back( sp );
		}

		// 検索用ソート
		struct {
			bool operator()( const ShaderProgram* lhs, const ShaderProgram* rhs ) const {
				return lhs->Name < rhs->Name;
			}
		} cmp;
		std::sort( mShaderProgramArray.begin(), mShaderProgramArray.end(), cmp );


		// この中で stream のシーク位置を操作する。最後でなければならない。
		_analyzeXML( (const char*)mTempBuffer.getBuffer(), xmlSize );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNFXFile::ShaderProgram* LNFXFile::_findShaderProgram( const char* programName )
	{
		struct {
			bool operator()( const ShaderProgram* lhs, const std::string& rhs ) const {
				return lhs->Name < rhs;
			}
		} cmp;
		lnString name = programName;
		ShaderProgramArray::iterator itr = std::lower_bound( 
			mShaderProgramArray.begin(), mShaderProgramArray.end(), name, cmp );
		if ( itr != mShaderProgramArray.end() && (*itr)->Name == name ) {
			return *itr;
		}
		return NULL;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNFXFile::_analyzeXML( const char* xml, size_t size )
	{
		XML::Document xmlDoc;
		xmlDoc.Parse( xml, size );

		XML::Element* xmlElement = xmlDoc.FirstChildElement();
		while ( xmlElement != NULL )
		{
			// <Variable>
			if ( strcmp( xmlElement->Name(), VariableElementName ) == 0 ) {
				_analyzeVariableElement( xmlElement );
			}
			// <Technique>
			else if ( strcmp( xmlElement->Name(), TechniqueElementName ) == 0 ) {
				_analyzeTechniqueElement( xmlElement );
			}

			xmlElement = xmlElement->NextSiblingElement();
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNFXFile::_analyzeVariableElement( XML::Element* xmlElement )
	{
		// 属性
		const char* attrName		= xmlElement->Attribute( NameAttributeName );
		const char* attrSemantic	= xmlElement->Attribute( SemanticAttributeName );
		const bool attrShared		= xmlElement->BoolAttribute( SharedAttributeName );
		const char* attrTexture		= xmlElement->Attribute( TextureAttributeName );
		LN_THROW_FileFormat( attrName );

		Variable var;
		var.Name = attrName;
		var.Semantic = ( attrSemantic ) ? attrSemantic : "";
		var.Shared = attrShared;
		var.Texture = ( attrSemantic ) ? attrSemantic : "";

		// 子要素
		XML::Element* xmlChild = xmlElement->FirstChildElement();
		while ( xmlChild != NULL )
		{
			// <Annotation>
			if ( strcmp( xmlChild->Name(), AnnotationElementName ) == 0 ) {
				_analyzeAnnotationElement( xmlChild, &var.Annotations );
			}

			xmlChild = xmlChild->NextSiblingElement();
		}

		// 確定
		mVariableArray.push_back( var );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNFXFile::_analyzeAnnotationElement( XML::Element* xmlElement, AnnotationArray* annotations )
	{
		// 属性
		const char* attrName	= xmlElement->Attribute( NameAttributeName );
		const char* attrValue	= xmlElement->Attribute( ValueAttributeName );
		LN_THROW_FileFormat( attrName );

		Annotation anno;
		anno.Name	= attrName;
		anno.Value	= (attrValue) ? attrValue : "";

		// 確定
		annotations->push_back( anno );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNFXFile::_analyzeTechniqueElement( XML::Element* xmlElement )
	{
		// 属性
		const char* attrName = xmlElement->Attribute( NameAttributeName );
		LN_THROW_FileFormat( attrName );

		GLShaderTechnique* tech = NULL;

		// 子要素
		XML::Element* xmlChild = xmlElement->FirstChildElement();
		while ( xmlChild != NULL )
		{
			// <Pass>
			if ( strcmp( xmlChild->Name(), PassElementName ) == 0 ) {
				_analyzePassElement( xmlChild, tech );
			}

			xmlChild = xmlChild->NextSiblingElement();
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void LNFXFile::_analyzePassElement( XML::Element* xmlElement, GLShaderTechnique* parentTech )
	{
		// 属性
		const char* attrName = xmlElement->Attribute( NameAttributeName );
		LN_THROW_FileFormat( attrName );

		//GLShaderPass* pass = LN_;
		LRefPtr<GLShaderPass> pass( LN_NEW GLShaderPass() );
		pass->create( mShader );

		// 子要素
		XML::Element* xmlChild = xmlElement->FirstChildElement();
		while ( xmlChild != NULL )
		{
			// <VertexShader>
			if ( strcmp( xmlChild->Name(), VertexShaderElementName ) == 0 ) {
				attrName = xmlChild->Attribute( EntryPointAttributeName );
				ShaderProgram* sp = _findShaderProgram( attrName );
				LN_THROW_FileFormat( sp );

				mTempBuffer.request( sp->Size );
				mStream->seek( sp->SeekPos, SEEK_SET );
				mStream->read( mTempBuffer.getBuffer(), sp->Size );
				pass->createVertexShader( mTempBuffer.getBuffer(), sp->Size );
			}
			// <PixelShader>
			else if ( strcmp( xmlChild->Name(), PixelShaderElementName ) == 0 ) {
				attrName = xmlChild->Attribute( EntryPointAttributeName );
				ShaderProgram* sp = _findShaderProgram( attrName );
				LN_THROW_FileFormat( sp );

				mTempBuffer.request( sp->Size );
				mStream->seek( sp->SeekPos, SEEK_SET );
				mStream->read( mTempBuffer.getBuffer(), sp->Size );
				pass->createPixelShader( mTempBuffer.getBuffer(), sp->Size );
			}

			xmlChild = xmlChild->NextSiblingElement();
		}

		pass->linkShader();
		// TODO:ADD

	}
	
#endif
} // namespace Graphics
} // namespace Core
} // namespace LNote
#else

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../../Task/Manager.h"
#include "../../../FileIO/Manager.h"
#include "../../../FileIO/InFile.h"
#include "../../../Math/Vector4.h"
#include "../../../Math/Matrix.h"
#include "../../Common/GraphicsUtil.h"
#include "../../Manager.h"
#include "LNGL/LNGL.h"
#include "GLGraphicsDevice.h"
#include "GLTexture.h"
#include "GLShader.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
namespace OpenGL
{

//==============================================================================
// ■ Shader クラス
//==============================================================================

	//----------------------------------------------------------------------
	// ● コンストラクタ
	//----------------------------------------------------------------------
    Shader::Shader( ShaderManager* manager_ )
        : mManager          ( manager_ )
        , mVertexShader     ( NULL )
        , mFragmentShader   ( NULL )
        , mProgram          ( NULL )
        , mCompileResult    ( LN_SHADERRESULT_ERROR )
        , mShaderTechnique  ( NULL )
        , mLastTextureUnit  ( 0 )
    {
        LN_SAFE_ADDREF( mManager );
    }

	//----------------------------------------------------------------------
	// ● デストラクタ
	//----------------------------------------------------------------------
    Shader::~Shader()
    {
        if (mManager)
        {
            mManager->removeShader( this );
        }

        onDisposeGLResource();

        // ShaderTechnique 解放
        LN_SAFE_RELEASE( mShaderTechnique );

        // ShaderVariable 解放
        for ( lnU32 i = 0; i < mShaderVariableArray.size(); ++i )
        {
            SAFE_DELETE( mShaderVariableArray[ i ] );
        }
        mShaderVariableArray.clear();


        LN_SAFE_RELEASE( mManager );
    }

	//----------------------------------------------------------------------
	// ● 初期化
	//----------------------------------------------------------------------
    void Shader::initialize( const lnChar* filename_ )
    {
        mFilename = filename_;

        _createOnInitial();

        // スクリプト実行
        if ( !mScript.empty() )
        {
            mManager->doScript( mScript.c_str(), this );
        }
    }

	//----------------------------------------------------------------------
	// ● ブール値の設定
	//----------------------------------------------------------------------
	LNRESULT Shader::setBool( const lnChar* name_, bool b_ )
    {
        return getVariableByName( name_ )->setBool( b_ );
    }

	//----------------------------------------------------------------------
	// ● 整数値の設定
	//----------------------------------------------------------------------
	LNRESULT Shader::setInt( const lnChar* name_, int value_ )
    {
        return getVariableByName( name_ )->setInt( value_ );
    }

	//----------------------------------------------------------------------
	// ● 実数値の設定
	//----------------------------------------------------------------------
	LNRESULT Shader::setFloat( const lnChar* name_, lnFloat value_ )
    {
        return getVariableByName( name_ )->setFloat( value_ );
    }

	//----------------------------------------------------------------------
	// ● ベクトルの設定
	//----------------------------------------------------------------------
	LNRESULT Shader::setVector( const lnChar* name_, const LVector4& vec_ )
    { 
        return getVariableByName( name_ )->setVector( vec_ );
    }

	//----------------------------------------------------------------------
	// ● ベクトル配列の設定
	//----------------------------------------------------------------------
	LNRESULT Shader::setVectorArray( const lnChar* name_, const LVector4* vec_, lnU32 count_ )
    {
        return getVariableByName( name_ )->setVectorArray( vec_, count_ );
    }

	//----------------------------------------------------------------------
	// ● 行列の設定
	//----------------------------------------------------------------------
	LNRESULT Shader::setMatrix( const lnChar* name_, const LMatrix& matrix_ )
    {
        return getVariableByName( name_ )->setMatrix( matrix_ );
    }

	//----------------------------------------------------------------------
	// ● 行列の配列の設定
	//----------------------------------------------------------------------
	LNRESULT Shader::setMatrixArray( const lnChar* name_, const LMatrix* matrices_, lnU32 count_ )
    {
        return getVariableByName( name_ )->setMatrixArray( matrices_, count_ );
    }
   
	//----------------------------------------------------------------------
	// ● テクスチャの設定
	//----------------------------------------------------------------------
	LNRESULT Shader::setTexture( const lnChar* name_, ITexture* texture_ )
    {
        return getVariableByName( name_ )->setTexture( texture_ );
    }

	//----------------------------------------------------------------------
	// ● 名前によって変数を取得する
	//----------------------------------------------------------------------
    IShaderVariable* Shader::getVariableByName( const lnChar* name_ )
    {
        ShaderVariableMap::iterator it = mShaderVariableMap.find( name_ );
        if ( it != mShaderVariableMap.end() )
        {
            return it->second;
        }
        return NULL;
    }

	//----------------------------------------------------------------------
	// ● インデックスによって変数を取得する
	//----------------------------------------------------------------------
    IShaderVariable* Shader::getVariableByIndex( lnU32 index_ )
    {
        if ( index_ >= mShaderVariableArray.size() ) { return NULL; }
        return mShaderVariableArray[ index_ ];
    }

	//// セマンティクスによって変数を取得する
	//IShaderVariable* Shader::getVariableBySemantic( const char* name_ )
	//{
	//    return NULL;
	//}
    
	//----------------------------------------------------------------------
	// ● 名前によってテクニックを取得する
	//----------------------------------------------------------------------
    IShaderTechnique* Shader::getTechniqueByName( const lnChar* name_ )
    {
        return mShaderTechnique;
    }

	//----------------------------------------------------------------------
	// ● インデックスによってテクニックを取得する
	//----------------------------------------------------------------------
    IShaderTechnique* Shader::getTechniqueByIndex( lnU32 index_ )
    {
        if ( index_ == 0 )
        {
            return mShaderTechnique;
        }
        return NULL;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Shader::onCommit()
    {
        lnU32 n = mShaderVariableArray.size();
        for ( lnU32 i = 0; i < n; ++i )
        {
            mShaderVariableArray[ i ]->onCommit();
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Shader::onCommitGLResource()
    {
        if (!mProgram)
        {
            _createOnRefresh();
        }

        // onCommit() は別途 begin() 等から呼ばれるので、ここでは必要ないかも
        //onCommit()
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Shader::onDisposeGLResource()
    {
        // 子要素
        ln_foreach(ShaderVariable* v, mShaderVariableArray)
        {
            v->onDisposeGLResource();
        }
        if (mShaderTechnique)
        {
            mShaderTechnique->onDisposeGLResource();
        }

        // シェーダオブジェクトの削除
        //if ( mVertexShader )
        //{
        //    LNGL::glDeleteShader( mVertexShader );
        //    //glGetError();
        //    mVertexShader = NULL;
        //}
        //if ( mFragmentShader )
        //{
        //    LNGL::glDeleteShader( mFragmentShader );
        //    //glGetError();
        //    mFragmentShader = NULL;
        //}

        // プログラムの削除
        if ( mProgram )
        {
            LNGL::glDeleteProgram( mProgram );
            //glGetError();
            mProgram = NULL;
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Shader::_createOnInitial()
    {
        //---------------------------------------------------------
        // シェーダプログラムのビルド
        _buildFromFile();

        //---------------------------------------------------------
        // すべての変数を列挙

        // uniform 変数の数を調べて、その数分 ShaderVariable 作成
        GLint params;
        LNGL::glGetProgramiv( mProgram, GL_ACTIVE_UNIFORMS, &params );
        //LN_CHECK_GLERROR();

        for ( int i = 0; i < params; ++i )
        {
            ShaderVariable* v = LN_NEW ShaderVariable( this, i );
            v->onCreateGLResource();
            mShaderVariableMap.insert( ShaderVariablePair( v->getName(), v ) );
            mShaderVariableArray.push_back( v );
        }

        //---------------------------------------------------------
        // シェーダ情報

        mDesc.ParameterNum = params;
        mDesc.TechniqueNum = 1;

        //---------------------------------------------------------
        // テクニック作成

        mShaderTechnique = LN_NEW ShaderTechnique( this );


        //---------------------------------------------------------
        // GLリソース確保
        //ln_foreach(ShaderVariable* v, mShaderVariableArray)
        //{
        //    
        //}
        mShaderTechnique->onCreateGLResource();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Shader::_createOnRefresh()
    {
        //---------------------------------------------------------
        // シェーダプログラムのビルド
        _buildFromFile();

        //---------------------------------------------------------
        // GLリソース確保
        ln_foreach(ShaderVariable* v, mShaderVariableArray)
        {
            v->onCreateGLResource();
        }
        mShaderTechnique->onCreateGLResource();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Shader::_buildFromFile()
    {
        //FileIO::InFile* file = NULL;
        //mManager->getFileManager()->createInFile( &file, mFilename.c_str() );
        //Base::FinallyReleasePtr< FileIO::InFile > file_ptr( file );
		LRefPtr<FileIO::InFile> file(
			mManager->getFileManager()->createInFile( mFilename.c_str() ) );

        // mTempBuffer に全部読み込む
        lnU32 size = file->getSize();
        char* buf = (char*)mManager->getTempBuffer().lock( size + 1 );
        file->read( buf, size );
        buf[ size ] = '\0';

        _build( buf );

        // TODO:lockミス。
        mManager->getTempBuffer().unlock();

        if ( !mCompileErrors.empty() )
        {
            ln_std_tstring str = mFilename.c_str();
            str += _T("\n");
            str += mCompileErrors.c_str();
            //LN_ERRLOG_WRITE( str.c_str() );
            lnprintf( "%s\n", mFilename.c_str() );
            lnprintf( mCompileErrors.c_str() );
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Shader::_build( const char* code_ )
    {
        mCompileResult = LN_SHADERRESULT_ERROR; // エラーにしておく
        mCompileErrors.clear();

        //---------------------------------------------------------
        // コード分割
#if 0
#define VERTEX_SHADER_TAG       "VERTEX_SHADER:"
#define VERTEX_SHADER_TAG_LEN   ( 14 )
#define FRAGMENT_SHADER_TAG     "FRAGMENT_SHADER:"
#define FRAGMENT_SHADER_TAG_LEN ( 16 )
#define METADATA_TAG            "METADATA:"
#define METADATA_TAG_LEN        ( 9 )

        std::string str( code_ );
        
        // "VERTEX_SHADER:" を探す
	    std::string::size_type vs_index = str.find( VERTEX_SHADER_TAG );
        if ( vs_index == std::string::npos )
        {
            mCompileErrors = "not found "VERTEX_SHADER_TAG;
            return;
        }
	    
        // "FRAGMENT_SHADER:" を探す
	    std::string::size_type fs_index = str.find( FRAGMENT_SHADER_TAG );
        if ( fs_index == std::string::npos )
        {
            mCompileErrors = "not found "FRAGMENT_SHADER_TAG;
            return;
        }
	    
        // 頂点シェーダコード
        std::string vs_code( str, vs_index + VERTEX_SHADER_TAG_LEN, fs_index - vs_index - VERTEX_SHADER_TAG_LEN );
        
        // "METADATA:" を探す
	    std::string::size_type mt_index = str.find( METADATA_TAG );
        if ( fs_index == std::string::npos )
        {
            mCompileErrors = "not found "METADATA_TAG;
            return;
        }

        // フラグメントシェーダコード
        std::string fs_code( str, fs_index + FRAGMENT_SHADER_TAG_LEN, mt_index - fs_index - FRAGMENT_SHADER_TAG_LEN );

        // メタデータコード
        mScript = std::string( str.c_str() + (mt_index + METADATA_TAG_LEN) );
#endif

        // ソース分割
        LNFXShaderSource source;
        source.initialize(code_, LNFXShaderSource::SHADER_GLSL);

        // メタデータコード
        mScript = source.getGLSLMetaDataSource();





        // 頂点シェーダ
        const char* vs_codes[] = 
        {
            "#define LN_GLSL_VERT 1\n",
            source.getSourceData()
        };
        int vs_sizes[] =
        {
            strlen(vs_codes[0]),
            source.getSourceDataSize(),
        };
        //if ( !_compileShader( mVertexShader, 2, vs_codes, vs_sizes ) ) { return; }

        // フラグメントシェーダ
        const char* fs_codes[] = 
        {
            "#define LN_GLSL_FLAG 1\n",
            source.getSourceData()
        };
        int fs_sizes[] =
        {
            strlen(fs_codes[0]),
            source.getSourceDataSize(),
        };

        LNGL::ShaderBuildArgs args;
        args.vs_codes = vs_codes;
        args.vs_sizes = vs_sizes;
        args.vs_count = 2;
        args.fs_codes = fs_codes;
        args.fs_sizes = fs_sizes;
        args.fs_count = 2;
        bool error = false;
        LNGL::lnBuildShader(&args, &mProgram, &mCompileResult, &mCompileErrors);

    }

    
//==============================================================================
// ■ ShaderVariable クラス
//==============================================================================


    void convertVariableTypeGLToLN( GLenum gl_type_, GLsizei gl_var_size_, LNShaderVariableTypeDesc* desc_ )
    {
        desc_->Elements = gl_var_size_;
        desc_->Shared = false;

#define SET_LNDESC( c_, t_, row_, col_ ) { desc_->Class = (c_); desc_->Type = (t_); desc_->Rows = (row_); desc_->Columns = (col_); }

        switch ( gl_type_ )
        {
            case GL_FLOAT:      SET_LNDESC( LN_SVC_SCALAR, LN_SVT_FLOAT, 1, 1 ); break;
            case GL_FLOAT_VEC2: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 2 ); break;
            case GL_FLOAT_VEC3: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 3 ); break;
            case GL_FLOAT_VEC4: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 4 ); break;

            case GL_INT:        SET_LNDESC( LN_SVC_SCALAR, LN_SVT_INT, 1, 1 ); break;
            case GL_INT_VEC2:   SET_LNDESC( LN_SVC_VECTOR, LN_SVT_INT, 1, 2 ); break;
            case GL_INT_VEC3:   SET_LNDESC( LN_SVC_VECTOR, LN_SVT_INT, 1, 3 ); break;
            case GL_INT_VEC4:   SET_LNDESC( LN_SVC_VECTOR, LN_SVT_INT, 1, 4 ); break;

            case GL_BOOL:        SET_LNDESC( LN_SVC_SCALAR, LN_SVT_BOOL, 1, 1 ); break;
            case GL_BOOL_VEC2:   SET_LNDESC( LN_SVC_VECTOR, LN_SVT_BOOL, 1, 2 ); break;
            case GL_BOOL_VEC3:   SET_LNDESC( LN_SVC_VECTOR, LN_SVT_BOOL, 1, 3 ); break;
            case GL_BOOL_VEC4:   SET_LNDESC( LN_SVC_VECTOR, LN_SVT_BOOL, 1, 4 ); break;

            case GL_FLOAT_MAT2:     SET_LNDESC( LN_SVC_MATRIX, LN_SVT_FLOAT, 2, 2 ); break;
            case GL_FLOAT_MAT3:     SET_LNDESC( LN_SVC_MATRIX, LN_SVT_FLOAT, 3, 3 ); break;
            case GL_FLOAT_MAT4:     SET_LNDESC( LN_SVC_MATRIX, LN_SVT_FLOAT, 4, 4 ); break;
            
            case GL_SAMPLER_2D:         SET_LNDESC( LN_SVC_SAMPLER, LN_SVT_SAMPLER2D, 1, 1 ); break;
            case GL_SAMPLER_CUBE:       SET_LNDESC( LN_SVC_SAMPLER, LN_SVT_SAMPLERCUBE, 1, 1 ); break;
            
#if !defined(LNOTE_GLES)
            case GL_FLOAT_MAT2x3:   SET_LNDESC( LN_SVC_MATRIX, LN_SVT_FLOAT, 2, 3 ); break;
            case GL_FLOAT_MAT2x4:   SET_LNDESC( LN_SVC_MATRIX, LN_SVT_FLOAT, 2, 4 ); break;
            case GL_FLOAT_MAT3x2:   SET_LNDESC( LN_SVC_MATRIX, LN_SVT_FLOAT, 3, 2 ); break;
            case GL_FLOAT_MAT3x4:   SET_LNDESC( LN_SVC_MATRIX, LN_SVT_FLOAT, 3, 4 ); break;
            case GL_FLOAT_MAT4x2:   SET_LNDESC( LN_SVC_MATRIX, LN_SVT_FLOAT, 4, 2 ); break;
            case GL_FLOAT_MAT4x3:   SET_LNDESC( LN_SVC_MATRIX, LN_SVT_FLOAT, 4, 3 ); break;

            //case GL_FLOAT:      SET_LNDESC( LN_SVC_SCALAR, LN_SVT_FLOAT, 1, 1 ); break;
            //case GL_FLOAT_VEC2: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 2 ); break;
            //case GL_FLOAT_VEC3: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 3 ); break;
            //case GL_FLOAT_VEC4: SET_LNDESC( LN_SVC_VECTOR, LN_SVT_FLOAT, 1, 4 ); break;

            case GL_SAMPLER_1D:         SET_LNDESC( LN_SVC_SAMPLER, LN_SVT_SAMPLER1D, 1, 1 ); break;
            case GL_SAMPLER_3D:         SET_LNDESC( LN_SVC_SAMPLER, LN_SVT_SAMPLER3D, 1, 1 ); break;
            
            case GL_SAMPLER_1D_SHADOW:  SET_LNDESC( LN_SVC_SAMPLER, LN_SVT_UNKNOWN, 1, 1 ); break;
            case GL_SAMPLER_2D_SHADOW:  SET_LNDESC( LN_SVC_SAMPLER, LN_SVT_UNKNOWN, 1, 1 ); break;
#endif
        }
    }


	//----------------------------------------------------------------------
	// ● コンストラクタ
	//----------------------------------------------------------------------
    ShaderVariable::ShaderVariable( Shader* shader_, int var_index_ )
        : mShader       ( shader_ )
        , mVarIndex     ( var_index_ )
        , mLocation     ( -1 )
        , mSamplerState ( NULL )
    {
        
    }

	//----------------------------------------------------------------------
	// ● デストラクタ
	//----------------------------------------------------------------------
    ShaderVariable::~ShaderVariable()
    {
        
        if ( mSamplerState )
        {
            LN_SAFE_RELEASE( mSamplerState->Texture );
            SAFE_DELETE( mSamplerState );
        }
        for ( lnU32 i = 0; i < mAnnotationArray.size(); ++i )
        {
            SAFE_DELETE( mAnnotationArray[ i ] );
        }
    }

	//----------------------------------------------------------------------
	// ● ブール値の取得
	//----------------------------------------------------------------------
	LNRESULT ShaderVariable::getBool( bool* b_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● ブール値の設定
	//----------------------------------------------------------------------
	LNRESULT ShaderVariable::setBool( bool b_ )
    {
        if (mVariableBuffer.Type == TYPE_BOOL)
        {
            mVariableBuffer.Bool = b_;
        }
        /*
        glUseProgram( mShader->getGLProgram() );
        LN_CHECK_GLERROR();

        glUniform1i( mLocation, b_ );
        LN_CHECK_GLERROR();
        */
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● 整数値の取得
	//----------------------------------------------------------------------
	LNRESULT ShaderVariable::getInt( int* value_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● 整数値の設定
	//----------------------------------------------------------------------
	LNRESULT ShaderVariable::setInt( int value_ )
    {
        if (mVariableBuffer.Type == TYPE_INT)
        {
            mVariableBuffer.Int = value_;
        }
        /*
        glUseProgram( mShader->getGLProgram() );
        LN_CHECK_GLERROR();

        glUniform1i( mLocation, value_ );
        LN_CHECK_GLERROR();
        */
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● 実数値の取得
	//----------------------------------------------------------------------
    LNRESULT ShaderVariable::getFloat( lnFloat* value_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● 実数値の設定
	//----------------------------------------------------------------------
    LNRESULT ShaderVariable::setFloat( lnFloat value_ )
    {
        if (mVariableBuffer.Type == TYPE_FLOAT)
        {
            mVariableBuffer.Float = value_;
        }
        /*
        glUseProgram( mShader->getGLProgram() );
        LN_CHECK_GLERROR();

        glUniform1f( mLocation, value_ );
        LN_CHECK_GLERROR();
        */
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● ベクトルの取得
	//----------------------------------------------------------------------
	LNRESULT ShaderVariable::getVector( LVector4* vec_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● ベクトルの設定
	//----------------------------------------------------------------------
	LNRESULT ShaderVariable::setVector( const LVector4& vec_ )
    {
        if (mVariableBuffer.Type == TYPE_VECTOR)
        {
            *mVariableBuffer.Vector = vec_;
        }

        /*
        glUseProgram( mShader->getGLProgram() );
        LN_CHECK_GLERROR();

        switch ( mVariableTypeDesc.Columns )
        {
            case 2:
                glUniform2f( mLocation, vec_.x, vec_.y );
                LN_CHECK_GLERROR();
                break;
            case 3:
                glUniform3f( mLocation, vec_.x, vec_.y, vec_.z );
                LN_CHECK_GLERROR();
                break;
            case 4:
                glUniform4f( mLocation, vec_.x, vec_.y, vec_.z, vec_.w );
                LN_CHECK_GLERROR();
                break;
        }
        */
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● ベクトル配列の取得
	//----------------------------------------------------------------------
	LNRESULT ShaderVariable::getVectorArray( LVector4* vec_, lnU32 count_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }
    
	//----------------------------------------------------------------------
	// ● ベクトル配列の設定
	//----------------------------------------------------------------------
	LNRESULT ShaderVariable::setVectorArray( const LVector4* vec_, lnU32 count_ )
    {
        if (mVariableBuffer.Type == TYPE_VECTORARRAY)
        {
            memcpy(
                mVariableBuffer.VectorArray.Vectors,
                vec_,
                sizeof(LVector4) * (LMath::min(mVariableBuffer.VectorArray.Count, count_)));
        }

        /*
        glUseProgram( mShader->getGLProgram() );
        LN_CHECK_GLERROR();

        glUniform4fv( mLocation, count_, (const GLfloat*)vec_ );
        LN_CHECK_GLERROR();
        */
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● 行列の取得
	//----------------------------------------------------------------------
    LNRESULT ShaderVariable::getMatrix( LMatrix* matrix_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● 行列の設定
	//----------------------------------------------------------------------
    LNRESULT ShaderVariable::setMatrix( const LMatrix& matrix_ )
    {
        if (mVariableBuffer.Type == TYPE_MATRIX)
        {
            LMatrix mat;
            LMatrix::transpose( &mat, matrix_ );
            *mVariableBuffer.Matrix = mat;
        }

        /*
        LMatrix mat;
        LMatrix::transpose( &mat, matrix_ );

        glUseProgram( mShader->getGLProgram() );
        LN_CHECK_GLERROR();

        glUniformMatrix4fv( mLocation, 1, GL_FALSE, (const GLfloat*)&mat );
        LN_CHECK_GLERROR();
        */
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● 行列の配列の取得
	//----------------------------------------------------------------------
	LNRESULT ShaderVariable::getMatrixArray( LMatrix* matrices_, lnU32 count_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● 行列の配列の設定
	//----------------------------------------------------------------------
	LNRESULT ShaderVariable::setMatrixArray( const LMatrix* matrices_, lnU32 count_ )
    {
        if (mVariableBuffer.Type == TYPE_MATRIXARRAY)
        {
            memcpy(
                mVariableBuffer.VectorArray.Vectors,
                matrices_,
                sizeof(LVector4) * (LMath::min(mVariableBuffer.MatrixArray.Count, count_)));
        }

        /*
        glUseProgram( mShader->getGLProgram() );
        LN_CHECK_GLERROR();

        glUniformMatrix4fv( mLocation, count_, GL_TRUE, (const GLfloat*)&matrices_ );
        LN_CHECK_GLERROR();
        */
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● テクスチャの設定
	//----------------------------------------------------------------------
	LNRESULT ShaderVariable::setTexture( ITexture* texture_ )
    {
        if ( mSamplerState )
        {
            TextureBase* t = dynamic_cast< TextureBase* >( texture_ );
            LN_SAFE_ADDREF( t );
            LN_SAFE_RELEASE( mSamplerState->Texture );
            mSamplerState->Texture = t;
        }
        return LN_OK;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    LNRESULT ShaderVariable::setTexture( ITexture* texture_, const LSamplerState& state_ )
    {
        return setTexture( texture_ );
    }

	//----------------------------------------------------------------------
	// ● 文字列の取得
	//----------------------------------------------------------------------
    LNRESULT ShaderVariable::getString( const char** str_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : getting the value is not supported." );
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● 文字列の設定
	//----------------------------------------------------------------------
    LNRESULT ShaderVariable::setString( const char* str_ )
    {
        LN_THROW_InvalidCall( 0, "ShaderVariable : setting the string value is not supported." );
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● 変数名の取得
	//----------------------------------------------------------------------
    const lnChar* ShaderVariable::getName() const
    {
        return mName.c_str();
    }

	//----------------------------------------------------------------------
	// ● セマンティクス名の取得
	//----------------------------------------------------------------------
    const lnChar* ShaderVariable::getSemanticName() const
    {
        return mSemanticName.c_str();
    }

	//----------------------------------------------------------------------
	// ● 型情報の取得
	//----------------------------------------------------------------------
    const LNShaderVariableTypeDesc& ShaderVariable::getTypeDesc() const
    {
        return mVariableTypeDesc;
    }

	//----------------------------------------------------------------------
	// ● インデックスによってアノテーションを取得する
	//----------------------------------------------------------------------
    IShaderVariable* ShaderVariable::getAnnotationByIndex( lnU32 index_ )
    {
        if ( index_ >= mAnnotationArray.size() ) { return NULL; }
        return mAnnotationArray[ index_ ];
    }

	//----------------------------------------------------------------------
	// ● 名前によってアノテーションを取得する
	//----------------------------------------------------------------------
    IShaderVariable* ShaderVariable::getAnnotationByName( const char* name_ )
    {
        ShaderAnnotationMap::iterator it = mAnnotationMap.find( name_ );
        if ( it != mAnnotationMap.end() )
        {
            return it->second;
        }
        return NULL;
    }

	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
    void ShaderVariable::onCreateGLResource()
    {
        GLsizei name_len = 0;
        GLsizei var_size = 0;   // 配列サイズっぽい
        GLenum  var_type = 0;
        GLchar  name[256] = { 0 };
        LNGL::glGetActiveUniform( mShader->getGLProgram(), mVarIndex, 256, &name_len, &var_size, &var_type, name );
        //LN_CHECK_GLERROR();

        // 変数情報変換
        convertVariableTypeGLToLN( var_type, var_size, &mVariableTypeDesc );

        // 名前記憶
        mName = name;

        // サンプラの場合は割り当てるべきテクスチャユニット番号を取得する
        if ( mVariableTypeDesc.Class == LN_SVC_SAMPLER && !mSamplerState )
        {
            mSamplerState = LN_NEW GLSamplerState;
            mSamplerState->TextureUnitIndex = mShader->getTextureUnitIncr();
            mSamplerState->Texture   = NULL;
            mSamplerState->MagFilter = GL_NEAREST;//GL_LINEAR;
            mSamplerState->MinFilter = GL_NEAREST;//GL_LINEAR;
            mSamplerState->AddressU  = GL_REPEAT;//GL_CLAMP_TO_EDGE;
            mSamplerState->AddressV  = GL_REPEAT;//GL_CLAMP_TO_EDGE;
        }

        mLocation = LNGL::glGetUniformLocation( mShader->getGLProgram(), name );
        //LN_CHECK_GLERROR();

        // 値の一時格納
        memset(&mVariableBuffer, 0, sizeof(mVariableBuffer));
        switch (mVariableTypeDesc.Type)
        {
            case LN_SVT_BOOL: mVariableBuffer.Type = TYPE_BOOL; break;
            case LN_SVT_INT: mVariableBuffer.Type = TYPE_INT; break;
            case LN_SVT_FLOAT: mVariableBuffer.Type = TYPE_FLOAT; break;
        }
        switch (mVariableTypeDesc.Class)
        {
            case LN_SVC_VECTOR:
                // Matrix
                if (mVariableTypeDesc.Elements <= 1)
                {
                    mVariableBuffer.Type = TYPE_VECTOR;
                    mVariableBuffer.Vector = LN_NEW LVector4();
                }
                // MatrixArray
                else
                {
                    mVariableBuffer.Type = TYPE_VECTORARRAY;
                    mVariableBuffer.VectorArray.Count = mVariableTypeDesc.Elements;
                    mVariableBuffer.VectorArray.Vectors = LN_NEW LVector4[mVariableBuffer.VectorArray.Count];
                }
                break;
            case LN_SVC_MATRIX:
                // Matrix
                if (mVariableTypeDesc.Elements <= 1)
                {
                    mVariableBuffer.Type = TYPE_MATRIX;
                    mVariableBuffer.Matrix = LN_NEW LMatrix();
                }
                // MatrixArray
                else
                {
                    mVariableBuffer.Type = TYPE_MATRIXARRAY;
                    mVariableBuffer.MatrixArray.Count = mVariableTypeDesc.Elements;
                    mVariableBuffer.MatrixArray.Matrices = LN_NEW LMatrix[mVariableBuffer.MatrixArray.Count];
                }
                break;
        }
    }

	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
    void ShaderVariable::onDisposeGLResource()
    {
        // onCreateGLResource() で確保したのでここで開放…
        switch (mVariableBuffer.Type)
        {
            case TYPE_VECTOR:
                SAFE_DELETE(mVariableBuffer.Vector)
                break;
            case TYPE_VECTORARRAY:
                SAFE_DELETE_ARRAY(mVariableBuffer.VectorArray.Vectors);
                break;
            case TYPE_MATRIX:
                SAFE_DELETE(mVariableBuffer.Matrix)
                break;
            case TYPE_MATRIXARRAY:
                SAFE_DELETE_ARRAY(mVariableBuffer.MatrixArray.Matrices);
                break;
        }
    }

	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
    void ShaderVariable::onCommit()
    {
        if ( mSamplerState && mSamplerState->Texture )
        {
            LNGL::glActiveTexture( GL_TEXTURE0 + mSamplerState->TextureUnitIndex );
            //LN_CHECK_GLERROR();

            LNGL::glBindTexture( GL_TEXTURE_2D, mSamplerState->Texture->getGLTexture() );
            //LN_CHECK_GLERROR();

            LNGL::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mSamplerState->MinFilter );
            //LN_CHECK_GLERROR();
            LNGL::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mSamplerState->MagFilter );
            //LN_CHECK_GLERROR();
            LNGL::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mSamplerState->AddressU );
            //LN_CHECK_GLERROR();
            LNGL::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mSamplerState->AddressV );
            //LN_CHECK_GLERROR();

            LNGL::glUniform1i( mLocation, mSamplerState->TextureUnitIndex );
            //LN_CHECK_GLERROR();
           
            // テクスチャ変換行列の設定
            //glMatrixMode(GL_TEXTURE);
            //glLoadIdentity();
            //glTranslated(-0.5, -0.5, 0.0);


            
        }

        LNGL::glUseProgram( mShader->getGLProgram() );
        //LN_CHECK_GLERROR();

        switch (mVariableBuffer.Type)
        {
            case TYPE_BOOL:
            {
                LNGL::glUniform1i( mLocation, mVariableBuffer.Bool );
                //LN_CHECK_GLERROR();
                break;
            }
            case TYPE_INT:
            {
                LNGL::glUniform1i( mLocation, mVariableBuffer.Int );
                //LN_CHECK_GLERROR();
                break;
            }
            case TYPE_FLOAT:
            {
                LNGL::glUniform1f( mLocation, mVariableBuffer.Float );
                //LN_CHECK_GLERROR();
                break;
            }
            case TYPE_VECTOR:
            {
                LVector4* vec = mVariableBuffer.Vector;
                switch ( mVariableTypeDesc.Columns )
                {
                    case 2:
                        LNGL::glUniform2f( mLocation, vec->x, vec->y );
                        //LN_CHECK_GLERROR();
                        break;
                    case 3:
                        LNGL::glUniform3f( mLocation, vec->x, vec->y, vec->z );
                        //LN_CHECK_GLERROR();
                        break;
                    case 4:
                        LNGL::glUniform4f( mLocation, vec->x, vec->y, vec->z, vec->w );
                        //LN_CHECK_GLERROR();
                        break;
                }
                break;
            }
            case TYPE_VECTORARRAY:
            {
                 LNGL::glUniform4fv(
                    mLocation,
                    mVariableBuffer.VectorArray.Count,
                    (const GLfloat*)mVariableBuffer.VectorArray.Vectors );
                //LN_CHECK_GLERROR();
                break;
            }
            case TYPE_MATRIX:
            {
                 LNGL::glUniformMatrix4fv( mLocation, 1, GL_FALSE, (const GLfloat*)mVariableBuffer.Matrix );
                //LN_CHECK_GLERROR();
                break;
            }
            case TYPE_MATRIXARRAY:
            {
                 LNGL::glUniformMatrix4fv(
                    mLocation,
                    mVariableBuffer.MatrixArray.Count,
                    GL_TRUE,
                    (const GLfloat*)&mVariableBuffer.MatrixArray.Matrices );
                //LN_CHECK_GLERROR();
                break;
            }
        }
    }

	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
    void ShaderVariable::addAnnotationBool( const char* name_, bool v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderVariable::addAnnotationInt( const char* name_, int v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderVariable::addAnnotationFloat( const char* name_, float v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderVariable::addAnnotationVector( const char* name_, float x_, float y_, float z_, float w_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, x_, y_, z_, w_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderVariable::addAnnotationString( const char* name_, const char* v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    

//==============================================================================
// ■ ShaderAnnotation クラス
//==============================================================================

    void resetLNVariableTypeDesc( LNShaderVariableTypeDesc* desc_ )
    {
        desc_->Class = LN_SVC_SCALAR;
        desc_->Type = LN_SVT_BOOL;
        desc_->Rows = 1;
        desc_->Columns = 1;
        desc_->Elements = 1;
        desc_->Shared = false;
    }

	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
    ShaderAnnotation::ShaderAnnotation( Shader* shader_, const char* name_, bool v_ )
        : mShader   ( shader_ )
        , mName     ( name_ )
    {
        resetLNVariableTypeDesc( &mVariableTypeDesc );
        mValue.Bool = v_;
    }
    ShaderAnnotation::ShaderAnnotation( Shader* shader_, const char* name_, int v_ )
        : mShader   ( shader_ )
        , mName     ( name_ )
    {
        resetLNVariableTypeDesc( &mVariableTypeDesc );
        mVariableTypeDesc.Type = LN_SVT_INT;
        mValue.Int = v_;
    }
    ShaderAnnotation::ShaderAnnotation( Shader* shader_, const char* name_, float v_ )
        : mShader   ( shader_ )
        , mName     ( name_ )
    {
        resetLNVariableTypeDesc( &mVariableTypeDesc );
        mVariableTypeDesc.Type = LN_SVT_FLOAT;
        mValue.Float = v_;
    }
    ShaderAnnotation::ShaderAnnotation( Shader* shader_, const char* name_, float x_, float y_, float z_, float w_ )
        : mShader   ( shader_ )
        , mName     ( name_ )
    {
        resetLNVariableTypeDesc( &mVariableTypeDesc );
        mVariableTypeDesc.Class = LN_SVC_VECTOR;
        mVariableTypeDesc.Type = LN_SVT_FLOAT;
        mVariableTypeDesc.Columns = 4;
        mValue.Vector.x = x_;
        mValue.Vector.y = y_;
        mValue.Vector.z = z_;
        mValue.Vector.w = w_;
    }
    ShaderAnnotation::ShaderAnnotation( Shader* shader_, const char* name_, const char* v_ )
        : mShader   ( shader_ )
        , mName     ( name_ )
    {
        resetLNVariableTypeDesc( &mVariableTypeDesc );
        mVariableTypeDesc.Type = LN_SVT_STRING;
        mStringValue = v_;
    }


//==============================================================================
// ■ ShaderTechnique クラス
//==============================================================================

	//----------------------------------------------------------------------
	// ● コンストラクタ
	//----------------------------------------------------------------------
    ShaderTechnique::ShaderTechnique( Shader* shader_ )
        : mShader       ( shader_ )
        , mShaderPass   ( NULL )
        , mName         ( _T( "GLTechnique" ) )
    {
        mShaderPass = LN_NEW ShaderPass( mShader );
    }

	//----------------------------------------------------------------------
	// ● デストラクタ
	//----------------------------------------------------------------------
    ShaderTechnique::~ShaderTechnique()
    {
        for ( lnU32 i = 0; i < mAnnotationArray.size(); ++i )
        {
            SAFE_DELETE( mAnnotationArray[ i ] );
        }

        LN_SAFE_RELEASE( mShaderPass );
    }

	//----------------------------------------------------------------------
	// ● 名前によってアノテーションを取得する
	//----------------------------------------------------------------------
    IShaderVariable* ShaderTechnique::getAnnotationByName( const char* name_ )
    {
        ShaderAnnotationMap::iterator it = mAnnotationMap.find( name_ );
        if ( it != mAnnotationMap.end() )
        {
            return it->second;
        }
        return NULL;
    }

	//----------------------------------------------------------------------
	// ● インデックスによってアノテーションを取得する
	//----------------------------------------------------------------------
    IShaderVariable* ShaderTechnique::getAnnotationByIndex( lnU32 index_ )
    {
        if ( index_ >= mAnnotationArray.size() ) { return NULL; }
        return mAnnotationArray[ index_ ];
    }

	//----------------------------------------------------------------------
	// ● 名前によってパスを取得する
	//----------------------------------------------------------------------
    IShaderPass* ShaderTechnique::getPassByName( const lnChar* name_ )
    {
        return mShaderPass;
    }

	//----------------------------------------------------------------------
	// ● インデックスによってパスを取得する
	//----------------------------------------------------------------------
    IShaderPass* ShaderTechnique::getPassByIndex( lnU32 index_ )
    {
        if ( index_ == 0 )
        {
            return mShaderPass;
        }
        return NULL;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ShaderTechnique::onCreateGLResource()
    {
        mShaderPass->onCreateGLResource();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ShaderTechnique::onDisposeGLResource()
    {
        mShaderPass->onDisposeGLResource();
    }

	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
    void ShaderTechnique::addAnnotationBool( const char* name_, bool v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderTechnique::addAnnotationInt( const char* name_, int v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderTechnique::addAnnotationFloat( const char* name_, float v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderTechnique::addAnnotationVector( const char* name_, float x_, float y_, float z_, float w_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, x_, y_, z_, w_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderTechnique::addAnnotationString( const char* name_, const char* v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

//==============================================================================
// ■ ShaderPass クラス
//==============================================================================

    static const char* gGLSL_LNAttributeName[LN_VERTEXDECLUSAGE_MAX] =
    {
        "",                 // LN_VERTEXDECLUSAGE_UNKNOWN = 0,
        "lnPosition",       // LN_VERTEXDECLUSAGE_POSITION,        ///< 頂点位置
        "lnBlendWeight",    // LN_VERTEXDECLUSAGE_BLENDWEIGHT,     ///< ブレンドウェイト値
        "lnBlendIndices",   // LN_VERTEXDECLUSAGE_BLENDINDICES,    ///< ブレンドインデックス値
        "lnNormal",         // LN_VERTEXDECLUSAGE_NORMAL,          ///< 法線
        "lnTexCoord0",      // LN_VERTEXDECLUSAGE_TEXCOORD0,       ///< UV
        "lnTexCoord1",      // LN_VERTEXDECLUSAGE_TEXCOORD1,       ///< 追加UV1
        "lnTexCoord2",      // LN_VERTEXDECLUSAGE_TEXCOORD2,       ///< 追加UV2
        "lnTexCoord3",      // LN_VERTEXDECLUSAGE_TEXCOORD3,       ///< 追加UV3
        "lnTexCoord4",      // LN_VERTEXDECLUSAGE_TEXCOORD4,       ///< 追加UV4
        "lnTexCoord5",      // LN_VERTEXDECLUSAGE_TEXCOORD5,       ///< Sdef - C値
        "lnTexCoord6",      // LN_VERTEXDECLUSAGE_TEXCOORD6,       ///< Sdef - R0値
        "lnTexCoord7",      // LN_VERTEXDECLUSAGE_TEXCOORD7,       ///< Sdef - R1値
        "lnTexCoord8",      // LN_VERTEXDECLUSAGE_TEXCOORD8,       ///< エッジウェイト
        "lnPSize15",        // LN_VERTEXDECLUSAGE_PSIZE15,         ///< 頂点インデックス値
        "lnColor",          // LN_VERTEXDECLUSAGE_COLOR,
    };

	//----------------------------------------------------------------------
	// ● コンストラクタ
	//----------------------------------------------------------------------
    ShaderPass::ShaderPass( Shader* shader_ )
        : mShader   ( shader_ )
        , mBegan    ( false )
    {
        
    }

	//----------------------------------------------------------------------
	// ● デストラクタ
	//----------------------------------------------------------------------
    ShaderPass::~ShaderPass()
    {
        for ( lnU32 i = 0; i < mAnnotationArray.size(); ++i )
        {
            SAFE_DELETE( mAnnotationArray[ i ] );
        }
    }

	//----------------------------------------------------------------------
	// ● 名前によってアノテーションを取得する
	//----------------------------------------------------------------------
    IShaderVariable* ShaderPass::getAnnotationByName( const lnChar* name_ )
    {
        ShaderAnnotationMap::iterator it = mAnnotationMap.find( name_ );
        if ( it != mAnnotationMap.end() )
        {
            return it->second;
        }
        return NULL;
    }

	//----------------------------------------------------------------------
	// ● インデックスによってアノテーションを取得する
	//----------------------------------------------------------------------
    IShaderVariable* ShaderPass::getAnnotationByIndex( lnU32 index_ )
    {
        if ( index_ >= mAnnotationArray.size() ) { return NULL; }
        return mAnnotationArray[ index_ ];
    }

	//----------------------------------------------------------------------
	// ● パスの適用を開始する
	//----------------------------------------------------------------------
    LNRESULT ShaderPass::begin()
    {
        if ( !mBegan )
        {
            mBegan = true;
            mShader->getGraphicsDevice()->setCurrentShaderrPass( this );
            commit();
        }
        
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● パスの適用を終了する
	//----------------------------------------------------------------------
    LNRESULT ShaderPass::end()
    {
        if ( mBegan )
        {
            LNGL::glUseProgram( NULL );
            //LN_CHECK_GLERROR();
        }
        mBegan = false;
        return LN_OK;
    }

	//----------------------------------------------------------------------
	// ● パスの実行中に変化したステートをデバイスに送信する
	//----------------------------------------------------------------------
    void ShaderPass::commit()
    {
        if ( mBegan )
        {
            LNGL::glUseProgram( NULL );
            //LN_CHECK_GLERROR();

            LNGL::glUseProgram( mShader->getGLProgram() );
            //LN_CHECK_GLERROR();

            mShader->onCommit();
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ShaderPass::onCreateGLResource()
    {
        GLint params;
        LNGL::glGetProgramiv( mShader->getGLProgram(), GL_ACTIVE_ATTRIBUTES, &params );
        //LN_CHECK_GLERROR();

        // 初期化
        for ( int i = 0; i < LN_VERTEXDECLUSAGE_MAX; ++i )
        {
            mUsageEntryLocation[i] = -1;
        }

        // 総当たりだけどまぁ今のところ数少ないのでこれで・・・
        for ( int i = 0; i < params; ++i )
        {
            GLsizei name_len = 0;
            GLsizei var_size = 0;   // 配列サイズっぽい
            GLenum  var_type = 0;
            GLchar  name[256] = { 0 };
            LNGL::glGetActiveAttrib( mShader->getGLProgram(), i, 256, &name_len, &var_size, &var_type, name );
            //LN_CHECK_GLERROR();

            for ( int ln_attr_i = 0; ln_attr_i < LN_VERTEXDECLUSAGE_MAX; ++ln_attr_i )
            {
                if ( Base::StringUtil::strcmpI( name, gGLSL_LNAttributeName[ln_attr_i] ) )
                {
                    //printf( "%s: idx=%d loc:%d\n", name, i, glGetAttribLocation( shader_->getGLProgram(), name ) );
                    mUsageEntryLocation[ln_attr_i] = LNGL::glGetAttribLocation( mShader->getGLProgram(), name );//i;
                    //LN_CHECK_GLERROR();
                    break;
                }
            }
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ShaderPass::onDisposeGLResource()
    {
    }

	//----------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------
    void ShaderPass::addAnnotationBool( const char* name_, bool v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderPass::addAnnotationInt( const char* name_, int v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderPass::addAnnotationFloat( const char* name_, float v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderPass::addAnnotationVector( const char* name_, float x_, float y_, float z_, float w_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, x_, y_, z_, w_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

    void ShaderPass::addAnnotationString( const char* name_, const char* v_ )
    {
        ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, name_, v_ );
        mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
        mAnnotationArray.push_back( v );
    }

#if 0

    /*
printf( "g]%d\n", glGetError() );

mShader->onCommit();
printf( "%d\n", glGetError() );
// テクスチャ最大数
	GLint num_tex_units;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &num_tex_units);
    std::cout << "Max texture units = " << num_tex_units << std::endl;
	glActiveTexture(GL_TEXTURE0+num_tex_units);
	glBindTexture(GL_TEXTURE_2D, 0);

printf( "%d\n", glGetError() );
            glEnable(GL_TEXTURE_2D);printf( "%d\n", glGetError() );
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

            
            glUniform1i(glGetUniformLocation(mShader->getGLProgram(), "gMaterialSampler"), 0);
printf( "g]%d\n", glGetError() );
*/
           D3DTADDRESS_WRAP        GL_REPEAT
整数区切りごとにテクスチャを繰り返します

 D3DTADDRESS_CLAMP      GL_CLAMP_TO_EDGE
 境界引き伸ばし

 D3DTADDRESS_BORDER     GL_CLAMP_TO_BORDER
 範囲外塗りつぶし

            );
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, );
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE

                GL_CLAMP_TO_BORDER
GL_NEAREST
GL_LINEAR

        GL_REPEAT　→　画像が繰り返されているとするとして画像の色を拾う
GL_CLAMP　→　0～１の範囲に固定
　→　画像の端のピクセルの色を使う。
GL_CLAMP_TO_BORDER　→　境界の色を設定してそれを使う。(OpenGL 1.3以上)
GL_MIRRORED_REPEAT　→　繰り返すとき画像を反転させる (OpenGL 1.4 以上)
#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace OpenGL
} // namespace Graphics
} // namespace Core

} // namespace LNote
#endif

//==============================================================================
//
//==============================================================================