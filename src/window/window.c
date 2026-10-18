#include "window/window.h"
#include "window/window_internal.h"

// Window Dimensions
static int window_width = 2560;
static int window_height = 1440;

// Create window handle
static HWND hwnd = NULL;

// Window dimensions and device context handle
RECT rect;
HDC hdc;

// Getters for hwnd and window dimensions
HWND get_HWND() 
{
    return hwnd;
}
RECT get_window_rect() 
{
    return rect;
}
LONG get_window_width()
{
    return rect.right;
}
LONG get_window_height()
{
    return rect.bottom;  
}
void set_window_wxh(int width, int height)
{
    window_width = width;
    window_height = height;
}

// Maps windows keys to an enum
static const GameKey key_map[256] = {
    ['W']         = KEY_W,
    ['A']         = KEY_A,
    ['S']         = KEY_S,
    ['D']         = KEY_D,
    ['Q']         = KEY_Q,
    ['E']         = KEY_E,
    [VK_SHIFT]    = KEY_SHIFT,
    [VK_SPACE]    = KEY_SPACE,
    [VK_ESCAPE]   = KEY_ESC,
    [VK_LBUTTON]  = KEY_LM,
    [VK_RBUTTON]  = KEY_RM,
    [VK_MBUTTON]  = KEY_MM,
    [VK_TAB]      = KEY_TAB,
    [VK_XBUTTON1] = KEY_MB3,
    [VK_XBUTTON2] = KEY_MB4
};

// TODO: Key binding (prolly in another file) 
void set_keybind(GameKey oldKey, GameKey newKey)
{

}

// Initalize window
HWND window_init(HINSTANCE hInstance, int nShowCmd) 
{
    // Name of the window class
    const char windowClass[] = "Game_Windows";
    
    //  Set up the window
    WNDCLASSEX wc = {
        .cbClsExtra    = 0,
        .cbSize        = sizeof(WNDCLASSEX),
        .cbWndExtra    = 0,
        .hbrBackground = NULL,
        .hCursor       = LoadCursor(NULL, IDC_ARROW),
        .hIcon         = LoadIcon(NULL, IDI_APPLICATION),
        .hIconSm       = LoadIcon(NULL, IDI_APPLICATION),
        .hInstance     = hInstance,
        .lpfnWndProc   = WinProc,
        .lpszClassName = windowClass,
        .lpszMenuName  = NULL,
        .style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW
    };
    
    // Check for registration failure
    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
            DestroyWindow(hwnd);
    }
    
    // Create the window and give it the handle
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        windowClass,
        "MineClone",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, window_width, window_height,
        NULL, NULL, hInstance, NULL
    );
    
    // Check window creation failure
    if (hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        DestroyWindow(hwnd);
    }
    
    hdc = GetDC(hwnd);
    
    // Set up raw mouse input
    RAWINPUTDEVICE rid = 
    {  
        HID_USAGE_PAGE_GENERIC,
        HID_USAGE_GENERIC_MOUSE,
        0,
        hwnd
    };
    
    // Check mouse registration
    if (!RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE)))
    {
        MessageBox(NULL, "Input Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        DestroyWindow(hwnd);
    }
    
    // Get window dimensions
    GetClientRect(hwnd, &rect);
    
    // Show and update the window on the screen
    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

    // Hide cursor
    ShowCursor(FALSE);
    ClipCursor(&rect);
    
    return hwnd;
}

// Recives msgs from the window and process the inputs
LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    bool is_down = false;
    switch (msg)
    {
        // Raw mouse for relative mouse pos
        case WM_INPUT:
        {
            set_raw_mouse((HRAWINPUT) lParam);
            break;
        }
        // Mouse for absolute position of mouse
        case WM_MOUSEMOVE:
        {
            set_absPos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); 
            break;
        }
        // When ever a key/mouse button is pressed is released it updates that keys index in the key map with its state
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            is_down = (msg == WM_KEYDOWN);
            set_key(key_map[wParam], is_down);
            break;
        }
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        {
            is_down = (msg == WM_LBUTTONDOWN);
            set_key(key_map[VK_LBUTTON], is_down);
            break;
        }
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        {
            is_down = (msg == WM_RBUTTONDOWN);
            set_key(key_map[VK_RBUTTON], is_down);
            break;
        }
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        {
            is_down = (msg == WM_MBUTTONDOWN);
            set_key(key_map[VK_MBUTTON], is_down);
            break;
        }
        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
        {
            is_down = (msg == WM_XBUTTONDOWN);
            set_key(HIWORD(wParam) == 1 ? key_map[VK_XBUTTON1] : key_map[VK_XBUTTON2] , is_down);
            break;
        }
        case WM_ENTERSIZEMOVE:
        {
            SetTimer(hwnd, 1, 1, NULL);
            break;
        }
        // Handle resizing and moving window and have elements be rendered mid input
        case WM_TIMER:
        {
            GetClientRect(hwnd, &rect);
            if (glViewport)
            {
                glViewport(0, 0, get_window_width(), get_window_height());
                renderer_update(hdc);
            }
            break;
        }
        case WM_EXITSIZEMOVE:
        {
            KillTimer(hwnd, 1);
            break;
        }
        case WM_MOVING:
        {
            glViewport(0, 0, get_window_width(), get_window_height());
            break;
        }
        case WM_SIZE:
        {
            GetClientRect(hwnd, &rect);
            if (glViewport)
            {
                glViewport(0, 0, get_window_width(), get_window_height());
                renderer_update(hdc);
            }
            break;
        }
        // Destroys the window handle to quit game
        case WM_CLOSE:
        {
            DestroyWindow(hwnd);
            break;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
        // Undefined inputs do nothing and do
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    // Msg has been handeled 
    return 0;
}

//  Sends the windows events (aka inputs) as msgs to WinProc for processing. Returns gameloop state (T/F)
bool window_events()
    {
        MSG msg;
        
        // Loops through all available msgs and sends them to winproc and if quit is gameloop is set to false
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT)
                return false;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return true;
    }    