
#include "stdafx.h"
#include "CompilationException.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
CompilationException::CompilationException()
{
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
CompilationException::CompilationException(const lnChar* message)
{
	mErrorMessage = message;
}

} // namespace Graphics
} // namespace Core
} // namespace LNote
