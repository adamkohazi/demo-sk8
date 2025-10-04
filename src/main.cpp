// Based on Demo-Framework-4k by Inigo Quilez (iq)

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <GL/gl.h>
#include <math.h>
#include "main.h"
#include "glext.h"
#include "shaders/fragmentShader.inl"
#include "../assets/music/output/4klang.h"
#include "audio.h"

#ifdef DEBUG
    #include <windowsx.h>
    #include <mmsystem.h>
    #include <string.h>
    #include <stdio.h>
    #include <cassert> 
#endif

// OpenGL definitions
#define glGenFramebuffers ((PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffers"))
#define glFramebufferTexture2D ((PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2D"))
#define glActiveTexture ((PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture"))
#define glBindFramebuffer ((PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer"))
#define glDrawBuffers ((PFNGLDRAWBUFFERSPROC)wglGetProcAddress("glDrawBuffers"))
#define glBindFragDataLocation ((PFNGLBINDFRAGDATALOCATIONPROC)wglGetProcAddress("glBindFragDataLocation"))
#define glUniform1i ((PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i"))
#define glUniform1f ((PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f"))
#define glUniform3f ((PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f"))
#define glUniform1fv ((PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv"))
#define glUniform3fv ((PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv"))
#define glGetUniformLocation ((PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation"))
#define glUseProgram ((PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram"))
#define glCreateShader ((PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader"))
#define glCreateShaderProgramv ((PFNGLCREATESHADERPROGRAMVPROC)wglGetProcAddress("glCreateShaderProgramv"))
#define glShaderSource ((PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource"))
#define glCompileShader ((PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader"))
#define glGenerateMipmap ((PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap"))
#define glGenerateTextureMipmap ((PFNGLGENERATETEXTUREMIPMAPPROC)wglGetProcAddress("glGenerateTextureMipmap"))
#define glGetShaderiv ((PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv"))
#define glGetShaderInfoLog ((PFNGLGETSHADERINFOLOGPROC) wglGetProcAddress("glGetShaderInfoLog"))
#define glDeleteShader ((PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader"))
#define glBindAttribLocation ((PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation"))
#define glCreateProgram ((PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram"))
#define glAttachShader ((PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader"))
#define glLinkProgram ((PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram"))
#define glGetProgramiv ((PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv"))
#define glGetProgramInfoLog ((PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog"))
#define glUseProgramStages ((PFNGLUSEPROGRAMSTAGESPROC)wglGetProcAddress("glUseProgramStages"))
#define glGenProgramPipelines ((PFNGLGENPROGRAMPIPELINESPROC)wglGetProcAddress("glGenProgramPipelines"))
#define glBindProgramPipeline ((PFNGLBINDPROGRAMPIPELINEPROC)wglGetProcAddress("glBindProgramPipeline"))


#ifdef DEBUG
static bool isPaused = false;
#endif


// Floating point support flag for MSVC
#ifdef __cplusplus
    extern "C" {int  _fltused = 0;}
#else
    int  _fltused = 0;
#endif

// Pixel format descriptor for OpenGL context creation
static const PIXELFORMATDESCRIPTOR pixelFormatDesc = {
    .nSize = sizeof(PIXELFORMATDESCRIPTOR),
    .nVersion = 1,
    .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    .iPixelType = PFD_TYPE_RGBA,
    .cColorBits = 32,

#ifdef DEBUG
    .cAlphaBits = 8,
#else
    .cAlphaBits = 0,
#endif

    .cDepthBits = 32,
    .iLayerType = PFD_MAIN_PLANE
    // All other fields are zero-initialized by default
    };

// Fullscreen display settings
static DEVMODE fullscreenSettings = {
    .dmSize = sizeof(DEVMODE),
    .dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT,
    .dmBitsPerPel = 32,
    .dmPelsWidth = XRES,
    .dmPelsHeight = YRES
    // All other fields are zero-initialized by default
};

#ifdef DEBUG
// Window Procedure: Handles messages sent to the window (keyboard, resize, close, etc.)
static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_SYSCOMMAND:
            // Prevent screensaver or monitor sleep while active
            if (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER)
                return 0;
            break;

        case WM_CLOSE:
        case WM_DESTROY:
            // Trigger app shutdown on window close
            PostQuitMessage(0);
            return 0;

        case WM_KEYDOWN:
            switch (wParam) {
                case VK_ESCAPE:
                    // Exit the app when ESC is pressed
                    PostQuitMessage(0);
                    return 0;

                case VK_SPACE:
                    // Toggle playback pause/resume when SPACE is pressed
                    isPaused = !isPaused;
                    if (isPaused)
                        pauseAudio();
                    else
                        resumeAudio();
                    return 0;

                case VK_LEFT:
                    // Step back 1s
                    stepAudio(-1.0);
                    return 0;

                case VK_RIGHT:
                    // Step forward 1s
                    stepAudio(1.0);
                    return 0;
            }
            break;

        case WM_SIZE: {
            // Update OpenGL viewport when the window is resized
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            glViewport(0, 0, width, height);
            return 0;
        }
    }

    // Let Windows handle any unprocessed messages
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
#endif


