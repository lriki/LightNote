//==============================================================================
// Manager 
//------------------------------------------------------------------------------
/*
	���v���c
		�E���[�U�[�R���g���[��������悤�ɂ�����
			��	�N���X�I�Ȍp���ł͂Ȃ��A�R���|�[�l���g�u���݂����Ȋ����ŁA
				�D���Ȃ����D���ȃv���p�e�B���p��������悤�Ȃ̂������B
				�ŁA�����Unity�v���n�u�݂����Ɏ���Ă����āA���ł��g����悤�ɂ���B
				��	�e���v���[�g�B
					�e���v���[�g����C���X�^���X������͓̂��R�Ƃ��āA
					�C���X�^���X���e���v���[�g�����邱�Ƃ��ł���B
					��	�e���v���[�g������̂́A�v���p�e�B�ƃC�x���g�B
						
		�E�C�x���g�n���h���̓o�^
			delegate �͎g������_���B����o�C���_���ɂ����Ȃ�B
			��	�u�C�x���g�v���p�e�B�v�Ƃ�����ނ̃v���p�e�B���K�v�B(WPF��������)
			��	�C�x���g�v���p�e�B�ւ� static �R�[���o�b�N�֐��o�^
			��	�O���[�o���ɂ�1�~�����B�ǂ�ȃC�x���g�����������Ƃ��ł��K���ʂ�Ƃ���BHSP�p�B

		�E�A�j���[�V����

		�E�f�[�^�o�C���f�B���O
			�u����Ύg���邩���v�~�܂�ȋC������B
			ViewModel �������AC/C++ �ɂ��Ă�����o�C���_�̍����ɂ��Ă��A
			�g�����ɂ���Ȃ�̒m���ƕ��S���|���Ȃ��Ƃ����Ȃ��B
			�g���ȏ� MVVM �Ɏ����p�^�[���ɂȂ�킯�����ǁA���΋����I�ɑa���������߂��� MVVM�A
			�Q�[���͋Ɩ��A�v���Ƃ͔�r�ɂȂ�Ȃ��قǏ_������߂���Ƃ��낾���A
			�Ƃ肠����������ł��������B

		�EXML���C�A�E�g
			�EStaticResource/ResourceDictionary/MergedDictionary

		�E���[�U�[�����̋C�ɂȂ�΃��{���Ƃ��h�b�L���O�E�B���h�E�Ƃ������ł���_��Ȋ��

		�E���z��
			�R�[���o�b�N�K�{�B����o�C���_�l������Ƃ������ʓ|�B
			�N���b�v�̈�O�͕`�悵�Ȃ����炢�����ł��Ȃ������B

	��How to
		�E�e�[�}�ύX
			ResourceDictionary �X�V��A�S�Ă� Resource �Ƀo�C���h���Ă���v���p�e�B���X�V

		�E�A�C�R���t���c���[�r���[
			�@ Image �� FrameworkElementFactory �����A��c�� UserControl(_ctrl �Ƃ����O�t���� ElementName) �� Icon �v���p�e�B(����)�Ƀo�C���h����B
			�A TextBlock �� FrameworkElementFactory �����A(��c�� UserControl �� Text?Content? �v���p�e�B(����))�Ƀo�C���h����B
			�B StackPanel �� FrameworkElementFactory �����A�@�ƇA�� AddChild()
			�C TreeViewItem �� ControlTemplate �����AIcon ���[�U�[�v���p�e�B���A�^�b�`���AVisualTree �v���p�e�B�� �B ���Z�b�g����B
			�D TreeViewItem �����ATemplate �v���p�e�B�� �C ���Z�b�g����B
			http://www.oborodukiyo.info/WPF/VS2008/WPF-DifferenceBetweenXAMLAndCode.aspx
			FrameworkElementFactory �͍쐬���� UIElement �̌^�ƁA���蓖�Ă�v���p�e�B�ƒl�����X�g�Ŏ��B
		
			



	�����Y�^
		�E�|�b�v�A�b�v���j���[�͕��ʂ� VisualTree �ł͂Ȃ��B
			����Ȋ����ɂȂ��Ă��B
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

		�EIAddChild �C���^�[�t�F�[�X
			http://blogs.wankuma.com/shuujin/archive/2007/05/20/77391.aspx


	��How to (���p���x��)

		�EWW �̃E�B���h�E�O��
			�X�^�C����`�B�N���[�Y�{�^���Ȃ��B
			�w�i��RGSSWindow�^�C���X�^�C���A�g��Line�ŕ`��B
			�������� WWWindow �Ƃ��e���v���[�g����Ă����B

		�EWW �� ���C�����j���[
			���[�g�� Grid �ŕ����BRowSpan �g���Ȃ�AGrid ���l�X�g����Ȃ�B
			�������A�E�B���h�E�V�X�e�������߂Ȃ��悤��GUI�ł́A
			Canvas ���g������΍��W�w��̕��������I�ɂł��ėǂ���������Ȃ��B
			�E�㕔���j���[
				2�s��Horizontal���X�g�B(Window�AListBox�AUniformGrid�AItemTemplate)
				Window�ɍ��ږ��̃R���N�V�����̃v���p�e�B���������AListBox�ƃo�C���h�B
				�u����{�^���v�Ɓu�N���b�N�v�����ѕt����K�v������B
				�� �O���[�o���Ȑݒ�ŁA����{�^�� = �X�y�[�X�L�[���숵������悤�Ȏd�g��
			�E�������X�e�[�^�X
				�E�A�N�^�[���Ƃ̃X�e�[�^�X��\�����郆�[�U�[�R���g���[��
					(UserControl�AImage�ATextBlock)
					���ƁA��Ԉُ�A�C�R�����X�g�Ƃ��B
				�E����UserControl �̃��X�g
			�E�E���C���t�H���[�V����
				(Window�AGrid�ATextBlock)
				Window �� �v���C���ԂƂ����݈ʒu�Ƃ��̃v���p�e�B���������A
				�e TextBlock �͂���Ƀo�C���h����B

		�E�A�C�e��
			�E�w�i
				���C�����j���[���������K���X������ꂽ�݂����Ɉڂ��Ă�B
				�� 
			�E�J�e�S���E�B���h�E
			�E�A�C�e�����X�g�E�B���h�E
			�E�����E�B���h�E
			�E�A�C�e���g�p�ΏۑI���E�B���h�E
				�P�̑I���E�S�̑I��
				�� ItemsControl �ɂ͑I����Ԃ��Œ肷��@�\���~�����B(A�͏�ɑI����Ԃɂ���A���A�A�C�e�����Ƃ�)

		�EWW ���j���[����
			�L�[�w���v�E�B���h�E
			�t�H�[�J�X�ւ̃J�[�\���ړ�
			
		�EWW �p�Z���j���[
			�E�A�N�^�[�X�e�[�^�X�E�B���h�E
				[Image, TextBlock]
				�A�N�^�[�̕��s�O���t�B�b�N���E�B���h�E�̊O�ɂ͂ݏo�������B
				�� �N���b�s���O���Ȃ��B�E�B���h�E�g�͎�O�ɕ`�悵�Ȃ��Ă��������B
			�E������@�E�B���h�E
				�e�L�X�g���X�g
			�E���蓖�ă��X�g�E�B���h�E
				[ListBox, Image(�p���p���A�j��), TextBlock, Template]
				�L�[�̃A�C�R�����A�j���[�V�����������B
				ListBox �A�C�e���ʂ̍����ݒ�B(�������Ȃ����ǃZ�p���[�^�������ق������������H)
			�E�X�L�����X�g�A�N�^�[�E�B���h�E
				�V���[�g�J�b�g�X�L���̐ݒ�Ŏg���BL2 R2 �{�^���ɃZ�b�g����Ƃ��̂ݕ\�������B
				�A�C�e���̃J�e�S���E�B���h�E���ۂ��Ȃ�Ǝv�����Ǔ��ɓ���Ȃ��B
			�E�X�L�����X�g�E�B���h�E
				���ʂɃA�C�R���ƃe�L�X�g�̃��X�g�ŁB
			�E�X�L�������E�B���h�E (�t���[�e�B���O)
				��{����������OK�B�K�v�ɉ����đ����Ȃǂ����A��{�e�L�X�g�B
				�t�H�[�J�X�ʒu�ɂ���āA��ʏ�̌��₷���ʒu�Ɏ����ړ�����悤�ȋ@�\������΁E�E�E�i���O�ł�����j
			�E�X�L���^�[�Q�b�g�E�B���h�E
				�A�C�e���̂Ɠ���
				
	��memo

		�E�l�X�g���ꂽUserControl
			�EWinForm ���C�N
				Loaded �܂��� Update �C�x���g�ŁA�q�����B
				���[�U�[�f�[�^���[�����������̂̓R�[�h���ō�������������I�����B




		�E�����ƌ���o�C���_
			���������f�[�^�o�C���h���K�v�Ȃ̂́A�f�[�^�\�[�X�����鎞�B
			�R���g���[�����C�u�����Ƃ��Ŋg���R���g���[�������Ƃ���
			(�����I�ɂ͎g����������Ȃ�����)���[�U�[�f�[�^�̌`������������
			�f�[�^�o�C���h�͎g��Ȃ��͂��B��������C�x���g�n���h����p�ɂɍ�肻���B
			������ۂ̃A�v���Ŏg�����̂� StatusWindow �݂����ɁA�R�����ł��� Window �ɂ�������
			���̃A�v���Ŏg�p����f�[�^�p�ɓ������������̂����B
			�������͋t�Ƀf�[�^�o�C���h���g�������ȋC�����邪�E�E�B

		�e���v���[�g�͖������������H
			�R���N�V�����̃o�C���h������c
			�E

		�EBinding �͂����܂Łu������Ƃ������[�e�B���e�B�v���x�ɂƂǂ߂Ă������������������B
			ViewModel �g��Ȃ����_�ŁAItemsSource ���̃R���N�V�����o�C���f�B���O�������s�\�ɂȂ�B
			�uElementName ���g�������[�J���ȑ��̃R���g���[���̃v���p�e�B�����Q�Ɖ\�v�Ȋ����ŁB


		�EListBoxItem�ATreeViewItem
			���A�q�R���g���[�����z�X�g����

		�EListBox �� ComboBox �� Separator
			PrepareContainer �Ƃ��� intrenal ���\�b�h�ŁA�e�R���e�i (ListBoxItem ��) �� Enabled �� false �ɂ��Ă���B
			���X�g�ɂ͗v�f�Ƃ��ē����Ă���B
			http://reflector.webtropy.com/default.aspx/Dotnetfx_Win7_3@5@1/Dotnetfx_Win7_3@5@1/3@5@1/DEVDIV/depot/DevDiv/releases/Orcas/NetFXw7/wpf/src/Framework/System/Windows/Controls/Separator@cs/1/Separator@cs

	���ǂ��܂ł�GUI���W���[�����ŃN���X�����邩�H
		UI�v�f�̐�����傫�����߂�Ƃ���܂ł� �e��Presenter �ł����Ǝv���B
		��̓I�ɂ� Control�AItemsControl�AShape�AAdorner�APane�AContentPresenter �Ƃ��B

		UIElement
			ContentPresenter
			Control	�c�q�v�f�����v�f�̊��B�܂��ATAB�L�[�ɂ��t�H�[�J�X�ړ����s���P�ʁB
				ContentControl	�c�q�v�f���ЂƂ��v�f�̊��B
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
					Window			�c �ŏ����A�ő剻�����B
				ItemsControl	�c�����̎q�v�f�����v�f�̊��B
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

	���e����̃��[�U�[�R���g���[���̍��� �c NumUpDown ������Ă݂�
		�EC#
			�@UserControl �N���X���p������ NumUpDown �N���X���쐬
			�AGrid, TextBox, Button ���q�R���g���[���Ƃ��ēo�^
			�BTextBox �� �e�L�X�g�`��̈�̉E�����󂯂� (Template �� "ContentElement" �Ɩ��O�̕t���� ScrollViewer �� Margin ��ς��� http://msdn.microsoft.com/ja-jp/library/cc645061%28v=vs.95%29.aspx)
			�Cstatic �� ValueProperty �쐬�B�R���X�g���N�^�œo�^
			�Dstatic �� ValueChanged �C�x���g�쐬�B�R���X�g���N�^�œo�^
			�FAddHandler() �ŁA2�� Button �� Click �C�x���g���󂯎��悤�ɂ���B
				Create() {
					this.AddHandler(Button.Click, 1);
					this.AddHandler(Button.Click, 2);
					this.AddHandler(TextBox.TextChanged, 3);
				}
				OnEvent(ev) {
					if (ev.Event == UIElement.PropertyChanged && ev.PropName == "Value")
						RaiseEvent(ValueChanged, new EventArgs());
					if (ev.Event == Button.Click && ev.MessageId == 1)
						// Up�N���b�N
						// Text �X�V
					if (ev.Event == Button.Click && ev.MessageId == 2)
						// Down �N���b�N
						// Text �X�V
					if (ev.Event == TextBox.TextChanged && ev.MessageId == 3)
						SetValue(ValueProperty, ((TextBox)ev.Sender).Text.ToInt()); �� PropertyChanged
				}

		�EC�ō���ă��[�U�[�R���g���[���Ƃ��Ĕz�z

	�����[�U�[�R���g���[�����C�u����
		�r�n�C���h�R�[�h��C/C++����Ȃ��āA�������X�N���v�g������Hmruby�Ƃ��B
		��������Δz�z���₷���Ȃ�C�����邪�E�E�E�B

		�C��t����_������Bmruby �� C �������C���ŗ��p���悤�Ƃ���Ə����ʓ|�B
		http://blog.64p.org/entry/2012/09/12/065743

	��VisualStateManager


	��internal class StyleHelper
		http://referencesource.microsoft.com/#PresentationFramework/src/Framework/System/Windows/StyleHelper.cs
		Style �Ƃ����K�p������ FrameworkElement ��n���ƁA
		FrameworkElement ���X�V���Ă���� static �֐��̎����B

	���C���X�^���X�̍쐬
		�t�@�N�g���֐���ʂ��ׂ��BC++�Ŏ��������R���g���[���N���X��
		�t�@�N�g���֐��Ɏw�肳�ꂽ���O�ō��Ȃ���΂Ȃ�Ȃ��B
		C++�ŉ����N���X�������A����̐����֐��� Manager �ɖ��O�ƂƂ��ɓo�^����B
		���ڂ� new �͂����Ă��������A�����o�H��2����̂��C�ɂȂ�B�����ē��ꂷ��̂��A�������E�E�B


	���A�j���[�V����
		<Window>
			<Window.Triggers>
				<EventTrigger Event="Loaded">
					<BeginStoryboard>	�c WPF �ł͕K�v�B�ق��� Pause ��������BLuminoUI�ł͂���Ȃ��B����Animation�@�\�ł���Ȃɂ�����g�ނ��Ƃ�z�肵�Ȃ��B�g�݂�����΃R�[�h�����Ă��������B
						<Storyboard>
							<DoubleAnimation TargetProperty="Width">	�c AnimationTimeline ���p��
								<DoubleKeyFrame KeyTime="0" Value="0">
								<DoubleKeyFrame KeyTime="2" Value="1">
								<DoubleKeyFrame KeyTime="5" Value="2">
							</DoubleAnimation>
							<ColorAnimation TargetProperty="Background" Time="1" From="#00000000" To="#FFFF0000">	�c From ���ȗ������猻�ݒl����
						</Storyboard>
					</BeginStoryboard>
				<EventTrigger>
			</Window.Triggers>
		</Window>

	��XAML ���O��� (x:) ����@�\
		http://msdn.microsoft.com/ja-jp/library/ms753327%28v=vs.110%29.aspx
		
	�� �Y�t�v���p�e�B
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

		�i�[�͒ʏ�̈ˑ��֌W�v���p�e�B���l�ASetValue() / GetValue() ���g���B
		�� �v���p�e�B�̎�ʊ֌W�Ȃ��ADependencyPropery �̃|�C���^���A
		�V�X�e���S�̂ł̈�ӂ̃v���p�e�B�L�[�ɂȂ�B


	��[2014/11/20]�C�x���g�̏����菇
		��jButton�� Clicked
		�@���[�U�[�̃}�E�X�N���b�N�C�x���g��Manager�ցB
		�AManager �� GUI���C�x���g�ɕϊ����āA�A�N�e�B�u�� UIElement(Button�N���X) �� MouseButtonDown �𑗂�B(�R���g���[����OnEvent() ���Ă�)
		�B

		��) ���[�U�[�R���g���[���Ŏ��O Clicked (C����/HSP)
		��UIElement �����B��ʁiMY_BUTTOM�j��\���萔 (������̕����ǂ�����) ���Z�b�g���č��B
		�@���[�U�[�̃}�E�X�N���b�N�C�x���g��Manager�ցB
		�AManager �� GUI���C�x���g�ɕϊ����āA�A�N�e�B�u�� UIElement�� MouseButtonDown �𑗂�B(SendEvent)
		�B���b�Z�[�W���[�v�ŃL���b�` (PeekEvent)
		�CEvent�������� MY_BUTTON && MouseButtonDown ��������ARaiseEvent ()


		��) ���[�U�[�R���g���[�� Clicked (C#)


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

	// �v���p�e�B�쐬
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
// �� Manager
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

		// GUI �V�X�e���̏�����
		CEGUI::System::create( *mCEGUIRenderer, NULL, NULL, NULL );

		// �J�����g�f�B���N�g��
		std::string dir;
		mFileManager->getCurrentDirectory().getUTF8String( &dir );
		dir += "/GUIDataFiles/";

		// ���\�[�X�f�B���N�g���̐ݒ�
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

		// ���\�[�X�O���[�v�̐ݒ�
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

		// �X�L�[�}�Ƃ����̂��AWPF�ł����Ƃ���̃e�[�}�t�@�C���݂����ɂȂ��Ă���B
		// �uTaharezLook�v�Ƃ������O�̃e�[�}�ŁA���̃t�@�C���̒���
		// �t�H���g��g�p����C���[�W�t�@�C���Ȃǂ��L�q����Ă���B
		CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");

		guiContext->getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

		CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

		// �f�t�H���g�E�B���h�E�́A��LNGUI��Desktop �I�Ȃ��́B���ꎩ�̂̓����_�����O�@�\�������Ȃ��B�Ƃ̂��ƁB
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
