//==============================================================================
// DX9Shader 
//------------------------------------------------------------------------------
///**
//  @file       DX9Shader.h
//  @brief      DX9Shader
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../../Base/Cache.h"
#include "../../Interface.h"
#include "../GraphicsResource.h"
#include "../ShaderVariableBase.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{
class DX9Shader;
class DX9ShaderVariable;
class DX9ShaderAnnotation;
class DX9ShaderTechnique;
class DX9ShaderPass;

typedef std::vector< DX9ShaderVariable* >                  DX9ShaderVariableArray;
typedef std::map< LRefTString, DX9ShaderVariable* >        DX9ShaderVariableMap;
typedef std::pair< LRefTString, DX9ShaderVariable* >       DX9ShaderVariablePair;

typedef std::vector< DX9ShaderAnnotation* >                DX9ShaderAnnotationArray;
typedef std::map< LRefTString, DX9ShaderAnnotation* >      DX9ShaderAnnotationMap;
typedef std::pair< LRefTString, DX9ShaderAnnotation* >     DX9ShaderAnnotationPair;

typedef std::vector< DX9ShaderTechnique* >                 DX9ShaderTechniqueArray;
typedef std::map< LRefTString, DX9ShaderTechnique* >       DX9ShaderTechniqueMap;
typedef std::pair< LRefTString, DX9ShaderTechnique* >      DX9ShaderTechniquePair;

typedef std::vector< DX9ShaderPass* >                      DX9ShaderPassArray;
typedef std::map< LRefTString, DX9ShaderPass* >            DX9ShaderPassMap;
typedef std::pair< LRefTString, DX9ShaderPass* >           DX9ShaderPassPair;

//==============================================================================
// ■ DX9Shader
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class DX9Shader
    : public GraphicsResource
    , public IShader
	, public Base::ICacheObject
{
	LN_CACHE_OBJECT_DECL;
public:
	DX9Shader();
	virtual ~DX9Shader();

public:

	/// 作成
	//void create( DX9GraphicsDevice* device, const lnChar* filePath );
	//void create( DX9GraphicsDevice* device, const char* code_, int size );
	void create(DX9GraphicsDevice* device, FileIO::Stream* stream, lnString* errors);

public:
	// IShader
	virtual IShaderVariable* getVariableByName( const char* name );
	virtual IShaderVariable* getVariableByIndex( lnU32 index );
	virtual IShaderTechnique* getTechniqueByName( const char* name );
	virtual IShaderTechnique* getTechniqueByIndex( lnU32 index );
	virtual LNShaderCompileResult getCompileResult() { return mCompileResult; }
	virtual const lnChar* getCompileErrors() { return mCompileErrors.c_str(); }
	
public:
	// GraphicsResource
	virtual void onPauseDevice();
	virtual void onResumeDevice();

private:
	DX9GraphicsDevice*		mGraphicsDevice;
    ID3DXEffect*            mDxEffect;
    LNShaderCompileResult   mCompileResult;
    lnRefString             mCompileErrors;
	int						mVariableCount;			///< パラメータの数
    int						mTechniqueCount;		///< テクニックの数

	DX9ShaderTechniqueArray	mShaderTechniqueArray;
    DX9ShaderTechniqueMap	mShaderTechniqueMap;
    DX9ShaderVariableArray	mShaderVariableArray;
    DX9ShaderVariableMap	mShaderVariableMap;
    DX9ShaderVariableArray	mTextureVariableArray;  ///< (サンプラステート実装のための検索対象)
};

//==============================================================================
// ■ DX9ShaderVariable
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class DX9ShaderVariable
    : public ShaderVariableBase
{
public:
	DX9ShaderVariable( DX9Shader* shader, ID3DXEffect* dxEffect, D3DXHANDLE handle );
	virtual ~DX9ShaderVariable();


public:

    struct TextureParam
    {
        Texture*       Texture;
        LSamplerState   SamplerState;
    };

public:

	/// テクスチャ型であるかを判定 (LN_SVT_TEXTURE[*D])
    bool isTextureType() const { return (mTextureParam != NULL); }

	/// setTexture() で設定された情報
    const TextureParam& getTextureParam() const { return *mTextureParam; }

public:
	virtual bool getBool();
	virtual void setBool( bool b );
	virtual int getInt();
	virtual void setInt( int value );
    virtual lnFloat getFloat();
    virtual void setFloat( lnFloat value );
	virtual const LVector4& getVector();
	virtual void setVector( const LVector4& vec );
	virtual const LVector4* getVectorArray();
	virtual void setVectorArray( const LVector4* vec, lnU32 count );
    virtual const LMatrix& getMatrix();
    virtual void setMatrix( const LMatrix& matrix );
	virtual const LMatrix* getMatrixArray();
	virtual void setMatrixArray( const LMatrix* matrices, lnU32 count );
	virtual Texture* getTexture();
	virtual void setTexture( Texture* texture );
	//virtual void setTexture( ITexture* texture, const LSamplerState& state );
	virtual const char* getString();
	virtual void setString( const char* str );
	virtual IShaderVariable* getAnnotationByIndex( lnU32 index );
	virtual IShaderVariable* getAnnotationByName( const char* name );

private:
	DX9Shader*					mShader;
    ID3DXEffect*                mDxEffect;
    D3DXHANDLE                  mHandle;
	//LRefTString                 mName;
	//LRefTString                 mSemanticName;
    DX9ShaderAnnotationArray	mAnnotationArray;
    DX9ShaderAnnotationMap		mAnnotationMap;
    LNShaderVariableTypeDesc    mVariableTypeDesc;
    TextureParam*               mTextureParam;      ///< (テクスチャ型の場合のみ作成)

};

