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
    
    // 3d model
    bool bOptimize = true;
    bool bOk = human.loadModel("./3dModel/human.obj", bOptimize);
    if(bOk){
        ofLogNotice() << "OK! load human.obj";
        human.setScale(humanScale.get().x, humanScale.get().y, humanScale.get().z) ;
        human.setPosition(humanPosition.get().x, humanPosition.get().y, humanPosition.get().z);
        human.setRotation(0, humanOrientationY, 0, 1, 0);
    }else{
        ofLogError() << "Can not load human.obj";
    }
    
    loadProject("./projects/testProject");
}

void ofApp::update()
{
    fanL.update();
    fanR.update();
}

void ofApp::draw()
{
    ofBackground(0);
    gui.begin();
    
    dock();
    //ImGui::ShowDemoWindow();
    draw3DWindow();
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
                dialogueSaveProject();
            }
            
            if (ImGui::MenuItem("Save As", "Ctrl+Shift+S")) {
            }
            
            ImGui::EndMenu();
        }
        
        ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
        
        if (ImGui::BeginMenu("Panel")) {
            ImGui::Checkbox("Demo", (bool *) &bDemoOpen.get());
            ImGui::Checkbox("3D Scene", (bool *) &b3dSceneOpen.get());
            ImGui::Checkbox("Gui Panel", (bool *) &bGuiOpen.get());
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
        ofRectangle controlArea = ofRectangle(winPos.x, winPos.y, winSize.x, winSize.y);
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

}

void ofApp::mousePressed(ofMouseEventArgs & args){

}

