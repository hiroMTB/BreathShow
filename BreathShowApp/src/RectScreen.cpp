#include "RectScreen.h"

RectScreen::RectScreen(){
    position.setMin(vec3(-200));
    position.setMax(vec3(200));
    position.setName("position");
    
    grp.add(position);
    grp.add(size);
    
    grp.setName("Rect");
    
    loadTestImage();
    setup();    
}


void RectScreen::setup(){
    setPosition(position);
    setOrientation(orientation);
    
    float w = size.get().x;
    float h = size.get().y;
    plane.set( w, h, 2, 2, OF_PRIMITIVE_TRIANGLE_STRIP );
    if(bFlipX){
        plane.mapTexCoords(1, 0, 0, 1);
    }else{
        plane.mapTexCoords(0, 0, 1, 1);
    }
}

void RectScreen::update(){
    if(bOn){
        vid.update();
    }
}

void RectScreen::draw(){
    if(bOn){
        
        transformGL();
        if(bShowTest){
            const ofMesh & mesh = plane.getMesh();
            ofTexture & tex = img.getTexture();
            tex.bind();
            mesh.draw();
            tex.unbind();
            
            mesh.drawWireframe();
        }else{
            if(vid.isLoaded()){
                ofTexture & tex = vid.getTexture();
                if(tex.isAllocated()){
                    const ofMesh & mesh = plane.getMesh();
                    tex.bind();
                    mesh.draw();
                    tex.unbind();
                }
            }
        }
        restoreTransformGL();
    }
}
