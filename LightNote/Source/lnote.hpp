//==============================================================================
//Åy lnote Åz
//-----------------------------------------------------------------------------
/*
*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
// include

#include "Core/InternalCommon.h"

#include "Core/Base/CommandLineArguments.h"
#include "Core/Base/Time.h"
#include "Core/Base/RefPtr.h"

#include "Core/Math/Common.h"
#include "Core/Math/Math.h"
#include "Core/Math/Random.h"

#include "Core/Math/Vector2.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"
#include "Core/Math/Matrix.h"
#include "Core/Math/Quaternion.h"

#include "Core/Animation/RoundTripValue.h"
#include "Core/Animation/ScalarAnimation.h"
#include "Core/Animation/EventAnimation.h"
#include "Core/Animation/AnimationClip.h"
#include "Core/Animation/AnimationController.h"

#include "Core/FileIO/Manager.h"
#include "Core/FileIO/InFile.h"
#include "Core/FileIO/OutFile.h"

#include "Core/System/Environment.h"
#include "Core/System/Common/PerfStopwatch.h"
#include "Core/System/Manager.h"

#include "Core/Input/Manager.h"

#include "Core/Audio/Manager.h"

#include "Core/Physics/Manager.h"
#include "Core/Physics/RigidBody/Plane.h"
#include "Core/Physics/RigidBody/Shapes.h"
#include "Core/Physics/CollisionBody/CollisionBox.h"
#include "Core/Physics/Joint/JointBase.h"
#include "Core/Physics/CharacterObject.h"

#include "Core/Graphics/Interface.h"
#include "Core/Graphics/Manager.h"
#include "Core/Graphics/Common/GraphicsUtil.h"
#include "Core/Graphics/Common/SpriteRenderer.h"
#include "Core/Graphics/Common/Model/ModelManager.h"
#include "Core/Graphics/Common/Model/Model.h"
#include "Core/Graphics/Common/Model/Motion.h"
#include "Core/Graphics/Common/DebugText.h"

#include "Core/Effect/Manager.h"
#include "Core/Effect/Interface.h"

#include "Core/GUI/Manager.h"


#include "Core/Scene/Manager.h"
#include "Core/Scene/ImageFilter/ImageFilter.h"
#include "Core/Scene/ImageFilter/ToneImageFilter.h"
#include "Core/Scene/ImageFilter/TransitionImageFilter.h"
#include "Core/Scene/ImageFilter/BlurImageFilter.h"
#include "Core/Scene/SceneGraph.h"
#include "Core/Scene/Pane.h"
#include "Core/Scene/SceneShader/SceneShaderManager.h"
#include "Core/Scene/SceneShader/SceneShader.h"
#include "Core/Scene/SceneNode/SceneNode.h"
#include "Core/Scene/SceneNode/Grid.h"
#include "Core/Scene/SceneNode/Camera.h"
#include "Core/Scene/SceneNode/Light.h"
#include "Core/Scene/SceneNode/Sprite.h"
#include "Core/Scene/SceneNode/Sprite3D.h"
#include "Core/Scene/SceneNode/BatchSprite.h"
#include "Core/Scene/SceneNode/SpritePath.h"
#include "Core/Scene/SceneNode/Mesh.h"
#include "Core/Scene/SceneNode/Model.h"
#include "Core/Scene/SceneNode/PointParticle.h"
#include "Core/Scene/SceneNode/Tilemap.h"
#include "Core/Scene/SceneNode/Effect.h"

//#include "Core/Game/GameSceneManager.h"
//#include "Core/Game/GameScene.h"
//#include "Core/Game/GameObject.h"
//#include "Core/Game/Component/ModelComponent.h"

#include "Core/Manager.h"

//-------------------------------------------------------------------------
// lib

#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "msacm32.lib" )
#pragma comment( lib, "ws2_32.lib" )
#pragma comment( lib, "OpenGL32.lib" )

// CEGUI
#pragma comment( lib, "imagehlp.lib" )

/*
#if defined(LNOTE_MSVC)
    #include "link.h"
    
    #ifndef LNOTE_NOT_LINK_LIB
        #pragma comment( lib, "winmm.lib" )
        #pragma comment( lib, "msacm32.lib" )
		#pragma comment( lib, "ws2_32.lib" )
        #pragma comment( lib, "OpenGL32.lib" )
		       
        #pragma comment( lib, LNOTE_LIB_NAME )
        #pragma comment( lib, LNOTE_DEPEND_LIB_NAME )
    #endif
#endif
*/

//==============================================================================
//
//==============================================================================