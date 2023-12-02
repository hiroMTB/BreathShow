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
#include "Shape.h"
#include "Fan.h"
#include "RectScreen.h"
#include "Ellipse.h"
#include "io.h"
#include "Helper.h"
#include "AnimHuman.h"
#include "Human.h"
#include "RampEdit.h"

Sequencer::Sequencer(){
    setup();
}

void Sequencer::setup(){
    //setupTestSequences();
}

SequenceUser Sequencer::addTrack( int type, int st, int end, bool bExpanded ){
    
    SequenceUser user = std::monostate{};
    
    // Shapes: Fan, Rect, Ellipse
    if(type < 3){
        ShapeType shapeType = (ShapeType) type; // convert track type to shapeType
        
        switch (shapeType){
            case ShapeType::FAN:
            {
                user = make_shared<Fan>();
                //shapes.emplace_back( shape );
                mySequence.myItems.push_back(MySequence::MySequenceItem{ type, st, end, bExpanded, user});
                ofLogNotice("Sequencer::AddTrack() - ") << "Fan";
                
            }
                break;
            case ShapeType::RECT_SCREEN:
            {
                user = make_shared<RectScreen>();
                //shapes.emplace_back( shape );
                mySequence.myItems.push_back(MySequence::MySequenceItem{ type, st, end, bExpanded, user});
                ofLogNotice("Sequencer::AddTrack() - ") << "RECT_SCREEN";
            }
                break;
            case ShapeType::ELLIPSE:
            {
                user = make_shared<Ellipse>();
                //shapes.emplace_back( shape );
                mySequence.myItems.push_back(MySequence::MySequenceItem{ type, st, end, bExpanded, user});
                ofLogNotice("Sequencer::AddTrack() - ") << "ELLIPSE";
            }
                break;
            case ShapeType::NONE:
                ofLogError("Sequencer::AddTrack() - ") << "ShapeType::NONE";
                break;
            default:
                ofLogError("Sequencer::AddTrack() - ") << "Unknown ShapeType" << type;
                break;
        }
    }else{

        // vezer
        if(type == 3){
            user = make_shared<Vezer>();
            mySequence.myItems.push_back(MySequence::MySequenceItem{ type, st, end, bExpanded, user});
            ofLogError("Sequencer::AddTrack() - ") << "Vezer";
        }
        
        // human
        if(type == 4){
            user = make_shared<AnimHuman>();
            mySequence.myItems.push_back(MySequence::MySequenceItem{ type, st, end, bExpanded, user});
            ofLogError("Sequencer::AddTrack() - ") << "AnimHuman";
        }
    }
    
    return user;
}

void Sequencer::deleteTrackAll(){

    mySequence.myItems.clear();
}

void Sequencer::deleteTrack(int index){
    
    mySequence.myItems.erase(mySequence.myItems.begin() + index);

    // delete from ofApp::shapes container
//    MySequence::MySequenceItem & item = mySequence.myItems[index];
//    vector<shared_ptr<Shape>> & shapes = ofApp::get()->shapes;
//    std::remove( shapes.begin(), shapes.end(), item.shape );
}

void Sequencer::startTrack(const SequenceUser & user, int frame){

    if(checkUserIsValid(user)){
        if (std::holds_alternative<shared_ptr<Shape>>(user)) {
            auto & shape = std::get<shared_ptr<Shape>>(user);
            if(shape){
                shape->start(frame);
            }else{
                ofLogError("Sequencer::startTrack") << "shape is null";
            }
        }else if (std::holds_alternative<shared_ptr<Vezer>>(user)) {
            // vezer
            auto & vezer = std::get<shared_ptr<Vezer>>(user);
            if(vezer){
                vezer->start();
                vezer->setFrame(frame);
            }
        }else if (std::holds_alternative<shared_ptr<AnimHuman>>(user)) {
            auto & human = std::get<shared_ptr<AnimHuman>>(user);
            human->applyTransformation();
        }
    }
}

