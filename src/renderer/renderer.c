#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image.h"
#include "renderer/renderer.h"

mat4 view;
mat4 proj;
GLuint tex;
GLuint shader_program;

World world;

vec4 block_color;

// Time keeping shit
float current_time  = 0.0f;
float previous_time = 0.0f;
float delta_time    = 0.0f;

static float get_time_seconds(void) 
{
    static struct timespec start_ts;
    static int initialized = 0;
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    if (!initialized) 
    {
        start_ts = ts;
        initialized = 1;
    }
    return (float)(ts.tv_sec - start_ts.tv_sec) + ((float)(ts.tv_nsec - start_ts.tv_nsec) / 1000000000.0f);
}

// Getter for shader program
GLuint get_shader_program(void)
{
    return shader_program;
}

// Intialize render passing in the window handle
bool renderer_init(HWND hwnd)
{ 
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLint status;
    GLint link_status;

    // Intialize camera
    block_color            = (vec4){1.0f};
    vec3  camera_position  = (vec3){8.0f, 0.0f, 8.0f};
    vec3  camera_direction = (vec3){1.0f , 1.0f ,   1.0f};
    float camera_fov       = 100.0f;
    camera_set_pos(camera_position);
    camera_set_dir(camera_direction);
    camera_set_fov(camera_fov);

    // Get the device context from the window handle
    HDC hdc = GetDC(hwnd);
    if (!hdc)
    {
        MessageBox(NULL, "Setting HDC Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        DestroyWindow(hwnd);
        return false;
    }

    // Create the skeleton function for opengl post 1.1
    typedef HGLRC (WINAPI * PFN_wglCreateContextAttribsARB) (HDC hdc, HGLRC hglrc, const int *atribList);

    // Set the opengl version
    int atribList[] = 
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    // Format the pixels
    PIXELFORMATDESCRIPTOR pfd = 
    {
        .nSize        = sizeof(PIXELFORMATDESCRIPTOR),
        .nVersion     = 1,
        .dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .iPixelType   = PFD_TYPE_RGBA,
        .cColorBits   = 32,
        .cDepthBits   = 24,
        .cStencilBits = 8
    };

    // Set the pixel format that will be used by opengl
    int pf = ChoosePixelFormat(hdc, &pfd);
    if (pf == 0)
    {
        MessageBox(NULL, "Choosing Pixel Format Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        DestroyWindow(hwnd);
        return false;
    }

    if (!SetPixelFormat(hdc, pf, &pfd))
    {
        DWORD error = GetLastError();
        printf("%ld", error);
        MessageBox(NULL, "Setting Pixel Format Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        DestroyWindow(hwnd);
        return false; 
    }

    // Create a dummy context for opengl 1.1
    HGLRC dummy_hglrc = wglCreateContext(hdc);
    if (!dummy_hglrc)
    {
        MessageBox(NULL, "Create Context for OpenGl 1.1 Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        DestroyWindow(hwnd);
        return false;
    }

    wglMakeCurrent(hdc, dummy_hglrc);

    // Get the new opengl create context funciton for drivers
    PFN_wglCreateContextAttribsARB wglCreateContextAttribsARB = (PFN_wglCreateContextAttribsARB)wglGetProcAddress("wglCreateContextAttribsARB");
    if (!wglCreateContextAttribsARB)
    {
        MessageBox(NULL, "Create Context for OpenGl post 1.1 Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        DestroyWindow(hwnd);
        return false;
    }

    // Create the new opengl context as current
    HGLRC hglrc = wglCreateContextAttribsARB(hdc, NULL, atribList);
    
    // Delete dummy contect
    wglDeleteContext(dummy_hglrc);

    // Set the new opengl context as current
    wglMakeCurrent(hdc, hglrc);

    // Load glad funcitons for newer opengl versions
    if (!gladLoadGL())
    {
        MessageBox(NULL, "Glad Loader Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        DestroyWindow(hwnd);
        return false;
    }

    // Get the window dimensions and set allign with viewport and enable depth testing for depth sorting
    glViewport(0, 0, get_window_width(), get_window_height());
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    world_init();

    // Load and compile shaders
    load_shader(&vertex_shader, GL_VERTEX_SHADER, "assets/shaders/vertex/test.vert");
    load_shader(&fragment_shader, GL_FRAGMENT_SHADER, "assets/shaders/fragments/test.frag");

    glCompileShader(vertex_shader);
    glCompileShader(fragment_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        char status_buffer[512];
        glGetShaderInfoLog(vertex_shader, 512, NULL, status_buffer);
        MessageBox(NULL, "Vertex shader compilation failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
            printf("%s", status_buffer);
        return false;
    }
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        char status_buffer[512];
        glGetShaderInfoLog(fragment_shader, 512, NULL, status_buffer);
        MessageBox(NULL, "Fragment shader compilation failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
            printf("%s", status_buffer);
        return false;
    }

    // Create and link the shader program
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glBindFragDataLocation(shader_program, 0, "out_color");
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE)
    {
        char link_status_buffer[512];
        glGetProgramInfoLog(shader_program, 512, NULL, link_status_buffer);
        MessageBox(NULL, link_status_buffer, "Shader Link Error", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }
    
    glUseProgram(shader_program);

    load_textures();
    
    // Intialize rendering window
    glViewport(0, 0, get_window_width(), get_window_height());
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    return true;
}

// Load Shader files
bool load_shader(GLuint *shader, GLenum shader_type, const char *filepath)
{
    

    FILE *file = fopen(filepath, "rb");
    if (!file)
    {
        MessageBox(NULL, "File loading for shader failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return false;
    }
    
    // Measure the file size to dynamically allocate memory for it
    fseek(file, 0, SEEK_END);
    long file_length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the loaded shadder
    char *shader_buffer = (char*) malloc(file_length + 1);
    if (!shader_buffer)
    {
        MessageBox(NULL, "Shader memory allocation failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    // "Copy" the source file into the shadder buffer pointer and append a null terminator
    fread(shader_buffer, 1, file_length, file);
    shader_buffer[file_length] = '\0';
    fclose(file);
    
    // Create the shader and free the buffer
    *shader = glCreateShader(shader_type);
    glShaderSource(*shader, 1, (const GLchar**) &shader_buffer, NULL);
    free(shader_buffer);

    return true;
}

// Load texture atlas
bool load_textures(void)
{
    int width, height, channels;
    unsigned char *image;

    // Generate and bind textures
    // Atlas Texture
    stbi_set_flip_vertically_on_load(true);
    image = stbi_load("assets/textures/block_atlas.png", &width, &height, &channels, 4);
    if (!image)
    {
        MessageBox(NULL, "Texture loading failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glUniform1i(glGetUniformLocation(shader_program, "atlas"), 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image);
    return true;
}

// Update the rendering (called each frame)
bool renderer_update(HDC hdc)
{
    if (glViewport)
    {
        // Get current time and camera states and set delta time
        current_time     = get_time_seconds();
        delta_time       = current_time - previous_time;

        // Update camera
        update_camera(&view, &proj);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, (GLfloat *) &view);
        glUniformMatrix4fv(glGetUniformLocation(shader_program, "proj"), 1, GL_FALSE, (GLfloat *) &proj); 
        glUniform4f(glGetUniformLocation(shader_program, "color"), 1, 1, 1, 1); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int j = 0; j < surronding_render; j++)
        {
            for (int i = 0; i < surronding_render; i++)
            {
                world = get_world();
                int chunk_index = get_chunk_index(i,j);
                glBindVertexArray(world[chunk_index]->mesh.vao);
                glDrawArrays(GL_TRIANGLES, 0, world[chunk_index]->mesh.vertex_count);
            }
        }

        SwapBuffers(hdc);
        
        // Update previous time
        previous_time = current_time;
        return true;
    }

    return false;
}