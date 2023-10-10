#include "ofApp.h"

void ofApp::drawGui()
{
    drawGui_WindowManager();
    drawGui_ShowSettings();
}

void ofApp::drawGui_ShowSettings()
{
    auto settings = ofxImGui::Settings();
    
    if(ofxImGui::BeginWindow("Gui", settings, false, (bool*)&bGuiOpen.get())) {
        
        ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;
        ImGui::PushID("Human");
        
        if(ImGui::CollapsingHeader("Human", flag)){
            
            if(ImGui::SliderFloat3("position", (float*)&humanPosition.get().x, humanPosition.getMin().x, humanPosition.getMax().x)){
                human.setPosition(humanPosition.get().x, humanPosition.get().y, humanPosition.get().z);
            }

            if(ImGui::SliderFloat3("scale", (float*)&humanScale.get().x, humanScale.getMin().x, humanScale.getMax().x)){
                human.setScale(humanScale.get().x, humanScale.get().y, humanScale.get().z) ;
            }
            
            if(ImGui::SliderFloat("orientationY", (float*)&humanOrientationY.get(), humanOrientationY.getMin(), humanOrientationY.getMax())){
                human.setRotation(0, humanOrientationY.get(), 0, 1, 0);
            }
        }
        ImGui::PopID();

        
        ImGui::PushID("Fan L");
        if(ImGui::CollapsingHeader("Fan L", flag)){
            if(ImGui::SliderInt("direction", (int*)&fanL.direction.get(), fanL.direction.getMin(), fanL.direction.getMax())){
            }

            if(ImGui::SliderFloat3("position", (float*)&fanL.position.get().x, fanL.position.getMin().x, fanL.position.getMax().x)){
            }
            
            if(ImGui::SliderFloat3("scale", (float*)&fanL.scale.get().x, fanL.scale.getMin().x, fanL.scale.getMax().x)){
            }
            
            if(ImGui::SliderFloat("orientationY", (float*)&fanL.orientationY.get(), fanL.orientationY.getMin(), fanL.orientationY.getMax())){
            }

            if(ImGui::SliderFloat("openAngle", (float*)&fanL.openAngle.get(), fanL.openAngle.getMin(), fanL.openAngle.getMax())){
            }
            
            if(ImGui::SliderFloat("length", (float*)&fanL.length.get(), fanL.length.getMin(), fanL.length.getMax())){
            }
        }
        ImGui::PopID();

        
        ImGui::PushID("Fan R");
        if(ImGui::CollapsingHeader("Fan R", flag)){
            if(ImGui::SliderInt("direction", (int*)&fanR.direction.get(), fanR.direction.getMin(), fanR.direction.getMax())){
            }

            if(ImGui::SliderFloat3("position", (float*)&fanR.position.get().x, fanR.position.getMin().x, fanR.position.getMax().x)){
            }
            
            if(ImGui::SliderFloat3("scale", (float*)&fanR.scale.get().x, fanR.scale.getMin().x, fanR.scale.getMax().x)){
            }
            
            if(ImGui::SliderFloat("orientationY", (float*)&fanR.orientationY.get(), fanR.orientationY.getMin(), fanR.orientationY.getMax())){
            }
            
            if(ImGui::SliderFloat("openAngle", (float*)&fanR.openAngle.get(), fanR.openAngle.getMin(), fanR.openAngle.getMax())){
            }

            if(ImGui::SliderFloat("length", (float*)&fanR.length.get(), fanR.length.getMin(), fanR.length.getMax())){
            }
        }
        ImGui::PopID();
        
        ofxImGui::AddGroup(projector.getParametersRef(), settings);
        
    }
    ofxImGui::EndWindow(settings);
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
