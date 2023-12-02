#include "ofApp.h"
#include "Fan.h"
#include "RectScreen.h"
#include "Ellipse.h"
#include "Helper.h"
#include "io.h"

void ofApp::drawGui()
{
    if(bWindowManagerOpen) drawGui_WindowManager();
    if(bHumanOpen) drawGui_Human();
    if(bProjectorOpen) drawGui_Projector();
    if(bSeqOpen) drawGui_Sequencer();
    if(b3dSceneOpen) draw3DWindow();
    //if(bDemoOpen) ImGui::ShowDemoWindow();
}

void ofApp::drawGui_Projector(){
    auto settings = ofxImGui::Settings();
    
    if(ofxImGui::BeginWindow("Projector", settings, false, (bool*)&bProjectorOpen.get())) {
        ofxImGui::AddGroup(projector.grp, settings);
    }
    ofxImGui::EndWindow(settings);
}

void ofApp::drawGui_Human()
{
    auto settings = ofxImGui::Settings();
   
    if(ofxImGui::BeginWindow("Human", settings, false, (bool*)&bHumanOpen.get())) {
       
        ImGui::Checkbox("ON", (bool*)&human.bOn.get());
                
        if(ImGui::SliderInt("height(cm)", (int*)&human.height.get(), human.height.getMin(), human.height.getMax())){
            //human.model.setScale(human.scale.get().x, human.scale.get().y, human.scale.get().z) ;
            human.setModelScale();
        }

        if(ImGui::SliderFloat3("position", (float*)&human.root.position.get().x, human.root.position.getMin().x, human.root.position.getMax().x)){
            human.root.setPosition(human.root.position);
        }
                        
        if(ImGui::SliderFloat("orientation Y", (float*)&human.orientationY.get(), -360, 360)){
            glm::quat q = glm::angleAxis(glm::radians(human.orientationY.get()), vec3(0,1,0));
            human.root.setOrientation(q);
        }
        
        if(ImGui::Button("Reset")){
            human.root.position = vec3(0);
            human.root.setPosition(human.root.position);
            human.orientationY = 0;
            human.root.setOrientation(vec3(0));
        }

        ///
        /// Tracker
        ///
        ImGui::Separator();
        ImGui::Checkbox("Live Tracking", (bool*)&bLiveTracking.get());
        
        if(bLiveTracking){
            
            if(ImGui::CollapsingHeader("Tracker", ImGuiTreeNodeFlags_DefaultOpen)){
                if(ImGui::TreeNodeEx("OSC", ImGuiTreeNodeFlags_DefaultOpen)){
                    if(ImGui::SliderInt("port", (int*)&osc.receivePort.get(), osc.receivePort.getMin(), osc.receivePort.getMax())){
                        osc.setupReceiver();
                    }
                    
                    if(ImGui::TreeNodeEx("OSC Logger")){
                        if(ImGui::SliderInt("Queue Size", (int*)&osc.maxMsgQueueNum.get(), osc.maxMsgQueueNum.getMin(), osc.maxMsgQueueNum.getMax())){
                            osc.resizeQueue();
                        }
                        gui::Helper::drawMsgLogger(osc.msgQueue);
                        ImGui::TreePop();
                    }
                    
                    ImGui::TreePop();
                }
            }
        }
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
