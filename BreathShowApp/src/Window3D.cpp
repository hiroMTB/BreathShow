#include "ofApp.h"
#include "Helper.h"

void ofApp::draw3DWindow(){
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    
    auto settings = ofxImGui::Settings();
    ImGuiWindowFlags flags = ImGuiWindowFlags_None;
    if(ofxImGui::BeginWindow("3D Scene", settings, false, (bool*)&b3dSceneOpen.get())){
        
        ofRectangle area = gui::Helper::calcCameraControlArea();
        if(area.width == 0 && area.height == 0){
            cam.disableMouseInput();
            cam.clearControlArea();
        }else{
            cam.setControlArea(area);
            cam.enableMouseInput();
        }
        
        // Allocate FBO on demand
        bool bAllocated = fbo3dScene.isAllocated();
        int fboW = fbo3dScene.getWidth();
        int fboH = fbo3dScene.getHeight();
        int winW = area.width;
        int winH = area.height;
        bool bNeedResize = fboW != winW || fboH != winH;
        
        if (!bAllocated || bNeedResize) {
            ofFbo::Settings fboSettings = getFboSettings(winW, winH, true);
            fboSettings.numSamples = ofFbo::maxSamples();
            fbo3dScene.allocate(fboSettings);
        }
        
        fbo3dScene.begin();
        draw3DScene();
        fbo3dScene.end();
        ofxImGui::AddImage(fbo3dScene.getTexture(), ofVec2f(fbo3dScene.getWidth(), fbo3dScene.getHeight()));
    }else{
        cam.disableMouseInput();
    }
    
    ofxImGui::EndWindow(settings);
    ImGui::PopStyleVar();
}

void ofApp::draw3DScene(){
    ofEnableAlphaBlending();
    ofDisableAntiAliasing();
    ofClear(0,0,0,0);
    ofBackground(50);
    cam.begin();
    
    ofEnableDepthTest();
    
    ofDrawAxis(100);

    // Floor (Grid)
    drawFloor();

//    ofPushMatrix();
//    ofTranslate(0, 100, 0);
//    drawFloor();
//    ofPopMatrix();
//
//    ofPushMatrix();
//    ofTranslate(0, 200, 0);
//    drawFloor();
//    ofPopMatrix();
    
    human.draw();

    if(bUseTracker){
    	body.draw(2, ofColor(255));
    }
    
    // Fan, RectScreen, Ellipse
    const auto & items = sequencer.getSequenceItems();
    for( auto & i : items){
        i.shape->draw();
    }
    
    // Projector
    projector.drawProjector();
    
    ofDisableDepthTest();
    cam.end();
}

void ofApp::drawProjectorFbo(){
        
    projector.begin();
    
    ofClear(0);
    ofBackground(0);
    
    // Fan
    ofSetColor(255);
    ofPushMatrix();
    const auto & items = sequencer.getSequenceItems();
    for( auto & i : items){
        i.shape->draw();
    }
    ofPopMatrix();
    
    projector.end();
}

void ofApp::drawFloor(){
    ofPushMatrix();
    ofRotateZDeg(90);
    ofSetColor(180);
    ofDrawGridPlane(25, 4*2);   // 25cm, 2m x 2m grid
    ofPopMatrix();
}
