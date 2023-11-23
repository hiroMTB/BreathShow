//
//  Sequencer.cpp
//

#include "Sequencer.h"
#include "ofxImGui.h"
#include "ImHelpers.h"
#include "MySequence.h"
#include "ImCurveEdit.h"
#include "imgui_internal.h"
#include "ofApp.h"

Sequencer::Sequencer(){
    setup();
}

void Sequencer::setup(){
    setupTestSequences();
}

void Sequencer::setupTestSequences(){
    mySequence.mFrameMin = 0;
    mySequence.mFrameMax = 30 * 20;

    int fps = 30;
    int offset = fps * 1;
    int duration = fps * 3;
    int pause = fps * 1;
    
    for(int i=0; i<4; i++){
        int st = offset+duration*i+pause*i;
        int end = st + duration;
        mySequence.myItems.push_back(MySequence::MySequenceItem{i, st, end, false});
    }
}

void Sequencer::startTrack(int type, int frame){
    shared_ptr<ofApp> & app = ofApp::get();
    switch(type){
        case 0: app->fanL.start(frame); break;
        case 1: app->fanR.start(frame); break;
        case 2: app->rectScreen.start(frame); break;
        case 3: app->ellipse.start(frame); break;
        default: ofLogError("Sequencer") << "Unknow Sequence Type!!"; break;
    }
}

void Sequencer::stopTrack(int type){
    shared_ptr<ofApp> & app = ofApp::get();
    switch(type){
        case 0: app->fanL.stop(); break;
        case 1: app->fanR.stop(); break;
        case 2: app->rectScreen.stop(); break;
        case 3: app->ellipse.stop(); break;
        default: ofLogError("Sequencer") << "Unknow Sequence Type!!"; break;
    }
}

void Sequencer::update(){

    shared_ptr<ofApp> & app = ofApp::get();
    bool bPlay = app->bPlay.get();
    bool bLoop = app->bLoop.get();
    int max = mySequence.GetFrameMax();
    int min = mySequence.GetFrameMin();
    
    if(bPlay){
        uint64_t diff = ofGetCurrentTime().getAsMilliseconds() - lastUpdateMs;
        diffFrame += diff / float(1000.0 / 30.0);
        if(diffFrame<1){
            
        }else{
            currentFrame += floor(diffFrame);
            diffFrame -= floor(diffFrame);
        }
        
        if(max <= currentFrame){
            if(bLoop){
                //currentFrame = currentFrame % max;
                currentFrame = min;
            }else{
                currentFrame = min;
                app->bPlay = false;
            }
        }
    }
    lastUpdateMs = ofGetCurrentTime().getAsMilliseconds();

    
    std::vector<MySequence::MySequenceItem> & items = mySequence.myItems;

    for(int i=0; i<items.size(); i++){
        MySequence::MySequenceItem & item = items[i];
        int st = item.mFrameStart;
        int end = item.mFrameEnd;
        int type = item.mType;
        
        if(st == currentFrame){
            startTrack(type, 0);
        }
        if(st <= currentFrame && currentFrame < end){
            // we can not call start too often
            //startTrack(type, currentFrame);
        }else if(end == currentFrame){
            if(bLoop && st == min && end == max){
                // special case
                // we have to start movie immediately after finish
                startTrack(type, 0);
            }else{
                // otherwise stop it
                stopTrack(type);
            }
        }else{
            stopTrack(type);
        }
        
        if(0){
            /// TODO: Read Curve Value
            if(st <= currentFrame && currentFrame <= end){
                
                RampEdit & ramp = item.rampEdit;
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
                            //std::cout << "Between points " << x1 << " and " << x2 << std::endl;
                            ImCurveEdit::CurveType ctype = ramp.GetCurveType(j);
                            float t = (currentFrame-x1) / (x2 - x1);
                            const ImVec2 sp1 = ImLerp(p1, p2, t);
                            
                            if(ctype == ImCurveEdit::CurveType::CurveSmooth){
                                // calculate smooth value
                                const float rt1 = ImCurveEdit::smoothstep(p1.x, p2.x, sp1.x);
                                const float y = ImLerp(p1.y, p2.y, rt1);
                                //std::cout << y << std::endl;
                            }else if(ctype == ImCurveEdit::CurveType::CurveLinear){
                                // return sp1
                            }
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
        static bool expanded = true;

        ImGui::PushItemWidth(130);
        ImGui::InputInt("Frame Min", &mySequence.mFrameMin);
        ImGui::SameLine();
        ImGui::InputInt("Frame ", &currentFrame);
        ImGui::SameLine();
        ImGui::InputInt("Frame Max", &mySequence.mFrameMax);
        ImGui::PopItemWidth();
        SequencerGui(&mySequence, &currentFrame, &expanded, &selectedEntry, &firstFrame,
                     ImSequencer::SEQUENCER_EDIT_STARTEND
                     | ImSequencer::SEQUENCER_ADD
                     | ImSequencer::SEQUENCER_DEL
                     //| ImSequencer::SEQUENCER_COPYPASTE
                     | ImSequencer::SEQUENCER_CHANGE_FRAME);
        // add a UI to edit that particular item
//        if (selectedEntry != -1)
//        {
//            const MySequence::MySequenceItem &item = mySequence.myItems[selectedEntry];
//            ImGui::Text("Draw Custom GUI here for %s \nAAA\nBBB\nCCC\nDDD", SequencerItemTypeNames[item.mType]);
//            ImGui::Text("%d", selectedEntry);
//            // switch (type) ....
//        }
    }
    ofxImGui::EndWindow(settings);
}
