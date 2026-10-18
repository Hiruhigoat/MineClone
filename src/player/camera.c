#include "player/camera.h"

static Camera camera;
Mouse mouse;

// Intialize yaw and pitch which are the angles in degrees of the camera. Yaw starts at -90 to point at -z instead of +x cause orientation shenaningans
float yaw = -90.0f;
float pitch = 0.0f;

// Getters and setters for camera position, directionm, and fov
void camera_set_pos(vec3 position)
{
    camera.position = position;
}
void camera_set_dir(vec3 direction)
{
    camera.direction = direction;
}
void camera_set_fov(float fov)
{
    camera.fov = fov;
}
vec3 get_camera_position() {return camera.position;}
vec3 get_camera_directon() {return camera.direction;}
float get_camera_fov() {return camera.fov;}

// Move camera by in direction of a vector
void move_camera(vec3 movement)
{
    camera.position = vec3_add(camera.position, movement);
}

// Orients the camera depending on relative mouse movement. Sensitivity adjusts how fast the rotation is
void tilt_camera(float sensitivity)
{
    mouse = get_mouse();
    
    yaw += mouse.relX * sensitivity;
    pitch -= mouse.relY * sensitivity;
    
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    
    camera.direction = normalize((vec3){cosf(degrees_to_rad(yaw)) * cosf(degrees_to_rad(pitch)), sinf(degrees_to_rad(pitch)), sinf(degrees_to_rad(yaw)) * cosf(degrees_to_rad(pitch))});
}

void update_camera(mat4 *view, mat4 *proj)
{
    // Get transformation matrix of mapping the world vertices relative from the camera position to each vertex. +Y is set as up
    *view = lookat(
                camera.position,
                vec3_add(camera.position, camera.direction),
                (vec3){0.0f, 1.0f, 0.0f}
            );

    // Get the projection matric to map the scene onto the clipping planes
    *proj = perspective(camera.fov, 16.0f / 9.0f, 0.3f, 1000.0f);


}
