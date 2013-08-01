#pragma once

#include "ofMain.h"

class Particle {
public:
    void setup(ofVec3f pos, ofVec3f dir = ofVec3f(1, 0, 0), ofVec3f size = ofVec3f(40, 4, 4));
    void update();
    void draw();
    
    ofVec3f basePos;
    ofVec3f pos;
    ofVec3f dir;
    ofVec3f size;
    float scale;
    float scaleMultiplier;
    
    float directionFactor;
    
    bool useCone;
    bool useSecondCone;
    bool useSphereInCone;
    bool drawDirection;
    
    float speed;
};