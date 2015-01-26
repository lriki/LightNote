#include "TestConfig.h"
#include "TestUtils.h"


//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
Core::FileIO::PathNameW TestUtils::getDirectoryPath(const char* baseSourceFilePath)
{
	Core::FileIO::PathNameW base(baseSourceFilePath);
	return base.getParent();
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
Core::FileIO::PathNameW TestUtils::getFilePath(const char* baseSourceFilePath, const char* fileName)
{
	Core::FileIO::PathNameW base(baseSourceFilePath);
	Core::FileIO::PathNameW path(base.getParent(), fileName);
	return path;
}


//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void TestUtils::SaveScreenCapture(const char* filePath)
{
	Core::Manager* core = (Core::Manager*)LNCore_GetInternalObject();
	core->getGraphicsManager()->getGraphicsDevice()->getDefaultCanvas()->SaveScreenCapture(filePath);
}