//==============================================================================
// ■ DX9ShaderAnnotation
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class DX9ShaderAnnotation
    : public ShaderVariableBase
{
public:
	DX9ShaderAnnotation( DX9Shader* shader, ID3DXEffect* dxEffect, D3DXHANDLE handle );
	virtual ~DX9ShaderAnnotation();

public:
	virtual bool getBool();
	virtual int getInt();
    virtual lnFloat getFloat();
	virtual const LVector4& getVector();
	virtual const LVector4* getVectorArray();
    virtual const LMatrix& getMatrix();
	virtual const LMatrix* getMatrixArray();

	virtual IShaderVariable* getAnnotationByIndex( lnU32 index ) { return NULL; }
	virtual IShaderVariable* getAnnotationByName( const char* name ) { return NULL; }

private:
    DX9Shader*					mShader;
    ID3DXEffect*                mDxEffect;
    D3DXHANDLE                  mHandle;
	//LRefTString                 mName;
    LNShaderVariableTypeDesc    mVariableTypeDesc;
	//LRefTString                 mString;            ///< String 型のアノテーションの場合の文字列値 (wchar_t 取得時の高速化用)

};

//==============================================================================
// ■ DX9ShaderTechnique
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class DX9ShaderTechnique
    :/* public Base::ReferenceObject
    ,*/ public IShaderTechnique
{
public:
	DX9ShaderTechnique( DX9Shader* shader, ID3DXEffect* dxEffect, D3DXHANDLE tech );
	virtual ~DX9ShaderTechnique();
	//LN_REFOBJ_METHODS;

public:
    virtual const lnChar*		getName() const { return mName.c_str(); }
	virtual lnU32				getPasses() const { return mShaderPassArray.size(); }
    virtual IShaderPass*		getPassByName( const lnChar* name );
	virtual IShaderPass*		getPassByIndex( lnU32 index );
	virtual IShaderVariable*	getAnnotationByName( const lnChar* name );
	virtual IShaderVariable*	getAnnotationByIndex( lnU32 index );

private:
	DX9Shader*					mShader;
    ID3DXEffect*				mDxEffect;
    D3DXHANDLE					mTechnique;
    lnU32						mPassCount;	// いらないかも
    lnRefString					mName;

    DX9ShaderPassArray			mShaderPassArray;
    DX9ShaderPassMap			mShaderPassMap;
    DX9ShaderAnnotationArray	mAnnotationArray;
    DX9ShaderAnnotationMap		mAnnotationMap;
};

//==============================================================================
// ■ DX9ShaderPass
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class DX9ShaderPass
    : /*public Base::ReferenceObject
    , */public IShaderPass
{
public:
	DX9ShaderPass( DX9Shader* shader, ID3DXEffect* dxEffect, D3DXHANDLE handle, lnU32 passIndex, D3DXHANDLE tech );
	virtual ~DX9ShaderPass();
	//LN_REFOBJ_METHODS;

public:
    virtual const lnChar* getName() const { return mName.c_str(); }
	virtual IShaderVariable* getAnnotationByName( const lnChar* name );
	virtual IShaderVariable* getAnnotationByIndex( lnU32 index );
	virtual void begin();
	virtual void end();
	virtual void commit();

private:
	DX9Shader*					mShader;
    ID3DXEffect*				mDxEffect;
    D3DXHANDLE					mHandle;
    D3DXHANDLE					mTechnique;
    lnU32						mPassIndex;
    lnRefString					mName;
    DX9ShaderAnnotationArray	mAnnotationArray;
    DX9ShaderAnnotationMap		mAnnotationMap;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================