#include "testApp.h"

#pragma mark - Setup

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(0);
    ofSetVerticalSync(true);
    ofSetFrameRate(30);
    
	// create the directories needed for export
	char cmd[256];
	sprintf(cmd, "mkdir -p %s %s %s %s", ofToDataPath("framedata/frames/rgb").c_str(), ofToDataPath("framedata/frames/depth").c_str(), ofToDataPath("framedata/screenshots/rgb").c_str(), ofToDataPath("framedata/screenshots/depth").c_str());
	int a = system(cmd);
	
    cam.setup();
    cam.dampen = true;
    createGui();
    setInitialValues();
    setupPreviewFrameBuffer(previewSize);
    loadShaders();
}

//--------------------------------------------------------------
void testApp::createGui(){
    bar.init("GUI", 400, 600, 100, 100, 100, 100);
    bar.enable();
    
    bar.addParam("Save settings", &saveSettingsButton);
    
    bar.addSeparator();
    bar.addParam("Enable camera", &enableCamera);
    bar.addParam("Base color rotation", &baseColorRotationAngle, " min=0.0 max=360.0 step=0.5");
    bar.addParam("Base color", baseColor, " ", false, TW_TYPE_COLOR3F);
    bar.addParam("Base color 2", baseColor2, " ", false, TW_TYPE_COLOR3F);
    bar.addParam("Background color rotation", &backgroundColorRotationAngle, " min=0.0 max=360.0 step=0.5");
    bar.addParam("Background color top", backgroundTopColor, " ", false, TW_TYPE_COLOR3F);
    bar.addParam("Background color bottom", backgroundBottomColor, " ", false, TW_TYPE_COLOR3F);
    
    bar.addSeparator();
    bar.addParam("Export width", &exportSize.x, " min=1.0 max=8185.0 step=1.0 group='Export'");
    bar.addParam("Export height", &exportSize.y, " min=1.0 max=8185.0 step=1.0 group='Export'");
    bar.addParam(": A2 320dpi (5291 x 7483)", &setExportSizeToA2, " group='Export'");
    bar.addParam(": A5 350dpi (2039 x 2894)", &setExportSizeToA5, " group='Export'");
    bar.addParam(": HD video  (1920 x 1080)", &setExportSizeToVideo, " group='Export'");
    
    bar.addSeparator();
    bar.addParam("Enable recording", &enableRecording, " group='Recording'");
    bar.addParam("Cam speed X", &camSpeedX, " min=-10.0 max=10.0 step=0.01 group='Recording'");
    bar.addParam("Cam speed Y", &camSpeedY, " min=-10.0 max=10.0 step=0.01 group='Recording'");
    bar.addParam("Cam speed Z", &camSpeedZ, " min=-10.0 max=10.0 step=0.01 group='Recording'");
    bar.addParam("Wave in duration (frames)", &waveInDuration, " min=0.0 max=1000.0 step=1 group='Recording'");
    bar.addParam("Wave out duration (frames)", &waveOutDuration, " min=0.0 max=1000.0 step=1 group='Recording'");
    bar.addParam("Wave in", &waveIn, " group='Recording'");
    bar.addParam("Wave out", &waveOut, " group='Recording'");
    
    bar.addSeparator();
    bar.addParam("Num rows", &numRows, " min=0. max=500.0 step=1 group='Particles'");
    bar.addParam("Num cols", &numCols, " min=0. max=500.0 step=1 group='Particles'");
    bar.addParam("Row spacing", &rowSpacing, " min=0. max=80.0 step=0.1 group='Particles'");
    bar.addParam("Column spacing", &colSpacing, " min=0. max=80.0 step=0.1 group='Particles'");
    bar.addParam("Direction amount", &directionFactor, " min=0. max=5.0 step=0.01 group='Particles'");
    bar.addParam("Use cone", &useCone, " group='Particles'");
    bar.addParam("Use second cone", &useSecondCone, " group='Particles'");
    bar.addParam("Use sphere in cone", &useSphereInCone, " group='Particles'");
    bar.addParam("Draw direction", &drawDirection, " group='Particles'");
    bar.addParam("Particle length", &particleSize.x, " min=0.0 max=50.0 step=0.1 group='Particles'");
    bar.addParam("Particle height", &particleSize.y, " min=0.0 max=50.0 step=0.1 group='Particles'");
    bar.addParam("Particle width", &particleSize.z, " min=0.0 max=50.0 step=0.1 group='Particles'");
    bar.addParam("Particle speed", &particleSpeed, " min=-10.0 max=10.0 step=0.01 group='Particles'");
    bar.addParam("Particle rotation", &rotationSpeed, " min=-0.1 max=0.1 step=0.001 group='Particles'");

    bar.addSeparator();
    bar.addParam("Noise amount", &noiseAmount, " min=0.0 max=1000.0 step=0.01 group='Noise'");
    bar.addParam("Noise X", &noiseX, " min=0.0 max=1000.0 step=0.01 group='Noise'");
    bar.addParam("Noise Y", &noiseY, " min=0.0 max=1000.0 step=0.01 group='Noise'");
    bar.addParam("Noise Z", &noiseZ, " min=0.0 max=1000.0 step=0.01 group='Noise'");
    bar.addParam("Noise X speed", &noiseSpeedX, " min=0.0 max=100.0 step=0.01 group='Noise'");
    bar.addParam("Noise Y speed", &noiseSpeedY, " min=0.0 max=100.0 step=0.01 group='Noise'");
    bar.addParam("Noise Z speed", &noiseSpeedZ, " min=0.0 max=100.0 step=0.01 group='Noise'");
    
    bar.addSeparator();
    bar.addParam("Scale noise amount", &scaleNoiseAmount, " min=0.0 max=1000.0 step=0.01 group='Scale noise'");
    bar.addParam("Scale noise X", &scaleNoiseX, " min=0.0 max=1000.0 step=0.01 group='Scale noise'");
    bar.addParam("Scale noise Y", &scaleNoiseY, " min=0.0 max=1000.0 step=0.01 group='Scale noise'");
    bar.addParam("Scale noise Z", &scaleNoiseZ, " min=0.0 max=1000.0 step=0.01 group='Scale noise'");
    bar.addParam("Scale noise X speed", &scaleNoiseSpeedX, " min=0.0 max=100.0 step=0.01 group='Scale noise'");
    bar.addParam("Scale noise Y speed", &scaleNoiseSpeedY, " min=0.0 max=100.0 step=0.01 group='Scale noise'");
    bar.addParam("Scale noise Z speed", &scaleNoiseSpeedZ, " min=0.0 max=100.0 step=0.01 group='Scale noise'");
    
    bar.addSeparator();
    bar.addParam("Color noise amount", &colorNoiseAmount, " min=0.0 max=1.0 step=0.01 group='Color noise'");
    bar.addParam("Color noise X", &colorNoiseX, " min=0.0 max=1000.0 step=0.01 group='Color noise'");
    bar.addParam("Color noise Y", &colorNoiseY, " min=0.0 max=1000.0 step=0.01 group='Color noise'");
    bar.addParam("Color noise Z", &colorNoiseZ, " min=0.0 max=1000.0 step=0.01 group='Color noise'");
    bar.addParam("Color noise X speed", &colorNoiseSpeedX, " min=0.0 max=100.0 step=0.01 group='Color noise'");
    bar.addParam("Color noise Y speed", &colorNoiseSpeedY, " min=0.0 max=100.0 step=0.01 group='Color noise'");
    bar.addParam("Color noise Z speed", &colorNoiseSpeedZ, " min=0.0 max=100.0 step=0.01 group='Color noise'");
    
    bar.addSeparator();
    bar.addParam("Enable SSAO", &enableSSAO);
    bar.addParam("SSAO radius", &ssaoRadius, " min=0.0 max=200.0 step=0.1 group='SSAO'");
    bar.addParam("SSAO min threshold", &ssaoMinThresh, " min=0.0 max=0.2 step=0.001 group='SSAO'");
    bar.addParam("SSAO max threshold", &ssaoMaxThresh, " min=0.0 max=0.2 step=0.001 group='SSAO'");
    bar.addParam("SSAO exponent", &ssaoExponent, " min=0.0 max=8.0 step=0.01 group='SSAO'");
    bar.addParam("SSAO num samples", &ssaoNumSamples, " min=0 max=512 step=1 group='SSAO'");

    bar.addSeparator();
    bar.addParam("Enable shading", &enableShading, " group='Shading'");
    bar.addParam("Ambient amount", &ambientAmount, " min=0.0 max=1.0 step=0.01 group='Shading'");
    bar.addParam("Diffuse amount", &diffuseAmount, " min=0.0 max=1.0 step=0.01 group='Shading'");
    bar.addParam("Light X", &lightPos.x, " min=-1.0 max=1.0 step=0.01 group='Shading'");
    bar.addParam("Light Y", &lightPos.y, " min=-1.0 max=1.0 step=0.01 group='Shading'");
    bar.addParam("Light Z", &lightPos.z, " min=-1.0 max=1.0 step=0.01 group='Shading'");
    
    bar.addSeparator();
    bar.addParam("Enable filter", &enableFilter, " group='Filter'");
    bar.addParam("Vignette size", &filterVignetteSize, " min=0.0 max=10000.0 step=0.1 group='Filter'");
    bar.addParam("Vignette amount", &filterVignetteAmount, " min=-1.0 max=1.0 step=0.001 group='Filter'");

    bar.addSeparator();
    bar.addParam("Enable depth map (export only)", &exportDepth, " group='Export settings'");
    bar.addParam("Show depth map", &drawDepth, " group='Export settings'");
    bar.addParam("Depth near value", &depthNear, " min=0.0 max=1000.0 step=0.01 group='Export settings'");
    bar.addParam("Depth far value", &depthFar, " min=1.0 max=1000.0 step=0.5 group='Export settings'");
    
    bar.addSeparator();
    bar.addParam("Show logo", &showLogo, " group='Logo'");
    bar.addParam("Font size", &logoFontsize, " min=1 max=1000 step=1 group='Logo'");
    bar.addParam("Logo color", logoColor, " group='Logo' ", false, TW_TYPE_COLOR3F);
    bar.addParam("Logo position X", &logoPos.x, " min=-10000 max=10000 step=1 group='Logo'");
    bar.addParam("Logo position Y", &logoPos.y, " min=-10000 max=10000 step=1 group='Logo'");
    bar.addParam("Logo position Z", &logoPos.z, " min=-10000 max=10000 step=1 group='Logo'");
    
}

