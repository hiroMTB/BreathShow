#pragma once

#include "ofApp.h"
#include "Vezer.h"
#include "portable-file-dialogs.h"
#include "ofxCameraSaveLoad.h"
#include "Serializer.h"

namespace io{

inline bool saveProject(string dirpath){
    auto & app = ofApp::get();
    app->currentProjectPath = dirpath;
    
    ofFile file(dirpath); // actually this is directory
    string filename = file.getBaseName();
    ofSetWindowTitle(filename);

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
    bool bBody = Serializer::save(app->body.grp, dirpath + "/body.json");

    Serializer::save(app->initGrp, ofToDataPath("./init.json"));

    return bApp && bCamera && bHuman && bProjector && bWin && bPrj && bBody;
}

inline bool loadProject(string dirpath){
    auto & app = ofApp::get();
    app->currentProjectPath = dirpath;
    
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
    bool bBody = Serializer::load(app->body.grp, dirpath + "/body.json");
    
    // This limits fps under 30 somehow
    // ofSetFrameRate(targetFps);
    app->human.setup();

    const auto & items = app->sequencer.getSequenceItems();
    for( auto & i : items){
        if (std::holds_alternative<shared_ptr<Shape>>(i.user)) {
          auto & shape = std::get<shared_ptr<Shape>>(i.user);
            shape->loadVideo( shape->videoPath.get() );
            shape->setup();
        }
    }
    
    return bApp && bCamera && bHuman && bProjector && bWin && bPrj && bSeq && bBody;
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

inline void dialogueOpenVezer(const shared_ptr<Vezer> & vezer){
    string defaultDirToOpen = ofToDataPath( ".", true);
    if(!ofDirectory::doesDirectoryExist(defaultDirToOpen)){
        defaultDirToOpen = "";
    }
    vector<string> files = pfd::open_file("Select Vezer XML file", defaultDirToOpen, {"XML Files", "*.xml"}).result();
    if( !files.empty() ) {
        const string & filepath = files.at(0);
        if (ofFile::doesFileExist(filepath, false)) {
            vezer->load(filepath);
        } else {
            ofLogError("dialogueOpenFBX") << "FBX file does not exist: " << filepath;
        }
    }else{
        ofLogError("dialogueOpenFBX") << "Something went wrong or canceled?";
    }
}

};
