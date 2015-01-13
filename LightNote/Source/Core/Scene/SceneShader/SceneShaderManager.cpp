//==============================================================================
// SceneShaderManager 
//------------------------------------------------------------------------------
/*
REFERENCE

	■OffscreenScene
		描画はカメラ単位。
		また、Light はデフォルトライトのみ参照可能。

	■shared キーワードについて
		アノテーションでテクスチャ作成情報を記述できるのはいずれか1か所のみ。
		既に作成されている同じ型名、名前、セマンティクスでアノテーションを記述した場合はエラー。
*/
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Resource/LNResource.h"
#include "../../Graphics/Common/GraphicsUtil.h"
#include "../SceneGraph.h"
#include "../SceneNode/Light.h"
#include "../Pane.h"	// 必要なくなるはず
#include "SceneShader.h"
#include "SceneShaderTechnique.h"
#include "OffscreenScene.h"
#include "SceneShaderManager.h"

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
// ■ SceneShaderManager
//==============================================================================

    struct LNVerNameDesc
    {
        const lnChar* Semantic;
        const lnChar* Variable;   ///< シェーダ固定時に使う変数名
    };

    static LNVerNameDesc gMMESemanticNames[ MME_MAX_SEMANTICS ] =
    {
        { _T( "NONE" ), NULL },  // ダミー
        { _T( "WORLD" ),                                _T( "gMatrix_w" ) },
        { _T( "VIEW" ),                                 _T( "gMatrix_v" ) },
        { _T( "PROJECTION" ),                           _T( "gMatrix_p" ) },
        { _T( "WORLDVIEW" ),                            _T( "gMatrix_wv" ) },
        { _T( "VIEWPROJECTION" ),                       _T( "gMatrix_vp" ) },
        { _T( "WORLDVIEWPROJECTION" ),                  _T( "gMatrix_wvp" ) },      // [ 6 ]

        { _T( "WORLDINVERSE" ),                         _T( "gMatrix_wi" ) },
        { _T( "VIEWINVERSE" ),                          _T( "gMatrix_vi" ) },
        { _T( "PROJECTIONINVERSE" ),                    _T( "gMatrix_pi" ) },
        { _T( "WORLDVIEWINVERSE" ),                     _T( "gMatrix_wvi" ) },
        { _T( "VIEWPROJECTIONINVERSE" ),                _T( "gMatrix_vpi" ) },
        { _T( "WORLDVIEWPROJECTIONINVERSE" ),           _T( "gMatrix_wvpi" ) },     // [ 12 ]

        { _T( "WORLDTRANSPOSE" ),                       _T( "gMatrix_wt" ) },
        { _T( "VIEWTRANSPOSE" ),                        _T( "gMatrix_vt" ) },
        { _T( "PROJECTIONTRANSPOSE" ),                  _T( "gMatrix_pt" ) },
        { _T( "WORLDVIEWTRANSPOSE" ),                   _T( "gMatrix_wvt" ) },
        { _T( "VIEWPROJECTIONTRANSPOSE" ),              _T( "gMatrix_vpt" ) },
        { _T( "WORLDVIEWPROJECTIONTRANSPOSE" ),         _T( "gMatrix_wvpt" ) },     // [ 18 ]
         
        { _T( "WORLDINVERSETRANSPOSE" ),                _T( "gMatrix_wit" ) },
        { _T( "VIEWINVERSETRANSPOSE" ),                 _T( "gMatrix_vit" ) },
        { _T( "PROJECTIONINVERSETRANSPOSE" ),           _T( "gMatrix_pit" ) },
        { _T( "WORLDVIEWINVERSETRANSPOSE" ),            _T( "gMatrix_wvit" ) },
        { _T( "VIEWPROJECTIONINVERSETRANSPOSE" ),       _T( "gMatrix_vpit" ) },
        { _T( "WORLDVIEWPROJECTIONINVERSETRANSPOSE" ),  _T( "gMatrix_wvpit" ) },    // [ 24 ]

        { _T( "DIFFUSE" ),                              _T( "gDiffuse" ) },
        { _T( "AMBIENT" ),                              _T( "gAmbient" ) },
        { _T( "EMISSIVE" ),                             _T( "gEmissive" ) },
        { _T( "SPECULAR" ),                             _T( "gSpecular" ) },
        { _T( "SPECULARPOWER" ),                        _T( "gSpecularPower" ) },
        { _T( "TOONCOLOR" ),                            _T( "gToonColor" ) },
        { _T( "EDGECOLOR" ),                            _T( "gEdgeColor" ) },
        { _T( "POSITION" ),                             _T( "gLightPosition" ) },
        { _T( "DIRECTION" ),                            _T( "gLightDirection" ) },
        { _T( "MATERIALTEXTURE" ),                      _T( "gMaterialTexture" ) },
        { _T( "MATERIALSPHEREMAP" ),        NULL },
        { _T( "VIEWPORTPIXELSIZE" ),        NULL },
        { _T( "TIME" ),                     NULL },
        { _T( "ELAPSEDTIME" ),              NULL },
        { _T( "MOUSEPOSITION" ),            NULL },
        { _T( "LEFTMOUSEDOWN" ),            NULL },
        { _T( "MIDDLEMOUSEDOWN" ),          NULL },
        { _T( "RIGHTMOUSEDOWN" ),           NULL },
        { _T( "CONTROLOBJECT" ),            NULL },
        { _T( "RENDERCOLORTARGET" ),        NULL },
        { _T( "RENDERDEPTHSTENCILTARGET" ), NULL },
        { _T( "ANIMATEDTEXTURE" ),          NULL },
        { _T( "OFFSCREENRENDERTARGET" ),    NULL },
        { _T( "TEXTUREVALUE" ),             NULL },
        { _T( "STANDARDSGLOBAL" ),          NULL },

		// MMM
		//{ _T("LIGHTWVPMATRICES"),	NULL },
		//{ _T("LIGHTPOSITIONS"),		NULL },
		//{ _T("LIGHTDIRECTIONS"),	NULL },
		//{ _T("LIGHTENABLES"),		NULL },
		//{ _T("LIGHTZFARS"),			NULL },

        // 以下は独自仕様
        //{ _T( "ORTHOGRAPHIC" ),    _T( "gMatrix_wt" ) },
        { _T( "PROJECTION2D" ),     _T( "gMatrix_p2D" ) },
        { _T( "OPACITY" ),          _T( "gOpacity" ) },
        { _T( "COLORSCALE" ),       _T( "gColorScale" ) },
        { _T( "BLENDCOLOR" ),       _T( "gBlendColor" ) },
        { _T( "TONE" ),             _T( "gTone" ) },
        { _T( "UVTRANSFORM" ),      _T( "gUVTransform" ) }
    };

    static const char* gMMEAnnotationNames[ MME_MAX_ANNOTATIONS ] =
    {
        "NONE",
        "OBJECT",
        "SYNCINEDITMODE",

        "NAME",
        "ITEM",

        "RESOURCETYPE",
        "RESOURCENAME",
        "WIDTH",
        "HEIGHT",
        "DEPTH",
        "DIMENSIONS",
        "VIEWPORTRATIO",
        "FORMAT",
        "MIPLEVELS",
        "LEVELS",

        "OFFSET",
        "SPEED",
        "SEEKVARIABLE",

        "CLEARCOLOR",
        "CLEARDEPTH",
        "ANTIALIAS",
        "DESCRIPTION",
        "DEFAULTEFFECT",

        "TEXTURENAME",

        "SCRIPTOUUTPUT",
        "SCRIPTCLASS",
        "SCRIPTORDER",
        
        "SCRIPT"
    };

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    SceneShaderManager::SceneShaderManager(/* MMESceneBase* scene */)
        : /*mScene            ( scene )
        , */mFileManager      ( NULL )
        , mGraphicsDevice   ( NULL )
        , mPlayStartTime    ( 0.0f )
    {
        //LN_SAFE_ADDREF( mScene );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    SceneShaderManager::~SceneShaderManager()
    {
        //LN_SAFE_RELEASE( mScene );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void SceneShaderManager::initialize( const ConfigData& configData )
    {
        mFileManager     = configData.FileManager;
        mGraphicsManager = configData.GraphicsManager;
        mGraphicsDevice  = configData.GraphicsManager->getGraphicsDevice();
        mPlayStartTime   = 0.0f;

        // シェーダキャッシュ作成
		Base::CacheManager::initialize( configData.ShaderCacheSize, 0 );

        // セマンティクス名と識別番号の対応表を作る
        // ※対応表は変数検索の際、総当たりを避けるためのもの
        for ( int i = 0; i < MME_MAX_SEMANTICS; ++i )
        {
            // セマンティクス名を使う
            if ( configData.UseSemantics )
            {
                mMMESemanticsMap.insert( MMESemanticsPair( gMMESemanticNames[ i ].Semantic, (MMESemantics)i ) );
            }
            // 内部で定義された変数名を使う
            else
            {
                if ( gMMESemanticNames[ i ].Variable )
                {
                    mMMESemanticsMap.insert( MMESemanticsPair( gMMESemanticNames[ i ].Variable, (MMESemantics)i ) );
                }
            }
        }
        // アノテーション名と識別番号の対応表を作る
        for ( int i = 0; i < MME_MAX_ANNOTATIONS; ++i )
        {
            mMMEAnnotationMap.insert( MMEAnnotationPair( gMMEAnnotationNames[ i ], (MMEAnnotation)i ) );
        }

#ifdef LNOTE_DEBUG
        printf("MMEShaderVariable::ControlObject の設定を忘れそう！注意\n");
#endif       
		for ( int i = 0; i < MaxOffscreenScenes; ++i )
		{
			mOffscreenSceneIDStack.push( i );
		}
    }

	//----------------------------------------------------------------------
	// ● 終了処理
	//----------------------------------------------------------------------
    void SceneShaderManager::finalize()
    {
		//mOffscreenSceneList.clear();

        ln_foreach( SceneShader* shader, mSceneShaderList )
        {
            shader->release();
        }
        mSceneShaderList.clear();

        Base::CacheManager::finalize();
    }

	//----------------------------------------------------------------------
	// ● シェーダの作成
	//----------------------------------------------------------------------
	//LNRESULT SceneShaderManager::createSceneShader( SceneShader** shader_, const void* data_, lnU32 size_, const lnChar* name_ )
	//{
	//    return _createSceneShader( shader_, data_, size_, name_ );
	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	SceneShader* SceneShaderManager::createSceneShader(const lnChar* fulePath, lnString* errors)
    {
		LRefPtr<FileIO::InFile> file(
			mFileManager->createInFile( fulePath ) );
       

        //mFileManager->moveCurrentDirectory( FileIO::Path::getDirectoryPath( fulePath ).c_str() );

		LRefPtr<SceneShader> shader(
			_createSceneShader(file, fulePath, errors));
    
        // 読み込み・ファイル処理が終わった後、必要なリソースを確保する
		if ( shader.getPtr() )
        {
			shader->refreshResource( getBaseViewSize() );
        }

        // 元のパスに戻す
		//mFileManager->returnCurrentDirectory();

		shader.addRef();
		return shader;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	SceneShader* SceneShaderManager::createSceneShader( int dataResourceID, const lnSharingKey& key )
	{
        // キャッシュを検索
        SceneShader* t = (SceneShader*)Base::CacheManager::findCacheAddRef( key );
        if ( t ) {
            return t;
        }

        // 新しく作る
		LRefPtr<SceneShader> shader( LN_NEW SceneShader( this ) );
		shader->create( dataResourceID );
		
		// 読み込み・ファイル処理が終わった後、必要なリソースを確保する
		shader->refreshResource( getBaseViewSize() );

		// キャッシュへ登録
        if ( shader->isShareable() ) {
            Base::CacheManager::registerCachedObject( key, shader );
        }

		// 管理リストへ
		Threading::ScopedLock lock( mShaderListMutex );
        mSceneShaderList.push_back( shader );
		shader->addRef();

		shader.addRef();
        return shader;
	}

	//----------------------------------------------------------------------
	// ● 解放が必要なシェーダがないかチェックする
	//----------------------------------------------------------------------
    void SceneShaderManager::updateShaderList()
    {
		Threading::ScopedLock lock( mShaderListMutex );

        SceneShaderList::iterator itr = mSceneShaderList.begin();
        SceneShaderList::iterator end = mSceneShaderList.end();
        while ( itr != end )
        {
            if ( (*itr)->getRefCount() == 1 )
            {
                (*itr)->release();
                itr = mSceneShaderList.erase( itr );
				end = mSceneShaderList.end();
                continue;
            }
            ++itr;
        }
    }

	//----------------------------------------------------------------------
	// ● 全シェーダにシーン単位で設定するべきデータを設定する
	//----------------------------------------------------------------------
	void SceneShaderManager::updateAllShaderGlobalParam(double totalTime, float elapsedTime, MMESceneBase* scene)
    {
        // とりあえずリアルタイムじゃなくてゲームタイムで計算
		mMMESceneParam.Time = static_cast< lnFloat >(totalTime - mPlayStartTime);
		mMMESceneParam.ElapsedTime = elapsedTime;

        //-----------------------------------------------------
        // Scene 単位でシェーダに設定する必要がるデータをまとめる (ここではデフォルトライト)
        // この辺ローカル変数なので描画コマンド実行はこの関数の中で行うこと。

        MMELightBase* light = scene->getDefaultMMELight();

        mMMESceneParam.Mat_LightView     = &light->getViewMatrix();   
        mMMESceneParam.Mat_LightProj     = &light->getProjectionMatrix();
        mMMESceneParam.Mat_LightViewProj = &light->getViewProjectionMatrix();

        mMMESceneParam.LightPosition     = LVector4( light->getPosition() );
        mMMESceneParam.LightDirection    = LVector4( light->getDirection() );

        mMMESceneParam.LightDiffuse      = (LVector4*)&light->getDiffuseColor();
        mMMESceneParam.LightAmbient      = (LVector4*)&light->getAmbientColor();
        mMMESceneParam.LightSpecular     = (LVector4*)&light->getSpecularColor();

        mMMESceneParam.calcMatrices();

		
		Threading::ScopedLock lock( mShaderListMutex );

        // すべてのシェーダにシーン単位の設定項目を送り、
        // ついでに CONTROLOBJECT を要求している変数の値を更新する
        SceneShaderList::iterator itr = mSceneShaderList.begin();
        SceneShaderList::iterator end = mSceneShaderList.end();
        for ( ; itr != end; ++itr )
        {
            (*itr)->updateSceneParam( mMMESceneParam, scene );
            //(*itr)->updateControlObjectVariables();
        }
    }

	//----------------------------------------------------------------------
	// ● 全ェーダにカメラ単位で設定するべきデータを設定する
	//----------------------------------------------------------------------
    void SceneShaderManager::updateAllShaderCameraParam( const LNCameraSceneParam& param_ )
    {
		Threading::ScopedLock lock( mShaderListMutex );

        SceneShaderList::iterator itr = mSceneShaderList.begin();
        SceneShaderList::iterator end = mSceneShaderList.end();
        for ( ; itr != end; ++itr )
        {
            (*itr)->updateCameraParam( param_ );
        }
    }


	////---------------------------------------------------------------------
	////
	////---------------------------------------------------------------------
	//void SceneShaderManager::addSceneObjectToOffscreenScene( MMESceneObjectBase* obj )
	//{
	//    ln_foreach( OffscreenScene* ofs, mOffscreenSceneList )
	//    {
	//        ofs->onAddSceneObject( obj );
	//    }
	//}

	////---------------------------------------------------------------------
	////
	////---------------------------------------------------------------------
	//void SceneShaderManager::removeSceneObjectFromOffscreenScene( MMESceneObjectBase* obj )
	//{
	//    ln_foreach( OffscreenScene* ofs, mOffscreenSceneList )
	//    {
	//        ofs->onRemoveSceneNode( obj );
	//    }
	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SceneShaderManager::renderAllOffscreenScene( 
		MMESceneBase* scene, 
		ContextDrawParam& param, 
		MMESceneObjectBase* renderRootNode )
	{
		Threading::ScopedLock lock( mObjectListMutex );
		ln_foreach( OffscreenScene* ofs, mOffscreenSceneList )
        {
			ofs->render( scene, param, renderRootNode );
        }
	}

	//----------------------------------------------------------------------
	// ● (SceneShader 内から呼ばれる)
	//----------------------------------------------------------------------
    LNRESULT SceneShaderManager::createTexture(
        Graphics::Texture** texture_,
        MMEVariableRequest req_,
        //const lnChar* resource_name_,
		FileIO::PathNameW resourcePath,
        lnU32 width_,
        lnU32 height_,
        lnU32 depth_,
        lnU32 miplevels_,
        Graphics::SurfaceFormat format_)
    {
		LN_THROW(depth_ <= 1, Base::ArgumentException, "3D texture is not supported.");	// 3D テクスチャは未対応です

        switch ( req_ )
        {
            /////////////////////////////////////// 普通テクスチャ
            case MME_VARREQ_TEXTURE:
            {
                // テクスチャ名がある場合はそれを読む
				if (resourcePath.isEmpty()) {

					//*texture_ = mGraphicsDevice->createTexture(resourcePath.getPath(), 0, miplevels_, format_, lnSharingKey(resourcePath.getPath()));
					*texture_ = Graphics::Util::createTexture(mGraphicsManager, resourcePath);
                }
                // ファイル名がない場合はサイズ指定
                else {
                    *texture_ = mGraphicsDevice->createTexture( width_, height_, miplevels_, format_ );
                }
                break;
            }
            /////////////////////////////////////// レンダーターゲット
            case MME_VARREQ_RENDERCOLORTARGET:
            {
                *texture_ = mGraphicsDevice->createRenderTarget( width_, height_, miplevels_, format_ );
                break;
            }
            /////////////////////////////////////// 深度バッファ
            case MME_VARREQ_RENDERDEPTHSTENCILTARGET:
            {
                // アノテーションにフォーマットの指定がない場合は D24S8
                if ( format_ == Graphics::SurfaceFormat_A8R8G8B8 )
                {
                    format_ = Graphics::SurfaceFormat_D24S8;
                }
                *texture_ = mGraphicsDevice->createDepthBuffer( width_, height_, format_ );
                break;
            }
            /////////////////////////////////////// アニメテクスチャ
            case MME_VARREQ_ANIMATEDTEXTURE:
            {
                printf( "ANIMATEDTEXTURE の読み込みは未対応です。\n" );
                break;
            }
            /////////////////////////////////////// オフスクリーン RT
            case MME_VARREQ_OFFSCREENRENDERTARGET:
            {
                *texture_ = mGraphicsDevice->createRenderTarget( width_, height_, miplevels_, format_ );
                break;
            }
        }
		return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	// ● メッセージ処理 (Manager から呼ばれる)
	//----------------------------------------------------------------------
    bool SceneShaderManager::onEvent( const System::EventArgs& e )
    {
        const System::MouseEventArgs& ev_mouse = (const System::MouseEventArgs&)(e);

		//TOOD:マウス座標の座標変換が必要

        // シェーダに送るマウスデータ
        switch ( e.Type )
        {
            case LNEV_MOUSE_DOWN:
            {
                switch ( ev_mouse.Button )
                {
                    case LN_MOUSE_LEFT:
                        mMMESceneParam.LeftMouseDown.z = 0.0f;
                        break;
                    case LN_MOUSE_RIGHT:
                        mMMESceneParam.RightMouseDown.z = 0.0f;
                        break;
                    case LN_MOUSE_MIDDLE:
                        mMMESceneParam.MiddleMouseDown.z = 0.0f;
                        break;
                }
                break;
            }
            case LNEV_MOUSE_UP:
            {
                switch ( ev_mouse.Button )
                {
                    case LN_MOUSE_LEFT:
                        mMMESceneParam.LeftMouseDown.set(
                            static_cast< lnFloat >( ev_mouse.X ),
                            static_cast< lnFloat >( ev_mouse.Y ),
                            1.0f,
                            mMMESceneParam.Time );
                        break;
                    case LN_MOUSE_RIGHT:
                        mMMESceneParam.RightMouseDown.set(
                            static_cast< lnFloat >( ev_mouse.X ),
                            static_cast< lnFloat >( ev_mouse.Y ),
                            1.0f,
                            mMMESceneParam.Time );
                        break;
                    case LN_MOUSE_MIDDLE:
                        mMMESceneParam.MiddleMouseDown.set(
                            static_cast< lnFloat >( ev_mouse.X ),
                            static_cast< lnFloat >( ev_mouse.Y ),
                            1.0f,
                            mMMESceneParam.Time );
                        break;
                }
                break;
            }
            case LNEV_MOUSE_MOVE:
            {
                mMMESceneParam.MousePosition.set(
                    static_cast< lnFloat >( ev_mouse.X ),
                    static_cast< lnFloat >( ev_mouse.Y ),
                    0, 0 );
                break;
            }
        }

		return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SceneShaderManager::addMMETextureObject( MMETextureObject* tex ) 
	{
		Threading::ScopedLock lock( mObjectListMutex );
		mMMETextureObjectArray.push_back( tex ); 
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SceneShaderManager::removeMMETextureObject( MMETextureObject* tex ) 
	{
		Threading::ScopedLock lock( mObjectListMutex );
		Base::STLUtils::remove( mMMETextureObjectArray, tex ); 
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SceneShaderManager::addOffscreenScene( OffscreenScene* ofs ) 
	{
		Threading::ScopedLock lock( mObjectListMutex );

		// ID 割り当て
		ofs->setOffscreenSceneID( mOffscreenSceneIDStack.top() );
		mOffscreenSceneIDStack.pop();

		//mOffscreenSceneList.registerObject( ofs ); 
		mOffscreenSceneList.push_back( ofs );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SceneShaderManager::removeOffscreenScene( OffscreenScene* ofs ) 
	{ 
		Threading::ScopedLock lock( mObjectListMutex );

		// ID 返却
		mOffscreenSceneIDStack.push( ofs->getOffscreenSceneID() );
		
		//mOffscreenSceneList.unregisterObject( ofs ); 
		Base::STLUtils::remove( mOffscreenSceneList, ofs );
	}

	//----------------------------------------------------------------------
	// ● シェーダの作成
	//----------------------------------------------------------------------
	LNRESULT SceneShaderManager::_createSceneShader(SceneShader** shader_, const void* data_, lnU32 size_, const lnChar* name_, lnString* errors)
    {
        lnSharingKey key = lnSharingKey( name_ );
        *shader_ = NULL;

        // キャッシュを検索
        *shader_ = (SceneShader*)Base::CacheManager::findCacheAddRef( key );
        if ( *shader_ )
        {
			return ResultCode_OK;
        }

        // 新しく作る
        //SceneShader::InitData data;
        //data.ShaderData         = data_;
        //data.ShaderDataSize     = size_;
        //data.ShaderName         = name_;
        SceneShader* shader = LN_NEW SceneShader( this );
		shader->create(name_, errors);

        if ( shader->isShareable() )
        {
            Base::CacheManager::registerCachedObject( key, shader );
        }

		
		Threading::ScopedLock lock( mShaderListMutex );
        mSceneShaderList.push_back( shader );
        shader->addRef();

        *shader_ = shader;
		return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	SceneShader* SceneShaderManager::_createSceneShader(FileIO::Stream* stream, const lnChar* key, lnString* errors)
    {
        lnU32 size = stream->getSize();

        lnByte* buf = mTempBuffer.lock( size );

        // mTempBuffer に全部読み込む
        stream->read( buf, size );

		SceneShader* shader;
		_createSceneShader(&shader, buf, size, key, errors);

        mTempBuffer.unlock();
        return shader;
    }

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================