//--------------------------------------------------------------
void testApp::setInitialValues(){
    xml.loadFile("settings.xml");
    
    enableRecording = false;
    camSpeedX = xml.getValue("camSpeedX", 0);
    camSpeedY = xml.getValue("camSpeedY", 0);
    camSpeedZ = xml.getValue("camSpeedZ", 0);
    waveIn = false;
    waveInDuration = xml.getValue("waveInDuration", 250);
    waveInStartFrame = -1;
    waveOut = false;
    waveOutDuration = xml.getValue("waveOutDuration", 250);
    waveOutStartFrame = -1;
    
    baseColorRotationAngle = xml.getValue("baseColorRotationAngle", 0);
    baseColor[0] = xml.getValue("baseColorR", 78. / 255.);
    baseColor[1] = xml.getValue("baseColorG", 36. / 255.);
    baseColor[2] = xml.getValue("baseColorB", 93. / 255.);
    
    baseColor2[0] = xml.getValue("baseColor2R", 42. / 255.);
    baseColor2[1] = xml.getValue("baseColor2G", 166. / 255.);
    baseColor2[2] = xml.getValue("baseColor2B", 62. / 255.);
    
    backgroundColorRotationAngle = xml.getValue("backgroundColorRotationAngle", 0);
    backgroundBottomColor[0] = xml.getValue("backgroundBottomColorR", 230. / 255.);
    backgroundBottomColor[1] = xml.getValue("backgroundBottomColorG", 240. / 255.);
    backgroundBottomColor[2] = xml.getValue("backgroundBottomColorB", 255. / 255.);
    
    backgroundTopColor[0] = xml.getValue("backgroundTopColorR", 255. / 255.);
    backgroundTopColor[1] = xml.getValue("backgroundTopColorG", 255. / 255.);
    backgroundTopColor[2] = xml.getValue("backgroundTopColorB", 255. / 255.);
    
    directionFactor = xml.getValue("directionFactor", 1.5);
    
    numRows = xml.getValue("numRows", 40);
    numCols = xml.getValue("numCols", 150);
    rowSpacing = xml.getValue("rowSpacing", 7.5);
    colSpacing = xml.getValue("colSpacing", 10.0);
    
    noiseAmount = xml.getValue("noiseAmount", 500.0);
    noiseX = xml.getValue("noiseX", 1000.0);
    noiseY = xml.getValue("noiseY", 500.0);
    noiseZ = xml.getValue("noiseZ", 600.0);
    noiseSpeedX = xml.getValue("noiseSpeedX", 1.0);
    noiseSpeedY = xml.getValue("noiseSpeedY", 0.75);
    noiseSpeedZ = xml.getValue("noiseSpeedZ", 0.25);
    
    scaleNoiseAmount = xml.getValue("scaleNoiseAmount", 1.5);
    scaleNoiseX = xml.getValue("scaleNoiseX", 1000.0);
    scaleNoiseY = xml.getValue("scaleNoiseY", 100.0);
    scaleNoiseZ = xml.getValue("scaleNoiseZ", 250.0);
    scaleNoiseSpeedX = xml.getValue("scaleNoiseSpeedX", 5.0);
    scaleNoiseSpeedY = xml.getValue("scaleNoiseSpeedY", 1.0);
    scaleNoiseSpeedZ = xml.getValue("scaleNoiseSpeedZ", 2.5);
    
    colorNoiseAmount = xml.getValue("colorNoiseAmount", 0.2);
    colorNoiseX = xml.getValue("colorNoiseX", 100.0);
    colorNoiseY = xml.getValue("colorNoiseY", 150.0);
    colorNoiseZ = xml.getValue("colorNoiseXZ", 125.0);
    colorNoiseSpeedX = xml.getValue("colorNoiseSpeedX", 2.0);
    colorNoiseSpeedY = xml.getValue("colorNoiseSpeedY", 0.3);
    colorNoiseSpeedZ = xml.getValue("colorNoiseSpeedZ", 1.4);
    
    enableSSAO = xml.getValue("enableSSAO", true);
    ssaoWeight = xml.getValue("ssaoWeight", 1.);
    ssaoRadius = xml.getValue("ssaoRadius", 13.3);
    ssaoMinThresh = xml.getValue("ssaoMinThresh", 0.003);
    ssaoMaxThresh = xml.getValue("ssaoMaxThresh", 0.097);
    ssaoExponent = xml.getValue("ssaoExponent", 0.47);
    ssaoNumSamples = xml.getValue("ssaoNumSamples", 25);
    ssaoRayReflection = xml.getValue("ssaoRayReflection", true);
    
    enableShading = xml.getValue("enableShading", true);
    ambientAmount = xml.getValue("ambientAmount", 0.57);
    diffuseAmount = xml.getValue("diffuseAmount", 0.83);
    lightPos = ofVec3f(xml.getValue("lightPosX", 0.93), xml.getValue("lightPosY", -1.), xml.getValue("lightPosZ", -1));
    
    enableFilter = xml.getValue("enableFilter", false);
    filterVignetteAmount = xml.getValue("filterVignetteAmount", -1.0);
    filterVignetteSize = xml.getValue("filterVignetteSize", 500);
    
    useCone = xml.getValue("useCone", true);
    useSecondCone = xml.getValue("useSecondCone", true);
    useSphereInCone = xml.getValue("useSphereInCone", false);
    drawDirection = xml.getValue("drawDirection", false);
    particleSize = ofVec3f(xml.getValue("particleSizeX", 30), xml.getValue("particleSizeY", 4), xml.getValue("particleSizeZ", 4));
    particleSpeed = xml.getValue("particleSpeed", 0);
    
    enableCamera = xml.getValue("enableCamera", true);
    
    exportDepth = xml.getValue("exportDepth", true);
    drawDepth = xml.getValue("drawDepth", false);
    depthNear = xml.getValue("depthNear", 1.0);
    depthFar = xml.getValue("depthFar", 40.0);
    
    rotationPhase = xml.getValue("rotationPhase", 0.);
    rotationSpeed = xml.getValue("rotationSpeed", 0.);
    
    exportSize = ofVec2f(xml.getValue("exportSizeX", 1920), xml.getValue("exportSizeY", 1080));
    previewSize = ofVec2f(ofGetWidth(), ofGetHeight());
    
    showLogo = xml.getValue("exportDepth", true);
    logoColor[0] = xml.getValue("logoColorR", 255. / 255.);
    logoColor[1] = xml.getValue("logoColorG", 255. / 255.);
    logoColor[2] = xml.getValue("logoColorB", 255. / 255.);
    logoFontsize = xml.getValue("logoFontsize", 50);
    logoPos = ofVec3f(xml.getValue("logoPosX", 0), xml.getValue("logoPosY", 0), xml.getValue("logoPosZ", 0));
    
    saveSettingsButton = false;
    
    cam.loadCameraPosition();
}

