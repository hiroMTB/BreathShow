#include "ofApp.h"

void ofApp::draw3DWindow(){
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    
    auto settings = ofxImGui::Settings();
    ImGuiWindowFlags flags = ImGuiWindowFlags_None;
    if(ofxImGui::BeginWindow("3D Scene", settings, false, (bool*)&b3dSceneOpen.get())){
        
        ofRectangle area = calcCameraControArea();
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
    
    //
    ofDrawAxis(100);

    // Grid
    ofPushMatrix();
    ofRotateZDeg(90);
    ofSetColor(180);
    ofDrawGridPlane(10, 10);
    ofPopMatrix();
    
    // human
    ofSetColor(180);
    ofPushMatrix();
    human.drawWireframe();
    ofPopMatrix();
    
    // Fan
    ofPushMatrix();
    fanL.draw();
    fanR.draw();
    ofPopMatrix();
    
    // Projector
    projector.drawProjector();
    cam.end();
}

void ofApp::drawProjectorFbo(){
    
    ofSetBackgroundColor(0);
    
    projector.begin();
    
    // Grid
    ofPushMatrix();
    ofRotateZDeg(90);
    ofSetColor(180);
    ofDrawGridPlane(10, 10);
    ofPopMatrix();
    
    // human
    ofSetColor(180);
    ofPushMatrix();
    human.drawWireframe();
    ofPopMatrix();
    
    // Fan
    ofSetColor(255);
    ofPushMatrix();
    fanL.draw();
    fanR.draw();
    ofPopMatrix();
    
    projector.end();
}

void ofApp::drawProjector(ofEventArgs & args){
    
//    int w = ofGetWindowWidth();
//    int h = ofGetWindowHeight();
    ofRectangle v1 = ofGetCurrentViewport();
    ofRectangle v2 = projector.getViewport();
    v2.scaleTo(v1);
    projector.draw(v2.x, v2.y, v2.width, v2.height);
}
