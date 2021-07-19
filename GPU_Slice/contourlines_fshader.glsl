#version 330
//layout(location = 0) out vec4 fragColor;
out vec4 fragColor;

//contourlines fShader
void main()
{
    //weiß ist im Kontrast zu rot gut sichtbar -> Konturen deutlich
   fragColor = vec4(1,1,1,1);
}

