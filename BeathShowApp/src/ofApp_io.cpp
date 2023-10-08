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
        string error = saveProject(dirname);
    }
}

void ofApp::dialogueLoadProject(){
    auto dir = pfd::select_folder("Select directory to open project", ofToDataPath("./projects", true));
    string dirname = dir.result();
    if(ofDirectory::doesDirectoryExist(dirname, false)) {
        string error = loadProject(dirname);
    }
}

string ofApp::saveProject(string dirpath){
    
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

    stringstream ss;
    if(!bCamera)    ss << "Error saving camera.settings" << "\n";
    if(!bProjector) ss << "Error saving projector.json" << "\n";
    if(!bFanL)      ss << "Error saving fanL.json" << "\n";
    if(!bFanR)      ss << "Error saving fanR.json" << "\n";
    string error = ss.str();
    
    if(error == ""){
        ofLogNotice("saveProject()") << dirpath << " --> Success";
    }else{
        ofLogError("saveProject()") << dirpath << " --> Error" << endl << error;
    }
    
    return error;
}

string ofApp::loadProject(string dirpath){

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
    
    stringstream ss;
    if(!bCamera) ss << "Error loading camera.settings" << "\n";
    if(!bProjector) ss << "Error loading projector.json" << "\n";
    if(!bFanL)      ss << "Error loading fanL.json" << "\n";
    if(!bFanR)      ss << "Error loading fanR.json" << "\n";
    string error = ss.str();
    if(error == ""){
        ofLogNotice("loadProject()") << dirpath << " --> Success";
    }else{
        ofLogError("loadProject()") << dirpath << " --> Error" << endl << error;
    }
    return error;
}
