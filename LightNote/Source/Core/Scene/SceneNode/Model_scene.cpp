//==============================================================================
// Model 
//==============================================================================

#include "stdafx.h"
#include "../../Resource/LNResource.h"
#include "../../Graphics/Common/Model/ModelManager.h"
#include "../SceneGraph.h"
#include "../SceneShader/SceneShaderManager.h"
#include "../SceneShader/SceneShader.h"
#include "Model.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// Model::DrawingContext
//==============================================================================
class Model::DrawingContext
    : public VisualNodeContext
{
public:
    DrawingContext()
        : mSkinningBoneTexture	( NULL )
    {}

	virtual ~DrawingContext() {}

public:

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    virtual void updateContext( SceneNodeContext* parentContext )
    {
        VisualNodeContext::updateContext( parentContext );

        // メンバコピー
        Model* n = static_cast<Model*>( mSceneNode );
        mModel = n->mModel;
        LN_REFOBJ_SET( mSkinningBoneTexture, n->mSkinningBoneTexture );

        // スキニングテクスチャ更新
		if ( mSkinningBoneTexture )
		{
			LMatrix* t = (LMatrix*)mSkinningBoneTexture->lock();
			memcpy( t, mModel->getSkinningMatrices(), sizeof(LMatrix) * mModel->getFrameCount() );
			mSkinningBoneTexture->unlock();
		}

        //if ( mSkinningBoneTexture )
        //{
        //    mModel->updateSkinningMatrix();

        //    lnU32 frame_num = mModel->getFrameNum();
        //    LMatrix* t = (LMatrix*)mSkinningBoneTexture->lock();
        //    for ( lnU32 i = 0; i < frame_num; ++i )
        //    {
        //        t[ i ] = mModel->getFrame( i )->getSkinningMatrix();
        //    }
        //    mSkinningBoneTexture->unlock();
        //}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    virtual void releaseAllResource()
    {
        VisualNodeContext::releaseAllResource();
        LN_SAFE_RELEASE( mSkinningBoneTexture );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    virtual void onSetNodeParameter( SceneShader* shader )
    {
        if ( mSkinningBoneTexture )
        {
            Graphics::IShader* core = shader->getCoreShader();
            Graphics::IShaderVariable* v;

            v = core->getVariableByName( _T("lnBoneTextureReciprocalSize") );
            if ( v ) v->setVector( LVector4( mSkinningBoneTexture->getRealSize( true ), 0, 0 ) );

            v = core->getVariableByName( _T("lnBoneTexture") );
            if ( v ) v->setTexture( mSkinningBoneTexture );
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    virtual void drawSubset( ContextDrawParam& param, int index )
    {
		if ( mModel ) {
			mModel->drawSubset( index );
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	virtual const LMatrix& findFrameMarix( const lnChar* name ) const 
	{
		// TODO: wchar_t 決め打ちにしたい
		if ( mModel ) {
			Graphics::ModelFrame2* frame = mModel->findFrame( lnRefStringW( name ).c_str() );
			if ( frame )
				return *frame->getBackendGlobalMatrix();
		}
		return LMatrix::IDENTITY;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	virtual lnFloat findMorphBlend( const lnChar* name ) const 
	{ 
		// TODO:
		return 0.0f; 
	}


private:

    Graphics::Model2*		mModel;
    Graphics::Texture*		mSkinningBoneTexture;
};

//==============================================================================
// Model
//==============================================================================

	LN_TYPE_INFO_ACCESS_IMPL(Model);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Model::Model( SceneGraph* scene )
        : VisualNode			( scene )
		, mModelForAsyncLoad	( NULL )
		, mShaderForAsyncLoad	( NULL )
        , mModel				( NULL )
        , mSkinningBoneTexture  ( NULL )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Model::~Model()
    {
        LN_SAFE_RELEASE( mModelForAsyncLoad );
        LN_SAFE_RELEASE( mShaderForAsyncLoad );

        LN_SAFE_RELEASE( mSkinningBoneTexture );
        LN_SAFE_RELEASE( mModel );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Model::create( const lnChar* filePath, lnU32 flags, bool asyncLoad )
    {
        mModelFilePath = filePath;
		mModelCreateFlags = flags;

		// コンテキストを指定して基底初期化
        DrawingContext* context = LN_NEW DrawingContext();
        context->initialize( this );
        VisualNode::initialize( 1, LN_DRAWCOORD_3D, context );

		// 読み込み開始
		startLoadProc( asyncLoad );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Model::onLoadProc()
	{
		//lnChar dir[LN_MAX_PATH];
  //      lnChar filename[LN_MAX_PATH];
  //      FileIO::Path::getDirPathAndFilePath( mModelFilePath.c_str(), dir, filename );

		FileIO::PathNameW filePath(mModelFilePath.c_str());
		FileIO::PathNameW baseDir = filePath.canonicalizePath(mSceneGraph->getFileManager()).getParent();

		LRefPtr<FileIO::InFile> file( mSceneGraph->getFileManager()->createInFile( mModelFilePath.c_str() ) );

//		mSceneGraph->getFileManager()->moveCurrentDirectory( dir );

		// 3D メッシュ作成
		mModelForAsyncLoad = mSceneGraph->getGraphicsManager()->getModelManager()->createModel2(
			baseDir, file, mModelCreateFlags, lnSharingKey(mModelFilePath.c_str()));

//		mSceneGraph->getFileManager()->returnCurrentDirectory();

		//-----------------------------------------------------
        // 同名の .fx がないかチェック
        //      MME の仕様対応。同名のシェーダファイルを読み込み、設定する。

		// パス生成
		lnRefString path = FileIO::Path::getExtName( mModelFilePath.c_str() );
        path += _T( ".fx" );

		if ( mSceneGraph->getFileManager()->existsFile( path.c_str() ) )
		{
			// シェーダ作成
			try {
				lnString errors;	// TODO
				mShaderForAsyncLoad = mSceneGraph->getSceneShaderManager()->createSceneShader(path.c_str(), &errors);
			}
			catch ( Base::FileNotFoundException e ) {
				//_p(e.getOutputMessage());
				throw;	// TODO
			}
		}
		return true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Model::onLoadProcFinished()
	{
		mModel = this->mModelForAsyncLoad;
		mModelForAsyncLoad = NULL;

        // モデル本体のマテリアルを、このノードのデフォルトとして設定する
        int sn = this->mModel->getSubsetCount();
		setSubsetCount( sn );
        for ( int i = 0; i < sn; ++i )
        {
            mNodeRenderParam.Subset[i].Material = *mModel->getMaterial(i);
			mNodeRenderParam.RenderState.Culling = Graphics::CullingMode_Double;	// 両面描画
        }

        // スキンメッシュの場合はスキニング用のシェーダを設定し、
        // ボーン行列を格納するテクスチャを作成する
        if ( mModel->isSkinMesh() )
        {
            SceneShader* shader = 
				mSceneGraph->getSceneShaderManager()->createSceneShader(
					Resource::ID_Shader_SSBasic3DSkinning,
					lnSharingKey( _T( "LNResource\\Shader\\SSBasic3DSkinning.lnfx" ) ) );
            setShader( shader );
            LN_SAFE_RELEASE( shader );

            mSkinningBoneTexture = mSceneGraph->getGraphicsManager()->getGraphicsDevice()->createTexture(
                4, mModel->getFrameCount(), 1, Graphics::SurfaceFormat_A32B32G32R32F );
        }

		// 名前の設定は必ずオフスクリーンRT作成前に設定しておくこと！
        //setName( FileIO::Path::getFileName( this->mModelFilePath.c_str() ).c_str() );
        
		// ファイル名をオブジェクト名にする
		setName( FileIO::Path::getFileName( mModelFilePath.c_str() ).c_str() );

		// シェーダは生成に失敗しても、とりあえず設定する (ツールから生成失敗を確認できるように)
		if ( this->mShaderForAsyncLoad ) 
		{
			setShader( this->mShaderForAsyncLoad );
			this->mShaderForAsyncLoad->release();
			this->mShaderForAsyncLoad = NULL;
		}
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Model::updateBeforePhysics()
	{
		mModel->setWorldMatrix( mCombinedWorldMatrix );
		
			// ここでアニメーション更新
			
		mModel->preUpdateTransform();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Model::update( float elapsedTime )
    {
		if ( mModel )
		{




			////// 任意のボーン更新をする場合はここに処理を書く必要がある…

		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Model::updateAfterPhysics()
	{
		mModel->postUpdateTransform( mCombinedWorldMatrix );

		//mModel->skinning();
	}


} // namespace Scene
} // namespace Core
} // namespace LNote
