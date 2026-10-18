#ifndef GAME_LOOP
#define GAME_LOOP

#include "renderer/renderer.h"
#include "player/movement.h"
#include <windows.h>
#include "game/game_loop.h"


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
void game_cycle(HWND hwnd);

#endif