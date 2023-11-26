//
//  Human.h
//

#pragma once
#include "ofParameter.h"
#include "ofxAssimpModelLoader.h"

class Human {
    
public:
    
    Human(){
        root.position.setName("position");
        root.position.setMin(glm::vec3(-500));
        root.position.setMax(glm::vec3(500));
        root.position = glm::vec3(0);
        grp.add(root.position);
        
    }
    bool setup(){
        

#if OF_VERSION_MINOR >= 12
        bool bOk = model.load("./3dModel/human.obj", ofxAssimpModelLoader::OPTIMIZE_HIGH);
#else
        bool bOk = model.loadModel("./3dModel/human.obj", true);
#endif
        if(bOk){
            ofLogNotice() << "OK! load human.obj";
            model.setScale(scale.get().x, scale.get().y, scale.get().z) ;
            //model.setPosition(position.get().x, position.get().y, position.get().z);
            //model.setRotation(0, orientationY, 0, 1, 0);
        }else{
            ofLogError() << "Can not load human.obj";
        }
        return bOk;
    }
    
    
    void draw(){
        if(bOn){
            ofSetColor(180);
            root.transformGL();
            model.drawWireframe();            
            root.restoreTransformGL();
        }
    }
    
    ofNode root; // empty Root Node, place holder
    
    ofxAssimpModelLoader model;
    ofParameter<bool> bOn{"ON", true};
    //ofParameter<glm::vec3> position{"position", vec3(0), vec3(-500), vec3(500)};
    ofParameter<glm::vec3> scale{"scale", glm::vec3(1), glm::vec3(-3), glm::vec3(3)};
    ofParameter<float> orientationY{"orientationY", 0, -360, 360};
    ofParameterGroup grp{"human", bOn, scale, orientationY};
};
