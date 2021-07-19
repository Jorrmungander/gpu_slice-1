#version 330
uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
in vec4 streamVertexPosition;


// streamlines vShader
void main()
{

      // Calculate vertex position in screen space.
      // Berechnung der Vertexposition (Transformation von Objekt- zu Clipkoordinaten)
      gl_Position = mvpMatrix * streamVertexPosition;
}
