//==============================================================================
// GLPlatformContext 
//------------------------------------------------------------------------------
///**
//  @file       GLPlatformContext.h
//  @brief      GLPlatformContext
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <glfw/include/GLFW/glfw3.h>
//#include <glfw/deps/GL/glext.h>
#if defined(LNOTE_WIN32)
	//#include <gl/glew.h>
	//#include <gl/wglew.h>
#elif defined(LNOTE_ANDROID)
    #include <EGL/egl.h>
    #include <EGL/eglext.h>
    #include <GLES/gl.h>
    #include <GLES/glext.h> // for glGenFramebuffersOES
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#endif
#include "../../Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System { class Manager; }
    
namespace Graphics
{

enum LNGLenum
{
#if defined(LNOTE_WIN32)
    LNGL_WRITE_ONLY = GL_WRITE_ONLY,
#elif defined(LNOTE_ANDROID)
    LNGL_WRITE_ONLY = GL_WRITE_ONLY_OES,
#endif
};

//==============================================================================
// ■ GLPlatformContext
//------------------------------------------------------------------------------
///**
//  @brief     
//*/
//==============================================================================
class GLPlatformContext
{
public:

    struct InitData
    {
        Base::LogFile*      LogFile;
        System::Manager*    SystemManager;
    };

public:
    GLPlatformContext();
    ~GLPlatformContext();

public:

	/// 初期化 (OpenGLのAPIを呼び出すスレッドでしょきかすること)
	//void initialize( const InitData& init_data_ );
	void initialize( System::Manager* systemManager );

	void finalize();

	/// シーン開始
    void beginScene();

	/// シーン終了
    void endScene();

	/// バックバッファをウィンドウに転送
    void present();

	/// デバイスリセット直前 GraphicsDevice から呼ばれる
    void onLostDevice();

	/// デバイスリセット後に GraphicsDevice から呼ばれる
    void onResetDevice();

	void attachCurrentThread();

public:

    static void glClearDepth( GLclampf depth_ );

    static void* glMapBuffer( GLenum target_, GLuint buffer_ );

    static GLboolean glUnmapBuffer( GLenum target_ );

public:

	/// LNDeclType -> GLenum, size(要素数 LN_DECL_FLOAT3 なら 3), normalized(正規化の有無)
    static void convertElementLNToGL( LNDeclType ln_type_, GLenum* gl_type_, GLint* size_, GLboolean* normalized_ );

private:

    Base::LogFile*      mLogFile;
    System::Manager*    mSystemManager;

#if defined(LNOTE_WIN32)
    HWND    mHWND;
    HDC     mHDC;
    HGLRC   mGLRC;
#elif defined(LNOTE_GLES)
    EGLDisplay  mEGLDisplay;
    EGLConfig   mEGLConfig;
    EGLContext  mEGLContext;
    EGLSurface  mEGLSurface;
    
    static PFNGLMAPBUFFEROESPROC   m_glMapBufferOES;
    static PFNGLUNMAPBUFFEROESPROC m_glUnmapBufferOES;
    
#endif

};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================