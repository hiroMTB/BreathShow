//
//  Sequencer.cpp
//

#include "Sequencer.h"
#include "ofxImGui.h"
#include "ImHelpers.h"
#include "MySequence.h"
#include "ImCurveEdit.h"
#include "imgui_internal.h"

Sequencer::Sequencer(){
    setup();
}

void Sequencer::setup(){
    setupTestSequences();
}

void Sequencer::setupTestSequences(){
    mySequence.mFrameMin = 0;
    mySequence.mFrameMax = 1000;
    mySequence.myItems.push_back(MySequence::MySequenceItem{0,10,30,false});
    mySequence.myItems.push_back(MySequence::MySequenceItem{1,20,30,true});
    mySequence.myItems.push_back(MySequence::MySequenceItem{3, 12, 60, false});
    mySequence.myItems.push_back(MySequence::MySequenceItem{2, 61, 90, false});
    mySequence.myItems.push_back(MySequence::MySequenceItem{4, 90, 99, false});
}

void Sequencer::update(){

    std::vector<MySequence::MySequenceItem> & items = mySequence.myItems;

    for(int i=0; i<items.size(); i++){
        MySequence::MySequenceItem & item = items[i];
        int st = item.mFrameStart;
        int end = item.mFrameEnd;
        int type = item.mType;
        RampEdit & ramp = item.rampEdit;
        if(st <= currentFrame && currentFrame <= end){
            std::cout << item.mType << std::endl;

            int cnt = ramp.GetCurveCount();
            for(int j=0; j<cnt; j++){
                std::vector<ImVec2> & pts = ramp.GetPoints(j);
                if(pts.size() == 1){
                    // only one points, value is the same
                    std::cout << "y = " << pts[0].y << std::endl;
                }
                for(int k=0; k<pts.size()-1; k++){
                    ImVec2 & p1 = pts[k];
                    ImVec2 & p2 = pts[k+1];
                    float x1 = p1.x;
                    float x2 = p2.x;
                    if( x1<=currentFrame && currentFrame<=x2){
                        std::cout << "Between points " << x1 << " and " << x2 << std::endl;
                        ImCurveEdit::CurveType ctype = ramp.GetCurveType(j);
                        float t = (currentFrame-x1) / (x2 - x1);
                        const ImVec2 sp1 = ImLerp(p1, p2, t);

                        if(ctype == ImCurveEdit::CurveType::CurveSmooth){
                            // calculate smooth value
                            const float rt1 = ImCurveEdit::smoothstep(p1.x, p2.x, sp1.x);
                            const float y = ImLerp(p1.y, p2.y, rt1);
                            std::cout << y << std::endl;
                        }else if(ctype == ImCurveEdit::CurveType::CurveLinear){
                            // return sp1
                        }
                    }
                }
            }
            
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
