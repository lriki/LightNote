//==============================================================================
// SceneShaderTechnique 
//==============================================================================

#include "stdafx.h"
#include "SceneShaderManager.h"
#include "SceneShader.h"
#include "SceneShaderUtil.h"
#include "SceneShaderTechnique.h"
#include "MMETextureObject.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

#define LN_SET_SHADER_ERR_MSG( fmt_, ... ) \
{ \
    LRefTString err; \
    err.format( fmt_, __VA_ARGS__ ); \
    mSceneShader->setErrorMessage( err ); \
}

//==============================================================================
// �� SceneShaderTechnique
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    SceneShaderTechnique::SceneShaderTechnique()
        : mSceneShader              ( NULL )
        , mCoreShader               ( NULL )
        , mTechnique                ( NULL )
        , mMMDPass                  ( MMD_PASS_object )
        , mFlags                    ( 0 )
        , mCommandExistFlags        ( 0 )
		, mValid					( false )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    SceneShaderTechnique::~SceneShaderTechnique()
    {
		ln_foreach( DrawingCommandList* list, mPassDrawingCommandListArray )
		{
			SAFE_DELETE( list );
		}
		mPassDrawingCommandListArray.clear();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void SceneShaderTechnique::initialize( SceneShader* shader, Graphics::IShaderTechnique* tech )
    {
        mSceneShader    = shader;
        mCoreShader     = mSceneShader->getCoreShader();
        mTechnique      = tech;
        mMMDPass        = MMD_PASS_object; // �f�t�H���g�� "object"
		mValid			= false;

		mDrawingCommandList.initialize( mSceneShader );

        //-------------------------------------------
        // MMD �p�X�ƃA�m�e�[�V�����𒲂ׂ�

        const lnChar* mmd_pass = NULL;
        bool use_texture = false;
        bool use_sphere_map = false;
        bool use_toon = false;
        Graphics::IShaderVariable* anno = NULL;

        anno = mTechnique->getAnnotationByName( _T( "MMDPass" ) );
        if ( anno )
        {
            mmd_pass = anno->getString();
            if ( mmd_pass )
            {
                if      ( _tcsicmp( mmd_pass, "object" ) == 0 ) {
					mMMDPass = MMD_PASS_object;
				}
                else if ( _tcsicmp( mmd_pass, "zplot" ) == 0 ) {
					mMMDPass = MMD_PASS_zplot;
				}
                else if ( _tcsicmp( mmd_pass, "object_ss" ) == 0 ) {
					mMMDPass = MMD_PASS_object_ss;
				}
                else if ( _tcsicmp( mmd_pass, "shadow" ) == 0 ) {
					mMMDPass = MMD_PASS_shadow;
				}
                else if ( _tcsicmp( mmd_pass, "edge" ) == 0 ) {
					mMMDPass = MMD_PASS_edge;
				}
            }
        }

        anno = mTechnique->getAnnotationByName( _T( "UseTexture" ) );
        if ( anno ) { 
        	use_texture = anno->getBool(); 
			if ( use_texture ) {
				mFlags |= MME_TECHSTATE_UseTexture;
			}
		} 
		else { 
			mFlags |= MME_TECHSTATE_OMITTED_Texture; 
		}

        anno = mTechnique->getAnnotationByName( _T( "UseSphereMap" ) );
        if ( anno ) { 
        	use_sphere_map = anno->getBool(); 
			if ( use_sphere_map ) {
				mFlags |= MME_TECHSTATE_UseSphereMap;
			}
		} 
		else { 
			mFlags |= MME_TECHSTATE_OMITTED_SphereMap; 
		}

        anno = mTechnique->getAnnotationByName( _T( "UseToon" ) );
        if ( anno ) { 
        	use_toon = anno->getBool(); 
			if ( use_toon ) {
				mFlags |= MME_TECHSTATE_UseToon;
			}
		} 
		else { 
			mFlags |= MME_TECHSTATE_OMITTED_Toon; 
		}

        //mFlags |=
        //    ( use_texture )     ? MME_TECHSTATE_UseTexture : 0 ||
        //    ( use_sphere_map )  ? MME_TECHSTATE_UseSphereMap : 0 ||
        //    ( use_toon )        ? MME_TECHSTATE_UseToon : 0;

        //-------------------------------------------
        // �`�悷��Subset�ԍ��̎w��𒲂ׂ�

        const lnChar* subset_text = NULL;
        mSubsetIndexArray.clear();

        anno = mTechnique->getAnnotationByName( _T( "Subset" ) );
        if ( anno ) { 
        	subset_text = anno->getString(); }

        if ( subset_text )
        {
            SceneShaderUtil::makeSubsetIndexArray( &mSubsetIndexArray, subset_text );
        }

		mDrawingCommandList.setDrawingSubsetNumbers( mSubsetIndexArray );
        
        //-------------------------------------------
        // Script

        // "Script" �A�m�e�[�V���������邩�`�F�b�N 
        Graphics::IShaderVariable* sc_var;
        sc_var = mTechnique->getAnnotationByName( _T( "Script" ) );
        if ( sc_var )
        {
            // ������^�̏ꍇ�̓X�N���v�g��͂ɉ�
            if ( sc_var->getTypeDesc().Type == LN_SVT_STRING )
            {
                if ( !_lexScriptCommandString( sc_var, NULL, &mDrawingCommandList ) )
				{
					return;
				}
            }
        }
        // �ȗ�����Ă���ꍇ�̓p�X�̕������̏���
        else
        {
            lnU32 i = 0;
            Graphics::IShaderPass* pass;
            
            while ( true )
            {
                pass = mTechnique->getPassByIndex( i );
                if ( pass )
                {
                    // �p�X���̃X�N���v�g���
                    sc_var = pass->getAnnotationByName( _T( "Script" ) );
                    if ( !_lexScriptCommandString( sc_var, pass, &mDrawingCommandList ))
					{
						return;
					}
					mDrawingCommandList.add_Pass( pass, NULL );
                }
                else
                {
                    break;
                }
                ++i;
            }
        }

        //-------------------------------------------
        // ���g�`�F�b�N
#if 0
        printf( "�� Script\n" );
        MMEScriptCommandList::iterator it = mScriptCommandList.begin();
        for ( ; it != mScriptCommandList.end(); ++it )
        {
            MMEScriptCommand* c = &(*it);
            switch ( c->Type )
            {
                case MME_SCRIPTCOM_RenderColorTarget:           
                	printf( "RenderColorTarget index:%d var:%p\n", c->Index, c->Variable ); break;
                case MME_SCRIPTCOM_RenderDepthStencilTarget:    
                	printf( "RenderDepthStencilTarget\n" ); break;
                case MME_SCRIPTCOM_ClearSetColor:               
                	printf( "ClearSetColor\n" ); break;
                case MME_SCRIPTCOM_ClearSetDepth:               
                	printf( "ClearSetDepth\n" ); break;
                case MME_SCRIPTCOM_Clear_Color:                 
                	printf( "Clear_Color        %X\n", (it)->Color ); break;
                case MME_SCRIPTCOM_Clear_Depth:                 
                	printf( "Clear_Depth        %f\n", (it)->Depth ); break;
                case MME_SCRIPTCOM_ScriptExternal_Color:        
                	printf( "ScriptExternal_Color\n" ); break;
                case MME_SCRIPTCOM_Pass:                        
                	printf( "Pass\n" ); break;
                case MME_SCRIPTCOM_BeginPass:                   
                	printf( "BeginPass\n" ); break;
                case MME_SCRIPTCOM_EndPass:                     
                	printf( "EndPass\n" ); break;
                case MME_SCRIPTCOM_LoopByCount:                 
                	printf( "LoopByCount\n" ); break;
                case MME_SCRIPTCOM_LoopEnd:                     
                	printf( "LoopEnd\n" ); break;
                case MME_SCRIPTCOM_LoopGetIndex:                
                	printf( "LoopGetIndex\n" ); break;
                case MME_SCRIPTCOM_Draw_Geometry:               
                	printf( "Draw_Geometry\n" ); break;
                case MME_SCRIPTCOM_Draw_Buffer:                 
                	printf( "Draw_Buffer\n" ); break;
                case MME_SCRIPTCOM_RenderPort:                  
                	printf( "RenderPort\n" ); break;
            }  
        }
#endif
		mValid = true;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool SceneShaderTechnique::_lexScriptCommandString( Graphics::IShaderVariable* shaderVar, Graphics::IShaderPass* pass, DrawingCommandList* commandList )
    {
        const lnChar* script = NULL;
        lnChar* str = NULL;
        char* s;
        char* tp;
        Graphics::IShaderVariable* tv;

        if ( shaderVar )
        {
            script = shaderVar->getString();

            if ( script )
            {
                // ��؂蕶��(;)���I�[�����ɒu�����ď��������邽�߁A��x�S���R�s�[����
                str = Base::StringUtils::copyLength( script, -1 );

                s = tp = str;

                while ( tp != NULL )
                {
                    // ; ��T���� '\0' �ɒu���B������ΏI�[�Ƃ������ƂŏI��
                    tp = strpbrk( s, _T( ";" ) );
                    if ( tp )
                    {
                        *tp = '\0';
                    }
                    else
                    {
                        break;
                    }

                    // �u������ '\0' �܂ŁA(������̕�����v�ł�����) �L�[���[�h��T���Ă���
                    switch ( *s )
                    {
                        /////////////////////////////
                        // RenderColorTarget_0�`3
                        // RenderDepthStencilTarget
                        // RenderPort
                        case 'R':
                        {
                            ///////////////////////////// RenderColorTarget_0�`3
                            if ( *( s + 6 ) == 'C' )
                            {
                                // �ݒ�C���f�b�N�X�𒲂ׂ�
                                lnU32 index;
								char lc = *( s + 17 );
                                if ( '0' <= lc && lc <= '3' )
                                {
                                    index = lc - '0';
                                }
                                else
                                {
                                    index = 0;
                                }

                                // �e�N�X�`���ϐ�����e�N�X�`�����Ƃ��Ă���
                                char* name = strpbrk( s, "=" ) + 1;

								// �e�N�X�`���ϐ������ȗ�����Ă���
                                if ( name == NULL || *name == '\0' )	// TODO: nullOrEmpty
                                {
									commandList->add_RenderColorTarget( index, NULL );
                                }
                                else
                                {
                                    MMEShaderVariable* mmeVariable = mSceneShader->findShaderVariable( name );

                                    // �ϐ��� RT ��v�����Ă��邩�`�F�b�N
                                    if ( mmeVariable && 
										( mmeVariable->Request == MME_VARREQ_RENDERCOLORTARGET || 
										  mmeVariable->Request == MME_VARREQ_OFFSCREENRENDERTARGET ) )
                                    {
										commandList->add_RenderColorTarget( index, mmeVariable->Variable );
                                    }
                                    else
                                    {
                                        LN_SET_SHADER_ERR_MSG(
                                            _T( "RenderColorTarget invalid texture name.\n%s" ),
                                            name );
                                        return false;
                                    }
                                }
                            }
                            ///////////////////////////// RenderDepthStencilTarget
                            else if ( *( s + 6 ) == 'D' )
                            {
                                // �e�N�X�`���ϐ�����e�N�X�`�����Ƃ��Ă���
                                char* name = strpbrk( s, "=" ) + 1;
                                if ( name == NULL || *name == '\0' )
                                {
									commandList->add_RenderDepthStencilTarget( NULL );
                                }
                                else
                                {
                                    MMEShaderVariable* mmeVariable = mSceneShader->findShaderVariable( name );

                                    // �ϐ����[�x�o�b�t�@��v�����Ă��邩�`�F�b�N
                                    if ( mmeVariable && mmeVariable->Request == MME_VARREQ_RENDERDEPTHSTENCILTARGET )
                                    {
										commandList->add_RenderDepthStencilTarget( mmeVariable->Variable );
                                    }
                                    else
                                    {
                                        LN_SET_SHADER_ERR_MSG(
                                            _T( "RenderDepthStencilTarget Texture name is invalid.\n%s" ),
                                            name );
                                        return false;
                                    }
                                }
                            }
                            ///////////////////////////// RenderPort
                            else if ( *( s + 6 ) == 'P' )
                            {
                                // MME ���̂��������̃R�}���h
                            }
                            break;
                        }

                        /////////////////////////////
                        // ClearSetColor
                        // ClearSetDepth
                        // Clear_Color
                        // Clear_Depth
                        case 'C':
                        {
                            ///////////////////////////// ClearSetColor
                            if ( *( s + 8 ) == 'C' )
                            {
                                lnChar* name = strpbrk( s, "=" ) + 1;
								// ��Ŏg��
                                //Graphics::IShaderVariable* coreVariable = mCoreShader->getVariableByName( name );

                                LColorF color;
                                tv = mCoreShader->getVariableByName( name );
                                if ( !tv )
                                {
                                    LN_SET_SHADER_ERR_MSG(
                                        _T( "ClearSetColor Parameter name is invalid.\n%s" ),
                                        name );
                                        return false;
                                }

                                color = tv->getVector();
								commandList->add_ClearSetColor( color );
                            }
                            ///////////////////////////// ClearSetDepth
                            else if ( *( s + 8 ) == 'D' )
                            {
                                lnChar* name = strpbrk( s, "=" ) + 1;
								// ��Ŏg��
                                Graphics::IShaderVariable* coreVariable = mCoreShader->getVariableByName( name );
                                
                                lnFloat depth;
                                tv = mCoreShader->getVariableByName( name );
                                if ( !tv )
                                {
                                    LN_SET_SHADER_ERR_MSG(
                                        _T( "ClearSetDepth Parameter name is invalid.\n%s" ),
                                        name );
                                        return false;
                                } 
                                depth = tv->getFloat();
								commandList->add_ClearSetDepth( depth );
                            }
                            ///////////////////////////// Clear_Color
                            else if ( *( s + 6 ) == 'C' )
                            {
								commandList->add_ClearColor();
                            }
                            ///////////////////////////// Clear_Depth
                            else if ( *( s + 6 ) == 'D' )
                            {
								commandList->add_ClearDepth();
                            }
                            break;
                        }

                        /////////////////////////////
                        // ScriptExternal_Color
                        case 'S':
                        {
							commandList->add_ScriptExternal_Color();
                            mCommandExistFlags |= MME_SCEXIST_ScriptExternal_Color;
                            break;
                        }

                        /////////////////////////////
                        // Pass
                        case 'P':
                        {
                            char* name = strpbrk( s, "=" ) + 1;
                            Graphics::IShaderPass* pass = mTechnique->getPassByName( name );
                            if ( pass )
                            {
                                // �p�X���ɂ���� Script ������ꍇ�͍ċN (�Ȃ��ꍇ�� Draw_Geometry ���ݒ肳���)
                                Graphics::IShaderVariable* var = pass->getAnnotationByName( "Script" );

								if ( var )
								{
									DrawingCommandList* list = LN_NEW DrawingCommandList();
									list->initialize( mSceneShader );
									mPassDrawingCommandListArray.push_back( list );

									// �ċA
									bool r = _lexScriptCommandString( var, pass, list );
									if ( !r ) return false;

									commandList->add_Pass( pass, list );
								}
								else
								{
									_lexScriptCommandString( var, pass, commandList );
									//LN_CALL_R( _lexScriptCommandString( var, pass, NULL ) );
								}
                            }
                            else
                            {
                                LN_SET_SHADER_ERR_MSG(
                                    _T( "Pass Name is invalid.\n%s" ),
                                    name );
                                    return false;
                            }
                            break;
                        }

                        /////////////////////////////
                        // LoopByCount
                        // LoopEnd
                        // LoopGetIndex
                        case 'L':
                        {
                            //MMEScriptCommand com;

                            ///////////////////////////// LoopByCount
                            if ( *( s + 4 ) == 'B' )
                            {
                                char* name = strpbrk( s, "=" ) + 1;
                                tv = mCoreShader->getVariableByName( name );
                                if ( !tv )
                                {
                                    LN_SET_SHADER_ERR_MSG(
                                        _T( "LoopByCount Parameter name is invalid.\n%s" ),
                                        name );
                                        return false;
                                }
								int loopCount = 0;
                                loopCount = tv->getInt();

								commandList->add_LoopByCount( loopCount );
                            }
                            ///////////////////////////// LoopEnd
                            else if ( *( s + 4 ) == 'E' )
                            {
								commandList->add_LoopEnd();
                            }
                            ///////////////////////////// LoopGetIndex
                            else if ( *( s + 4 ) == 'G' )
                            {
                                char* name = strpbrk( s, "=" ) + 1;
                                Graphics::IShaderVariable* loopVar = mCoreShader->getVariableByName( name );
								
								commandList->add_LoopGetIndex( loopVar );

                                if ( !loopVar )
                                {
                                    LN_SET_SHADER_ERR_MSG(
                                        _T( "LoopGetIndex Parameter name is invalid.\n%s" ),
                                        name );
                                        return false;
                                }
                            }
                            break;
                        }

                        /////////////////////////////
                        // Draw_Geometry
                        // Draw_Buffer
                        case 'D':
                        {
                            ///////////////////////////// Draw_Buffer
                            if ( *( s + 5 ) == 'B' )
                            {
                                mCommandExistFlags |= MME_SCEXIST_Draw_Buffer;

								commandList->add_DrawBuffer( pass );
                            }
                            ///////////////////////////// Draw_Geometry
                            else
                            {
                                mCommandExistFlags |= MME_SCEXIST_Draw_Geometry;

								commandList->add_DrawGeometry( pass );
                            }
                            break;
                        }
                    }

                    s = tp + 1; // ; �̎����w��
                }

                SAFE_DELETE_ARRAY( str );
            }
        }

        // �p�X�̏������ŁA�X�N���v�g�������������ꍇ�� Draw_Geometry �ЂƂ������l�߂�
        if ( !script && pass )
        {
			mCommandExistFlags |= MME_SCEXIST_Draw_Geometry;
			commandList->add_DrawGeometry( pass );
        }
        return true;
    }

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================