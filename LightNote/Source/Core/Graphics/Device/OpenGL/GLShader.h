//==============================================================================
// GLShader 
//------------------------------------------------------------------------------
///**
//  @file       GLShader.h
//  @brief      GLShader
//  @author     Riki
//*/
//==============================================================================

#pragma once
#ifdef LNOTE_FOR_200
#include <glfw/include/GLFW/glfw3.h>
#include "../../../FileIO/StreamObject.h"
#include "../../../FileIO/BinaryReader.h"
#include "../../Interface.h"
#include "../LNFXManager.h"
#include "../GraphicsResource.h"
#include "../ShaderVariableBase.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{
class GLShader;
class GLShaderVariable;
class GLShaderAnnotation;
class GLShaderTechnique;
class GLShaderPass;

// 検索は map を使わず、vector の先頭からひとつずつ見る。100 個とかまず使うはずないし…
typedef std::vector<GLShaderVariable*>		GLShaderVariableArray;
typedef std::vector<GLShaderAnnotation*>	GLShaderAnnotationArray;
typedef std::vector<GLShaderTechnique*>		GLShaderTechniqueArray;
typedef std::vector<GLShaderPass*>			GLShaderPassArray;

//==============================================================================
// GLAnnotatedObjectImpl
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class GLAnnotatedObjectImpl
{
public:
	~GLAnnotatedObjectImpl();
	
public:
	void				addAnnotation( const char* name, const char* value );
	IShaderVariable*	getAnnotationByIndex( lnU32 index );
	IShaderVariable*	getAnnotationByName( const lnChar* name );
	
private:
	GLShaderAnnotationArray	mShaderAnnotationArray;
};

//==============================================================================
// ■ GLShader
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class GLShader
    : public GraphicsResource
    , public IShader
	, public Base::ICacheObject
	, public IGLShaderBuilder
{
	LN_CACHE_OBJECT_DECL;
public:
	GLShader();
	virtual ~GLShader();

public:

	/// 作成
	void create( GLGraphicsDevice* device, FileIO::Stream* stream );

	void addLeveledLogs( LNShaderCompileResult result, const lnChar* log );

	void addShaderVariable( GLShaderVariable* var ) { mShaderVariableArray.push_back( var ); }

public:
	// IShader
	virtual IShaderVariable* getVariableByIndex( lnU32 index );
	virtual IShaderVariable* getVariableByName( const char* name );
	virtual IShaderTechnique* getTechniqueByIndex( lnU32 index );
	virtual IShaderTechnique* getTechniqueByName( const char* name );
	virtual LNShaderCompileResult getCompileResult() { return mCompileResult; }
	virtual const lnChar* getCompileErrors() { return mCompileErrors.c_str(); }
	
public:
	// GraphicsResource
	virtual void onPauseDevice();
	virtual void onResumeDevice();

private:

	void _create();
	void _dispose();
	
private:
	// IGLShaderBuilder
	virtual IAnnotatedObject*	OnCreateTechnique( const char* name );
	virtual IAnnotatedObject*	OnCreatePass( IAnnotatedObject* parentTech, const char* name );
	virtual void				OnCreateVertexShader( IAnnotatedObject* parentPass, const char* code, size_t size );
	virtual void				OnCreatePixelShader( IAnnotatedObject* parentPass, const char* code, size_t size );
	virtual void				OnSetRenderState( IAnnotatedObject* parentPass, const char* name, const char* value );
	virtual void				OnEndPass( IAnnotatedObject* pass );
	virtual IAnnotatedObject*	OnTrySetVariableParam( const char* targetName, const char* semantic, bool shared, const char* attachedTextureVarName );

private:
	GLGraphicsDevice*		mGraphicsDevice;
	LNFXManager				mLNFXManager;		// 元のコードとして持っておく
	LNShaderCompileResult   mCompileResult;
    lnString				mCompileErrors;

	GLShaderVariableArray	mShaderVariableArray;
	GLShaderTechniqueArray	mShaderTechniqueArray;
};

//==============================================================================
// ■ GLShaderVariable
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class GLShaderVariable
    : public ShaderVariableBase
{
public:
	GLShaderVariable();
	virtual ~GLShaderVariable();

public:
	void create( const char* name, const LNShaderVariableTypeDesc& desc, ShaderVariableType lnType );

public:
	virtual IShaderVariable* getAnnotationByIndex( lnU32 index );
	virtual IShaderVariable* getAnnotationByName( const char* name );

private:

	GLShaderAnnotationArray	mShaderAnnotationArray;
};

