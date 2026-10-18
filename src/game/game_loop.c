#include "game/game_loop.h"

// Main window function that instantiates the window, devices and rendering and hands of contol to the main game loop
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    // Initalize window and renderer
    HWND hwnd = window_init(hInstance, nShowCmd);
    HDC hdc = GetDC(hwnd);
    renderer_init(hwnd);
    input_update();

    // Main game cycle with the handle to the window passed
    while(window_events())
    {
        move(30.0f);
        tilt_camera(0.03f);
        renderer_update(hdc);
        input_update();
    }

    // TODO: Window clean up
    ReleaseDC(hwnd, hdc);
    // End execution 
    return 0;
}