//
//  Sequencer.hpp
//

#pragma once

#include "MySequence.h"

class Shape;

class Sequencer{
    
public:
    Sequencer();
    void setup();
    void setupTestSequences();
    void update();
    void updateCurrentFrame();
    void updateSequenceItemAll(bool bSeek);
    void updateSequenceItem(int entry, bool bSeek);
    void draw(bool * bOpen);
        void drawGui_Fan(shared_ptr<Shape> & s);
        void drawGui_RectScreen(shared_ptr<Shape> & s);
        void drawGui_Ellipse(shared_ptr<Shape> & s);
    bool save(const std::string & filepath);
    bool load(const std::string & filepath);

    void addTrack(ShapeType type, int st=0, int end=1000, bool bExpanded=false  );
    void deleteTrack(int index);
    
    const vector<MySequence::MySequenceItem> & getSequenceItems(){ return mySequence.myItems; };
    
private:
    
    void startTrack(const shared_ptr<Shape> & shape, int frame);
    void stopTrack(const shared_ptr<Shape> & shape);

    // Gui helper
    void drawGui_Fan(shared_ptr<Shape> & s);
    void drawGui_RectScreen(shared_ptr<Shape> & s);
    void drawGui_Ellipse(shared_ptr<Shape> & s);
    void videoSection(shared_ptr<Shape> s);

    
    MySequence mySequence;
    
    int currentFrame{0};
    float diffFrame{0.0f};
    uint64_t lastUpdateMs{0};
};


