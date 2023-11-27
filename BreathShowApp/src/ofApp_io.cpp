//
//  ofApp_io.cpp
//  BeathShowApp
//
//

#include "ofApp.h"
#include "portable-file-dialogs.h"
#include "ofxCameraSaveLoad.h"
#include "Serializer.h"

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
    
    bool bApp = Serializer::save(grp, dirpath + "/app.json");
    bool bCamera = ofxSaveCamera(cam, dirpath + "/camera.settings");
    bool bHuman = Serializer::save(human.grp, dirpath + "/human.json");
    bool bProjector = Serializer::save(projector.grp, dirpath + "/projector.json");
    bool bWin = mainWindow->save(dirpath + "/mainWindow.json");
    bool bPrj = projectorWindow->save(dirpath + "/projectorWindow.json");
    bool bSeq = sequencer.save(dirpath + "/sequencer.json");

    return bApp && bCamera && bHuman && bProjector && bWin && bPrj;
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
    
    bool bApp = Serializer::load(grp, dirpath + "/app.json");
    bool bCamera = ofxLoadCamera(cam, dirpath + "/camera.settings");
    bool bHuman = Serializer::load(human.grp, dirpath + "/human.json");
    bool bProjector = Serializer::load(projector.grp, dirpath + "/projector.json");

    bool bWin = mainWindow->load(dirpath + "/mainWindow.json");
    bool bPrj = projectorWindow->load(dirpath + "/projectorWindow.json");
    bool bSeq = sequencer.load(dirpath + "/sequencer.json");

    // This limits fps under 30 somehow
    // ofSetFrameRate(targetFps);
    human.setup();
//    fanL.loadVideo( "vid/fanL_h264.mp4" );
//    fanR.loadVideo( "vid/fanR_h264.mp4" );
//    rectScreen.loadVideo( "vid/rectScreen_h264.mp4" );
//    ellipse.loadVideo( "vid/ellipse_h264.mp4" );

    const auto & items = sequencer.getSequenceItems();
    for( auto & i : items){
        i.shape->loadVideo( i.shape->videoPath.get() );
        i.shape->setup();
    }
    return bApp && bCamera && bHuman && bProjector && bWin && bPrj && bSeq;
}

std::string ofApp::dialogueLoadVideo(){
    
    auto res = pfd::open_file("Select video file", ofToDataPath(".", true)).result();

    if(res.size() > 0){
        string & filepath = res[0];
        if(ofFile::doesFileExist(filepath, false)) {
            return filepath;
        }
    }
    return "";
}

