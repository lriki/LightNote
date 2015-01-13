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
// ■ SceneGraph
//==============================================================================

	//----------------------------------------------------------------------
	// ● コンストラクタ
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
	// ● デストラクタ
	//----------------------------------------------------------------------
    SceneGraph::~SceneGraph()
    {
        SAFE_DELETE( mSceneGraphContext );

        // SceneShaderManager は SceneNode のデストラクタからいろいろ経由して
        // オフスクリーン RT からノードを外すところまで使う。
        // そのため、finalize ではなく一番最後のデストラクタで開放する
        LN_SAFE_RELEASE( mSceneShaderManager );
        LN_SAFE_RELEASE( mManager );
    }

	//----------------------------------------------------------------------
	// ● 初期化
	//----------------------------------------------------------------------
    LNRESULT SceneGraph::initialize( const InitData& init_data_ )
    {
        mFileManager        = init_data_.FileManager;
        mGraphicsManager    = init_data_.GraphicsManager;
        mGeometryRenderer   = mGraphicsManager->getGeometryRenderer();
        mEffectManager      = init_data_.EffectManager;
        mPhysicsManager     = init_data_.PhysicsManager;
        //mVirtualViewSize    = init_data_.VirtualViewSize;

		// 描画情報
		mSceneGraphContext = LN_NEW SceneGraphContext();
        mSceneGraphContext->initialize( this );
		
        // シェーダ管理クラス
        SceneShaderManager::ConfigData ss_data;
        ss_data.FileManager     = init_data_.FileManager;
        ss_data.GraphicsManager = init_data_.GraphicsManager;
        ss_data.ShaderCacheSize = init_data_.ShaderCacheSize;
        ss_data.UseSemantics    = true;
        mSceneShaderManager = LN_NEW SceneShaderManager();
        mSceneShaderManager->initialize( ss_data );
		mSceneShaderManager->setBaseViewSize( init_data_.VirtualViewSize );
		
        // デフォルトのシェーダプログラムを作る 
        mDefaultShaderArray[ LN_DEFSHADER_BASIC_2D ] = 
			mSceneShaderManager->createSceneShader( Resource::ID_Shader_SSBasic2D, lnSharingKey( _T("ID_Shader_SSBasic2D") ) );
		
		mDefaultShaderArray[ LN_DEFSHADER_BASIC_3D ] = 
			mSceneShaderManager->createSceneShader( Resource::ID_Shader_SSBasic3D, lnSharingKey( _T("ID_Shader_SSBasic3D") ) );

        mDefaultShaderArray[ LN_DEFSHADER_NOLIGHTING ] = 
			mSceneShaderManager->createSceneShader( Resource::ID_Shader_SSNoLighting, lnSharingKey( _T("ID_Shader_SSNoLighting") ) );
		
        // デフォルトのノード
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
		
        // デフォルトのカメラ
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

		
        // デフォルトのライト
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
	// ● デフォルトで作成されるカメラの取得 (0:3D 1:2D)
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
		// カメラ
		ln_foreach( Camera* c, mCameraObjectArray.getObjectArray() )
        {
            c->updateFrame( gameTime );
        }
#endif

		// 全ノードのフレーム更新
		ln_foreach( SceneNode* n, mSceneNodeList2.getObjectArray() )
        {
			if (n->isEnableAutoUpdate()) n->update(mElapsedTime);
		}

		// 追加要求されているノードもここで更新してしまう
		ln_foreach( SceneNode* n, mSceneNodeList2.getRegistOrderObjectArray() )
        {
			if (n->isEnableAutoUpdate()) n->update(mElapsedTime);
        }
		
        mSceneShaderManager->updateShaderList();

        // ワールド行列を階層的に更新する
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

		// 追加要求されているノードも更新する。
		// (SpritePath なんかは作った後すぐにでも DrawRequest() するだろうし)
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
	// ● メッセージ処理 (Manager から呼ばれる)
	//----------------------------------------------------------------------
    bool SceneGraph::onEvent( const System::EventArgs& e )
    {
        if ( mSceneShaderManager->onEvent( e ) ) return true;

		
		
        // カメラのイベント処理
		CameraList::iterator itr = mCameraObjectArray.getObjectArray().begin();
		CameraList::iterator end = mCameraObjectArray.getObjectArray().end();
        for ( ; itr != end; ++itr )
        {
            if ( (*itr)->onEvent( e ) ) return true;
        }

        return false;
    }

	//----------------------------------------------------------------------
	// ● (SceneNode のコンストラクタから呼ばれる。ツリーではなく管理リストに追加する)
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
	// ● (SceneNode のデストラクタから呼ばれる)
	//----------------------------------------------------------------------
    void SceneGraph::onDeleteNode( SceneNode* node_ )
    {
        // update() で参照カウントをチェックして削除するようにしたので、
        // ここではリストから外さない
        //mSceneNodeList.remove( node_ );

        // 名前と、自身のポインタに一致する要素を取り除く
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
                        break;  // すべて除く場合はこれを外して it を再取得するようにする
                    }
                }
            }
        }

        //mSceneShaderManager->removeSceneNodeFromOffscreenScene( node_ );
    }

	//----------------------------------------------------------------------
	// ● SceneNode の名前を変更する直前に呼び出す (SceneNode の setName() から呼ばれる)
	//----------------------------------------------------------------------
    void SceneGraph::onSetNodeName( SceneNode* node_, const lnChar* old_name_, const lnChar* new_name_ )
    {
        // 前の名前と、自身のポインタに一致する要素を取り除く
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
                        break;  // すべて除く場合はこれを外して it を再取得するようにする
                    }
                }
            }
        }

        // 新しい名前で map に追加
        if ( _tcslen( new_name_ ) )
        {
            mNodeNameMap.insert( SceneNodeNamePair( new_name_, node_ ) );
        }
    }

	//----------------------------------------------------------------------
	// ● 有効ライトの中からライトを列挙する 
	//----------------------------------------------------------------------
    void SceneGraph::enumLights( LightArray* out_, const LVector3& ref_pos_ )
    {
        // ソート基準値の設定
        ActiveLightDataArray::iterator itr = mActiveLightDataArray.begin();
        ActiveLightDataArray::iterator end = mActiveLightDataArray.end();
        for ( ; itr != end; ++itr )
        {
            itr->Distance = LVector3::squareLength( itr->ActiveLight->getPosition() - ref_pos_ );//itr->DistanceToCamera;
        }

        // ソート (先頭から順に、優先度高、基準値の昇順)
        std::sort( mActiveLightDataArray.begin(), mActiveLightDataArray.end(), _cmpLights );

        // 出力 (足りない分は NULL で埋める)
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
	// ● ライト列挙用の比較関数
	//----------------------------------------------------------------------
    bool SceneGraph::_cmpLights( const ActiveLightData& l_, const ActiveLightData& r_ )
    {
        if ( l_.ActiveLight->getLightingPriority() == r_.ActiveLight->getLightingPriority() )
        {
            // 昇順
            return l_.Distance <= r_.Distance;
        }
        // 降順
        return l_.ActiveLight->getLightingPriority() >= r_.ActiveLight->getLightingPriority();
    }

//==============================================================================
// ■ SceneGraphContext
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
            // ソート基準値の設定
            ln_foreach( LightContext* lc, mActiveLightContextArray)
            {
                lc->DistanceForSort = LVector3::squareLength( lc->getPosition() - ref_pos_ );
            }

            // ソート (先頭から順に、優先度高、基準値の昇順)
            std::sort( mActiveLightContextArray.begin(), mActiveLightContextArray.end(), _cmpLights );

            // 出力
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
        //  視錐台の作成

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
        // エフェクトへの行列設定

		if ( mSceneGraph->mEffectManager ) {
			mSceneGraph->mEffectManager->setViewProjection(
				param.CameraContext->getViewMatrix(), 
				param.CameraContext->getProjectionMatrix()/*, 
				!param.CameraContext->isProjection2D()*/ );
		}

        //-----------------------------------------------------
        // シェーダへのカメラデータ設定

		mSceneGraph->mSceneShaderManager->updateAllShaderCameraParam( 
			param.CameraContext->getCameraSceneParam() );
		
		

        //-----------------------------------------------------
        // ライトを視錐台カリング

        mActiveLightContextArray.clear();

        ln_foreach( LightContext* lc, mLightContextArray )
        {
            if ( lc->isEnable() )
            {
                // ディレクショナルライトの場合はそのまま追加
                if ( lc->getType() == LN_LIGHTTYPE_DIRECTIONAL )
                {
                    mActiveLightContextArray.push_back( lc );
                }
                // ポイント、スポットは有効距離を含めて視錐台に入っているかを確認
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
        // ノードのコンテキストにも通知

        // TODO:カリング後のノードだけ処理するようにすれば少し最適化できるかも
        ln_foreach( SceneNode* node, mSceneGraph->mSceneNodeList2.getObjectArray() )
        {
            node->getContext()->preRender( param );
        }

		//-----------------------------------------------------
		// OffscreenScene 描画 (仮位置)
		//	とりあえず、preRender() でカレントのカメラを設定した後でなければ NULL アクセスで落ちる

		// TODO:とりあえず3dのみ許可
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
        //  カメラ選択

        CameraContext* camera_context_3d;
        CameraContext* camera_context_2d;
        pane_context_->getCameraContext( &camera_context_3d, &camera_context_2d );
        CameraContext* camera_context = (is_3d_) ? camera_context_3d : camera_context_2d;

        //-----------------------------------------------------
        //  プロジェクション行列・視錐台の作成

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
        // エフェクトへの行列設定

        mSceneGraph->mEffectManager->setViewProjection(
            camera_context->getViewMatrix(), proj, is_3d_ );

        //-----------------------------------------------------
        // シェーダへのカメラデータ設定

        mSceneGraph->mSceneShaderManager->updateAllShaderCameraParam( camera_context->getCameraSceneParam() );

        //-----------------------------------------------------
        // ライトを視錐台カリング

        mActiveLightContextArray.clear();

        ln_foreach( LightContext* lc, mLightContextArray )
        {
            if ( lc->isEnable() )
            {
                // ディレクショナルライトの場合はそのまま追加
                if ( lc->getType() == LN_LIGHTTYPE_DIRECTIONAL )
                {
                    mActiveLightContextArray.push_back( lc );
                }
                // ポイント、スポットは有効距離を含めて視錐台に入っているかを確認
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
        // ノードのコンテキストにも通知

        // TODO:カリング後のノードだけ処理するようにすれば少し最適化できるかも
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
            // 昇順
            return l_->DistanceForSort <= r_->DistanceForSort;
        }
        // 降順
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