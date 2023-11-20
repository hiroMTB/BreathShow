//
//  Elipse.cpp
//

#include "Elipse.h"

Elipse::Elipse(){
    setup(30, 40);
    
    bool ok = img.load("./tex/test2.jpg");
    if(ok){
        ofLogNotice("Fan") << "OK loading texture";
    }else{
        ofLogError("Fan") << "Can not load texture";
    }
}

void Elipse::setup(float r, float centerWidth, int res){
    mesh.clear();
    
    
    float ch = centerWidth * 0.5;
    float halfCircleLen = PI * r;
    float totalLen = ch*4.0f + halfCircleLen * 2.0f;
    
    
    // Right top rect
    mesh.addVertex( vec3(0, 0, 0) );
    mesh.addVertex( vec3(0, r, 0) );
    mesh.addVertex( vec3(ch, 0, 0) );

    if(bShowTest){
        mesh.addTexCoord( vec2(0, 0) );
        mesh.addTexCoord( vec2(1, 0) );
        mesh.addTexCoord( vec2(0, ch/totalLen) );
    }else{
        mesh.addTexCoord( vec2(0, 1) );
        mesh.addTexCoord( vec2(1, 1) );
        mesh.addTexCoord( vec2(0, 1-ch/totalLen) );
    }
    
    mesh.addVertex( vec3(ch, 0, 0) );
    mesh.addVertex( vec3(0, r, 0) );
    mesh.addVertex( vec3(ch, r, 0) );

    if(bShowTest){
        mesh.addTexCoord( vec2(0, ch/totalLen) );
        mesh.addTexCoord( vec2(1, 0) );
        mesh.addTexCoord( vec2(1, ch/totalLen) );
    }else{
        mesh.addTexCoord( vec2(0, 1-ch/totalLen) );
        mesh.addTexCoord( vec2(1, 1) );
        mesh.addTexCoord( vec2(1, 1-ch/totalLen) );
    }

    vec3 offset = {ch, 0, 0};
    vec2 offsetCoord = {0, ch/totalLen};
    {
        // Right half circle
        int direction = -1;
        float dA = direction * 180.0f / (float)res;
        float start = ofDegToRad(90);
        for(int i=0; i<res; i++){
            float a1 = start + ofDegToRad((i + 0) * dA);
            float a2 = start + ofDegToRad((i + 1) * dA);
            mesh.addVertex( offset + vec3(0) );
            mesh.addVertex( offset + vec3(r * cos(a1), r * sin(a1), 0) );
            mesh.addVertex( offset + vec3(r * cos(a2), r * sin(a2), 0) );
            
            if(bShowTest){
                float ty = 1.0f / (float)res * halfCircleLen/totalLen;
                mesh.addTexCoord( offsetCoord + vec2(0, i*ty) );
                mesh.addTexCoord( offsetCoord + vec2(1, i*ty) );
                mesh.addTexCoord( offsetCoord + vec2(1, (i+1)*ty) );
            }else{
                if(vid.isLoaded()){
                    float ty = 1.0f / (float)res * halfCircleLen/totalLen;
                    mesh.addTexCoord( vec2(offsetCoord.x + 0, 1.0f-(offsetCoord.y+i*ty) ));
                    mesh.addTexCoord( vec2(offsetCoord.x + 1, 1.0f-(offsetCoord.y+i*ty) ));
                    mesh.addTexCoord( vec2(offsetCoord.x + 1, 1.0f-(offsetCoord.y+(i+1)*ty)) );
                }
            }
        }
    }
    
    
    // Right bottom rect
    mesh.addVertex( vec3(ch, 0, 0) );
    mesh.addVertex( vec3(ch, -r, 0));
    mesh.addVertex( vec3(0, 0, 0) ); 
    mesh.addVertex( vec3(0, 0, 0) ); 
    mesh.addVertex( vec3(ch, -r, 0));
    mesh.addVertex( vec3(0, -r, 0) );
    offsetCoord = {0, (ch+halfCircleLen)/totalLen};

    if(bShowTest){
        mesh.addTexCoord( offsetCoord + vec2(0, 0) );
        mesh.addTexCoord( offsetCoord + vec2(1, 0) );
        mesh.addTexCoord( offsetCoord + vec2(0, ch/totalLen) );
        mesh.addTexCoord( offsetCoord + vec2(0, ch/totalLen) );
        mesh.addTexCoord( offsetCoord + vec2(1, 0) );
        mesh.addTexCoord( offsetCoord + vec2(1, ch/totalLen) );
    }else{    
        mesh.addTexCoord( vec2(offsetCoord.x + 0, 1.0f - (offsetCoord.y + 0)) );
        mesh.addTexCoord( vec2(offsetCoord.x + 1, 1.0f - (offsetCoord.y + 0)) );
        mesh.addTexCoord( vec2(offsetCoord.x + 0, 1.0f - (offsetCoord.y + ch/totalLen)) );
        mesh.addTexCoord( vec2(offsetCoord.x + 0, 1.0f - (offsetCoord.y + ch/totalLen)) );
        mesh.addTexCoord( vec2(offsetCoord.x + 1, 1.0f - (offsetCoord.y + 0)) );
        mesh.addTexCoord( vec2(offsetCoord.x + 1, 1.0f - (offsetCoord.y + ch/totalLen)) );
    }

    // Left bottom rect
    mesh.addVertex( vec3(0, 0, 0) );   
    mesh.addVertex( vec3(0, -r, 0));   
    mesh.addVertex( vec3(-ch, 0, 0) ); 
    mesh.addVertex( vec3(-ch, 0, 0) ); 
    mesh.addVertex( vec3(0,  -r, 0));  
    mesh.addVertex( vec3(-ch, -r, 0) );

    offsetCoord = {0, (ch+halfCircleLen+ch)/totalLen};

    if(bShowTest){
        mesh.addTexCoord( offsetCoord + vec2(0, 0) );
        mesh.addTexCoord( offsetCoord + vec2(1, 0) );
        mesh.addTexCoord( offsetCoord + vec2(0, ch/totalLen) );
        mesh.addTexCoord( offsetCoord + vec2(0, ch/totalLen) );
        mesh.addTexCoord( offsetCoord + vec2(1, 0) );
        mesh.addTexCoord( offsetCoord + vec2(1, ch/totalLen) );
    }else{
        mesh.addTexCoord( vec2(offsetCoord.x + 0, 1.0f - (offsetCoord.y + 0)) );
        mesh.addTexCoord( vec2(offsetCoord.x + 1, 1.0f - (offsetCoord.y + 0)) );
        mesh.addTexCoord( vec2(offsetCoord.x + 0, 1.0f - (offsetCoord.y + ch/totalLen)) );
        mesh.addTexCoord( vec2(offsetCoord.x + 0, 1.0f - (offsetCoord.y + ch/totalLen)) );
        mesh.addTexCoord( vec2(offsetCoord.x + 1, 1.0f - (offsetCoord.y + 0)) );
        mesh.addTexCoord( vec2(offsetCoord.x + 1, 1.0f - (offsetCoord.y + ch/totalLen)) );
    }
    
    {
        offset = {-ch, 0, 0};
        offsetCoord = {0, (ch+halfCircleLen+ch+ch)/totalLen};

        // Left half circle
        int direction = -1;
        float dA = direction * 180.0f / (float)res;
        float start = ofDegToRad(-90);
        for(int i=0; i<res; i++){
            float a1 = start + ofDegToRad((i + 0) * dA);
            float a2 = start + ofDegToRad((i + 1) * dA);
            mesh.addVertex( offset + vec3(0) );
            mesh.addVertex( offset + vec3(r * cos(a1), r * sin(a1), 0) );
            mesh.addVertex( offset + vec3(r * cos(a2), r * sin(a2), 0) );
            
            if(bShowTest){
                float ty = 1.0f / (float)res * halfCircleLen/totalLen;
                mesh.addTexCoord( offsetCoord + vec2(0, i*ty) );
                mesh.addTexCoord( offsetCoord + vec2(1, i*ty) );
                mesh.addTexCoord( offsetCoord + vec2(1, (i+1)*ty) );
            }else{
                if(vid.isLoaded()){
                    float ty = 1.0f / (float)res * halfCircleLen/totalLen;
                    mesh.addTexCoord( vec2(offsetCoord.x + 0, 1.0f - (offsetCoord.y + i*ty)) );
                    mesh.addTexCoord( vec2(offsetCoord.x + 1, 1.0f - (offsetCoord.y + i*ty)) );
                    mesh.addTexCoord( vec2(offsetCoord.x + 1, 1.0f - (offsetCoord.y + (i+1)*ty)) );
                }
            }
        }
    }
    
    // Left top rect
    mesh.addVertex( vec3(-ch, 0, 0));
    mesh.addVertex( vec3(-ch, r, 0));
    mesh.addVertex( vec3(0, 0, 0));  
    mesh.addVertex( vec3(0, 0, 0));  
    mesh.addVertex( vec3(-ch, r, 0));
    mesh.addVertex( vec3(0, r, 0));  
    
    offsetCoord = {0, (ch+halfCircleLen+ch+ch+halfCircleLen)/totalLen};
    if(bShowTest){
        mesh.addTexCoord( offsetCoord + vec2(0, 0) );
        mesh.addTexCoord( offsetCoord + vec2(1, 0) );
        mesh.addTexCoord( offsetCoord + vec2(0, ch/totalLen) );
        mesh.addTexCoord( offsetCoord + vec2(0, ch/totalLen) );
        mesh.addTexCoord( offsetCoord + vec2(1, 0) );
        mesh.addTexCoord( offsetCoord + vec2(1, ch/totalLen) );
    }else{
        mesh.addTexCoord( vec2(offsetCoord.x + 0, 1.0f - (offsetCoord.y )) );
        mesh.addTexCoord( vec2(offsetCoord.x + 1, 1.0f - (offsetCoord.y )) );
        mesh.addTexCoord( vec2(offsetCoord.x + 0, 1.0f - (offsetCoord.y + ch/totalLen)) );
        mesh.addTexCoord( vec2(offsetCoord.x + 0, 1.0f - (offsetCoord.y + ch/totalLen)) );
        mesh.addTexCoord( vec2(offsetCoord.x + 1, 1.0f - (offsetCoord.y )) );
        mesh.addTexCoord( vec2(offsetCoord.x + 1, 1.0f - (offsetCoord.y + ch/totalLen)) );
    }
    mesh.setMode(OF_PRIMITIVE_TRIANGLES );
}

void Elipse::update(){
    if(bOn){
        setup(radius, centerWidth, resolution); // TODO: Stop calling this
        vid.update();
    }
}

void Elipse::draw(){
    if(bOn){
        ofPushMatrix();
        ofTranslate(position);
        //ofTranslate(scale);
        if(bShowTest){
            ofTexture & tex = img.getTexture();
            tex.bind();
            mesh.draw();
            tex.unbind();
            
            mesh.drawWireframe();
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
}

void Elipse::setPlayVideo(bool b){
    vid.setPaused(b);
}

void Elipse::loadVideo(string path){

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
