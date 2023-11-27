#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "imgui_internal.h"
#include "ImHelpers.h"
#include "Projector.h"
#include "RenderWindow.h"
#include "Human.h"
#include "Shape.h"
#include "Sequencer.h"

using glm::vec2;
using glm::vec3;

class ofApp : public ofBaseApp{

	public:
        
        static shared_ptr<ofApp> app;
        static shared_ptr<ofApp> & get(){
            if(app==nullptr) app = make_shared<ofApp>();
            return app;
        }
    
		void setup();
		void update();
		void draw();
		void keyPressed(ofKeyEventArgs & args);
		void mousePressed(ofMouseEventArgs & args);
		
        void drawProjector(ofEventArgs & args);
        void drawProjectorFbo();
        void drawFloor();
        
        void dialogueSaveProject();
        void dialogueLoadProject();
        bool saveProject(string dirpath);
        bool loadProject(string dirpath);
    
        void setPlay(bool b);
        void setLoop(bool b);
    
        void exit();
    private:
        void dock();
        void menu();
    
        // Window3D.cpp
        void draw3DWindow();
        void draw3DScene();
    
        // WindowGui.cpp
        void drawGui();
        void drawGui_WindowManager();
        void drawGui_ShowSettings();
        
        void drawGui_Human();

    
        // WindowSequencer.cpp
        void drawGui_Sequencer();
    
        ofRectangle calcCameraControArea();

        ofFbo::Settings getFboSettings(int w, int h, bool bUseDepth=false, GLint glFormat=GL_RGBA8, GLenum glTexTarget=GL_TEXTURE_2D);
        ofxImGui::Gui gui;
        ofFbo fbo3dScene;
        ofEasyCam cam;
    
        glm::vec2 mouseOffset{0,0};
    
    public:
        ofParameter<int> targetFps{"target fps", 60, 0, 120};
        ofParameter<bool> bPlay{"Play", true};
        ofParameter<bool> bLoop{"Loop", true};
        ofParameterGroup genGrp{"General", targetFps, bPlay, bLoop};

        ofParameter<bool> b3dSceneOpen{"3DScene Window Open", true};
        ofParameter<bool> bDemoOpen{"Demo Window Open", true};
        ofParameter<bool> bGuiOpen{"Gui Panel Open", true};
        ofParameter<bool> bSeqOpen{"Sequencer Panel Open", false};
        ofParameter<bool> bWindowManagerOpen{"Window Manager Open", true};
        ofParameterGroup panelGrp{"Panels", b3dSceneOpen, bDemoOpen, bGuiOpen, bSeqOpen, bWindowManagerOpen};
        
        ofParameterGroup grp{"app settings", genGrp, panelGrp};

        Human human;
        //vector<shared_ptr<Shape>> shapes;
            
        string currentProjectPath;
    
    public:
        std::shared_ptr<MainWindow> mainWindow;
        std::shared_ptr<ProjectorWindow> projectorWindow;
        Projector projector;
        Sequencer sequencer;
};
