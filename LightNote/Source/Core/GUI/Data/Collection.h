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
	IAddChild ���g�p�����A����I�u�W�F�N�g�̃v���p�e�B�ɒ��ڗv�f��ǉ��������ꍇ�ɕK�v�ɂȂ�B

	<Application>
		<Application.Resources>		���R���Ƃ�
			<ResourceDictionary>
				<ResourceDictionary.MergedDictionaries>		���R���Ƃ�
					<ResourceDictionary Source="SampleDictionary.xaml" />
				</ResourceDictionary.MergedDictionaries>
			</ResourceDictionary>
		</Application.Resources>
	</Application>

	Collection �̎��v���p�e�B�� Count �����B
	�v�f�ւ̃A�N�Z�X�̓C���f�N�T���g�p���邪�AC++�ł͂���Ȃ̂Ȃ��̂� getItem(int)
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
