//
//  Ellipse.hpp
//

#pragma once

#include "ofMain.h"
#include "Shape.h"

class Ellipse : public Shape{
  
public:
    Ellipse();
    void setup(float radius, float centerWidth, int res=32);
    void update();
    void draw();

    ofParameter<float> centerWidth{"center width", 40, 0, 100};
    ofParameter<float> radius{"radius", 30, 0, 60};
    ofParameter<float> resolution{"resolution", 32, 1, 72};

};