//--------------------------------------------------------------
void testApp::saveSettings(){

    xml.setValue("camSpeedX", camSpeedX);
    xml.setValue("camSpeedY", camSpeedY);
    xml.setValue("camSpeedZ", camSpeedZ);
    xml.setValue("waveInDuration", waveInDuration);
    xml.setValue("waveOutDuration", waveOutDuration);
    
    xml.setValue("baseColorRotationAngle", baseColorRotationAngle);
    
    xml.setValue("baseColorR", baseColor[0]);
    xml.setValue("baseColorG", baseColor[1]);
    xml.setValue("baseColorB", baseColor[2]);
    
    xml.setValue("baseColor2R", baseColor2[0]);
    xml.setValue("baseColor2R", baseColor2[1]);
    xml.setValue("baseColor2R", baseColor2[2]);
    
    xml.setValue("backgroundColorRotationAngle", backgroundColorRotationAngle);
    xml.setValue("backgroundBottomColorR", backgroundBottomColor[0]);
    xml.setValue("backgroundBottomColorG", backgroundBottomColor[1]);
    xml.setValue("backgroundBottomColorB", backgroundBottomColor[2]);
    
    xml.setValue("backgroundTopColorR", backgroundTopColor[0]);
    xml.setValue("backgroundTopColorG", backgroundTopColor[1]);
    xml.setValue("backgroundTopColorB", backgroundTopColor[2]);
    
    xml.setValue("directionFactor", directionFactor);
    
    xml.setValue("numRows", numRows);
    xml.setValue("numCols", numCols);
    xml.setValue("rowSpacing", rowSpacing);
    xml.setValue("colSpacing", colSpacing);
    
    xml.setValue("noiseAmount", noiseAmount);
    xml.setValue("noiseX", noiseX);
    xml.setValue("noiseY", noiseY);
    xml.setValue("noiseZ", noiseZ);
    xml.setValue("noiseSpeedX", noiseSpeedX);
    xml.setValue("noiseSpeedY", noiseSpeedY);
    xml.setValue("noiseSpeedZ", noiseSpeedZ);
    
    xml.setValue("scaleNoiseAmount", scaleNoiseAmount);
    xml.setValue("scaleNoiseX", scaleNoiseX);
    xml.setValue("scaleNoiseY", scaleNoiseY);
    xml.setValue("scaleNoiseZ", scaleNoiseZ);
    xml.setValue("scaleNoiseSpeedX", scaleNoiseSpeedX);
    xml.setValue("scaleNoiseSpeedY", scaleNoiseSpeedY);
    xml.setValue("scaleNoiseSpeedZ", scaleNoiseSpeedZ);
    
    xml.setValue("colorNoiseAmount", colorNoiseAmount);
    xml.setValue("colorNoiseX", colorNoiseX);
    xml.setValue("colorNoiseY", colorNoiseY);
    xml.setValue("colorNoiseXZ", colorNoiseZ);
    xml.setValue("colorNoiseSpeedX", colorNoiseSpeedX);
    xml.setValue("colorNoiseSpeedY", colorNoiseSpeedY);
    xml.setValue("colorNoiseSpeedZ", colorNoiseSpeedZ);
    
    xml.setValue("enableSSAO", enableSSAO);
    xml.setValue("ssaoWeight", ssaoWeight);
    xml.setValue("ssaoRadius", ssaoRadius);
    xml.setValue("ssaoMinThresh", ssaoMinThresh);
    xml.setValue("ssaoMaxThresh", ssaoMaxThresh);
    xml.setValue("ssaoExponent", ssaoExponent);
    xml.setValue("ssaoNumSamples", ssaoNumSamples);
    xml.setValue("ssaoRayReflection", ssaoRayReflection);
    
    xml.setValue("enableShading", enableShading);
    xml.setValue("ambientAmount", ambientAmount);
    xml.setValue("diffuseAmount", diffuseAmount);
    ofVec3f(xml.setValue("lightPosX", lightPos.x), xml.setValue("lightPosY", lightPos.y), xml.setValue("lightPosZ", lightPos.z));
    
    xml.setValue("enableFilter", enableFilter);
    xml.setValue("filterVignetteAmount", filterVignetteAmount);
    xml.setValue("filterVignetteSize", filterVignetteSize);
    
    xml.setValue("useCone", useCone);
    xml.setValue("useSecondCone", useSecondCone);
    xml.setValue("useSphereInCone", useSphereInCone);
    xml.setValue("drawDirection", drawDirection);
    ofVec3f(xml.setValue("particleSizeX", particleSize.x), xml.setValue("particleSizeY", particleSize.y), xml.setValue("particleSizeZ", particleSize.z));
    xml.setValue("particleSpeed", particleSpeed);
    
    xml.setValue("enableCamera", enableCamera);
    
    xml.setValue("exportDepth", exportDepth);
    xml.setValue("drawDepth", drawDepth);
    xml.setValue("depthNear", depthNear);
    xml.setValue("depthFar", depthFar);
    
    xml.setValue("rotationPhase", rotationPhase);
    xml.setValue("rotationSpeed", rotationSpeed);
    
    ofVec2f(xml.setValue("exportSizeX", exportSize.x), xml.setValue("exportSizeY", exportSize.y));
    
    xml.setValue("exportDepth", showLogo);
    xml.setValue("logoColorR", logoColor[0]);
    xml.setValue("logoColorG", logoColor[1]);
    xml.setValue("logoColorB", logoColor[2]);
    xml.setValue("logoFontsize", logoFontsize);
    ofVec3f(xml.setValue("logoPosX", logoPos.x), xml.setValue("logoPosY", logoPos.y), xml.setValue("logoPosZ", logoPos.z));
    
    xml.saveFile();
    
    cam.saveCameraPosition();
}

