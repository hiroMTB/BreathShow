#pragma once

#include "ofMain.h"
#include "ofxVezer.h"
#include "ofxOscMessage.h"
#include "ParserCereal.h"
#include "Body.h"

class Vezer {
    
public:
    
    Vezer(){}
    
    bool load() {
        return load(filepath);
    }
    
    bool load(const string& pathXml){
        ofFile file(pathXml);
        if(file.exists()) {
            string pathBin = file.getEnclosingDirectory() + "/" +  file.getBaseName() + ".bin";
            return load(pathXml, pathBin);
        }else{
            ofLogError("Vezer") << "Vezer XML file does not exist";
            return false;
        }
    }
    
    bool load(const string& pathXml, const string & pathBin){
        ofFile file(pathXml);
        if(file.exists()) {
            filepath = pathXml;
            vector<ofxVezer::Composition> comps = ofxVezer::ParserCereal::load(pathXml, pathBin);
            
            comp.tracks.clear();
            comp.tmp_tracks.clear();
            
            if(comps.size() !=0){
                comp = comps[0];
                //comp.combine(comps[1]);
                parser.redraw(comp);
            }else{
                ofLogError("Vezer") << "Empty Composition";
            }
            return true;
        }else{
            ofLogError("Vezer") << "Vezer XML file does not exist";
            return false;
        }
    }
    
    void draw(){
        parser.debugDraw();
        //        //int frame = int(ofGetElapsedTimef() * comp.fps) % comp.length;
        //        int frame = currentFrame;
        //        float x = (frame / float(comp.length)) * ofGetWidth();
        //        ofRect(x, 0, 1, ofGetHeight());
    }
    
    void redraw(){
        parser.redraw(comp);
    }
    
    template<typename Visitor>
    void receive(Visitor visitor){
        //int frame = int(ofGetElapsedTimef() * comp.fps)  % comp.length;
        if( !comp.state ) {
            ofLogError("Vezer") << "No Composition loaded?";
        }
        
        if( isPlaying || bNeedUpdate ) {
            //            if(!bNeedUpdate)currentFrame++;
            //            currentFrame %= comp.length;
            //            setFrame(currentFrame);
            
            while (provider.hasWaitingMessages()) {
                ofxOscMessage m;
                provider.getNextMessage(&m);
                visitor(m);
                
                if(maxMsgQueueNum !=0 ) {
                    resizeQueue();
                    std::stringstream ss;
                    ss << m;
                    msgQueue.emplace_back(ss.str());
                }
            }
            bNeedUpdate = false;
        }
    }
    
    void start(){
        isPlaying = true;
    }
    
    void stop(){
        isPlaying = false;
    }
    
    void setFrame(int frame){
        currentFrame = frame;
        provider.setCurrentTracks(comp, frame);
        bNeedUpdate = true;
    }
    
    void resizeQueue(){
        while(msgQueue.size() >= maxMsgQueueNum){
            msgQueue.pop_front();
        }
    }
    
    int getTotalFrames(){
        return comp.length;
    }
    
    int getCurrentFrame(){
        return currentFrame;
    }
    
    bool isReady(){
        return comp.state;
    }
    
    void prepareFbo(float w, float h){
        parser.redraw(comp, w, h);
    }
    
    bool getIsPlaying(){
        return isPlaying;
    }
    
    static bool getIsPlayingSomeVezer(){
        return isPlayingSomeVezer;
    }
    
    static void setIsPlayingSomeVezer(bool b){
        isPlayingSomeVezer = b;
    }
    
    Body body;
    
    ofxVezer::Parser parser;
    ofxVezer::Composition comp;
    ofxVezer::Provider provider;
    
    std::deque<string> msgQueue;
    
    ofParameter<string> filepath{ "File Path", "./vezer/rec01.xml"};
    ofParameter<int> maxMsgQueueNum{"Max Message Queue Num", 5, 0, 100};
    ofParameterGroup grp{"vezer", filepath, maxMsgQueueNum, body.grp};
    ofEventListeners listeners;
    
private:
    bool isPlaying{false};
    bool bNeedUpdate{false};
    int currentFrame{0};  // this must be synched with sequencer time line

    static bool isPlayingSomeVezer;
};
