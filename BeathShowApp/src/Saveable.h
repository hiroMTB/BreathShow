#pragma once

#include "ofMain.h"

class Saveable
{
public:
    bool load(const string& fileName);
    bool save(const string& fileName);
    
    virtual ofParameterGroup& getParametersRef() = 0;
};

