#include "ofApp.h"

void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofDisableArbTex();
    ofSetCircleResolution(64);
    
#ifndef USE_MULTI_VIEWPORT
    gui.setup(nullptr, false, ImGuiConfigFlags_DockingEnable, true);
#else
    // ImGuiConfigFlags_ViewportsEnable conflicts with ofxEasyCam controlArea
    gui.setup(nullptr, false, ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable, true);
#endif
    
    ofSetBackgroundAuto(false);
    ofEnableAlphaBlending();
    cam.setNearClip(1);
    cam.setFarClip(10000);
    
    currentProjectPath = "./projects/testProject";
    loadProject(currentProjectPath);
    
    
    mySequence.mFrameMin = -100;
    mySequence.mFrameMax = 1000;
    mySequence.myItems.push_back(MySequence::MySequenceItem{0,10,30,false});
    mySequence.myItems.push_back(MySequence::MySequenceItem{1,20,30,true});
    mySequence.myItems.push_back(MySequence::MySequenceItem{3, 12, 60, false});
    mySequence.myItems.push_back(MySequence::MySequenceItem{2, 61, 90, false});
    mySequence.myItems.push_back(MySequence::MySequenceItem{4, 90, 99, false});

}

void ofApp::update()
{
    drawProjectorFbo();
    fanL.update();
    fanR.update();
}

void ofApp::draw()
{
    ofBackground(0);
    gui.begin();
    
    dock();
    drawGui();
     
    gui.end();
    gui.draw();
}

void ofApp::dock(){
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
    window_flags |= ImGuiWindowFlags_NoBackground;
    
    static bool p_open = NULL;
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0,0,0,1));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    
    ImGui::Begin("CentralNode", &p_open, window_flags);
    
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);
    
    string nameSt = "MyDockspace";
    const char * name = nameSt.c_str();
    
    if (ImGui::DockBuilderGetNode(ImGui::GetID(name)) == NULL){
        ImGuiID dockspace_id = ImGui::GetID(name);
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags);
        ImGuiID dock_main_id = dockspace_id;
        ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, NULL, &dock_main_id);
        
        ImGui::DockBuilderDockWindow("3D Scene", dock_main_id);
        ImGui::DockBuilderFinish(dockspace_id);
    }
    
    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable){
        ImGuiID dockspace_id = ImGui::GetID(name);
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        
        
    }
    
    menu();
    
    ImGui::End();
    ImGui::PopStyleColor();
}

void ofApp::menu(){
    
    if (ImGui::BeginMenuBar()){
        if (ImGui::BeginMenu("File")){
            
            //if (ImGui::MenuItem("New")) {}
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                dialogueLoadProject();
            }
            
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                saveProject(currentProjectPath);
            }
            
            if (ImGui::MenuItem("Save As", "Ctrl+Shift+S")) {
                dialogueSaveProject();
            }
            
            ImGui::EndMenu();
        }
        
        ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
        
        if (ImGui::BeginMenu("Panel")) {
            ImGui::Checkbox("Demo", (bool *) &bDemoOpen.get());
            ImGui::Checkbox("3D Scene", (bool *) &b3dSceneOpen.get());
            ImGui::Checkbox("Gui Panel", (bool *) &bGuiOpen.get());
            ImGui::Checkbox("Sequencer Panel", (bool *) &bSeqOpen.get());
            ImGui::EndMenu();
        }
        
        ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
        ImGui::Text("%3.1f fps", ofGetFrameRate());
        
        ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
        
        ImGui::EndMenuBar();
    }
}

ofRectangle ofApp::calcCameraControArea(){
    ImVec2 max = ImGui::GetWindowContentRegionMax();
    ImVec2 min = ImGui::GetWindowContentRegionMin();
    
    // winSize includes padding and tab, so we use max - min instead
    ImVec2 winSize;
    winSize.x = max.x - min.x;
    winSize.y = max.y - min.y;
    
    ImVec2 winPos = ImGui::GetWindowPos(); // absolute window position
    winPos.x = winPos.x + min.x;
    winPos.y = winPos.y + min.y;
    
    if(winPos.x >= 0 && winPos.y >= 0){
        // store actual canvas start position in order to calculate mouse action properly
        mouseOffset = vec2(min.x, min.y);

        // make it smaller to avoid moving camera while draging neiboring panel
        int margin = 3;
        ofRectangle controlArea = ofRectangle(winPos.x+margin, winPos.y+margin, winSize.x-margin*2, winSize.y-margin*2);
        return controlArea;
    }
    return ofRectangle(0, 0, 0, 0);
}

ofFbo::Settings ofApp::getFboSettings(int w, int h, bool bUseDepth, GLint glFormat, GLenum glTexTarget) {
    ofFbo::Settings fboSettings;
    fboSettings.width = w;
    fboSettings.height = h;
    fboSettings.numSamples = 8;
    fboSettings.internalformat = glFormat;
    fboSettings.textureTarget = glTexTarget;
    fboSettings.useDepth = bUseDepth;
    return fboSettings;
}

void ofApp::keyPressed(ofKeyEventArgs & args)
{
    switch(args.key){
        case 's':
            if(ofGetKeyPressed(OF_KEY_CONTROL) || ofGetKeyPressed(OF_KEY_COMMAND)){
                if(ofGetKeyPressed(OF_KEY_SHIFT)){
                    dialogueSaveProject();
                }else{
                    saveProject(currentProjectPath);
                }
            }
            break;
        case 'o':
            if(ofGetKeyPressed(OF_KEY_CONTROL) || ofGetKeyPressed(OF_KEY_COMMAND)){
                dialogueLoadProject();
            }
            break;
            
        case ' ':
            bPlayVideo = !bPlayVideo;
            fanL.setPlayVideo(bPlayVideo);
            fanR.setPlayVideo(bPlayVideo);
            break;            
    }
}

void ofApp::mousePressed(ofMouseEventArgs & args){

}

void ofApp::exit(){
    
//    ofGetCurrentRenderer().reset();
//    //mainWindow->ofBaseWindow->setWindowShouldClose();
//    projectorWindow->ofBaseWindow->setWindowShouldClose();
//    ofGetCurrentWindow()->close();

    ofLogNotice("ofApp") << "exit()";
    
}
