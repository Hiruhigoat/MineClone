#ifndef MATH_LIB
#define MATH_LIB

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define LOCAL_SPACE 0
#define WORLD_SPACE 1

#define V_PI 3.14159265358979323846f
#define V_DEG2RAD (V_PI / 180.0f)
#define V_RAD2DEG (180.0f / V_PI)

typedef struct 
{
    float x, y;

} vec2;

typedef union 
{
    struct {float x, y, z; };
    struct {float r, g, b; };
    struct {vec2 f_vec2; float bd; };
    struct { float fd; vec2 b_vec2; };
    float data[3];
} vec3;

typedef union 
{
    struct {float x, y, z, w; };
    struct {float r, g, b, a; };
    struct {vec3 vec3; float d;};
    float data[4];
} vec4;

typedef union 
{
    float data[16];
    float m_2d[4][4];
    vec4 col[4];
} mat4;

typedef union 
{
    float data[9];
    float m_2d[3][3];
    vec3 col[3];
} mat3;

void mat4_print(mat4 A);
void vec4_print(vec4 v);
void vec3_print(vec3 v);

float rad_to_degree(float rad);
float degrees_to_rad(float degrees);

mat4 mat4_i();

vec2 vec2_add(vec2 v1, vec2 v2);
vec3 vec3_add(vec3 v1, vec3 v2);
vec4 vec4_add(vec4 v1, vec4 v2);

vec2 vec2_sub(vec2 v1, vec2 v2);
vec3 vec3_sub(vec3 v1, vec3 v2);
vec4 vec4_sub(vec4 v1, vec4 v2);

vec3 vec3_mult(vec3 v, float mult);

float vec4_dot(vec4 v1, vec4 v2);
float vec3_dot(vec3 v1, vec3 v2);

mat4 mat4_sum(mat4 A, mat4 B);
mat4 mat4_sub(mat4 A, mat4 B);
mat4 mat4_scalar(mat4 A, float scalar);

vec4 get_row_mat4(mat4 A, int row_index);
vec3 get_row_mat3(mat3 A, int row_index);

vec4 mat4_vec4_mult(mat4 A, vec4 v);
vec3 mat3_vec3_mult(mat3 A, vec3 v);

vec4 vec4_translation(vec4 v, vec4 translation);
vec3 vec3_translation(vec3 v, vec3 translation);

vec4 vec4_scale(vec4 v, vec4 scale);
vec3 vec3_scale(vec3 v, vec3 scale);

mat4 mat4_mult(mat4 A, mat4 B);
mat3 mat3_mult(mat3 A, mat3 B);

vec4 vec4_rot_x(vec4 v, float theta);
vec4 vec4_rot_y(vec4 v, float theta);
vec4 vec4_rot_z(vec4 v, float theta);

float vec3_magnitude(vec3 v);
bool is_normal(vec3 v);
vec3 normalize(vec3 v);
mat4 rotate(mat4 mat, float theta, vec3 axis, int space);

vec3 cross_product(vec3 v1, vec3 v2);
float determinant_2d(vec2 v1, vec2 v2);

void set_row_mat4(mat4 *A, int row_index, vec4 row);
void set_row_mat3(mat3 *A, int row_index, vec3 row);

mat4 lookat(vec3 from, vec3 to, vec3 up);
mat4 perspective(float fov, float aspect, float nearVal, float farVal);

#endif

