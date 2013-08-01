uniform sampler2DRect ssaoTex;
uniform sampler2DRect colorTex;
uniform float ssaoWeight;

varying vec3 norm;
void main(void)
{
	vec4 color = texture2DRect( colorTex, gl_TexCoord[0].st);
	vec4 ssao = texture2DRect( ssaoTex, gl_TexCoord[0].st);
    gl_FragColor = ssao * color * gl_Color;
}