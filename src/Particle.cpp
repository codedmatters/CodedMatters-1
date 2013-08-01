//
//  Particle.cpp
//  emptyExample
//
//  Created by Daan van Hasselt on 7/18/13.
//
//

#include "Particle.h"

void Particle::setup(ofVec3f _pos, ofVec3f _dir, ofVec3f _size){
    basePos = _pos;
    pos  = _pos;
    dir = _dir;
    size = _size;
    scale = 1.;
    directionFactor = 1.0;
    useCone = true;
    drawDirection = false;
    
    speed = 0;
    scaleMultiplier = 1.0;
    useSecondCone = false;
    useSphereInCone = false;
}

void Particle::update(){
    basePos.x += speed;
}

void Particle::draw(){
    ofPushMatrix();
    ofTranslate(pos);
    
    
    dir.normalize();
    ofVec3f forward = ofVec3f(1, 0, 0);
    ofVec3f axis = forward.crossed(dir);
    axis.normalize();
    
    float angle = forward.angle(dir);
    angle = pow(angle, directionFactor);
    
    ofRotate(angle, axis.x, axis.y, axis.z);
    ofTranslate(-size.x / 2., -size.y / 2., -size.z / 2.);
    ofScale(size.x, size.y, size.z);
    ofScale(1, scale * scaleMultiplier, scale * scaleMultiplier);
    
    float thresh = 0.5;
    if(size.x * scale * scaleMultiplier < thresh || size.y * scale * scaleMultiplier < thresh || size.z * scale * scaleMultiplier < thresh){
        ofPopMatrix();
        return;
    }
    
    if(!useCone){
        ofBox(1);
    }
    else {
        ofPushMatrix();
        
        if(useSphereInCone){
            ofPushMatrix();
            ofPushStyle();
            ofScale(1. / size.x, 1. / size.y, 1. / size.z);
            float sphereScale = MAX(size.y, size.z);
            ofScale(sphereScale, size.y * 0.98, size.z * 0.98);
            ofSphere(1);
            ofPopStyle();
            ofPopMatrix();
        }
        
        ofRotateY(90);
        ofCone(1, 2);
        
        if(useSecondCone){
            ofRotateY(180);
            ofCone(1, 2);
        }
        
        ofPopMatrix();
    }
    
    if(drawDirection){
        ofScale(2, 0.35, 0.35);
        ofBox(1);
    }
    

    ofPopMatrix();
}