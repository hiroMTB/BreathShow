//
//  Ellipse.hpp
//

#pragma once

#include "ofMain.h"
#include "Shape.h"

class Ellipse : public Shape{
  
public:
    Ellipse();
    void setup() override;
    void update() override;
    void draw() override;

    ofParameter<float> centerWidth{"center width", 40, 0, 100};
    ofParameter<float> radius{"radius", 30, 0, 60};
    ofParameter<float> resolution{"resolution", 32, 1, 72};

};