//--------------------------------------------------------------
void testApp::setupExportFrameBuffer(ofVec2f _exportSize){
    exportSize = _exportSize;
    
    ofFbo::Settings settings;
    settings.width = exportSize.x;
    settings.height = exportSize.y;
    settings.numSamples = 4;
    settings.useDepth = true;
    exportFbo.allocate(settings);
    
    ssao.setup(exportSize.x, exportSize.y, GL_RGBA16F);
}

//--------------------------------------------------------------
void testApp::setupPreviewFrameBuffer(ofVec2f _previewSize){
    previewSize = _previewSize;
    ofFbo::Settings settings;
    settings.width = _previewSize.x;
    settings.height = _previewSize.y;
    settings.numSamples = 4;
    settings.useDepth = true;
    previewFbo.allocate(settings);
}

//--------------------------------------------------------------
void testApp::loadShaders(){
    lighting.load("shaders/lighting");
    antiAliasShader.load("shaders/aa");
    filter.load("shaders/filter");
    depthMapShader.load("shaders/depthBuffer");
}


#pragma mark - Update

//--------------------------------------------------------------
void testApp::update(){
    rebuildGrid();
    adjustGridSpacing();
    adjustScale();
    updateCamera();
    updateSSAO();
    updateWaveState();
    updateLogo();
    updateGradient();
    updatePreviewSize();
    
    rotationPhase += rotationSpeed;
    
    static ofVec3f posNoiseOffset = ofVec3f(0);
    posNoiseOffset += ofVec3f(noiseSpeedX, noiseSpeedY, noiseSpeedZ);
    
    static ofVec3f scaleNoiseOffset = ofVec3f(0);
    scaleNoiseOffset += ofVec3f(scaleNoiseSpeedX, scaleNoiseSpeedY, scaleNoiseSpeedZ);
    
    
    float mostLeft, mostRight;
    findBounds(&mostLeft, &mostRight);
    
    for (int i = 0; i < particles.size(); i++) {
        updateParticle(particles[i], mostLeft, mostRight, posNoiseOffset, scaleNoiseOffset);
    }
    
    bar.update();
    if(saveSettingsButton){
        saveSettingsButton = false;
        saveSettings();
    }
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    if(enableRecording){
        saveFrame("framedata/frames");
    }
}

//--------------------------------------------------------------
void testApp::rebuildGrid(){
    // check if the number of rows or the number of cols changed
    // if so, delete all particles and rebuild the grid
    static int lastNumRows = 0;
    static int lastNumCols = 0;
    if(lastNumRows != numRows || lastNumCols != numCols){
        lastNumRows = numRows;
        lastNumCols = numCols;
        
        for (int i = 0; i < particles.size(); i++) {
            delete particles[i];
        }
        particles.clear();
        
        ofVec3f currentPoint = ofVec3f(0);
        for (int x = 0; x < numCols; x++) {
            currentPoint.z = 0;
            
            for (int y = 0; y < numRows; y++) {
                Particle *p = new Particle();
                p->setup(currentPoint + ofVec3f(0, 0, ofRandom(-10, 10)));
                particles.push_back(p);
                
                currentPoint.z += rowSpacing;
            }
            
            currentPoint.x += colSpacing;
        }
    }
}

