//==============================================================================
// Manager 
//------------------------------------------------------------------------------
/*
	■要求…
		・ユーザーコントロールが作れるようにしたい
			→	クラス的な継承ではなく、コンポーネント志向みたいな感じで、
				好きなだけ好きなプロパティを継ぎ足せるようなのがいい。
				で、それをUnityプレハブみたいに取っておいて、いつでも使えるようにする。
				→	テンプレート。
					テンプレートからインスタンスを作れるのは当然として、
					インスタンスをテンプレート化することもできる。
					→	テンプレート化するのは、プロパティとイベント。
						
		・イベントハンドラの登録
			delegate は使っちゃダメ。言語バインダ作りにくくなる。
			→	「イベントプロパティ」という種類のプロパティが必要。(WPFもあるやつ)
			→	イベントプロパティへの static コールバック関数登録
			→	グローバルにも1つ欲しい。どんなイベントが発生したときでも必ず通るところ。HSP用。

		・アニメーション

		・データバインディング
			「あれば使えるかも」止まりな気がする。
			ViewModel 実装も、C/C++ にしても言語バインダの作り方にしても、
			使う側にそれなりの知識と負担を掛けないといけない。
			使う以上 MVVM に似たパターンになるわけだけど、半ば強制的に疎結合が求められる MVVM、
			ゲームは業務アプリとは比較にならないほど柔軟性が求められるところだし、
			とりあえず見送りでいいかも。

		・XMLレイアウト
			・StaticResource/ResourceDictionary/MergedDictionary

		・ユーザーがその気になればリボンとかドッキングウィンドウとか実装できる柔軟な基盤

		・仮想化
			コールバック必須。言語バインダ考慮するとすごく面倒。
			クリップ領域外は描画しないくらいしかできないかも。

	■How to
		・テーマ変更
			ResourceDictionary 更新後、全ての Resource にバインドしているプロパティを更新

		・アイコン付きツリービュー
			① Image の FrameworkElementFactory を作り、先祖の UserControl(_ctrl とか名前付けて ElementName) の Icon プロパティ(自作)にバインドする。
			② TextBlock の FrameworkElementFactory を作り、(先祖の UserControl の Text?Content? プロパティ(自作))にバインドする。
			③ StackPanel の FrameworkElementFactory を作り、①と②を AddChild()
			④ TreeViewItem の ControlTemplate を作り、Icon ユーザープロパティをアタッチし、VisualTree プロパティに ③ をセットする。
			⑤ TreeViewItem を作り、Template プロパティに ④ をセットする。
			http://www.oborodukiyo.info/WPF/VS2008/WPF-DifferenceBetweenXAMLAndCode.aspx
			FrameworkElementFactory は作成する UIElement の型と、割り当てるプロパティと値をリストで持つ。
		
			



	■備忘録
		・ポップアップメニューは普通の VisualTree ではない。
			こんな感じになってた。
			PopupRoot
				Decorator
					NonLogicalAdornerDecorator
						SystemDropShadowChrome
							Border
								ScrollViewer
									Grid
										Border
											ScrollContentPresenter
												Grid
												ItemsPresenter
													StackPanel
														MenuItem

		・IAddChild インターフェース
			http://blogs.wankuma.com/shuujin/archive/2007/05/20/77391.aspx


	■How to (実用レベル)

		・WW のウィンドウ外観
			スタイル定義。クローズボタンなし。
			背景はRGSSWindowタイルスタイル、枠はLineで描画。
			もしくは WWWindow とかテンプレート作っておく。

		・WW の メインメニュー
			ルートは Grid で分割。RowSpan 使うなり、Grid をネストするなり。
			ただし、ウィンドウシステムを求めないようなGUIでは、
			Canvas を使った絶対座標指定の方が直感的にできて良いかもしれない。
			・上部メニュー
				2行のHorizontalリスト。(Window、ListBox、UniformGrid、ItemTemplate)
				Windowに項目名のコレクションのプロパティを持たせ、ListBoxとバインド。
				「決定ボタン」と「クリック」を結び付ける必要がある。
				→ グローバルな設定で、決定ボタン = スペースキー動作扱いするような仕組み
			・中央部ステータス
				・アクターごとのステータスを表示するユーザーコントロール
					(UserControl、Image、TextBlock)
					あと、状態異常アイコンリストとか。
				・↑のUserControl のリスト
			・右部インフォメーション
				(Window、Grid、TextBlock)
				Window に プレイ時間とか現在位置とかのプロパティを持たせ、
				各 TextBlock はそれにバインドする。

		・アイテム
			・背景
				メインメニューが半透明ガラスかけられたみたいに移ってる。
				→ 
			・カテゴリウィンドウ
			・アイテムリストウィンドウ
			・説明ウィンドウ
			・アイテム使用対象選択ウィンドウ
				単体選択・全体選択
				→ ItemsControl には選択状態を固定する機能が欲しい。(Aは常に選択状態にする、等、アイテムごとに)

		・WW メニュー共通
			キーヘルプウィンドウ
			フォーカスへのカーソル移動
			
		・WW 術技メニュー
			・アクターステータスウィンドウ
				[Image, TextBlock]
				アクターの歩行グラフィックがウィンドウの外にはみ出したい。
				→ クリッピングしない。ウィンドウ枠は手前に描画しなくていいかも。
			・操作方法ウィンドウ
				テキストリスト
			・割り当てリストウィンドウ
				[ListBox, Image(パラパラアニメ), TextBlock, Template]
				キーのアイコンをアニメーションしたい。
				ListBox アイテム個別の高さ設定。(今回入らないけどセパレータあったほうがいいかも？)
			・スキルリストアクターウィンドウ
				ショートカットスキルの設定で使う。L2 R2 ボタンにセットするときのみ表示される。
				アイテムのカテゴリウィンドウっぽくなると思うけど特に難しくない。
			・スキルリストウィンドウ
				普通にアイコンとテキストのリストで。
			・スキル説明ウィンドウ (フローティング)
				基本文字だけでOK。必要に応じて属性などつくが、基本テキスト。
				フォーカス位置によって、画面上の見やすい位置に自動移動するような機能があれば・・・（自前でやった）
			・スキルターゲットウィンドウ
				アイテムのと同じ
				
	■memo

		・ネストされたUserControl
			・WinForm ライク
				Loaded または Update イベントで、子を作る。
				ユーザーデータが深くかかわるものはコード側で作った方が直感的かも。




		・役割と言語バインダ
			そもそもデータバインドが必要なのは、データソースがある時。
			コントロールライブラリとかで拡張コントロールを作るときは
			(内部的には使うかもしれないけど)ユーザーデータの形式を強制する
			データバインドは使わないはず。それよりもイベントハンドラを頻繁に作りそう。
			一方実際のアプリで使うものは StatusWindow みたいに、コモンである Window にたいして
			そのアプリで使用するデータ用に特化させたものを作る。
			こっちは逆にデータバインドを使いそうな気がするが・・。

		テンプレートは無い方がいい？
			コレクションのバインドが難しい…
			・

		・Binding はあくまで「ちょっとしたユーティリティ」程度にとどめておいた方がいいかも。
			ViewModel 使わない時点で、ItemsSource 等のコレクションバインディングが多分不可能になる。
			「ElementName を使ったローカルな他のコントロールのプロパティだけ参照可能」な感じで。


		・ListBoxItem、TreeViewItem
			が、子コントロールをホストする

		・ListBox や ComboBox の Separator
			PrepareContainer という intrenal メソッドで、親コンテナ (ListBoxItem 等) の Enabled を false にしている。
			リストには要素として入っている。
			http://reflector.webtropy.com/default.aspx/Dotnetfx_Win7_3@5@1/Dotnetfx_Win7_3@5@1/3@5@1/DEVDIV/depot/DevDiv/releases/Orcas/NetFXw7/wpf/src/Framework/System/Windows/Controls/Separator@cs/1/Separator@cs

	■どこまでをGUIモジュール内でクラス化するか？
		UI要素の性質を大きく決めるところまでと 各種Presenter でいいと思う。
		具体的には Control、ItemsControl、Shape、Adorner、Pane、ContentPresenter とか。

		UIElement
			ContentPresenter
			Control	…子要素を持つ要素の基底。また、TABキーによるフォーカス移動を行う単位。
				ContentControl	…子要素をひとつ持つ要素の基底。
					Label
					ListBoxItem
					ButtonBase
						Button
						RepeatButton
						ToggleButton
							RadioButton
					ScrollViewer
					ToolTip
					UserControl
					Window			… 最小化、最大化等も。
				ItemsControl	…複数の子要素を持つ要素の基底。
					(DataGridCellsPresenter)
					HeaderedItemsControl
						MenuItem
						(ToolBar)
						TreeViewItem
					MenuBase
						Menu
					Selector
						ComboBox
						ListBox
							ListView
						TabControl
					TreeView
				RangeBase
					ScrollBar
					ProgressBar
					Slider
				Separator
				TextBoxBase
					(RichTextBox)
					TextBox
			Decorator
				Border
				Viewbox
				AdornerDecorator
			Image
			ItemsPresenter
			Page
			Panel
				Canvas
				DockPanel
				Grid
				UniformGrid
				StackPanel
				WrapPanel
			TextBlock
			Adorner
			AdornerLayer
			Shape
              Ellipse
              Line
              Path
              Polygon
              Polyline
              Rectangle

	■各言語のユーザーコントロールの作り方 … NumUpDown を作ってみる
		・C#
			①UserControl クラスを継承した NumUpDown クラスを作成
			②Grid, TextBox, Button を子コントロールとして登録
			③TextBox の テキスト描画領域の右側を空ける (Template で "ContentElement" と名前の付いた ScrollViewer の Margin を変える http://msdn.microsoft.com/ja-jp/library/cc645061%28v=vs.95%29.aspx)
			④static に ValueProperty 作成。コンストラクタで登録
			⑤static に ValueChanged イベント作成。コンストラクタで登録
			⑦AddHandler() で、2つの Button の Click イベントを受け取るようにする。
				Create() {
					this.AddHandler(Button.Click, 1);
					this.AddHandler(Button.Click, 2);
					this.AddHandler(TextBox.TextChanged, 3);
				}
				OnEvent(ev) {
					if (ev.Event == UIElement.PropertyChanged && ev.PropName == "Value")
						RaiseEvent(ValueChanged, new EventArgs());
					if (ev.Event == Button.Click && ev.MessageId == 1)
						// Upクリック
						// Text 更新
					if (ev.Event == Button.Click && ev.MessageId == 2)
						// Down クリック
						// Text 更新
					if (ev.Event == TextBox.TextChanged && ev.MessageId == 3)
						SetValue(ValueProperty, ((TextBox)ev.Sender).Text.ToInt()); → PropertyChanged
				}

		・Cで作ってユーザーコントロールとして配布

	■ユーザーコントロールライブラリ
		ビハインドコードはC/C++じゃなくて、いっそスクリプト化する？mrubyとか。
		こうすれば配布しやすくなる気がするが・・・。

		気を付ける点もある。mruby は C 側をメインで利用しようとすると少し面倒。
		http://blog.64p.org/entry/2012/09/12/065743

	■VisualStateManager


	■internal class StyleHelper
		http://referencesource.microsoft.com/#PresentationFramework/src/Framework/System/Windows/StyleHelper.cs
		Style とそれを適用したい FrameworkElement を渡すと、
		FrameworkElement を更新してくれる static 関数の実装。

	■インスタンスの作成
		ファクトリ関数を通すべき。C++で実装したコントロールクラスは
		ファクトリ関数に指定された名前で作れなければならない。
		C++で何かクラス作ったら、それの生成関数は Manager に名前とともに登録する。
		直接の new はさせてもいいが、生成経路が2つあるのが気になる。あえて統一するのもアリかも・・。


	■アニメーション
		<Window>
			<Window.Triggers>
				<EventTrigger Event="Loaded">
					<BeginStoryboard>	… WPF では必要。ほかに Pause 等がある。LuminoUIではいらない。このAnimation機能でそんなにがっつり組むことを想定しない。組みたければコード書いてください。
						<Storyboard>
							<DoubleAnimation TargetProperty="Width">	… AnimationTimeline を継承
								<DoubleKeyFrame KeyTime="0" Value="0">
								<DoubleKeyFrame KeyTime="2" Value="1">
								<DoubleKeyFrame KeyTime="5" Value="2">
							</DoubleAnimation>
							<ColorAnimation TargetProperty="Background" Time="1" From="#00000000" To="#FFFF0000">	… From を省略したら現在値から
						</Storyboard>
					</BeginStoryboard>
				<EventTrigger>
			</Window.Triggers>
		</Window>

	■XAML 名前空間 (x:) 言語機能
		http://msdn.microsoft.com/ja-jp/library/ms753327%28v=vs.110%29.aspx
		
	■ 添付プロパティ
		public class AttachedPropertyObject : DependencyObject 
		{ 
		    public static readonly DependencyProperty IsAttachedProperty; 
		  
		    static AttachedPropertyObject() 
		    { 
		        IsAttachedProperty = DependencyProperty.RegisterAttached( 
		            "IsAttached", typeof(bool), typeof(AttachedPropertyObject), new FrameworkPropertyMetadata(false)); 
		    } 
		  
		    public static bool GetIsAttached(DependencyObject target) 
		    { 
		        return (bool)target.GetValue(IsAttachedProperty); 
		    } 
		  
		    public static void SetIsAttached(DependencyObject target, bool value) 
		    { 
		        target.SetValue(IsAttachedProperty, value); 
		    } 
		}

		格納は通常の依存関係プロパティ同様、SetValue() / GetValue() を使う。
		→ プロパティの種別関係なく、DependencyPropery のポインタが、
		システム全体での一意のプロパティキーになる。


	■[2014/11/20]イベントの処理手順
		例）Buttonの Clicked
		①ユーザーのマウスクリックイベントをManagerへ。
		②Manager は GUI内イベントに変換して、アクティブな UIElement(Buttonクラス) に MouseButtonDown を送る。(コントロールのOnEvent() を呼ぶ)
		③

		例) ユーザーコントロールで自前 Clicked (C言語/HSP)
		○UIElement を作る。種別（MY_BUTTOM）を表す定数 (文字列の方が良いかも) をセットして作る。
		①ユーザーのマウスクリックイベントをManagerへ。
		②Manager は GUI内イベントに変換して、アクティブな UIElementに MouseButtonDown を送る。(SendEvent)
		③メッセージループでキャッチ (PeekEvent)
		④Eventが発生元 MY_BUTTON && MouseButtonDown だったら、RaiseEvent ()


		例) ユーザーコントロール Clicked (C#)


*/
//==============================================================================

