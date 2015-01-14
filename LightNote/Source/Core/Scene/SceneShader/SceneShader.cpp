//==============================================================================
// SceneShader 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Resource/LNResource.h"
#include "../../Graphics/Common/GraphicsUtil.h"
#include "../SceneGraph.h"
#include "../SceneNode/SceneNode.h"
#include "../SceneNode/Light.h"
#include "../SceneNode/Viewport.h"
#include "../Pane.h"	// �K�v�Ȃ��Ȃ�͂�
#include "OffscreenScene.h"
#include "SceneShaderTechnique.h"
#include "SceneShader.h"
#include "SceneShaderUtil.h"
#include "MMETextureObject.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// SceneShader
//==============================================================================

	LN_TYPE_INFO_ACCESS_IMPL(SceneShader);

	//static const char MME_MMM_EffectHeader[] = 
	//	"#include <LNResource\\Shader\\MMM_EffectHeader.h>\n";

	//----------------------------------------------------------------------
	// �� �R���X�g���N�^
	//----------------------------------------------------------------------
    SceneShader::SceneShader( SceneShaderManager* manager_ )
        : mManager          ( manager_ )
        , mShader           ( NULL )
        , mRequiredLightNum ( 0 )
        , mScriptOutput     ( MME_SCROUT_color )
        , mScriptClass      ( MME_SCRCLS_object )
        , mScriptOrder      ( MME_SCRORDER_standard )
        , mSelfSceneNode    ( NULL )
        , mIsError          ( false )
        , mShareable        ( true )
    {
        LN_SAFE_ADDREF( mManager );
    }

	//----------------------------------------------------------------------
	// �� �f�X�g���N�^
	//----------------------------------------------------------------------
    SceneShader::~SceneShader()
    {
#if 0
        if ( mManager && isPostEffect() )
        {
            MMEViewportArray& list = mManager->getScene()->getViewportList();
            ln_foreach( MMEViewport* vp, list )
            {
                vp->removePostEffectShader( this );
            }
        }
#endif 
        _release();
        LN_SAFE_RELEASE( mShader );
        LN_SAFE_RELEASE( mManager );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//LNRESULT SceneShader::initialize( const InitData& init_data_ )
	void SceneShader::create(const lnChar* filePath, lnString* errors)
    {
		mFilePath = FileIO::PathNameW(mManager->getFileManager()->getCurrentDirectory(), filePath);
		//mManager->getFileManager()->getFullPath( mFileFullPath, filePath );
        //mSelfSceneNode = init_data_.SelfNode;
        mIsError = true;    // �ŏ��̓G���[�ɂ��Ă����B���������������� false

//		lnString data = MME_MMM_EffectHeader;

		LRefPtr<FileIO::InFile> file(mManager->getFileManager()->createInFile(mFilePath.getPath()));
		//file->load();
		//lnString data = (char*)file->getData();

		
        // �V�F�[�_�v���O���������
        //  (���L�� Scene ���ł��Ă�̂ŁA�����ł� key_ �̎w��͖����� OK)
        try
        {
#if 1
			//mShader = mManager->getGraphicsDevice()->createShader( data.c_str(), data.size() + 1 );
			//mShader = mManager->getGraphicsDevice()->createShader( init_data_.ShaderData, init_data_.ShaderDataSize );
			mShader = mManager->getGraphicsDevice()->createShader(file, lnSharingKey(), errors);
#else
            mShader = mManager->getGraphicsDevice()->createShader( mName.c_str() );
#endif
		}
        catch ( Base::Exception& e )
        {
            if ( mShader )
            {
                mErrorMessage = mShader->getCompileErrors();
            }
            else
            {
                mErrorMessage = e.getMessage();
            }
            throw;
            //return LN_ERR_ASSERT;
        }

        if ( mShader->getCompileResult() != LN_SHADERRESULT_SUCCESS )
        {
            mErrorMessage = mShader->getCompileErrors();
			_p(mErrorMessage.c_str());

			// TODO:
			//LN_THROW_InvalidCall( 0, mErrorMessage.c_str() );
        }

		//mFileManager->moveCurrentDirectory( FileIO::Path::getDirectoryPath( fulePath ).c_str() )
		//mFileFullPath

		// �J�����g�f�B���N�g���ړ�
		//FileIO::ScopedMoveCurrentDirectory moveDir( 
		//	mManager->getFileManager(),
		//	FileIO::Path::getDirectoryPath( mFileFullPath ).c_str() );

        // �ϐ��A�e�N�j�b�N�����\�z
		_build();
        //LN_CALL_R( _build() );

        mIsError = false;
        //return LN_OK;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SceneShader::create( int dataResourceID )
	{
		mIsError = true;    // �ŏ��̓G���[�ɂ��Ă���

		//memset( mFileFullPath, 0, sizeof(mFileFullPath) );
		try {			
			lnString errors;	// TODO
			mShader = Graphics::Util::createShader(mManager->getGraphicsManager(), dataResourceID, &errors);
		}
        catch ( Base::Exception& e ) {
			mErrorMessage = e.getMessage();
			throw;
		}

		if ( mShader->getCompileResult() != LN_SHADERRESULT_SUCCESS ) {
			mErrorMessage = mShader->getCompileErrors();
		}

		// �J�����g�f�B���N�g���ړ�
		//FileIO::ScopedMoveCurrentDirectory moveDir( 
		//	mManager->getFileManager(),
		//	FileIO::Path::getDirectoryPath( mFileFullPath ).c_str() );

        // �ϐ��A�e�N�j�b�N�����\�z
		_build();

		mIsError = false;
	}

	//----------------------------------------------------------------------
	// �� �ēǂݍ���
	//----------------------------------------------------------------------
    LNRESULT SceneShader::reload()
    {
        LN_PRINT_NOT_IMPL_FUNCTION;

        mIsError = true;
		return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool SceneShader::hasScriptExternal()
	{
		ln_foreach( SceneShaderTechnique* tech, mTechniqueList )
		{
			if ( tech->isPostProcess() ) {
				return true;
			}
		}
		return false;
	}

	//----------------------------------------------------------------------
	// �� Scene �P�ʂŕK�v�ȃp�����[�^��ݒ肷�� (�{�͕̂`�抮���܂ŉ�����Ȃ�����)
	//----------------------------------------------------------------------
    void SceneShader::updateSceneParam( const MMESceneParam& param_, MMESceneBase* scene )
    {
        MMEShaderVariable* sv;
        Graphics::IShaderVariable* var;
    
        ln_foreach( sv, mShaderVariableArray )
        {
            //sv = ( mShaderVarArray + i );

            // �ϐ����z��̃��C�g���͂��̊֐����ł͐ݒ肵�Ȃ��̂Ŏ���
			if ( sv->LightNum >= 1 && sv->LightParamIsArray)
            {
                continue;
            }

            var = sv->Variable;

			// CONTROLOBJECT
			if ( MME_VARREQ_CONTROLOBJECT_BEGIN <= sv->Request && sv->Request <= MME_VARREQ_CONTROLOBJECT_END ) {
				if ( sv->ObjectName == _T("(self)") ) {
					//LN_THROW_NotImpl( 0 );
				}
				else if ( sv->ObjectName == _T("(OffscreenOwner)") ) {
					//LN_THROW_NotImpl( 0 );
				}
				else {
					MMESceneObjectBase* obj = scene->findMMESceneObject( sv->ObjectName.c_str() );
					SceneShaderUtil::setControllObjectParam( sv, obj );
				}
				continue;
			}

            switch ( sv->Request )
            {
                // �s��
                case MME_VARREQ_MATRIX_LIGHT_View:
                    var->setMatrix( *param_.Mat_LightView );
                    break;
                case MME_VARREQ_MATRIX_LIGHT_Proj:
                    var->setMatrix( *param_.Mat_LightProj );
                    break;
                case MME_VARREQ_MATRIX_LIGHT_ViewProj:
                    var->setMatrix( *param_.Mat_LightViewProj );
                    break;

                // �t�s��
                case MME_VARREQ_MATRIX_LIGHT_View_I:
                    var->setMatrix( param_.Mat_LightView_Inverse );
                    break;
                case MME_VARREQ_MATRIX_LIGHT_Proj_I:
                    var->setMatrix( param_.Mat_LightView_Inverse );
                    break;
                case MME_VARREQ_MATRIX_LIGHT_ViewProj_I:
                    var->setMatrix( param_.Mat_LightView_Inverse );
                    break;

                // �]�u�s��
                case MME_VARREQ_MATRIX_LIGHT_View_T:
                    var->setMatrix( param_.Mat_LightView_Transpose );
                    break;
                case MME_VARREQ_MATRIX_LIGHT_Proj_T:
                    var->setMatrix( param_.Mat_LightView_Transpose );
                    break;
                case MME_VARREQ_MATRIX_LIGHT_ViewProj_T:
                    var->setMatrix( param_.Mat_LightView_Transpose );
                    break;

                // �t�s��~�]�u�s��
                case MME_VARREQ_MATRIX_LIGHT_View_IT:
                    var->setMatrix( param_.Mat_LightView_InverseTranspose );
                    break;
                case MME_VARREQ_MATRIX_LIGHT_Proj_IT:
                    var->setMatrix( param_.Mat_LightView_InverseTranspose );
                    break;
                case MME_VARREQ_MATRIX_LIGHT_ViewProj_IT:
                    var->setMatrix( param_.Mat_LightView_InverseTranspose );
                    break;

                // ���C�g�̐F
                case MME_VARREQ_LIGHT_DIFFUSE:
					var->setVector( *param_.LightDiffuse );
                    break;
                case MME_VARREQ_LIGHT_AMBIENT:
					var->setVector( *param_.LightAmbient );
                    break;
                case MME_VARREQ_LIGHT_SPECULAR:
					var->setVector( *param_.LightSpecular );
                    break;

                // ���C�g�̈ʒu�E����
                case MME_VARREQ_LIGHT_POSITION:
					var->setVector( param_.LightPosition );
                    break;
                case MME_VARREQ_LIGHT_DIRECTION:
					var->setVector( param_.LightDirection );
                    break;

                // ���̑�
                case MME_VARREQ_TIME:
                    var->setFloat( param_.Time );
                    break;
                case MME_VARREQ_ELAPSEDTIME:
                    var->setFloat( param_.ElapsedTime );
                    break;
                case MME_VARREQ_MOUSEPOSITION:
                    var->setVector( param_.MousePosition );
                    break;
                case MME_VARREQ_LEFTMOUSEDOWN:
                    var->setVector( param_.LeftMouseDown );
                    break;
                case MME_VARREQ_MIDDLEMOUSEDOWN:
                    var->setVector( param_.MiddleMouseDown );
                    break;
                case MME_VARREQ_RIGHTMOUSEDOWN:
                    var->setVector( param_.RightMouseDown );
                    break;
            }
        }

    }

	//----------------------------------------------------------------------
	// �� Camera �P�ʂŕK�v�ȃp�����[�^��ݒ肷��
	//----------------------------------------------------------------------
    void SceneShader::updateCameraParam( const LNCameraSceneParam& param_ )
    {
        MMEShaderVariable* sv;
        Graphics::IShaderVariable* var;
        
        ln_foreach( sv, mShaderVariableArray )
        {
            var = sv->Variable;

            switch ( sv->Request )
            {
                // �s��
                case MME_VARREQ_MATRIX_CAMERA_View:
                    var->setMatrix( *param_.Mat_CameraView );
                    break;
                case MME_VARREQ_MATRIX_CAMERA_Proj:
                    var->setMatrix( *param_.Mat_CameraProj );
                    break;
                case MME_VARREQ_MATRIX_CAMERA_ViewProj:
                    var->setMatrix( *param_.Mat_CameraViewProj );
                    break;

                // �t�s��
                case MME_VARREQ_MATRIX_CAMERA_View_I:
                    var->setMatrix( param_.Mat_CameraView_Inverse );
                    break;
                case MME_VARREQ_MATRIX_CAMERA_Proj_I:
                    var->setMatrix( param_.Mat_CameraProj_Inverse );
                    break;
                case MME_VARREQ_MATRIX_CAMERA_ViewProj_I:
                    var->setMatrix( param_.Mat_CameraViewProj_Inverse );
                    break;

                // �]�u�s��
                case MME_VARREQ_MATRIX_CAMERA_View_T:
                    var->setMatrix( param_.Mat_CameraView_Transpose );
                    break;
                case MME_VARREQ_MATRIX_CAMERA_Proj_T:
                    var->setMatrix( param_.Mat_CameraView_Transpose );
                    break;
                case MME_VARREQ_MATRIX_CAMERA_ViewProj_T:
                    var->setMatrix( param_.Mat_CameraView_Transpose );
                    break;
               
                // �t�s��~�]�u�s��
                case MME_VARREQ_MATRIX_CAMERA_View_IT:
                    var->setMatrix( param_.Mat_CameraView_InverseTranspose );
                    break;

                case MME_VARREQ_MATRIX_CAMERA_Proj_IT:
                    var->setMatrix( param_.Mat_CameraView_InverseTranspose );
                    break;

                case MME_VARREQ_MATRIX_CAMERA_ViewProj_IT:
                    var->setMatrix( param_.Mat_CameraView_InverseTranspose );
                    break;
                // �J�����̈ʒu�E����
                case MME_VARREQ_CAMERA_POSITION:
					//if (param_.CameraPosition.y == 0)
					{
						//_p(mName.c_str());
						//param_.CameraPosition.dump();
					}
                    var->setVector( param_.CameraPosition );
                    break;
                case MME_VARREQ_CAMERA_DIRECTION:
                    var->setVector( param_.CameraDirection );
                    break;

                // ���ˉe�s��
                //case LN_VARREQ_MATRIX_CAMERA_ORTHOGRAPHIC:
                //    var->setMatrix( param_.Mat_CameraOrtho );
                //    break;
					
				// �`���RT�T�C�Y
                case MME_VARREQ_VIEWPORTPIXELSIZE:
                    var->setVector( param_.ViewPixelSize );
                    break;
            }
        }
    }

	//----------------------------------------------------------------------
	// �� Node �P�ʂŕK�v�ȃp�����[�^��ݒ肷��
	//----------------------------------------------------------------------
    LNRESULT SceneShader::updateNodeParam( const MMESceneObjectBase* param_ )// const NodeRenderParam& param_ )
    {
        MMEShaderVariable* sv;
        
        ln_foreach( sv, mShaderVariableArray )
        {
			// CONTROLOBJECT
			if ( MME_VARREQ_CONTROLOBJECT_BEGIN <= sv->Request && sv->Request <= MME_VARREQ_CONTROLOBJECT_END ) {
				if ( sv->ObjectName == _T("(self)") ) {
					SceneShaderUtil::setControllObjectParam( sv, param_ );
				}
				else if ( sv->ObjectName == _T("(OffscreenOwner)") ) {
					//LN_THROW_NotImpl( 0 );
				}
				else {
					// �V�[���P�ʂ̃p�����[�^�ݒ�Őݒ�ς�
				}
				continue;
			}

            // ���m�Ȕ͈͂���Ȃ����ǁA�����ł���������̉񐔂����炷���߂�
            if ( MME_VARREQ_GEOMETRY_MATRIX_BEGIN <= sv->Request && sv->Request <= MME_VARREQ_GEOMETRY_MATRIX_END )
            {
                // ���C�g�̏�񂪕K�v�ł���
                if ( sv->LightNum > 0 )
                {
                    // setMatrixArray() �Őݒ肷��K�v�����鍀��
					if ( sv->LightParamIsMatrix && sv->LightParamIsArray )
                    {
                        Base::TempBuffer* buf = mManager->getTempBuffer();
                        LMatrix* matrices = reinterpret_cast< LMatrix* >( buf->lock( sizeof( LMatrix ) * sv->LightNum ) );
 
                        for ( lnU32 i = 0; i < sv->LightNum; ++i )
                            param_->getObjectMatrix( &matrices[ i ], sv->Request, i );

                        sv->Variable->setMatrixArray( matrices, sv->LightNum );

                        buf->unlock();
                    }
					// setMatrix() �Őݒ肷��K�v�����鍀��
                    else if ( sv->LightParamIsMatrix && !sv->LightParamIsArray )
                    {
						LMatrix mat;
						param_->getObjectMatrix( &mat, sv->Request, 0 );
						sv->Variable->setMatrix( mat );
					}
                    // setVectorArray() �Őݒ肷��K�v�����鍀��
                    else if ( !sv->LightParamIsArray && sv->LightParamIsArray )
                    {
                        const MMELightArray& lights = param_->getAffectLightArray();

                        Base::TempBuffer* buf = mManager->getTempBuffer();
                        LVector4* vectors = reinterpret_cast< LVector4* >( buf->lock( sizeof( LVector4 ) * sv->LightNum ) );
                        
                        switch ( sv->Request )
                        {
                            case MME_VARREQ_LIGHT_DIFFUSE:
                                for ( lnU32 i = 0; i < sv->LightNum; ++i )
                                    vectors[ i ] = static_cast< const LVector4& >( lights[ i ]->getDiffuseColor() );
                                break;
                            case MME_VARREQ_LIGHT_AMBIENT:
                                for ( lnU32 i = 0; i < sv->LightNum; ++i )
                                    vectors[ i ] = static_cast< const LVector4& >( lights[ i ]->getAmbientColor() );
                                break;
                            case MME_VARREQ_LIGHT_SPECULAR:
                                for ( lnU32 i = 0; i < sv->LightNum; ++i )
                                    vectors[ i ] = static_cast< const LVector4& >( lights[ i ]->getSpecularColor() );
                                break;
                            case MME_VARREQ_LIGHT_POSITION:
                                for ( lnU32 i = 0; i < sv->LightNum; ++i )
                                {
                                    const LVector3& v = lights[ i ]->getPosition();
                                    vectors[ i ].Set( v.X, v.Y, v.Z, 0.0f );
                                }
                                break;
                            case MME_VARREQ_LIGHT_DIRECTION:
                                for ( lnU32 i = 0; i < sv->LightNum; ++i )
                                {
                                    const LVector3& v = lights[ i ]->getDirection();
                                    vectors[ i ].Set( v.X, v.Y, v.Z, 0.0f );
                                }
                                break;
                        }

						sv->Variable->setVectorArray( vectors, sv->LightNum );
                        buf->unlock();
                    }
					// �s��^�A�z��^�ł͂Ȃ� (setVector())
					else
					{
						const MMELightArray& lights = param_->getAffectLightArray();
						switch ( sv->Request )
                        {
                            case MME_VARREQ_LIGHT_DIFFUSE:
								sv->Variable->setVector( static_cast< const LVector4& >( lights[0]->getDiffuseColor() ) );
                                break;
                            case MME_VARREQ_LIGHT_AMBIENT:
								sv->Variable->setVector( static_cast< const LVector4& >( lights[0]->getAmbientColor() ) );
                                break;
                            case MME_VARREQ_LIGHT_SPECULAR:
								sv->Variable->setVector( static_cast< const LVector4& >( lights[0]->getSpecularColor() ) );
                                break;
                            case MME_VARREQ_LIGHT_POSITION:
							{
								const LVector3& v = lights[0]->getPosition();
								LVector4 vec( v, 0.0f );
								sv->Variable->setVector( vec );
                                break;
							}
                            case MME_VARREQ_LIGHT_DIRECTION:
							{
								const LVector3& v = lights[0]->getDirection();
								LVector4 vec( v, 0.0f );
								sv->Variable->setVector( vec );
                                break;
							}
                        }
					}
                }
                // ���C�g�ȊO
                else
                {
					
                    LMatrix mat;
                    if ( param_->getObjectMatrix( &mat, sv->Request, 0 ) )
                    {
						//if ( sv->Request == MME_VARREQ_MATRIX_CAMERA_WorldViewProj )
						//{
						//	_p(sv->Variable->getName());
						//	mat.cdump();
						//	//_p(sv->Request);
						//}

                        sv->Variable->setMatrix( mat );
                    }
                }
            }
        }

		return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	// �� Subset �P�ʂŕK�v�ȃp�����[�^��ݒ肷��
	//----------------------------------------------------------------------
    void SceneShader::updateSubsetParam( const LNSubsetRenderParam& param_ )
    {
        MMEShaderVariable* sv;
        Graphics::IShaderVariable* var;

        LVector4 temp_vec;

        ln_foreach( sv, mShaderVariableArray )
        {
            var = sv->Variable;

            switch ( sv->Request )
            {
                // �f�B�t���[�Y�F�i�g�U���j
                case MME_VARREQ_OBJECT_DIFFUSE:
                    var->setVector( param_.Material.Diffuse );
                    break;

                // �A���r�G���g�F�i�����j
                case MME_VARREQ_OBJECT_AMBIENT:
                    var->setVector( param_.Material.Ambient );
                    break;

                // �G�~�b�V�����F�i���ˌ��j
                case MME_VARREQ_OBJECT_EMISSIVE:
                    var->setVector( param_.Material.Emissive );
                    break;

                // �X�y�L�����F�i���ʌ��j
                case MME_VARREQ_OBJECT_SPECULAR:
                    var->setVector( param_.Material.Specular );
                    break;

                // �X�y�L�����̋��x
                case MME_VARREQ_OBJECT_SPECULARPOWER:
                    var->setFloat( param_.Material.Power );
                    break;

                // �g�D�[���F
                case MME_VARREQ_OBJECT_TOONCOLOR:
                    var->setVector( param_.ToonColor );
                    break;

                // �֊s�F
                case MME_VARREQ_OBJECT_EDGECOLOR:
                    var->setVector( param_.EdgeColor );
                    break;

                // �}�e���A���ɐݒ肳��Ă���e�N�X�`��
                case MME_VARREQ_OBJECT_MATERIALTEXTURE:
                    if ( param_.Material.getTexture() )
                    {
                        var->setTexture( param_.Material.getTexture() );
                    }
                    // �e�N�X�`�����Ȃ���΃_�~�[��ݒ�
                    else
                    {
                        var->setTexture( mManager->getGraphicsManager()->getDummyTexture() );
                    }
                    break;

                // �}�e���A���ɐݒ肳��Ă���A�X�t�B�A�}�b�v�e�N�X�`��
                case MME_VARREQ_OBJECT_MATERIALSPHEREMAP:
                    if ( param_.Material.getSphereTexture() )
                    {
                        var->setTexture( param_.Material.getSphereTexture() );
                    }
                    // �e�N�X�`�����Ȃ���΃_�~�[��ݒ�
                    else
                    {
                        var->setTexture( mManager->getGraphicsManager()->getDummyTexture() );
                    }
                    break;

                // �s�����x
                case LN_VARREQ_OPACITY:
                {
                    var->setFloat( param_.Opacity );
                    break;
                }
                // ��Z����F
                case LN_VARREQ_COLOR_SCALE:
                {
                    var->setVector( param_.ColorScale );
                    break;
                }
                // �u�����h����F
                case LN_VARREQ_BLEND_COLOR:
                {
                    var->setVector( param_.BlendColor );
                    break;
                }
                // �F��
                case LN_VARREQ_TONE:
                {
                    var->setVector( param_.Tone );
                    break;
                }
                // �e�N�X�`�����W�ϊ��s��
                case LN_VARREQ_UVTRANSFORM:
                {
                    var->setMatrix( param_.UVTransform );
                    break;
                }
            }
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void SceneShader::updateOffscreenOwner( const MMESceneObjectBase* obj )
	{
		ln_foreach( MMEShaderVariable* sv, mShaderVariableArray )
        {
			if ( MME_VARREQ_CONTROLOBJECT_BEGIN <= sv->Request && sv->Request <= MME_VARREQ_CONTROLOBJECT_END ) {
				if ( sv->ObjectName == _T("(OffscreenOwner)") ) {
					SceneShaderUtil::setControllObjectParam( sv, obj );
				}
				else {
					// �V�[���E�I�u�W�F�N�g�P�ʂ̃p�����[�^�ݒ�Őݒ�ς�
				}
			}
		}
	}

	//----------------------------------------------------------------------
	// �� CONTROLOBJECT ��v�����Ă���ϐ��̒l�����ׂčX�V����
	//----------------------------------------------------------------------
	//void SceneShader::updateControlObjectVariables()
	//{
	//    //LN_PRINT_NOT_IMPL_FUNCTION;
	//}

	//----------------------------------------------------------------------
	// �� SceneNode::setShader() �ł̐擪�ŌĂ΂��
	//----------------------------------------------------------------------
	//void SceneShader::onSettingShader( MMESceneObjectBase* node_ )
	//{
	//    if ( isShareable() ) return;

	//    LN_THROW_InvalidCall( !mSelfSceneNode, Resource::String::ERR_SceneShader_onSettingShader );

	//    mSelfSceneNode = node_;
	//}

	//----------------------------------------------------------------------
	// �� ���o��
	//----------------------------------------------------------------------
    void SceneShader::dumpInfo( FILE* stream_ )
    {
        FILE* fp = ( stream_ ) ? stream_ : stdout;

        ln_foreach( MMEShaderVariable* sv, mShaderVariableArray )
        {
            _ftprintf( fp, "[%s]\n", sv->Variable->getName() );
            _ftprintf( fp, "    Request : %d\n", sv->Request );
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	SceneShaderTechniqueArray* SceneShader::findTechnique( MMDPass pass, bool UseTexture, bool UseSphereMap, bool UseToon )
	{
		int size = mTechniqueGroupArray.size();
		for ( int i = 0; i < size; ++i )
		{
			SceneShaderTechnique* t = mTechniqueGroupArray[i].front();
			if ( t->getMMDPass() == pass )
			{
				lnU32 f = t->getMMETechEnableFlags();

				// ���ꂼ��ȗ�����Ă��炸�A�����ƈ�v���Ȃ��ꍇ�͂͂���
				if ( (f & MME_TECHSTATE_OMITTED_Texture) == 0 )
				{
					if ( ((f & MME_TECHSTATE_UseTexture) != 0) != UseTexture ) continue;
				}
				
				if ( (f & MME_TECHSTATE_OMITTED_SphereMap) == 0 )
				{
					if ( ((f & MME_TECHSTATE_UseSphereMap) != 0) != UseSphereMap ) continue;
				}
				
				if ( (f & MME_TECHSTATE_OMITTED_Toon) == 0 )
				{
					if ( ((f & MME_TECHSTATE_UseToon) != 0) != UseToon ) continue;
				}
				return &mTechniqueGroupArray[i];
			}
		}
		return NULL;
	}

	//----------------------------------------------------------------------
	// �� �p�����[�^�Ȃǂ����ۂɐݒ肵�Ă���
	//----------------------------------------------------------------------
    LNRESULT SceneShader::_build()
    {
        // SceneShader ���ō쐬�������\�[�X�̉��
        _release();

        //-----------------------------------------------------
        // �V�F�[�_�v���O�������̂��ׂĂ̕ϐ����`�F�b�N����

        lnU32 i = 0;
        Graphics::IShaderVariable* var;

        while ( true )
        {
            var = mShader->getVariableByIndex( i );
            if ( !var ) { 
            	break; 
			}

            // �V�F�[�_�ϐ��B�Ƃ肠�����Ńt�H���g�l�����Ă���
            MMEShaderVariable* sv = LN_NEW MMEShaderVariable();
            sv->Variable            = var;
            sv->Request             = MME_VARREQ_NONE;
            //sv->ControlObject       = NULL;
            sv->TextureObject		= NULL;
            sv->LightNum            = 0;
            sv->LightParamIsMatrix  = false;
            mShaderVariableArray.push_back( sv );

            // �Z�}���e�B�N�X�m�F�BCONTROLOBJECT �̓���Ȓl�������ꍇ�� sv �� ObjectName ���ɒl������
            MMEScriptOutput op1;
            MMEScriptClass  op2;
            MMEScriptOrder  op3;
            bool is_controllobject;
            SceneShaderUtil::checkVariableRequest(
                var,
                mManager->getSemanticsMap(),
                sv,
                &op1,
                &op2,
                &op3,
                &is_controllobject );
            if ( sv->Request == MME_VARREQ_STANDARDSGLOBAL )
            {
                mScriptOutput = op1;
                mScriptClass  = op2;
                mScriptOrder  = op3;
            }
            if ( is_controllobject )
            {
                mControlObjectVarArray.push_back( sv );
            }

            // ���׌y���̂��߁A�K�v�ȍs�񉉎Z���`�F�b�N����
            mWorldMatrixCalcMask |= SceneShaderUtil::checkMatrixMask( sv->Request );

            // �ϐ����Ƃ̑Ή��\�ɒǉ�
            mShaderVariableMap.insert( MMEShaderVariablePair( var->getName(), sv ) );

            // �K�v�ȃ��C�g�ő吔�`�F�b�N
            if ( sv->LightNum > mRequiredLightNum )
            {
                mRequiredLightNum = sv->LightNum;
            }

            ++i;
        }
                
        //-----------------------------------------------------
        // �e�N�j�b�N���쐬

        Graphics::IShaderTechnique* tech;
        i = 0;
        while ( true )
        {
            // STANDARDSGLOBAL �� Script �A�m�e�[�V�����Ńe�N�j�b�N�̌���������
            // �w�肳��Ă���ꍇ�͂����ŏ��Ԃ𑀍삷��

            tech = mShader->getTechniqueByIndex( i );
            if ( !tech ) break;

            SceneShaderTechnique* sstech = LN_NEW SceneShaderTechnique();
			sstech->initialize( this, tech );

            mTechniqueList.push_back( sstech );

			// MMDPass �� UseTexture ���ɂ��O���[�v����
			lnU32 enableFlags = sstech->getMMETechEnableFlags();
			int size = mTechniqueGroupArray.size();
			int j = 0;
			for ( ; j < size; ++j )
			{
				SceneShaderTechnique* t = mTechniqueGroupArray[j].front();
				if ( sstech->getMMDPass() == t->getMMDPass()  && enableFlags == t->getMMETechEnableFlags())
				{
					/*
					// <bool UseTexture>
					lnU32 mask = MME_TECHSTATE_UseTexture | MME_TECHSTATE_OMITTED_Texture;
					if ( (enableFlags & mask) != (t->getMMETechEnableFlags() & mask) ) continue;

					// <bool UseTexture>
					mask = MME_TECHSTATE_UseSphereMap | MME_TECHSTATE_OMITTED_SphereMap;
					if ( (enableFlags & mask) != (t->getMMETechEnableFlags() & mask) ) continue;

					// <bool UseTexture>
					mask = MME_TECHSTATE_UseToon | MME_TECHSTATE_OMITTED_Toon;
					if ( (enableFlags & mask) != (t->getMMETechEnableFlags() & mask) ) continue;
					*/
					mTechniqueGroupArray[j].push_back( sstech );
					break;
				}
			}

			// �����̃O���[�v�Ɉ�v���Ȃ�����
			if ( j == size )
			{
				SceneShaderTechniqueArray ary;
				ary.push_back( sstech );
				mTechniqueGroupArray.push_back( ary );
			}
			

            //-------------------------------------
            // �Ó����`�F�b�N
            // 
            if ( mScriptClass == MME_SCRCLS_object )
            {
				/*
				_p(mName.c_str());
                // �p�X�̃X�N���v�g�� Draw=Buffer �����s���Ă͂Ȃ�Ȃ�
                LN_THROW_InvalidCall(
                    !( sstech->getCommandExistFlags() & MME_SCEXIST_Draw_Buffer ),
                    Resource::String::ERR_SceneShader_build1 );
				*/
            }
            else if ( mScriptClass == MME_SCRCLS_scene )
            {
                // �p�X�̃X�N���v�g�� Draw=Geometry �����s���Ă͂Ȃ�Ȃ�
				LN_THROW_InvalidOperation(
                    !( sstech->getCommandExistFlags() & MME_SCEXIST_Draw_Geometry ),
                    Resource::String::ERR_SceneShader_build2 );
            }

            if ( mScriptOrder == MME_SCRORDER_postprocess )
            {
                // ScriptExternal=Color ���K���K�v
				LN_THROW_InvalidOperation(
                    sstech->getCommandExistFlags() & MME_SCEXIST_ScriptExternal_Color,
                    Resource::String::ERR_SceneShader_build3 );
            }
            else
            {
                // ScriptExternal=Color �������Ă͂Ȃ�Ȃ�
				LN_THROW_InvalidOperation(
                    !( sstech->getCommandExistFlags() & MME_SCEXIST_ScriptExternal_Color ),
                    Resource::String::ERR_SceneShader_build4 );
            }

            

            ++i;
        }

        //-----------------------------------------------------
        

        

#if 0
        printf( "----------------------------------------\n" );
        dumpInfo();
        printf( "----------------------------------------\n" );
#endif
		return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	// �� ���\�[�X�̍č\�z
	//----------------------------------------------------------------------
    LNRESULT SceneShader::refreshResource( const LVector2& view_size_ )
    {
		LNRESULT lr = ResultCode_OK;

        _releaseResource();

        mIsError = true;

        ln_foreach( MMEShaderVariable* sv, mShaderVariableArray )
        {
            // �e�N�X�`�����v������Ă��邩�`�F�b�N���č쐬����
            SceneShaderUtil::TextureResourceData resource_data;
            if ( SceneShaderUtil::checkTextureResourceData( sv, mManager->getAnnotationMap(), &resource_data ) )
            {
                // �쐬����ׂ��e�N�X�`���̃T�C�Y�𒲂ׂ�
     //           int width, height;
     //           SceneShaderUtil::checkTextureSize(
     //               &width,
     //               &height,
     //               resource_data.Dimensions,
     //               resource_data.ViewportRatio, 
					//mManager->getBaseViewSize() );
				//LN_A;
                //�e�N�X�`���쐬
                //lr = mManager->createTexture(
                //    &sv->Texture,
                //    sv->Request,
                //    resource_data.ResourceName.c_str(),
                //    width,
                //    height,
                //    static_cast< lnU32 >( resource_data.Dimensions.z ),
                //    resource_data.Miplevels,
                //    resource_data.Format );
                //if ( LN_FAILED( lr ) )
                //{
                //    mErrorMessage = _T( "Failed to create texture" );
                //    return lr;
                //}

				sv->TextureObject = LN_NEW MMETextureObject( this->mManager );
				sv->TextureObject->initialize( 
					this,
					sv->Request,
					resource_data.ResourceName,
					resource_data.Dimensions,
					resource_data.ViewportRatio,
					resource_data.Miplevels,
					resource_data.Format,
					resource_data.AntiAlias );
				sv->TextureObject->refresh( mManager->getBaseViewSize() );

                // ����ɍ쐬�ł����ꍇ�̓V�F�[�_�̕ϐ��ɐݒ肷�� (�[�x�o�b�t�@�̓e�N�X�`���Ƃ��č쐬����Ȃ��̂Őݒ肵�Ȃ�)
                if ( sv->Request != MME_VARREQ_RENDERDEPTHSTENCILTARGET )
                {
					sv->Variable->setTexture( sv->TextureObject->getTexture() );
                }

                // �I�t�X�N���[��RT �̏ꍇ
                if ( sv->Request == MME_VARREQ_OFFSCREENRENDERTARGET )
                {
					OffscreenScene* ofs = LN_NEW OffscreenScene( mManager, this );
					ofs->initialize(
						resource_data.ClearColor,
						resource_data.ClearDepth,
						resource_data.Description,
						resource_data.DefaultEffect,
						sv->TextureObject->getTexture() );
					sv->Variable->setTexture( sv->TextureObject->getTexture() );
					mOffscreenSceneList.push_back( ofs );
#if 0
                    OffscreenScene::InitData data;
                    data.ClearColor             = resource_data.ClearColor;
                    data.ClearDepth             = resource_data.ClearDepth;
                    data.Description            = resource_data.Description;
                    data.DefaultEffectStrings   = resource_data.DefaultEffect;
                    data.RTTexture              = sv->Texture;
                    OffscreenScene* ofs = LN_NEW OffscreenScene( mManager, this );
                    LN_CALL_R( ofs->initialize( data ) );

                    mOffscreenSceneList.push_back( ofs );

                    // shared ���Ȃ��ꍇ�� SceneShader ���L�s��
                    if ( !sv->Variable->getTypeDesc().Shared )
                    {
                        mShareable = false;
                    }
                    // shared �����邪�A"self" ������ꍇ�����L�s��
                    else if ( sv->Variable->getTypeDesc().Shared && ofs->isThereSelf())
                    {
                        mShareable = false;
                    }
#endif
                }
            }
        }

        mIsError = false;
		return ResultCode_OK;
    }

	//----------------------------------------------------------------------
	// �� �ϐ����ɑΉ����� MMEShaderVariable ����������
	//----------------------------------------------------------------------
    MMEShaderVariable* SceneShader::findShaderVariable( const lnChar* name_ )
    {
        MMEShaderVariableMap::iterator it = mShaderVariableMap.find( name_ );
        if ( it != mShaderVariableMap.end() )
        {
            return it->second;
        }
        return NULL;
    }

	//----------------------------------------------------------------------
	// �� ���
	//----------------------------------------------------------------------
    void SceneShader::_release()
    {
        _releaseResource();

        // �e�N�j�b�N���
        ln_foreach( SceneShaderTechnique* tech, mTechniqueList )
        {
            SAFE_DELETE( tech );
        }
        mTechniqueList.clear();

        // �ϐ������
        ln_foreach( MMEShaderVariable* sv, mShaderVariableArray )
        {
            SAFE_DELETE( sv );
        }
        mShaderVariableArray.clear();

        mRequiredLightNum = 0;
    }

	//----------------------------------------------------------------------
	// �� ���\�[�X���
	//----------------------------------------------------------------------
    void SceneShader::_releaseResource()
    {
        ln_foreach( MMEShaderVariable* sv, mShaderVariableArray )
        {
            LN_SAFE_RELEASE( sv->TextureObject );
        }
        //mShaderVariableArray.clear(); // �����ł̓e�N�X�`���̉�������s���̂� clear �͂��Ȃ�
        ln_foreach( OffscreenScene* ofs, mOffscreenSceneList )
        {
            LN_SAFE_RELEASE( ofs );
        }
        mOffscreenSceneList.clear();
    }

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================