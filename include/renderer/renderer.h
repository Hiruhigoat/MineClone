#ifndef RENDERER
#define RENDERER

#include "window/window.h"
#include "window/window_internal.h"
#include "glad/glad.h"
#include "KHR/khrplatform.h"
#include "math/vmath.h"
#include "player/camera.h"
#include "blocks/blocks.h"


#define WGL_CONTEXT_MAJOR_VERSION_ARB  0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001

extern float delta_time;

bool renderer_init(HWND hwnd);
char *load_shader_src(const char *filepath);

GLuint get_shader_program(void);
GLuint get_vao(void);

bool load_shader(GLuint *shader, GLenum shader_type, const char *filepath);
bool renderer_update(HDC hdc);

bool load_textures(void);


#endif