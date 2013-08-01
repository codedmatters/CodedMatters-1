uniform sampler2DRect tex0;
varying vec2 texCoords;
uniform vec2 textureSize;

uniform float vignetteAmount;
uniform float vignetteSize;

void main(){
    vec4 col = texture2DRect(tex0, texCoords);

	// vignette
	vec2 center = textureSize / 2.0;
	vec2 d = center - vec2(texCoords);
	float distSQ = d.x*d.x + d.y*d.y;
	float invDistSQ = 1.0 / distSQ;
    
    float vignetteSizeSquared = pow(vignetteSize, 2.0);
    float vignetteValue = (distSQ / vignetteSizeSquared) * vignetteAmount;
    col.rgb += vignetteValue;
	col = clamp(col, 0.0, 1.0);

    gl_FragColor = col;
}