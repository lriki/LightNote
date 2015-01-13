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
// �� Mesh::DrawingContext
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

        // �����o�R�s�[
        Mesh* n = static_cast<Mesh*>( mSceneNode );
        mModel = n->mModel;
        LN_REFOBJ_SET( mSkinningBoneTexture, n->mSkinningBoneTexture );

        // �X�L�j���O�e�N�X�`���X�V
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
// �� Mesh
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

		// �R���e�L�X�g���w�肵�Ċ�ꏉ����
        DrawingContext* context = LN_NEW DrawingContext();
        context->initialize( this );
        VisualNode::initialize( 1, LN_DRAWCOORD_3D, context );

		startLoadProc( asyncLoad );
#if 0
        LN_CALL_R( this->mSceneGraph->getGraphicsManager()->getModelManager()->createModel(
            &mModel, filename_, flags_, lnSharingKey( filename_ ) ) );

        // �R���e�L�X�g���w�肵�Ċ�ꏉ����
        DrawingContext* context = LN_NEW DrawingContext();
        context->initialize( this, mModel->getSubsetCount() );
        SceneNode::initialize( mModel->getSubsetCount(), LN_DRAWCOORD_3D, context );

        // ���O�̐ݒ�͕K���I�t�X�N���[��RT�쐬�O�ɐݒ肵�Ă������ƁI
        this->setName( FileIO::Path::getFileName( filename_ ).c_str() );

        // ���f���{�̂̃}�e���A�����A���̃m�[�h�̃f�t�H���g�Ƃ��Đݒ肷��
        int sn = mModel->getSubsetCount();
        for ( int i = 0; i < sn; ++i )
        {
            this->mNodeRenderParam.Subset[ i ].Material = *mModel->getMaterial( i );
        }

        // �X�L�����b�V���̏ꍇ�̓X�L�j���O�p�̃V�F�[�_��ݒ肵�A
        // �{�[���s����i�[����e�N�X�`�����쐬����
        if ( mModel->isSkinMesh() )
        {
            SceneShader* shader = 
				mSceneGraph->getSceneShaderManager()->createSceneShader( _T( "LNResource\\Shader\\SSBasic3DSkinning.lnfx" ) );
            this->setShader( shader );
            LN_SAFE_RELEASE( shader );

            this->mSceneGraph->getGraphicsManager()->getGraphicsDevice()->createTexture(
                &mSkinningBoneTexture, 4, mModel->getFrameNum(), 1, LN_FMT_A32B32G32R32F );
        }
        
		// �t�@�C�������I�u�W�F�N�g���ɂ���
		this->setName( FileIO::Path::getFileName( filename_ ).c_str() );

        //-----------------------------------------------------
        // ������ .fx ���Ȃ����`�F�b�N
        //      MME �̎d�l�Ή��B�����̃V�F�[�_�t�@�C����ǂݍ��݁A�ݒ肷��B

		// �p�X����
		lnRefString path = FileIO::Path::getExtName( filename_ );
        path += _T( ".fx" );

		if ( mSceneGraph->getFileManager()->existsFile( path.c_str() ) )//FileIO::File::exists( path.c_str() ) )
		{
			// �V�F�[�_�쐬
			SceneShader* shader = NULL;
			try
			{
				shader = this->mSceneGraph->getSceneShaderManager()->createSceneShader( path.c_str() );
			}
			catch ( Base::FileNotFoundException e )
			{
				_p(e.getOutputMessage());
			}

			// �����Ɏ��s���Ă��A�Ƃ肠�����ݒ肷�� (�c�[�����琶�����s���m�F�ł���悤��)
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
		// 3D ���b�V���쐬
		mSceneGraph->getGraphicsManager()->getModelManager()->createModel(
			&mModelForAsyncLoad, mModelFilePath.c_str(), mModelCreateFlags, lnSharingKey( mModelFilePath.c_str() ) );

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
				mShaderForAsyncLoad = mSceneGraph->getSceneShaderManager()->createSceneShader( path.c_str() );
			}
			catch ( Base::FileNotFoundException e ) {
				_p(e.getOutputMessage());
			}
		}
