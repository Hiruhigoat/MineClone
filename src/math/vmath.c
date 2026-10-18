#include "math/vmath.h"

// Identity Matricies
static const mat4 MAT4_IDENTITY = 
{
//r: 0 |   1   |  2  |  3   
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

static const mat3 MAT3_IDENTITY = 
{
//r: 0 |   1   |  2   
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

// Getter for identity matrix
mat4 mat4_i()
{
    return MAT4_IDENTITY;
}

// Vector addition subtraction and multiplication
vec2 vec2_add(vec2 v1, vec2 v2)
{
    vec2 sum = 
    {
        v1.x + v2.x,
        v1.y + v2.y
    };
    return sum;
}

vec3 vec3_add(vec3 v1, vec3 v2)
{
    vec3 sum = 
    {
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z
    };
    return sum;
}

vec4 vec4_add(vec4 v1, vec4 v2)
{
    vec4 sum = 
    {
        .data = {
            v1.x + v2.x,
            v1.y + v2.y,
            v1.z + v2.z,
            v1.a + v2.a
        }
    };
    return sum;
}

vec2 vec2_sub(vec2 v1, vec2 v2)
{
    vec2 sum = 
    {
        v1.x - v2.x,
        v1.y - v2.y
    };
    return sum;
}

vec3 vec3_sub(vec3 v1, vec3 v2)
{
    vec3 sum = 
    {
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z
    };
    return sum;
}

vec3 vec3_mult(vec3 v, float mult)
{
    vec3 sum = 
    {
        v.x * mult,
        v.y * mult,
        v.z * mult
    };
    return sum;
}

vec4 vec4_sub(vec4 v1, vec4 v2)
{
    vec4 sum = 
    {
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z,
        v1.a - v2.a
    };
    return sum;
}

// Vector dot product
float vec4_dot(vec4 v1, vec4 v2)
{
    float sum = 
        v1.x * v2.x +
        v1.y * v2.y +
        v1.z * v2.z +
        v1.w * v2.w;
    return sum;
}

float vec3_dot(vec3 v1, vec3 v2)
{
    float sum = 
        v1.x * v2.x +
        v1.y * v2.y +
        v1.z * v2.z;
    return sum;
}

// Matrix addition subtraction and scaling
mat4 mat4_sum(mat4 A, mat4 B)
{
    mat4 sum;
    
    for (int i = 0; i < 16; i++)
    {
        sum.data[i] = A.data[i] + B.data[i];
    }
    
    return sum;
}

mat4 mat4_sub(mat4 A, mat4 B)
{
    mat4 result;
    
    for (int i = 0; i < 16; i++)
    {
        result.data[i] = A.data[i] + B.data[i];
    }
    
    return result;
}

mat4 mat4_scalar(mat4 A, float scalar)
{
    mat4 result;
    
    for (int i = 0; i < 16; i++)
    {
        result.data[i] =  A.data[i] * scalar;
    }
    
    return result;
}

// Getters and Setters for matricies
vec4 get_row_mat4(mat4 A, int row_index)
{
    vec4 row = 
    {
        .x = A.data[row_index],
        .y = A.data[row_index + 4],
        .z = A.data[row_index + 8],
        .w = A.data[row_index + 12],
    };

    return row;
}

void set_row_mat4(mat4 *A, int row_index, vec4 row)
{

    A-> data[row_index]      = row.x;
    A-> data[row_index + 4]  = row.y;
    A-> data[row_index + 8]  = row.z;
    A-> data[row_index + 12] = row.w;

}

vec3 get_row_mat3(mat3 A, int row_index)
{
    vec3 row = 
    {
        .x = A.data[row_index],
        .y = A.data[row_index + 3],
        .z = A.data[row_index + 2 * (3)]
    };

    return row;
}

void set_row_mat3(mat3 *A, int row_index, vec3 row)
{

    A-> data[row_index]      = row.x;
    A-> data[row_index + 3]  = row.y;
    A-> data[row_index + 6]  = row.z;
}

// Matric vertex multiplication
vec4 mat4_vec4_mult(mat4 A, vec4 v)
{
    vec4 result = 
    {
        .x = vec4_dot(get_row_mat4(A, 0), v),
        .y = vec4_dot(get_row_mat4(A, 1), v),
        .z = vec4_dot(get_row_mat4(A, 2), v),
        .w = vec4_dot(get_row_mat4(A, 3), v),
    };    
    
    return result;
}

vec3 mat3_vec3_mult(mat3 A, vec3 v)
{
    vec3 result = 
    {
        .x = vec3_dot(get_row_mat3(A, 0), v),
        .y = vec3_dot(get_row_mat3(A, 1), v),
        .z = vec3_dot(get_row_mat3(A, 2), v)
    };
    
    return result;
}

// Vector translation and scaling
vec4 vec4_translation(vec4 v, vec4 translation)
{
    mat4 translation_matrix = MAT4_IDENTITY;

    translation_matrix.col[3] = 
    (vec4) {
        .x = translation.x,
        .y = translation.y,
        .z = translation.z,
        .w = translation.w
    };

    return mat4_vec4_mult(translation_matrix, v);  
}

vec3 vec3_translation(vec3 v, vec3 translation)
{
    mat4 translation_matrix = MAT4_IDENTITY;

    translation_matrix.col[3] = 
    (vec4) {
        .x = translation.x,
        .y = translation.y,
        .z = translation.z,
        .w = 1.0f
    };

    return mat4_vec4_mult(translation_matrix, (vec4){v.x, v.y, v.z, 1}).vec3;  
}

vec4 vec4_scale(vec4 v, vec4 scale)
{
    mat4 scale_matrix = MAT4_IDENTITY;

    scale_matrix.data[0]  = scale.x;
    scale_matrix.data[5]  = scale.y;
    scale_matrix.data[10] = scale.z;
    scale_matrix.data[15] = scale.w;

    return mat4_vec4_mult(scale_matrix, v);  
}

vec3 vec3_scale(vec3 v, vec3 scale)
{
    mat3 scale_matrix = MAT3_IDENTITY;

    scale_matrix.data[0] = scale.x;
    scale_matrix.data[4] = scale.y;
    scale_matrix.data[8] = scale.z;

    return mat3_vec3_mult(scale_matrix, v);  
}

// Matrix multiplication
mat4 mat4_mult(mat4 A, mat4 B)
{
    mat4 result;

    for (size_t i = 0; i < 4; i++)
    {
        result.col[i] = 
        (vec4) {
            .x = vec4_dot(get_row_mat4(A, 0), B.col[i]),
            .y = vec4_dot(get_row_mat4(A, 1), B.col[i]),
            .z = vec4_dot(get_row_mat4(A, 2), B.col[i]),
            .w = vec4_dot(get_row_mat4(A, 3), B.col[i])
        };
    }
    
    return result;
}

mat3 mat3_mult(mat3 A, mat3 B)
{
    mat3 result;

    for (size_t i = 0; i < 3; i++)
    {
        result.col[i] = 
        (vec3) {
            .x = vec3_dot(get_row_mat3(A, 0), B.col[i]),
            .y = vec3_dot(get_row_mat3(A, 1), B.col[i]),
            .z = vec3_dot(get_row_mat3(A, 2), B.col[i])
        };
    }
    
    return result;
}

// Vector rotation
vec4 vec4_rot_x(vec4 v, float theta)
{
    mat4 translation_matrix = MAT4_IDENTITY;

    translation_matrix.data[5]  = cosf(theta);
    translation_matrix.data[6]  = sinf(theta);
    translation_matrix.data[9]  = (-1.0f * sinf(theta));
    translation_matrix.data[10] = cosf(theta);

    return mat4_vec4_mult(translation_matrix, v);  
}
vec4 vec4_rot_y(vec4 v, float theta)
{
    mat4 translation_matrix = MAT4_IDENTITY;

    translation_matrix.data[0]  = cosf(theta);
    translation_matrix.data[2]  = (-1.0f * sinf(theta));
    translation_matrix.data[8]  = sinf(theta);
    translation_matrix.data[10] = cosf(theta);

    return mat4_vec4_mult(translation_matrix, v);  
}
vec4 vec4_rot_z(vec4 v, float theta)
{
    mat4 translation_matrix = MAT4_IDENTITY;

    translation_matrix.data[0] = cosf(theta);
    translation_matrix.data[1] = sinf(theta);
    translation_matrix.data[4] = (-1.0f * sinf(theta));
    translation_matrix.data[5] = cosf(theta);

    return mat4_vec4_mult(translation_matrix, v);  
}

// Vector magnitude
float vec3_magnitude(vec3 v)
{
    float magnitude = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return magnitude;
}

// Checks if a vector is normal
bool is_normal(vec3 v)
{
    float magnitude = vec3_magnitude(v);

    if (magnitude == 1)
        return true;
    return false;
}

// Returns a normalized vector
vec3 normalize(vec3 v)
{
    float magnitude = vec3_magnitude(v);

    if (magnitude == 0)
        return (vec3){0.0f, 0.0f, 0.0f};

    vec3 normalized_vec = 
    {
        v.x = v.x / magnitude,
        v.y = v.y / magnitude,
        v.z = v.z / magnitude
    };

    return normalized_vec;
}

// Rotate about an axis
mat4 rotate(mat4 mat, float theta, vec3 axis, int space)
{
    mat4 result;
    mat4 rotation_matrix = MAT4_IDENTITY;

    if (!is_normal(axis))
        axis = normalize(axis);

    theta = degrees_to_rad(theta);
    float c = cosf(theta);
    float s = sinf(theta);

    rotation_matrix.col[0] = (vec4) {
        .x = axis.x * axis.x * (1.0f - c) + c,
        .y = axis.x * axis.y * (1.0f - c) + axis.z * s,
        .z = axis.x * axis.z * (1.0f - c) - axis.y * s,
        .w = 0.0f
    };
    
    rotation_matrix.col[1] = (vec4) {
        .x = axis.x * axis.y * (1.0f - c) - axis.z * s,
        .y = axis.y * axis.y * (1.0f - c) + c,
        .z = axis.y * axis.z * (1.0f - c) + axis.x * s,
        .w = 0.0f
    };

    rotation_matrix.col[2] = (vec4) {
        .x = axis.x * axis.z * (1.0f - c) + axis.y * s,
        .y = axis.y * axis.z * (1.0f - c) - axis.x * s,
        .z = axis.z * axis.z * (1.0f - c) + c,
        .w = 0.0f
    }; 

    result = (space == 1) ? mat4_mult(rotation_matrix, mat) : mat4_mult(mat, rotation_matrix);
    return rotation_matrix;
}

// Matrix and vertex print statements
void mat4_print(mat4 A)
{
    for (size_t i = 0; i < 4; i++)
    {
        printf("(%f, %f, %f, %f)\n", A.col[i].x, A.col[i].y, A.col[i].z, A.col[i].w);
    }
}

void vec4_print(vec4 v)
{
    printf("(%f, %f, %f, %f)\n", v.x, v.y, v.z, v.w);
}

void vec3_print(vec3 v)
{
    printf("(%f, %f, %f)\n", v.x, v.y, v.z);
}

// Radians and degrees conversion
float rad_to_degree(float rad)
{
    return rad * V_RAD2DEG;
}

float degrees_to_rad(float degrees)
{
    return degrees * V_DEG2RAD;
}

// Cross product
vec3 cross_product(vec3 v1, vec3 v2)
{   
    vec3 result =
    {
        .x = determinant_2d(v1.b_vec2, v2.b_vec2),
        .y = determinant_2d((vec2){v1.z, v1.x}, (vec2){v2.z, v2.x}),
        .z = determinant_2d(v1.f_vec2, v2.f_vec2)
    };

    return result;
}

// Determinant
float determinant_2d(vec2 v1, vec2 v2)
{
    return (v1.x * v2.y) - (v1.y * v2.x);
}

// Returns a transformation matrix of how the points need to be adjusted to be relative to a camera's position and orientation
mat4 lookat(vec3 from, vec3 to, vec3 up)
{
    mat4 lookat_mat = MAT4_IDENTITY;

    vec3 up_vector, right_vector, forward_vector;

    forward_vector = normalize(vec3_sub(from, to));
    right_vector = normalize(cross_product(normalize(up), forward_vector));
    up_vector = cross_product(forward_vector, right_vector);
    
    set_row_mat4(&lookat_mat, 0, (vec4){.vec3 = right_vector,   .d = -1.0f * vec3_dot(right_vector, from)});
    set_row_mat4(&lookat_mat, 1, (vec4){.vec3 = up_vector,      .d = -1.0f * vec3_dot(up_vector, from)});
    set_row_mat4(&lookat_mat, 2, (vec4){.vec3 = forward_vector, .d = -1.0f * vec3_dot(forward_vector, from)});
    set_row_mat4(&lookat_mat, 3, (vec4){0.0f, 0.0f, 0.0f, 1.0f});

    return lookat_mat;
}

// Returns a transfromation matrix that maps the camera relative points onto the clip planes. FOV is the "zoom" or angle of the lens.
// Aspect is aspect ratio of the screen. near and far values are the clipping planes where things that are too close or far from camera arent rendered. 
mat4 perspective(float fov, float aspect, float nearVal, float farVal)
{
    float tangent = tanf(degrees_to_rad(fov / 2));
    float top = tangent * nearVal;
    float right = top * aspect;
    
    mat4 perspective = {
        .data = {
            nearVal / right, 0.0f,             0.0f,                                         0.0f,
            0.0f,            nearVal / top,    0.0f,                                         0.0f,
            0.0f,            0.0f,            -1 * (farVal + nearVal) / (farVal - nearVal), -1.0f,
            0.0f,            0.0f,            -2 *(farVal * nearVal) / (farVal - nearVal),   0.0f
        }          
    };

    return perspective;
}