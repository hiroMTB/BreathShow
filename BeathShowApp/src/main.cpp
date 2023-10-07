#include "ofMain.h"
#include "ofApp.h"

int main( ){

    // Create main window
    ofSetLogLevel("ofFbo", OF_LOG_NOTICE);
    ofSetLogLevel("ofShader", OF_LOG_NOTICE);
    ofSetLogLevel("ofxAssimpModelLoader", OF_LOG_NOTICE);

    ofGLFWWindowSettings mainWinSettings;
    mainWinSettings.setGLVersion(4, 1);
    mainWinSettings.setSize(1280, 720);
    mainWinSettings.resizable = true;
    mainWinSettings.decorated = true;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(mainWinSettings);

    // create a separate window for imGui
    ofGLFWWindowSettings prjWinSettings;
    prjWinSettings.setGLVersion(4, 1);
    prjWinSettings.setSize(1280, 720);
    prjWinSettings.resizable = true;
    prjWinSettings.decorated = true;
    prjWinSettings.shareContextWith = mainWindow;
    shared_ptr<ofAppBaseWindow> prjWindow = ofCreateWindow(prjWinSettings);
    
    shared_ptr<ofApp> mainApp(new ofApp);
    ofAddListener(prjWindow->events().draw, mainApp.get(), &ofApp::drawProjector);
    
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
}
