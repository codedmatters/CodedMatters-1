uniform sampler2DRect depthtex;
uniform sampler2DRect normaltex;

uniform int reflectRays;
uniform float minThreshold;
uniform float maxThreshold;
uniform float numSamples;
uniform float radius;
uniform vec2 randSeed;
uniform float nearClip;
uniform float farClip;
uniform float exponent;

uniform vec3 samples[24];

float linearizeDepth( in float d ) {
    return (2.0 * nearClip) / (farClip + nearClip - d * (farClip - nearClip));
}

float rand(vec2 n){
    return 0.5 + 0.5 *fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

void main(void)
{

    float depth = linearizeDepth( texture2DRect( depthtex, gl_TexCoord[0].st ).r );
    if( depth == 1. ){
        gl_FragColor = vec4(1.);
        return;
    }
    bool refRay = (reflectRays == 1);
    
    vec3 norm = (refRay)? texture2DRect( normaltex, gl_TexCoord[0].st ).xyz : vec3(0,0,1);

    float ao = 0.;
    float delta, rnd;
    float dpthScl = pow(1.-depth, 2.);
    float rad = radius * dpthScl;
    float mn = minThreshold * dpthScl;
    float mx = maxThreshold * dpthScl;
    vec3 ray;

    for(int i=0; i<int(numSamples); i++){
        rnd = rand( gl_FragCoord.xy+randSeed+rnd);
        
        int sampleIndex = i;
        while(sampleIndex > 23)
            sampleIndex -= 23;
        
        ray = (refRay)? reflect( norm, -samples[sampleIndex]) * rad * rnd : samples[sampleIndex] * rad * rnd;
        delta = ( depth - mn - linearizeDepth( texture2DRect( depthtex, gl_TexCoord[0].st + ray.xy).r ));
        ao += min( 1., ( delta > 0. ) ? delta/max(delta,mx) : (mx-delta)/mx );
    }
    
    ao = pow( ao/numSamples, exponent);
    
    gl_FragColor = vec4( vec3(ao), 1. );
}
