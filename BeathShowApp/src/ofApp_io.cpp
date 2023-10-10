//
//  ofApp_io.cpp
//  BeathShowApp
//
//

#include "ofApp.h"
#include "portable-file-dialogs.h"
#include "ofxCameraSaveLoad.h"

void ofApp::dialogueSaveProject(){
    auto dir = pfd::select_folder("Select directory to save project", ofToDataPath("./projects", true));
    string dirname = dir.result();
    if(ofDirectory::doesDirectoryExist(dirname, false)) {
        bool ok = saveProject(dirname);
    }
}

void ofApp::dialogueLoadProject(){
    auto dir = pfd::select_folder("Select directory to open project", ofToDataPath("./projects", true));
    string dirname = dir.result();
    if(ofDirectory::doesDirectoryExist(dirname, false)) {
        bool ok = loadProject(dirname);
    }
}

bool ofApp::saveProject(string dirpath){
    
    bool bRel = ofDirectory::doesDirectoryExist(dirpath, true);
    bool bAbs = ofDirectory::doesDirectoryExist(dirpath, false);
    if(!bRel && !bAbs){
        ofLogError("saveProject()") << "Directory does not exist: " << dirpath;
        return "No directory error";
    }
    
    dirpath = ofToDataPath(dirpath, true);
    bool bCamera = ofxSaveCamera(cam, dirpath + "/camera.settings");
    bool bProjector = projector.save(dirpath + "/projector.json");
    bool bFanL = fanL.save(dirpath + "/fanL.json");
    bool bFanR = fanR.save(dirpath + "/fanR.json");
    bool bWin = mainWindow->save(dirpath + "/mainWindow.json");
    bool bPrj = projectorWindow->save(dirpath + "/projectorWindow.json");
    
    return bCamera && bProjector && bFanL && bFanR && bWin && bPrj;
}

bool ofApp::loadProject(string dirpath){

    bool bRel = ofDirectory::doesDirectoryExist(dirpath, true);
    bool bAbs = ofDirectory::doesDirectoryExist(dirpath, false);
    if(!bRel && !bAbs){
        ofLogError("loadProject()") << "Directory does not exist: " << dirpath;
        return "No directory error";
    }

    ofFile file(dirpath); // actually this is directory
    string filename = file.getBaseName();
    ofSetWindowTitle(filename);
    
    bool bCamera = ofxLoadCamera(cam, dirpath + "/camera.settings");
    bool bProjector = projector.load(dirpath + "/projector.json");
    bool bFanL = fanL.load(dirpath + "/fanL.json");
    bool bFanR = fanR.load(dirpath + "/fanR.json");
    bool bWin = mainWindow->load(dirpath + "/mainWindow.json");
    bool bPrj = projectorWindow->load(dirpath + "/projectorWindow.json");
    
    return bCamera && bProjector && bFanL && bFanR && bWin && bPrj;
}