void Sequencer::stopTrack(const SequenceUser & user){
    if (std::holds_alternative<shared_ptr<Shape>>(user)) {
        auto & shape = std::get<shared_ptr<Shape>>(user);
        if(shape){
            shape->stop();
        }else{
            ofLogError("Sequencer::startTrack") << "shape is null";
        }
    }else if (std::holds_alternative<shared_ptr<Vezer>>(user)) {
        // vezer
        auto & vezer = std::get<shared_ptr<Vezer>>(user);
        if(vezer){
            vezer->stop();
        }
    }
}

void Sequencer::update(){
    updateCurrentFrame();
    
    bool bPlay = ofApp::get()->bPlay.get();
    if(bPlay){ updateSequenceItemAll(false); }
}

void Sequencer::updateCurrentFrame(){
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
}

void Sequencer::updateSequenceItemAll(bool bSeek){
    
    //Vezer::setIsPlayingSomeVezer(false);
    
    std::vector<MySequence::MySequenceItem> & items = mySequence.myItems;
    for(int i=0; i<items.size(); i++){
        updateSequenceItem(i, bSeek);
    }
}

void Sequencer::updateSequenceItem(int entry, bool bSeek){
    int max = mySequence.GetFrameMax();
    int min = mySequence.GetFrameMin();
    bool bLoop = ofApp::get()->bLoop.get();

    /// TODO: null check
    //if(items.find()
    MySequence::MySequenceItem & item = mySequence.myItems[entry];
    int st = item.mFrameStart;
    int end = item.mFrameEnd;
    int type = item.mType;
    bool isVezer = (type == 3);
    bool isHuman = (type == 4);
    {
        if(st == currentFrame){
            startTrack(item.user, 0);
//            if(isVezer) {
//                //Vezer::setIsPlayingSomeVezer(true);
//            }
            
            ofLogVerbose("Sequencer") << entry << " : start";
        }
        else if(st < currentFrame && currentFrame < end){
            int frame = currentFrame - st;

            if(bSeek){
                // we can not call start too often
                startTrack(item.user, frame);
                ofLogVerbose("Sequencer") << entry << " : seek, frame=" << frame;
            }

            if(isVezer || isHuman){
                startTrack(item.user, frame);
                //Vezer::setIsPlayingSomeVezer(true);
            }

        }else if(end == currentFrame){
            if(bLoop && st == min && end == max){
                // special case
                // we have to start movie immediately after finish
                startTrack(item.user, 0);
                ofLogVerbose("Sequencer") << entry << " : Loop back";
            }else{
                // otherwise stop it
                //ofLogVerbose("Sequencer") << entry << " : stop";
                stopTrack(item.user);
            }
        }else{
            //ofLogVerbose("Sequencer") << entry << " : stop";
            stopTrack(item.user);
        }
    }
    
    if(type==4){
        if (std::holds_alternative<shared_ptr<AnimHuman>>(item.user)) {
            auto & h = std::get<shared_ptr<AnimHuman>>(item.user);
            
            /// Read Curve Value
            if(st <= currentFrame && currentFrame <= end){
                
                RampEdit & ramp = item.rampEdit;
                int cnt = ramp.GetCurveCount();
                for(int j=0; j<cnt; j++){
                    std::vector<ImVec2> & pts = ramp.GetPoints(j);
                    if(pts.size() == 1){
                        // only one points, value is the same
                        std::cout << "y = " << pts[0].y << std::endl;
                    }
                    
                    vec3 tPos = h->position;
                    float oriY = h->orientationY;
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
                                float v = ofMap(y, 0.0f, 1.0f, h->position.getMin().x, h->position.getMax().x);
                                if(j == 3){
                                    oriY = v;
                                }else{
                                    tPos[j] = v;
                                }
                            }else if(ctype == ImCurveEdit::CurveType::CurveLinear){
                                // return sp1
                            }
                        }
                    }
                    
                    h->position = tPos;
                    h->orientationY = oriY;
                    h->applyTransformation();
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
        
        ImGui::PushItemWidth(100);
        
        ImGui::InputInt("Min", &mySequence.mFrameMin);

        ImGui::SameLine(); ImGui::Dummy({30, 0}); ImGui::SameLine();
        ImGui::InputInt("Frame ", &currentFrame);

        ImGui::SameLine();
        int sec = std::floor(currentFrame/30.0);
        int frame = currentFrame%30;
        ImGui::Text("%02d:%02d:%02d", sec/60, sec%60, frame);
        
        ImGui::SameLine(); ImGui::Dummy({30, 0}); ImGui::SameLine();
        ImGui::InputInt("Max", &mySequence.mFrameMax);

        ImGui::PopItemWidth();
        
        int prevCurrentFrame = currentFrame;
        int options = ImSequencer::SEQUENCER_EDIT_STARTEND
        | ImSequencer::SEQUENCER_ADD
        | ImSequencer::SEQUENCER_DEL
        //| ImSequencer::SEQUENCER_COPYPASTE
        | ImSequencer::SEQUENCER_CHANGE_FRAME;
        
        auto onIndicatorMove = [&](){
            //std::cout << "OnIndicatorMove" << std::endl;
            updateSequenceItemAll(true);
        };
        
        auto onSequenceMove = [&](int movedEntry){
            //std::cout << "onSequenceMove: " << movedEntry << std::endl;
            updateSequenceItem(movedEntry, true);
        };

        auto onSequenceAdd = [&](int type){
            //std::cout << "onSequenceAdd: " << type << std::endl;
            addTrack(type);
        };

        auto onSequenceDel = [&](int index){
            //std::cout << "onSequenceDel: " << index << std::endl;
            deleteTrack(index);
        };

        auto onSequenceDup = [&](int index){
            std::cout << "onSequenceDel: " << index << std::endl;
            MySequence::MySequenceItem & item = mySequence.myItems[index];
            int st = item.mFrameStart;
            int end = item.mFrameEnd;
            int type = item.mType;
            addTrack(type, st, end, false);
        };

        SequencerGui(&mySequence,
                     &currentFrame,
                     &expanded,
                     &selectedEntry,
                     &firstFrame,
                     options,
                     onIndicatorMove,
                     onSequenceMove,
                     onSequenceAdd,
                     onSequenceDel,
                     onSequenceDup);
        
        
        ImGui::Dummy({0, 15});
        
        // add a UI to edit that particular item
        if (selectedEntry == -1)
        {
            ImGui::Text("%s", "No Sequence Item selected");
        }else
        {
            MySequence::MySequenceItem &item = mySequence.myItems[selectedEntry];
            int type = item.mType;
            int start = item.mFrameStart;
            int end = item.mFrameEnd;
            int total = end - start;
            ImGui::Text("%s", SequencerItemTypeNames[type]);
            
            shared_ptr<Shape> shape = nullptr;
            shared_ptr<Vezer> vezer = nullptr;
            shared_ptr<AnimHuman> human = nullptr;

            if (std::holds_alternative<shared_ptr<Shape>>(item.user)) {
                shape = std::get<shared_ptr<Shape>>(item.user);
            }

            if (std::holds_alternative<shared_ptr<Vezer>>(item.user)) {
                vezer = std::get<shared_ptr<Vezer>>(item.user);
            }

            if (std::holds_alternative<shared_ptr<AnimHuman>>(item.user)) {
                human = std::get<shared_ptr<AnimHuman>>(item.user);
            }

            if( checkUserIsValid(item.user) ){
                static bool disable_mouse_wheel = false;
                static bool disable_menu = false;
                static bool bordar = false;
                ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
                float height = 180;
                float width = ImGui::GetContentRegionAvail().x * 0.333f;
                {
                    // Left Gui Area
                    ImGui::BeginChild("ChildL", ImVec2(width, height), bordar, window_flags);
                    
                    if(type < 3 && type != (int)shape->type ){
                        ofLogError("Sequencer::draw()") << "Something went wrong about ShapeType";
                    }
                    
                    if(type==ShapeType::NONE ){
                        ofLogError("Sequencer::draw()") << "ShapeType::NONE detected";
                    }
                    
                    switch(type){
                        case ShapeType::FAN: drawGui_Fan(shape); break;
                        case ShapeType::RECT_SCREEN: drawGui_RectScreen(shape); break;
                        case ShapeType::ELLIPSE: drawGui_Ellipse(shape); break;
                        case 3: drawGui_VezerL(vezer); break;
                        case 4: drawGui_Human(human); break;
                        default: ofLogError("Sequencer::draw()") << "Something went wrong about ShapeType"; break;
                    }
                    
                    ImGui::EndChild();
                }
                
                ImGui::SameLine();
                
                {
                    /// Middle Gui Area
                    ImGui::BeginChild("ChildM", ImVec2(width, height), true, window_flags);
                    if(shape){
                        videoSection(shape);
                    }
                    
                    if(vezer){
                        drawGui_VezerM(vezer);
                    }
                    
                    // preview Vezer timelien, too small space
//                    if(vezer && vezer->isReady()){
//                        ofFbo & fbo = vezer->parser.getFbo();
//                        if(!fbo.isAllocated()){
//                            fbo.getTexture().getTextureData().bFlipTexture = true;
//                            vezer->prepareFbo(width, height);
//                        }
//                        ofxImGui::AddImage(fbo.getTexture(), glm::vec2(width, height));
//                    }
                    
                    ImGui::EndChild();
                }
                
                ImGui::SameLine();
             
                {
                    /// Middle Gui Area
                    ImGui::BeginChild("ChildR", ImVec2(width, height), true, window_flags);
                    ImGui::Text("Sequence Item");
                    ImGui::Text("[%02d] %s", selectedEntry, SequencerItemTypeNames[type]);
                    int min = mySequence.mFrameMin;
                    int max = mySequence.mFrameMax;
                    ImGui::SliderInt("start", &item.mFrameStart, min, max);
                    ImGui::SliderInt("end", &item.mFrameEnd, min, max);
                    ImGui::Text("total %d frames (%0.2f sec)", total, total/30.0f);
                    ImGui::EndChild();
                }
            } // end if(shape)
        }
    }
    ofxImGui::EndWindow(settings);

}

