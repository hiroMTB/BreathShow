//
//  Body.h
//

#pragma once

#include "ofMain.h"
#include "ofxOscMessage.h"
#include "Landmark.h"

class Body {
    
public:
    
    Body();
    ~Body();

    void processOsc(const ofxOscMessage & m, const vector<string> & tokens);

    inline void updateValue(Landmark & landmark, float v, int xyz, bool bTransform=true);

    void calc();
    void draw(float sSize = 1, const ofColor & c = ofColor(0,0,255));

private:
    void setFreqAll();
    void setMinCutoffAll();
    void setBetaAll();
    void setDerivateCutoffAll();
    void calculateAbsoluteTransform();

public:
    std::vector<Landmark> landmarks;

    glm::vec3 hipCenter{0};
    glm::vec3 shoulderCenter{0};
    glm::vec3 rootPos{0};
    glm::vec3 rootOri{0};
    
    ofParameter<glm::vec3> offset{ "offset", glm::vec3(0, 110, 0), glm::vec3(-100), glm::vec3(100)};
    ofParameter<glm::vec3> scale{ "scale", glm::vec3(1), glm::vec3(-30), glm::vec3(30)};
    ofParameterGroup landmarkGrp {"Landmark", offset, scale};

    ofParameter<float> lowpass{ "lowpass", 0.7, 0, 1};
    ofParameterGroup legacyGrp{ "Legacy Filter", lowpass};

    // check example for the effect of each parameter
    // https://gery.casiez.net/1euro/InteractiveDemo/
    ofParameter<double> frequency{ "frequency", 60, 0, 120 };
    ofParameter<double> mincutoff{ "mincutoff", 1.0, 0, 10 };
    ofParameter<double> beta{ "beta", 0.07, 0, 1 };
    ofParameter<double> dcutoff{ "dcutoff", 1.0, 0, 10 };
    ofParameterGroup oneEuroGrp{ "One Euro Filter", frequency, mincutoff, beta, dcutoff};

    ofParameter<int> filterType{"Filter Type", 2, 0, 2};    // 0: none, 1: legacy, 2: OneEuro

    ofParameterGroup filterGrp{ "Filter", filterType, oneEuroGrp, legacyGrp};

    ofParameterGroup grp {"Body", landmarkGrp, filterGrp};


    ofEventListeners listeners;

private:
    bool bEnter{false};
};
