//
//  Fan.h
//  BeathShowApp
//
//  Created by Hiroshi Matoba on 04.10.23.
//

#ifndef Fan_h
#define Fan_h

#include "ofMain.h"
#include "Saveable.h"

using glm::vec2;
using glm::vec3;

class Fan : public Saveable{
    
    public:
    Fan();    
    void setup(float angle, float len=30, int direction=1, int res = 36);
    void update();
    void draw();
    void loadVideo(string path);
    void setPlayVideo(bool b);
    
    ofParameterGroup & getParametersRef() override { return grp; }

    ofParameter<bool> bOn{"On", true};
    ofParameter<bool> bShowTest{"test texture", false};
    ofParameter<vec3> position{"position", vec3(5,36,0), vec3(-100), vec3(100)};
    ofParameter<vec3> scale{"scale", vec3(1), vec3(-2), vec3(2)};
    ofParameter<float> orientationY{"orientationY", 0, -180, 180};
    ofParameter<float> length{"length", 30, 0, 60};
    ofParameter<float> openAngle{"openAngle", 90, 0, 180};
    ofParameter<float> resolution{"resolution", 32, 1, 72};
    ofParameter<int> direction{"direction", 1, -1, 1};

    ofParameterGroup grp{"Fun", bOn, bShowTest, position, orientationY, scale, openAngle, resolution, direction, length};

    private:
    ofMesh mesh;
    
    ofImage img;
    ofVideoPlayer vid;
    
};

#endif /* Fan_h */
