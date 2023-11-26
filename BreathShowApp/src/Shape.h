#pragma once

#include "ofMain.h"

using glm::vec2;
using glm::vec3;

class Shape{
    
public:
    
    Shape(){
//        ofTexture & tex = img.getTexture();
//        tex.bind();
//        mesh.draw();
//        tex.unbind();
    }
    
    virtual ~Shape(){};

    virtual void setup() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
    
    void loadTestImage(){
        bool ok = img.load("./tex/test2.jpg");
        if(ok){
            ofLogNotice("Fan") << "OK loading texture";
        }else{
            ofLogError("Fan") << "Can not load texture";
        }
    }
    
    void loadVideo(string path){

        if( ofFile::doesFileExist(path)){
            vid.load(path);
            vid.setUseTexture(true);
            vid.setLoopState(OF_LOOP_NORMAL);
            vid.play();
            ofLogNotice("Fan") << "Video file loaded: ";
        }else{
            ofLogError("Fan") << "Video file does not exist: " << path;
        }
    }

    void pause(bool b){
        vid.setPaused(b);
    }

    void setLoop(bool b){
        if(b){
            vid.setLoopState(OF_LOOP_NORMAL);
        }else{
            vid.setLoopState(OF_LOOP_NONE);
        }
    }
    
    void start(int frame = 0){ 
        if(!vid.isPlaying()){
            vid.play();
        }
        
        if(vid.getCurrentFrame() != frame){
            vid.setFrame(frame);
        }
        
        bOn = true;
    }
    
    void stop(){
        if(vid.isPlaying()){
            vid.setPosition(0);
            vid.stop();
        }
        bOn = false;
    }

    ofParameter<bool> bOn{"On", true};
    ofParameter<bool> bFlipX{"Flip X", false};
    ofParameter<bool> bShowTest{"test texture", false};
    ofParameter<vec3> position{"position", vec3(5,36,0), vec3(-100), vec3(100)};
    ofParameter<float> orientationY{"orientationY", 0, -180, 180};

    ofParameterGroup grp{"Fun", bOn, bShowTest, position, orientationY };
    
protected:
    ofMesh mesh;

    ofImage img;
    ofVideoPlayer vid;
};
