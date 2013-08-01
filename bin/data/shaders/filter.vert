uniform sampler2DRect tex0;
varying vec2 texCoords;

void main()
{
    texCoords = gl_MultiTexCoord0.st;
    gl_FrontColor = gl_Color;
    gl_Position = ftransform();
}