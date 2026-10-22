#include "game/game_loop.h"

// Main window function that instantiates the window, devices and rendering and hands of contol to the main game loop
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    // Initalize window and renderer
    HWND hwnd = window_init(hInstance, nShowCmd);
    HDC hdc = GetDC(hwnd);
    renderer_init(hwnd);
    world_init();
    input_update();
    vec2 last_chunk_position = get_camera_chunk_position();
    vec3 last_camera_position = get_camera_position();

    // Main game cycle with the handle to the window passed
    while(window_events())
    {
        if ((last_chunk_position.x != get_camera_chunk_position().x) || (last_chunk_position.y != get_camera_chunk_position().y))
        {
            generate_chunks(vec2_sub(get_camera_chunk_position(), last_chunk_position));
            printf("\nChanged camera. From : (%f,%f)  to : (%f,%f)\n", last_camera_position.x, last_camera_position.z, get_camera_position().x, get_camera_position().z);
            last_camera_position = get_camera_position();
            printf("Changed chunks. From : (%f,%f)  to : (%f,%f)\n", last_chunk_position.x, last_chunk_position.y, get_camera_chunk_position().x, get_camera_chunk_position().y);
            last_chunk_position = get_camera_chunk_position();
        }
        
        move(50.0f);
        tilt_camera(0.03f);
        renderer_update(hdc);
        input_update();
    }

    // TODO: Window clean up
    ReleaseDC(hwnd, hdc);
    // End execution 
    return 0;
}