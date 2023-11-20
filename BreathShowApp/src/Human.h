//
//  Human.h
//  BeathShowApp
//
//  Created by Hiroshi Matoba on 16.10.23.
//

#ifndef Human_h
#define Human_h

#include "ofParameter.h"
#include "ofxAssimpModelLoader.h"

class Human {
    
public:
    
    Human(){}
    bool setup(){
#if OF_VERSION_MINOR >= 12
        bool bOk = model.load("./3dModel/human.obj", ofxAssimpModelLoader::OPTIMIZE_HIGH);
#else
        bool bOk = model.loadModel("./3dModel/human.obj", true);
#endif
        if(bOk){
            ofLogNotice() << "OK! load human.obj";
            model.setScale(scale.get().x, scale.get().y, scale.get().z) ;
            model.setPosition(position.get().x, position.get().y, position.get().z);
            model.setRotation(0, orientationY, 0, 1, 0);
        }else{
            ofLogError() << "Can not load human.obj";
        }
        return bOk;
    }
    
    void draw(){
        if(bOn){
            ofSetColor(180);
            model.drawWireframe();
        }
    }
    
    ofxAssimpModelLoader model;
    ofParameter<bool> bOn{"ON", true};
    ofParameter<glm::vec3> position{"position", vec3(0), vec3(-500), vec3(500)};
    ofParameter<glm::vec3> scale{"scale", vec3(1), vec3(-3), vec3(3)};
    ofParameter<float> orientationY{"orientationY", 0, -360, 360};
    ofParameterGroup grp{"human", bOn, position, orientationY, scale};
};
#endif /* Human_h */
