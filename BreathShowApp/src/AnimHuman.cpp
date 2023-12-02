//
//  AnimHuman.cpp
//

#include "AnimHuman.h"
#include "ofApp.h"
#include "Human.h"

void AnimHuman::applyTransformation(){
    
    shared_ptr<ofApp> & app = ofApp::get();
    Human & human = app->human;
        
    human.root.setPosition(position);
    glm::quat q = glm::angleAxis(glm::radians(orientationY.get()), vec3(0,1,0));
    human.orientationY = orientationY;
    human.root.setOrientation(q);
}

