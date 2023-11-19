//
//  Sequencer.cpp
//

#include "Sequencer.h"
#include "ofxImGui.h"
#include "ImHelpers.h"
#include "MySequence.h"

Sequencer::Sequencer(){
    setup();
}

void Sequencer::setup(){
    setupTestSequences();
}

void Sequencer::setupTestSequences(){
    mySequence.mFrameMin = -100;
    mySequence.mFrameMax = 1000;
    mySequence.myItems.push_back(MySequence::MySequenceItem{0,10,30,false});
    mySequence.myItems.push_back(MySequence::MySequenceItem{1,20,30,true});
    mySequence.myItems.push_back(MySequence::MySequenceItem{3, 12, 60, false});
    mySequence.myItems.push_back(MySequence::MySequenceItem{2, 61, 90, false});
    mySequence.myItems.push_back(MySequence::MySequenceItem{4, 90, 99, false});
}

void Sequencer::update(){

    const std::vector<MySequence::MySequenceItem> & items = mySequence.myItems;

    for(int i=0; i<items.size(); i++){
        const MySequence::MySequenceItem & item = items[i];
        int st = item.mFrameStart;
        int end = item.mFrameEnd;
        int type = item.mType;
        
        if(st <= currentFrame && currentFrame <= end){
            std::cout << item.mType << std::endl;
        }
    }
}

void Sequencer::draw(bool * bOpen){
    auto settings = ofxImGui::Settings();
    
    if(ofxImGui::BeginWindow("Sequencer", settings, false, bOpen)) {

        // let's create the sequencer
        static int selectedEntry = -1;
        static int firstFrame = 0;
        static bool expanded = false;

        ImGui::PushItemWidth(130);
        ImGui::InputInt("Frame Min", &mySequence.mFrameMin);
        ImGui::SameLine();
        ImGui::InputInt("Frame ", &currentFrame);
        ImGui::SameLine();
        ImGui::InputInt("Frame Max", &mySequence.mFrameMax);
        ImGui::PopItemWidth();
        SequencerGui(&mySequence, &currentFrame, &expanded, &selectedEntry, &firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);
        // add a UI to edit that particular item
        if (selectedEntry != -1)
        {
            const MySequence::MySequenceItem &item = mySequence.myItems[selectedEntry];
            ImGui::Text("Draw Custom GUI here for %s \nAAA\nBBB\nCCC\nDDD", SequencerItemTypeNames[item.mType]);
            // switch (type) ....
        }
    }
    ofxImGui::EndWindow(settings);
}