void Sequencer::drawGui_Fan(shared_ptr<Shape> & shape){
    
    auto fan = dynamic_pointer_cast<Fan>(shape);
    
    ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;

    ImGui::PushID("Fan");
    {
        ImGui::Checkbox("ON", (bool*)&fan->bOn.get() );
        ImGui::SameLine();
        if(ImGui::Checkbox("Test", (bool*)&fan->bShowTest.get())){
            fan->setup();
        }
        ImGui::SameLine();
        if(ImGui::Checkbox("Flip X", (bool*)&fan->bFlipX.get())){
            fan->setup();
        }

        if(ImGui::SliderInt("direction", (int*)&fan->direction.get(), fan->direction.getMin(), fan->direction.getMax())){
            fan->setup();
        }

        if(ImGui::SliderFloat3("position", (float*)&fan->position.get().x, fan->position.getMin().x, fan->position.getMax().x)){
            fan->setPosition(fan->position);
        }

        if(ImGui::SliderFloat("orientationY", (float*)&fan->orientationY.get(), fan->orientationY.getMin(), fan->orientationY.getMax())){
            glm::quat q = glm::angleAxis(glm::radians(fan->orientationY.get()), vec3(0,1,0));
            fan->setOrientation(q);
        }

        if(ImGui::SliderFloat("openAngle", (float*)&fan->openAngle.get(), fan->openAngle.getMin(), fan->openAngle.getMax())){
            fan->setup();
        }

        if(ImGui::SliderFloat("radius", (float*)&fan->radius.get(), fan->radius.getMin(), fan->radius.getMax())){
            fan->setup();
        }

        if(ImGui::SliderInt("resolution", (int*)&fan->resolution.get(), fan->resolution.getMin(), fan->resolution.getMax())){
            fan->setup();
        }
    }
    ImGui::PopID();
}

