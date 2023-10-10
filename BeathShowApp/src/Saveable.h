#pragma once

#include "ofMain.h"

class Saveable
{
public:
    bool load(const string& filepath);
    bool save(const string& filepath);
    
    virtual ofParameterGroup& getParametersRef() = 0;
};

