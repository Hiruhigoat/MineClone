#include "game/input.h"
// Mouse and keys
static Keys keys[KEY_COUNT];
static Mouse mouse;

// Getters for a key and all its states
Keys get_key(GameKey key) 
{ 
    return keys[key];
}
bool is_pressed(GameKey key) 
{
    if (key == KEY_NONE || key >= KEY_COUNT) return false; 
    return keys[key].pressed;
}
bool is_released(GameKey key) 
{
    if (key == KEY_NONE || key >= KEY_COUNT) return false; 
    return keys[key].released;
}
bool is_down(GameKey key) 
{
    if (key == KEY_NONE || key >= KEY_COUNT) return false; 
    return keys[key].down;
}

// Getters for the mouse and all its positions
Mouse get_mouse() {return mouse;}
LONG get_mouseRX() {return mouse.relX / 100;}
LONG get_mouseRY() {return mouse.relY / 100;}
LONG get_mouseAX() {return mouse.absX;}
LONG get_mouseAY() {return mouse.absY;}

// Setter for the key and its states
void set_key(GameKey key, bool is_down)
{
    if (key == KEY_NONE || key >= KEY_COUNT) return; 
    keys[key].pressed |= (!keys[key].down && is_down);
    keys[key].released |= (keys[key].down && !is_down);
    keys[key].down = is_down;
}

// Setter for mouse
void set_raw_mouse(HRAWINPUT raw_mouse_input)
{
    RAWINPUT raw;
    UINT size_of_raw = sizeof(RAWINPUT);
    if (GetRawInputData(raw_mouse_input, RID_INPUT, &raw, 
                        &size_of_raw, sizeof(RAWINPUTHEADER)) == (UINT) -1)
        return;

    if (raw.header.dwType != RIM_TYPEMOUSE)
        return;
    
    // Relative mouse position accumulation each frame (resets each frame in input_update)
    mouse.relX += raw.data.mouse.lLastX;
    mouse.relY += raw.data.mouse.lLastY;
}

// Setter for absolute mosuse position
void set_absPos(int absX, int absY) 
{
    mouse.absX = absX;
    mouse.absY = absY;
}

// Update input each frame; resets pressed and released for keys to 0 and relative mouse position. 
void input_update(void)
{
    for (int i = 0; i < KEY_COUNT; i++)
    {
        keys[i].pressed = false;
        keys[i].released = false;
    }

    mouse.relX = 0;
    mouse.relY = 0;
}




