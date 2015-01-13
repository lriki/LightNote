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

        // �����o�R�s�[
        Model* n = static_cast<Model*>( mSceneNode );
        mModel = n->mModel;
        LN_REFOBJ_SET( mSkinningBoneTexture, n->mSkinningBoneTexture );

        // �X�L�j���O�e�N�X�`���X�V
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
		// TODO: wchar_t ���ߑł��ɂ�����
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

		// �R���e�L�X�g���w�肵�Ċ�ꏉ����
        DrawingContext* context = LN_NEW DrawingContext();
        context->initialize( this );
        VisualNode::initialize( 1, LN_DRAWCOORD_3D, context );

		// �ǂݍ��݊J�n
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

		// 3D ���b�V���쐬
		mModelForAsyncLoad = mSceneGraph->getGraphicsManager()->getModelManager()->createModel2(
			baseDir, file, mModelCreateFlags, lnSharingKey(mModelFilePath.c_str()));

//		mSceneGraph->getFileManager()->returnCurrentDirectory();

		//-----------------------------------------------------
        // ������ .fx ���Ȃ����`�F�b�N
        //      MME �̎d�l�Ή��B�����̃V�F�[�_�t�@�C����ǂݍ��݁A�ݒ肷��B

		// �p�X����
		lnRefString path = FileIO::Path::getExtName( mModelFilePath.c_str() );
        path += _T( ".fx" );

		if ( mSceneGraph->getFileManager()->existsFile( path.c_str() ) )
		{
			// �V�F�[�_�쐬
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

        // ���f���{�̂̃}�e���A�����A���̃m�[�h�̃f�t�H���g�Ƃ��Đݒ肷��
        int sn = this->mModel->getSubsetCount();
		setSubsetCount( sn );
        for ( int i = 0; i < sn; ++i )
        {
            mNodeRenderParam.Subset[i].Material = *mModel->getMaterial(i);
			mNodeRenderParam.RenderState.Culling = Graphics::CullingMode_Double;	// ���ʕ`��
        }

        // �X�L�����b�V���̏ꍇ�̓X�L�j���O�p�̃V�F�[�_��ݒ肵�A
        // �{�[���s����i�[����e�N�X�`�����쐬����
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

		// ���O�̐ݒ�͕K���I�t�X�N���[��RT�쐬�O�ɐݒ肵�Ă������ƁI
        //setName( FileIO::Path::getFileName( this->mModelFilePath.c_str() ).c_str() );
        
		// �t�@�C�������I�u�W�F�N�g���ɂ���
		setName( FileIO::Path::getFileName( mModelFilePath.c_str() ).c_str() );

		// �V�F�[�_�͐����Ɏ��s���Ă��A�Ƃ肠�����ݒ肷�� (�c�[�����琶�����s���m�F�ł���悤��)
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
		
			// �����ŃA�j���[�V�����X�V
			
		mModel->preUpdateTransform();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Model::update( float elapsedTime )
    {
		if ( mModel )
		{




			////// �C�ӂ̃{�[���X�V������ꍇ�͂����ɏ����������K�v������c

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
