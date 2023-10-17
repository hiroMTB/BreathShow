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
        
        if(bShowTest){
            float ty = 890.0 * (openAngle/180.0) / res;
            mesh.addTexCoord( vec2(0, i*ty) );
            mesh.addTexCoord( vec2(192, i*ty) );
            mesh.addTexCoord( vec2(192, (i+1)*ty) );
        }else{
            if(vid.isLoaded()){
                float ty = 1.0f / res;
                mesh.addTexCoord( vec2(0, (res-i)*ty) );
                mesh.addTexCoord( vec2(1, (res-i)*ty) );
                mesh.addTexCoord( vec2(1, (res-i-1)*ty) );
            }
        }
        mesh.setMode(OF_PRIMITIVE_TRIANGLES );
    }
}

void Fan::loadVideo(string path){

    if( ofFile::doesFileExist(path)){
        vid.load(path);
        vid.setUseTexture(true);
        vid.setLoopState(OF_LOOP_NORMAL);
        vid.play();
        ofLogNotice("Fan") << "Video file loaded: ";
    }else{
        ofLogError("Fan") << "Video file does not exist: " << path;
    }
}

void Fan::update(){
    setup(openAngle, length, direction, resolution);
    vid.update();
}

void Fan::draw(){
    ofPushMatrix();
    ofTranslate(position);
    //ofTranslate(scale);
    //mesh.drawWireframe();
    if(bShowTest){
        ofTexture & tex = img.getTexture();
        tex.bind();
        mesh.draw();
        tex.unbind();
    }else{
        if(vid.isLoaded()){
            ofTexture & tex = vid.getTexture();
            tex.bind();
            mesh.draw();
            tex.unbind();
        }
    }
    ofPopMatrix();
}
