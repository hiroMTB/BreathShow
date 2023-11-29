//
// Created by hiroshi on 22/11/11.
//

#include "ofMain.h"
#include "ofApp.h"
#include "RenderWindow.h"
#include "Projector.h"

void ProjectorWindow::draw(ofEventArgs & args) {

    //app->drawProjectorFbo();
    
    ofClear(0, 0, 0, 0);
    ofBackground(30,255);
    ofPushMatrix();
    ofScale(scale);

    Projector & projector = app->projector;
    ofRectangle v1 = ofGetCurrentViewport();
    ofRectangle v2 = projector.getViewport();
    v2.scaleTo(v1);
    
    vec2 offset = projector.fboOffsetPosition;
    projector.fbo.setAnchorPoint(-offset.x, offset.y);
    
    //projector.draw(x, 0, w, h);
    projector.draw(v2.x, v2.y, v2.width, v2.height);

    ofPopMatrix();
}

void ProjectorWindow::keyPressed(ofKeyEventArgs & args) {
    ofLogNotice("ofApp") << "keyPressedInProjectorWindow() " << args.key;

    switch(args.key){
        default:
            app->keyPressed(args);
            break;
    }
}
