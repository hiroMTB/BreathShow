//
// Created by hiroshi on 22/11/11.
//

#include "ofMain.h"
#include "ofApp.h"
#include "RenderWindow.h"
#include "Projector.h"

void ProjectorWindow::draw(ofEventArgs & args) {

    ofClear(0, 0, 0, 0);
    ofBackground(30,255);
    ofPushMatrix();
    ofScale(scale);
    int x = 0;

    Projector & projector = app->projector;

    {
        //vec2 size = ofGetWindowSize();
        int w = projector.fbo.getWidth();
        int h = projector.fbo.getHeight();
        ofSetColor(0);
        ofFill();
        ofDrawRectangle(x,0,w, h);

        ofSetColor(255);
        {
            vec2 offset = projector.fboOffsetPosition;
            projector.fbo.setAnchorPoint(-offset.x, offset.y);
            projector.draw(x, 0, w, h);
        }
        x += w;
    }
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
