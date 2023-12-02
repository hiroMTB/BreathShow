//
//  AnimHuman.hpp
//

#pragma once

#include "ofMain.h"

using glm::vec3;

class AnimHuman{
    
public:
    AnimHuman(){};
    void applyTransformation();
    
    ofParameter<vec3> position{"position", vec3(0), vec3(-200), vec3(200) };
    ofParameter<float> orientationY{"orientationY", 0, -360, 360};
    ofParameterGroup grp{"AnimHuman", position, orientationY };
};
