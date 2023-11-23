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
    void update();
    void draw(bool * bOpen);

private:
    
    void startTrack(int type, int frame);
    void stopTrack(int type);
    
    MySequence mySequence;
    
    int currentFrame{0};
    float diffFrame{0.0f};
    uint64_t lastUpdateMs{0};
};


