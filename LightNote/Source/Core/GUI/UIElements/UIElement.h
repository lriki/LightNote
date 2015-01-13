//==============================================================================
// UIElement 
//------------------------------------------------------------------------------
///**
//  @file       UIElement.h
//  @brief      UIElement
//*/
//==============================================================================

#pragma once

#include "../../Geometry/DrawingShapes.h"
#include "../Common.h"
#include "../Data/DependencyObject.h"
#include "../Data/Event.h"
#include "../Data/EventArgs.h"

namespace LNote
{
namespace Core
{
namespace GUI
{

//==============================================================================
// UIElement
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class UIElement
	: public DependencyObject
{
public:
	static Property*	WidthProperty;
	static Property*	HeightProperty;

public:
	UIElement();
	~UIElement();

public:

	/*
	/// マウスのヒットテストを行うか
	IsHitTestVisible

		ActualHeight	この要素の描画時の高さを取得します。
		パブリック プロパティ	ActualWidth
		void                setVisible(bool flag_) { mVisible = flag_; }
	bool                isVisible() const { return mVisible; }

	Opacity
		Visibility

		// この要素上にマウス ポインターが置かれたときに表示されるカーソル
		Cursor

		ClipToBounds	// 子要素クリップ
		AddHandler(RoutedEvent, Delegate)
		AddVisualChild

		// この要素がキーボード フォーカスを受け取ったときに適用される外観
		FocusVisualStyle

		Height	要素の高さの推奨値を取得または設定します。
		パブリック プロパティ	HorizontalAlignment	この要素が、Panel またはアイテム コントロールのような親要素内に構成されるときに適用される水平方向の配置特性を取得または設定します。

		Margin	要素の外側の余白を取得または設定します。
		パブリック プロパティ	MaxHeight	要素の高さの最大値を取得または設定します。
		パブリック プロパティ	MaxWidth	要素の幅の最大値を取得または設定します。
		パブリック プロパティ	MinHeight	要素の高さの最小値を取得または設定します。
		パブリック プロパティ	MinWidth	要素の幅の最小値を取得または設定します。
		パブリック プロパティ	Name	要素の識別名を取得または設定します。 この名前は、XAML プロセッサによる処理時に構築されたマークアップ要素を、その後に分離コード(イベント ハンドラーのコードなど) から参照するために使用できます。

		Parent	この要素の論理上の親要素を取得します。

		Resources	ローカルで定義されたリソース ディクショナリを取得または設定します。


		Style	表示時に、この要素が使用するスタイルを取得または設定します。
		Tag	この要素に関するユーザー定義情報の格納に使用できる任意のオブジェクト値を取得または設定します。
		Triggers	この要素で直接(または子要素で) 設定されているトリガーのコレクションを取得します。
		Width	要素の幅を取得または設定します。

		void captureMouse();
	void releaseMouseCapture();

	BeginAnimation(DependencyProperty, AnimationTimeline, HandoffBehavior)
		HitTest
		RaiseEvent
	*/
	/// この要素が有効であるか
	void setEnable(bool enabled) { mEnabled = enabled; }

	/// この要素が有効であるか
	bool isEnable() const { return mEnabled; }

	/// フォーカスを得ることができるか
	virtual bool isFocusable() const { return false; }

	/// キーボードフォーカス及び論理フォーカスを設定する
	void focus();

	/// この要素に論理フォーカスがあるかどうかを判定する
	bool isFocused() const;

public:
	void beginAnimation(Property* targetProp, AnimationTimeline* timeline);

public:
	virtual void measure(LSize availableSize);
	virtual void arrange(LRect finalRect);
	virtual void updateFrame(float elapsedTime);

public:
	const LSize& getDesiredSize() const { return mDesiredSize; }

	UIElement* getVisualChild(int index) { LN_THROW_NotImpl(0); return NULL; }
	int getVisualChildrenCount() const { LN_THROW_NotImpl(0); return 0; }
		
	// RoutedEvent
	void			registerEvent(Event* ev) { LN_THROW_NotImpl(0); }							// EventManager.RegisterRoutedEvent()
	void			addEventHandler(const Event* ev, int messageId) { LN_THROW_NotImpl(0); }	// AddHandler()
	void			raiseEvent(const Event* ev, EventArgs* args) { LN_THROW_NotImpl(0); }						// RaiseEvent()

protected:
	bool	mEnabled;
	LSize	mDesiredSize;
};

} // namespace GUI
} // namespace Core
} // namespace LNote
