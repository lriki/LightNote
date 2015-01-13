//==============================================================================
// SceneShaderManager 
//------------------------------------------------------------------------------
/*
REFERENCE

	��OffscreenScene
		�`��̓J�����P�ʁB
		�܂��ALight �̓f�t�H���g���C�g�̂ݎQ�Ɖ\�B

	��shared �L�[���[�h�ɂ���
		�A�m�e�[�V�����Ńe�N�X�`���쐬�����L�q�ł���̂͂����ꂩ1�����̂݁B
		���ɍ쐬����Ă��铯���^���A���O�A�Z�}���e�B�N�X�ŃA�m�e�[�V�������L�q�����ꍇ�̓G���[�B
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
#include "../Pane.h"	// �K�v�Ȃ��Ȃ�͂�
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
// �� SceneShaderManager
//==============================================================================

    struct LNVerNameDesc
    {
        const lnChar* Semantic;
        const lnChar* Variable;   ///< �V�F�[�_�Œ莞�Ɏg���ϐ���
    };

    static LNVerNameDesc gMMESemanticNames[ MME_MAX_SEMANTICS ] =
    {
        { _T( "NONE" ), NULL },  // �_�~�[
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

        // �ȉ��͓Ǝ��d�l
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

        // �V�F�[�_�L���b�V���쐬
		Base::CacheManager::initialize( configData.ShaderCacheSize, 0 );

        // �Z�}���e�B�N�X���Ǝ��ʔԍ��̑Ή��\�����
        // ���Ή��\�͕ϐ������̍ہA�������������邽�߂̂���
        for ( int i = 0; i < MME_MAX_SEMANTICS; ++i )
        {
            // �Z�}���e�B�N�X�����g��
            if ( configData.UseSemantics )
            {
                mMMESemanticsMap.insert( MMESemanticsPair( gMMESemanticNames[ i ].Semantic, (MMESemantics)i ) );
            }
            // �����Œ�`���ꂽ�ϐ������g��
            else
            {
                if ( gMMESemanticNames[ i ].Variable )
                {
                    mMMESemanticsMap.insert( MMESemanticsPair( gMMESemanticNames[ i ].Variable, (MMESemantics)i ) );
                }
            }
        }
        // �A�m�e�[�V�������Ǝ��ʔԍ��̑Ή��\�����
        for ( int i = 0; i < MME_MAX_ANNOTATIONS; ++i )
        {
            mMMEAnnotationMap.insert( MMEAnnotationPair( gMMEAnnotationNames[ i ], (MMEAnnotation)i ) );
        }

#ifdef LNOTE_DEBUG
        printf("MMEShaderVariable::ControlObject �̐ݒ��Y�ꂻ���I����\n");
#endif       
		for ( int i = 0; i < MaxOffscreenScenes; ++i )
		{
			mOffscreenSceneIDStack.push( i );
		}
    }

	//----------------------------------------------------------------------
	// �� �I������
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
	// �� �V�F�[�_�̍쐬
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
    
        // �ǂݍ��݁E�t�@�C���������I�������A�K�v�ȃ��\�[�X���m�ۂ���
		if ( shader.getPtr() )
        {
			shader->refreshResource( getBaseViewSize() );
        }

        // ���̃p�X�ɖ߂�
		//mFileManager->returnCurrentDirectory();

		shader.addRef();
		return shader;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	SceneShader* SceneShaderManager::createSceneShader( int dataResourceID, const lnSharingKey& key )
	{
        // �L���b�V��������
        SceneShader* t = (SceneShader*)Base::CacheManager::findCacheAddRef( key );
        if ( t ) {
            return t;
        }

        // �V�������
		LRefPtr<SceneShader> shader( LN_NEW SceneShader( this ) );
		shader->create( dataResourceID );
		
		// �ǂݍ��݁E�t�@�C���������I�������A�K�v�ȃ��\�[�X���m�ۂ���
		shader->refreshResource( getBaseViewSize() );

		// �L���b�V���֓o�^
        if ( shader->isShareable() ) {
            Base::CacheManager::registerCachedObject( key, shader );
        }

		// �Ǘ����X�g��
		Threading::ScopedLock lock( mShaderListMutex );
        mSceneShaderList.push_back( shader );
		shader->addRef();

		shader.addRef();
        return shader;
	}

	//----------------------------------------------------------------------
	// �� ������K�v�ȃV�F�[�_���Ȃ����`�F�b�N����
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
	// �� �S�V�F�[�_�ɃV�[���P�ʂŐݒ肷��ׂ��f�[�^��ݒ肷��
	//----------------------------------------------------------------------
	void SceneShaderManager::updateAllShaderGlobalParam(double totalTime, float elapsedTime, MMESceneBase* scene)
    {
        // �Ƃ肠�������A���^�C������Ȃ��ăQ�[���^�C���Ōv�Z
		mMMESceneParam.Time = static_cast< lnFloat >(totalTime - mPlayStartTime);
		mMMESceneParam.ElapsedTime = elapsedTime;

        //-----------------------------------------------------
        // Scene �P�ʂŃV�F�[�_�ɐݒ肷��K�v����f�[�^���܂Ƃ߂� (�����ł̓f�t�H���g���C�g)
        // ���̕Ӄ��[�J���ϐ��Ȃ̂ŕ`��R�}���h���s�͂��̊֐��̒��ōs�����ƁB

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

        // ���ׂẴV�F�[�_�ɃV�[���P�ʂ̐ݒ荀�ڂ𑗂�A
        // ���ł� CONTROLOBJECT ��v�����Ă���ϐ��̒l���X�V����
        SceneShaderList::iterator itr = mSceneShaderList.begin();
        SceneShaderList::iterator end = mSceneShaderList.end();
        for ( ; itr != end; ++itr )
        {
            (*itr)->updateSceneParam( mMMESceneParam, scene );
            //(*itr)->updateControlObjectVariables();
        }
    }

	//----------------------------------------------------------------------
	// �� �S�F�[�_�ɃJ�����P�ʂŐݒ肷��ׂ��f�[�^��ݒ肷��
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
	// �� (SceneShader ������Ă΂��)
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
		LN_THROW(depth_ <= 1, Base::ArgumentException, "3D texture is not supported.");	// 3D �e�N�X�`���͖��Ή��ł�

        switch ( req_ )
        {
            /////////////////////////////////////// ���ʃe�N�X�`��
            case MME_VARREQ_TEXTURE:
            {
                // �e�N�X�`����������ꍇ�͂����ǂ�
				if (resourcePath.isEmpty()) {

					//*texture_ = mGraphicsDevice->createTexture(resourcePath.getPath(), 0, miplevels_, format_, lnSharingKey(resourcePath.getPath()));
					*texture_ = Graphics::Util::createTexture(mGraphicsManager, resourcePath);
                }
                // �t�@�C�������Ȃ��ꍇ�̓T�C�Y�w��
                else {
                    *texture_ = mGraphicsDevice->createTexture( width_, height_, miplevels_, format_ );
                }
                break;
            }
            /////////////////////////////////////// �����_�[�^�[�Q�b�g
            case MME_VARREQ_RENDERCOLORTARGET:
            {
                *texture_ = mGraphicsDevice->createRenderTarget( width_, height_, miplevels_, format_ );
                break;
            }
            /////////////////////////////////////// �[�x�o�b�t�@
            case MME_VARREQ_RENDERDEPTHSTENCILTARGET:
            {
                // �A�m�e�[�V�����Ƀt�H�[�}�b�g�̎w�肪�Ȃ��ꍇ�� D24S8
                if ( format_ == Graphics::SurfaceFormat_A8R8G8B8 )
                {
                    format_ = Graphics::SurfaceFormat_D24S8;
                }
                *texture_ = mGraphicsDevice->createDepthBuffer( width_, height_, format_ );
                break;
            }
            /////////////////////////////////////// �A�j���e�N�X�`��
            case MME_VARREQ_ANIMATEDTEXTURE:
            {
                printf( "ANIMATEDTEXTURE �̓ǂݍ��݂͖��Ή��ł��B\n" );
                break;
            }
            /////////////////////////////////////// �I�t�X�N���[�� RT
            case MME_VARREQ_OFFSCREENRENDERTARGET:
            {
                *texture_ = mGraphicsDevice->createRenderTarget( width_, height_, miplevels_, format_ );
                break;
            }
        }
		return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	// �� ���b�Z�[�W���� (Manager ����Ă΂��)
	//----------------------------------------------------------------------
    bool SceneShaderManager::onEvent( const System::EventArgs& e )
    {
        const System::MouseEventArgs& ev_mouse = (const System::MouseEventArgs&)(e);

		//TOOD:�}�E�X���W�̍��W�ϊ����K�v

        // �V�F�[�_�ɑ���}�E�X�f�[�^
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

		// ID ���蓖��
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

		// ID �ԋp
		mOffscreenSceneIDStack.push( ofs->getOffscreenSceneID() );
		
		//mOffscreenSceneList.unregisterObject( ofs ); 
		Base::STLUtils::remove( mOffscreenSceneList, ofs );
	}

	//----------------------------------------------------------------------
	// �� �V�F�[�_�̍쐬
	//----------------------------------------------------------------------
	LNRESULT SceneShaderManager::_createSceneShader(SceneShader** shader_, const void* data_, lnU32 size_, const lnChar* name_, lnString* errors)
    {
        lnSharingKey key = lnSharingKey( name_ );
        *shader_ = NULL;

        // �L���b�V��������
        *shader_ = (SceneShader*)Base::CacheManager::findCacheAddRef( key );
        if ( *shader_ )
        {
			return ResultCode_OK;
        }

        // �V�������
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

        // mTempBuffer �ɑS���ǂݍ���
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