//
//  Elipse.hpp
//

#pragma once


#include "ofMain.h"

using glm::vec2;
using glm::vec3;

class Elipse{
  
public:
    Elipse();
    void setup(float radius, float centerWidth, int res=32);
    void update();
    void draw();
    void loadVideo(string path);
    void setPlayVideo(bool b);

    ofParameter<bool> bOn{"On", true};
    ofParameter<bool> bShowTest{"test texture", false};
    ofParameter<vec3> position{"position", vec3(0,50,0), vec3(-100), vec3(100)};
    ofParameter<vec3> scale{"scale", vec3(1), vec3(-2), vec3(2)};
    ofParameter<float> orientationY{"orientationY", 0, -180, 180};
    ofParameter<float> centerWidth{"center width", 40, 0, 100};
    ofParameter<float> radius{"radius", 30, 0, 60};
    ofParameter<float> resolution{"resolution", 32, 1, 72};

    ofParameterGroup grp{"Elipse", bOn, bShowTest, position, orientationY, scale, centerWidth, radius, resolution};

private:
    ofMesh mesh;

    ofImage img;
    ofVideoPlayer vid;
};
