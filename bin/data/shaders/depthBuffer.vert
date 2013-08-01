varying float distanceToCam;

uniform float near;
uniform float far;

float LinearizeDepth(float z)
{
  return (2.0 * near) / (far + near - z * (far - near));
}

void main(){
    gl_FrontColor = gl_Color;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    distanceToCam = 1.0 - LinearizeDepth(gl_Position.z / gl_Position.w);
}