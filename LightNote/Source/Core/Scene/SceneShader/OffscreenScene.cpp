//==============================================================================
// OffscreenScene 
//------------------------------------------------------------------------------
/*
	■DefaultEffect アノテーションのためのグループ分け
		ひとつの OffscreenScene 内でのエフェクトファイルの割り当て方法や
		描画そのものの有無を決めるため、ワイルドカード指定可能なオブジェクト名の
		マッチング判定が行われる。
		ただ、この処理を毎フレーム行うのはかなり処理時間的にキツイ。

		そこで、一度マッチングを行った後はどのエフェクトを割り当てるかを示す
		整数値を持たせて、以降これを使って割り当てを決める。
		例えば以下のようになる。

		string DefaultEffect = 
            "self = hide;"				… グループ 0
            "Mirror*.x = hide;"			… グループ 0
            "Player1=MirrorMain.fx;";	… グループ 1
            "Player2=MirrorMain.fx;";	… グループ 1
            "*=MirrorObject.fx;";		… グループ 2

		もしいずれにもマッチしなければグループ -1。
		グループ -1 は非表示を表す。
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
// ■ OffscreenScene
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

		// TODO:オフスクリーンで使用する深度バッファは公開する必要ないので、同サイズは共有できるようにしたい
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

		// 最後に obj の mOffscreenSceneID 番目のフィルタ情報を設定したものが this ではない
		// (obj が新しく作成されたオブジェクトであるか、
		//	前の OffscreenScene が解放され ID が返却された後、新たに作成された OffscreenScene が同じ ID を取得した場合)
		if ( filterGroup->OffscreenScene != this )
		{
			filterGroup->OffscreenScene = this;
			filterGroup->Group = -1;	// 一致なしを考慮して、初期値は非表示グループ

			int count = mDefaultEffectEntryList.size();
			for ( int i = 0 ; i < count; ++i )
			{
				DefaultEffectEntry& e = mDefaultEffectEntryList[i];

				// そのまま追加
				if ( e.ObjectName == _T( "*" ) )
				{
					filterGroup->Group = i;
					break;
				}
				// このオフスクリーンRTを持つシェーダが設定されているノード自身
				else if ( e.ObjectName == _T( "self" ) )
				{
					if ( obj->getShader() && obj->getShader() == mSceneShader/*e.PriorityParameter.Shader*/ )
					{
						filterGroup->Group = i;
						break;
					}
				}
				// ワイルドカード付きの比較
				else if ( Base::StringUtils::match( e.ObjectName.c_str(), obj->getName() ) )
				{
					filterGroup->Group = i;
					break;
				}
			}
		}

		// この OffscreenScene を生成したシェーダと同一のシェーダを持っているものを適当にチョイスする
		// (OffscreenScene を含むシェーダは基本的に共有をサポートしない。そのため、一意になるはず)
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


		// 初回実行時等、すべてのノードに対してこのオフスクリーンRTで描画するべきかをチェックして選択
        //if ( mNeedRefreshAffectAllNodes )
        //{
        //    _refreshAffectAllNodes( scene );
        //    mNeedRefreshAffectAllNodes = false;
        //}

        // オフスクリーン描画時の基準カメラをデフォルトのもの以外にするときは
        // この辺に更新処理を書いておく

        // 以下のノード配列の取得も、事前に視錐台カリングした後の
        // 一覧について行えばもっと最適化できるかも

        // このオフスクリーンRTで描画するノードには優先パラメータを設定。
        // 描画しない場合は hide。
		// TODO: ここ少し無駄な気がする…  hide も含めて mSceneNodeEntryArray に入れてしまえば？
        //MMESceneObjectArray& node_list = scene->getMMESceneObjectArray();
        //ln_foreach( MMESceneObjectBase* node, node_list )
        //{
        //    node->setPriorityParameter( &LNPriorityParameter::HIDE_PARAM );
        //}
        //ln_foreach( SceneNodeEntry& e, mSceneNodeEntryArray )
        //{
        //    //LNPriorityParameter pp;
        //    //pp.Shader = e.Shader;
        //    //pp.Hide   = true;       // mSceneNodeEntryArray に入っているノードはすべて false でOK
        //    e.Node->setPriorityParameter( &e.PriorityParameter );
        //}

		if ( mOffscreenOwner )
		{
			ln_foreach( DefaultEffectEntry& e, mDefaultEffectEntryList )
			{
				if ( e.PriorityParameter.Shader ) {	// "Mirror*.x = hide;" 等は Shader=NULL
					e.PriorityParameter.Shader->updateOffscreenOwner( mOffscreenOwner );
				}
			}

			// 下の renderRootNode->render( param ); でも設定される可能性がある
			mOffscreenOwner = NULL;
		}


        // 前準備
		Graphics::Texture* oldRT = param.Renderer->getRenderTarget( 0 );
		Graphics::Texture* oldDepth = param.Renderer->getDepthBuffer();	// TODO:Depthも作る必要がありそう
		param.Renderer->setRenderTarget( 0, mRTTexture );
		param.Renderer->setDepthBuffer( mDepthTexture );
		param.Renderer->clear( true, true, mClearColor, mClearDepth );

		// 描画
		param.OffscreenScene = this;
		renderRootNode->render( param );
		param.OffscreenScene = NULL;

        // 後処理
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

        // ; で分割 (終端が ; の場合、それは無視)
		std::vector< lnString > entrys;
		mDefaultEffectStrings.Split(_T(";"), Base::StringSplitOptions_RemoveEmptyEntries, &entrys);
		//std::vector< lnString > entrys = mDefaultEffectStrings
            //Base::StringUtils::split( mDefaultEffectStrings.c_str(), _T( ";" ) );

		ln_foreach(lnString& entry, entrys)
        {
            // 次に = で分割
			std::vector< lnString > items;
			entry.Split(_T("="), Base::StringSplitOptions_RemoveEmptyEntries, &entrys);
			//std::vector< lnString > items =
			//	Base::StringUtils::split(entry, _T("="), StringSplitOptions_RemoveEmptyEntries);

            // 分割後の要素数は必ず 2。それ以外はエラー
            if ( items.size() == 2 )
            {
                // 前後のスペースを削除
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
                    // 必要なシェーダを作成
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

                // "self" がある？
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