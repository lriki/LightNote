//==============================================================================
// Mesh 
//==============================================================================

#include "stdafx.h"
#include "../../Resource/LNResource.h"
#include "../../Graphics/Common/Model/ModelManager.h"
#include "../SceneGraph.h"
#include "../SceneShader/SceneShaderManager.h"
#include "../SceneShader/SceneShader.h"
#include "Mesh.h"

#if 0

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// ■ Mesh::DrawingContext
//==============================================================================
class Mesh::DrawingContext
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
        Mesh* n = static_cast<Mesh*>( mSceneNode );
        mModel = n->mModel;
        LN_REFOBJ_SET( mSkinningBoneTexture, n->mSkinningBoneTexture );

        // スキニングテクスチャ更新
        if ( mSkinningBoneTexture )
        {
            mModel->updateSkinningMatrix();

            lnU32 frame_num = mModel->getFrameNum();
            LMatrix* t = (LMatrix*)mSkinningBoneTexture->lock();
            for ( lnU32 i = 0; i < frame_num; ++i )
            {
                t[ i ] = mModel->getFrame( i )->getSkinningMatrix();
            }
            mSkinningBoneTexture->unlock();
        }
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
		if ( this->mModel ) {
			this->mModel->drawSubset( index );
		}
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	virtual const LMatrix& findFrameMarix( const lnChar* name ) const 
	{
		if ( this->mModel ) {
			return mModel->findFrame( name )->getWorldMatrix();
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

    Graphics::Model*    mModel;
    Graphics::Texture*	mSkinningBoneTexture;
};

//==============================================================================
// ■ Mesh
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Mesh::Mesh( SceneGraph* scene_ )
        : VisualNode				( scene_ )
		//, FileIO::AsyncLoadObject	( scene_->getFileManager() )
		, mModelForAsyncLoad	( NULL )
		, mShaderForAsyncLoad	( NULL )
        , mModel				( NULL )
        , mSkinningBoneTexture  ( NULL )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Mesh::~Mesh()
    {
        LN_SAFE_RELEASE( mModelForAsyncLoad );
        LN_SAFE_RELEASE( mShaderForAsyncLoad );

        LN_SAFE_RELEASE( mSkinningBoneTexture );
        LN_SAFE_RELEASE( mModel );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Mesh::create( const lnChar* filename_, lnU32 flags_, bool asyncLoad )
    {
#if 1
        LNRESULT lr;

        this->mModelFilePath = filename_;
		this->mModelCreateFlags = flags_;

		// コンテキストを指定して基底初期化
        DrawingContext* context = LN_NEW DrawingContext();
        context->initialize( this );
        VisualNode::initialize( 1, LN_DRAWCOORD_3D, context );

		startLoadProc( asyncLoad );
#if 0
        LN_CALL_R( this->mSceneGraph->getGraphicsManager()->getModelManager()->createModel(
            &mModel, filename_, flags_, lnSharingKey( filename_ ) ) );

        // コンテキストを指定して基底初期化
        DrawingContext* context = LN_NEW DrawingContext();
        context->initialize( this, mModel->getSubsetCount() );
        SceneNode::initialize( mModel->getSubsetCount(), LN_DRAWCOORD_3D, context );

        // 名前の設定は必ずオフスクリーンRT作成前に設定しておくこと！
        this->setName( FileIO::Path::getFileName( filename_ ).c_str() );

        // モデル本体のマテリアルを、このノードのデフォルトとして設定する
        int sn = mModel->getSubsetCount();
        for ( int i = 0; i < sn; ++i )
        {
            this->mNodeRenderParam.Subset[ i ].Material = *mModel->getMaterial( i );
        }

        // スキンメッシュの場合はスキニング用のシェーダを設定し、
        // ボーン行列を格納するテクスチャを作成する
        if ( mModel->isSkinMesh() )
        {
            SceneShader* shader = 
				mSceneGraph->getSceneShaderManager()->createSceneShader( _T( "LNResource\\Shader\\SSBasic3DSkinning.lnfx" ) );
            this->setShader( shader );
            LN_SAFE_RELEASE( shader );

            this->mSceneGraph->getGraphicsManager()->getGraphicsDevice()->createTexture(
                &mSkinningBoneTexture, 4, mModel->getFrameNum(), 1, LN_FMT_A32B32G32R32F );
        }
        
		// ファイル名をオブジェクト名にする
		this->setName( FileIO::Path::getFileName( filename_ ).c_str() );

        //-----------------------------------------------------
        // 同名の .fx がないかチェック
        //      MME の仕様対応。同名のシェーダファイルを読み込み、設定する。

		// パス生成
		lnRefString path = FileIO::Path::getExtName( filename_ );
        path += _T( ".fx" );

		if ( mSceneGraph->getFileManager()->existsFile( path.c_str() ) )//FileIO::File::exists( path.c_str() ) )
		{
			// シェーダ作成
			SceneShader* shader = NULL;
			try
			{
				shader = this->mSceneGraph->getSceneShaderManager()->createSceneShader( path.c_str() );
			}
			catch ( Base::FileNotFoundException e )
			{
				_p(e.getOutputMessage());
			}

			// 生成に失敗しても、とりあえず設定する (ツールから生成失敗を確認できるように)
			this->setShader( shader );
			shader->release();
		}
#endif
#endif
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool Mesh::onLoadProc()
	{
		// 3D メッシュ作成
		mSceneGraph->getGraphicsManager()->getModelManager()->createModel(
			&mModelForAsyncLoad, mModelFilePath.c_str(), mModelCreateFlags, lnSharingKey( mModelFilePath.c_str() ) );

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
				mShaderForAsyncLoad = mSceneGraph->getSceneShaderManager()->createSceneShader( path.c_str() );
			}
			catch ( Base::FileNotFoundException e ) {
				_p(e.getOutputMessage());
			}
		}
#if 0
		this->mSceneGraph->getGraphicsManager()->getModelManager()->createModel(
			&mModel, mModelFilePath.c_str(), mModelCreateFlags, lnSharingKey( mModelFilePath.c_str() ) );

        // コンテキストを指定して基底初期化
        DrawingContext* context = LN_NEW DrawingContext();
        context->initialize( this, mModel->getSubsetCount() );
        SceneNode::initialize( mModel->getSubsetCount(), LN_DRAWCOORD_3D, context );

        // 名前の設定は必ずオフスクリーンRT作成前に設定しておくこと！
        this->setName( FileIO::Path::getFileName( mModelFilePath.c_str() ).c_str() );

        // モデル本体のマテリアルを、このノードのデフォルトとして設定する
        int sn = mModel->getSubsetCount();
        for ( int i = 0; i < sn; ++i )
        {
            this->mNodeRenderParam.Subset[ i ].Material = *mModel->getMaterial( i );
        }

        // スキンメッシュの場合はスキニング用のシェーダを設定し、
        // ボーン行列を格納するテクスチャを作成する
        if ( mModel->isSkinMesh() )
        {
            SceneShader* shader = 
				mSceneGraph->getSceneShaderManager()->createSceneShader( _T( "LNResource\\Shader\\SSBasic3DSkinning.lnfx" ) );
            this->setShader( shader );
            LN_SAFE_RELEASE( shader );

            this->mSceneGraph->getGraphicsManager()->getGraphicsDevice()->createTexture(
                &mSkinningBoneTexture, 4, mModel->getFrameNum(), 1, LN_FMT_A32B32G32R32F );
        }
        
		// ファイル名をオブジェクト名にする
		this->setName( FileIO::Path::getFileName( mModelFilePath.c_str() ).c_str() );

        //-----------------------------------------------------
        // 同名の .fx がないかチェック
        //      MME の仕様対応。同名のシェーダファイルを読み込み、設定する。

		// パス生成
		lnRefString path = FileIO::Path::getExtName( mModelFilePath.c_str() );
        path += _T( ".fx" );

		if ( mSceneGraph->getFileManager()->existsFile( path.c_str() ) )//FileIO::File::exists( path.c_str() ) )
		{
			// シェーダ作成
			SceneShader* shader = NULL;
			try
			{
				shader = this->mSceneGraph->getSceneShaderManager()->createSceneShader( path.c_str() );
			}
			catch ( Base::FileNotFoundException e )
			{
				_p(e.getOutputMessage());
			}

			// 生成に失敗しても、とりあえず設定する (ツールから生成失敗を確認できるように)
			this->setShader( shader );
			shader->release();
		}
#endif
		return true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Mesh::onLoadProcFinished()
	{
		this->mModel = this->mModelForAsyncLoad;
		this->mModelForAsyncLoad = NULL;

        // モデル本体のマテリアルを、このノードのデフォルトとして設定する
        int sn = this->mModel->getSubsetCount();
		setSubsetCount( sn );
        for ( int i = 0; i < sn; ++i )
        {
            mNodeRenderParam.Subset[i].Material = *mModel->getMaterial(i);
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
                4, mModel->getFrameNum(), 1, Graphics::SurfaceFormat_A32B32G32R32F );
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
    void Mesh::update( float elapsedTime )
    {
		if ( this->mModel )
		{
			// ここでアニメーション更新

			mModel->updateFrameMatrix();

			////// 任意のボーン更新をする場合はここに処理を書く必要がある…

			mModel->postUpdate( elapsedTime );

			//mModel->skinning();
		}
    }


} // namespace Scene
} // namespace Core
} // namespace LNote

#endif

//==============================================================================
//
//==============================================================================