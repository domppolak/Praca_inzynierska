#version 450

uniform sampler2D inTexture;

in vec2 varyingTextureCoordinate;

out vec4 fragColor;

void main(void)
{
   fragColor = texture(inTexture, varyingTextureCoordinate);
}

