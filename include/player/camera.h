#ifndef CAMERA_CONTROLLER
#define CAMERA_CONTROLLER

#include "renderer/renderer.h"
#include "renderer/renderer_internal.h"
#include "math/vmath.h"

typedef struct 
{
    vec3 position;
    vec3 direction;
    float fov;
} Camera;


void camera_set_pos(vec3 position);
void camera_set_dir(vec3 direction);
void camera_set_fov(float fov);
void move_camera(vec3 movement);
void tilt_camera(float sensitivity);

vec3 get_camera_position(void);
vec2 get_camera_chunk_position(void);
vec3 get_camera_directon(void);
float get_camera_fov(void);

void update_camera(mat4 *view, mat4 *proj);



#endif