//==============================================================================
// ■ GLShaderAnnotation
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class GLShaderAnnotation
    : public ShaderVariableBase
{
public:
	GLShaderAnnotation();
	virtual ~GLShaderAnnotation();

public:
	void create( const char* name, const char* value )
	{
		LNShaderVariableTypeDesc desc;
		desc.Class = LN_SVC_SCALAR;
		desc.Type = LN_SVT_STRING;
		desc.Rows = 1;
		desc.Columns = 1;
		desc.Elements = 1;
		desc.Shared = false;
		ShaderVariableBase::initialize(
			SVT_STRING, lnRefString( name ), lnRefString(), desc );
		ShaderVariableBase::setString( value );
	}

public:
	virtual IShaderVariable* getAnnotationByIndex( lnU32 index ) { return NULL; }
	virtual IShaderVariable* getAnnotationByName( const char* name ) { return NULL; }

};

//==============================================================================
// ■ GLShaderTechnique
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class GLShaderTechnique
    : /*public Base::ReferenceObject
    ,*/ public IShaderTechnique
	, public IAnnotatedObject
{
public:
	GLShaderTechnique();
	virtual ~GLShaderTechnique();
	LN_REFOBJ_METHODS;

public:
	void addPass( GLShaderPass* pass );

public:
    virtual const lnChar*		getName() const { return mName.c_str(); }
	virtual lnU32				getPasses() const { return mShaderPassArray.size(); }
	virtual IShaderPass*		getPassByIndex( lnU32 index );
    virtual IShaderPass*		getPassByName( const lnChar* name );
	virtual IShaderVariable*	getAnnotationByIndex( lnU32 index ) { return mGLAnnotatedObjectImpl.getAnnotationByIndex( index ); }
	virtual IShaderVariable*	getAnnotationByName( const lnChar* name ) { return mGLAnnotatedObjectImpl.getAnnotationByName( name ); }

private:
	// IAnnotatedObject
	virtual void addAnnotation( const char* name, const char* value ) { mGLAnnotatedObjectImpl.addAnnotation( name, value ); }

private:
	lnString	mName;

	GLShaderPassArray		mShaderPassArray;
	GLAnnotatedObjectImpl	mGLAnnotatedObjectImpl;
};

//==============================================================================
// ■ GLShaderPass
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class GLShaderPass
    :/* public Base::ReferenceObject
    ,*/ public IShaderPass
	, public IAnnotatedObject
{
public:
	GLShaderPass();
	virtual ~GLShaderPass();
///	LN_REFOBJ_METHODS;

public:
	void create( GLShader* shader );
	void createVertexShader( const char* code, size_t size );
	void createPixelShader( const char* code, size_t size );

	/// createVertexShader() と createPixelShader() が完了した後に呼ぶ
	void initialLinkShader();

public:
	virtual const lnChar* getName() const { return mName.c_str(); }
	virtual IShaderVariable*	getAnnotationByIndex( lnU32 index ) { return mGLAnnotatedObjectImpl.getAnnotationByIndex( index ); }
	virtual IShaderVariable*	getAnnotationByName( const lnChar* name ) { return mGLAnnotatedObjectImpl.getAnnotationByName( name ); }
	virtual void begin();
	virtual void end();
	virtual void commit();
	
private:
	// IAnnotatedObject
	virtual void addAnnotation( const char* name, const char* value ) { mGLAnnotatedObjectImpl.addAnnotation( name, value ); }

private:
	void _linkShader();
	void _compileShader( GLuint shader, const char* code, GLint size );
	void _tryCreateVariable( int varIndex );
	void _lookupAttribLocation();

private:
	// シェーダプログラムごとに異なるため Pass 側で管理する変数情報
	struct VariableInfo
	{
		GLShaderVariable*	ShaderVariable;
		int					VarIndex;
		GLint				Location;
		int					TextureUnitIndex;
	};
	typedef std::vector<VariableInfo>	VariableInfoArray;

	GLShader*				mShader;
	lnRefString				mName;
	GLuint					mProgram;

	const char*				mVertexShaderCode;
	size_t					mVertexShaderCodeSize;
	const char*				mPixelShaderCode;
	size_t					mPixelShaderCodeSize;
	int						mNextTextureUnitIndex;
	VariableInfoArray		mVariableInfoArray;

	/*GLint*/lnS8			mAttribLocationTable[LN_USAGE_MAX][LNUsageIndexMax];	///< Texcoord 1 の Location は？みたいなテーブル。サイズをおさえるためとりあえず s8


	//GLuint					mVertexShader;
	//GLuint					mPixelShader;
	//LNShaderCompileResult	mCompileResult;
	//lnString				mCompileLog;

	GLAnnotatedObjectImpl	mGLAnnotatedObjectImpl;
};

