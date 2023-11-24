#include "RectScreen.h"

RectScreen::RectScreen(){

    grp.setName("Rect");
    grp.add(size);
    setup();
    
    loadTestImage();
}


void RectScreen::setup(){
    float w = size.get().x;
    float h = size.get().y;
    plane.set( w, h, 2, 2, OF_PRIMITIVE_TRIANGLE_STRIP );
    plane.mapTexCoords(0, 0, 1, 1);
}

void RectScreen::update(){
    if(bOn){
        vid.update();
    }
}

void RectScreen::draw(){
    if(bOn){
        
        ofPushMatrix();
        ofTranslate(position);
        //ofTranslate(scale);
        ofRotateYDeg(orientationY);
        if(bShowTest){
            plane.mapTexCoords(0, 0, 1, 1);
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
        ofPopMatrix();
    }
}
