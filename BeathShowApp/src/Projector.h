/*
 *  Projector.h
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

#pragma once

#include "ofMain.h"
#include "Saveable.h"
#include <glm/gtx/euler_angles.hpp>

class Projector : public ofCamera, public Saveable
{
public:
    Projector(const glm::vec2& resolution = glm::vec2(1920.f, 1080.0f));

    void drawProjector(const ofColor& color = ofColor::gold);
    void drawCalibration() const;
    void drawFrustum(const ofColor& color = ofColor(255, 255, 0, 50));

    void beginFbo();
    void endFbo();
    void beginCamera();
    void endCamera();
    void begin() override;
    void end() override;
    
    void renderTestGrid();
    
    void draw(float x, float y, float w, float h) const;
    
    glm::vec2 getPrincipalPoint() const { return principalPoint; }
    void setPrincipalPoint(const glm::vec2& principalPoint) { this->principalPoint = principalPoint; }
    
    float getThrowToWidthRatio() const { return throwToWidthRatio; }
    void setThrowToWidthRatio(float throwToWidthRatio) { this->throwToWidthRatio = throwToWidthRatio; }
    
    glm::vec3 getRotationDegs() const { return rotationDegs; }
    void setRotationDegs(const glm::vec3& eulerDegs) { this->rotationDegs = eulerDegs; }
    void setRotationRads(const glm::vec3& eulerRads);
    
    void setFrustumVisible(bool frustumVisible) { this->frustumVisible = frustumVisible; }
    bool getFrustumVisible() { return frustumVisible; }

    void setClearColor(const ofFloatColor& clearColor) { this->clearColor = clearColor; }
    
    glm::mat4 getProjectiveTextureMatrix() const { return projectiveTextureMatrix; }
    
    glm::vec2 getResolution() const { return resolution; }

    ofParameterGroup& getParametersRef() override { return parameters; }
    
    ofTexture& getTexture()  { return fbo.getTexture(); }

    const ofTexture& getTexture() const  { return fbo.getTexture(); }

    ofRectangle getViewport() const;

    // parameter event handlers
    // void onThrowToWidthRatio(float& throwToWidthRatio);
    void onHeadingPitchRoll(glm::vec3& headingPitchRoll);
    // void onPosition(glm::vec3& position);

    void onNearFarClipChanged(float & f){
        setNearClip(nearClip);
        setFarClip(farClip);
    }

    ofFbo & getFbo() { return fbo; };

    void calculateProjectiveTextureMatrix();

    // Copy from distribution_v2
    float getThrowAngleRad() {
        auto throwRatio = 2.f * atan(.5f * 1.f / getThrowToWidthRatio());
        return throwRatio;
    }

    // TODO should be reviewed
    glm::mat4 getTransformationMatrix(){
        glm::mat4x4 mat;
        // _mat = glm::scale(_mat,glm::vec3(1,-1,1));
        mat = glm::translate(mat, position.get());
        glm::vec3 rad = glm::radians(rotationDegs.get());
        mat = mat * glm::eulerAngleXYZ(rad.x, rad.y, rad.z);
        return mat;
    }

    // TODO should be reviewed
     vector<glm::vec2> getProjectionAreaTriangle(){
        auto projectorMatrix = getTransformationMatrix();
        glm::vec2 projectorCenter = projectorMatrix * glm::vec4(0, 0,0, 1.0);

        float angleTotal = getThrowAngleRad();
        float angleTotal2 = angleTotal/2.0f;
        float angleProjector = ofDegToRad(getRotationDegs().z)+PI/2;
        float angleStart = -angleTotal2+angleProjector;
        float angleEnd = angleStart+angleTotal;

        //char m[255];
        //sprintf(m, "Projector Angles\nStart: %0.2f\nEnd: 0.2%\nTotal: %0.2f\nprojector angle: %0.2f", ofRadToDeg(angleStart), ofRadToDeg(angleEnd), ofRadToDeg(angleTotal), ofRadToDeg(angleProjector));
        //ofLogNotice("Nozzles") << m;

        glm::vec2 rayEnd1 = projectorMatrix* glm::vec4(0,10000,0,1);
        rayEnd1 = glm::rotate(rayEnd1,-angleTotal/2.0f);
        glm::vec2 rayEnd2 = projectorMatrix* glm::vec4(0,10000,0,1);
        rayEnd2 = glm::rotate(rayEnd2,angleTotal/2.0f);

        vector<glm::vec2> ret = {projectorCenter,rayEnd1,rayEnd2};
        return ret;
     }

private:

    ofFbo fbo;
    // glm::vec2 resolution;

public:
    ofFloatColor clearColor {ofFloatColor(0.f, 1.f)};
    ofParameter<bool> frustumVisible {"frustumVisible", false};
    ofParameter<glm::vec3> position {"position", glm::vec3(0,60, 100), glm::vec3(-500.f), glm::vec3(500.f)};
    ofParameter<glm::vec3> rotationDegs {"rotationDegs", glm::vec3(), glm::vec3(-180.f), glm::vec3(180.f)};
    ofParameter<float> throwToWidthRatio {"throwToWidthRatio", 1.25f, 1.f, 2.0f};
    ofParameter<glm::vec2> principalPoint {"principalPoint", glm::vec2(960,0), glm::vec2(0.f), glm::vec2(1920.f)};
    ofParameter<glm::vec2> resolution {"resolution", glm::vec2(), glm::vec2(1260.f,980.f), glm::vec2(3840.f, 2160.f)};
    ofParameter<bool> projectorCalibration {"calibration", true};
    ofParameter<float> nearClip{"naerClip", 1, 0, 1000};
    ofParameter<float> farClip{"farClip", 1000, 0, 10000};
    ofParameter<glm::vec2> fboOffsetPosition{"fbo offset position", glm::vec2(0), glm::vec2(-1000), glm::vec2(1000)};


private:
    glm::mat4 projectiveTextureMatrix;

    ofParameterGroup parameters {
        "projectorParameters",
        frustumVisible,
        projectorCalibration,
        position,
        rotationDegs,
        throwToWidthRatio,
        principalPoint,
        resolution,
        nearClip,
        farClip,
        fboOffsetPosition
    };
    
    // called from ofNode
    void onPositionChanged() override;
    
    // parameter event handlers
    void onThrowToWidthRatio(float& throwToWidthRatio);
    void onRotationDegs(glm::vec3& eulerDegs);
    void onPosition(glm::vec3& position);
    void onPrincipalPoint(glm::vec2& principalPoint);
    void onActivated(bool& activate);

};

