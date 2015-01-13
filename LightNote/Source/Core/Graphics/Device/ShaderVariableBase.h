//==============================================================================
// ShaderVariableBase 
//------------------------------------------------------------------------------
///**
//  @file       ShaderVariableBase.h
//  @brief      ShaderVariableBase
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../Interface.h"
#include "../Common/Types.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ ShaderVariableBase
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ShaderVariableBase
    : /*public Base::ReferenceObject
    , */public IShaderVariable
{
public:
    ShaderVariableBase();
    virtual ~ShaderVariableBase();
    LN_REFOBJ_METHODS;

protected:

	/// 初期化
	void initialize( 
		ShaderVariableType type, 
		lnRefString name, 
		lnRefString semanticName,
		LNShaderVariableTypeDesc desc );

	/// initialize() 後、一度でも何かが設定されたか
	bool isModifid() const { return mModifid; }

	/// デバイス停止前処理
	void onPauseDevice();

	/// デバイス再開後処理 (再度 set～ を呼ぶ)
    void onResumeDevice();

public:
	virtual ShaderVariableType getType() const { return mType; }
	virtual const char* getName() const { return mName.c_str(); }
	virtual const char* getSemanticName() const { return mSemanticName.c_str(); }
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
	virtual void setTexture( Texture* texture, const LSamplerState& state );
	virtual const char* getString();
	virtual void setString( const char* str );
	virtual const LNShaderVariableTypeDesc& getTypeDesc() const { return mDesc; }

protected:
	void _allocValueBuffer( int byteCount );

private:

	/// 変数値
	struct Value
	{
		union
		{
			bool		Bool;
			int			Int;
			lnFloat		Float;
			lnFloat*	VectorArray;	// 普通のVector(Matrix) もこれを使用
			Texture*	Texture;
			char*		String;
		};
		int ByteCount;
	} ;
  
protected:

	ShaderVariableType			mType;
	lnRefString					mName;
	lnRefString					mSemanticName;
	Value						mValue;
	bool						mModifid;	///< initialize() 後、一度でも何かが設定された
	LNShaderVariableTypeDesc	mDesc;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================