//--------------------------------------------------------------
void testApp::adjustGridSpacing(){
    // check if the spacing has changed
    // if so, adjust it on the existing grid
    static float lastRowSpacing = rowSpacing;
    static float lastColSpacing = colSpacing;
    if(lastRowSpacing != rowSpacing || lastColSpacing != colSpacing){
        lastRowSpacing = rowSpacing;
        lastColSpacing = colSpacing;
        
        ofVec3f currentPoint = ofVec3f(0);
        for (int x = 0; x < numCols; x++) {
            currentPoint.z = 0;
            
            for (int y = 0; y < numRows; y++) {
                int index = x * numRows + y;
                Particle *p = particles[index];
                p->basePos = currentPoint + ofVec3f(0, 0, ofRandom(-10, 10));
                currentPoint.z += rowSpacing;
            }
            
            currentPoint.x += colSpacing;
        }
    }
}

//--------------------------------------------------------------
void testApp::adjustScale(){    
    // we only have to reallocate the preview fbo if the previewSize changed
    static ofVec2f lastPreviewSize = previewSize;
    if(previewSize != lastPreviewSize){
        lastPreviewSize = previewSize;
        setupPreviewFrameBuffer(previewSize);
    }
}

//--------------------------------------------------------------
void testApp::updateCamera(){
    cam.applyRotation = enableCamera;
    cam.applyTranslation = enableCamera;
    
    cam.targetNode.truck(camSpeedX);
    cam.targetNode.boom(camSpeedY);
    cam.targetNode.dolly(camSpeedZ);
}

//--------------------------------------------------------------
void testApp::updateSSAO(){
    float exportScale = MAX(exportSize.x / 1280., exportSize.y / 720.);
    ssao.setWeight(ssaoWeight * exportScale);
    ssao.setRadius(ssaoRadius * exportScale);
    ssao.setMaxThreshold(ssaoMaxThresh * exportScale);
    ssao.setMinThreshold(ssaoMinThresh * exportScale);
    ssao.setExponent(ssaoExponent);
    ssao.setNumSamples(ssaoNumSamples);
    ssao.setRayReflection(ssaoRayReflection);
}

//--------------------------------------------------------------
void testApp::updateWaveState(){
    // check if any of the waves (in or out) has started or ended
    // update the startframes accordingly
    if(waveIn && waveOutStartFrame == -1){
        waveIn = false;
        waveInStartFrame = ofGetFrameNum();
    }
    if(ofGetFrameNum() - waveInStartFrame >= waveInDuration){
        waveInStartFrame = -1;
    }
    if(waveOut && waveInStartFrame == -1){
        waveOut = false;
        waveOutStartFrame = ofGetFrameNum();
    }
    if(ofGetFrameNum() - waveOutStartFrame >= waveOutDuration){
        waveOutStartFrame = -1;
    }
}

//--------------------------------------------------------------
void testApp::updateLogo(){
    static int lastLogoFontSize = 0;
    if(lastLogoFontSize != logoFontsize) {
        logoFont.loadFont(LOGO_FONT_FILE, logoFontsize, true, false, true);
        float stringWidth = logoFont.stringWidth("CODED MATTER(S)");
        float stringHeight = logoFont.stringHeight("CODED MATTER(S)");
        logoPos = ofVec3f((exportSize.x - stringWidth) / 2., (exportSize.y - stringHeight) / -2., 0);
        lastLogoFontSize = logoFontsize;
    }
}

//--------------------------------------------------------------
void testApp::updateGradient(){
    static float lastBaseColorRotationAngle = -1;
    if(lastBaseColorRotationAngle != baseColorRotationAngle){
        lastBaseColorRotationAngle = baseColorRotationAngle;
        
        // we calculate a factor for every corner, depending on the angle
        // we create a normalized vector for the given angle
        ofVec2f angleVector = ofVec2f(cos( ofDegToRad(baseColorRotationAngle) ), sin( ofDegToRad(baseColorRotationAngle) )).normalize();
        
        // now we create a vector for every corner, pointing to the center
        float angle = M_PI_4 * 5.;
        ofVec2f topLeftVector = ofVec2f(cos(angle), sin(angle)).normalize();
        
        angle = M_PI_4 * 7.;
        ofVec2f topRightVector = ofVec2f(cos(angle), sin(angle)).normalize();
        
        angle = M_PI_4 * 1.;
        ofVec2f bottomRightVector = ofVec2f(cos(angle), sin(angle)).normalize();
        
        angle = M_PI_4 * 3.;
        ofVec2f bottomLeftVector = ofVec2f(cos(angle), sin(angle)).normalize();
        
        // now, for every corner calculate a factor. completely aligning with the angleVector means a factor of 1, being the opposite means a factor of 0
        float topLeftAngle = fabs(angleVector.angleRad(topLeftVector));
        float topRightAngle = fabs(angleVector.angleRad(topRightVector));
        float bottomRightAngle = fabs(angleVector.angleRad(bottomRightVector));
        float bottomLeftAngle = fabs(angleVector.angleRad(bottomLeftVector));
        
        // we map the values from 0 to 1
        baseColorTopLeftFactor = ofMap(topLeftAngle, 0, M_PI, 1, 0);
        baseColorTopRightFactor = ofMap(topRightAngle, 0, M_PI, 1, 0);
        baseColorBottomRightFactor = ofMap(bottomRightAngle, 0, M_PI, 1, 0);
        baseColorBottomLeftFactor = ofMap(bottomLeftAngle, 0, M_PI, 1, 0);
        
        // find the smallest and largest values
        float largestFactor = MAX(MAX(MAX(baseColorTopLeftFactor, baseColorTopRightFactor), baseColorBottomRightFactor), baseColorBottomLeftFactor);
        float smallestFactor = MIN(MIN(MIN(baseColorTopLeftFactor, baseColorTopRightFactor), baseColorBottomRightFactor), baseColorBottomLeftFactor);
        
        // normalize the values
        baseColorTopLeftFactor = ofMap(baseColorTopLeftFactor, smallestFactor, largestFactor, 0, 1);
        baseColorTopRightFactor = ofMap(baseColorTopRightFactor, smallestFactor, largestFactor, 0, 1);
        baseColorBottomRightFactor = ofMap(baseColorBottomRightFactor, smallestFactor, largestFactor, 0, 1);
        baseColorBottomLeftFactor = ofMap(baseColorBottomLeftFactor, smallestFactor, largestFactor, 0, 1);
        
    }
}

