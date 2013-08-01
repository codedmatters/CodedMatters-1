#pragma once

#include "ofMain.h"

class DepthOfField {
public:
    void setup(int w = ofGetWidth(), int h = ofGetHeight(), int MSAA = 0, float focus = 0.985, float aperture = 0.8, float maxBlur = 0.6);
    
    void beginDepth(float near = 1, float far = 500);
    void endDepth();
    
    void begin();
    void end();
    
    float focus, aperture, maxBlur;
    
private:
    ofFbo fbo;
    ofFbo depthFbo;
    
    ofShader depthShader;
    ofShader dofShader;
    
    void texturedQuad(float width, float height);
};