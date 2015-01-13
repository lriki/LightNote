//==============================================================================
// UIElementTemplate 
//------------------------------------------------------------------------------
///**
//  @file       UIElementTemplate.h
//  @brief      UIElementTemplate
//*/
//==============================================================================

#pragma once

namespace LNote
{
namespace Core
{
namespace GUI
{

//==============================================================================
// UIElementTemplate
//------------------------------------------------------------------------------
///**
//  @brief      
//	@note
//				WPF で言うところの FrameworkTemplate や ControlTemplate。
//*/
//------------------------------------------------------------------------------
/*
	■UIElementTemplate(ControlTemplate) と、UIElementFactory(FrameworkElementFactory) の違い
		両方とも SetValue や Style、Trigger を設定することができ
		なんとなく似ているが、使用目的はぜんぜん違う。
		・UIElementFactory
			単純に1つの UIElement を作る。難しい制約とか無し。
			子要素を追加することで、ツリーそのものを表せる。
			インスタンス化したものはそのまま VisualツリーにもLogicalツリーにも使える。
		・UIElementTemplate
			1つの論理的なコントロールの作成に必要な情報の集合。例えば Button は Chrome や TextBlock から成り立っているが、
			使うときはそんなの気にしないで、ひとつの Button として Visual または Logical ツリーに繋ぐことができる。
			UIElementTemplate を階層構造にすることはできない。
			【対応する Presenter が必ず必要になる。】
	
	■UIElementFactoryからのインスタンス化
		UIElementFactory::createUIElement() で自身を作成。
		子要素 Factor があれば、それぞれの createUIElement() を呼び、
		作成された UIElement を階層構造に結合して返す。
		UIElement を作成するときは、名前によってManager から UIElementTemplate を取得し、
		UIElementTemplate.createUIElement() を呼び出す。
		1つの create 処理で UIElementTemplate と UIElementFactory が互い違いに呼び出されてかなり複雑になるので注意。
		【Manager にコントロールとして直接登録されるのは UIElementTemplate である】
*/
//==============================================================================
class UIElementTemplate
	: public Base::ReferenceObject
{
public:
	UIElementTemplate();
	~UIElementTemplate();

public:

};

} // namespace GUI
} // namespace Core
} // namespace LNote
