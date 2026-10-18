#version 330 core

#define LAYOUT_POSITION 0
#define LAYOUT_TEXTURE 1

layout (location = LAYOUT_POSITION) in vec3 position;
layout (location = LAYOUT_TEXTURE) in vec2 texture_position;

out vec2 texture_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * vec4(position, 1.0);
    texture_coords = texture_position;
}

