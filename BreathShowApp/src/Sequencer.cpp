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
    //setupTestSequences();
}

void Sequencer::setupTestSequences(){
    mySequence.mFrameMin = 0;
    mySequence.mFrameMax = 30 * 60 * 3;

    int fps = 30;
    int offset = fps * 1;
    int duration = fps * 30;
    int pause = fps * 2;

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

void Sequencer::updateSequenceItem(int entry, bool bNeedUpdateVidFrame){
    std::vector<MySequence::MySequenceItem> & items = mySequence.myItems;
    int max = mySequence.GetFrameMax();
    int min = mySequence.GetFrameMin();
    bool bLoop = ofApp::get()->bLoop.get();

    /// TODO: null check
    //if(items.find()
    MySequence::MySequenceItem & item = items[entry];
    int st = item.mFrameStart;
    int end = item.mFrameEnd;
    int type = item.mType;
    
    if(st == currentFrame){
        startTrack(type, 0);
        cout << entry << " : start" << endl;
    }
    if(st < currentFrame && currentFrame < end){
        // we can not call start too often
        if(bNeedUpdateVidFrame){
            int frame = currentFrame - st;
            startTrack(type, frame);
            //cout << entry << " : seek, frame=" << frame << endl;
        }
    }else if(end == currentFrame){
        if(bLoop && st == min && end == max){
            // special case
            // we have to start movie immediately after finish
            startTrack(type, 0);
            //cout << entry << " : Loop back" << endl;
        }else{
            // otherwise stop it
            //cout << entry << " : stop" << endl;
            stopTrack(type);
        }
    }else{
        //cout << entry << " : stop" << endl;
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


void Sequencer::update(bool bNeedUpdateVidFrame){

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
        updateSequenceItem(i, bNeedUpdateVidFrame);
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
        
        int prevCurrentFrame = currentFrame;
        int options = ImSequencer::SEQUENCER_EDIT_STARTEND
                                    | ImSequencer::SEQUENCER_ADD
                                    | ImSequencer::SEQUENCER_DEL
                                    //| ImSequencer::SEQUENCER_COPYPASTE
                                    | ImSequencer::SEQUENCER_CHANGE_FRAME;

        auto onIndicatorMoveCb = [&](){
            //std::cout << "OnIndicatorMoveCb" << std::endl;
            update(true);
        };
        
        auto onSequenceMoveCb = [&](int movedEntry){
            //std::cout << "onSequenceMoveCb: " << movedEntry << std::endl;
            updateSequenceItem(movedEntry, true);
        };
        
        SequencerGui(&mySequence,
                     &currentFrame,
                     &expanded,
                     &selectedEntry,
                     &firstFrame,
                     options,
                     onIndicatorMoveCb,
                     onSequenceMoveCb);

        
        // add a UI to edit that particular item
        if (selectedEntry != -1)
        {
            const MySequence::MySequenceItem &item = mySequence.myItems[selectedEntry];
            int type = item.mType;
            int start = item.mFrameStart;
            int end = item.mFrameEnd;
            int total = end - start;
            ImGui::Text("%s: %df -> %df, total %d frames (%0.2f sec)",
                        SequencerItemTypeNames[type],
                        start,
                        end,
                        total,
                        total/30.0f);
        }
    }
    ofxImGui::EndWindow(settings);
}


bool Sequencer::save(const std::string & filepath){
    ofJson json;
    int min = mySequence.GetFrameMin();
    int max = mySequence.GetFrameMax();
    json["min"] = ofToString(min);
    json["max"] = ofToString(max);
    
    std::vector<MySequence::MySequenceItem> & items = mySequence.myItems;

    ofJson tracks = ofJson::array();
    for(int i=0; i<items.size(); i++){
        MySequence::MySequenceItem & item = items[i];
        int st = item.mFrameStart;
        int end = item.mFrameEnd;
        int type = item.mType;
     
        ofJson t;
        t["start"] = ofToString(st);
        t["end"] = ofToString(end);
        t["type"] = ofToString(type);
        tracks.push_back(t);
    }
    
    json["tracks"] = tracks;
    
    bool ok = ofSavePrettyJson(filepath, json);
    
    return ok;
}

bool Sequencer::load(const std::string & filepath){
    
    if(ofFile::doesFileExist(filepath)){
        
        ofJson json = ofLoadJson(filepath);
        if(!json.is_null()){
            int min = ofToInt(json.value("min", "0"));
            int max = ofToInt(json.value("max", "1000"));
            
            mySequence.mFrameMin = min;
            mySequence.mFrameMax = max;
            
            ofJson tracks = json["tracks"];
            
            for( auto it = tracks.begin(); it!=tracks.end(); ++it){
                const ofJson & j = it.value();
                int type = ofToInt(j.value("type", "0"));
                int st = ofToInt(j.value("start", "0"));
                int end = ofToInt(j.value("end", "1000"));
                mySequence.myItems.push_back(MySequence::MySequenceItem{type, st, end, false});
            }
            return true;
        }
    }
    return false;
}