void Sequencer::drawGui_RectScreen(shared_ptr<Shape> & shape){

    auto rectScreen = dynamic_pointer_cast<RectScreen>(shape);
    
    ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;
    ImGui::PushID("RectScreen");
    {
        ImGui::Checkbox("ON", (bool*)&rectScreen->bOn.get());
        ImGui::SameLine();

        if(ImGui::Checkbox("Test", (bool*)&rectScreen->bShowTest.get())){
            rectScreen->setup();
        }
        ImGui::SameLine();
        if(ImGui::Checkbox("Flip X", (bool*)&rectScreen->bFlipX.get())){
            rectScreen->setup();
        }

        if(ImGui::SliderFloat3("position", (float*)&rectScreen->position.get().x, rectScreen->position.getMin().x, rectScreen->position.getMax().x)){
            rectScreen->setPosition(rectScreen->position);
        }

        if(ImGui::SliderFloat2("size", (float*)&rectScreen->size.get().x, rectScreen->size.getMin().x, rectScreen->size.getMax().x)){
            rectScreen->setup();
        }

        if(ImGui::SliderFloat("orientationY", (float*)&rectScreen->orientationY.get(), rectScreen->orientationY.getMin(), rectScreen->orientationY.getMax())){
            glm::quat q = glm::angleAxis(glm::radians(rectScreen->orientationY.get()), vec3(0,1,0));
            rectScreen->setOrientation(q);
        }
        
    }
    ImGui::PopID();
}

