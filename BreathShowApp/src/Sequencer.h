//
//  Sequencer.hpp
//

#pragma once

#include "MySequence.h"

class Sequencer{
    
public:
    Sequencer();
    void setup();
    void setupTestSequences();
    void update(bool bNeedUpdateVidFrame=false);
    void updateSequenceItem(int entry, bool bNeedUpdateVidFrame=false);
    void draw(bool * bOpen);
    
    bool save(const std::string & filepath);
    bool load(const std::string & filepath);

private:
    
    void startTrack(int type, int frame);
    void stopTrack(int type);
    
    MySequence mySequence;
    
    int currentFrame{0};
    float diffFrame{0.0f};
    uint64_t lastUpdateMs{0};
};


