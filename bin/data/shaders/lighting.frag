varying vec2 texCoords;
varying vec3 normal;

varying vec3 toLight;
uniform float ambientAmount;
uniform float diffuseAmount;
uniform int enableShading;

void main(void)
{
    if(enableShading == 1){
        vec3 color = gl_Color.rgb;
        vec3 ambientColor = color * vec3(ambientAmount);
        vec3 diffuseColor = color * vec3(diffuseAmount);

        vec3 normalizedToLight = normalize(toLight);
        
        // basic cross product based diffusion
        float diffuseTerm = pow(abs( dot(normalize(normal), normalizedToLight)), 4.);
        diffuseTerm = min(diffuseTerm, 1.);
        gl_FragColor = vec4(ambientColor + diffuseColor * diffuseTerm, 1.0);
    }
    else {
        gl_FragColor = gl_Color;
    }
}