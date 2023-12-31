//
//  GuiHelper.cpp
//  BreathShowApp
//
//  Created by Hiroshi Matoba on 29.11.23.
//

#include "ofApp.h"
#include "Helper.h"
#include "io.h"

namespace gui{

glm::vec2 mouseOffset{0};

void Helper::menu(const shared_ptr<ofApp> & app) {
    
    if (ImGui::BeginMenuBar()){
        if (ImGui::BeginMenu("File")){
            
            //if (ImGui::MenuItem("New")) {}
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                io::dialogueLoadProject();
            }
            
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                io::saveProject(app->currentProjectPath);
            }
            
            if (ImGui::MenuItem("Save As", "Ctrl+Shift+S")) {
                io::dialogueSaveProject();
            }
            
            ImGui::EndMenu();
        }
        
        ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
        
        if (ImGui::BeginMenu("Panel")) {
            //ImGui::Checkbox("Demo", (bool *) &bDemoOpen.get());
            ImGui::Checkbox("3D Scene", (bool *) &app->b3dSceneOpen.get());
            ImGui::Checkbox("Human Panel", (bool *) &app->bHumanOpen.get());
            ImGui::Checkbox("Projector Panel", (bool *) &app->bProjectorOpen.get());
            ImGui::Checkbox("Sequencer Panel", (bool *) &app->bSeqOpen.get());
            ImGui::Checkbox("Window Manager Panel", (bool *) &app->bWindowManagerOpen.get());
            ImGui::EndMenu();
        }
        
        ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
        ImGui::Text("%3d fps", (int)ofGetFrameRate());
        ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

        ImGui::PushItemWidth(50);
        if(ImGui::DragInt("Target fps", (int*)&app->targetFps.get(), 1, 1, 200, "%2d")){
        }
        
        ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
        if(ImGui::Checkbox("Play", (bool*)&app->bPlay.get())){
            app->setPlay(app->bPlay);
        }
        
        ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
        if(ImGui::Checkbox("Loop", (bool*)&app->bLoop.get())){
            app->setLoop(app->bLoop);
        }
        ImGui::EndMenuBar();
    }
}

void Helper::dock(const shared_ptr<ofApp> & app) {
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;
    
    static bool p_open = NULL;
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    
    ImGui::Begin("CentralNode", &p_open, window_flags);
    
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);
    
    string nameSt = "MyDockspace";
    const char *name = nameSt.c_str();
    
    if (ImGui::DockBuilderGetNode(ImGui::GetID(name)) == NULL) {
        ImGuiID dockspace_id = ImGui::GetID(name);
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags);
        ImGuiID dock_main_id = dockspace_id;
        ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, NULL, &dock_main_id);
        
        ImGui::DockBuilderDockWindow("3D Scene", dock_main_id);
        ImGui::DockBuilderFinish(dockspace_id);
    }
    
    // DockSpace
    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID(name);
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    
    menu(app);
    
    ImGui::End();
    ImGui::PopStyleColor();
}

ofRectangle Helper::calcCameraControlArea(){
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

}; // namespace gui