//==============================================================================
// ■ LNFXFile
//------------------------------------------------------------------------------
///**
//  @brief 
//*/
//==============================================================================
class LNFXFile
{
public:

	static const lnChar* Magic;						///< "lnfx"

	static const lnChar* VariableElementName;		///< "Variable"
	static const lnChar* AnnotationElementName;		///< "Annotation"
	static const lnChar* TechniqueElementName;		///< "Technique"
	static const lnChar* PassElementName;			///< "Pass"
	static const lnChar* VertexShaderElementName;	///< "VertexShader"
	static const lnChar* PixelShaderElementName;	///< "PixelShader"
	static const lnChar* RenderStateElementName;	///< "RenderState"

	static const lnChar* NameAttributeName;			///< "Name"
	static const lnChar* SemanticAttributeName;		///< "Semantic"
	static const lnChar* SharedAttributeName;		///< "Shared"
	static const lnChar* TextureAttributeName;		///< "Texture"
	static const lnChar* ValueAttributeName;		///< "Value"
	static const lnChar* EntryPointAttributeName;	///< "Shader"	TODO

public:

	struct Annotation
	{
		lnString	Name;
		lnRefString	Value;
	};

	typedef std::vector<Annotation>	AnnotationArray;

	struct Variable
	{
		lnString		Name;
		lnRefString		Semantic;
		AnnotationArray	Annotations;
		lnRefString		Texture;
		bool			Shared;
	};

	typedef std::vector<Variable>	VariableArray;

	struct ShaderProgram
	{
		lnString		Name;
		size_t			SeekPos;
		size_t			Size;
	};

	typedef std::vector<ShaderProgram*>	ShaderProgramArray;

public:
	~LNFXFile();

public:

