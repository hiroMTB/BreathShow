//
//  Fan.cpp
//  BeathShowApp
//
//  Created by Hiroshi Matoba on 04.10.23.
//

#include "Fan.h"

Fan::Fan(){
    setup(180);
    
    //bool ok = img.load("./tex/test.png");
    bool ok = img.load("./tex/test2.jpg");
    if(ok){
        ofLogNotice("Fan") << "OK loading texture";
    }else{
        ofLogError("Fan") << "Can not load texture";
    }
};

void Fan::setup(float angle, float len, int direction, int res){
    
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

void Fan::update(){
    setup(openAngle, length, direction, resolution);
}

void Fan::draw(){
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
