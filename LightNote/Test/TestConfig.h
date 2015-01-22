#include <gtest/gtest.h>
#include "../Source/lnote.hpp"
#include "../Source/C_API/lnote.h"

#ifdef _DEBUG
#pragma comment (lib, "../Debug/Deps.Effekseer.lib")
#pragma comment (lib, "../Debug/Deps.freetype.lib")
#pragma comment (lib, "../Debug/Deps.glfw.lib")
#pragma comment (lib, "../Debug/Deps.libogg.lib")
#pragma comment (lib, "../Debug/Deps.libpng.lib")
#pragma comment (lib, "../Debug/Deps.libvorbis.lib")
#pragma comment (lib, "../Debug/Deps.pcre.lib")
#pragma comment (lib, "../Debug/Deps.tinyxml2.lib")
#pragma comment (lib, "../Debug/Deps.zlib.lib")
#pragma comment (lib, "../Debug/Deps.bullet.lib")
//#pragma comment (lib, "../Debug/Deps.CEGUI.lib")
#pragma comment (lib, "../Debug/Deps.Modules.lib")
#pragma comment (lib, "../Debug/LNote.Static.lib")
#pragma comment (lib, "../../Source/Lumino/Lumino.Math/lib/LuminoMath-msvc120-static_d.lib")
#else
#pragma comment (lib, "../Release/Deps.Effekseer.lib")
#pragma comment (lib, "../Release/Deps.freetype.lib")
#pragma comment (lib, "../Release/Deps.glfw.lib")
#pragma comment (lib, "../Release/Deps.libogg.lib")
#pragma comment (lib, "../Release/Deps.libpng.lib")
#pragma comment (lib, "../Release/Deps.libvorbis.lib")
#pragma comment (lib, "../Release/Deps.pcre.lib")
#pragma comment (lib, "../Release/Deps.tinyxml2.lib")
#pragma comment (lib, "../Release/Deps.zlib.lib")
#pragma comment (lib, "../Release/Deps.bullet.lib")
//#pragma comment (lib, "../Release/Deps.CEGUI.lib")
#pragma comment (lib, "../Release/Deps.Modules.lib")
#pragma comment (lib, "../Release/LNote.Static.lib")
#endif

using namespace LNote;
using namespace LNote::Core;

#include "TestUtils.h"

