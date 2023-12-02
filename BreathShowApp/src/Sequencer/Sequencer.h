//
//  Sequencer.hpp
//

#pragma once

#include "MySequence.h"

class Shape;
class Vezer;
class AnimHuman;

using SequenceUser = std::variant<std::monostate, shared_ptr<Shape>, shared_ptr<Vezer>, shared_ptr<AnimHuman>>;

class Sequencer{
    
public:
    Sequencer();
    void setup();
    void update();
    void updateCurrentFrame();
    void updateSequenceItemAll(bool bSeek);
    void updateSequenceItem(int entry, bool bSeek);
    void draw(bool * bOpen);
    bool save(const std::string & filepath);
    bool load(const std::string & filepath);

    SequenceUser addTrack(int type, int st=0, int end=1000, bool bExpanded=false  );
    void deleteTrack(int index);
    void deleteTrackAll();
    
    const vector<MySequence::MySequenceItem> & getSequenceItems(){ return mySequence.myItems; };
    
    int getCurrentFrame() const { return currentFrame; }
    
private:
    
    void startTrack(const SequenceUser & user, int frame);
    void stopTrack(const SequenceUser & user);

    // Gui helper
    void drawGui_Fan(shared_ptr<Shape> & s);
    void drawGui_RectScreen(shared_ptr<Shape> & s);
    void drawGui_Ellipse(shared_ptr<Shape> & s);
    void drawGui_VezerL(shared_ptr<Vezer> & vezer);
    void drawGui_VezerM(shared_ptr<Vezer> & vezer);
    void drawGui_Human(shared_ptr<AnimHuman> & human);

    void videoSection(shared_ptr<Shape> s);
    bool checkUserIsValid(const SequenceUser & user);
    
    MySequence mySequence;
    
    int currentFrame{0};
    float diffFrame{0.0f};
    uint64_t lastUpdateMs{0};
};