// Program entry point
#ifdef DEBUG
// Default entry point
int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow
) {
#else
// Crinkler compatible, minimal entry point
void entrypoint(void) {
#endif

    // Display presets
#ifdef DEBUG
    // Set the DPI awareness
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
#else
    // Switch to fullscreen
    ChangeDisplaySettings(&fullscreenSettings, CDS_FULLSCREEN);
    ShowCursor(FALSE);
#endif

    // Window registration
#ifdef DEBUG
    // Only register in debug
    WNDCLASS windowClass = {
        .style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = WindowProcedure,
        .hInstance = GetModuleHandle(0), // Retrieve instance handle
        .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
        .lpszClassName = "sk8" // window class name
    };
    assert(RegisterClass(&windowClass) && "Failed to register window class");
#endif

    // Create window
#ifdef DEBUG
    // Create a standard window
    HWND windowHandle = CreateWindow(
        windowClass.lpszClassName,
        "sk8 (debug)",
        WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        XRES, YRES,
        nullptr, nullptr,
        windowClass.hInstance,
        nullptr
    );

    // Check for errors
    assert(windowHandle && "Failed to create window");
#else
    // Create a borderless fullscreen window
    HWND windowHandle = CreateWindow(
        "static",
        nullptr,
        WS_POPUP | WS_VISIBLE,
        0, 0,
        XRES, YRES,
        nullptr, nullptr,
        nullptr,
        nullptr
    );
#endif

    // Get device context handle
    HDC deviceContext = GetDC(windowHandle);

#ifdef DEBUG
    // Check for errors
    assert(deviceContext && "Failed to get device context");
#endif

    // Set pixel format
#ifdef DEBUG
    // Check for errors
    int pixelFormat = ChoosePixelFormat(deviceContext, &pixelFormatDesc);
    assert(pixelFormat && "Failed to choose a pixel format");
    assert(SetPixelFormat(deviceContext, pixelFormat, &pixelFormatDesc) && "Failed to set pixel format");
#else
    // Pray for no errors
    SetPixelFormat(deviceContext, ChoosePixelFormat(deviceContext, &pixelFormatDesc), &pixelFormatDesc);
#endif

    // Create OpenGL rendering context
#ifdef DEBUG
    // Check for errors
    HGLRC glRenderContext = wglCreateContext(deviceContext);
    assert(glRenderContext && "Failed to create OpenGL rendering context");
    assert(wglMakeCurrent(deviceContext, glRenderContext) && "Failed to activate OpenGL rendering context");
#else
    // Pray for no errors
    wglMakeCurrent(deviceContext, wglCreateContext(deviceContext));
#endif

#ifdef DEBUG
    // Check if OpenGL functions are available.
    assert(glCreateShaderProgramv && "Missing glCreateShaderProgramv");
    assert(glUseProgram && "Missing glUseProgram");
    assert(glGetProgramiv && "Missing glGetProgramiv");
    assert(glGetProgramInfoLog && "Missing glGetProgramInfoLog");
#endif

    // Compile GLSL fragment shader
    const unsigned int shaderProgram = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fragmentShader_frag);

#ifdef DEBUG
    // Check if the shader program linked successfully.
    GLint result;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    if(!result) {
        char error[1024];
        glGetProgramInfoLog(shaderProgram, 1024, nullptr, (char*)error);
        MessageBox(windowHandle, error, "Error", MB_OK);
        return 0;
    }
#endif

    // Activate fragment shader
    glUseProgram(shaderProgram);

    // Start audio rendering thread
    initAudio();

    // Main loop
    MSG message;
    do {
        
        // Message handling
#ifdef DEBUG
        int done = 0;
        while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
            if (message.message == WM_QUIT)
                done = 1;
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        if (done)
            break;
#else
        // Non-blocking message polling
        PeekMessage(&message, windowHandle, 0, 0, PM_REMOVE);
#endif

        // Always time uniform
        glUniform1f(glGetUniformLocation(shaderProgram, VAR_t), GetAudioPlaybackTime());

        // Draw fullscreen quad
        glRects(-1, -1, 1, 1);

        // Present the frame
#ifdef DEBUG
        SwapBuffers(deviceContext); // Cleaner
#else
        wglSwapLayerBuffers(deviceContext, WGL_SWAP_MAIN_PLANE); // Smaller
#endif
        
        // Frame cap (approx. 60 FPS)
        Sleep(16);

    } while ((message.message != WM_KEYDOWN || message.wParam != VK_ESCAPE) && GetAudioPlaybackTime() < float(MAX_SAMPLES) / SAMPLE_RATE);


#ifdef DEBUG
    // If a valid OpenGL rendering context exists, release it
    if (glRenderContext) {
        wglMakeCurrent(0, 0); // Detach the rendering context
        wglDeleteContext(glRenderContext); // Delete the OpenGL context
    }

    // Release the device context if it exists
    if (deviceContext)
        ReleaseDC(windowHandle, deviceContext);

    // Destroy the window if it was created
    if (windowHandle)
        DestroyWindow(windowHandle);

    // Unregister the window class
    UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

    return 0;
#else

    // Return from fullscreen
    ChangeDisplaySettings(nullptr, 0);
    ShowCursor(TRUE);

    ExitProcess(0);
#endif
}