	/// 作成
	void create( FileIO::Stream* stream, GLShader* shader );

private:
	ShaderProgram* _findShaderProgram( const char* programName );
	void _analyzeXML( const char* xml, size_t size );
	void _analyzeVariableElement( XML::Element* xmlElement );
	void _analyzeAnnotationElement( XML::Element* xmlElement, AnnotationArray* annotations );
	void _analyzeTechniqueElement( XML::Element* xmlElement );
	void _analyzePassElement( XML::Element* xmlElement, GLShaderTechnique* parentTech );

private:
	FileIO::Stream*		mStream;
	GLShader*			mShader;
	Base::TempBuffer	mTempBuffer;
	VariableArray		mVariableArray;
	ShaderProgramArray	mShaderProgramArray;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote


#else

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include <map>
#include "../../../Base/Cache.h"
#include "../../../Base/RefString.h"
#include "../../Interface.h"
#include "GLUtil.h"
#include "GLShaderManager.h"

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
class GraphicsDevice;
class TextureBase;
class ShaderManager;
class Shader;
class ShaderVariable;
class ShaderAnnotation;
class ShaderTechnique;
class ShaderPass;

typedef Base::CacheObjectManager< Shader >    ShaderCache;

typedef std::vector< ShaderVariable* >                  ShaderVariableArray;
typedef std::map< LRefTString, ShaderVariable* >        ShaderVariableMap;
typedef std::pair< LRefTString, ShaderVariable* >       ShaderVariablePair;

typedef std::vector< ShaderAnnotation* >                ShaderAnnotationArray;
typedef std::map< LRefTString, ShaderAnnotation* >      ShaderAnnotationMap;
typedef std::pair< LRefTString, ShaderAnnotation* >     ShaderAnnotationPair;

typedef std::vector< ShaderTechnique* >                 ShaderTechniqueArray;
typedef std::map< LRefTString, ShaderTechnique* >       ShaderTechniqueMap;
typedef std::pair< LRefTString, ShaderTechnique* >      ShaderTechniquePair;

typedef std::vector< ShaderPass* >                      ShaderPassArray;
typedef std::map< LRefTString, ShaderPass* >            ShaderPassMap;
typedef std::pair< LRefTString, ShaderPass* >           ShaderPassPair;


//==============================================================================
// ■ Shader クラス
//------------------------------------------------------------------------------
///**
//  @brief      シェーダプログラムを扱うクラス
//*/
//==============================================================================
class Shader
    : public GLResource//public Base::ReferenceObject
    , public ShaderCache::CachedObject
    , public IShader
{
public:

	/// コンストラクタ
    Shader( ShaderManager* manager_ );

	/// デストラクタ
    virtual ~Shader();

    LN_CACHED_REFOBJ_METHODS;

public:

	/// 初期化
    void initialize( const lnChar* filename_ );

public:

	/// ブール値の設定
	virtual LNRESULT setBool( const lnChar* name_, bool b_ );

	/// 整数値の設定
	virtual LNRESULT setInt( const lnChar* name_, int value_ );

	/// 実数値の設定
	virtual LNRESULT setFloat( const lnChar* name_, lnFloat value_ );

	/// ベクトルの設定
	virtual LNRESULT setVector( const lnChar* name_, const LVector4& vec_ );

	/// ベクトル配列の設定
	virtual LNRESULT setVectorArray( const lnChar* name_, const LVector4* vec_, lnU32 count_ );

	/// 行列の設定
	virtual LNRESULT setMatrix( const lnChar* name_, const LMatrix& matrix_ );

	/// 行列の配列の設定
	virtual LNRESULT setMatrixArray( const lnChar* name_, const LMatrix* matrices_, lnU32 count_ );

	/// テクスチャの設定
	virtual LNRESULT setTexture( const lnChar* name_, ITexture* texture_ );

	/// 名前によって変数を取得する
	virtual IShaderVariable* getVariableByName( const lnChar* name_ );

	/// インデックスによって変数を取得する
	virtual IShaderVariable* getVariableByIndex( lnU32 index_ );

	/// セマンティクスによって変数を取得する
	//virtual IShaderVariable* getVariableBySemantic( const char* name_ );
    
	/// 名前によってテクニックを取得する
	virtual IShaderTechnique* getTechniqueByName( const lnChar* name_ );

	/// インデックスによってテクニックを取得する
	virtual IShaderTechnique* getTechniqueByIndex( lnU32 index_ );

	/// シェーダの情報を取得する
    virtual const LNShaderDesc& getDesc() { return mDesc; }
    
	/// コンパイル結果を取得する
    virtual LNShaderCompileResult getCompileResult() { return mCompileResult; }

	/// コンパイルエラー・警告の文字列を返す
    virtual const lnChar* getCompileErrors() { return mCompileErrors.c_str(); }

public:

    const LRefTString& getName() const { return mFilename; }

	///// デバイスリセット直前 GraphicsDevice から呼ばれる
	//void onLostDevice();

	///// デバイスリセット後に GraphicsDevice から呼ばれる
	//void onResetDevice();

	/// プログラムオブジェクトの取得
    GLuint getGLProgram() { return mProgram; }

	/// (0スタート)
    int getTextureUnitIncr() { int i = mLastTextureUnit; ++mLastTextureUnit; return i; }

	/// GraphicsDevice の取得
    GraphicsDevice* getGraphicsDevice() { return mManager->getGraphicsDevice(); }

	/// パスの begin()、commit() から呼ばれる
    void onCommit();


    virtual void onCommitGLResource();
    virtual void onDisposeGLResource();
    void onLostDevice() { GLResource::onLostDevice(); }
    void onResetDevice() { GLResource::onResetDevice(); GLResource::commitChanges(); }

private:

	// 以下3つは描画タスクと同じスレッドから呼ぶ。
    void _createOnInitial();    // こっちはまっさらな状態から作成
    void _createOnRefresh();    // ShaderVariable 等は出来上がっている前提
    void _buildFromFile();      // 今のところ、_buildFromFile()→_build()→_compileShader() でビルド
    void _build( const char* code_ );
    bool _compileShader( GLuint shader_, int code_count_, const char** code_, GLint* size_ );

private:

    ShaderManager*          mManager;
    LRefTString             mFilename;

    GLuint                  mVertexShader;      // メンバに持つ必要ないかも？
    GLuint                  mFragmentShader;
    GLuint                  mProgram;

    LNShaderCompileResult   mCompileResult;
    LRefTString             mCompileErrors;
    LNShaderDesc            mDesc;

    ShaderTechnique*        mShaderTechnique;   ///< テクニックはひとつだけ
	//ShaderTechniqueArray    mShaderTechniqueArray;
	//ShaderTechniqueMap      mShaderTechniqueMap;
    ShaderVariableArray     mShaderVariableArray;
    ShaderVariableMap       mShaderVariableMap;

    std::string             mScript;

    int                     mLastTextureUnit;

private:

	// 描画スレッドを使う場合にのみ使用
    class Initializer
    {
    public:

        Initializer(Shader* shader_)
            : mShader       (shader_)
            , mInitialized  (false)
        {}

        /// call by delegate
        void initialize()
        {
            mShader->_createOnInitial();
            mInitialized.setTrue();
        }

        /// call by Shader class
        void waitInitialized() { mInitialized.wait(); }

    private:

        Shader*     mShader;
        Threading::EventFlag   mInitialized;
    };

    friend class Initializer; 
};

/*
enum LNShaderVariableSourceType
{
    LN_SVST_BOOL = 0,
    LN_SVST_INT,
    LN_SVST_FLOAT,
    LN_SVST_VECTOR,
    LN_SVST_VECTOR_ARRAY,
    LN_SVST_MATRIX,
    LN_SVST_MATRIX_ARRAY,
    LN_SVST_TEXTURE,
    LN_SVST_STRING,
};

/// アノテーションの生成情報
struct LNShaderAnnotationSource
{
    LNShaderVariableType    Type;
    LRefTString             AnnotationName;
    
    union
    {
        struct
        {
            bool        Value;
        } Bool;
        
        struct
        {
            int         Value;
        } Int;
        
        struct
        {
            lnFloat     Value;
        } Float;
        
        struct
        {
            LVector4    Value;
        } Vector;
        
        struct
        {
            std::vector< LVector4 > Values;
        } VectorArray;
        
        struct
        {
            LMatrix     Value;
        } Matrix;
        
        struct
        {
            std::vector< LMatrix >  Values;
        } MatrixArray;
        
        //struct
        //{
        //} Texture;
        
        struct
        {
            LRefTString     Value;
        } String;
    };
};

typedef std::vector< LNShaderAnnotationSource > ShaderAnnotationSourceArray;

/// シェーダ変数に付加する情報
struct LNShaderVariableAdditionalData
{
    LNShaderVariableType        Type;
    LRefTString                 VariableName;       ///< 対象の変数名 (見つからなかった場合はエラー)
    LRefTString                 Semantics;          ///< セマンティクス
    ShaderAnnotationSourceArray AnnotationArray;    ///< アノテーション
    
    union
    {
        struct
        {
        } Bool;
        
        struct
        {
            int         Value;
        } Int;
        
        struct
        {
            lnFloat     Value;
        } Float;
        
        struct
        {
            LVector4    Value;
        } Vector;
        
        struct
        {
            std::vector< LVector4 > Values;
        } VectorArray;
        
        struct
        {
            LMatrix     Value;
        } Matrix;
        
        struct
        {
            std::vector< LMatrix >  Values;
        } MatrixArray;
        
        struct
        {
            std::vector< LMatrix >  Values;
        } Texture;
        
        struct
        {
            std::vector< LMatrix >  Values;
        } String;
    };
};
*/

//==============================================================================
// ■ ShaderVariable クラス
//------------------------------------------------------------------------------
///**
//  @brief      シェーダプログラム内の変数を扱うクラス
//*/
//==============================================================================
class ShaderVariable
    : public Base::ReferenceObject
    , public IShaderVariable
{
public:

	/// コンストラクタ
    ShaderVariable( Shader* shader_, int var_index_ );

	/// デストラクタ
    virtual ~ShaderVariable();

    LN_REFOBJ_METHODS;

public:
	virtual ShaderVariableType getType() const { return SVT_UNKNOWN; }

	/// ブール値の取得
	virtual LNRESULT getBool( bool* b_ );

	/// ブール値の設定
	virtual LNRESULT setBool( bool b_ );

	/// 整数値の取得
	virtual LNRESULT getInt( int* value_ );

	/// 整数値の設定
	virtual LNRESULT setInt( int value_ );

	/// 実数値の取得
    virtual LNRESULT getFloat( lnFloat* value_ );

	/// 実数値の設定
    virtual LNRESULT setFloat( lnFloat value_ );

	/// ベクトルの取得
	virtual LNRESULT getVector( LVector4* vec_ );

	/// ベクトルの設定
	virtual LNRESULT setVector( const LVector4& vec_ );

	/// ベクトル配列の取得
	virtual LNRESULT getVectorArray( LVector4* vec_, lnU32 count_ );
    
	/// ベクトル配列の設定
	virtual LNRESULT setVectorArray( const LVector4* vec_, lnU32 count_ );

	/// 行列の取得
    virtual LNRESULT getMatrix( LMatrix* matrix_ );

	/// 行列の設定
    virtual LNRESULT setMatrix( const LMatrix& matrix_ );

	/// 行列の配列の取得
	virtual LNRESULT getMatrixArray( LMatrix* matrices_, lnU32 count_ );

	/// 行列の配列の設定
	virtual LNRESULT setMatrixArray( const LMatrix* matrices_, lnU32 count_ );

	/// テクスチャの取得
	virtual ITexture* getTexture() { return NULL; }

	/// テクスチャの設定
	virtual LNRESULT setTexture( ITexture* texture_ );

	/// テクスチャの設定
	virtual LNRESULT setTexture( ITexture* texture_, const LSamplerState& state_ );

	/// 文字列の取得
	virtual LNRESULT getString( const char** str_ );

	/// 文字列の設定
	virtual LNRESULT setString( const char* str_ );

	/// 変数名の取得
	virtual const char* getName() const;

	/// セマンティクス名の取得
	virtual const char* getSemanticName() const;

	/// 型情報の取得
    virtual const LNShaderVariableTypeDesc& getTypeDesc() const;

	/// インデックスによってアノテーションを取得する
	virtual IShaderVariable* getAnnotationByIndex( lnU32 index_ );

	/// 名前によってアノテーションを取得する
	virtual IShaderVariable* getAnnotationByName( const char* name_ );

public:

    void onCreateGLResource();
    void onDisposeGLResource();

	/// パスの begin()、commit() から呼ばれる
    void onCommit();

	// 以下、追加設定用。スクリプトから設定すること前提なので char

	/// (Shader でセマンティクス名から変数を検索する機能を実装する場合はmap更新する必要があるので注意)
    void setSemanticName( const char* name_ ) { mSemanticName = name_; }

    void addAnnotationBool( const char* name_, bool v_ );
    void addAnnotationInt( const char* name_, int v_ );
    void addAnnotationFloat( const char* name_, float v_ );
    void addAnnotationVector( const char* name_, float x_, float y_, float z_, float w_ );
    void addAnnotationString( const char* name_, const char* v_ );

private:

    struct GLSamplerState
    {
        int             TextureUnitIndex;   ///< サンプラ型の場合、どのテクスチャユニットを使うか (0スタート)
        TextureBase*    Texture;           ///< サンプラ型の場合に setTexture() されたテクスチャ
        GLint           MagFilter;
        GLint           MinFilter;
        GLint           AddressU;
        GLint           AddressV;
    };

    enum UsableType
    {
        TYPE_BOOL = 0,
        TYPE_INT,
        TYPE_FLOAT,
        TYPE_VECTOR,
        TYPE_VECTORARRAY,
        TYPE_MATRIX,
        TYPE_MATRIXARRAY,
        //TYPE_TEXTURE,
    };

    struct
    {
        UsableType Type;    ///< テクスチャ型は TYPE_BOOL になるので注意。

        union
        {
            bool        Bool;
            int         Int;
            lnFloat     Float;
            LVector4*   Vector;         // new

            struct
            {
                LVector4*   Vectors;    // new
                lnU32       Count;
            } VectorArray;

            LMatrix*    Matrix;         // new

            struct
            {
                LMatrix*    Matrices;   // new
                lnU32       Count;
            } MatrixArray;
            
            //ITexture*   Texture;        // mSamplerState で扱っているのでここではナシ

            //String
        };   
    } mVariableBuffer;

private:

    Shader*                     mShader;
    int                         mVarIndex;
    GLint                       mLocation;
    LRefTString                 mName;
    LRefTString                 mSemanticName;
    ShaderAnnotationArray       mAnnotationArray;
    ShaderAnnotationMap         mAnnotationMap;
    LNShaderVariableTypeDesc    mVariableTypeDesc;

    GLSamplerState*             mSamplerState;      ///< サンプラ型の場合に作成される


};

//==============================================================================
// ■ ShaderAnnotation クラス
//------------------------------------------------------------------------------
///**
//  @brief      シェーダプログラムのアノテーションのクラス
//*/
//==============================================================================
class ShaderAnnotation
    : public Base::ReferenceObject
    , public IShaderVariable
{
public:

    ShaderAnnotation( Shader* shader_, const char* name_, bool v_ );
    ShaderAnnotation( Shader* shader_, const char* name_, int v_ );
    ShaderAnnotation( Shader* shader_, const char* name_, float v_ );
    ShaderAnnotation( Shader* shader_, const char* name_, float x_, float y_, float z_, float w_ );
    ShaderAnnotation( Shader* shader_, const char* name_, const char* v_ );

    LN_REFOBJ_METHODS;

public:
	virtual ShaderVariableType getType() const { return SVT_UNKNOWN; }

	/// ブール値の取得
    virtual LNRESULT getBool( bool* b_ ) { *b_ = mValue.Bool; return LN_OK; }

	/// ブール値の設定
    virtual LNRESULT setBool( bool b_ ) { return LN_OK; }

	/// 整数値の取得
	virtual LNRESULT getInt( int* value_ ) { *value_ = mValue.Int; return LN_OK; }

	/// 整数値の設定
	virtual LNRESULT setInt( int value_ ) { return LN_OK; }

	/// 実数値の取得
    virtual LNRESULT getFloat( lnFloat* value_ ) { *value_ = mValue.Float; return LN_OK; }

	/// 実数値の設定
    virtual LNRESULT setFloat( lnFloat value_ ) { return LN_OK; }

	/// ベクトルの取得
    virtual LNRESULT getVector( LVector4* vec_ ) { vec_->set( mValue.Vector.x, mValue.Vector.y, mValue.Vector.z, mValue.Vector.w ); return LN_OK; }

	/// ベクトルの設定
	virtual LNRESULT setVector( const LVector4& vec_ ) { return LN_OK; }

	/// ベクトル配列の取得
    virtual LNRESULT getVectorArray( LVector4* vec_, lnU32 count_ ) { return LN_OK; }
    
	/// ベクトル配列の設定
	virtual LNRESULT setVectorArray( const LVector4* vec_, lnU32 count_ ) { return LN_OK; }

	/// 行列の取得
    virtual LNRESULT getMatrix( LMatrix* matrix_ ) { return LN_OK; }

	/// 行列の設定
    virtual LNRESULT setMatrix( const LMatrix& matrix_ ) { return LN_OK; }

	/// 行列の配列の取得
    virtual LNRESULT getMatrixArray( LMatrix* matrices_, lnU32 count_ ) { return LN_OK; }

	/// 行列の配列の設定
	virtual LNRESULT setMatrixArray( const LMatrix* matrices_, lnU32 count_ ) { return LN_OK; }

	/// テクスチャの取得
	virtual ITexture* getTexture() { return NULL; }

	/// テクスチャの設定
    virtual LNRESULT setTexture( ITexture* texture_ ) { return LN_OK; }

	/// テクスチャの設定
	virtual LNRESULT setTexture( ITexture* texture_, const LSamplerState& state_ ) { return LN_OK; }

	/// 文字列の取得
    virtual LNRESULT getString( const lnChar** str_ ) { *str_ = mStringValue.c_str(); return LN_OK; }

	/// 文字列の設定
	virtual LNRESULT setString( const lnChar* str_ ) { return LN_OK; }

	/// 変数名の取得
    virtual const lnChar* getName() const { return mName.c_str(); }

	/// セマンティクス名の取得
    virtual const lnChar* getSemanticName() const { return NULL; }

	/// 型情報の取得
    virtual const LNShaderVariableTypeDesc& getTypeDesc() const { return mVariableTypeDesc; }

	/// インデックスによってアノテーションを取得する
    virtual IShaderVariable* getAnnotationByIndex( lnU32 index_ ) { return NULL; }

	/// 名前によってアノテーションを取得する
	virtual IShaderVariable* getAnnotationByName( const char* name_ ) { return NULL; }

public:

    

private:

    Shader*                     mShader;
    LRefTString                 mName;
    LNShaderVariableTypeDesc    mVariableTypeDesc;

    union
    {
        bool        Bool;
        int         Int;
        lnFloat     Float;
        struct
        {
            float x, y, z, w;
        } Vector;
              
    } mValue;

    LRefTString mStringValue;   ///< コピーコンストラクタ云々でエラーになるので union からはずす
};

//==============================================================================
// ■ ShaderTechnique クラス
//------------------------------------------------------------------------------
///**
//  @brief      シェーダプログラム内のテクニックを扱うクラス
//*/
//==============================================================================
class ShaderTechnique
    : public Base::ReferenceObject
    , public IShaderTechnique
{
public:

	/// コンストラクタ
    ShaderTechnique( Shader* shader_ );

	/// デストラクタ
    virtual ~ShaderTechnique();

    LN_REFOBJ_METHODS;

public:

	/// 名前によってアノテーションを取得する
	virtual IShaderVariable* getAnnotationByName( const lnChar* name_ );

	/// インデックスによってアノテーションを取得する
	virtual IShaderVariable* getAnnotationByIndex( lnU32 index_ );

	/// テクニックの名前を取得する
    virtual const lnChar* getName() const { return mName.c_str(); }

	/// テクニック内のパスの数を取得する
	virtual lnU32 getPasses() const { return 1; }

	/// 名前によってパスを取得する
    virtual IShaderPass* getPassByName( const lnChar* name_ );

	/// インデックスによってパスを取得する
	virtual IShaderPass* getPassByIndex( lnU32 index_ );

public:

    void onCreateGLResource();
    void onDisposeGLResource();

    void addAnnotationBool( const char* name_, bool v_ );
    void addAnnotationInt( const char* name_, int v_ );
    void addAnnotationFloat( const char* name_, float v_ );
    void addAnnotationVector( const char* name_, float x_, float y_, float z_, float w_ );
    void addAnnotationString( const char* name_, const char* v_ );

private:

    Shader*                 mShader;

    lnU32                     mPasses;
    LRefTString             mName;
   
    ShaderAnnotationArray   mAnnotationArray;
    ShaderAnnotationMap     mAnnotationMap;

    ShaderPass*             mShaderPass;

	//ShaderPassArray         mShaderPassArray;
	//ShaderPassMap           mShaderPassMap;
};

//==============================================================================
// ■ ShaderPass クラス
//------------------------------------------------------------------------------
///**
//  @brief      シェーダプログラム内のパスを扱うクラス
//*/
//==============================================================================
class ShaderPass
    : public Base::ReferenceObject
    , public IShaderPass
{
public:

	/// コンストラクタ
    ShaderPass( Shader* shader_ );

	/// デストラクタ
    virtual ~ShaderPass();

    LN_REFOBJ_METHODS;

public:

	/// パスの名前を取得する
    virtual const lnChar* getName() const { return mName.c_str(); }

	/// 名前によってアノテーションを取得する
	virtual IShaderVariable* getAnnotationByName( const lnChar* name_ );

	/// インデックスによってアノテーションを取得する
	virtual IShaderVariable* getAnnotationByIndex( lnU32 index_ );

	/// パスの適用を開始する
	virtual LNRESULT begin();

	/// パスの適用を終了する
	virtual LNRESULT end();

	/// パスの実行中に変化したステートをデバイスに送信する
	virtual void commit();

public:

    void onCreateGLResource();
    void onDisposeGLResource();

    void addAnnotationBool( const char* name_, bool v_ );
    void addAnnotationInt( const char* name_, int v_ );
    void addAnnotationFloat( const char* name_, float v_ );
    void addAnnotationVector( const char* name_, float x_, float y_, float z_, float w_ );
    void addAnnotationString( const char* name_, const char* v_ );

    GLint getUsageEntryLocation( LNVertexDeclUsage usage_ ) { return mUsageEntryLocation[usage_]; }

private:

   /* struct UsageEntry
    {
        LNVertexDeclUsage   Usage;
        
    };*/

private:

    Shader*                 mShader;
    LRefTString             mName;
    ShaderAnnotationArray   mAnnotationArray;
    ShaderAnnotationMap     mAnnotationMap;
    GLint                   mUsageEntryLocation[LN_VERTEXDECLUSAGE_MAX];
	//UsageEntry              mUsageEntrys[LN_VERTEXDECLUSAGE_MAX];
    bool                    mBegan;
};

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