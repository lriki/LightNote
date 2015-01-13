//==============================================================================
// DX9Shader 
//------------------------------------------------------------------------------
/*	[13/12/22] メモリバッファからの読み込みはサポートしない
		今のところ以下のような問題がある。
		・#include 対応のためのカレントディレクトリの管理は誰がやるの？
		・OpenGL ES の時、デバイスロストしたときの再構築に、シェーダソースが必要。
		  ずっとメンバに持っててもいいけど、メモリが無駄になる。
		特に後者は重要で、テクスチャも同じ扱いにする必要がありそう。
			…でも、そうなると頂点バッファ・インデックスバッファもデータ持っておく必要がある…。
			→やっぱり、メモリデータごとバックアップ持っておいた方が良いかもしれない。
			  マルチスレッド完璧に対応するにはダブルバッファ的なものが必要になって来るし…。

		⇒とりあえず本件については、カレントディレクトリに限らず.lnfx の読み方をどうするとか
		  ファイル名だけの方が都合がいいいので、今のところサポートしない。
		  エラーもわかりやすいのが出せるはず。
		  要望あれば警告したうえで実装する。

	[14/2/12] やっぱりメモリバッファから読みたい
		それにその方が自然な気がする…。
		それに、Android とかフレームワークに則って作るなら、
		onResume() とかで onPause()時点の状態に復帰させる のはプログラマの責任。
		でも、onStop() は完全にメモリ上から削除されるときだからこれは使う人に任せないとダメだけど、
		デバイスリソースくらいはライブラリ側で面倒見たい…。
		ってなると、ヒープにバックアップ取っておく必要がある。
		テクスチャなら、ビデオメモリとヒープ両方に同じものを置いておく。
		…いっそこの方が良い気がする。メモリはすごい使うと思うけど、
		携帯機がメモリ少な目なことくらい開発者なら知ってるはずだし…。

	[14/11/23] エラー処理について
		他のエンジンとかは…
		・Unity … 動的コンパイルできないみたい。
		・Irrihit … ログ書き出した後 return false;
		・GamePlay … ログ書き出した後 throw
		・XNA … CompledEffect 構造体を返す。これがエラーを持っている。
		・Cg … Context (Manager) が、最後に発生したエラーを持っている。
		・SlimDX … throw CompilationException
		・Esenthel Engine
			ShaderCompileTry … return false;
			ShaderCompile … throw

		エラーは文字列として返したいし、警告の場合も考えないとダメ。
		ログファイルへの出力はあり得ないし、Cg みたいにスレッドセーフにできないのもNG。
		とすると SlimDX を参考にするのが素直か…。
*/
//==============================================================================

#include "stdafx.h"
#include "../../../Base/Parser.h"
#include "../../../FileIO/Manager.h"
#include "../../../FileIO/InFile.h"
#include "../OpenGL/GLShader.h"
#include "../LNFXManager.h"
#include "../../CompilationException.h"
#include "DX9Module.h"
#include "DX9GraphicsDevice.h"
#include "DX9Texture.h"
#include "DX9Shader.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ D3DXInclude
//==============================================================================
class D3DXInclude
	: public ID3DXInclude
{
public:

	D3DXInclude( FileIO::Manager* manager )
	{
		mIOManager = manager;
	}

	STDMETHOD(Open)(THIS_ 
		D3DXINCLUDE_TYPE IncludeType,
		LPCSTR pFileName,
		LPCVOID pParentData,
		LPCVOID * ppData,
		UINT * pBytes)
	{
		LRefPtr<FileIO::InFile> file( mIOManager->createInFile( pFileName ) );

		lnU32 size = file->getSize();
		lnByte* data = LN_NEW lnByte[size];
		file->read( data, size );

		*ppData = data;
		*pBytes = size;
		return S_OK;
	}

	STDMETHOD(Close)(THIS_ LPCVOID pData )
	{
		lnByte* data = (lnByte*)pData;
		SAFE_DELETE_ARRAY( data );
		return S_OK;
	}

private:
	
	FileIO::Manager*	mIOManager;
};

//==============================================================================
// ■ DX9Shader
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9Shader::DX9Shader()
		: mGraphicsDevice	( NULL )
        , mDxEffect         ( NULL )
        , mCompileResult    ( LN_SHADERRESULT_ERROR )
		, mVariableCount	( 0 )
		, mTechniqueCount	( 0 )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9Shader::~DX9Shader()
    {
        // ShaderTechnique 解放
        for ( lnU32 i = 0; i < mShaderTechniqueArray.size(); ++i ) {
            SAFE_DELETE( this->mShaderTechniqueArray[i] );
        }

        // ShaderVariable 解放
        for ( lnU32 i = 0; i < mShaderVariableArray.size(); ++i ) {
            SAFE_DELETE( this->mShaderVariableArray[i] );
        }

        SAFE_RELEASE( this->mDxEffect );
        LN_SAFE_RELEASE( this->mGraphicsDevice );
    }

