//
//  RectScreen.h
//

#pragma once

#include "ofMain.h"
#include "Shape.h"

class RectScreen : public Shape{
    
public:
    RectScreen();
    
    void setup(float w, int h);
    void update();
    void draw();
    ofParameter<vec2> size{"size", vec2(200), vec2(10), vec2(300)};

private:
    ofPlanePrimitive plane;
};
