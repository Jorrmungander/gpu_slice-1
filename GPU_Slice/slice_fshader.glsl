#version 330
uniform sampler2D colorMapTexture;
in vec2 textureCoordinate;

layout(location = 0) out vec4 fragColor;

//texture slice fShader
void main()
{
   fragColor = texture(colorMapTexture, textureCoordinate);
}
