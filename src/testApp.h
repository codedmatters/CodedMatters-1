#pragma once

#include "ofMain.h"
#include "ofxGameCamera.h"
#include "Particle.h"
#include "ofxTwBar.h"
#include "ofxSSAO.h"
#include "ofxXmlSettings.h"

#define LOGO_FONT_FILE "font/AkkoStd-Bold.otf"

class testApp : public ofBaseApp{
public:
    void setup();
    void createGui();
    void setInitialValues();
    void setupExportFrameBuffer(ofVec2f exportSize);
    void setupPreviewFrameBuffer(ofVec2f previewSize);
    void loadShaders();
    
    void rebuildGrid();
    void adjustGridSpacing();
    void adjustScale();
    
    void updateCamera();
    void updateSSAO();
    void updateWaveState();
    void updateLogo();
    void updateGradient();
    void findBounds(float *mostLeft, float *mostRight, float *mostBackOut = NULL, float *mostFrontOut = NULL);
    void updatePreviewSize();
    
    void updateParticle(Particle *p, float mostLeft, float mostRight, ofVec3f posNoiseOffset, ofVec3f scaleNoiseOffset);
    vector<Particle *> findVisibleParticles();
    
    void update();
    void draw();
    void drawBackground(float width, float height);
    void drawLogo();
    void drawScene(vector<Particle *>visibleParticles, float w, float h);
    void drawParticle(Particle *p, float mostLeft, float mostRight, float mostBack, float mostFront, ofVec3f colorNoiseOffset);
    void keyReleased(int key);
    void saveFrame(string dir);
    void windowResized(int w, int h);
    
    ofxGameCamera cam;
    vector<Particle *> particles;

    ofxTwBar bar;
    
    float directionFactor;

    int numRows;
    int numCols;
    
    float rowSpacing;
    float colSpacing;
    
    float noiseAmount;
    float noiseX;
    float noiseY;
    float noiseZ;
    float noiseSpeedX;
    float noiseSpeedY;
    float noiseSpeedZ;
    
    float scaleNoiseAmount;
    float scaleNoiseX;
    float scaleNoiseY;
    float scaleNoiseZ;
    float scaleNoiseSpeedX;
    float scaleNoiseSpeedY;
    float scaleNoiseSpeedZ;
    
    float colorNoiseAmount;
    float colorNoiseX;
    float colorNoiseY;
    float colorNoiseZ;
    float colorNoiseSpeedX;
    float colorNoiseSpeedY;
    float colorNoiseSpeedZ;
    
    bool enableSSAO;
    float ssaoWeight;
    float ssaoRadius;
    float ssaoMaxThresh;
    float ssaoMinThresh;
    float ssaoExponent;
    int ssaoNumSamples;
    bool ssaoRayReflection;
    ofxSSAO ssao;
    
    bool enableCamera;
    
    float camSpeedX;
    float camSpeedY;
    float camSpeedZ;
    bool enableRecording;
    
    bool enableShading;
    ofShader lighting;
    float ambientAmount;
    float diffuseAmount;
    ofVec3f lightPos;
    float baseColor[3];
    float baseColor2[3];
    float backgroundBottomColor[3];
    float backgroundTopColor[3];
    
    float baseColorRotationAngle;
    float baseColorTopLeftFactor;
    float baseColorTopRightFactor;
    float baseColorBottomRightFactor;
    float baseColorBottomLeftFactor;
    
    float backgroundColorRotationAngle;
    
    bool useCone;
    bool useSecondCone;
    bool useSphereInCone;
    bool drawDirection;
    ofVec3f particleSize;
    float particleSpeed;
    
    bool waveIn;
    bool waveOut;
    float waveInDuration;
    float waveOutDuration;
    
    int waveInStartFrame;
    int waveOutStartFrame;
    
    ofShader antiAliasShader;
    
    bool exportDepth;
    bool drawDepth;
    float depthNear;
    float depthFar;
    ofShader depthMapShader;
    
    ofShader filter;
    bool enableFilter;
    float filterVignetteSize;
    float filterVignetteAmount;
    
    float rotationPhase;
    float rotationSpeed;
    
    ofFbo previewFbo;
    ofVec2f previewSize;
    ofFbo exportFbo;
    ofVec2f exportSize;
    bool setExportSizeToA5;
    bool setExportSizeToA2;
    bool setExportSizeToVideo;
    
    bool showLogo;
    ofVec3f logoPos;
    int logoFontsize;
    ofTrueTypeFont logoFont;
    float logoColor[3];
    
    bool saveSettingsButton;
    void saveSettings();
    ofxXmlSettings xml;
};
