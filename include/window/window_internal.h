#ifndef WINDOW_INTERNAL
#define WINDOW_INTERNAL

#include "renderer/renderer_internal.h"
#include "glad/glad.h"

HWND getHWND(void);
HWND window_init(HINSTANCE hInstance, int nShowCmd);
void set_window_wxh(int width, int height);
void set_keybind(GameKey oldKey, GameKey newKey);

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool window_events(void);

#endif