//--------------------------------------------------------------
void testApp::updatePreviewSize(){
    if(setExportSizeToA2){
        setExportSizeToA2 = false;
        exportSize = ofVec2f(5291, 7483);
    }
    if(setExportSizeToA5){
        setExportSizeToA5 = false;
        exportSize = ofVec2f(2039, 2894);
    }
    if(setExportSizeToVideo){
        setExportSizeToVideo = false;
        exportSize = ofVec2f(1920, 1080);
    }
}

//--------------------------------------------------------------
void testApp::findBounds(float *mostLeftOut, float *mostRightOut, float *mostBackOut, float *mostFrontOut){
    // find out what the leftmost and rightmost positions in the grid are
    // particles can move so we have to keep checking this
    float mostLeft = INFINITY;
    float mostRight = -INFINITY;
    float mostBack = INFINITY;
    float mostFront = -INFINITY;
    
    for (int i = 0; i < particles.size(); i++) {
        if(particles[i]->pos.x < mostLeft)
            mostLeft = particles[i]->pos.x;
        if(particles[i]->pos.x > mostRight)
            mostRight = particles[i]->pos.x;
        
        if(particles[i]->pos.z < mostBack)
            mostBack = particles[i]->pos.z;
        if(particles[i]->pos.z > mostFront)
            mostFront = particles[i]->pos.z;
    }
    
    if(mostLeftOut != NULL)
        *mostLeftOut = mostLeft;
    
    if(mostRightOut != NULL)
        *mostRightOut = mostRight;

    if(mostBackOut != NULL)
        *mostBackOut = mostBack;

    if(mostFrontOut != NULL)
    *mostFrontOut = mostFront;
}

//--------------------------------------------------------------
void testApp::updateParticle(Particle *p, float mostLeft, float mostRight, ofVec3f posNoiseOffset, ofVec3f scaleNoiseOffset){
    // transfer state variables from main application to individual particles
    p->directionFactor = directionFactor;
    p->useCone = useCone;
    p->useSecondCone = useSecondCone;
    p->useSphereInCone = useSphereInCone;
    p->drawDirection = drawDirection;
    p->size = particleSize;
    p->speed = particleSpeed;
    
    ofVec3f pos = p->basePos;
    ofVec3f center = ofVec3f(numCols * colSpacing / 2., 0, numRows * rowSpacing / 2.);
    float radius = pos.z - center.z;
    pos = ofVec3f(pos.x, sin(rotationPhase) * radius, cos(rotationPhase) * radius) + center;
    
    // if a wave is in progress, update the scaleMultiplier
    if(waveInStartFrame != -1){
        // the progression of the wave
        // we scale this to [-1, 1] so it will take a bit of time before the wave starts
        float factor = ofMap((ofGetFrameNum() - waveInStartFrame) / (float)waveInDuration, 0, 1, -1, 1);
        
        // the x-position scaled to [0, 1] relative to the grid
        float xFactor = ofMap(pos.x, mostLeft, mostRight, 0, 1);
        
        // the two factors combined and clamped
        float waveScaleFactor = MIN(MAX(factor + xFactor, 0), 1);
        p->scaleMultiplier = waveScaleFactor;
        
        // if we're done, dont wait for the time to pass by but stop the wave
        if(xFactor == 0 && waveScaleFactor == 1)
            waveInStartFrame = -1;
    }
    if(waveOutStartFrame != -1){
        float factor = ofMap((ofGetFrameNum() - waveOutStartFrame) / (float)waveOutDuration, 0, 1, 2, -2);
        float xFactor = ofMap(pos.x, mostLeft, mostRight, 0, -1);
        float waveScaleFactor = MIN(MAX(factor + xFactor, 0), 1);
        p->scaleMultiplier = waveScaleFactor;
        
        if(xFactor == 0 && waveScaleFactor == 0)
            waveOutStartFrame = -1;
    }
    
    // generate a perlin noise value for the current position
    float noiseValue = ofNoise((pos.x + posNoiseOffset.x) / noiseX, (pos.y + posNoiseOffset.y) / noiseY, (pos.z + posNoiseOffset.z) / noiseZ);
    ofVec3f noiseVec = ofVec3f(0, noiseValue * noiseAmount, 0);
    // calculate a new position out of this
    ofVec3f newPos = pos + noiseVec;
    
    // do the same for the next position, so we can determine direction
    ofVec3f nextPos = pos + ofVec3f(colSpacing, 0, 0);
    float nextNoiseValue = ofNoise((nextPos.x + posNoiseOffset.x) / noiseX, (nextPos.y + posNoiseOffset.y) / noiseY, (nextPos.z + posNoiseOffset.z) / noiseZ);
    ofVec3f nextNoiseVec = ofVec3f(0, nextNoiseValue * noiseAmount, 0);
    nextPos += nextNoiseVec;
    
    // directional vector from current to next position
    ofVec3f dir = nextPos - newPos;
    p->pos = newPos;
    p->dir = dir;
    
    // use noise value for scaling the particle
    float scaleNoiseValue = ofNoise((pos.x + scaleNoiseOffset.x) / scaleNoiseX, (pos.y + scaleNoiseOffset.y) / scaleNoiseY, (pos.z + scaleNoiseOffset.z) / scaleNoiseZ);
    p->scale = scaleNoiseValue * scaleNoiseAmount;
    
    p->update();
}

#pragma mark - Draw

