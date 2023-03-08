#version 450

//uniform mat4 matrix;
uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 mMatrix;

layout (location = 0) in vec3 vertex;
layout (location = 1) in  vec3 color;

out vec3 _color;

void main(void)
{
    _color = color;
    gl_Position = pMatrix * vMatrix * mMatrix * vec4(vertex, 1.0);
}
