#version 330
uniform mat4 mvpMatrix;
in vec4 textureCoordinates;

//smooth: standard interpolater https://www.geeks3d.com/20130514/opengl-interpolation-qualifiers-glsl-tutorial/
smooth out vec2 textureCoordinate;

//texture slice vShader
void main()
{
    // Calculate vertex position in screen space.
    //LEARN: gl_position is predefined
    gl_Position = mvpMatrix * textureCoordinates;
    textureCoordinate = vec2(textureCoordinates.xy);
}