void Sequencer::drawGui_Ellipse(shared_ptr<Shape> & shape){

    auto ellipse = dynamic_pointer_cast<Ellipse>(shape);
    
    ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;
    ImGui::PushID("Ellipse");
    {
        ImGui::Checkbox("ON", (bool*)&ellipse->bOn.get());
        ImGui::SameLine();

        if(ImGui::Checkbox("Test", (bool*)&ellipse->bShowTest.get())){
            ellipse->setup();
        }
        ImGui::SameLine();
        if(ImGui::Checkbox("Flip X", (bool*)&ellipse->bFlipX.get())){
            ellipse->setup();
        }

        if(ImGui::SliderFloat3("position", (float*)&ellipse->position.get().x, ellipse->position.getMin().x, ellipse->position.getMax().x)){
            ellipse->setPosition(ellipse->position);
        }

        if(ImGui::SliderFloat("radius", (float*)&ellipse->radius.get(), ellipse->radius.getMin(), ellipse->radius.getMax())){
            ellipse->setup();
        }

        if(ImGui::SliderFloat("center Width", (float*)&ellipse->centerWidth.get(), ellipse->centerWidth.getMin(), ellipse->centerWidth.getMax())){
            ellipse->setup();
        }

        if(ImGui::SliderFloat("orientationY", (float*)&ellipse->orientationY.get(), ellipse->orientationY.getMin(), ellipse->orientationY.getMax())){
            glm::quat q = glm::angleAxis(glm::radians(ellipse->orientationY.get()), vec3(0,1,0));
            ellipse->setOrientation(q);
        }
    }
    ImGui::PopID();
}

void Sequencer::drawGui_VezerL(shared_ptr<Vezer> & vezer){
    
    if(vezer->isReady()){

        string filepath = ofToDataPath(vezer->filepath.get(), true);
        filesystem::path p(filepath);
        ImGui::Text("File Name     : %s", p.filename().c_str());

        {
            ImGui::SameLine();
            ImGui::TextDisabled("(path)");
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort) && ImGui::BeginTooltip())
            {
                ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                ImGui::TextUnformatted(filepath.c_str());
                ImGui::PopTextWrapPos();
                ImGui::EndTooltip();
            }
        }

        
        if( ImGui::Button("Load Vezer XML file") ){
            io::dialogueOpenVezer(vezer);
        }
        ImGui::Text("current frame : %d", vezer->getCurrentFrame());
        ImGui::Text("total frames  : %d", vezer->getTotalFrames());
        
        if(ImGui::TreeNodeEx("OSC Logger")){
            if(ImGui::SliderInt("Queue Size", (int*)&vezer->maxMsgQueueNum.get(), vezer->maxMsgQueueNum.getMin(), vezer->maxMsgQueueNum.getMax())){
                vezer->resizeQueue();
            }
            gui::Helper::drawMsgLogger( vezer->msgQueue );
            ImGui::TreePop();
        }
    }else{
        ImGui::Text("Vezer file not loaded");
    }
    
}

