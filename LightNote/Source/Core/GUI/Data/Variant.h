//==============================================================================
// Variant 
//------------------------------------------------------------------------------
///**
//  @file       Variant.h
//  @brief      Variant
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
// Variant
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class Variant
{
public:
	Variant();
	~Variant();

public:
	int getInt() const { LN_THROW_NotImpl(0); return 0; }
	float getFloat() const { LN_THROW_NotImpl(0); return 0; }
	bool getBool() const { LN_THROW_NotImpl(0); return false; }
	const lnRefString& getString() const { LN_THROW_NotImpl(0); return lnRefString(); }
	void* getObjectPtr() const { LN_THROW_NotImpl(0); return NULL; }

	
};

/// Variant ��C�ӂ̃|�C���^�^�� cast
template<class T>
static inline T variant_cast(const Variant& value)
{
	// TODO: �^�`�F�b�N
	//if (value.isA(T::TypeDefine))
	return (T)value.getObjectPtr();
}

/// Variant �� int �^�� cast
template<>
static inline int variant_cast<int>(const Variant& value)
{
	return value.getInt();
}

/// Variant �� float �^�� cast
template<>
static inline float variant_cast<float>(const Variant& value)
{
	return value.getFloat();
}

/// Variant �� bool �^�� cast
template<>
static inline bool variant_cast<bool>(const Variant& value)
{
	return value.getBool();
}

/// Variant �� string �^�� cast
template<>
static inline const lnRefString& variant_cast<const lnRefString&>(const Variant& value)
{
	return value.getString();
}


} // namespace GUI
} // namespace Core
} // namespace LNote