//--------------------------------------------------------------
void testApp::draw(){
    // we only want to draw particles that are in screen
//    vector<Particle *> visibleParticles = findVisibleParticles();
    vector<Particle *> visibleParticles = particles;
    previewFbo.begin();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofClear(0);
    
    bool ssaoWasEnabled = enableSSAO;
    bool shadingWasEnabled = enableShading;
    if(drawDepth){
        enableSSAO = false;
        enableShading = false;
        depthMapShader.begin();
        depthMapShader.setUniform1f("near", depthNear);
        depthMapShader.setUniform1f("far", depthFar);
        glEnable(GL_DEPTH_TEST);
    }
    else{
        drawBackground(previewSize.x, previewSize.y);
    }
    
    if(showLogo){
        ofPushMatrix();
        float xScale = previewSize.x / exportSize.x;
        float yScale = previewSize.y / exportSize.y;
        ofScale(xScale, yScale);
        drawLogo();
        ofPopMatrix();
    }
    
    enableSSAO = false;
    drawScene(visibleParticles, previewSize.x, previewSize.y);
    enableSSAO = ssaoWasEnabled;

    if(drawDepth){
        depthMapShader.end();
        enableSSAO = ssaoWasEnabled;
        enableShading = shadingWasEnabled;
    }
    
    previewFbo.end();
    
    // this filter is only for display
    // if it is enabled and we are exporting, it will be reapplied
    if(enableFilter){
        filter.begin();
        filter.setUniform2f("textureSize", previewFbo.getWidth(), previewFbo.getHeight());
        float exportScale = MAX(previewSize.x / 1280., previewSize.y / 720.);
        filter.setUniform1f("vignetteSize", filterVignetteSize * exportScale);
        filter.setUniform1f("vignetteAmount", filterVignetteAmount);
    }
    
    ofRectangle drawRectangle;
    
    if(exportSize.x/exportSize.y > (float)ofGetWidth() / (float)ofGetHeight()){
        float scalingFactor = (ofGetWidth() / exportSize.x);
        float width = ofGetWidth();
        float height = exportSize.y * scalingFactor;
        float offset = (ofGetHeight() - height) / 2.;
        previewSize.x = width;
        previewSize.y = height;
        previewFbo.draw(0, offset, width, height);
    }
    else {
        float scalingFactor = (ofGetHeight() / exportSize.y);
        float width = exportSize.x * scalingFactor;
        float height = ofGetHeight();
        float offset = (ofGetWidth() - width) / 2.;
        previewSize.x = width;
        previewSize.y = height;
        previewFbo.draw(offset, 0, width, height);
    }
    
    if(enableFilter)
        filter.end();
    
    // draw gui
    bar.draw();
}

