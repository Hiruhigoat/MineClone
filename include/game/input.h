/**
 * @file input.h
 * @brief Handles the games inputs
 * @author Fares
 * @date 2026-10-15
 */

#ifndef INPUT_HANDLER
#define INPUT_HANDLER

#include <stdbool.h>
#include <windows.h>

/**
 * @brief Key options can can be accepted as an input
 */
typedef enum 
{
    KEY_NONE, /**< Any input that isn't specified */
    KEY_W,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_Q,
    KEY_E,
    KEY_SHIFT,
    KEY_SPACE,
    KEY_ESC,
    KEY_LM, /**< Left Mouse button */
    KEY_MM, /**< Scroll Wheel button */
    KEY_RM, /**< Right Mouse button */
    KEY_MB3, /**< Side Mouse button 1 */
    KEY_MB4, /**< Side Mouse button 2 */
    KEY_TAB,
    KEY_COUNT /**< keeps track of size for indicing purposes */
} GameKey;

/**
 * @brief Represents a key and the states it can be in such as: is the key down, has it been pressed or released this frame
 */
typedef struct 
{
    bool down;
    bool pressed;
    bool released;
} Keys;

/**
 * @brief Represents a mouse and the values it hold, both relative and absolute mouse positions. 
 * Absolute position is relative to the top left pixel of the client area. 
 * Relative is relative to the mouses last position
 */
typedef struct
{
    LONG relX;
    LONG relY;
    int absX;
    int absY;
} Mouse;

/**
 * @brief Retrieves the current game key.
 *
 * @param[in] key Which key to be retrived.
 * @return The desired key's struct.
 * @see set_key
 */
Keys get_key(GameKey key);

/**
 * @brief Gives the state of wheather the game key was pressed this frame.
 *
 * @param[in] key Which key to be checked.
 * @return Boolean if the key was pressed this frame.
 */
bool is_pressed(GameKey key);

/**
 * @brief Gives the state of wheather the game key was released this frame.
 *
 * @param[in] key Which key to be checked.
 * @return Boolean if the key was released this frame.
 */
bool is_released(GameKey key);

/**
 * @brief Gives the state of wheather the game key is down.
 *
 * @param[in] key Which key to be checked.
 * @return Boolean if the key is down this frame.
 */
bool is_down(GameKey key);


/**
 * @brief Retrieves the mouse struct.
 *
 * @return The mouse struct.
 */
Mouse get_mouse();

/**
 * @brief Retrieves the current relative X position of the mouse compared to its last position.
 *
 * @return The relative x position of the mouse
 * @see set_raw_mouse
 */
LONG get_mouseRX();

/**
 * @brief Retrieves the current relative Y position of the mouse compared to its last position.
 *
 * @return The relative Y position of the mouse
 * @see set_raw_mouse
 */
LONG get_mouseRY();

/**
 * @brief Retrieves the current absolute X position of the mouse compared to the top left of the client area.
 *
 * @return The absolute X position of the mouse
 * @see set_absPos
 */
LONG get_mouseAX();

/**
 * @brief Retrieves the current absolute Y position of the mouse compared to the top left of the client area.
 *
 * @return The absolute Y position of the mouse
 * @see set_absPos
 */
LONG get_mouseAY();

/**
 * @brief Sets the desired game key's down state
 *
 * @param[in] key Which key to be set.
 * @param[in] is_down wheather the key should be set as down or not.
 * 
 * @note Setting the wheather the key is down automatically sets the keys pressed and released states this frame
 * which is then updated in the inpute update function
 * 
 * @see get_key, input_update
 */
void set_key(GameKey key, bool is_down);

/**
 * @brief Sets the mouses's relatives position
 *
 * @param[in] raw_mouse_input The raw input of the mouse.
 * 
 * @note The relative positions are accumulated to handle multiple calls in a single frame due to high polling rates and is then updated in the inpute update function.
 * 
 * @see get_mouseRX, get_mouseRY, input_update
 */
void set_raw_mouse(HRAWINPUT raw_mouse_input);

/**
 * @brief Sets the mouses's absolute position
 *
 * @param[in] absXPosition The absolute X position of the mouse.
 * @param[in] absYPosition The absolute Y position of the mouse.
 *  
 * @see get_mouseAX, get_mouseAY
 */
void set_absPos(int absXPosition, int absYPosition);


/**
 * @brief Updates the inputs each frame
 * 
 * The relative mouse positions are set to 0 each frame to account for mouse stopping.
 * The pressed and released of the keys are also reset at each frame so that they are only true in the frames where the event occured
 * 
 * @see set_key, set_raw_mouse
 */
void input_update(void);

#endif