#include "stdafx.h"
#include "../FileIO/Manager.h"
#include "../FileIO/InFile.h"
#include "../Graphics/Manager.h"
#include "UIElements/UIElement.h"
#include "Manager.h"

namespace LNote
{
namespace Core
{
namespace GUI
{

//==============================================================================
// Manager
//==============================================================================

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
Manager::Manager()
    : mSystemMainWindow			( NULL )
    , mFileManager				( NULL )
    , mGraphicsManager			( NULL )
{  
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
Manager::~Manager()
{  
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void Manager::initialize( const ConfigData& configData )
{
    mSystemMainWindow = configData.MainWindow;
    mFileManager = configData.FileManager;
    mGraphicsManager = configData.GraphicsManager;

	// プロパティ作成
	//Property::trySetProperty(&UIElement::WidthProperty, VariantType_Int, _T("Width"));
	//Property::trySetProperty(&UIElement::HeightProperty, VariantType_Int, _T("Height"));
		
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void Manager::finalize()
{
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void Manager::updateFrame(const Core::Game::GameTime& gameTime)
{
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
bool Manager::doEvent( const System::EventArgs& e )
{
    return false;
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void Manager::updateRenderContents()
{
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void Manager::render()
{
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void Manager::registerType(ClassDefinition* type)
{
	LN_THROW_NotImpl(0);
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
DependencyObject* Manager::newInstance(const lnRefString& typeName)
{
	LN_THROW_NotImpl(0);
	return NULL;
}

} // namespace GUI
} // namespace Core
} // namespace LNote

#if 0
#define CEGUI_STATIC
#include <CEGUI/CEGUI.h>
#include <CEGUI/System.h>
#include <CEGUI/RendererModules/Direct3D9/Renderer.h>
#include <CEGUI/DefaultResourceProvider.h>
#include <CEGUI/ImageManager.h>
#include <CEGUI/Image.h>
#include <CEGUI/Font.h>
#include <CEGUI/Scheme.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/falagard/WidgetLookManager.h>
#include <CEGUI/ScriptModule.h>
#include <CEGUI/XMLParser.h>
#include <CEGUI/GeometryBuffer.h>
#include <CEGUI/GUIContext.h>
#include <CEGUI/RenderTarget.h>
#include <CEGUI/AnimationManager.h>
#ifdef LNOTE_DIRECTX
	#include "../Graphics/Device/DirectX9/DX9GraphicsDevice.h"
#endif
#include "../FileIO/Manager.h"
#include "../FileIO/InFile.h"
#include "../Graphics/Manager.h"
#include "Common.h"
#include "Manager.h"

namespace LNote
{
namespace Core
{
namespace GUI
{

//==============================================================================
// ■ Manager
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Manager::Manager()
        : mSystemMainWindow			( NULL )
        , mFileManager				( NULL )
        , mGraphicsManager			( NULL )
		, mCEGUIRenderer			( NULL )
    {  
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Manager::~Manager()
    {  
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::initialize( const ConfigData& configData )
    {
        mSystemMainWindow = configData.MainWindow;
        mFileManager = configData.FileManager;
        mGraphicsManager = configData.GraphicsManager;


#if 0
		//

		Graphics::DX9GraphicsDevice* dx9Device = dynamic_cast<Graphics::DX9GraphicsDevice*>( mGraphicsManager->getGraphicsDevice() );
		if ( dx9Device )
		{
			CEGUI::Direct3D9Renderer* renderer = &CEGUI::Direct3D9Renderer::create( dx9Device->getIDirect3DDevice9() );

			mCEGUIRenderer = renderer;
		}
		else {
			LN_THROW_NotImpl(0);
		}

		// GUI システムの初期化
		CEGUI::System::create( *mCEGUIRenderer, NULL, NULL, NULL );

		// カレントディレクトリ
		std::string dir;
		mFileManager->getCurrentDirectory().getUTF8String( &dir );
		dir += "/GUIDataFiles/";

		// リソースディレクトリの設定
		CEGUI::DefaultResourceProvider* rp =
			static_cast<CEGUI::DefaultResourceProvider*>( CEGUI::System::getSingleton().getResourceProvider() );
		rp->setResourceGroupDirectory("schemes",		dir + "schemes/");
		rp->setResourceGroupDirectory("imagesets",		dir + "imagesets/");
		rp->setResourceGroupDirectory("fonts",			dir + "fonts/");
		rp->setResourceGroupDirectory("layouts",		dir + "layouts/");
		rp->setResourceGroupDirectory("looknfeels",		dir + "looknfeel/");
		rp->setResourceGroupDirectory("lua_scripts",	dir + "lua_scripts/");
		rp->setResourceGroupDirectory("schemas",		dir + "xml_schemas/");   
		rp->setResourceGroupDirectory("animations",		dir + "animations/"); 

		// リソースグループの設定
		CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
		CEGUI::Font::setDefaultResourceGroup("fonts");
		CEGUI::Scheme::setDefaultResourceGroup("schemes");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
		CEGUI::WindowManager::setDefaultResourceGroup("layouts");
		CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
		CEGUI::AnimationManager::setDefaultResourceGroup("animations");
		
		CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();
		if (parser->isPropertyPresent("SchemaDefaultResourceGroup")) {
			parser->setProperty("SchemaDefaultResourceGroup", "schemas");
		}


#if 1
		CEGUI::GUIContext* guiContext = &CEGUI::System::getSingleton().getDefaultGUIContext();

		// スキーマというのが、WPFでいうところのテーマファイルみたいになっている。
		// 「TaharezLook」という名前のテーマで、このファイルの中に
		// フォントや使用するイメージファイルなどが記述されている。
		CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");

		guiContext->getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

		CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

		// デフォルトウィンドウは、旧LNGUIのDesktop 的なもの。これ自体はレンダリング機能を持たない。とのこと。
		CEGUI::DefaultWindow* d_root = (CEGUI::DefaultWindow*)winMgr.createWindow("DefaultWindow", "Root");

		CEGUI::Font& defaultFont = CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-12.font");

		guiContext->setDefaultFont(&defaultFont);


		// Set the root window as root of our GUI Context
		guiContext->setRootWindow(d_root);






		CEGUI::FrameWindow* wnd = (CEGUI::FrameWindow*)winMgr.createWindow("TaharezLook/FrameWindow", "Demo Window");
		d_root->addChild(wnd);

		wnd->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.25f)));
		 wnd->setSize(CEGUI::USize(cegui_reldim(0.5f), cegui_reldim( 0.5f)));


		 wnd->setMaxSize(CEGUI::USize(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
    wnd->setMinSize(CEGUI::USize(cegui_reldim(0.1f), cegui_reldim( 0.1f)));

	wnd->setText("LNote GUI - CEGUI");
#endif
#endif
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::finalize()
    {
#if 0
		if ( mCEGUIRenderer )
		{
			CEGUI::System::destroy();

			CEGUI::Direct3D9Renderer::destroy( *static_cast<CEGUI::Direct3D9Renderer*>(mCEGUIRenderer) );
			mCEGUIRenderer = NULL;
		}
#endif
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::update( const Core::Game::GameTime& gameTime )
    {
#if 0
		CEGUI::System& gui_system(CEGUI::System::getSingleton());

		gui_system.injectTimePulse( gameTime.getElapsedTime() );
#endif

    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool Manager::doEvent( const System::EventArgs& e )
	{
        return false;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::updateRenderContents()
    {
#if 0
		CEGUI::WindowManager::getSingleton().cleanDeadPool();
#endif
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Manager::render()
    {
#if 0
		CEGUI::System::getSingleton().renderAllGUIContexts();
#endif
    }

} // namespace GUI
} // namespace Core
} // namespace LNote

#endif
