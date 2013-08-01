#include "depthOfField.h"


void DepthOfField::setup(int w, int h, int MSAA, float _focus, float _aperture, float _maxBlur) {
    ofFbo::Settings settings;
    settings.width = w;
    settings.height = h;
    settings.useDepth = true;
    settings.numSamples = MSAA;
    fbo.allocate(settings);
    depthFbo.allocate(settings);
    
    depthShader.load("shaders/depthBuffer");
    dofShader.load("shaders/dof");
    
    focus = _focus;
    aperture = _aperture;
    maxBlur = _maxBlur;
}

void DepthOfField::beginDepth(float near, float far){
    depthFbo.begin();
    ofClear(0);
    depthShader.begin();
    depthShader.setUniform1f("near", MAX(near, 1));
    depthShader.setUniform1f("far", far);
}

void DepthOfField::endDepth(){
    depthShader.end();
    depthFbo.end();
}

void DepthOfField::begin(){
    fbo.begin();
    ofClear(0);
}

void DepthOfField::end(){
    fbo.end();

    dofShader.begin();
    dofShader.setUniformTexture("tColor", fbo.getTextureReference(), 0);
    dofShader.setUniformTexture("tDepth", depthFbo.getTextureReference(), 1);
    dofShader.setUniform2f("resolution", fbo.getWidth(), fbo.getHeight());
    dofShader.setUniform1f("focus", focus);
    dofShader.setUniform1f("maxBlur", maxBlur);
    dofShader.setUniform1f("aperture", aperture);
    
    texturedQuad(fbo.getWidth(), fbo.getHeight());
    
    dofShader.end();
}

void DepthOfField::texturedQuad(float width, float height){
    ofSetColor(255);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(0, 0, 0);
    
    glTexCoord2f(width, 0);
    glVertex3f(width, 0, 0);
    
    glTexCoord2f(width, height);
    glVertex3f(width, height, 0);
    
    glTexCoord2f(0, height);
    glVertex3f(0, height, 0);
    glEnd();
}
