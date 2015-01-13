//==============================================================================
// Collection 
//------------------------------------------------------------------------------
///**
//  @file       Collection.h
//  @brief      Collection
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
// Collection
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//------------------------------------------------------------------------------
/*
	IAddChild を使用せず、あるオブジェクトのプロパティに直接要素を追加したい場合に必要になる。

	<Application>
		<Application.Resources>		←コレとか
			<ResourceDictionary>
				<ResourceDictionary.MergedDictionaries>		←コレとか
					<ResourceDictionary Source="SampleDictionary.xaml" />
				</ResourceDictionary.MergedDictionaries>
			</ResourceDictionary>
		</Application.Resources>
	</Application>

	Collection の持つプロパティは Count だけ。
	要素へのアクセスはインデクサを使用するが、C++ではそんなのないので getItem(int)
*/
//==============================================================================
class Collection
	: public Base::ReferenceObject
{
public:
	Collection();
	~Collection();

public:

};

} // namespace GUI
} // namespace Core
} // namespace LNote
