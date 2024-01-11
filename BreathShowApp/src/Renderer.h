#pragma once

#include "ofMain.h"
#include "ofxTextureRecorder.h"

class Renderer  {
    
public:
    Renderer();
    ~Renderer();
    
    void setup(const ofFbo & fbo);
    
    void start(const std::string & _dirpath);
    void stop();
    void render(const ofTexture & tex);
    
    bool isRendering(){ return bRender; };
    
private:
    bool bRender{false};
    
    ofxTextureRecorder recorder;
};
