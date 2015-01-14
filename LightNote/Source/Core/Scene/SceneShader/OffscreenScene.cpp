//==============================================================================
// OffscreenScene 
//------------------------------------------------------------------------------
/*
	��DefaultEffect �A�m�e�[�V�����̂��߂̃O���[�v����
		�ЂƂ� OffscreenScene ���ł̃G�t�F�N�g�t�@�C���̊��蓖�ĕ��@��
		�`�悻�̂��̗̂L�������߂邽�߁A���C���h�J�[�h�w��\�ȃI�u�W�F�N�g����
		�}�b�`���O���肪�s����B
		�����A���̏����𖈃t���[���s���̂͂��Ȃ菈�����ԓI�ɃL�c�C�B

		�����ŁA��x�}�b�`���O���s������͂ǂ̃G�t�F�N�g�����蓖�Ă邩������
		�����l���������āA�ȍ~������g���Ċ��蓖�Ă����߂�B
		�Ⴆ�Έȉ��̂悤�ɂȂ�B

		string DefaultEffect = 
            "self = hide;"				�c �O���[�v 0
            "Mirror*.x = hide;"			�c �O���[�v 0
            "Player1=MirrorMain.fx;";	�c �O���[�v 1
            "Player2=MirrorMain.fx;";	�c �O���[�v 1
            "*=MirrorObject.fx;";		�c �O���[�v 2

		����������ɂ��}�b�`���Ȃ���΃O���[�v -1�B
		�O���[�v -1 �͔�\����\���B
*/
//==============================================================================

#include "stdafx.h"
#include "../../Graphics/Device/Texture.h"
#include "../SceneGraph.h"
#include "../DrawCommand.h"
#include "SceneShader.h"
#include "SceneShaderManager.h"
#include "OffscreenScene.h"

