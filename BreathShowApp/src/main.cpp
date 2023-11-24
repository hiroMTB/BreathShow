#include "ofMain.h"
#include "ofApp.h"
#include "RenderWindow.h"

int main( ){

    ofDisableArbTex();

    // app packaging
    //ofSetDataPathRoot("../Resources/data/");
    
    // Create main window
    ofSetLogLevel("ofFbo", OF_LOG_NOTICE);
    ofSetLogLevel("ofShader", OF_LOG_NOTICE);
    ofSetLogLevel("ofxAssimpModelLoader", OF_LOG_NOTICE);
    ofSetLogLevel("ofMaterial", OF_LOG_NOTICE);

    ofSetEscapeQuitsApp(false);

    {
        shared_ptr<MainWindow> mainWindow = std::make_shared<MainWindow>();
        mainWindow->create("Main Window", nullptr, nullptr);
        
        shared_ptr<ofApp> & app = ofApp::get();
        shared_ptr<ProjectorWindow> prjWindow = std::make_shared<ProjectorWindow>();
        
        prjWindow->create("Projector Window", mainWindow->ofBaseWindow, app);
        
        app->mainWindow = mainWindow;
        app->projectorWindow = prjWindow;
        
        ofRunApp(mainWindow->ofBaseWindow, app);
        ofRunMainLoop();
    }
}