#if 0
	void printContantTable( ID3DXConstantTable* ct )
	{
		#pragma comment(lib, "d3dx9.lib")
		D3DXHANDLE param;
        D3DXCONSTANT_DESC desc;
        for (int i = 0; (param = ct->GetConstant(NULL, i)) != NULL; i++)
        {
            UINT count;
            ct->GetConstantDesc(param, &desc, &count);
			printf("      Name	:%s\n", desc.Name);
			printf("      Count	:%u\n", count);

            
        }
	}
#endif

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//void DX9Shader::create( DX9GraphicsDevice* device, const lnChar* filePath )//const char* code, int size )
	//void DX9Shader::create( DX9GraphicsDevice* device, const char* code, int size )
	void DX9Shader::create(DX9GraphicsDevice* device, FileIO::Stream* stream, lnString* errors)
	{
		LN_REFOBJ_SET( this->mGraphicsDevice, device );

		// 最初に成功にしておく
        this->mCompileResult = LN_SHADERRESULT_SUCCESS;

		// ストリームからコードを読み込む
		LNFXManager lnfxManager;
		lnfxManager.open( stream, LN_GRAPHICSAPI_DIRECTX9, false, NULL );	//TODO: とりあえず true 固定
		const char* hlslCode = lnfxManager.getHLSLCode().c_str();
		size_t hlslCodeSize = lnfxManager.getHLSLCode().length();

		// UTF-8のBOM対策
		//if ( *hlslCode == 0xEF ) { hlslCode++; hlslCodeSize--; }
		//if ( *hlslCode == 0xBB ) { hlslCode++; hlslCodeSize--; }
		//if ( *hlslCode == 0xBF ) { hlslCode++; hlslCodeSize--; }

		//---------------------------------------------------------
        // シェーダプログラムのコンパイル

		// 定義マクロ
        D3DXMACRO deflist[] =
        {
            { LNFXManager::LNoteHLDLMacro, "1" },
	        { NULL, NULL },
        };

		
		D3DXInclude dxInclude( device->getIOManager() );
        ID3DXBuffer* buffer = NULL;
        HRESULT hr = DX9Module::getInstance()->D3DXCreateEffect(
			device->getIDirect3DDevice9(),
			hlslCode,//analyzer.getOutput().c_str(),//source.getSourceData(),
			hlslCodeSize,//analyzer.getOutput().size(),//source.getSourceDataSize(),
            deflist,
            &dxInclude,

            D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY,	// これがないと古いシェーダをコンパイルしたときにエラーが出る可能性がある (X3025 とか)
            device->getD3DXEffectPool(),
            &mDxEffect,
            &buffer );
        // 失敗
        if ( FAILED( hr ) && buffer )
		{
            mCompileResult = LN_SHADERRESULT_ERROR;
			char* c = static_cast<char*>( buffer->GetBufferPointer() );
            mCompileErrors = c;
            SAFE_RELEASE( buffer );

			lnString str = _T("shader program Compile error.\n\n");
			str += mCompileErrors.c_str();
			if (errors)
				*errors = str;
			LN_THROW(0, CompilationException, str.c_str());
		}
        // 警告がある
		else if ( buffer )
		{
            mCompileResult = LN_SHADERRESULT_WARNING;
            char* c = static_cast<char*>( buffer->GetBufferPointer() );
            mCompileErrors = c;
            SAFE_RELEASE( buffer );
		}

		//---------------------------------------------------------
        // すべての変数を列挙
        D3DXHANDLE handle;
        UINT idx = 0;
        while ( true )
        {
            handle = mDxEffect->GetParameter( NULL, idx );
            if ( !handle ) break;

            DX9ShaderVariable* v = LN_NEW DX9ShaderVariable( this, mDxEffect, handle );
            mShaderVariableMap.insert( DX9ShaderVariablePair( v->getName(), v ) );
            mShaderVariableArray.push_back( v );
            ++idx;

			// テクスチャ型の場合はサンプラ設定用リストにも分けておく
            //if ( v->isTextureType() ) {
            //    mTextureVariableArray.push_back( v );
            //}
        }

		//---------------------------------------------------------
        // すべてのテクニックを列挙
        D3DXHANDLE tech = NULL;
        D3DXHANDLE next = NULL;
        do
        {
            mDxEffect->FindNextValidTechnique( tech, &next );
            if ( next )
            {
                DX9ShaderTechnique* t = LN_NEW DX9ShaderTechnique( this, mDxEffect, next );
                mShaderTechniqueMap.insert( DX9ShaderTechniquePair( t->getName(), t ) );
                mShaderTechniqueArray.push_back( t );
            }
            tech = next;
        } while ( tech );

		GraphicsResource::registerResource( device );


#if 0
		// パラメータ一覧
		// sampler も取れる
		idx = 0;
		while ( true )
        {
            handle = mDxEffect->GetParameter( NULL, idx );
            if ( !handle ) break;

			D3DXPARAMETER_DESC desc;
			mDxEffect->GetParameterDesc( handle, &desc );
			printf( "Name:%s\n", desc.Name );

			++idx;
		}

		// テクニック一覧
		D3DXHANDLE tech = NULL;
        D3DXHANDLE next = NULL;
        do
        {
            mDxEffect->FindNextValidTechnique( tech, &next );

			if ( next )
			{
				D3DXTECHNIQUE_DESC tech_desc;
				mDxEffect->GetTechniqueDesc( next, &tech_desc );
				
			
				printf( "TechName:%s\n", tech_desc.Name );

				// パス
				D3DXHANDLE pass;
				for ( lnU32 i = 0; i < tech_desc.Passes; ++i )
				{
					pass = mDxEffect->GetPass( next, i );
					if ( pass ) // 念のため
					{
						D3DXPASS_DESC pass_desc;
						mDxEffect->GetPassDesc( pass, &pass_desc );
						printf( "    Pass\n" );
						printf( "    Name:%s\n", pass_desc.Name );

						if ( pass_desc.pVertexShaderFunction ) {
							printf( "      VertexShader\n" );
							ID3DXConstantTable* ct;
							D3DXGetShaderConstantTable( (const DWORD*)pass_desc.pVertexShaderFunction, &ct );
							printContantTable( ct );
						}
						if ( pass_desc.pPixelShaderFunction ) {
							printf( "      PixelShader\n" );
							ID3DXConstantTable* ct;
							D3DXGetShaderConstantTable( (const DWORD*)pass_desc.pPixelShaderFunction, &ct );
							printContantTable( ct );
						}

					}
				}
			}

			
            tech = next;
        } while ( tech );

		//device->getIDirect3DDevice9()->GetSamplerState(
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IShaderVariable* DX9Shader::getVariableByName( const char* name )
	{
        DX9ShaderVariableMap::iterator it = mShaderVariableMap.find( name );
        if ( it != mShaderVariableMap.end() ) {
            return it->second;
        }
        return NULL;
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IShaderVariable* DX9Shader::getVariableByIndex( lnU32 index )
	{
        if ( index >= mShaderVariableArray.size() ) return NULL;
        return mShaderVariableArray[index];
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IShaderTechnique* DX9Shader::getTechniqueByName( const char* name )
	{
        DX9ShaderTechniqueMap::iterator it = mShaderTechniqueMap.find( name );
        if ( it != mShaderTechniqueMap.end() ) {
            return it->second;
        }
        return NULL;
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	IShaderTechnique* DX9Shader::getTechniqueByIndex( lnU32 index )
	{
        if ( index >= mShaderTechniqueArray.size() ) return NULL;
        return mShaderTechniqueArray[index];
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9Shader::onPauseDevice()
	{
		HRESULT hr;
		LN_COMCALL( mDxEffect->OnLostDevice() );
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9Shader::onResumeDevice()
	{
		HRESULT hr;
		LN_COMCALL( mDxEffect->OnResetDevice() );
	}


//==============================================================================
// ■ DX9ShaderVariable
//==============================================================================

//---------------------------------------------------------------------
// ● 変数クラスと変数型の変換
//---------------------------------------------------------------------
void D3DXParamDescToLNParamDesc(
    const D3DXPARAMETER_DESC&   dx_desc_,
    LNShaderVariableClass*      ln_class_,
    LNShaderVariableType*       ln_type_,
	ShaderVariableType*			type )
{
	*type = SVT_UNKNOWN;

	// 変数クラス (LN_SVC_SAMPLER は Type がテクスチャまたはサンプラ型のときこの次の switch でセットする)
    switch ( dx_desc_.Class )
    {
        case D3DXPC_SCALAR:         
        	*ln_class_ = LN_SVC_SCALAR; break;
        case D3DXPC_VECTOR:         
        	*ln_class_ = LN_SVC_VECTOR; break;
        case D3DXPC_MATRIX_ROWS:    
        	*ln_class_ = LN_SVC_MATRIX; break;
        case D3DXPC_MATRIX_COLUMNS: 
        	*ln_class_ = LN_SVC_MATRIX; break;
        case D3DXPC_OBJECT:         
        	*ln_class_ = LN_SVC_OBJECT; break;
        case D3DXPC_STRUCT:         
        	*ln_class_ = LN_SVC_STRUCT; break;
        //                                       LN_SVC_ARRAY;
        //                                       LN_SVC_SAMPLER;
        //                                       LN_SVC_BUFFER;
        default:                    
        	*ln_class_ = LN_SVC_UNKNOWN; break;
    }

	// 変数型
    switch ( dx_desc_.Type )
    {
        case D3DXPT_VOID:           
        	*ln_type_ = LN_SVT_VOID; break;
        case D3DXPT_BOOL:           
        	*ln_type_ = LN_SVT_BOOL; 
			*type = SVT_BOOL;
			break;
        case D3DXPT_INT:            
        	*ln_type_ = LN_SVT_INT;
			*type = SVT_INT;
			break;
        case D3DXPT_FLOAT:          
        	*ln_type_ = LN_SVT_FLOAT;
			*type = SVT_FLOAT;
			break;
        case D3DXPT_STRING:         
        	*ln_type_ = LN_SVT_STRING;
			*type = SVT_STRING;
			break;
        case D3DXPT_TEXTURE:        
        	*ln_type_ = LN_SVT_TEXTURE;
			*type = SVT_TEXTURE;
			break;
        case D3DXPT_TEXTURE1D:      
        	*ln_type_ = LN_SVT_TEXTURE1D;
			*type = SVT_TEXTURE;
			break;
        case D3DXPT_TEXTURE2D:      
        	*ln_type_ = LN_SVT_TEXTURE2D;
			*type = SVT_TEXTURE;
			break;
        case D3DXPT_TEXTURE3D:      
        	*ln_type_ = LN_SVT_TEXTURE3D;
			*type = SVT_TEXTURE;
			break;
        case D3DXPT_TEXTURECUBE:    
        	*ln_type_ = LN_SVT_TEXTURECUBE;
			*type = SVT_TEXTURE;
			break;
        case D3DXPT_SAMPLER:        
        	*ln_type_ = LN_SVT_SAMPLER;
			*type = SVT_TEXTURE;//SVT_SAMPLER;
			break;
        case D3DXPT_SAMPLER1D:      
        	*ln_type_ = LN_SVT_SAMPLER1D;
			*type = SVT_TEXTURE;//SVT_SAMPLER;
			break;
        case D3DXPT_SAMPLER2D:      
        	*ln_type_ = LN_SVT_SAMPLER2D;
			*type = SVT_TEXTURE;//SVT_SAMPLER;
			break;
        case D3DXPT_SAMPLER3D:      
        	*ln_type_ = LN_SVT_SAMPLER3D;
			*type = SVT_TEXTURE;//SVT_SAMPLER;
			break;
        case D3DXPT_SAMPLERCUBE:    
        	*ln_type_ = LN_SVT_SAMPLERCUBE;
			*type = SVT_TEXTURE;//SVT_SAMPLER;
			break;
        case D3DXPT_PIXELSHADER:    
        	*ln_type_ = LN_SVT_PIXELSHADER; break;
        case D3DXPT_VERTEXSHADER:   
        	*ln_type_ = LN_SVT_VERTEXSHADER; break;
        case D3DXPT_PIXELFRAGMENT:  
        	*ln_type_ = LN_SVT_UNKNOWN; break;
        case D3DXPT_VERTEXFRAGMENT: 
        	*ln_type_ = LN_SVT_UNKNOWN; break;
        case D3DXPT_UNSUPPORTED:    
        	*ln_type_ = LN_SVT_UNKNOWN; break;
        default:                    
        	*ln_type_ = LN_SVT_UNKNOWN; break;
    }
}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9ShaderVariable::DX9ShaderVariable( DX9Shader* shader, ID3DXEffect* dxEffect, D3DXHANDLE handle )
		: mShader       ( shader )
        , mDxEffect     ( dxEffect )
        , mHandle       ( handle )
        , mTextureParam ( NULL )
	{
		// 変数名とセマンティクス名
        D3DXPARAMETER_DESC desc;
        mDxEffect->GetParameterDesc( mHandle, &desc );
        //mName         = LRefTString( desc.Name );
        //mSemanticName = LRefTString( desc.Semantic );

        // 変数情報
		ShaderVariableType type;
        D3DXParamDescToLNParamDesc(
            desc, 
            &mVariableTypeDesc.Class,
            &mVariableTypeDesc.Type,
			&type );
        mVariableTypeDesc.Rows      = desc.Rows;
        mVariableTypeDesc.Columns   = desc.Columns;
        mVariableTypeDesc.Elements  = desc.Elements;
        mVariableTypeDesc.Shared    = ( desc.Flags & D3DX_PARAMETER_SHARED );

		if ( type == SVT_FLOAT )
		{
			if      ( mVariableTypeDesc.Class == LN_SVC_VECTOR ) type = SVT_VECTOR;
			else if ( mVariableTypeDesc.Class == LN_SVC_MATRIX ) type = SVT_MATRIX;

			if ( mVariableTypeDesc.Elements > 0 )
			{
				if      ( type == SVT_VECTOR ) type = SVT_VECTORARRAY;
				else if ( type == SVT_MATRIX ) type = SVT_MATRIXARRAY;
			}
		}

		ShaderVariableBase::initialize( type, lnRefString( desc.Name ), lnRefString( desc.Semantic ), mVariableTypeDesc );

        // すべてのアノテーションを配列と map に格納
        D3DXHANDLE anno;
        for ( lnU32 i = 0; i < desc.Annotations; ++i )
        {
            anno = mDxEffect->GetAnnotation( mHandle, i );
            if ( anno ) // 念のため
            {
                DX9ShaderAnnotation* v = LN_NEW DX9ShaderAnnotation( mShader, mDxEffect, anno );
                mAnnotationMap.insert( DX9ShaderAnnotationPair( v->getName(), v ) );
                mAnnotationArray.push_back( v );
            }
        }

        // テクスチャ型の場合はテクスチャとサンプラステートを記憶する構造体を作る
        if ( LN_SVT_TEXTURE_BEGIN <= mVariableTypeDesc.Type && mVariableTypeDesc.Type <= LN_SVT_TEXTURE_END )
        {
            mTextureParam = LN_NEW TextureParam;
            mTextureParam->Texture = NULL;
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9ShaderVariable::~DX9ShaderVariable()
    {
        for ( lnU32 i = 0; i < mAnnotationArray.size(); ++i ) {
            SAFE_DELETE( mAnnotationArray[i] );
        }

        SAFE_DELETE( mTextureParam );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool DX9ShaderVariable::getBool()
    {
        HRESULT hr;
        BOOL b;
        LN_COMCALL( mDxEffect->GetBool( mHandle, &b ) );
        return ( b != FALSE );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9ShaderVariable::setBool( bool b )
    {
        HRESULT hr;
        LN_COMCALL( mDxEffect->SetBool( mHandle, b ) );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    int DX9ShaderVariable::getInt()
    {
        HRESULT hr;
		int v;
        LN_COMCALL( mDxEffect->GetInt( mHandle, &v ) );
        return v;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9ShaderVariable::setInt( int value )
    {
        HRESULT hr;
        LN_COMCALL( mDxEffect->SetInt( mHandle, value ) );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    lnFloat DX9ShaderVariable::getFloat()
    {
        HRESULT hr;
		lnFloat v;
        LN_COMCALL( mDxEffect->GetFloat( mHandle, &v ) );
        return v;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9ShaderVariable::setFloat( lnFloat value )
    {
        HRESULT hr;
        LN_COMCALL( mDxEffect->SetFloat( mHandle, value ) );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    const LVector4& DX9ShaderVariable::getVector()
    {
        HRESULT hr;
		_allocValueBuffer( sizeof(LVector4) );
		LN_COMCALL( mDxEffect->GetVector( this->mHandle, (D3DXVECTOR4*)mValue.VectorArray ) );
        return *((LVector4*)mValue.VectorArray);
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9ShaderVariable::setVector( const LVector4& vec )
    {
        HRESULT hr;
        LN_COMCALL( mDxEffect->SetVector( mHandle, (D3DXVECTOR4*)&vec ) );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    const LVector4* DX9ShaderVariable::getVectorArray()
    {
        HRESULT hr;
		_allocValueBuffer( sizeof(LVector4) * mDesc.Elements );
		LN_COMCALL( mDxEffect->GetVectorArray( mHandle, (D3DXVECTOR4*)mValue.VectorArray, mDesc.Elements ) );
		return (LVector4*)mValue.VectorArray;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9ShaderVariable::setVectorArray( const LVector4* vec, lnU32 count )
    {
        HRESULT hr;
        LN_COMCALL( mDxEffect->SetVectorArray( mHandle, (const D3DXVECTOR4*)vec, count ) );
    }
    
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    const LMatrix& DX9ShaderVariable::getMatrix()
    {
        HRESULT hr;
		_allocValueBuffer( sizeof(LMatrix) );
		LN_COMCALL( mDxEffect->GetMatrix( this->mHandle, (D3DXMATRIX*)mValue.VectorArray ) );
        return *((LMatrix*)mValue.VectorArray);
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9ShaderVariable::setMatrix( const LMatrix& matrix )
    {
        HRESULT hr;
        LN_COMCALL( mDxEffect->SetMatrix( mHandle, (const D3DXMATRIX*)&matrix ) );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    const LMatrix* DX9ShaderVariable::getMatrixArray()
    {
        HRESULT hr;
		_allocValueBuffer( sizeof(LMatrix) * mDesc.Elements );
		LN_COMCALL( mDxEffect->GetMatrixArray( mHandle, (D3DXMATRIX*)mValue.VectorArray, mDesc.Elements ) );
		return (LMatrix*)mValue.VectorArray;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9ShaderVariable::setMatrixArray( const LMatrix* matrices, lnU32 count )
    {
        HRESULT hr;
        LN_COMCALL( mDxEffect->SetMatrixArray( mHandle, (const D3DXMATRIX*)matrices, count ) );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Texture* DX9ShaderVariable::getTexture()
	{
		return ShaderVariableBase::getTexture();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void DX9ShaderVariable::setTexture( Texture* texture )
    {
		ShaderVariableBase::setTexture( texture );

        HRESULT hr;
        if ( texture ) {
            LN_COMCALL( mDxEffect->SetTexture( 
				mHandle, static_cast<DX9TextureBase*>( texture )->getIDirect3DTexture9() ) );
        }
        else {
            LN_COMCALL( mDxEffect->SetTexture( mHandle, NULL ) );
        }

        mTextureParam->Texture = texture;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    const char* DX9ShaderVariable::getString()
    {
        HRESULT hr;
		LPCSTR str;
        LN_COMCALL( mDxEffect->GetString( mHandle, &str ) );
        return str;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9ShaderVariable::setString( const char* str )
    {
        HRESULT hr;
        LN_COMCALL( mDxEffect->SetString( mHandle, str ) );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    IShaderVariable* DX9ShaderVariable::getAnnotationByIndex( lnU32 index )
    {
        if ( index >= mAnnotationArray.size() ) return NULL;
        return mAnnotationArray[index];
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    IShaderVariable* DX9ShaderVariable::getAnnotationByName( const char* name )
    {
        DX9ShaderAnnotationMap::iterator it = mAnnotationMap.find( name );
        if ( it != mAnnotationMap.end() ) {
            return it->second;
        }
        return NULL;
    }

//==============================================================================
// ■ DX9ShaderAnnotation
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9ShaderAnnotation::DX9ShaderAnnotation( DX9Shader* shader, ID3DXEffect* dxEffect, D3DXHANDLE handle )
		: mShader       ( shader )
        , mDxEffect     ( dxEffect )
        , mHandle       ( handle )
	{
        // 変数名とセマンティクス名
        D3DXPARAMETER_DESC desc;
        mDxEffect->GetParameterDesc( mHandle, &desc );
        //mName         = LRefTString( desc.Name );

        // 変数情報
		ShaderVariableType type;
        D3DXParamDescToLNParamDesc(
            desc, 
            &mVariableTypeDesc.Class,
            &mVariableTypeDesc.Type,
			&type );
        mVariableTypeDesc.Rows      = desc.Rows;
        mVariableTypeDesc.Columns   = desc.Columns;
        mVariableTypeDesc.Elements  = desc.Elements;

		if ( type == SVT_FLOAT )
		{
			if      ( mVariableTypeDesc.Class == LN_SVC_VECTOR ) type = SVT_VECTOR;
			else if ( mVariableTypeDesc.Class == LN_SVC_MATRIX ) type = SVT_MATRIX;

			if ( mVariableTypeDesc.Elements > 0 )
			{
				if      ( type == SVT_VECTOR ) type = SVT_VECTORARRAY;
				else if ( type == SVT_MATRIX ) type = SVT_MATRIXARRAY;
			}
		}

		// 基底の初期化
		ShaderVariableBase::initialize( type, lnRefString( desc.Name ), lnRefString(), mVariableTypeDesc );

        // String 型の場合は wchar_t 取得時高速化のため、メンバに持っておく
        if ( this->mVariableTypeDesc.Type == LN_SVT_STRING )
        {
            const char* str = NULL;
            this->mDxEffect->GetString( mHandle, &str );
			setString( str );
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9ShaderAnnotation::~DX9ShaderAnnotation()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool DX9ShaderAnnotation::getBool()
    {
        HRESULT hr;
        BOOL b;
        LN_COMCALL( mDxEffect->GetBool( mHandle, &b ) );
        return ( b != FALSE );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    int DX9ShaderAnnotation::getInt()
    {
        HRESULT hr;
		int v;
        LN_COMCALL( mDxEffect->GetInt( mHandle, &v ) );
        return v;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    lnFloat DX9ShaderAnnotation::getFloat()
    {
        HRESULT hr;
		lnFloat v;
        LN_COMCALL( mDxEffect->GetFloat( mHandle, &v ) );
        return v;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    const LVector4& DX9ShaderAnnotation::getVector()
    {
        HRESULT hr;
		_allocValueBuffer( sizeof(LVector4) );
		LN_COMCALL( mDxEffect->GetVector( this->mHandle, (D3DXVECTOR4*)mValue.VectorArray ) );
        return *((LVector4*)mValue.VectorArray);
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    const LVector4* DX9ShaderAnnotation::getVectorArray()
    {
        HRESULT hr;
		_allocValueBuffer( sizeof(LVector4) * mDesc.Elements );
		LN_COMCALL( mDxEffect->GetVectorArray( mHandle, (D3DXVECTOR4*)mValue.VectorArray, mDesc.Elements ) );
		return (LVector4*)mValue.VectorArray;
    }
    
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    const LMatrix& DX9ShaderAnnotation::getMatrix()
    {
        HRESULT hr;
		_allocValueBuffer( sizeof(LVector4) );
		LN_COMCALL( mDxEffect->GetMatrix( this->mHandle, (D3DXMATRIX*)mValue.VectorArray ) );
        return *((LMatrix*)mValue.VectorArray);
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    const LMatrix* DX9ShaderAnnotation::getMatrixArray()
    {
        HRESULT hr;
		_allocValueBuffer( sizeof(LMatrix) * mDesc.Elements );
		LN_COMCALL( mDxEffect->GetMatrixArray( mHandle, (D3DXMATRIX*)mValue.VectorArray, mDesc.Elements ) );
		return (LMatrix*)mValue.VectorArray;
    }

//==============================================================================
// ■ DX9ShaderTechnique
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9ShaderTechnique::DX9ShaderTechnique( DX9Shader* shader, ID3DXEffect* dxEffect, D3DXHANDLE tech )
		: mShader       ( shader )
        , mDxEffect     ( dxEffect )
        , mTechnique    ( tech )
		, mPassCount	( 0 )
	{
		D3DXTECHNIQUE_DESC desc;
        mDxEffect->GetTechniqueDesc( mTechnique, &desc );
        mName		= desc.Name;
        mPassCount	= desc.Passes;

        // すべてのアノテーションを配列と map に格納
        D3DXHANDLE anno;
        for ( lnU32 i = 0; i < desc.Annotations; ++i )
        {
            anno = mDxEffect->GetAnnotation( mTechnique, i );
            if ( anno ) // 念のため
            {
                DX9ShaderAnnotation* v = LN_NEW DX9ShaderAnnotation( mShader, mDxEffect, anno );
                mAnnotationMap.insert( DX9ShaderAnnotationPair( v->getName(), v ) );
                mAnnotationArray.push_back( v );
            }
        }

        // パス
        D3DXHANDLE pass;
        for ( lnU32 i = 0; i < mPassCount; ++i )
        {
            pass = mDxEffect->GetPass( mTechnique, i );
            if ( pass ) // 念のため
            {
                DX9ShaderPass* ps = LN_NEW DX9ShaderPass( mShader, mDxEffect, pass, i, mTechnique );
                mShaderPassMap.insert( DX9ShaderPassPair( ps->getName(), ps ) );
                mShaderPassArray.push_back( ps );
            }
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9ShaderTechnique::~DX9ShaderTechnique()
    {
        for ( lnU32 i = 0; i < mAnnotationArray.size(); ++i ) {
            LN_SAFE_RELEASE( mAnnotationArray[ i ] );
        }

        for ( lnU32 i = 0; i < mShaderPassArray.size(); ++i ) {
            LN_SAFE_RELEASE( mShaderPassArray[ i ] );
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    IShaderVariable* DX9ShaderTechnique::getAnnotationByName( const char* name )
    {
        DX9ShaderAnnotationMap::iterator it = mAnnotationMap.find( name );
        if ( it != mAnnotationMap.end() ) {
            return it->second;
        }
        return NULL;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    IShaderVariable* DX9ShaderTechnique::getAnnotationByIndex( lnU32 index )
    {
        if ( index >= mAnnotationArray.size() ) {
			return NULL; 
		}
        return mAnnotationArray[index];
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    IShaderPass* DX9ShaderTechnique::getPassByName( const lnChar* name )
    {
        DX9ShaderPassMap::iterator it = mShaderPassMap.find( name );
        if ( it != mShaderPassMap.end() ) {
            return it->second;
        }
        return NULL;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    IShaderPass* DX9ShaderTechnique::getPassByIndex( lnU32 index )
    {
        if ( index >= mShaderPassArray.size() ) { 
			return NULL; 
		}
        return mShaderPassArray[index];
    }

//==============================================================================
// ■ DX9ShaderPass
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9ShaderPass::DX9ShaderPass( DX9Shader* shader, ID3DXEffect* dxEffect, D3DXHANDLE handle, lnU32 passIndex, D3DXHANDLE tech )
		: mShader       ( shader )
        , mDxEffect     ( dxEffect )
        , mHandle       ( handle )
        , mTechnique    ( tech )
        , mPassIndex    ( passIndex )
	{
		D3DXPASS_DESC desc;
        mDxEffect->GetPassDesc( mHandle, &desc );
        mName = LRefTString( desc.Name );

		// すべてのアノテーションを配列と map に格納
        D3DXHANDLE anno;
        for ( lnU32 i = 0; i < desc.Annotations; ++i )
        {
            anno = mDxEffect->GetAnnotation( mHandle, i );
            if ( anno ) // 念のため
            {
                DX9ShaderAnnotation* v = LN_NEW DX9ShaderAnnotation( mShader, mDxEffect, anno );
                mAnnotationMap.insert( DX9ShaderAnnotationPair( v->getName(), v ) );
                mAnnotationArray.push_back( v );
            }
        }


#if 0
		LPD3DXCONSTANTTABLE constantTable;
		//D3DXGetShaderConstantTable(desc.pVertexShaderFunction, &constantTable);
		D3DXGetShaderConstantTable(desc.pPixelShaderFunction, &constantTable);

		D3DXCONSTANTTABLE_DESC constDesc;
		constantTable->GetDesc(&constDesc);
		_p(constDesc.Creator);
		_p(constDesc.Version);
		_p(constDesc.Constants);
#pragma comment(lib, "d3dx9.lib")
		for (int i = 0; i < constDesc.Constants; ++i)
		{
			printf("--------\n");
			D3DXHANDLE handle = constantTable->GetConstant(NULL, i);
			D3DXCONSTANT_DESC cd;
			constantTable->GetConstantDesc(handle, &cd, NULL);
			_p(cd.Name);	// これでサンプラ変数が取れる
		}
#endif
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    DX9ShaderPass::~DX9ShaderPass()
    {
        for ( lnU32 i = 0; i < mAnnotationArray.size(); ++i ) {
			LN_SAFE_RELEASE( mAnnotationArray[ i ] );
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    IShaderVariable* DX9ShaderPass::getAnnotationByName( const lnChar* name )
    {
        DX9ShaderAnnotationMap::iterator it = mAnnotationMap.find( name );
        if ( it != mAnnotationMap.end() ) {
            return it->second;
        }
        return NULL;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    IShaderVariable* DX9ShaderPass::getAnnotationByIndex( lnU32 index )
    {
        if ( index >= mAnnotationArray.size() ) return NULL;
        return mAnnotationArray[index];
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9ShaderPass::begin()
    {
		HRESULT hr;
        UINT dummy;
		LN_COMCALL( mDxEffect->SetTechnique( mTechnique ) );
        LN_COMCALL( mDxEffect->Begin( &dummy, 0 ) );
        LN_COMCALL( mDxEffect->BeginPass( mPassIndex ) );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9ShaderPass::end()
    {
        mDxEffect->EndPass();
        mDxEffect->End();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void DX9ShaderPass::commit()
    {
        mDxEffect->CommitChanges();
    }

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================