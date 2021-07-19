#version 330
uniform mat4 mvpMatrix;
in vec4 contourVertexPosition;

//smooth: standard interpolater https://www.geeks3d.com/20130514/opengl-interpolation-qualifiers-glsl-tutorial/
//REMOVED: smooth out vec2 texCoord;

//contourlines vShader
void main()
{
    // Calculate vertex position in screen space.
    //LEARN: gl_position is predefined
    gl_Position = mvpMatrix * contourVertexPosition;
}


