#version 450 core
layout (location = 2) in vec3 position;
layout (location = 3) in vec3 color;
layout (location = 20) uniform mat4 vp;
layout (location = 21) uniform mat4 model;
out vec4 vcolor;

void main(void){
    gl_Position = vp* model * vec4(position, 1.0f);
    vcolor = vec4(color, 1.0f);
}