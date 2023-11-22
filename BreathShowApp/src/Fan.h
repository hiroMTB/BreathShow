//
//  Fan.h
//  BeathShowApp
//
//  Created by Hiroshi Matoba on 04.10.23.
//

#pragma once

#include "ofMain.h"
#include "Shape.h"

using glm::vec2;
using glm::vec3;

class Fan : public Shape{
    
    public:
    Fan();
    virtual ~Fan(){}
    void setup(float angle, float len=30, int direction=1, int res = 36);
    void update();
    void draw();
    
    ofParameter<float> radius{"radius", 30, 0, 150};
    ofParameter<float> openAngle{"openAngle", 90, 0, 180};
    ofParameter<float> resolution{"resolution", 32, 1, 72};
    ofParameter<int> direction{"direction", 1, -1, 1};

};
