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

shared_ptr<Shape> Sequencer::duplicateTrack( const shared_ptr<Shape> & shape, int st, int end, bool bExpanded ){

    shared_ptr<Shape> newShape = nullptr;

    if(!shape){
        ofLogError("Sequencer::addTrack") << "shape is nullptr";
        return newShape;
    }

    ShapeType type = shape->type;
    switch (type){
        case ShapeType::FAN:
        {
            auto fan = dynamic_pointer_cast<Fan>(shape);
            newShape = make_shared<Fan>(*fan);
            newShape->setup();
            mySequence.myItems.push_back(MySequence::MySequenceItem{ type, st, end, bExpanded, newShape});
            ofLogNotice("Sequencer::AddTrack() - ") << "Fan";
        }
            break;
        case ShapeType::RECT_SCREEN:
        {
            auto r = dynamic_pointer_cast<RectScreen>(shape);
            newShape = make_shared<RectScreen>(*r);
            newShape->setup();
            mySequence.myItems.push_back(MySequence::MySequenceItem{ type, st, end, bExpanded, newShape});
            ofLogNotice("Sequencer::AddTrack() - ") << "RECT_SCREEN";
        }
            break;
        case ShapeType::ELLIPSE:
        {
            auto e = dynamic_pointer_cast<Ellipse>(shape);
            newShape = make_shared<Ellipse>(*e);
            newShape->setup();

            mySequence.myItems.push_back(MySequence::MySequenceItem{ type, st, end, bExpanded, newShape});
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
    return newShape;
}

shared_ptr<Shape> Sequencer::addTrackShape( ShapeType type, int st, int end, bool bExpanded ){
    
    shared_ptr<Shape> shape = nullptr;
    
    switch (type){
        case ShapeType::FAN:
        {
            shape = make_shared<Fan>();
            //shapes.emplace_back( shape );
            mySequence.myItems.push_back(MySequence::MySequenceItem{ type, st, end, bExpanded, shape});
            ofLogNotice("Sequencer::AddTrack() - ") << "Fan";
        }
            break;
        case ShapeType::RECT_SCREEN:
        {
            shape = make_shared<RectScreen>();
            //shapes.emplace_back( shape );
            mySequence.myItems.push_back(MySequence::MySequenceItem{ type, st, end, bExpanded, shape});
            ofLogNotice("Sequencer::AddTrack() - ") << "RECT_SCREEN";
        }
            break;
        case ShapeType::ELLIPSE:
        {
            shape = make_shared<Ellipse>();
            //shapes.emplace_back( shape );
            mySequence.myItems.push_back(MySequence::MySequenceItem{ type, st, end, bExpanded, shape});
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
    
    return shape;
}

void Sequencer::deleteTrack(int index){
    
    mySequence.myItems.erase(mySequence.myItems.begin() + index);

    // delete from ofApp::shapes container
//    MySequence::MySequenceItem & item = mySequence.myItems[index];
//    vector<shared_ptr<Shape>> & shapes = ofApp::get()->shapes;
//    std::remove( shapes.begin(), shapes.end(), item.shape );
}

void Sequencer::startTrack(const std::variant<shared_ptr<Shape>, shared_ptr<Human>, shared_ptr<Vezer>> user, int frame){
    if (std::holds_alternative<shared_ptr<Shape>>(user)) {
        auto & shape = std::get<shared_ptr<Shape>>(user);
        if(shape){
            shape->start(frame);
        }else{
            ofLogError("Sequencer::startTrack") << "shape is null";
        }
    }else {
        // human
        // vezer
    }
}

void Sequencer::stopTrack(const std::variant<shared_ptr<Shape>, shared_ptr<Human>, shared_ptr<Vezer>> user){
    if (std::holds_alternative<shared_ptr<Shape>>(user)) {
        auto & shape = std::get<shared_ptr<Shape>>(user);
        if(shape){
            shape->stop();
        }else{
            ofLogError("Sequencer::startTrack") << "shape is null";
        }
    }else{
        // human
        // vezer
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
    {
        if(st == currentFrame){
            startTrack(item.user, 0);
            ofLogVerbose("Sequencer") << entry << " : start";
        }
        else if(st < currentFrame && currentFrame < end){
            // we can not call start too often
            if(bSeek){
                int frame = currentFrame - st;
                startTrack(item.user, frame);
                ofLogVerbose("Sequencer") << entry << " : seek, frame=" << frame;
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
        ImGui::Text("(%3.2f sec)", currentFrame/30.0f);
        
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
            if(type < 3){
                // 0, 1, 2
                addTrackShape((ShapeType)type);
            }else if(type == 3){
                // Human
            }else if(type == 4){
                // Vezer
            }
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
            
            if(type < 3){
                // 0, 1, 2
                addTrackShape((ShapeType)type, st, end, false);
            }else if(type == 3){
                // Human
            }else if(type == 4){
                // Vezer
            }
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
            if (std::holds_alternative<shared_ptr<Shape>>(item.user)) {
                auto & shape = std::get<shared_ptr<Shape>>(item.user);

                static bool disable_mouse_wheel = false;
                static bool disable_menu = false;
                static bool bordar = false;
                ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
                float height = 160;
                float width = ImGui::GetContentRegionAvail().x * 0.333f;
                {
                    // Left Gui Area
                    ImGui::BeginChild("ChildL", ImVec2(width, height), bordar, window_flags);
                    
                    if(type != (int)shape->type ){
                        ofLogError("Sequencer::draw()") << "Something went wrong about ShapeType";
                    }
                    
                    if(type==ShapeType::NONE ){
                        ofLogError("Sequencer::draw()") << "ShapeType::NONE detected";
                    }
                    
                    switch(type){
                        case ShapeType::FAN: drawGui_Fan(shape); break;
                        case ShapeType::RECT_SCREEN: drawGui_RectScreen(shape); break;
                        case ShapeType::ELLIPSE: drawGui_Ellipse(shape); break;
                        default: ofLogError("Sequencer::draw()") << "Something went wrong about ShapeType"; break;
                    }
                    
                    ImGui::EndChild();
                }
                
                ImGui::SameLine();
                
                {
                    /// Middle Gui Area
                    ImGui::BeginChild("ChildM", ImVec2(width, height), true, window_flags);
                    videoSection(shape);
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


        if (std::holds_alternative<shared_ptr<Shape>>(item.user)) {
            auto & shape = std::get<shared_ptr<Shape>>(item.user);
            ofJson shapeJson;
            ofSerialize(shapeJson, shape->grp);
            t["shape"] = shapeJson;
        }else{
            
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
                    shared_ptr<Shape> s = addTrackShape((ShapeType)type, st, end, false);
                    if(s && s->grp)
                        ofDeserialize(*itShape, s->grp);
                }
                
                auto itHuman = j.find("human");
                if( itHuman != j.end()){
                    //ofDeserialize(*itHuman, h->grp);
                }
                
                auto itVezer = j.find("vezer");
                if( itVezer != j.end()){
                    //ofDeserialize(*itVezer, v->grp);
                }
                return true;
            }
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
        ImGui::Text("state         : %s", isPlaying ? "Playing" : "Not playing");
        ImGui::Text("loop          : %s", isLoop ? "Loop" : "Loop None");
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
}
