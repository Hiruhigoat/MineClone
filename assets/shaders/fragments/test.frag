#version 330 core

in vec2 texture_coords;

out vec4 out_color;

uniform sampler2D atlas;
uniform vec4 color;

void main()
{
    vec4 texture_color =  texture(atlas, texture_coords);
    out_color = vec4(color) * texture_color;
}
