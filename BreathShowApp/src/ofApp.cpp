#include "ofApp.h"
#include "Fan.h"
#include "RectScreen.h"
#include "Ellipse.h"
#include "Util.h"
#include "Helper.h"
#include "io.h"

shared_ptr<ofApp> ofApp::app = nullptr;

void ofApp::setup()
{
//     ofSetLogLevel(OF_LOG_NOTICE);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetCircleResolution(64);
    
#ifndef USE_MULTI_VIEWPORT
    gui.setup(nullptr, false, ImGuiConfigFlags_DockingEnable, true);
#else
    // ImGuiConfigFlags_ViewportsEnable conflicts with ofxEasyCam controlArea
    gui.setup(nullptr, false, ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable, true);
#endif
    
    ofSetBackgroundAuto(false);
    ofEnableAlphaBlending();
    cam.setNearClip(1);
    cam.setFarClip(10000);
    
    currentProjectPath = "./projects/testProject";
    io::loadProject(currentProjectPath);
        
    osc.setupReceiver();
}

void ofApp::receiveOsc(){
    if(bUseTracker){
        osc.receive([&](const ofxOscMessage &m) -> void {
            string address = m.getAddress();
            const vector<string> &tokens = Util::split(address, "/");
            body.processOsc(m, tokens);
            //cout << m << endl;
        });
    }else{
        // Read sequencer to move Human
    }
}

void ofApp::update()
{
    ofSetFrameRate(app->targetFps);
    
    if(bUseTracker){
        receiveOsc();
        body.calc();

        human.root.setPosition(body.rootPos);
        
        // orientation
        human.orientationY = body.rootOri.y;
        glm::quat q = glm::angleAxis(glm::radians(human.orientationY.get()), vec3(0,1,0));
        human.root.setOrientation(q);
    }else{
        /// Read Sequencer?
    }
    
    const auto & items = sequencer.getSequenceItems();
    for( auto & i : items){
        i.shape->update();
    }
}

void ofApp::draw()
{
    drawProjectorFbo();

    ofBackground(0);
    gui.begin();
    
    gui::Helper::dock(app);
    drawGui();
     
    gui.end();
    gui.draw();
    
    // update sequencer at last as video needs time to update texture
    sequencer.update();
}

ofFbo::Settings ofApp::getFboSettings(int w, int h, bool bUseDepth, GLint glFormat, GLenum glTexTarget) {
    ofFbo::Settings fboSettings;
    fboSettings.width = w;
    fboSettings.height = h;
    fboSettings.numSamples = 8;
    fboSettings.internalformat = glFormat;
    fboSettings.textureTarget = glTexTarget;
    fboSettings.useDepth = bUseDepth;
    return fboSettings;
}

void ofApp::setPlay( bool b){
    bPlay = b;
    const auto & items = sequencer.getSequenceItems();
    for( auto & i : items){
        i.shape->pause(!bPlay);
    }
    
    if(bPlay){
        sequencer.updateSequenceItemAll(true);
    }
}

void ofApp::setLoop( bool b){
    bLoop = b;
    const auto & items = sequencer.getSequenceItems();
    for( auto & i : items){
        i.shape->setLoop(bLoop);
    }
}

void ofApp::keyPressed(ofKeyEventArgs & args)
{
    switch(args.key){
        case 's':
            if(ofGetKeyPressed(OF_KEY_CONTROL) || ofGetKeyPressed(OF_KEY_COMMAND)){
                if(ofGetKeyPressed(OF_KEY_SHIFT)){
                    io::dialogueSaveProject();
                }else{
                    io::saveProject(currentProjectPath);
                }
            }
            break;
        case 'o':
            if(ofGetKeyPressed(OF_KEY_CONTROL) || ofGetKeyPressed(OF_KEY_COMMAND)){
                io::dialogueLoadProject();
            }
            break;
            
        case ' ':
            setPlay(!bPlay);
            break;
    }
}

void ofApp::mousePressed(ofMouseEventArgs & args){

}

void ofApp::exit(){
    
//    ofGetCurrentRenderer().reset();
//    //mainWindow->ofBaseWindow->setWindowShouldClose();
//    projectorWindow->ofBaseWindow->setWindowShouldClose();
//    ofGetCurrentWindow()->close();

    ofLogNotice("ofApp") << "exit()";
    
}
