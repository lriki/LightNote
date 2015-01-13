//==============================================================================
// SceneGraph 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Resource/LNResource.h"
#include "../Graphics/Manager.h"
#include "../Effect/Manager.h"
#include "Manager.h"
#include "SceneNode/SceneNode.h"
#include "SceneNode/Camera.h"
#include "SceneNode/Light.h"
#include "SceneNode/Proxy.h"
#include "SceneShader/SceneShaderManager.h"
#include "SceneShader/SceneShader.h"
#include "SceneGraph.h"
#include "Layer.h"
#include "Pane.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// �� SceneGraph
//==============================================================================

	//----------------------------------------------------------------------
	// �� �R���X�g���N�^
	//----------------------------------------------------------------------
    SceneGraph::SceneGraph( Manager* manager_ )
        : mLogFile              ( NULL )
        , mManager              ( manager_ )
        , mFileManager          ( NULL )
        , mGraphicsManager      ( NULL )
        , mGeometryRenderer     ( NULL )
        , mEffectManager        ( NULL )
        , mPhysicsManager       ( NULL )
        , mSceneShaderManager   ( NULL )
		, mTotalTime(0.0)
		, mElapsedTime(0.0f)
        , mDefaultCamera        ( NULL )
        , mDefaultLight         ( NULL )
		, mMainCamera			( NULL )
        , mActiveViewFrustum    ( NULL )
        //, mEffect3DBatchProxy   ( NULL )
        //, mEffect2DBatchProxy   ( NULL )
        //, mPhysicsDebugDrawProxy( NULL )
        , mSceneGraphContext    ( NULL )
    {
        LN_SAFE_ADDREF( mManager );

        memset( mDefaultSceneNodes, 0, sizeof( mDefaultSceneNodes ) );
        memset( mDefaultShaderArray, 0, sizeof( mDefaultSceneNodes ) );
    }

	//----------------------------------------------------------------------
	// �� �f�X�g���N�^
	//----------------------------------------------------------------------
    SceneGraph::~SceneGraph()
    {
        SAFE_DELETE( mSceneGraphContext );

        // SceneShaderManager �� SceneNode �̃f�X�g���N�^���炢�낢��o�R����
        // �I�t�X�N���[�� RT ����m�[�h���O���Ƃ���܂Ŏg���B
        // ���̂��߁Afinalize �ł͂Ȃ���ԍŌ�̃f�X�g���N�^�ŊJ������
        LN_SAFE_RELEASE( mSceneShaderManager );
        LN_SAFE_RELEASE( mManager );
    }

	//----------------------------------------------------------------------
	// �� ������
	//----------------------------------------------------------------------
    LNRESULT SceneGraph::initialize( const InitData& init_data_ )
    {
        mFileManager        = init_data_.FileManager;
        mGraphicsManager    = init_data_.GraphicsManager;
        mGeometryRenderer   = mGraphicsManager->getGeometryRenderer();
        mEffectManager      = init_data_.EffectManager;
        mPhysicsManager     = init_data_.PhysicsManager;
        //mVirtualViewSize    = init_data_.VirtualViewSize;

		// �`����
		mSceneGraphContext = LN_NEW SceneGraphContext();
        mSceneGraphContext->initialize( this );
		
        // �V�F�[�_�Ǘ��N���X
        SceneShaderManager::ConfigData ss_data;
        ss_data.FileManager     = init_data_.FileManager;
        ss_data.GraphicsManager = init_data_.GraphicsManager;
        ss_data.ShaderCacheSize = init_data_.ShaderCacheSize;
        ss_data.UseSemantics    = true;
        mSceneShaderManager = LN_NEW SceneShaderManager();
        mSceneShaderManager->initialize( ss_data );
		mSceneShaderManager->setBaseViewSize( init_data_.VirtualViewSize );
		
        // �f�t�H���g�̃V�F�[�_�v���O��������� 
        mDefaultShaderArray[ LN_DEFSHADER_BASIC_2D ] = 
			mSceneShaderManager->createSceneShader( Resource::ID_Shader_SSBasic2D, lnSharingKey( _T("ID_Shader_SSBasic2D") ) );
		
		mDefaultShaderArray[ LN_DEFSHADER_BASIC_3D ] = 
			mSceneShaderManager->createSceneShader( Resource::ID_Shader_SSBasic3D, lnSharingKey( _T("ID_Shader_SSBasic3D") ) );

        mDefaultShaderArray[ LN_DEFSHADER_NOLIGHTING ] = 
			mSceneShaderManager->createSceneShader( Resource::ID_Shader_SSNoLighting, lnSharingKey( _T("ID_Shader_SSNoLighting") ) );
		
        // �f�t�H���g�̃m�[�h
        Geometry::Size view_size( (int)init_data_.VirtualViewSize.x, (int)init_data_.VirtualViewSize.y );
        Viewport* node_root = LN_NEW Viewport( this );
        Viewport* node_3d   = LN_NEW Viewport( this );
        Viewport* node_2d   = LN_NEW Viewport( this );
		node_root->create(view_size, NULL);
		node_3d->create(view_size, mGraphicsManager->getSprite3DRenderer());
		node_2d->create(view_size, mGraphicsManager->getSprite2DRenderer());

        node_root->addChild( node_3d );
        node_root->addChild( node_2d );

        mDefaultSceneNodes[ LN_DEFAULT_SCENENODE_ROOT ] = node_root;
        mDefaultSceneNodes[ LN_DEFAULT_SCENENODE_3D ]   = node_3d;
        mDefaultSceneNodes[ LN_DEFAULT_SCENENODE_2D ]   = node_2d;
		
        // �f�t�H���g�̃J����
        mDefaultCamera = LN_NEW Camera( this );
        mDefaultCamera->initialize( false );
		mDefaultCamera->setRenderRootNode( node_3d );
///		mDefaultCamera->setRenderTarget( mGraphicsManager->getGraphicsDevice()->getDefaultCanvas()->getBackbuffer() );
#if LNOTE_SCENE_CAMERA_SCREEN_EFFECT
		mDefaultCamera->setScreenEffectMode( LN_SCREENEFFECTMODE_BUFFERINGEFFECT );
#endif

        mDefault2DCamera = LN_NEW Camera( this );
        mDefault2DCamera->initialize( true );
		mDefault2DCamera->setRenderRootNode( node_2d );
		//mDefault2DCamera->setRenderTarget( mGraphicsManager->getGraphicsDevice()->getDefaultCanvas()->getBackbuffer() );

		LN_REFOBJ_SET( mMainCamera, mDefaultCamera );

		
        // �f�t�H���g�̃��C�g
        mDefaultLight = LN_NEW Light( this );
        mDefaultLight->create();
        mDefaultLight->setLightingPriority( LN_DEFAULT_LIGHT_PRIORITY );
        
		return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void SceneGraph::finalize()
    {
		mSceneNodeList2.clear();
		mCameraObjectArray.clear();
		mLightObjectArray.clear();
        //LN_SAFE_RELEASE( mEffect3DBatchProxy );
        //LN_SAFE_RELEASE( mEffect2DBatchProxy );
        //LN_SAFE_RELEASE( mPhysicsDebugDrawProxy );

        for ( int i = 0; i < LN_MAX_DEFAULT_NODES; ++i )
        {
            LN_SAFE_RELEASE( mDefaultSceneNodes[ i ] );
        }

        for ( int i = 0; i < LN_MAX_DEFAULT_SHADERS; ++i )
        {
            LN_SAFE_RELEASE( mDefaultShaderArray[ i ] );
        }

		LN_SAFE_RELEASE( mMainCamera );
        LN_SAFE_RELEASE( mDefaultLight );
        LN_SAFE_RELEASE( mDefaultCamera );
        LN_SAFE_RELEASE( mDefault2DCamera );

        if ( mSceneShaderManager )
        {
            mSceneShaderManager->finalize();
        }
    }
   
	//----------------------------------------------------------------------
	// �� �f�t�H���g�ō쐬�����J�����̎擾 (0:3D 1:2D)
	//----------------------------------------------------------------------
    Camera* SceneGraph::getDefaultCamera( int type_ )
    {
        switch ( type_ )
        {
            case 0 : 
            	return mDefaultCamera;
            case 1 : 
            	return mDefault2DCamera;
        }
        return NULL;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SceneGraph::updateBeforePhysics()
	{
		ln_foreach( SceneNode* n, mSceneNodeList2.getObjectArray() )
        {
			if ( n->isEnableAutoUpdate() ) n->updateBeforePhysics();
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SceneGraph::updateFrame(float elapsedTime)
    {
		//mLastUpdateTime = gameTime;

		mElapsedTime = elapsedTime;
		mTotalTime += mElapsedTime;

#if LNOTE_SCENE_CAMERA_SCREEN_EFFECT
		// �J����
		ln_foreach( Camera* c, mCameraObjectArray.getObjectArray() )
        {
            c->updateFrame( gameTime );
        }
#endif

		// �S�m�[�h�̃t���[���X�V
		ln_foreach( SceneNode* n, mSceneNodeList2.getObjectArray() )
        {
			if (n->isEnableAutoUpdate()) n->update(mElapsedTime);
		}

		// �ǉ��v������Ă���m�[�h�������ōX�V���Ă��܂�
		ln_foreach( SceneNode* n, mSceneNodeList2.getRegistOrderObjectArray() )
        {
			if (n->isEnableAutoUpdate()) n->update(mElapsedTime);
        }
		
        mSceneShaderManager->updateShaderList();

        // ���[���h�s����K�w�I�ɍX�V����
        mDefaultSceneNodes[ LN_DEFAULT_SCENENODE_ROOT ]->updateTransformHierarchy( LMatrix(), NULL );

    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SceneGraph::updateAfterPhysics()
	{
		ln_foreach( SceneNode* n, mSceneNodeList2.getObjectArray() )
        {
			if ( n->isEnableAutoUpdate() ) n->updateAfterPhysics();
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void SceneGraph::updateAllContext()
    {
		mSceneNodeList2.commit();
		ln_foreach( SceneNode* n, mSceneNodeList2.getObjectArray() )
        {
            n->updateContext();
        }

		mCameraObjectArray.commit();
		ln_foreach( Camera* c, mCameraObjectArray.getObjectArray() )
        {
            c->updateContext();
        }

		mLightObjectArray.commit();
		ln_foreach( Light* c, mLightObjectArray.getObjectArray() )
        {
            c->updateContext();
        }

		
		mSceneShaderManager->updateAllShaderGlobalParam(mTotalTime, mElapsedTime, mSceneGraphContext);

        mSceneGraphContext->updateContext();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SceneGraph::postFrameUpdate()
	{
		ln_foreach(SceneNode* n, mSceneNodeList2.getObjectArray())
		{
			n->postFrameUpdate();
		}

		// �ǉ��v������Ă���m�[�h���X�V����B
		// (SpritePath �Ȃ񂩂͍�����シ���ɂł� DrawRequest() ���邾�낤��)
		ln_foreach(SceneNode* n, mSceneNodeList2.getRegistOrderObjectArray())
		{
			n->postFrameUpdate();
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//void SceneGraph::renderByAllCameras()
	//{
	//	ln_foreach( Camera* c, mCameraObjectArray.getObjectArray() )
 //       {
	//		c->getCameraContext()->render();
 //       }
	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SceneGraph::setBaseViewSize( const LVector2& size )
	{
		mSceneShaderManager->setBaseViewSize( size );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void SceneGraph::addCamera( Camera* camera )
	{
		mCameraObjectArray.registerObject( camera );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void SceneGraph::addLight( Light* light )
	{
		mLightObjectArray.registerObject( light );
	}

	//----------------------------------------------------------------------
	// �� ���b�Z�[�W���� (Manager ����Ă΂��)
	//----------------------------------------------------------------------
    bool SceneGraph::onEvent( const System::EventArgs& e )
    {
        if ( mSceneShaderManager->onEvent( e ) ) return true;

		
		
        // �J�����̃C�x���g����
		CameraList::iterator itr = mCameraObjectArray.getObjectArray().begin();
		CameraList::iterator end = mCameraObjectArray.getObjectArray().end();
        for ( ; itr != end; ++itr )
        {
            if ( (*itr)->onEvent( e ) ) return true;
        }

        return false;
    }

	//----------------------------------------------------------------------
	// �� (SceneNode �̃R���X�g���N�^����Ă΂��B�c���[�ł͂Ȃ��Ǘ����X�g�ɒǉ�����)
	//----------------------------------------------------------------------
    void SceneGraph::addNode( SceneNode* node )
    {
        if ( node )
        {
            //mNodeRegistOrderList.push_back( node_ );
            //mSceneNodeList.push_back( node_ );
            //mSceneNodeAdditionalList.push_back( node_ );
            //node_->addRef();
			mSceneNodeList2.registerObject( node );

            //mSceneShaderManager->addSceneNodeToOffscreenScene( node );
        }
    }

	//----------------------------------------------------------------------
	// �� (SceneNode �̃f�X�g���N�^����Ă΂��)
	//----------------------------------------------------------------------
    void SceneGraph::onDeleteNode( SceneNode* node_ )
    {
        // update() �ŎQ�ƃJ�E���g���`�F�b�N���č폜����悤�ɂ����̂ŁA
        // �����ł̓��X�g����O���Ȃ�
        //mSceneNodeList.remove( node_ );

        // ���O�ƁA���g�̃|�C���^�Ɉ�v����v�f����菜��
        LRefTString old_name = node_->getName();
        if ( old_name.size() > 0 )
        {
            SceneNodeNameMap::iterator it  = mNodeNameMap.lower_bound( old_name );
            SceneNodeNameMap::iterator end = mNodeNameMap.end();
            if ( it != end )
            {
                SceneNodeNameMap::iterator up = mNodeNameMap.upper_bound( old_name );
                for ( ; it != up; ++it )
                {
                    if ( it->second == node_ )
                    {
                        mNodeNameMap.erase( it );
                        break;  // ���ׂď����ꍇ�͂�����O���� it ���Ď擾����悤�ɂ���
                    }
                }
            }
        }

        //mSceneShaderManager->removeSceneNodeFromOffscreenScene( node_ );
    }

	//----------------------------------------------------------------------
	// �� SceneNode �̖��O��ύX���钼�O�ɌĂяo�� (SceneNode �� setName() ����Ă΂��)
	//----------------------------------------------------------------------
    void SceneGraph::onSetNodeName( SceneNode* node_, const lnChar* old_name_, const lnChar* new_name_ )
    {
        // �O�̖��O�ƁA���g�̃|�C���^�Ɉ�v����v�f����菜��
        if ( _tcslen( old_name_ ) )
        {
            SceneNodeNameMap::iterator it  = mNodeNameMap.lower_bound( old_name_ );
            SceneNodeNameMap::iterator end = mNodeNameMap.end();
            if ( it != end )
            {
                SceneNodeNameMap::iterator up = mNodeNameMap.upper_bound( old_name_ );
                for ( ; it != up; ++it )
                {
                    if ( it->second == node_ )
                    {
                        mNodeNameMap.erase( it );
                        break;  // ���ׂď����ꍇ�͂�����O���� it ���Ď擾����悤�ɂ���
                    }
                }
            }
        }

        // �V�������O�� map �ɒǉ�
        if ( _tcslen( new_name_ ) )
        {
            mNodeNameMap.insert( SceneNodeNamePair( new_name_, node_ ) );
        }
    }

	//----------------------------------------------------------------------
	// �� �L�����C�g�̒����烉�C�g��񋓂��� 
	//----------------------------------------------------------------------
    void SceneGraph::enumLights( LightArray* out_, const LVector3& ref_pos_ )
    {
        // �\�[�g��l�̐ݒ�
        ActiveLightDataArray::iterator itr = mActiveLightDataArray.begin();
        ActiveLightDataArray::iterator end = mActiveLightDataArray.end();
        for ( ; itr != end; ++itr )
        {
            itr->Distance = LVector3::squareLength( itr->ActiveLight->getPosition() - ref_pos_ );//itr->DistanceToCamera;
        }

        // �\�[�g (�擪���珇�ɁA�D��x���A��l�̏���)
        std::sort( mActiveLightDataArray.begin(), mActiveLightDataArray.end(), _cmpLights );

        // �o�� (����Ȃ����� NULL �Ŗ��߂�)
        int mn = mActiveLightDataArray.size();
        int n  = out_->size();
        int i = 0;
        for ( ; i < mn; ++i )
        {
            (*out_)[ i ] = mActiveLightDataArray[ i ].ActiveLight;
        }
        for ( ;i < n; ++i )
        {
            (*out_)[ i ] = NULL;
        }
    }

	//----------------------------------------------------------------------
	// �� ���C�g�񋓗p�̔�r�֐�
	//----------------------------------------------------------------------
    bool SceneGraph::_cmpLights( const ActiveLightData& l_, const ActiveLightData& r_ )
    {
        if ( l_.ActiveLight->getLightingPriority() == r_.ActiveLight->getLightingPriority() )
        {
            // ����
            return l_.Distance <= r_.Distance;
        }
        // �~��
        return l_.ActiveLight->getLightingPriority() >= r_.ActiveLight->getLightingPriority();
    }

//==============================================================================
// �� SceneGraphContext
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    SceneGraphContext::SceneGraphContext()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void SceneGraphContext::initialize( SceneGraph* scene_ )
    {
        mSceneGraph = scene_;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void SceneGraphContext::updateContext()
    {
		mMMESceneObjectArray.clear();
        mCameraContextArray.clear();
        mLightContextArray.clear();

//        mVirtualViewSize = mSceneGraph->mVirtualViewSize;

		// Object
		SceneGraph::SceneNodeObjectArray::ObjectArray& nodeArray = 
			mSceneGraph->mSceneNodeList2.getObjectArray();
		int n = nodeArray.size();
		mMMESceneObjectArray.resize( n );
		for ( int i = 0; i < n; ++i )
        {
            mMMESceneObjectArray[i] = nodeArray[i]->getContext();
        }
		this->mSceneNodeNameMap = mSceneGraph->mNodeNameMap;
		
		// Camera
		SceneGraph::CameraObjectArray::ObjectArray& cameraArray = 
			mSceneGraph->mCameraObjectArray.getObjectArray();
        n = cameraArray.size();
        mCameraContextArray.resize( n );
        for ( int i = 0; i < n; ++i )
        {
            mCameraContextArray[i] = cameraArray[i]->getCameraContext();
        }

		// Light
		SceneGraph::LightObjectArray::ObjectArray& lightArray = 
			mSceneGraph->mLightObjectArray.getObjectArray();
        n = lightArray.size();
        mLightContextArray.resize( n );
        for ( int i = 0; i < n; ++i )
        {
            mLightContextArray[i] = lightArray[i]->getLightContext();
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void SceneGraphContext::selectLights( MMELightArray* out_, const LVector3& ref_pos_ )
    {
        int request_num = out_->size();
        int active_num = mActiveLightContextArray.size();

        if ( request_num >= active_num )
        {
            int i = 0;
            for (; i < active_num; ++i )
            {
                (*out_)[ i ] = mActiveLightContextArray[ i ];
            }
            for ( ;i < request_num; ++i )
            {
                (*out_)[ i ] = &LightContext::DUMMY;
            }
        }
        else
        {
            // �\�[�g��l�̐ݒ�
            ln_foreach( LightContext* lc, mActiveLightContextArray)
            {
                lc->DistanceForSort = LVector3::squareLength( lc->getPosition() - ref_pos_ );
            }

            // �\�[�g (�擪���珇�ɁA�D��x���A��l�̏���)
            std::sort( mActiveLightContextArray.begin(), mActiveLightContextArray.end(), _cmpLights );

            // �o��
            for ( int i = 0; i < request_num; ++i )
            {
                (*out_)[ i ] = mActiveLightContextArray[ i ];
            }
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SceneGraphContext::render( ContextDrawParam& param )
	{
		param.SceneGraphContext = this;

        //-----------------------------------------------------
        //  ������̍쐬

		const LVector2& view_size = param.CameraContext->getViewSize();

		// 3D
        if ( !param.CameraContext->isProjection2D() )
        {
            mActiveViewFrustum.create(
                param.CameraContext->getFovY(),
                view_size.x / view_size.y,
                param.CameraContext->getNearClip(),
                param.CameraContext->getFarClip() );
        }
		// 2D
        else
        {
            mActiveViewFrustum.create2DProjection(
                view_size.x,
                view_size.y,
                param.CameraContext->getNearClip(),
                param.CameraContext->getFarClip() );
        }

        //-----------------------------------------------------
        // �G�t�F�N�g�ւ̍s��ݒ�

		if ( mSceneGraph->mEffectManager ) {
			mSceneGraph->mEffectManager->setViewProjection(
				param.CameraContext->getViewMatrix(), 
				param.CameraContext->getProjectionMatrix()/*, 
				!param.CameraContext->isProjection2D()*/ );
		}

        //-----------------------------------------------------
        // �V�F�[�_�ւ̃J�����f�[�^�ݒ�

		mSceneGraph->mSceneShaderManager->updateAllShaderCameraParam( 
			param.CameraContext->getCameraSceneParam() );
		
		

        //-----------------------------------------------------
        // ���C�g��������J�����O

        mActiveLightContextArray.clear();

        ln_foreach( LightContext* lc, mLightContextArray )
        {
            if ( lc->isEnable() )
            {
                // �f�B���N�V���i�����C�g�̏ꍇ�͂��̂܂ܒǉ�
                if ( lc->getType() == LN_LIGHTTYPE_DIRECTIONAL )
                {
                    mActiveLightContextArray.push_back( lc );
                }
                // �|�C���g�A�X�|�b�g�͗L���������܂߂Ď�����ɓ����Ă��邩���m�F
                else
                {
                    if ( mActiveViewFrustum.checkInside( lc->getPosition(), lc->getRange() ) )
                    {
                        mActiveLightContextArray.push_back( lc );
                    }
                }
            }
        }

		

        //-----------------------------------------------------
        // �m�[�h�̃R���e�L�X�g�ɂ��ʒm

        // TODO:�J�����O��̃m�[�h������������悤�ɂ���Ώ����œK���ł��邩��
        ln_foreach( SceneNode* node, mSceneGraph->mSceneNodeList2.getObjectArray() )
        {
            node->getContext()->preRender( param );
        }

		//-----------------------------------------------------
		// OffscreenScene �`�� (���ʒu)
		//	�Ƃ肠�����ApreRender() �ŃJ�����g�̃J������ݒ肵����łȂ���� NULL �A�N�Z�X�ŗ�����

		// TODO:�Ƃ肠����3d�̂݋���
		if ( !param.CameraContext->isProjection2D() )
		{
			mSceneGraph->getSceneShaderManager()->renderAllOffscreenScene(
				this, param, param.RenderRootNode->getContext() );
		}

		param.RenderRootNode->getContext()->render( param );
	}
#if 0
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void SceneGraphContext::OnSetActivePaneContext( PaneContext* pane_context_, bool is_3d_ )
    {
        //-----------------------------------------------------
        //  �J�����I��

        CameraContext* camera_context_3d;
        CameraContext* camera_context_2d;
        pane_context_->getCameraContext( &camera_context_3d, &camera_context_2d );
        CameraContext* camera_context = (is_3d_) ? camera_context_3d : camera_context_2d;

        //-----------------------------------------------------
        //  �v���W�F�N�V�����s��E������̍쐬

        LMatrix proj;
        const LVector2& view_size = pane_context_->getViewSize();
        if ( is_3d_ )
        {
            LMatrix::perspectiveFovLH( 
                &proj, 
                camera_context->getFovY(), 
                view_size.x, 
                view_size.y, 
                camera_context->getNearClip(), 
                camera_context->getFarClip() );

            mActiveViewFrustum.create(
                camera_context->getFovY(),
                view_size.x / view_size.y,
                camera_context->getNearClip(),
                camera_context->getFarClip() );
        }
        else
        {
            LMatrix::perspective2DLH( 
                &proj, 
                view_size.x, 
                view_size.y, 
                camera_context->getNearClip(), 
                camera_context->getFarClip() );

            mActiveViewFrustum.create2DProjection(
                view_size.x,
                view_size.y,
                camera_context->getNearClip(),
                camera_context->getFarClip() );
        }

        camera_context->setProjectionMatrix( proj );

        //-----------------------------------------------------
        // �G�t�F�N�g�ւ̍s��ݒ�

        mSceneGraph->mEffectManager->setViewProjection(
            camera_context->getViewMatrix(), proj, is_3d_ );

        //-----------------------------------------------------
        // �V�F�[�_�ւ̃J�����f�[�^�ݒ�

        mSceneGraph->mSceneShaderManager->updateAllShaderCameraParam( camera_context->getCameraSceneParam() );

        //-----------------------------------------------------
        // ���C�g��������J�����O

        mActiveLightContextArray.clear();

        ln_foreach( LightContext* lc, mLightContextArray )
        {
            if ( lc->isEnable() )
            {
                // �f�B���N�V���i�����C�g�̏ꍇ�͂��̂܂ܒǉ�
                if ( lc->getType() == LN_LIGHTTYPE_DIRECTIONAL )
                {
                    mActiveLightContextArray.push_back( lc );
                }
                // �|�C���g�A�X�|�b�g�͗L���������܂߂Ď�����ɓ����Ă��邩���m�F
                else
                {
                    if ( mActiveViewFrustum.checkInside( lc->getPosition(), lc->getRange() ) )
                    {
                        mActiveLightContextArray.push_back( lc );
                    }
                }
            }
        }

        //-----------------------------------------------------
        // �m�[�h�̃R���e�L�X�g�ɂ��ʒm

        // TODO:�J�����O��̃m�[�h������������悤�ɂ���Ώ����œK���ł��邩��
        ln_foreach( SceneNode* node, mSceneGraph->mSceneNodeList2.getObjectArray() )
        {
            node->getContext()->OnSetActivePaneContext( this, pane_context_, camera_context );
        }
    }
#endif

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	MMELightBase* SceneGraphContext::getDefaultMMELight() 
	{ 
		return static_cast<MMELightBase*>( mSceneGraph->getDefaultLight()->getLightContext() ); 
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	MMESceneObjectBase* SceneGraphContext::findMMESceneObject( const lnChar* name )
	{
		SceneNodeNameMap::iterator itr = mSceneNodeNameMap.find( name );
		if ( itr != mSceneNodeNameMap.end() )
		{
			return itr->second->getContext();
		}
		return NULL;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool SceneGraphContext::_cmpLights( const LightContext* l_, const LightContext* r_ )
    {
        if ( l_->getLightingPriority() == r_->getLightingPriority() )
        {
            // ����
            return l_->DistanceForSort <= r_->DistanceForSort;
        }
        // �~��
        return l_->getLightingPriority() >= r_->getLightingPriority();
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================