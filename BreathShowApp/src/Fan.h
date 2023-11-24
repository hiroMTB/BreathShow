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
    void setup() override;
    void update() override;
    void draw() override;
    
    ofParameter<float> radius{"radius", 30, 0, 150};
    ofParameter<float> openAngle{"openAngle", 90, 0, 180};
    ofParameter<int> resolution{"resolution", 60, 2, 180};
    ofParameter<int> direction{"direction", 1, -1, 1};

};
