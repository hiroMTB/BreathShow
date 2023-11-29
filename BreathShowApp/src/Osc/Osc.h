#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

class Osc {

public:

    Osc(){
        //listeners.push( receivePort.newListener( [&](int & i){ setupReceiver(); }));
        //listeners.push( maxMsgQueueNum.newListener( [&](int & i){ resizeQueue(); }));
    }

    ~Osc(){
        listeners.unsubscribeAll();
    }


    void setupReceiver(){
        ofLogNotice() << "listening for osc messages on port " << receivePort;
        receiver.setup(receivePort);
    }

    template<typename Visitor>
    void receive(Visitor visitor){
        while(receiver.hasWaitingMessages()){
            ofxOscMessage m;
            receiver.getNextMessage(m);
            visitor(m);

            if(maxMsgQueueNum !=0 ) {
                resizeQueue();
                std::stringstream ss;
                ss << m;
                msgQueue.emplace_back(ss.str());
            }
        }
    }
    
    void resizeQueue(){
        while(msgQueue.size() >= maxMsgQueueNum){
            msgQueue.pop_front();
        }
    }

    std::deque<string> msgQueue;

    ofxOscReceiver receiver;
    ofParameter<int> receivePort{"port", 7500, 0, 10000};
    ofParameter<int> maxMsgQueueNum{"Max Message Queue Num", 5, 0, 100};
    ofParameterGroup grp{"OSC Receive", receivePort, maxMsgQueueNum};

    ofEventListeners listeners;


};
