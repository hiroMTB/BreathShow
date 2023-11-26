//
//  Fan.cpp
//  BeathShowApp
//
//  Created by Hiroshi Matoba on 04.10.23.
//

#include "Fan.h"

Fan::Fan(){
    grp.add(radius);
    grp.add(openAngle);
    grp.add(resolution);
    grp.add(direction);
    
    loadTestImage();
    setup();
};

void Fan::setup(){
    
    mesh.clear();

    float angle = openAngle.get();
    float len = radius.get();
    int res = resolution.get();
    
    float dA = direction * angle / (float)res;
    float start = ofDegToRad(-90);
    for(int i=0; i<res; i++){
        
        float a1 = start + ofDegToRad((i + 0) * dA);
        float a2 = start + ofDegToRad((i + 1) * dA);
        
        mesh.addVertex( vec3(0) );
        mesh.addVertex( vec3(len * cos(a1), len * sin(a1), 0) );
        mesh.addVertex( vec3(len * cos(a2), len * sin(a2), 0) );
             
        float ty = 1.0f / res;
        bool bLcenter = (!bFlipX && (direction>0)) || (bFlipX && (direction<0));
        if(bLcenter){
            // texture L is center of Fan
            mesh.addTexCoord( vec2(0, (res-i)*ty) );
            mesh.addTexCoord( vec2(1, (res-i)*ty) );
            mesh.addTexCoord( vec2(1, (res-i-1)*ty) );
        }else{
            // texture R is center of Fan
            mesh.addTexCoord( vec2(1, (res-i)*ty) );
            mesh.addTexCoord( vec2(0, (res-i)*ty) );
            mesh.addTexCoord( vec2(0, (res-i-1)*ty) );
        }
        mesh.setMode(OF_PRIMITIVE_TRIANGLES );
    }
}


void Fan::update(){
    if(bOn){
        vid.update();
    }
}

void Fan::draw(){
    if(bOn){
        ofPushMatrix();
        ofTranslate(position);
        //ofTranslate(scale);
        ofRotateYDeg(orientationY);
        if(bShowTest){
            ofTexture & tex = img.getTexture();
            tex.bind();
            mesh.draw();
            tex.unbind();
            
            mesh.drawWireframe();
        }else{
            if(vid.isLoaded()){
                ofTexture & tex = vid.getTexture();
                if(tex.isAllocated()){
                    tex.bind();
                    mesh.draw();
                    tex.unbind();
                }
            }
        }
        ofPopMatrix();
    }
}

