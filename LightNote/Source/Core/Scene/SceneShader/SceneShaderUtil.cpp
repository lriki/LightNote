//==============================================================================
// SceneShader 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
//#include "../../Resource/LNResource.h"
#include "../../Graphics/Common/GraphicsUtil.h"
//#include "../SceneGraph.h"
//#include "../SceneNode/SceneNode.h"
//#include "../SceneNode/Light.h"
//#include "../SceneNode/Viewport.h"
//#include "../Pane.h"	// �K�v�Ȃ��Ȃ�͂�
//#include "OffscreenScene.h"
//#include "SceneShaderTechnique.h"
#include "SceneShaderUtil.h"

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
// �� SceneShaderUtil
//==============================================================================

	//----------------------------------------------------------------------
	// �� �V�F�[�_�ϐ��̃Z�}���e�B�N�X�𒲂ׂđΉ�����v�����ڂ�Ԃ�
	//----------------------------------------------------------------------
    void SceneShaderUtil::checkVariableRequest(
        Graphics::IShaderVariable* var_,
        const MMESemanticsMap& semantics_map_,
        MMEShaderVariable* sv_,
        MMEScriptOutput* script_output_,
        MMEScriptClass* script_class_,
        MMEScriptOrder* script_order_,
        bool* is_controllobject_ )
    {
        const lnChar*       name;                           // �V�F�[�_���當������󂯎��ϐ�
        lnChar              temp[ MAX_SEMANTICS_NAME_LEN ]; // ������̃R�s�[�Ɏg���ꎞ�o�b�t�@
        MMEVariableRequest  req = MME_VARREQ_NONE;          // �߂�l

        MMESemanticsMap::const_iterator it; 
        MMESemanticsMap::const_iterator end = semantics_map_.end();
        const LNShaderVariableTypeDesc& var_desc = var_->getTypeDesc();  // �ϐ��̌^���

        *is_controllobject_ = false;

        name = var_->getSemanticName();

        // �Z�}���e�B�N�X������ꍇ
        if ( name )
        {
            // �啶�������āA�Ή��\����������
            _tcsncpy( temp, name, MAX_SEMANTICS_NAME_LEN );
            Base::StringUtils::toUpper( temp );
            it = semantics_map_.find( temp );
            if ( it != end )
            {
                int r;

                switch ( it->second )
                {
                    /////////////////////////////////////// �ϊ��s��
                    case MME_SEMANTIC_WORLD:
                        req = MME_VARREQ_MATRIX_World;
                        break;
                    case MME_SEMANTIC_VIEW:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_View : MME_VARREQ_MATRIX_LIGHT_View;
                        break;
                    case MME_SEMANTIC_PROJECTION:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_Proj : MME_VARREQ_MATRIX_LIGHT_Proj;
                        break;
                    case MME_SEMANTIC_WORLDVIEW:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_WorldView : MME_VARREQ_MATRIX_LIGHT_WorldView;
                        break;
                    case MME_SEMANTIC_VIEWPROJECTION:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_ViewProj : MME_VARREQ_MATRIX_LIGHT_ViewProj;
                        break;
                    case MME_SEMANTIC_WORLDVIEWPROJECTION:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_WorldViewProj : MME_VARREQ_MATRIX_LIGHT_WorldViewProj;
                        break;

                    ///////////////////////////////////////  �t�s��
                    case MME_SEMANTIC_WORLD_I:
                        req = MME_VARREQ_MATRIX_World_I;
                        break;
                    case MME_SEMANTIC_VIEW_I:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_View_I : MME_VARREQ_MATRIX_LIGHT_View_I;
                        break;
                    case MME_SEMANTIC_PROJECTION_I:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_Proj_I : MME_VARREQ_MATRIX_LIGHT_Proj_I;
                        break;
                    case MME_SEMANTIC_WORLDVIEW_I:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_WorldView_I : MME_VARREQ_MATRIX_LIGHT_WorldView_I;
                        break;
                    case MME_SEMANTIC_VIEWPROJECTION_I:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_ViewProj_I : MME_VARREQ_MATRIX_LIGHT_ViewProj_I;
                        break;
                    case MME_SEMANTIC_WORLDVIEWPROJECTION_I:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_WorldViewProj_I : MME_VARREQ_MATRIX_LIGHT_WorldViewProj_I;
                        break;

                    ///////////////////////////////////////  �]�u�s��
                    case MME_SEMANTIC_WORLD_T:
                        req = MME_VARREQ_MATRIX_World_T;
                        break;
                    case MME_SEMANTIC_VIEW_T:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_View_T : MME_VARREQ_MATRIX_LIGHT_View_T;
                        break;
                    case MME_SEMANTIC_PROJECTION_T:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_Proj_T : MME_VARREQ_MATRIX_LIGHT_Proj_T;
                        break;
                    case MME_SEMANTIC_WORLDVIEW_T:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_WorldView_T : MME_VARREQ_MATRIX_LIGHT_WorldView_T;
                        break;
                    case MME_SEMANTIC_VIEWPROJECTION_T:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_ViewProj_T : MME_VARREQ_MATRIX_LIGHT_ViewProj_T;
                        break;
                    case MME_SEMANTIC_WORLDVIEWPROJECTION_T:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_WorldViewProj_T : MME_VARREQ_MATRIX_LIGHT_WorldViewProj_T;
                        break;

                    ///////////////////////////////////////  �t�s��~�]�u�s��
                    case MME_SEMANTIC_WORLD_IT:
                        req = MME_VARREQ_MATRIX_World_IT;
                        break;
                    case MME_SEMANTIC_VIEW_IT:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_View_IT : MME_VARREQ_MATRIX_LIGHT_View_IT;
                        break;
                    case MME_SEMANTIC_PROJECTION_IT:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_Proj_IT : MME_VARREQ_MATRIX_LIGHT_Proj_IT;
                        break;
                    case MME_SEMANTIC_WORLDVIEW_IT:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_WorldView_IT : MME_VARREQ_MATRIX_LIGHT_WorldView_IT;
                        break;
                    case MME_SEMANTIC_VIEWPROJECTION_IT:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_ViewProj_IT : MME_VARREQ_MATRIX_LIGHT_ViewProj_IT;
                        break;
                    case MME_SEMANTIC_WORLDVIEWPROJECTION_IT:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_MATRIX_CAMERA_WorldViewProj_IT : MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT;
                        break;

                    /////////////////////////////////////// �F
                    case MME_SEMANTIC_DIFFUSE:
                        r = checkAnnotationGeometryOrLight( var_ );
                        if ( r != 0 )
                        {
                            req = ( r == 1 ) ? MME_VARREQ_OBJECT_DIFFUSE : MME_VARREQ_LIGHT_DIFFUSE;
                        }
                        break;

                    case MME_SEMANTIC_AMBIENT:
                        r = checkAnnotationGeometryOrLight( var_ );
                        if ( r != 0 )
                        {
                            req = ( r == 1 ) ? MME_VARREQ_OBJECT_AMBIENT : MME_VARREQ_LIGHT_AMBIENT;
                        }
                        break;

                    case MME_SEMANTIC_EMISSIVE:
                        req = MME_VARREQ_OBJECT_EMISSIVE;
                        break;

                    case MME_SEMANTIC_SPECULAR:
                        r = checkAnnotationGeometryOrLight( var_ );
                        if ( r != 0 )
                        {
                            req = ( r == 1 ) ? MME_VARREQ_OBJECT_SPECULAR : MME_VARREQ_LIGHT_SPECULAR;
                        }
                        break;

                    case MME_SEMANTIC_SPECULARPOWER:
                        req = MME_VARREQ_OBJECT_SPECULARPOWER;
                        break;

                    case MME_SEMANTIC_TOONCOLOR:
                        req = MME_VARREQ_OBJECT_TOONCOLOR;
                        break;

                    case MME_SEMANTIC_EDGECOLOR:
                        req = MME_VARREQ_OBJECT_EDGECOLOR;
                        break;

                    /////////////////////////////////////// �ʒu�E����
                    case MME_SEMANTIC_POSITION:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_CAMERA_POSITION : MME_VARREQ_LIGHT_POSITION;
                        break;

                    case MME_SEMANTIC_DIRECTION:
                        req = ( checkAnnotationCameraOrLight( var_ ) ) ? MME_VARREQ_CAMERA_DIRECTION : MME_VARREQ_LIGHT_DIRECTION;
                        break;
                    
                    /////////////////////////////////////// �}�e���A���̃e�N�X�`��
                    case MME_SEMANTIC_MATERIALTEXTURE:
                        req = MME_VARREQ_OBJECT_MATERIALTEXTURE;
                        break;

                    case MME_SEMANTIC_MATERIALSPHEREMAP:
                        req = MME_VARREQ_OBJECT_MATERIALSPHEREMAP;
                        break;

                    /////////////////////////////////////// misc
                    case MME_SEMANTIC_VIEWPORTPIXELSIZE:
                        req = MME_VARREQ_VIEWPORTPIXELSIZE;
                        break;

                    case MME_SEMANTIC_TIME:
                        req = MME_VARREQ_TIME;
                        break;

                    case MME_SEMANTIC_ELAPSEDTIME:
                        req = MME_VARREQ_ELAPSEDTIME;
                        break;

                    /////////////////////////////////////// �}�E�X
                    case MME_SEMANTIC_MOUSEPOSITION:
                        req = MME_VARREQ_MOUSEPOSITION;
                        break;
                    case MME_SEMANTIC_LEFTMOUSEDOWN:
                        req = MME_VARREQ_LEFTMOUSEDOWN;
                        break;
                    case MME_SEMANTIC_MIDDLEMOUSEDOWN:
                        req = MME_VARREQ_MIDDLEMOUSEDOWN;
                        break;
                    case MME_SEMANTIC_RIGHTMOUSEDOWN:
                        req = MME_VARREQ_RIGHTMOUSEDOWN;
                        break;

                    /////////////////////////////////////// CONTROLOBJECT
                    case MME_SEMANTIC_CONTROLOBJECT:
                    {
                        //const char* name = NULL;
                       // const char* item = NULL;

                        Graphics::IShaderVariable* name_anno = getAnnotationByName( var_, _T( "name" ) );
                        if ( !name_anno ) // name �͕K�{
                        {
                            break;
                        }
                        //name_anno->getString( &name );

                        // ����Ώۂ̖��O�Ƃ��Ċo���Ă���
                        sv_->ObjectName = name_anno->getString();

                        // "item" ���`�F�b�N
                        Graphics::IShaderVariable* anno = getAnnotationByName( var_, _T( "item" ) );
                        if ( anno )
                        {
                            const char* item = anno->getString();
                            if ( item )
                            {
                                if ( _tcsicmp( item, _T( "X" ) ) == 0 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_X;
                                }
                                else if ( _tcsicmp( item, _T( "Y" ) ) == 0 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_Y;
                                }
                                else if ( _tcsicmp( item, _T( "Y" ) ) == 0 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_Z;
                                }
                                else if ( _tcsicmp( item, _T( "XYZ" ) ) == 0 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_XYZ;
                                }
                                else if ( _tcsicmp( item, _T( "Rx" ) ) == 0 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_Rx;
                                }
                                else if ( _tcsicmp( item, _T( "Ry" ) ) == 0 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_Ry;
                                }
                                else if ( _tcsicmp( item, _T( "Rz" ) ) == 0 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_Rz;
                                }
                                else if ( _tcsicmp( item, _T( "Rxyz" ) ) == 0 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_Rxyz;
                                }
                                else if ( _tcsicmp( item, _T( "Si" ) ) == 0 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_Si;
                                }
                                else if ( _tcsicmp( item, _T( "Tr" ) ) == 0 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_Tr;
                                }
                                else if ( var_desc.Type == LN_SVT_FLOAT )
                                {
                                    // �^�� float �̏ꍇ�͕\��
                                    if ( var_desc.Class == LN_SVC_SCALAR )
                                    {
                                        req = MME_VARREQ_CONTROLOBJECT_MorphBlend;
                                        // ����Ώۂ̖��O�Ƃ��Ċo���Ă���
                                        sv_->ItemName = item;
                                    }
                                    // float3 4 �̏ꍇ�̓{�[���ʒu
                                    else if ( var_desc.Class == LN_SVC_VECTOR && var_desc.Columns >= 3 )
                                    {
                                        req = MME_VARREQ_CONTROLOBJECT_BoneOffset;
                                        sv_->ItemName = item;
                                    }
                                    // matrix4x4 �̏ꍇ�̓{�[���s��
                                    //else if ( ( var_desc.Class == LN_SVC_MATRIX_ROWS || var_desc.Class == LN_SVC_MATRIX_COLUMNS )
                                    else if ( var_desc.Class == LN_SVC_MATRIX
                                        && var_desc.Columns == 4 && var_desc.Rows == 4 )
                                    {
                                        req = MME_VARREQ_CONTROLOBJECT_BoneMatrix;
                                        sv_->ItemName = item;
                                    }
                                }
                            } // if ( item )
                        } // if ( anno ) 

                        // �����܂łł܂� req �����܂��ĂȂ��ꍇ
                        if ( req == MME_VARREQ_NONE )
                        {
                            // �^�� bool �̏ꍇ
                            if ( var_desc.Class == LN_SVC_SCALAR && var_desc.Type == LN_SVT_BOOL )
                            {
                                req = MME_VARREQ_CONTROLOBJECT_Visible;
                            }
                            else if ( var_desc.Type == LN_SVT_FLOAT )
                            {
                                // float �̏ꍇ
                                if ( var_desc.Class == LN_SVC_SCALAR )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_Scale;
                                }
                                // float3 or 4 �̏ꍇ
                                else if ( var_desc.Class == LN_SVC_VECTOR && var_desc.Columns >= 3 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_Position;
                                }
                                // matrix4x4 �̏ꍇ
                                //else if ( ( var_desc.Class == LN_SVC_MATRIX_ROWS || var_desc.Class == LN_SVC_MATRIX_COLUMNS )
                                else if ( var_desc.Class == LN_SVC_MATRIX
                                    && var_desc.Columns == 4 && var_desc.Rows == 4 )
                                {
                                    req = MME_VARREQ_CONTROLOBJECT_World;
                                }
                            }
                        }

                        // �����܂łŗv�����ڂ��������Ă�����A�L���� CONTROLOBJECT
                        if ( req != MME_VARREQ_NONE )
                        {
                            *is_controllobject_ = true;
                        }           

                        break;
                    }


                    /////////////////////////////////////// �e�N�X�`���֘A
                    case MME_SEMANTIC_RENDERCOLORTARGET:
                        req = MME_VARREQ_RENDERCOLORTARGET;
                        break;
                    case MME_SEMANTIC_RENDERDEPTHSTENCILTARGET:
                        req = MME_VARREQ_RENDERDEPTHSTENCILTARGET;
                        break;
                    case MME_SEMANTIC_ANIMATEDTEXTURE:
                        printf( "ANIMATEDTEXTURE �Z�}���e�B�N�X�͖��Ή��ł��B\n" );
                        req = MME_VARREQ_ANIMATEDTEXTURE;
                        break;
                    case MME_SEMANTIC_OFFSCREENRENDERTARGET:
                        req = MME_VARREQ_OFFSCREENRENDERTARGET;
                        break;
                    case MME_SEMANTIC_TEXTUREVALUE:
                        printf( "TEXTUREVALUE �Z�}���e�B�N�X�͖��Ή��ł��B\n" );
                        req = MME_VARREQ_TEXTUREVALUE;
                        break;

                    /////////////////////////////////////// STANDARDSGLOBAL
                    case MME_SEMANTIC_STANDARDSGLOBAL:
                    {
                        req = MME_VARREQ_STANDARDSGLOBAL;

                        const lnChar* script_class = NULL;
                        const lnChar* script_order = NULL;
                        const lnChar* script = NULL;

                        Graphics::IShaderVariable* anno = NULL;
                        anno = getAnnotationByName( var_, _T( "ScriptClass" ) );
                        if ( anno ) { 
                        	script_class = anno->getString(); 
						}
                        anno = getAnnotationByName( var_, _T( "ScriptOrder" ) );
                        if ( anno ) { 
                        	script_order = anno->getString(); 
						}
                        anno = getAnnotationByName( var_, _T( "Script" ) );
                        if ( anno ) { 
                        	script = anno->getString(); 
						}


                        *script_output_ = MME_SCROUT_color;
                        *script_class_ = MME_SCRCLS_object;
                        *script_order_ = MME_SCRORDER_standard;

                        // ���Ńf�t�H���g�l�Ƃ��đ�����Ă�̂ŃR�����g�A�E�g
                        //if ( stricmp( script_class, "object" ) ) { MME_SCRCLS_object; }
                        if      ( _tcsicmp( script_class, _T( "scene" ) ) == 0 )         { 
                        	*script_class_ = MME_SCRCLS_scene; }
                        else if ( _tcsicmp( script_class, _T("sceneorobject" ) ) == 0 ) { 
                        	*script_class_ = MME_SCRCLS_sceneorobject; }

                        //if ( stricmp( script_order, "object" ) ) { MME_SCRORDER_standard; }
                        if      ( _tcsicmp( script_order, _T("preprocess" ) ) == 0 )    { 
                        	*script_order_ = MME_SCRORDER_preprocess; }
                        else if ( _tcsicmp( script_order, _T("postprocess" ) ) == 0 )   { 
                        	*script_order_ = MME_SCRORDER_postprocess; }


                        if ( script )
                        {
                            printf( "STANDARDSGLOBAL �� Script �A�m�e�[�V�����͖��Ή��ł��B\n" );
                            // Script �ł͎g�p����e�N�j�b�N�̌����������w�肷��
                            
                            LN_PRINT_NOT_IMPL_FUNCTION;
                        }

                        break;
                    }

                    /////////////////////////////////////// �Ǝ��d�l����
                    /*
                    case LN_SEMANTIC_ORTHOGRAPHIC:
                    {
                        req = ( checkAnnotationCameraOrLight( var ) ) ? LN_VARREQ_MATRIX_CAMERA_ORTHOGRAPHIC : LN_VARREQ_MATRIX_LIGHT_ORTHOGRAPHIC;
                        break;
                    }
                    */
                    case LN_SEMANTIC_PROJECTION2D:
                    {
                        req = LN_VARREQ_MATRIX_PROJECTION2D;
                        break;
                    }
                    case LN_SEMANTIC_OPACITY:
                    {
                        req = LN_VARREQ_OPACITY;
                        break;
                    }
                    case LN_SEMANTIC_COLOR_SCALE:
                    {
                        req = LN_VARREQ_COLOR_SCALE;
                        break;
                    }
                    case LN_SEMANTIC_BLEND_COLOR:
                    {
                        req = LN_VARREQ_BLEND_COLOR;
                        break;
                    }
                    case LN_SEMANTIC_TONE:
                    {
                        req = LN_VARREQ_TONE;
                        break;
                    }
                    case LN_SEMANTIC_UVTRANSFORM:
                    {
                        req = LN_VARREQ_UVTRANSFORM;
                        break;
                    }

                } // switch ( it->second )
                
            } // if ( it != end ) // �Z�}���e�B�N�X���������đΉ�����l�����������ꍇ
        }

        // �܂����܂��Ă��Ȃ��ꍇ�̓Z�}���e�B�N�X�����e�N�X�`���̉\���𒲂ׂ�
        if ( req == MME_VARREQ_NONE )
        {
            //if ( var_desc.Class == LN_SVC_OBJECT && var_desc.Type == LN_SVT_TEXTURE )
			if ( var_->getType() == Graphics::SVT_TEXTURE )
            {
                req = MME_VARREQ_TEXTURE;
            }
        }

        // ���C�g�֌W�̏ꍇ�͔z��̗v�f�� = �K�v�ȃ��C�g�̐����m�F����
        sv_->LightParamIsMatrix = false;
        switch ( req )
        {
            case MME_VARREQ_MATRIX_LIGHT_WorldView:
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj:
            case MME_VARREQ_MATRIX_LIGHT_View:
            case MME_VARREQ_MATRIX_LIGHT_Proj:
            case MME_VARREQ_MATRIX_LIGHT_ViewProj:
            case MME_VARREQ_MATRIX_LIGHT_WorldView_I:
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_I:
            case MME_VARREQ_MATRIX_LIGHT_View_I:
            case MME_VARREQ_MATRIX_LIGHT_Proj_I:
            case MME_VARREQ_MATRIX_LIGHT_ViewProj_I:
            case MME_VARREQ_MATRIX_LIGHT_WorldView_T:
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_T:
            case MME_VARREQ_MATRIX_LIGHT_View_T:
            case MME_VARREQ_MATRIX_LIGHT_Proj_T:
            case MME_VARREQ_MATRIX_LIGHT_ViewProj_T:
            case MME_VARREQ_MATRIX_LIGHT_WorldView_IT:
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT:
            case MME_VARREQ_MATRIX_LIGHT_View_IT:
            case MME_VARREQ_MATRIX_LIGHT_Proj_IT:
            case MME_VARREQ_MATRIX_LIGHT_ViewProj_IT:
                sv_->LightParamIsMatrix = true;
            case MME_VARREQ_LIGHT_DIFFUSE:
            case MME_VARREQ_LIGHT_AMBIENT:
            case MME_VARREQ_LIGHT_SPECULAR:
            case MME_VARREQ_LIGHT_POSITION:
            case MME_VARREQ_LIGHT_DIRECTION:
				// �z��łȂ�
				if ( var_->getTypeDesc().Elements == 0 ) {
					sv_->LightParamIsArray = false;
					sv_->LightNum = 1;
				}
				// �z��ł���
				else {
					sv_->LightParamIsArray = true;
					sv_->LightNum = var_->getTypeDesc().Elements;
				}
                break;
        }

        // �v�����ڂ�Ԃ�
        sv_->Request = req;
    }

	//----------------------------------------------------------------------
	// �� �ϐ��� "Object" �A�m�e�[�V�����𒲂ׂāA"Light" �̏ꍇ�� fales�A����ȊO�� true ��Ԃ�
	//----------------------------------------------------------------------
    bool SceneShaderUtil::checkAnnotationCameraOrLight( Graphics::IShaderVariable* var_ )
    {
        Graphics::IShaderVariable* anno = getAnnotationByName( var_, _T( "Object" ) );
        if ( !anno ) { 
        	return true; 
		}

        const char* name = anno->getString();

        if ( !name ) { 
        	return true; 
		}

        if ( _tcsicmp( name, _T( "Light" ) ) == 0 )
        {
            return false;
        }
        return true;
    }

	//----------------------------------------------------------------------
	// �� �ϐ��� "Object" �A�m�e�[�V�����𒲂ׂāA"Geometry" �̏ꍇ�� 1�A"Light" �̏ꍇ�� 2�A����ȊO�� 0 ��Ԃ�
	//----------------------------------------------------------------------
    int SceneShaderUtil::checkAnnotationGeometryOrLight( Graphics::IShaderVariable* var_ )
    {
        Graphics::IShaderVariable* anno = getAnnotationByName( var_, _T( "Object" ) );
        if ( !anno ) { 
        	return 0; 
		}

        const lnChar* name = anno->getString();

        if ( !name ) { 
        	return 0; 
		}

        if ( _tcsicmp( name, _T( "Geometry" ) ) == 0 ) {
            return 1;
        }
        if ( _tcsicmp( name, _T( "Light" ) ) == 0 ) {
            return 2;
        }
        return 0;
    }

	//----------------------------------------------------------------------
	// �� ���O���w�肵�ăA�m�e�[�V�������������� (�啶���������̋�ʖ���)
	//----------------------------------------------------------------------
    Graphics::IShaderVariable* SceneShaderUtil::getAnnotationByName( Graphics::IShaderVariable* var_, const lnChar* name_ )
    {
        Graphics::IShaderVariable* anno;
        lnU32 idx = 0;
        while ( true )
        {
            anno = var_->getAnnotationByIndex( idx );
            if ( !anno ) { 
            	break; }

            if ( _tcsicmp( anno->getName(), name_ ) == 0 )
            {
                return anno;
            }
            ++idx;
        }
        return NULL;
    }

	//----------------------------------------------------------------------
	// �� �v�����ڂ���A�v�Z����K�v�̂���s��}�X�N�̃r�b�g����쐬���ĕԂ�
	//----------------------------------------------------------------------
    lnU32 SceneShaderUtil::checkMatrixMask( MMEVariableRequest req_ )
    {
        lnU32 flags = 0;

        switch ( req_ ) 
        {
            case MME_VARREQ_MATRIX_CAMERA_WorldView:        
            	flags = MME_WMCF_WORLD_VIEW_CAMERA; break;
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj:    
            	flags = MME_WMCF_WORLD_VIEW_PROJ_CAMERA; break;
            case MME_VARREQ_MATRIX_LIGHT_WorldView:         
            	flags = MME_WMCF_WORLD_VIEW_LIGHT; break;
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj:     
            	flags = MME_WMCF_WORLD_VIEW_PROJ_LIGHT; break;

            case MME_VARREQ_MATRIX_World_I:                 
            	flags = MME_WMCF_WORLD_I; break;
            case MME_VARREQ_MATRIX_CAMERA_WorldView_I:      
            	flags = MME_WMCF_WORLD_VIEW_CAMERA_I      | MME_WMCF_WORLD_VIEW_CAMERA; break;
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_I:  
            	flags = MME_WMCF_WORLD_VIEW_PROJ_CAMERA_I | MME_WMCF_WORLD_VIEW_PROJ_CAMERA; break;
            case MME_VARREQ_MATRIX_LIGHT_WorldView_I:       
            	flags = MME_WMCF_WORLD_VIEW_LIGHT_I       | MME_WMCF_WORLD_VIEW_LIGHT; break;
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_I:   
            	flags = MME_WMCF_WORLD_VIEW_PROJ_LIGHT_I  | MME_WMCF_WORLD_VIEW_PROJ_LIGHT; break;

            case MME_VARREQ_MATRIX_World_T:                 
            	flags = MME_WMCF_WORLD_T; break;
            case MME_VARREQ_MATRIX_CAMERA_WorldView_T:      
            	flags = MME_WMCF_WORLD_VIEW_CAMERA_T      | MME_WMCF_WORLD_VIEW_CAMERA; break;
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_T:  
            	flags = MME_WMCF_WORLD_VIEW_PROJ_CAMERA_T | MME_WMCF_WORLD_VIEW_PROJ_CAMERA; break;
            case MME_VARREQ_MATRIX_LIGHT_WorldView_T:       
            	flags = MME_WMCF_WORLD_VIEW_LIGHT_T       | MME_WMCF_WORLD_VIEW_LIGHT; break;
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_T:   
            	flags = MME_WMCF_WORLD_VIEW_PROJ_LIGHT_T  | MME_WMCF_WORLD_VIEW_PROJ_LIGHT; break;

            case MME_VARREQ_MATRIX_World_IT:                
            	flags = MME_WMCF_WORLD_I                  | MME_WMCF_WORLD_T                  | MME_WMCF_WORLD_IT; break;
            case MME_VARREQ_MATRIX_CAMERA_WorldView_IT:     
            	flags = MME_WMCF_WORLD_VIEW_CAMERA_I      | MME_WMCF_WORLD_VIEW_CAMERA_T      | MME_WMCF_WORLD_VIEW_CAMERA_IT; break;
            case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_IT: 
            	flags = MME_WMCF_WORLD_VIEW_PROJ_CAMERA_I | MME_WMCF_WORLD_VIEW_PROJ_CAMERA_T | MME_WMCF_WORLD_VIEW_PROJ_CAMERA_IT; break;
            case MME_VARREQ_MATRIX_LIGHT_WorldView_IT:      
            	flags = MME_WMCF_WORLD_VIEW_LIGHT_I       | MME_WMCF_WORLD_VIEW_LIGHT_T       | MME_WMCF_WORLD_VIEW_LIGHT_IT; break;
            case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT:  
            	flags = MME_WMCF_WORLD_VIEW_PROJ_LIGHT_I  | MME_WMCF_WORLD_VIEW_PROJ_LIGHT_T  | MME_WMCF_WORLD_VIEW_PROJ_LIGHT_IT; break;
        }
        return flags;
    }

	//----------------------------------------------------------------------
	// �� �e�N�X�`�����\�[�X�̏���Ԃ�
	//----------------------------------------------------------------------
    bool SceneShaderUtil::checkTextureResourceData(
        MMEShaderVariable* sv_,
        const MMEAnnotationMap& annotation_map_,
        TextureResourceData* data_ )
    {
        MMEVariableRequest req = sv_->Request;

        // �e�N�X�`�����쐬����K�v������ꍇ
        if ( needCreateTexture( req ) )
        {
            //-------------------------------------------------
            // �ϐ��̏����ƃf�t�H���g�l

            // ����Ȃ�ɋ���
            const char* resource_type = NULL;
            const char* resource_name = NULL;
            int width = 0;
            int height = 0;
            int depth = 0;
            LVector4 dimensions;
            LVector4 viewport_ratio( 1.0f, 1.0f, 0, 0 );
            const char* format = NULL;
            int miplevels = 0;
            Graphics::SurfaceFormat lnformat = Graphics::SurfaceFormat_A8R8G8B8;

            // ANIMATEDTEXTURE �̂�
            lnFloat offset = 0;     // �A�j���[�V�����̊J�n���Ԃ��炵��
            lnFloat speed = 1.0f;
            const char* seek_variable = NULL;

            // OFFSCREENRENDERTARGET �̂�
            LVector4 clear_color;
            lnFloat clear_depth = 1.0f;
            bool anti_alias = false;
            const char* description = NULL;
            const char* default_effect = NULL;

            //-------------------------------------------------
            // �A�m�e�[�V������ǂ�ł���
            Graphics::IShaderVariable* anno;
            MMEAnnotationMap::const_iterator it;
            MMEAnnotationMap::const_iterator end = annotation_map_.end();
            lnChar temp[ MAX_SEMANTICS_NAME_LEN ];
            lnU32 idx = 0;
            bool be_create = false;
			bool specifiedSize = false;

            while ( true )
            {
                anno = sv_->Variable->getAnnotationByIndex( idx );
                if ( !anno ) break;

                // ��r�̂��߁A���O��啶����
                _tcsncpy( temp, anno->getName(), MAX_SEMANTICS_NAME_LEN );
                Base::StringUtils::toUpper( temp );

                it = annotation_map_.find( temp );
                if ( it != end )
                {
                    switch ( it->second )
                    {
                        // �ʏ�A�����_�[�^�[�Q�b�g�A�[�x�o�b�t�@
                        case MME_ANNOTATION_ResourceType:
                            resource_type = anno->getString();
                            break;

                        case MME_ANNOTATION_ResourceName:
                            resource_name = anno->getString();
                            be_create = true;
                            break;
                        case MME_ANNOTATION_Width:
                            width = anno->getInt();
                            be_create = true;
							specifiedSize = true;
                            break;
                        
                        case MME_ANNOTATION_Height:
                            height = anno->getInt();
                            be_create = true;
							specifiedSize = true;
                            break;
                        
                        case MME_ANNOTATION_Depth:
                            depth = anno->getInt();
                            be_create = true;
                            break;
                        
                        case MME_ANNOTATION_Dimensions:
                            dimensions = anno->getVector();
                            be_create = true;
							specifiedSize = true;
                            break;
                        
                        case MME_ANNOTATION_ViewportRatio:
                            viewport_ratio = anno->getVector();
                            be_create = true;
							specifiedSize = true;
                            break;
                        
                        case MME_ANNOTATION_Format:
                            format = anno->getString();
                            break;
                        
                        case MME_ANNOTATION_Miplevels:
                            miplevels = anno->getInt();
                            break;
                        
                        case MME_ANNOTATION_Levels:
                            miplevels = anno->getInt();
                            break;
                        
                        // �A�j���[�V�����e�N�X�`��
                        case MME_ANNOTATION_Offset:
                            offset = anno->getFloat();
                            break;
                        
                        case MME_ANNOTATION_Speed:
                            speed = anno->getFloat();
                            break;
                        
                        case MME_ANNOTATION_SeekVariable:
                            seek_variable = anno->getString();
                            break;
                        
                        // �I�t�X�N���[�� RT
                        case MME_ANNOTATION_ClearColor:
                            clear_color = anno->getVector();
                            break;
                        
                        case MME_ANNOTATION_ClearDepth:
                            clear_depth = anno->getFloat();
                            break;
                        
                        case MME_ANNOTATION_AntiAlias:
                            anti_alias = anno->getBool();
                            break;

                        case MME_ANNOTATION_Description:
                            description = anno->getString();
                            break;

                        case MME_ANNOTATION_DefaultEffect:
                            default_effect = anno->getString();
                            break;
                    }
                }

                ++idx;
            };

			// �T�C�Y�n�����ׂďȗ�����Ă���ꍇ
			if ( specifiedSize = true )
			{
				// �ȉ��̂��̂� ViewportRatio = {1.0, 1.0} �Ƃ���
				if ( req == MME_VARREQ_RENDERCOLORTARGET ||
					 req == MME_VARREQ_RENDERDEPTHSTENCILTARGET ||
					 req == MME_VARREQ_OFFSCREENRENDERTARGET )
				{
					viewport_ratio.Set( 1.0f, 1.0f, 0, 0 );
					be_create = true;
				}
			}
				
            //-------------------------------------------------
            // �L���`�F�b�N

            // �K�v�ȃA�m�e�[�V�������ЂƂ��w�肳��Ă��Ȃ�
            if ( !be_create ) return false;

            // ResourceType ���ȗ�����Ă�ꍇ�� 2D
            if ( !resource_type )
            {
                resource_type = _T( "2D" );
            }

            // �s�N�Z���t�H�[�}�b�g  �ȗ�����Ă���ꍇ�� A8R8G8B8
            if ( !format )
            {
                format = _T( "A8R8G8B8" );
            }
            lnformat = Graphics::Util::convertFMTStringToLNFMT( format );
            
			if ( lnformat == Graphics::SurfaceFormat_Unknown ) {
				LN_THROW(0, Base::InvalidOperationException, "not support format \"%s\"", format);
			}

            //-------------------------------------------------
            // �T�C�Y����

            // �T�C�Y���w�肳��Ă��Ȃ���� dimensions �̒l���g��
            if ( width == 0 || height == 0 )
            {
                width  = static_cast< int >( dimensions.X );
                height = static_cast< int >( dimensions.Y );
            }
            if ( depth == 0 )
            {
                depth = static_cast< int >( dimensions.Z );
            }

            //-------------------------------------------------
            // 
            
            data_->ResourceName = resource_name;
            data_->Dimensions.Set( (lnFloat)width, (lnFloat)height, (lnFloat)depth );
            data_->ViewportRatio.Set( viewport_ratio.X, viewport_ratio.Y );
            data_->Miplevels = miplevels;
            data_->Format = lnformat;

            // �A�j���[�V�����e�N�X�`���p
            data_->Offset = offset;
            data_->Speed = speed;
            data_->SeekVariable = seek_variable;

            // �I�t�X�N���[��RT�p
            data_->ClearColor = clear_color;
            data_->ClearDepth = clear_depth;
            data_->AntiAlias = anti_alias;
            data_->Description = description;
            data_->DefaultEffect = default_effect;

           
            return true;
        } 

        return false;
    }

	//----------------------------------------------------------------------
	// �� �e�N�X�`�����\�[�X�̏�񂩂�A�e�N�X�`���T�C�Y��Ԃ�
	//----------------------------------------------------------------------
    void SceneShaderUtil::checkTextureSize(
        int* width_,
        int* height_,
        const LVector3& dimensions_,
        const LVector2& viewport_ratio_,
        const LVector2& viewport_size_ )
    {
        // dimensions_ ���L���Ȃ炻�̒l
        if ( dimensions_.X > 0 && dimensions_.Y > 0 )
        {
            *width_  = static_cast< int >( dimensions_.X );
            *height_ = static_cast< int >( dimensions_.Y );
        }
        // dimensions �ɒl���Ȃ��ꍇ�̓r���[�|�[�g�T�C�Y����
        else if ( viewport_ratio_.X >= 0.0f && viewport_ratio_.Y >= 0.0f )
        {
            *width_  = static_cast< int >( viewport_size_.X * viewport_ratio_.X );
            *height_ = static_cast< int >( viewport_size_.Y * viewport_ratio_.Y );
        }
        // ����ł������������� 64 * 64
        else
        {
            *width_ = *height_ = 64;
        }
    }

	//----------------------------------------------------------------------
	// �� �`��T�u�Z�b�g�����������񂩂�A�T�u�Z�b�g�ԍ��̔z����쐬����
	//----------------------------------------------------------------------
    void SceneShaderUtil::makeSubsetIndexArray( SubsetIndexArray* list_, const lnChar* subset_text_ )
    {
        
        const lnChar* c;
        const lnChar* lc; // �Ō�Ɍ������� ',' �܂͂� '-' �̎��̕���
        int   num;
        int   last_num = 0;
        bool  is_range = false;     // - �ɂ���Ĕ͈͎w�肪����ꍇ true
        char  temp[ 64 ];
        lnU32   size = 0;             // ���ۂɓW�J�����z��̗v�f��
        SubsetIndexOrderList    subse_index_order_list;

        SubsetIndexOrder si;
        SubsetIndexOrder last_si;
        last_si.Index = 0;
        last_si.RangeState = 0;
        
     
        c = subset_text_;
        lc = c;

        // �R���}���I�[�܂ŒT�����[�v
        while ( true )
        {
            if ( *c == _T( ',' ) || *c == _T( '\0' ) )
            {
                strncpy( temp, lc, c - lc );
                temp[ c - lc ] = _T( '\0' );
                num = atoi( temp );
                lc = c + 1;

                si.Index = num;

                // �ЂƂO�̕����� - �̏ꍇ�̓T�u�Z�b�g�I�[�܂ł͈͎̔w��
                if ( *( c - 1 ) == _T( '-' ) )
                {
                    si.RangeState = 2;
                    size += 2;          // ���l�ƁA�I�[�܂ł�\���l 0xffffffff ���i�[���镪
                }
                else
                {
                    si.RangeState = 0;

                    // �ЂƂO����͈͎̔w��̏ꍇ
                    if ( last_si.RangeState == 1 )
                    {
                        size += ( si.Index - last_si.Index ) + 1;   // ���̐������܂ނ̂� + 1  ( i < num �ł͂Ȃ� i <= num )
                    }
                    // �ЂƂ̐��l�̏ꍇ
                    else
                    {
                        ++size;
                    }
                }
                
                subse_index_order_list.push_back( si );
                last_si = si;   // ����Ȃ�����
               
                // �I�[���A�T�u�Z�b�g�̏I�[�܂œǂނ��̂����������ꍇ�͂����ŏI��
                if ( *c == _T( '\0' ) || si.RangeState == 2 )
                {
                    break;
                }
            }
            // - �����������ꍇ�͂ЂƂO�� , (�܂��͐擪) ����𐔒l�ɂ��Ă���
            else if ( *c == _T( '-' ) )
            {
                strncpy( temp, lc, c - lc );
                temp[ c - lc ] = _T( '\0' );
                last_num = atoi( temp );
                is_range = true;
                lc = c + 1;

                SubsetIndexOrder si;
                si.Index = last_num;
                si.RangeState = true;
                subse_index_order_list.push_back( si );

                last_si = si;
            }
            ++c;
        }
       


        // �쐬�Ɏ��s�����ꍇ
        if ( subse_index_order_list.empty() )
        {
            // ��ɂ���
            list_->clear();
        }
        else
        {
            list_->resize( size );

            lnU32 idx = 0;
            SubsetIndexOrderList::iterator  it1 = subse_index_order_list.begin();
            SubsetIndexOrderList::iterator end1 = subse_index_order_list.end();
            for ( ; it1 != end1;  )
            {
                // ���̒l�܂ł͈͎̔w��
                if ( it1->RangeState == 1 )
                {
                    lnU32 i = it1->Index;
                    ++it1;

                    // �����T�u�Z�b�g���܂ł͈͎̔w��̏ꍇ
                    if ( it1->RangeState == 2 )
                    {
                        (*list_)[ idx ] = i;
                        ++idx;
                        
                        (*list_)[ idx ] = 0xffffffff;
                        // ��쓮�h�~�̂��߁A�����ŏI���Bit1->RangeState == 2 �̂Ƃ��A�I�[�͕K�� 0xffffffff �ɂȂ�B
                        break;
                    }
                    else
                    {
                        for ( ; ( i <= it1->Index ); ++i )
                        {
                            (*list_)[ idx ] = i;
                            ++idx;
                        }
                        ++it1;
                    }
                }
                // ���ʂ̒l
                else
                {
                    (*list_)[ idx ] = it1->Index;
                    ++idx;
                    ++it1;
                }
            }
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void SceneShaderUtil::setControllObjectParam( MMEShaderVariable* sv, const MMESceneObjectBase* obj )
	{
		Graphics::IShaderVariable* var = sv->Variable;

		if ( sv->Request == MME_VARREQ_CONTROLOBJECT_Visible ) {
			if ( obj == NULL ) {
				var->setBool( false );
			}
			else {
				var->setBool( obj->isVisible() );
			}
		}
		else if ( obj != NULL )
		{
			switch ( sv->Request )
			{
				case MME_VARREQ_CONTROLOBJECT_Scale:
				{
					LVector4 scale;
					obj->getMatrix().Decompose((LVector3*)&scale, NULL, NULL);
					var->setVector( scale );
					break;
				}
				case MME_VARREQ_CONTROLOBJECT_Position:
					var->setVector( (const LVector4&)obj->getMatrix().GetPosition() );
					break;
				case MME_VARREQ_CONTROLOBJECT_World:
					var->setMatrix( obj->getMatrix() );
					break;

				case MME_VARREQ_CONTROLOBJECT_BoneOffset:
					var->setVector( 
						(const LVector4&)obj->findFrameMarix( sv->ItemName.c_str() ).GetPosition() );
					break;
				case MME_VARREQ_CONTROLOBJECT_BoneMatrix:
					var->setMatrix( 
						obj->findFrameMarix( sv->ItemName.c_str() ) );
					break;
				case MME_VARREQ_CONTROLOBJECT_MorphBlend:
					var->setFloat( 
						obj->findMorphBlend( sv->ItemName.c_str() ) );
					break;

				case MME_VARREQ_CONTROLOBJECT_X:
					var->setFloat( obj->getMatrix().GetPosition().X );
					break;
				case MME_VARREQ_CONTROLOBJECT_Y:
					var->setFloat( obj->getMatrix().GetPosition().Y );
					break;
				case MME_VARREQ_CONTROLOBJECT_Z:
					var->setFloat( obj->getMatrix().GetPosition().Z );
					break;
				case MME_VARREQ_CONTROLOBJECT_XYZ:
					var->setVector( (const LVector4&)obj->getMatrix().GetPosition() );
					break;

				case MME_VARREQ_CONTROLOBJECT_Rx:
				{
					LVector3 rad = obj->getMatrix().ToEulerAngles();
					var->setFloat( rad.X );
					break;
				}
				case MME_VARREQ_CONTROLOBJECT_Ry:
				{
					LVector3 rad = obj->getMatrix().ToEulerAngles();
					var->setFloat( rad.Y );
					break;
				}
				case MME_VARREQ_CONTROLOBJECT_Rz:
				{
					LVector3 rad = obj->getMatrix().ToEulerAngles();
					var->setFloat( rad.Z );
					break;
				}
				case MME_VARREQ_CONTROLOBJECT_Rxyz:
				{
					LVector4 rad(obj->getMatrix().ToEulerAngles(), 0);
					var->setVector( rad );
					break;
				}

				case MME_VARREQ_CONTROLOBJECT_Si:
				{
					LVector4 scale;
					obj->getMatrix().Decompose((LVector3*)&scale, NULL, NULL);
					var->setFloat( scale.X );
					break;
				}
				case MME_VARREQ_CONTROLOBJECT_Tr:
					var->setFloat( obj->getOpacity() );
					break;
			}
		}
	}

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================