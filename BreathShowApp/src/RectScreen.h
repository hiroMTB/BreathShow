//
//  RectScreen.h
//

#ifndef RectScreen_h
#define RectScreen_h

#include "ofMain.h"
#include "Saveable.h"

using glm::vec2;
using glm::vec3;

class RectScreen : public Saveable{
    
public:
    RectScreen();
    
    void setup(float w, int h);
    void update();
    void draw();
    void loadVideo(string path);
    void setPlayVideo(bool b);
    
    ofParameter<bool> bOn{"On", true};
    ofParameter<bool> bShowTest{"test texture", true};
    ofParameter<vec3> position{"position", vec3(0,100,0), vec3(-100), vec3(300)};
    ofParameter<vec2> size{"size", vec2(200), vec2(10), vec2(300)};
    ofParameter<float> orientationY{"orientationY", 0, -180, 180};

    ofParameterGroup grp{"Rect", bOn, bShowTest, position, size, orientationY};
    ofParameterGroup & getParametersRef() override { return grp; }

private:
    ofPlanePrimitive plane;
    ofImage img;
    ofVideoPlayer vid;
};

#endif /* RectScreen_h */
