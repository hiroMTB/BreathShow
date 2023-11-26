#include "ofApp.h"

void ofApp::drawGui()
{
    if(bWindowManagerOpen) drawGui_WindowManager();
    if(bGuiOpen) drawGui_ShowSettings();
    if(bSeqOpen) drawGui_Sequencer();
    if(b3dSceneOpen) draw3DWindow();
    //if(bDemoOpen) ImGui::ShowDemoWindow();
}

void ofApp::drawGui_ShowSettings()
{
    auto settings = ofxImGui::Settings();
    
    if(ofxImGui::BeginWindow("Gui", settings, false, (bool*)&bGuiOpen.get())) {
        
        ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;
        ImGui::PushID("Human");
        
        if(ImGui::CollapsingHeader("Human", flag)){
            ImGui::Checkbox("ON", (bool*)&human.bOn.get());
            if(ImGui::SliderFloat3("position", (float*)&human.root.position.get().x, human.root.position.getMin().x, human.root.position.getMax().x)){
                human.root.setPosition(human.root.position);
            }

            if(ImGui::SliderFloat3("scale", (float*)&human.scale.get().x, human.scale.getMin().x, human.scale.getMax().x)){
                human.model.setScale(human.scale.get().x, human.scale.get().y, human.scale.get().z) ;
            }
            
            if(ImGui::SliderFloat("orientation Y", (float*)&human.orientationY.get(), -360, 360)){
                glm::quat q = glm::angleAxis(glm::radians(human.orientationY.get()), vec3(0,1,0));
                human.root.setOrientation(q);
            }
        }
        ImGui::PopID();
        
        ImGui::PushID("Fan L");
        if(ImGui::CollapsingHeader("Fan L", flag)){
            ImGui::Checkbox("ON", (bool*)&fanL.bOn.get() );
            ImGui::SameLine();
            if(ImGui::Checkbox("Test", (bool*)&fanL.bShowTest.get())){
                fanL.setup();
            }
            ImGui::SameLine();
            if(ImGui::Checkbox("Flip X", (bool*)&fanL.bFlipX.get())){
                fanL.setup();
            }

            if(ImGui::SliderInt("direction", (int*)&fanL.direction.get(), fanL.direction.getMin(), fanL.direction.getMax())){
                fanL.setup();
            }

            if(ImGui::SliderFloat3("position", (float*)&fanL.position.get().x, fanL.position.getMin().x, fanL.position.getMax().x)){                
                fanL.setPosition(fanL.position);
            }
                
            if(ImGui::SliderFloat("orientationY", (float*)&fanL.orientationY.get(), fanL.orientationY.getMin(), fanL.orientationY.getMax())){
                glm::quat q = glm::angleAxis(glm::radians(fanL.orientationY.get()), vec3(0,1,0));
                fanL.setOrientation(q);
            }

            if(ImGui::SliderFloat("openAngle", (float*)&fanL.openAngle.get(), fanL.openAngle.getMin(), fanL.openAngle.getMax())){
                fanL.setup();
            }
            
            if(ImGui::SliderFloat("radius", (float*)&fanL.radius.get(), fanL.radius.getMin(), fanL.radius.getMax())){
                fanL.setup();
            }
            
            if(ImGui::SliderInt("resolution", (int*)&fanL.resolution.get(), fanL.resolution.getMin(), fanL.resolution.getMax())){
                fanL.setup();
            }
        }
        ImGui::PopID();

        
        ImGui::PushID("Fan R");
        if(ImGui::CollapsingHeader("Fan R", flag)){
            ImGui::Checkbox("ON", (bool*)&fanR.bOn.get());
            ImGui::SameLine();

            if(ImGui::Checkbox("Test", (bool*)&fanR.bShowTest.get())){
                fanR.setup();
            }
            ImGui::SameLine();
            if(ImGui::Checkbox("Flip X", (bool*)&fanR.bFlipX.get())){
                fanR.setup();
            }
            
            if(ImGui::SliderInt("direction", (int*)&fanR.direction.get(), fanR.direction.getMin(), fanR.direction.getMax())){
                fanR.setup();
            }

            if(ImGui::SliderFloat3("position", (float*)&fanR.position.get().x, fanR.position.getMin().x, fanR.position.getMax().x)){
            }
            
            if(ImGui::SliderFloat("orientationY", (float*)&fanR.orientationY.get(), fanR.orientationY.getMin(), fanR.orientationY.getMax())){
                glm::quat q = glm::angleAxis(glm::radians(fanR.orientationY.get()), vec3(0,1,0));
                fanR.setOrientation(q);
            }
            
            if(ImGui::SliderFloat("openAngle", (float*)&fanR.openAngle.get(), fanR.openAngle.getMin(), fanR.openAngle.getMax())){
                fanR.setup();
            }

            if(ImGui::SliderFloat("radius", (float*)&fanR.radius.get(), fanR.radius.getMin(), fanR.radius.getMax())){
                fanR.setup();
            }
            
            if(ImGui::SliderInt("resolution", (int*)&fanR.resolution.get(), fanR.resolution.getMin(), fanR.resolution.getMax())){
                fanR.setup();
            }

        }
        ImGui::PopID();
        
        ImGui::PushID("RectScreen");
        if(ImGui::CollapsingHeader("Rect Screen", flag)){
            ImGui::Checkbox("ON", (bool*)&rectScreen.bOn.get());
            ImGui::SameLine();

            if(ImGui::Checkbox("Test", (bool*)&rectScreen.bShowTest.get())){
                rectScreen.setup();
            }
            ImGui::SameLine();
            if(ImGui::Checkbox("Flip X", (bool*)&rectScreen.bFlipX.get())){
                rectScreen.setup();
            }

            if(ImGui::SliderFloat3("position", (float*)&rectScreen.position.get().x, rectScreen.position.getMin().x, rectScreen.position.getMax().x)){
                rectScreen.setPosition(rectScreen.position);
            }
            
            if(ImGui::SliderFloat2("size", (float*)&rectScreen.size.get().x, rectScreen.size.getMin().x, rectScreen.size.getMax().x)){
                rectScreen.setup();
            }
            
            if(ImGui::SliderFloat("orientationY", (float*)&rectScreen.orientationY.get(), rectScreen.orientationY.getMin(), rectScreen.orientationY.getMax())){
                glm::quat q = glm::angleAxis(glm::radians(rectScreen.orientationY.get()), vec3(0,1,0));
                rectScreen.setOrientation(q);
            }
        }
        ImGui::PopID();

        ImGui::PushID("Ellipse");
        if(ImGui::CollapsingHeader("Ellipse", flag)){
            ImGui::Checkbox("ON", (bool*)&ellipse.bOn.get());
            ImGui::SameLine();

            if(ImGui::Checkbox("Test", (bool*)&ellipse.bShowTest.get())){
                ellipse.setup();
            }
            ImGui::SameLine();
            if(ImGui::Checkbox("Flip X", (bool*)&ellipse.bFlipX.get())){
                ellipse.setup();
            }
            
            if(ImGui::SliderFloat3("position", (float*)&ellipse.position.get().x, rectScreen.position.getMin().x, rectScreen.position.getMax().x)){
                ellipse.setPosition(ellipse.position);
            }
            
            if(ImGui::SliderFloat("radius", (float*)&ellipse.radius.get(), ellipse.radius.getMin(), ellipse.radius.getMax())){
                ellipse.setup();
            }
            
            if(ImGui::SliderFloat("center Width", (float*)&ellipse.centerWidth.get(), ellipse.centerWidth.getMin(), ellipse.centerWidth.getMax())){
                ellipse.setup();
            }
            
            if(ImGui::SliderFloat("orientationY", (float*)&ellipse.orientationY.get(), ellipse.orientationY.getMin(), ellipse.orientationY.getMax())){
                glm::quat q = glm::angleAxis(glm::radians(ellipse.orientationY.get()), vec3(0,1,0));
                ellipse.setOrientation(q);
            }
        }
        ImGui::PopID();
        
        ofxImGui::AddGroup(projector.grp, settings);
        
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

void ofApp::drawGui_Sequencer(){
    sequencer.draw( (bool*)&bSeqOpen.get() );
}

