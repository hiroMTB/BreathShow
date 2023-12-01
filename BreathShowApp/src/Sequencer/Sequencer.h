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
    bool save(const std::string & filepath);
    bool load(const std::string & filepath);

    shared_ptr<Shape> addTrackShape(ShapeType type, int st=0, int end=1000, bool bExpanded=false  );
    void deleteTrack(int index);

    // TODO: not working well
    shared_ptr<Shape> duplicateTrack(const shared_ptr<Shape> & shape, int st=0, int end=1000, bool bExpanded=false  );
    
    const vector<MySequence::MySequenceItem> & getSequenceItems(){ return mySequence.myItems; };
    
    int getCurrentFrame() const { return currentFrame; }
    
private:
    
    void startTrack(const std::variant<shared_ptr<Shape>, shared_ptr<Human>, shared_ptr<Vezer>> user, int frame);
    void stopTrack(const std::variant<shared_ptr<Shape>, shared_ptr<Human>, shared_ptr<Vezer>> user);

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


