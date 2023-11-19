#pragma once

#include "ofMain.h"
#include <glm/gtx/euler_angles.hpp>

class Projector : public ofCamera
{
public:
    Projector(const glm::vec2& resolution = glm::vec2(1920.f, 1080.0f));

    void drawProjector(const ofColor& color = ofColor::gray);
    void drawCalibration() const;
    void drawFrustum(const ofColor& color = ofColor(255, 255, 0, 20));

    void beginFbo();
    void endFbo();
    void beginCamera();
    void endCamera();
    void begin() override;
    void end() override;
    void renderTestGrid();
    void draw(float x, float y, float w, float h) const;
    
    float getThrowAngleRad();
    ofRectangle getViewport() const;
    vector<glm::vec2> getProjectionAreaTriangle();  // TODO should be reviewed
    glm::mat4 getTransformationMatrix();     // TODO should be reviewed

    // called from ofNode
    void onPositionChanged() override;
    
    // parameter event handlers
    void onNearFarClipChanged(float & f);
    void onThrowToWidthRatio(float& throwToWidthRatio);
    void onRotationDegs(glm::vec3& eulerDegs);
    void onPosition(glm::vec3& position);
    void onPrincipalPoint(glm::vec2& principalPoint);
    void onActivated(bool& activate);

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

    ofFbo fbo;

    ofParameterGroup grp {
        "Projector",
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
};