#if 0
		this->mSceneGraph->getGraphicsManager()->getModelManager()->createModel(
			&mModel, mModelFilePath.c_str(), mModelCreateFlags, lnSharingKey( mModelFilePath.c_str() ) );

        // �R���e�L�X�g���w�肵�Ċ�ꏉ����
        DrawingContext* context = LN_NEW DrawingContext();
        context->initialize( this, mModel->getSubsetCount() );
        SceneNode::initialize( mModel->getSubsetCount(), LN_DRAWCOORD_3D, context );

        // ���O�̐ݒ�͕K���I�t�X�N���[��RT�쐬�O�ɐݒ肵�Ă������ƁI
        this->setName( FileIO::Path::getFileName( mModelFilePath.c_str() ).c_str() );

        // ���f���{�̂̃}�e���A�����A���̃m�[�h�̃f�t�H���g�Ƃ��Đݒ肷��
        int sn = mModel->getSubsetCount();
        for ( int i = 0; i < sn; ++i )
        {
            this->mNodeRenderParam.Subset[ i ].Material = *mModel->getMaterial( i );
        }

        // �X�L�����b�V���̏ꍇ�̓X�L�j���O�p�̃V�F�[�_��ݒ肵�A
        // �{�[���s����i�[����e�N�X�`�����쐬����
        if ( mModel->isSkinMesh() )
        {
            SceneShader* shader = 
				mSceneGraph->getSceneShaderManager()->createSceneShader( _T( "LNResource\\Shader\\SSBasic3DSkinning.lnfx" ) );
            this->setShader( shader );
            LN_SAFE_RELEASE( shader );

            this->mSceneGraph->getGraphicsManager()->getGraphicsDevice()->createTexture(
                &mSkinningBoneTexture, 4, mModel->getFrameNum(), 1, LN_FMT_A32B32G32R32F );
        }
        
		// �t�@�C�������I�u�W�F�N�g���ɂ���
		this->setName( FileIO::Path::getFileName( mModelFilePath.c_str() ).c_str() );

        //-----------------------------------------------------
        // ������ .fx ���Ȃ����`�F�b�N
        //      MME �̎d�l�Ή��B�����̃V�F�[�_�t�@�C����ǂݍ��݁A�ݒ肷��B

		// �p�X����
		lnRefString path = FileIO::Path::getExtName( mModelFilePath.c_str() );
        path += _T( ".fx" );

		if ( mSceneGraph->getFileManager()->existsFile( path.c_str() ) )//FileIO::File::exists( path.c_str() ) )
		{
			// �V�F�[�_�쐬
			SceneShader* shader = NULL;
			try
			{
				shader = this->mSceneGraph->getSceneShaderManager()->createSceneShader( path.c_str() );
			}
			catch ( Base::FileNotFoundException e )
			{
				_p(e.getOutputMessage());
			}

			// �����Ɏ��s���Ă��A�Ƃ肠�����ݒ肷�� (�c�[�����琶�����s���m�F�ł���悤��)
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

        // ���f���{�̂̃}�e���A�����A���̃m�[�h�̃f�t�H���g�Ƃ��Đݒ肷��
        int sn = this->mModel->getSubsetCount();
		setSubsetCount( sn );
        for ( int i = 0; i < sn; ++i )
        {
            mNodeRenderParam.Subset[i].Material = *mModel->getMaterial(i);
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
                4, mModel->getFrameNum(), 1, Graphics::SurfaceFormat_A32B32G32R32F );
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
    void Mesh::update( float elapsedTime )
    {
		if ( this->mModel )
		{
			// �����ŃA�j���[�V�����X�V

			mModel->updateFrameMatrix();

			////// �C�ӂ̃{�[���X�V������ꍇ�͂����ɏ����������K�v������c

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