#include "MMETextureObject.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// �� OffscreenScene
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    OffscreenScene::OffscreenScene( SceneShaderManager* manager, SceneShader* shader )
        : mManager						( manager )
        , mSceneShader					( shader )
        , mRTTexture					( NULL )
		, mDepthTexture					( NULL )
		, mOffscreenSceneID				( 0 )
		, mOffscreenOwner				( NULL )
        , mIsThereSelf					( false )
        , mNeedRefreshAffectAllNodes	( true )
    {
        LN_SAFE_ADDREF( mManager );
        mManager->addOffscreenScene( this );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    OffscreenScene::~OffscreenScene()
    {
        _releaseEffects();

        LN_SAFE_RELEASE( mRTTexture );
        LN_SAFE_RELEASE( mDepthTexture );
        
        if ( mManager )
        {
            mManager->removeOffscreenScene( this );
            LN_SAFE_RELEASE( mManager );
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void OffscreenScene::initialize(
		const LVector4 &	clearColor,
        lnFloat             clearDepth,
        lnRefString         description,
        lnRefString         defaultEffectStrings,
        Graphics::Texture*	renderTarget )
    {
        mErrorMessage.clear();
        setClearColor( clearColor );
        setClearDepth( clearDepth );
        setDescription( description );
        setDefaultEffectStrings( defaultEffectStrings );

        mRTTexture = renderTarget;
        LN_SAFE_ADDREF( mRTTexture );

		// TODO:�I�t�X�N���[���Ŏg�p����[�x�o�b�t�@�͌��J����K�v�Ȃ��̂ŁA���T�C�Y�͋��L�ł���悤�ɂ�����
		mDepthTexture = mManager->getGraphicsDevice()->createDepthBuffer( 
			(lnU32)mRTTexture->getSize().X, (lnU32)mRTTexture->getSize().Y, Graphics::SurfaceFormat_D24S8);

        _refreshEffects();

        mNeedRefreshAffectAllNodes = true;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void OffscreenScene::setClearColor( const LVector4& color_ )
    {
        mClearColor = reinterpret_cast< const LColorF& >( color_ );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNPriorityParameter* OffscreenScene::tryGetPriorityParameter( MMESceneObjectBase* obj )
	{
		OffscreenSceneFilterGroup* filterGroup = obj->getOffscreenSceneFilter( mOffscreenSceneID );

		// �Ō�� obj �� mOffscreenSceneID �Ԗڂ̃t�B���^����ݒ肵�����̂� this �ł͂Ȃ�
		// (obj ���V�����쐬���ꂽ�I�u�W�F�N�g�ł��邩�A
		//	�O�� OffscreenScene ��������� ID ���ԋp���ꂽ��A�V���ɍ쐬���ꂽ OffscreenScene ������ ID ���擾�����ꍇ)
		if ( filterGroup->OffscreenScene != this )
		{
			filterGroup->OffscreenScene = this;
			filterGroup->Group = -1;	// ��v�Ȃ����l�����āA�����l�͔�\���O���[�v

			int count = mDefaultEffectEntryList.size();
			for ( int i = 0 ; i < count; ++i )
			{
				DefaultEffectEntry& e = mDefaultEffectEntryList[i];

				// ���̂܂ܒǉ�
				if ( e.ObjectName == _T( "*" ) )
				{
					filterGroup->Group = i;
					break;
				}
				// ���̃I�t�X�N���[��RT�����V�F�[�_���ݒ肳��Ă���m�[�h���g
				else if ( e.ObjectName == _T( "self" ) )
				{
					if ( obj->getShader() && obj->getShader() == mSceneShader/*e.PriorityParameter.Shader*/ )
					{
						filterGroup->Group = i;
						break;
					}
				}
				// ���C���h�J�[�h�t���̔�r
				else if ( Base::StringUtils::match( e.ObjectName.c_str(), obj->getName() ) )
				{
					filterGroup->Group = i;
					break;
				}
			}
		}

		// ���� OffscreenScene �𐶐������V�F�[�_�Ɠ���̃V�F�[�_�������Ă�����̂�K���Ƀ`���C�X����
		// (OffscreenScene ���܂ރV�F�[�_�͊�{�I�ɋ��L���T�|�[�g���Ȃ��B���̂��߁A��ӂɂȂ�͂�)
		if ( obj->getShader() && obj->getShader() == mSceneShader ) {
			mOffscreenOwner = obj;
		}

		if ( filterGroup->Group >= 0 )
		{
			return &mDefaultEffectEntryList[filterGroup->Group].PriorityParameter;
		}
		return NULL;
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void OffscreenScene::render( MMESceneBase* scene, ContextDrawParam& param, MMESceneObjectBase* renderRootNode )
	{


		// ������s�����A���ׂẴm�[�h�ɑ΂��Ă��̃I�t�X�N���[��RT�ŕ`�悷��ׂ������`�F�b�N���đI��
        //if ( mNeedRefreshAffectAllNodes )
        //{
        //    _refreshAffectAllNodes( scene );
        //    mNeedRefreshAffectAllNodes = false;
        //}

        // �I�t�X�N���[���`�掞�̊�J�������f�t�H���g�̂��̈ȊO�ɂ���Ƃ���
        // ���̕ӂɍX�V�����������Ă���

        // �ȉ��̃m�[�h�z��̎擾���A���O�Ɏ�����J�����O�������
        // �ꗗ�ɂ��čs���΂����ƍœK���ł��邩��

        // ���̃I�t�X�N���[��RT�ŕ`�悷��m�[�h�ɂ͗D��p�����[�^��ݒ�B
        // �`�悵�Ȃ��ꍇ�� hide�B
		// TODO: �����������ʂȋC������c  hide ���܂߂� mSceneNodeEntryArray �ɓ���Ă��܂��΁H
        //MMESceneObjectArray& node_list = scene->getMMESceneObjectArray();
        //ln_foreach( MMESceneObjectBase* node, node_list )
        //{
        //    node->setPriorityParameter( &LNPriorityParameter::HIDE_PARAM );
        //}
        //ln_foreach( SceneNodeEntry& e, mSceneNodeEntryArray )
        //{
        //    //LNPriorityParameter pp;
        //    //pp.Shader = e.Shader;
        //    //pp.Hide   = true;       // mSceneNodeEntryArray �ɓ����Ă���m�[�h�͂��ׂ� false ��OK
        //    e.Node->setPriorityParameter( &e.PriorityParameter );
        //}

		if ( mOffscreenOwner )
		{
			ln_foreach( DefaultEffectEntry& e, mDefaultEffectEntryList )
			{
				if ( e.PriorityParameter.Shader ) {	// "Mirror*.x = hide;" ���� Shader=NULL
					e.PriorityParameter.Shader->updateOffscreenOwner( mOffscreenOwner );
				}
			}

			// ���� renderRootNode->render( param ); �ł��ݒ肳���\��������
			mOffscreenOwner = NULL;
		}


        // �O����
		Graphics::Texture* oldRT = param.Renderer->getRenderTarget( 0 );
		Graphics::Texture* oldDepth = param.Renderer->getDepthBuffer();	// TODO:Depth�����K�v�����肻��
		param.Renderer->setRenderTarget( 0, mRTTexture );
		param.Renderer->setDepthBuffer( mDepthTexture );
		param.Renderer->clear( true, true, mClearColor, mClearDepth );

		// �`��
		param.OffscreenScene = this;
		renderRootNode->render( param );
		param.OffscreenScene = NULL;

        // �㏈��
        param.Renderer->setRenderTarget( 0, oldRT );
		param.Renderer->setDepthBuffer( oldDepth );
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void OffscreenScene::_refreshEffects()
    {
        lnString tms;
        mIsThereSelf = false;

        _releaseEffects();
        mDefaultEffectEntryList.clear();

        // ; �ŕ��� (�I�[�� ; �̏ꍇ�A����͖���)
		std::vector< lnString > entrys;
		mDefaultEffectStrings.Split(_T(";"), Base::StringSplitOptions_RemoveEmptyEntries, &entrys);
		//std::vector< lnString > entrys = mDefaultEffectStrings
            //Base::StringUtils::split( mDefaultEffectStrings.c_str(), _T( ";" ) );

		ln_foreach(lnString& entry, entrys)
        {
            // ���� = �ŕ���
			std::vector< lnString > items;
			entry.Split(_T("="), Base::StringSplitOptions_RemoveEmptyEntries, &entrys);
			//std::vector< lnString > items =
			//	Base::StringUtils::split(entry, _T("="), StringSplitOptions_RemoveEmptyEntries);

            // ������̗v�f���͕K�� 2�B����ȊO�̓G���[
            if ( items.size() == 2 )
            {
                // �O��̃X�y�[�X���폜
                Base::StringUtils::trimHFSpace( &items[0], items[0] );
                Base::StringUtils::trimHFSpace( &items[1], items[1] );

                DefaultEffectEntry e;
                e.ObjectName = items[0].c_str();

                if ( items[1] == _T( "none" ) )
                {
                    //e.Type = ENTRY_NONE;
					//e.Shader = NULL;
					e.PriorityParameter.Hide = false;
					e.PriorityParameter.Shader = NULL;
                }
                else if ( items[1] == _T( "hide" ) )
                {
                    //e.Type = ENTRY_HIDE;
                    //e.Shader = NULL;
					e.PriorityParameter.Hide = true;
					e.PriorityParameter.Shader = NULL;
                }
                else
                {
                    // �K�v�ȃV�F�[�_���쐬
					lnString errors;	// TODO
					LRefPtr<SceneShader> shader(
						mManager->createSceneShader(items[1].c_str(), &errors));
                    //e.Type       = ENTRY_EFFECT;
                    e.EffectName = items[1].c_str();
                    //e.Shader     = shader;
					e.PriorityParameter.Hide = false;
					e.PriorityParameter.Shader = shader;
					e.PriorityParameter.Shader->addRef();
                }

                mDefaultEffectEntryList.push_back( e );

                // "self" ������H
                if ( items[1].c_str() == _T( "self" ) )
                {
                    mIsThereSelf = true;
                }
            }
            else
            {
                mErrorMessage = _T( "DefaultEffect item is disable." );
            }
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void OffscreenScene::_releaseEffects()
    {
        ln_foreach( DefaultEffectEntry& e, mDefaultEffectEntryList )
        {
			LN_SAFE_RELEASE( e.PriorityParameter.Shader );
        }
    }

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================