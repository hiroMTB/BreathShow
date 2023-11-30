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
        
        model.clear();
        mesh.clear();
        model.setScaleNormalization(false);
        
        //string filepath = "./3dModel/human.obj";
        string filepath = "./3dModel/human.ply";
        
#if OF_VERSION_MINOR >= 12
        bool bOk = model.load(filepath, ofxAssimpModelLoader::OPTIMIZE_HIGH);
#else
        bool bOk = model.loadModel(filepath, true);
#endif
        if(bOk){
            ofLogNotice() << "OK! load: " << filepath;
            
            cout << "Human Original Scale: " << model.getScale() << endl;
            
            setModelScale();
            //model.setScale(scale.get().x, scale.get().y, scale.get().z) ;
            //model.setPosition(position.get().x, position.get().y, position.get().z);
            //model.setRotation(0, orientationY, 0, 1, 0);
            
            unsigned int nMesh = model.getNumMeshes();
            for(int i=0; i<nMesh; i++){
                mesh = model.getMesh(i);
            }

        }else{
            ofLogError() << "Can not load: "<< filepath;
        }
        return bOk;
    }
    
    
    void draw(){
        if(bOn){
            root.transformGL();
            ofSetColor(200, 200);
            //model.drawWireframe();
            mesh.drawWireframe();
            root.restoreTransformGL();
        }
    }
    
    void setModelScale(){
        glm::vec3 s =  preScale * height.get() * 0.01f ;
        model.setScale(s.x, s.y, s.z);
        model.update();
    }
    
    ofNode root; // empty Root Node, place holder
    
    ofxAssimpModelLoader model;
    ofParameter<bool> bOn{"ON", true};
    //ofParameter<glm::vec3> position{"position", vec3(0), vec3(-500), vec3(500)};
    ofParameter<int> height{"height", 165, 100, 200};
    ofParameter<float> orientationY{"orientationY", 0, -360, 360};
    ofParameterGroup grp{"human", bOn, height, orientationY};
    ofMesh mesh;
private:
    
#if OF_VERSION_MINOR >= 12
    // can not load obj file with propery scaling editted by Blender
    glm::vec3 preScale{1};
#else
    glm::vec3 preScale{1};
#endif
};