void Sequencer::drawGui_VezerM(shared_ptr<Vezer> & vezer){
    
    Body & body = vezer->body;
    
    ImGui::Text("Skeleton");
  
    if(ImGui::SliderFloat3("offset", (float*)&body.offset.get().x, body.offset.getMin().x, body.offset.getMax().x)){
    }
    
//    if(ImGui::SliderFloat3("Scale", (float*)&body.scale.get().x, body.scale.getMin().x, body.scale.getMax().x)){
//    }

    if(ImGui::Button("Set Offset")){
        vec3 prevOffset = body.offset;
        float y = prevOffset.y;
        vec3 r = body.rootPos - prevOffset;
        body.offset = vec3(-r.x, y, -r.z);
    }

    ImGui::SameLine();

    if( ImGui::Button("Reset Offset")){
        vec3 prevOffset = body.offset;
        float y = prevOffset.y;
        body.offset = vec3(0, y, 0);
    }
    
    if (ImGui::TreeNodeEx("Filter")) {
        vector<string> fTypes{"none", "legacy", "OneEuro"};
        ofxImGui::VectorCombo("Filter Type", (int *) &body.filterType.get(), fTypes);
        if (body.filterType == 0) {
        } else if (body.filterType == 1) {
            ImGui::SliderFloat(body.lowpass.getName().c_str(), (float*)&body.lowpass.get(), body.lowpass.getMin(), body.lowpass.getMax());
        } else if (body.filterType == 2) {
            gui::Helper::DragDouble(body.frequency, 1);
            gui::Helper::DragDouble(body.mincutoff);
            gui::Helper::DragDouble(body.beta);
            gui::Helper::DragDouble(body.dcutoff);
        }
        ImGui::TreePop();
    }
}

void Sequencer::drawGui_Human(shared_ptr<AnimHuman> & human){
    if(ImGui::SliderFloat3("position", (float*)&human->position.get().x, human->position.getMin().x, human->position.getMax().x)){
        //human->setPosition(human->root.position);
    }
                    
    if(ImGui::SliderFloat("orientation Y", (float*)&human->orientationY.get(), -360, 360)){
//        glm::quat q = glm::angleAxis(glm::radians(human->orientationY.get()), vec3(0,1,0));
//        human->root.setOrientation(q);
    }
    
    if(ImGui::Button("Reset")){
        human->position = vec3(0);
        human->orientationY = 0;
    }
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

        if( checkUserIsValid(item.user) ){
            if (std::holds_alternative<shared_ptr<Shape>>(item.user)) {
                auto & shape = std::get<shared_ptr<Shape>>(item.user);
                ofJson shapeJson;
                ofSerialize(shapeJson, shape->grp);
                t["shape"] = shapeJson;
            }else if (std::holds_alternative<shared_ptr<Vezer>>(item.user)) {
                auto & vezer = std::get<shared_ptr<Vezer>>(item.user);
                ofJson vezerJson;
                ofSerialize(vezerJson, vezer->grp);
                t["vezer"] = vezerJson["vezer"];
            }else if( std::holds_alternative<shared_ptr<AnimHuman>>(item.user)) {
                auto & human = std::get<shared_ptr<AnimHuman>>(item.user);
                ofJson humanJson;
                ofSerialize(humanJson, human->grp);
                
                // Curve points
                const vector<vector<ImVec2>> & pts = item.rampEdit.mPts;
                if(pts.size() > 0){
                 
                    ofJson cj = ofJson::array();
                    for(int i=0; i<pts.size(); i++){
                        
                        ofJson ptsJ = ofJson::array();

                        const vector<ImVec2> & vec = pts[i];
                        auto it = vec.begin();
                        for(; it!=vec.end(); ++it){
                            const ImVec2 & p = *it;
                            ofJson ptJ;// = ofJson::array();
                            ptJ["type"] = "smooth";
                            ptJ["x"] = ofToString(p.x);
                            ptJ["y"] = ofToString(p.y);
                            ptsJ.push_back(ptJ);
                        }
                        
                        cj.push_back(ptsJ);
                    }
                    
                    humanJson["curve"] = cj;
                }
                
                t["human"] = humanJson;

            }
        }

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
        
            deleteTrackAll();
            
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
                
                auto itShape = j.find("shape");
                if( itShape != j.end()){
                    SequenceUser u = addTrack(type, st, end, false);
                    auto & s = std::get<shared_ptr<Shape>>(u);
                    if(s && s->grp){
                        ofDeserialize(*itShape, s->grp);
                    }
                }
             
                auto itVezer = j.find("vezer");
                if( itVezer != j.end()){
                    SequenceUser u = addTrack(type, st, end, false);
                    auto & v = std::get<shared_ptr<Vezer>>(u);
                    ofJson tmp;
                    tmp["vezer"] = *itVezer;
                    ofDeserialize(tmp, v->grp);
                    v->load();
                }
                                
                auto itHuman = j.find("human");
                if( itHuman != j.end()){
                    SequenceUser u = addTrack(type, st, end, false);
                    auto & h = std::get<shared_ptr<AnimHuman>>(u);
                    if(h && h->grp){
                        ofDeserialize(*itHuman, h->grp);
                    }
                    
                    auto cit = itHuman->find("curve");
                    if(cit != itHuman->end()){
                        auto & item = mySequence.myItems.back();
                        vector<vector<ImVec2>> & pts = item.rampEdit.mPts;
                        pts.clear();
                        ofJson ptsJ = *cit;
                        
                        for(int i=0; i<ptsJ.size(); i++){
                            pts.emplace_back( vector<ImVec2>() );
                            vector<ImVec2> & pt = pts.back();
                            
                            auto pit = ptsJ[i].begin();
                            for(; pit!=ptsJ[i].end(); ++pit){
                                ofJson pj = *pit;
                                string type = pj.value("type", "smooth");
                                float x = ofToFloat(pj.value("x", "0"));
                                float y = ofToFloat(pj.value("y", "0"));
                                pt.emplace_back(x, y); 
                            }
                        }
                        
                        item.rampEdit.sortAll();
                    }
                }
            }
            
            return true;
        }
    }
    return false;
}

