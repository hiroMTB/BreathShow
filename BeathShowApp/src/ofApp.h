#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofxAssimpModelLoader.h"
#include "imgui_internal.h"
#include "ImHelpers.h"
#include "Projector.h"

#include "Fan.h"

using glm::vec2;
using glm::vec3;

class ofApp : public ofBaseApp{

	public:
    
		void setup();
		void update();
		void draw();
		void keyPressed(ofKeyEventArgs & args);
		void mousePressed(ofMouseEventArgs & args);
		
        void drawProjector(ofEventArgs & args);
        void drawProjectorFbo();

        void dialogueSaveProject();
        void dialogueLoadProject();
        bool saveProject(string dirpath);
        bool loadProject(string dirpath);

    private:
        void dock();
        void menu();
    
        // Window3D.cpp
        void draw3DWindow();
        void draw3DScene();
    
        // WindowGui.cpp
        void drawGui();
    
        ofRectangle calcCameraControArea();

        ofFbo::Settings getFboSettings(int w, int h, bool bUseDepth=false, GLint glFormat=GL_RGBA8, GLenum glTexTarget=GL_TEXTURE_2D);
        ofxImGui::Gui gui;
        ofFbo fbo3dScene;
        ofEasyCam cam;
    
        glm::vec2 mouseOffset{0,0};
        ofParameter<bool> b3dSceneOpen{"3DScene Window Open", true};
        ofParameter<bool> bDemoOpen{"Demo Window Open", true};
        ofParameter<bool> bGuiOpen{"Gui Panel Open", true};
    
        ofxAssimpModelLoader human;
        ofParameter<vec3> humanPosition{"position", vec3(0), vec3(-500), vec3(500)};
        ofParameter<vec3> humanScale{"scale", vec3(0.1,-0.1,0.1), vec3(-3), vec3(3)};
        ofParameter<float> humanOrientationY{"orientationY", 0, -360, 360};
        ofParameterGroup grpHuman{"human", humanPosition, humanOrientationY, humanScale};

        Fan fanL;
        Fan fanR;

        Projector projector;
    
        string currentProjectPath;
};
