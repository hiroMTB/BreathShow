#pragma once

#include "ofApp.h"
#include "portable-file-dialogs.h"
#include "ofxCameraSaveLoad.h"
#include "Serializer.h"

namespace io{

inline bool saveProject(string dirpath){
    auto & app = ofApp::get();
    bool bRel = ofDirectory::doesDirectoryExist(dirpath, true);
    bool bAbs = ofDirectory::doesDirectoryExist(dirpath, false);
    if(!bRel && !bAbs){
        ofLogError("saveProject()") << "Directory does not exist: " << dirpath;
        return "No directory error";
    }
    
    dirpath = ofToDataPath(dirpath, true);
    
    bool bApp = Serializer::save(app->grp, dirpath + "/app.json");
    bool bCamera = ofxSaveCamera(app->cam, dirpath + "/camera.settings");
    bool bHuman = Serializer::save(app->human.grp, dirpath + "/human.json");
    bool bProjector = Serializer::save(app->projector.grp, dirpath + "/projector.json");
    bool bWin = app->mainWindow->save(dirpath + "/mainWindow.json");
    bool bPrj = app->projectorWindow->save(dirpath + "/projectorWindow.json");
    bool bSeq = app->sequencer.save(dirpath + "/sequencer.json");

    return bApp && bCamera && bHuman && bProjector && bWin && bPrj;
}

inline bool loadProject(string dirpath){
    auto & app = ofApp::get();

    bool bRel = ofDirectory::doesDirectoryExist(dirpath, true);
    bool bAbs = ofDirectory::doesDirectoryExist(dirpath, false);
    if(!bRel && !bAbs){
        ofLogError("loadProject()") << "Directory does not exist: " << dirpath;
        return "No directory error";
    }

    ofFile file(dirpath); // actually this is directory
    string filename = file.getBaseName();
    ofSetWindowTitle(filename);
    
    bool bApp = Serializer::load(app->grp, dirpath + "/app.json");
    bool bCamera = ofxLoadCamera(app->cam, dirpath + "/camera.settings");
    bool bHuman = Serializer::load(app->human.grp, dirpath + "/human.json");
    bool bProjector = Serializer::load(app->projector.grp, dirpath + "/projector.json");

    bool bWin = app->mainWindow->load(dirpath + "/mainWindow.json");
    bool bPrj = app->projectorWindow->load(dirpath + "/projectorWindow.json");
    bool bSeq = app->sequencer.load(dirpath + "/sequencer.json");

    // This limits fps under 30 somehow
    // ofSetFrameRate(targetFps);
    app->human.setup();

    const auto & items = app->sequencer.getSequenceItems();
    for( auto & i : items){
        i.shape->loadVideo( i.shape->videoPath.get() );
        i.shape->setup();
    }
    return bApp && bCamera && bHuman && bProjector && bWin && bPrj && bSeq;
}

inline void dialogueSaveProject(){
    auto dir = pfd::select_folder("Select directory to save project", ofToDataPath("./projects", true));
    string dirname = dir.result();
    if(ofDirectory::doesDirectoryExist(dirname, false)) {
        bool ok = saveProject(dirname);
    }
}

inline void dialogueLoadProject(){
    auto dir = pfd::select_folder("Select directory to open project", ofToDataPath("./projects", true));
    string dirname = dir.result();
    if(ofDirectory::doesDirectoryExist(dirname, false)) {
        bool ok = loadProject(dirname);
    }
}

inline string dialogueLoadVideo(){
    
    auto res = pfd::open_file("Select video file", ofToDataPath(".", true)).result();

    if(res.size() > 0){
        string & filepath = res[0];
        if(ofFile::doesFileExist(filepath, false)) {
            return filepath;
        }
    }
    return "";
}

};