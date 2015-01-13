//==============================================================================
// ShaderVariableBase 
//==============================================================================

#include "stdafx.h"
#include "../DeviceObjects.h"
#include "ShaderVariableBase.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ ShaderVariableBase
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ShaderVariableBase::ShaderVariableBase()
		: mType		( SVT_UNKNOWN )
		, mModifid	( false )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ShaderVariableBase::~ShaderVariableBase()
	{
		switch ( mType )
		{
			case SVT_VECTOR:
			case SVT_VECTORARRAY:
			{
				SAFE_DELETE_ARRAY( mValue.VectorArray );
				break;
			}
			case SVT_TEXTURE:
			{
				LN_SAFE_RELEASE( mValue.Texture );
				break;
			}
			case SVT_STRING:
			{
				SAFE_DELETE_ARRAY( mValue.String );
				break;
			}
		}
	}
	
#ifdef LNOTE_FOR_200
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ShaderVariableBase::initialize( 
		ShaderVariableType type, 
		lnRefString name, 
		lnRefString semanticName,
		LNShaderVariableTypeDesc desc )
	{
		mType			= type;
		mName			= name;
		mSemanticName	= semanticName;
		mDesc			= desc;
		memset( &mValue, 0, sizeof(mValue) );
		mModifid = false;
	}
#else
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ShaderVariableBase::initialize( ShaderVariableType type )
	{
		mType = type;
		memset( &mValue, 0, sizeof(mValue) );
		mModifid = false;
	}
#endif
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ShaderVariableBase::onPauseDevice()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ShaderVariableBase::onResumeDevice()
	{
		switch ( mType )
		{
			case SVT_BOOL:			setBool( mValue.Bool ); break;
			case SVT_INT:			setInt( mValue.Int ); break;
			case SVT_FLOAT:			setFloat( mValue.Float ); break;
			case SVT_VECTOR:		setVector( *((LVector4*)mValue.VectorArray) ); break;
			case SVT_VECTORARRAY:	setVectorArray( ((LVector4*)mValue.VectorArray), mValue.ByteCount / sizeof(LVector4) ); break;
			case SVT_MATRIX:		setMatrix( *((LMatrix*)mValue.VectorArray) ); break;
			case SVT_MATRIXARRAY:	setMatrixArray( ((LMatrix*)mValue.VectorArray), mValue.ByteCount / sizeof(LMatrix) ); break;
			case SVT_TEXTURE:		setTexture( mValue.Texture ); break;
			case SVT_STRING:		setString( mValue.String ); break;
		}
	}
	
