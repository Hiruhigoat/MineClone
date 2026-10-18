#ifndef WINDOW
#define WINDOW

#include <windows.h>
#include <windowsx.h>
#include <hidusage.h>
#include "game/input.h"
#include <stdio.h>
#include "renderer/renderer.h"
#include <time.h>


LONG get_window_width(void);
LONG get_window_height(void);
RECT get_window_rect(void) ;

#endif