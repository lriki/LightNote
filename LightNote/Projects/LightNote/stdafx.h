
#pragma once

// ���J�ł̋@�\�݂̂��r���h���邽�߂̃t���O
#define LNOTE_PUBLIC_ONLY
#define LNOTE_FOR_200

// DLL �r���h������ LNOTE_BUILD_DLL �̓v���W�F�N�g�ݒ�ɂĒ�`

#include "../../../Source/Core/BuildSetting.h"
#include "../../../Source/Core/InternalCommon.h"
#include "../../../Source/Core/System/Common/PerfStopwatch.h"

#if defined(LNOTE_BUILD_DLL)
	#ifdef LNOTE_DEBUG
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
		//#pragma comment (lib, "../Debug/Deps.mruby.lib")
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
		//#pragma comment (lib, "../Release/Deps.mruby.lib")
	#endif
#endif
