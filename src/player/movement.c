#include "player/movement.h"
#include "player/camera.h"

// Move with a set speed
void move(float speed)
{
    // Get the input direction
    float forward_input = 0.0f;
    float right_input   = 0.0f;
    float up_input   = 0.0f;

    if (is_down(KEY_W)) forward_input += 1.0f;
    if (is_down(KEY_S)) forward_input -= 1.0f;
    if (is_down(KEY_D)) right_input   += 1.0f;
    if (is_down(KEY_A)) right_input   -= 1.0f;
    if (!is_down(KEY_SHIFT) && is_down(KEY_SPACE)) up_input   += 1.0f;
    if (is_down(KEY_SHIFT) && is_down(KEY_SPACE)) up_input   -= 1.0f;


    vec3 camera_movement;
    vec3 movement_direction = get_camera_directon();

    // Find the forward and right directions relative to the camera
    vec3 up = {0.0f, 1.0f, 0.0f};
    vec3 right = normalize(cross_product(movement_direction, up));
    vec3 forward = normalize((vec3){movement_direction.x, movement_direction.y, movement_direction.z});

    // Set the movement magnitude with deltatime to accound for frame rate.
    vec3 movement_magnitude = 
    {
        forward_input * speed * delta_time,
        up_input * speed * delta_time,
        right_input * speed * delta_time
    };

    // Get movement vectors with the correct magnitude
    vec3 move_forward = vec3_mult(forward, movement_magnitude.x);
    vec3 move_up = vec3_mult(up, movement_magnitude.y);
    vec3 move_right = vec3_mult(right, movement_magnitude.z);
    camera_movement = vec3_add(vec3_add(move_forward, move_right), move_up);

    // Move the camera
    move_camera(camera_movement);
}