//--------------------------------------------------------------
void testApp::drawBackground(float width, float height){
    ofPushMatrix();
    glDisable(GL_DEPTH_TEST);
    ofColor bottom = ofColor(backgroundBottomColor[0] * 255., backgroundBottomColor[1] * 255., backgroundBottomColor[2] * 255.);
    ofColor top = ofColor(backgroundTopColor[0] * 255., backgroundTopColor[1] * 255., backgroundTopColor[2] * 255.);
    
    float rotationAngle = backgroundColorRotationAngle;
    float alpha = ofDegToRad(90);
    float gamma = ofDegToRad(rotationAngle);
    
    if(rotationAngle > 90 && rotationAngle <= 180){
        gamma = M_PI_2 - (gamma - M_PI_2);
    }
    else if(rotationAngle > 270 && rotationAngle <= 360){
        gamma = M_PI_2 * 3.0 - (gamma - M_PI_2 * 3.0);
    }
    
    float beta = ofDegToRad(90) - gamma;
    
    float newHeight = (sin(beta) * height) + (sin(gamma) * width);
    float heightScaleFactor = fabs(newHeight / height);

    float newWidth = (sin(beta) * width) + (sin(gamma) * height);
    float widthScaleFactor = fabs(newWidth / width);
    
    ofTranslate(width / 2., height / 2.);
    ofRotateZ(rotationAngle);
    ofScale(widthScaleFactor, heightScaleFactor);
    ofTranslate(-width / 2., -height / 2.);
    ofBackgroundGradient(bottom, top, OF_GRADIENT_LINEAR, width, height);
    glEnable(GL_DEPTH_TEST);
    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::drawLogo(){
    ofPushMatrix();
    ofScale(1, -1);
    ofTranslate(logoPos);
    ofPushStyle();
    ofSetColor(logoColor[0] * 255., logoColor[1] * 255., logoColor[2] * 255.);
    logoFont.drawStringAsShapes("CODED MATTER(S)", 0, 0);
    ofPopStyle();
    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::drawScene(vector<Particle *>visibleParticles, float w, float h){
    
    // start ambient occlusion
    if(enableSSAO)
        ssao.begin();
    
    // by default, the camera uses the current _window_ size.
    // because we are scaling, we want the current viewport
    ofRectangle viewPort = ofGetCurrentViewport();
    cam.begin(viewPort);
    
    // make sure that the center of the particle-grid is at the center of the screen
    ofTranslate(-numCols * colSpacing / 2., -200., -numRows * rowSpacing / 2.);
    
    // regular shading
    if(enableShading){
        lighting.begin();
        lighting.setUniform1i("enableShading", enableShading);
        lighting.setUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
        lighting.setUniform1f("ambientAmount", ambientAmount);
        lighting.setUniform1f("diffuseAmount", diffuseAmount);
    }
    
    ofPushStyle();
    
    static ofVec3f colorNoiseOffset = ofVec3f(0);
    colorNoiseOffset += ofVec3f(colorNoiseSpeedX, colorNoiseSpeedY, colorNoiseSpeedZ);
    
    float mostLeft, mostRight, mostBack, mostFront;
    findBounds(&mostLeft, &mostRight, &mostBack, &mostFront);
    
    // draw the particles
    for (int i = 0; i < visibleParticles.size(); i++) {
        drawParticle(visibleParticles[i], mostLeft, mostRight, mostBack, mostFront, colorNoiseOffset);
    }
    
    if(enableShading)
        lighting.end();
    
    ofPopStyle();
    
    cam.end();
    
    // SSAO does not automatically draw when ended
    if(enableSSAO){
        ssao.end();
        ssao.draw(0, h, w, -h);
    }
}

//--------------------------------------------------------------
void testApp::drawParticle(Particle *p, float mostLeft, float mostRight, float mostBack, float mostFront, ofVec3f colorNoiseOffset){
    ofVec3f pos = p->pos;
    float colorNoiseValue = ofNoise((pos.x + colorNoiseOffset.x) / colorNoiseX, (pos.y + colorNoiseOffset.y) / colorNoiseY, (pos.z + colorNoiseOffset.z) / colorNoiseZ);
    
    // interpolate between the 2 base colors
    ofColor base = ofColor(baseColor[0] * 255., baseColor[1] * 255., baseColor[2] * 255.);
    ofColor base2 = ofColor(baseColor2[0] * 255., baseColor2[1] * 255., baseColor2[2] * 255.);
    
    // first, we interpolate 2 values with the x factor: topLeft - topRight and bottomLeft - bottomRight
    float xFactor = ofMap(pos.x, mostLeft, mostRight, 0, 1);
    float topX = ofLerp(baseColorTopLeftFactor, baseColorTopRightFactor, xFactor);
    float bottomX = ofLerp(baseColorBottomLeftFactor, baseColorBottomRightFactor, xFactor);
    
    // then, we interpolate the two resulting values with the z factor
    // we also clamp it to 0-1
    float zFactor = ofMap(pos.z, mostBack, mostFront, 0, 1);
    float interpolationValue = MIN(MAX(ofLerp(topX, bottomX, zFactor), 0), 1);
    
    base = base.lerp(base2, interpolationValue);
    
    // add color highlight
    float b = base.getBrightness();
    float s = base.getSaturation();
    base.setBrightness(b + 255. * colorNoiseValue * colorNoiseAmount);
    base.setSaturation(s + 255. * colorNoiseValue * colorNoiseAmount);

    ofSetColor(base);
    p->draw();
}

//--------------------------------------------------------------
vector<Particle *> testApp::findVisibleParticles(){
    // project all particles on the screen and check if they're visible
    vector<Particle *> visibleParticles;
    
    for (int i = 0; i < particles.size(); i++) {
        ofVec3f pos = particles[i]->pos;
        pos += ofVec3f(-numCols * colSpacing / 2., -200., -numRows * rowSpacing / 2.);
        
        ofVec2f projected = cam.worldToScreen(pos);
        float exportScale = MAX(exportSize.x / 1280., exportSize.y / 720.);
        float margin = 150 * exportScale;
        if(projected.x > -margin && projected.x < exportSize.x + margin &&
           projected.y > -margin && projected.y < exportSize.y + margin){
            visibleParticles.push_back(particles[i]);
        }
        
        // TODO: fix issue where items behind the camera are 'visible'
    }
    
    return visibleParticles;
}

#pragma mark - Export

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    // save a single frame if the user releases spacebar
    // if we did this in keyPressed(), it would be easy to save a lot of frames accidentally
    if(key == ' '){
        saveFrame("framedata/screenshots");
    }
}

//--------------------------------------------------------------
void testApp::saveFrame(string dir){
    if(drawDepth){
        drawDepth = false;
        draw();
    }

    // we have to allocate our exportFbo
    static ofVec2f lastExportSize = ofVec2f(0, 0);
    if(exportSize != lastExportSize){
        lastExportSize = exportSize;
        setupExportFrameBuffer(exportSize);
    }
    
    // save rgb frame
    exportFbo.begin();
    ofEnableAlphaBlending();
    ofClear(0);
    
    drawBackground(exportSize.x, exportSize.y);
    if(showLogo){
        drawLogo();
    }
    drawScene(particles, exportSize.x, exportSize.y);
    
    exportFbo.end();
    
    // antialiasing and filtering
    ofFbo::Settings settings;
    settings.width = exportSize.x;
    settings.height = exportSize.y;
    ofFbo filteredExportFbo;
    filteredExportFbo.allocate(settings);
    filteredExportFbo.begin();
    ofClear(0);
    
    float exportScale = MAX(exportSize.x / 1280., exportSize.y / 720.);
    if(enableFilter){
        filter.begin();
        filter.setUniform2f("textureSize", exportSize.x, exportSize.y);
        filter.setUniform1f("vignetteSize", filterVignetteSize * exportScale);
        filter.setUniform1f("vignetteAmount", filterVignetteAmount);
    }
    
    exportFbo.draw(0, exportSize.y, exportSize.x, -exportSize.y);
    
    if(enableFilter)
        filter.end();
    filteredExportFbo.end();
    
    // transfer to exportFBO, if needed with AA
    exportFbo.begin();
    ofClear(0);
    
    antiAliasShader.begin();
    filteredExportFbo.draw(0, 0);
    antiAliasShader.end();

    exportFbo.end();
    
    
    string rgbDir = dir + "/rgb/";
    ofDirectory data = ofDirectory(rgbDir);
    
    ofPixels pixels;

    int numTileRows = 5;
    int numTileCols = 5;
    int numTiles = numTileCols * numTileRows;
    int count = data.listDir() / (float)numTiles;
    ofVec2f tileSize = ofVec2f(exportSize.x / (float)numTileCols, exportSize.y / (float)numTileRows);
    
    for(int i = 0; i < numTileRows; i++){
        for (int j = 0; j < numTileCols; j++) {
            int x = j * tileSize.x;
            int y = i * tileSize.y;
            int w = tileSize.x;
            int h = tileSize.y;
            
            ofFbo::Settings settings;
            settings.width = w;
            settings.height = h;
            ofFbo tile;
            tile.allocate(settings);
            tile.begin();
            ofClear(0);
            exportFbo.draw(-x, -y, exportSize.x, exportSize.y);
            tile.end();
            tile.readToPixels(pixels);
            
            ofSaveImage(pixels, rgbDir + ofToString(count, 0) + "." + ofToString(j, 0) + "." + ofToString(i, 0) +".bmp");
        }
    }    
    
    // save depth map
    if(!exportDepth)
        return;

    // antialias the depth map
    exportFbo.begin();
    ofClear(0);
    antiAliasShader.begin();
    
    bool ssaoWasEnabled = enableSSAO;
    bool shadingWasEnabled = enableShading;
    enableSSAO = false;
    enableShading = false;
    depthMapShader.begin();
    depthMapShader.setUniform1f("near", depthNear);
    depthMapShader.setUniform1f("far", depthFar);
    glEnable(GL_DEPTH_TEST);
    drawScene(particles, exportSize.x, exportSize.y);
    depthMapShader.end();
    enableSSAO = ssaoWasEnabled;
    enableShading = shadingWasEnabled;
    
    antiAliasShader.end();
    exportFbo.end();
    string depthDir = dir + "/depth/";
    data = ofDirectory(depthDir);
    count = data.listDir() / (float)numTiles;
    
    for(int i = 0; i < numTileRows; i++){
        for (int j = 0; j < numTileCols; j++) {
            int x = j * tileSize.x;
            int y = i * tileSize.y;
            int w = tileSize.x;
            int h = tileSize.y;
            
            ofFbo::Settings settings;
            settings.width = w;
            settings.height = h;
            ofFbo tile;
            tile.allocate(settings);
            tile.begin();
            ofClear(0);
            exportFbo.draw(-x, -y, exportSize.x, exportSize.y);
            tile.end();
            tile.readToPixels(pixels);
            
            ofSaveImage(pixels, depthDir + ofToString(count, 0) + "." + ofToString(j, 0) + "." + ofToString(i, 0) +".bmp");
        }
    }
}


#pragma mark - Window size
void testApp::windowResized(int w, int h){
    
}