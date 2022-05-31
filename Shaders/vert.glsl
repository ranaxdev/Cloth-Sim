#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 20) uniform mat4 vp;

out vec4 vcolor;

void main(void){
    gl_Position = vp* vec4(position, 1.0f);
    vcolor = vec4(color, 1.0f);
}