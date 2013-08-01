varying vec2 texCoords;
varying vec3 normal;

uniform vec3 lightPos;
varying vec3 toLight;

void main()
{
    texCoords = gl_MultiTexCoord0.st;
    normal = gl_Normal;
    
    toLight = normalize(vec3((gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(lightPos, 1.)).xyz - (gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex).xyz));
    
    gl_FrontColor = gl_Color;
    gl_Position = ftransform();
}