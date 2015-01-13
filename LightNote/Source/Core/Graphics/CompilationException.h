
#pragma once

namespace LNote
{
namespace Core
{
namespace Graphics
{

/**
	@brief	
*/
class CompilationException
    : public Base::Exception
{
public:
	CompilationException();
	CompilationException(const lnChar* message);

public:
	// override Exception
	virtual LNRESULT getErrorCode() const { return ResultCode_Compilation; }
	virtual Base::Exception* copy() const { return LN_NEW CompilationException(*this); }
};

} // namespace Graphics
} // namespace Core
} // namespace LNote