void Sequencer::videoSection(shared_ptr<Shape> s){

    ImGui::Text("Video");

    string filepath = ofToDataPath(s->videoPath.get(), true);
    filesystem::path p(filepath);
    ImGui::TextWrapped("File Name     : %s", p.filename().c_str());

    {
        ImGui::SameLine();
        ImGui::TextDisabled("(path)");
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort) && ImGui::BeginTooltip())
        {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(filepath.c_str());
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    const ofVideoPlayer & vid = s->getVideo();
    if( vid.isLoaded() ){
        int nFrames = vid.getTotalNumFrames();
        float duration = vid.getDuration();
        float frameRate = (float)nFrames / duration;
        int currentFrame = vid.getCurrentFrame();
        bool isPlaying = vid.isPlaying();
        bool isLoop = vid.getLoopState() != OF_LOOP_NONE;
        ImGui::Text("current frame : %5d frame", currentFrame);
        ImGui::Text("total frames  : %5d frames, (%3.2f sec)", nFrames, duration);
        ImGui::Text("frame rate    : %2.2f fps", frameRate);
        ImGui::Text("state         : %s, %s", isPlaying ? "Playing" : "Not playing", isLoop ? "Loop" : "Loop None");
        if(ImGui::SliderFloat("volume", (float *)&s->videoVolume.get(), s->videoVolume.getMin(), s->videoVolume.getMax())){
            s->setVideoVolume(s->videoVolume);
        }
        
    }else{
        ImGui::Text("%s", "Video is not loaded");
    }

    ImGui::Dummy({0, 10});

    if(ImGui::Button("Load Video")){
        string filepath = io::dialogueLoadVideo();
        if( filepath != ""){
            s->loadVideo(filepath);
        }
    }
    ImGui::SameLine();
    if(ImGui::Button("Reload")){
        s->loadVideo();
    }
}

bool Sequencer::checkUserIsValid(const SequenceUser & user){
    
    if(user.index() == 0){
        ofLogError("Sequencer") << "Error: User is monostate";
        return false;
    }
    return true;
}
