//
//  RectScreen.cpp
//

#include "RectScreen.h"

RectScreen::RectScreen(){
    
    setup(size.get().x, size.get().y);
    
    bool ok = img.load("./tex/test2.jpg");
    if(ok){
        ofLogNotice("Rect") << "OK loading texture";
    }else{
        ofLogError("Rect") << "Can not load texture";
    }
}


void RectScreen::setup(float w, int h){
    plane.set( w, h, 2, 2, OF_PRIMITIVE_TRIANGLE_STRIP );
}


void RectScreen::loadVideo(string path){

    if( ofFile::doesFileExist(path)){
        vid.load(path);
        vid.setUseTexture(true);
        vid.setLoopState(OF_LOOP_NORMAL);
        vid.play();
        ofLogNotice("RectScreen") << "Video file loaded: ";
    }else{
        ofLogError("RectScreen") << "Video file does not exist: " << path;
    }
}

void RectScreen::update(){
    if(bOn){
        setup(size.get().x, size.get().y); // TODO: Stop calling this
        vid.update();
    }
}

void RectScreen::draw(){
    if(bOn){
        
        ofPushMatrix();
        ofTranslate(position);
        //ofTranslate(scale);
        if(bShowTest){
            plane.mapTexCoords(0, 0, 1, 1);
            const ofMesh & mesh = plane.getMesh();
            ofTexture & tex = img.getTexture();
            tex.bind();
            mesh.draw();
            tex.unbind();
        }else{
            if(vid.isLoaded()){
                ofTexture & tex = vid.getTexture();
                const ofMesh & mesh = plane.getMesh();
                plane.mapTexCoords(0, 0, 1, 1);
                tex.bind();
                mesh.draw();
                tex.unbind();
            }
        }
        ofPopMatrix();
    }
}

void RectScreen::setPlayVideo(bool b){
    vid.setPaused(b);
}
