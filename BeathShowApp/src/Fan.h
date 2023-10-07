//
//  Fan.h
//  BeathShowApp
//
//  Created by Hiroshi Matoba on 04.10.23.
//

#ifndef Fan_h
#define Fan_h

#include "ofMain.h"

using glm::vec2;
using glm::vec3;

class Fan{
    
    public:

    Fan(){
        setup(180);
        
        //bool ok = img.load("./tex/test.png");
        bool ok = img.load("./tex/test2.jpg");
        if(ok){
            ofLogNotice("Fan") << "OK loading texture";
        }else{
            ofLogError("Fan") << "Can not load texture";
        }
    };
    
    void setup(float angle, float len=30, int direction=1, int res = 36){

        mesh.clear();
        
        float dA = direction * angle / (float)res;
        float start = ofDegToRad(-90);
        for(int i=0; i<res; i++){
            
            float a1 = start + ofDegToRad((i + 0) * dA);
            float a2 = start + ofDegToRad((i + 1) * dA);
            mesh.addVertex( vec3(0) );
            mesh.addVertex( vec3(len * cos(a1), len * sin(a1), 0) );
            mesh.addVertex( vec3(len * cos(a2), len * sin(a2), 0) );
            
            float ty = 890.0 * (openAngle/180.0) / res;
            mesh.addTexCoord( vec2(0, i*ty) );
            mesh.addTexCoord( vec2(192, i*ty) );
            mesh.addTexCoord( vec2(192, (i+1)*ty) );
                        
            mesh.setMode(OF_PRIMITIVE_TRIANGLES );
        }
    }
    
    void update(){
        setup(openAngle, length, direction, resolution);
    }
    
    void draw(){
        ofPushMatrix();
        ofTranslate(position);
        //ofTranslate(scale);
        //mesh.drawWireframe();
        ofTexture & tex = img.getTexture();
        tex.bind();
        mesh.draw();
        tex.unbind();
        ofPopMatrix();
    }
    
    ofParameter<vec3> position{"position", vec3(5,36,0), vec3(-100), vec3(100)};
    ofParameter<vec3> scale{"scale", vec3(1), vec3(-2), vec3(2)};
    ofParameter<float> orientationY{"orientationY", 0, -180, 180};
    ofParameter<float> length{"length", 30, 0, 60};
    ofParameter<float> openAngle{"openAngle", 90, 0, 180};
    ofParameter<float> resolution{"resolution", 32, 1, 72};
    ofParameter<int> direction{"direction", 1, -1, 1};
    ofParameterGroup grp{"Fun", position, orientationY, scale, openAngle, resolution, direction};

    private:
    ofMesh mesh;
    
    ofImage img;
};

#endif /* Fan_h */
