int WINAPI WinMain(HINSTANCE instance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    int         done = 0;
    WININFO* info = &wininfo;

    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);

    info->hInstance = GetModuleHandle(0);

    //if( MessageBox( 0, "fullscreen?", info->wndclass, MB_YESNO|MB_ICONQUESTION)==IDYES ) info->full++;

    if (!window_init(info))
    {
        window_end(info);
        MessageBox(0, "window_init()!", "error", MB_OK | MB_ICONEXCLAMATION);
        return(0);
    }

    while (!done)
    {
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) done = 1;
            TranslateMessage(&msg);

            DispatchMessage(&msg);
        }

        // Update shader time uniform
        glUniform1f(glGetUniformLocation(shaderProgram, VAR_t), GetAudioPlaybackTime());

        // Draw fullscreen quad
        glRects(-1, -1, 1, 1);

        // Present the frame
        wglSwapLayerBuffers(deviceContext, WGL_SWAP_MAIN_PLANE);

        // Frame cap (approx. 60 FPS)
        Sleep(16);
    }

    window_end(info);

    return(0);
}