#include "ofApp.h"
#include "Fan.h"
#include "RectScreen.h"
#include "Ellipse.h"

void ofApp::drawGui()
{
    if(bWindowManagerOpen) drawGui_WindowManager();
    if(bGuiOpen) drawGui_ShowSettings();
    if(bSeqOpen) drawGui_Sequencer();
    if(b3dSceneOpen) draw3DWindow();
    //if(bDemoOpen) ImGui::ShowDemoWindow();
}

void ofApp::drawGui_ShowSettings(){
    auto settings = ofxImGui::Settings();
    
    if(ofxImGui::BeginWindow("Gui", settings, false, (bool*)&bGuiOpen.get())) {
        drawGui_Human();
    }
    ofxImGui::AddGroup(projector.grp, settings);
    
    ofxImGui::EndWindow(settings);
}

void ofApp::drawGui_Human()
{
    ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;
    ImGui::PushID("Human");
    
    if(ImGui::CollapsingHeader("Human", flag)){
        ImGui::Checkbox("ON", (bool*)&human.bOn.get());
        if(ImGui::SliderFloat3("position", (float*)&human.root.position.get().x, human.root.position.getMin().x, human.root.position.getMax().x)){
            human.root.setPosition(human.root.position);
        }
        
        if(ImGui::SliderInt("height(cm)", (int*)&human.height.get(), human.height.getMin(), human.height.getMax())){
            //human.model.setScale(human.scale.get().x, human.scale.get().y, human.scale.get().z) ;
            human.setModelScale();
        }
        
        if(ImGui::SliderFloat("orientation Y", (float*)&human.orientationY.get(), -360, 360)){
            glm::quat q = glm::angleAxis(glm::radians(human.orientationY.get()), vec3(0,1,0));
            human.root.setOrientation(q);
        }
        
        if(bUseTracker){
            if(ImGui::Button("Set Offset")){
                float y = body.offset.get().y;
                body.offset = body.rootPos * vec3(-1, 0, -1);
                body.offset = vec3(body.offset.get().x, y, body.offset.get().z);
            }
            
            if(ImGui::SliderFloat3("offset", (float*)&body.offset.get().x, -100, 100)){
            }
        }
    }
    ImGui::PopID();
}
        
void ofApp::drawGui_WindowManager() {
    auto settings = ofxImGui::Settings();
    auto monitorNames = RenderWindow::monitorNames;
    
    if (ofxImGui::BeginWindow("Window Manager", settings, false, (bool *) &bWindowManagerOpen.get())) {
        if (ImGui::TreeNodeEx("Main Window", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::Checkbox("Open", (bool *) &mainWindow->bOpen.get())) {
                mainWindow->setOpen(mainWindow->bOpen);
            }
            if(ImGui::Checkbox("Fullscreen", (bool *) &mainWindow->bFull.get())){
                mainWindow->setFull(mainWindow->bFull);
            }
            ofxImGui::AddParameter(mainWindow->size);
            ofxImGui::AddParameter(mainWindow->position);
            ofxImGui::AddParameter(mainWindow->scale);
            if (ofxImGui::VectorCombo("Monitor", &mainWindow->monitorSelected, monitorNames)) {
                mainWindow->monitorName.set(monitorNames[mainWindow->monitorSelected]);
                mainWindow->setMonitor(monitorNames[mainWindow->monitorSelected]);
                mainWindow->monitorId.set(mainWindow->monitorSelected);
            }
            ImGui::TreePop();
        }
        
        if (ImGui::TreeNodeEx("Projector Window", ImGuiTreeNodeFlags_DefaultOpen)) {
            if(ImGui::Checkbox("Open", (bool *) &projectorWindow->bOpen.get())){
                projectorWindow->setOpen(projectorWindow->bOpen);
            };
            
            if(ImGui::Checkbox("Fullscreen", (bool *) &projectorWindow->bFull.get())){
                projectorWindow->setFull(projectorWindow->bFull);
            };
            
            ofxImGui::AddParameter(projectorWindow->size);
            ofxImGui::AddParameter(projectorWindow->position);
            ofxImGui::AddParameter(projectorWindow->scale);
            if (ofxImGui::VectorCombo("Monitor", &projectorWindow->monitorSelected, monitorNames)) {
                projectorWindow->monitorName.set(monitorNames[projectorWindow->monitorSelected]);
                projectorWindow->setMonitor(monitorNames[projectorWindow->monitorSelected]);
                projectorWindow->monitorId.set(projectorWindow->monitorSelected);
            }
            ImGui::TreePop();
        }
    }
    
    ofxImGui::EndWindow(settings);
}

void ofApp::drawGui_Sequencer(){
    sequencer.draw( (bool*)&bSeqOpen.get() );
}
