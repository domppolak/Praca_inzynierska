#version 450

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 mMatrix;

in vec3 vertex;
in vec2 textureCoordinate;

out vec2 varyingTextureCoordinate;

void main(void)
{
    varyingTextureCoordinate = textureCoordinate;
    gl_Position = pMatrix * vMatrix * mMatrix * vec4(vertex, 1.0);
}
