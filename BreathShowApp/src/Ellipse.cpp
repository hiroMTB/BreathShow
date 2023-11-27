//
//  Ellipse.cpp
//

#include "Ellipse.h"

Ellipse::Ellipse(){
    type = ShapeType::ELLIPSE;
    position.setMin(vec3(-200));
    position.setMax(vec3(200));
    position.setName("position");

    grp.setName("Ellipse");
    grp.add(centerWidth);
    grp.add(resolution);
    grp.add(radius);
    grp.add(position);
        
    loadTestImage();
    setup();
}

void Ellipse::setup(){
    setPosition(position);
    setOrientation(orientation);
    
    mesh.clear();
    
    float r = radius.get();
    int res = resolution.get();
    
    float ch = centerWidth * 0.5;
    float halfCircleLen = PI * r;
    float totalLen = ch*4.0f + halfCircleLen * 2.0f;
    
    
    // Right top rect
    mesh.addVertex( vec3(0, 0, 0) );
    mesh.addVertex( vec3(0, r, 0) );
    mesh.addVertex( vec3(ch, 0, 0) );

    if(bFlipX){
        mesh.addTexCoord( vec2(1, 0) );
        mesh.addTexCoord( vec2(0, 0) );
        mesh.addTexCoord( vec2(1, ch/totalLen) );
    }else{
        mesh.addTexCoord( vec2(0, 0) );
        mesh.addTexCoord( vec2(1, 0) );
        mesh.addTexCoord( vec2(0, ch/totalLen) );
    }
    
    mesh.addVertex( vec3(ch, 0, 0) );
    mesh.addVertex( vec3(0, r, 0) );
    mesh.addVertex( vec3(ch, r, 0) );

    if(bFlipX){
        mesh.addTexCoord( vec2(1, ch/totalLen) );
        mesh.addTexCoord( vec2(0, 0) );
        mesh.addTexCoord( vec2(0, ch/totalLen) );
    }else{
        mesh.addTexCoord( vec2(0, ch/totalLen) );
        mesh.addTexCoord( vec2(1, 0) );
        mesh.addTexCoord( vec2(1, ch/totalLen) );
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
            
            float ty = 1.0f / (float)res * halfCircleLen/totalLen;
            if(bFlipX){
                mesh.addTexCoord( offsetCoord + vec2(1, i*ty) );
                mesh.addTexCoord( offsetCoord + vec2(0, i*ty) );
                mesh.addTexCoord( offsetCoord + vec2(0, (i+1)*ty) );
            }else{
                mesh.addTexCoord( offsetCoord + vec2(0, i*ty) );
                mesh.addTexCoord( offsetCoord + vec2(1, i*ty) );
                mesh.addTexCoord( offsetCoord + vec2(1, (i+1)*ty) );
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

    if(bFlipX){
        mesh.addTexCoord( offsetCoord + vec2(1, 0) );
        mesh.addTexCoord( offsetCoord + vec2(0, 0) );
        mesh.addTexCoord( offsetCoord + vec2(1, ch/totalLen) );
        mesh.addTexCoord( offsetCoord + vec2(1, ch/totalLen) );
        mesh.addTexCoord( offsetCoord + vec2(0, 0) );
        mesh.addTexCoord( offsetCoord + vec2(0, ch/totalLen) );
    }else{
        mesh.addTexCoord( offsetCoord + vec2(0, 0) );
        mesh.addTexCoord( offsetCoord + vec2(1, 0) );
        mesh.addTexCoord( offsetCoord + vec2(0, ch/totalLen) );
        mesh.addTexCoord( offsetCoord + vec2(0, ch/totalLen) );
        mesh.addTexCoord( offsetCoord + vec2(1, 0) );
        mesh.addTexCoord( offsetCoord + vec2(1, ch/totalLen) );
    }

    // Left bottom rect
    mesh.addVertex( vec3(0, 0, 0) );   
    mesh.addVertex( vec3(0, -r, 0));   
    mesh.addVertex( vec3(-ch, 0, 0) ); 
    mesh.addVertex( vec3(-ch, 0, 0) ); 
    mesh.addVertex( vec3(0,  -r, 0));  
    mesh.addVertex( vec3(-ch, -r, 0) );

    offsetCoord = {0, (ch+halfCircleLen+ch)/totalLen};

    if(bFlipX){
        mesh.addTexCoord( offsetCoord + vec2(1, 0) );
        mesh.addTexCoord( offsetCoord + vec2(0, 0) );
        mesh.addTexCoord( offsetCoord + vec2(1, ch/totalLen) );
        mesh.addTexCoord( offsetCoord + vec2(1, ch/totalLen) );
        mesh.addTexCoord( offsetCoord + vec2(0, 0) );
        mesh.addTexCoord( offsetCoord + vec2(0, ch/totalLen) );
    }else{
        mesh.addTexCoord( offsetCoord + vec2(0, 0) );
        mesh.addTexCoord( offsetCoord + vec2(1, 0) );
        mesh.addTexCoord( offsetCoord + vec2(0, ch/totalLen) );
        mesh.addTexCoord( offsetCoord + vec2(0, ch/totalLen) );
        mesh.addTexCoord( offsetCoord + vec2(1, 0) );
        mesh.addTexCoord( offsetCoord + vec2(1, ch/totalLen) );
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
            
            float ty = 1.0f / (float)res * halfCircleLen/totalLen;
            if(bFlipX){
                mesh.addTexCoord( offsetCoord + vec2(1, i*ty) );
                mesh.addTexCoord( offsetCoord + vec2(0, i*ty) );
                mesh.addTexCoord( offsetCoord + vec2(0, (i+1)*ty) );
            }else{
                mesh.addTexCoord( offsetCoord + vec2(0, i*ty) );
                mesh.addTexCoord( offsetCoord + vec2(1, i*ty) );
                mesh.addTexCoord( offsetCoord + vec2(1, (i+1)*ty) );
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
    if(bFlipX){
        mesh.addTexCoord( offsetCoord + vec2(1, 0) );
        mesh.addTexCoord( offsetCoord + vec2(0, 0) );
        mesh.addTexCoord( offsetCoord + vec2(1, ch/totalLen) );
        mesh.addTexCoord( offsetCoord + vec2(1, ch/totalLen) );
        mesh.addTexCoord( offsetCoord + vec2(0, 0) );
        mesh.addTexCoord( offsetCoord + vec2(0, ch/totalLen) );
    }else{
        mesh.addTexCoord( offsetCoord + vec2(0, 0) );
        mesh.addTexCoord( offsetCoord + vec2(1, 0) );
        mesh.addTexCoord( offsetCoord + vec2(0, ch/totalLen) );
        mesh.addTexCoord( offsetCoord + vec2(0, ch/totalLen) );
        mesh.addTexCoord( offsetCoord + vec2(1, 0) );
        mesh.addTexCoord( offsetCoord + vec2(1, ch/totalLen) );
    }
    mesh.setMode(OF_PRIMITIVE_TRIANGLES );
}

void Ellipse::update(){
    if(bOn){
        vid.update();
    }
}

void Ellipse::draw(){
    if(bOn){
        transformGL();

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
        restoreTransformGL();
    }
}