#ifdef LNOTE_FOR_200
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool ShaderVariableBase::getBool()
	{
		LN_THROW_InvalidOperation(mType == SVT_BOOL);
		return mValue.Bool;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ShaderVariableBase::setBool( bool b )
	{
		LN_THROW_InvalidOperation(mType == SVT_BOOL);
		mValue.Bool = b;
		mModifid = true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	int ShaderVariableBase::getInt()
	{
		LN_THROW_InvalidOperation(mType == SVT_INT);
		return mValue.Int;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ShaderVariableBase::setInt( int value )
	{
		LN_THROW_InvalidOperation(mType == SVT_INT);
		mValue.Int = value;
		mModifid = true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	lnFloat ShaderVariableBase::getFloat()
	{
		LN_THROW_InvalidOperation(mType == SVT_FLOAT);
		return mValue.Float;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ShaderVariableBase::setFloat( lnFloat value )
	{
		LN_THROW_InvalidOperation(mType == SVT_FLOAT);
		mValue.Float = value;
		mModifid = true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	const LVector4& ShaderVariableBase::getVector()
	{
		LN_THROW_InvalidOperation(mType == SVT_VECTOR);
		return *((const LVector4*)mValue.VectorArray);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ShaderVariableBase::setVector( const LVector4& vec )
	{
		LN_THROW_InvalidOperation(mType == SVT_VECTOR);
		_allocValueBuffer( sizeof(LVector4) );
		memcpy( mValue.VectorArray, &vec, sizeof(LVector4) );
		mModifid = true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	const LVector4* ShaderVariableBase::getVectorArray()
	{
		LN_THROW_InvalidOperation(mType == SVT_VECTOR);
		return (const LVector4*)mValue.VectorArray;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ShaderVariableBase::setVectorArray( const LVector4* vec, lnU32 count )
	{
		LN_THROW_InvalidOperation(mType == SVT_VECTORARRAY);
		_allocValueBuffer( sizeof(LVector4) * count );
		memcpy( mValue.VectorArray, vec, sizeof(LVector4) * count );
		mModifid = true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	const LMatrix& ShaderVariableBase::getMatrix()
	{
		LN_THROW_InvalidOperation(mType == SVT_MATRIX);
		return *((const LMatrix*)mValue.VectorArray);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ShaderVariableBase::setMatrix( const LMatrix& matrix )
	{
		LN_THROW_InvalidOperation(mType == SVT_MATRIX);
		_allocValueBuffer( sizeof(LMatrix) );
		memcpy( mValue.VectorArray, &matrix, sizeof(LMatrix) );
		mModifid = true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	const LMatrix* ShaderVariableBase::getMatrixArray()
	{
		LN_THROW_InvalidOperation(mType == SVT_MATRIXARRAY);
		return (const LMatrix*)mValue.VectorArray;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ShaderVariableBase::setMatrixArray( const LMatrix* matrices, lnU32 count )
	{
		LN_THROW_InvalidOperation(mType == SVT_MATRIXARRAY);
		_allocValueBuffer( sizeof(LMatrix) * count );
		memcpy( mValue.VectorArray, matrices, sizeof(LMatrix) * count );
		mModifid = true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Texture* ShaderVariableBase::getTexture()
	{
		LN_THROW_InvalidOperation(mType == SVT_TEXTURE);
		return mValue.Texture;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ShaderVariableBase::setTexture( Texture* texture )
	{
		LN_THROW_InvalidOperation(mType == SVT_TEXTURE);
		LN_REFOBJ_SET( mValue.Texture, texture );
		mModifid = true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ShaderVariableBase::setTexture( Texture* texture, const LSamplerState& state )
	{
		// TODO:サンプラステートの記憶
		LN_THROW_NotImpl( 0 );

		LN_THROW_InvalidOperation(mType == SVT_TEXTURE);
		LN_REFOBJ_SET( mValue.Texture, texture );
		mModifid = true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	const char* ShaderVariableBase::getString()
	{
		LN_THROW_InvalidOperation(mType == SVT_STRING);
		return mValue.String;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ShaderVariableBase::setString( const char* str )
	{
		LN_THROW_InvalidOperation(mType == SVT_STRING);
		SAFE_DELETE_ARRAY( mValue.String );
		mValue.String = Base::StringUtils::copyLength( str, -1 );
		mModifid = true;
	}
#else
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNRESULT ShaderVariableBase::getBool( bool* b )
	{
		LN_THROW_InvalidCall( mType == SVT_BOOL );
		*b = mValue.Bool;
		return LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNRESULT ShaderVariableBase::setBool( bool b )
	{
		LN_THROW_InvalidCall( mType == SVT_BOOL );
		mValue.Bool = b;
		mModifid = true;
		return LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNRESULT ShaderVariableBase::getInt( int* value )
	{
		LN_THROW_InvalidCall( mType == SVT_INT );
		*value = mValue.Int;
		return LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNRESULT ShaderVariableBase::setInt( int value )
	{
		LN_THROW_InvalidCall( mType == SVT_INT );
		mValue.Int = value;
		mModifid = true;
		return LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNRESULT ShaderVariableBase::getFloat( lnFloat* value )
	{
		LN_THROW_InvalidCall( mType == SVT_FLOAT );
		*value = mValue.Float;
		return LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNRESULT ShaderVariableBase::setFloat( lnFloat value )
	{
		LN_THROW_InvalidCall( mType == SVT_FLOAT );
		mValue.Float = value;
		mModifid = true;
		return LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNRESULT ShaderVariableBase::getVector( LVector4* vec )
	{
		LN_THROW_InvalidCall( mType == SVT_VECTOR );
		_allocValueBuffer( sizeof(LVector4) );
		memcpy( vec, mValue.VectorArray, sizeof(LVector4) );
		return LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNRESULT ShaderVariableBase::setVector( const LVector4& vec )
	{
		LN_THROW_InvalidCall( mType == SVT_VECTOR );
		_allocValueBuffer( sizeof(LVector4) );
		memcpy( mValue.VectorArray, &vec, sizeof(LVector4) );
		mModifid = true;
		return LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNRESULT ShaderVariableBase::getVectorArray( LVector4* vec, lnU32 count )
	{
		LN_THROW_InvalidCall( mType == SVT_VECTOR );
		_allocValueBuffer( sizeof(LVector4) * count );
		memcpy( vec, mValue.VectorArray, sizeof(LVector4) * count );
		return LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNRESULT ShaderVariableBase::setVectorArray( const LVector4* vec, lnU32 count )
	{
		LN_THROW_InvalidCall( mType == SVT_VECTORARRAY );
		_allocValueBuffer( sizeof(LVector4) * count );
		memcpy( mValue.VectorArray, vec, sizeof(LVector4) * count );
		mModifid = true;
		return LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNRESULT ShaderVariableBase::getMatrix( LMatrix* matrix )
	{
		LN_THROW_InvalidCall( mType == SVT_MATRIX );
		_allocValueBuffer( sizeof(LMatrix) );
		memcpy( matrix, mValue.VectorArray, sizeof(LMatrix) );
		return LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNRESULT ShaderVariableBase::setMatrix( const LMatrix& matrix )
	{
		LN_THROW_InvalidCall( mType == SVT_MATRIX );
		_allocValueBuffer( sizeof(LMatrix) );
		memcpy( mValue.VectorArray, &matrix, sizeof(LMatrix) );
		mModifid = true;
		return LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNRESULT ShaderVariableBase::getMatrixArray( LMatrix* matrices, lnU32 count )
	{
		LN_THROW_InvalidCall( mType == SVT_MATRIXARRAY );
		_allocValueBuffer( sizeof(LMatrix) * count );
		memcpy( matrices, mValue.VectorArray, sizeof(LMatrix) * count );
		return LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNRESULT ShaderVariableBase::setMatrixArray( const LMatrix* matrices, lnU32 count )
	{
		LN_THROW_InvalidCall( mType == SVT_MATRIXARRAY );
		_allocValueBuffer( sizeof(LMatrix) * count );
		memcpy( mValue.VectorArray, matrices, sizeof(LMatrix) * count );
		mModifid = true;
		return LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ITexture* ShaderVariableBase::getTexture()
	{
		LN_THROW_InvalidCall( mType == SVT_TEXTURE );
		return mValue.Texture;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNRESULT ShaderVariableBase::setTexture( ITexture* texture )
	{
		LN_THROW_InvalidCall( mType == SVT_TEXTURE );
		LN_REFOBJ_SET( mValue.Texture, texture );
		mModifid = true;
		return LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNRESULT ShaderVariableBase::setTexture( ITexture* texture, const LSamplerState& state )
	{
		// TODO:サンプラステートの記憶
		LN_THROW_InvalidCall( mType == SVT_TEXTURE );
		LN_REFOBJ_SET( mValue.Texture, texture );
		mModifid = true;
		return LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNRESULT ShaderVariableBase::getString( const char** str )
	{
		LN_THROW_InvalidCall( mType == SVT_STRING );
		*str = mValue.String;
		return LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNRESULT ShaderVariableBase::setString( const char* str )
	{
		LN_THROW_InvalidCall( mType == SVT_STRING );
		SAFE_DELETE_ARRAY( mValue.String );
		mValue.String = Base::StringUtil::copyLength( str, -1 );
		mModifid = true;
		return LN_OK;
	}
#endif

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ShaderVariableBase::_allocValueBuffer( int byteCount )
	{
		if ( byteCount > mValue.ByteCount )
		{
			SAFE_DELETE_ARRAY( mValue.VectorArray );
			mValue.VectorArray = reinterpret_cast<lnFloat*>( LN_NEW lnByte[byteCount] );
			mValue.ByteCount = byteCount;
			memset( mValue.VectorArray, 0, byteCount );
		}
	}


} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================