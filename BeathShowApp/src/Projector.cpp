/*
 *  Projector.cpp
 *
 *  Copyright (c) 2020, Neil Mendoza, http://www.neilmendoza.com
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of Neil Mendoza nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "Projector.h"

Projector::Projector(const glm::vec2& reso)
{
    resolution.set(reso);
    ofFboSettings s;
    s.width = resolution.get().x;
    s.height = resolution.get().y;
    s.textureTarget = GL_TEXTURE_2D;
    s.wrapModeHorizontal = GL_CLAMP_TO_BORDER;
    s.wrapModeVertical = GL_CLAMP_TO_BORDER;
    s.internalformat = GL_RGBA8;
    s.numSamples = 0; //ofFbo::maxSamples();
    fbo.allocate(s);
    
    rotationDegs.addListener(this, &Projector::onRotationDegs);
    position.addListener(this, &Projector::onPosition);
    throwToWidthRatio.addListener(this, &Projector::onThrowToWidthRatio);
    principalPoint.addListener(this, &Projector::onPrincipalPoint);
    nearClip.addListener(this, &Projector::onNearFarClipChanged);
    farClip.addListener(this, &Projector::onNearFarClipChanged);

    setNearClip(nearClip);
    setFarClip(farClip);
}

void Projector::drawProjector(const ofColor& color)
{
    //glm::mat4 mat = getCurtainRelativeMat();

    transformGL();
    ofPushMatrix();
    
    ofPushStyle();
    ofSetColor(color);
    ofDrawBox(20.f, 5.f, 20.f);
    ofPopStyle();
    ofDrawAxis(20);
    restoreTransformGL();
    ofPopMatrix();

    if (frustumVisible) drawFrustum();
}

void Projector::drawFrustum(const ofColor& color)
{
    ofEnableDepthTest();
    ofPushStyle();
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    if (color.a != 255) glDepthMask(GL_FALSE);
    
    ofPushMatrix();
    transformGL();
    
    ofRectangle viewport(0, 0, resolution.get().x, resolution.get().y);
    ofMultMatrix(glm::inverse(getProjectionMatrix(viewport)));
    ofFill();
    ofEnableAlphaBlending();
    ofSetColor(color);
    ofDrawBox(2.0f, 2.0f, 2.0f);
    ofNoFill();
    ofSetColor(255, 0, 0, 100);
    ofDrawBox(2.0f, 2.0f, 2.0f);
    restoreTransformGL();
    ofPopMatrix();

    if (color.a != 255) glDepthMask(GL_TRUE);
    glPopAttrib();
    ofPopStyle();
    ofDisableDepthTest();
}

void Projector::beginFbo() {
    fbo.begin(OF_FBOMODE_NODEFAULTS);
    ofClear(clearColor);
}

void Projector::endFbo() {
    fbo.end();
    
    fbo.begin();
    if(projectorCalibration.get()) {
        drawCalibration();
    }
    fbo.end();
}

void Projector::beginCamera(){
    ofCamera::begin(ofRectangle(0, 0, resolution.get().x, resolution.get().y));

}

void Projector::endCamera(){
    ofCamera::end();
}

void Projector::begin()
{
    beginFbo();
    beginCamera();
}

void Projector::end()
{
    endCamera();
    endFbo();
}

void Projector::drawCalibration() const {
    const auto id = 1;
    const auto w = resolution.get().x;
    const auto h = resolution.get().y;
    auto w2 = w/2.0;
    auto h2 = h/2.0;
    // temp nono
    ofPushMatrix();
    ofPushStyle();
    int clr = 0xff << (8 * ((id-1)%3));
    ofSetHexColor(clr);
    ofFill();
    ofTranslate(w / 2, h / 2);
    ofSetLineWidth(10);
    ofDrawLine(-w2, -h2, w2, h2);
    ofDrawLine(-w2, h2, w2, -h2);
    float s = min(w, h);
    float s2 = s / 2.0;
    if(id<4){
        ofDrawCircle(0,0,s2);
    }else{
        ofBeginShape();
        ofVertex(0, -s2);
        ofVertex(s2, 0);
        ofVertex(0, s2);
        ofVertex(-s2, 0);
        ofEndShape(true);
    }
    ofSetColor(255);
    ofDrawLine(-w2, 0, w2, 0);

    ofNoFill();
    ofSetColor(0, 0, 255);
    ofDrawRectangle(-w2, -h2, w, h);

    ofPopStyle();
    ofPopMatrix();
}


void Projector::renderTestGrid()
{
    fbo.begin();
    ofClear(clearColor);
    const float gridSize = 100.f;
    ofPushStyle();
    unsigned numX = 1 + resolution.get().x / gridSize;
    unsigned numY = 1 + resolution.get().y / gridSize;
    for (unsigned y = 0; y < numY; ++y)
    {
        for (unsigned x = 0; x < numX; ++x)
        {
            ofSetColor(255.f * x / (numX - 1), 255.f * y / (numY - 1), 255);
            ofDrawCircle(x * gridSize, (y + .5f) * gridSize, .5f * gridSize - 2.f);
        }
    }
    ofPopStyle();
    fbo.end();
}

void Projector::draw(float x, float y, float w, float h) const
{
//      can not call
//    ofTexture & tex = fbo.getTexture();
//    ofTextureData & td = tex.getTextureData();
//    td.bFlipTexture = true;
    fbo.draw(x, y + h, w, -h);
    //fbo.draw(x, y, w, h);
//        fbo.draw(x+w, y, -w, h);
}

void Projector::calculateProjectiveTextureMatrix()
{
//    if(curtains && curtains->getCurtains().size() !=0 ) {
//
//        glm::mat4 translation = glm::translate(glm::vec3(.5f));
//        glm::mat4 scale = glm::scale(glm::vec3(.5f));
//        glm::mat4 local = getTransformationMatrix();
//        const Curtain &curtain = curtains->getCurtains()[curtainId - 1];
//        local = curtain.mat * local;
//
//        projectiveTextureMatrix =
//                local * getModelViewProjectionMatrix(ofRectangle(0.f, 0.f, resolution.get().x, resolution.get().y));
//    }
}

void Projector::setRotationRads(const glm::vec3& eulerRads)
{
    setRotationDegs(ofRadToDeg(1.f) * eulerRads);
}

void Projector::onThrowToWidthRatio(float& throwToWidthRatio)
{
    glm::vec2 fov;
    
    fov.x = 2.f * RAD_TO_DEG * atan(.5f * 1.f / throwToWidthRatio);
    float throwToHeightRatio = throwToWidthRatio * resolution.get().x / resolution.get().y;
    fov.y = 2.f * RAD_TO_DEG * atan(.5f * 1.f / throwToHeightRatio);
    
    setFov(fov.y);
    
    calculateProjectiveTextureMatrix();
}

void Projector::onRotationDegs(glm::vec3& eulerDegs)
{
    setOrientation(glm::quat(ofDegToRad(1.f) * eulerDegs));
    
    calculateProjectiveTextureMatrix();
}

// called from ofNode
void Projector::onPositionChanged()
{
    // disable the event so we don't get into
    // an infinite loop
    position.disableEvents();
    position = getPosition();
    position.enableEvents();
    
    calculateProjectiveTextureMatrix();
}

void Projector::onPosition(glm::vec3& position)
{
    setPosition(position);
    
    calculateProjectiveTextureMatrix();
}

void Projector::onPrincipalPoint(glm::vec2& principalPoint)
{
    glm::vec2 lensOffset(
        2.f * (.5f * getResolution().x - principalPoint.x) / getResolution().x,
        2.f * (principalPoint.y - .5f * getResolution().y) / getResolution().y
    );
    setLensOffset(lensOffset);
}

void Projector::onActivated(bool& activate) {
    ofLog() << "PJLink temporaily disabled...";
    return;
    ofLog() << "Projector activated: " << activate;
//        for(unsigned i=4;i<=6;i++){
        //string command = "%1AVMT 31\r";
        //if(activate) command = "%1AVMT 30\r";
        //pjLink.setProjectorIP(projectorIP);
//            pjLink.setProjectorIP("10.0.78."+ofToString(40+i));
        //pjLink.sendPJLinkCommand(command);
        //ofLog() << "bing";
//        }
}

ofRectangle Projector::getViewport() const
{
    return ofRectangle(0.f, 0.f, resolution.get().x, resolution.get().y);
}

