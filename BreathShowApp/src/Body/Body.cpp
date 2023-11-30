//
//  Body.cpp
//

#include "Body.h"

using glm::vec3;
using glm::normalize;

///
/// Body class
///
Body::Body(){
    landmarks.clear();
    for(int i=0; i<15; i++){
        landmarks.emplace_back(i, vec3(0));
        landmarks.back().setupFilters(frequency, mincutoff, beta, dcutoff);
    }

    listeners.push( frequency.newListener( [&](double & d){ setFreqAll(); }));
    listeners.push( mincutoff.newListener( [&](double & d){ setMinCutoffAll(); }));
    listeners.push( beta.newListener( [&](double & d){ setBetaAll(); }));
    listeners.push( dcutoff.newListener( [&](double & d){ setDerivateCutoffAll(); }));
}

Body::~Body(){
    listeners.unsubscribeAll();
}

void Body::setFreqAll(){
    for(auto & lm : landmarks){
        for(auto & f : lm.filters) f.setFrequency(frequency);
    }
}

void Body::setMinCutoffAll(){
    for(auto & lm : landmarks){
        for(auto & f : lm.filters) f.setMinCutoff(mincutoff);
    }
}

void Body::setBetaAll(){
    for(auto & lm : landmarks){
        for(auto & f : lm.filters) f.setBeta(beta);
    }
}

void Body::setDerivateCutoffAll(){
    for(auto & lm : landmarks){
        for(auto & f : lm.filters) f.setDerivateCutoff(dcutoff);
    }
}

void Body::processOsc(const ofxOscMessage & m, const vector<string> & tokens){

    string address = m.getAddress();
    // /tracker
    if( tokens[1] == "tracker" ){
        
        float x = m.getArgAsFloat(0);
        float y = m.getArgAsFloat(1);
        float z = m.getArgAsFloat(2);
        if(tokens.size() >= 3){
            string jointName = tokens[2];
            
            if(jointName == "hipL"){
                updateValue( landmarks[Landmark::LEFT_HIP], x, 0);
                updateValue( landmarks[Landmark::LEFT_HIP], y, 1);
                updateValue( landmarks[Landmark::LEFT_HIP], z, 2);
            } else if(jointName == "hipR"){
                updateValue( landmarks[Landmark::RIGHT_HIP], x, 0);
                updateValue( landmarks[Landmark::RIGHT_HIP], y, 1);
                updateValue( landmarks[Landmark::RIGHT_HIP], z, 2);
            } else if(jointName == "torso"){
                updateValue( landmarks[Landmark::TORSO], x, 0);
                updateValue( landmarks[Landmark::TORSO], y, 1);
                updateValue( landmarks[Landmark::TORSO], z, 2);
            } else if(jointName == "neck"){
                updateValue( landmarks[Landmark::NECK], x, 0);
                updateValue( landmarks[Landmark::NECK], y, 1);
                updateValue( landmarks[Landmark::NECK], z, 2);
            } else if(jointName == "head"){
                updateValue( landmarks[Landmark::HEAD], x, 0);
                updateValue( landmarks[Landmark::HEAD], y, 1);
                updateValue( landmarks[Landmark::HEAD], z, 2);
            } else if(jointName == "shoulderL"){
                updateValue( landmarks[Landmark::LEFT_SHOULDER], x, 0);
                updateValue( landmarks[Landmark::LEFT_SHOULDER], y, 1);
                updateValue( landmarks[Landmark::LEFT_SHOULDER], z, 2);
            } else if(jointName == "shoulderR"){
                updateValue( landmarks[Landmark::RIGHT_SHOULDER], x, 0);
                updateValue( landmarks[Landmark::RIGHT_SHOULDER], y, 1);
                updateValue( landmarks[Landmark::RIGHT_SHOULDER], z, 2);
            } else if(jointName == "elbowL"){
                updateValue( landmarks[Landmark::LEFT_ELBOW], x, 0);
                updateValue( landmarks[Landmark::LEFT_ELBOW], y, 1);
                updateValue( landmarks[Landmark::LEFT_ELBOW], z, 2);
            } else if(jointName == "elbowR"){
                updateValue( landmarks[Landmark::RIGHT_ELBOW], x, 0);
                updateValue( landmarks[Landmark::RIGHT_ELBOW], y, 1);
                updateValue( landmarks[Landmark::RIGHT_ELBOW], z, 2);
            } else if(jointName == "handL"){
                updateValue( landmarks[Landmark::LEFT_HAND], x, 0);
                updateValue( landmarks[Landmark::LEFT_HAND], y, 1);
                updateValue( landmarks[Landmark::LEFT_HAND], z, 2);
            } else if(jointName == "handR"){
                updateValue( landmarks[Landmark::RIGHT_HAND], x, 0);
                updateValue( landmarks[Landmark::RIGHT_HAND], y, 1);
                updateValue( landmarks[Landmark::RIGHT_HAND], z, 2);
            } else if(jointName == "kneeL"){
                updateValue( landmarks[Landmark::LEFT_KNEE], x, 0);
                updateValue( landmarks[Landmark::LEFT_KNEE], y, 1);
                updateValue( landmarks[Landmark::LEFT_KNEE], z, 2);
            } else if(jointName == "kneeR"){
                updateValue( landmarks[Landmark::RIGHT_KNEE], x, 0);
                updateValue( landmarks[Landmark::RIGHT_KNEE], y, 1);
                updateValue( landmarks[Landmark::RIGHT_KNEE], z, 2);
            } else if(jointName == "footL"){
                updateValue( landmarks[Landmark::LEFT_FOOT], x, 0);
                updateValue( landmarks[Landmark::LEFT_FOOT], y, 1);
                updateValue( landmarks[Landmark::LEFT_FOOT], z, 2);
            } else if(jointName == "footR"){
                updateValue( landmarks[Landmark::RIGHT_FOOT], x, 0);
                updateValue( landmarks[Landmark::RIGHT_FOOT], y, 1);
                updateValue( landmarks[Landmark::RIGHT_FOOT], z, 2);
            } else{
                ofLogError("Body") << "Strange joint name: " <<jointName;
            }
        }else{
            // legacy
            for(int i=0; i<15; i++){
                float x = m.getArgAsFloat(i*3+0);
                float y = m.getArgAsFloat(i*3+1);
                float z = m.getArgAsFloat(i*3+2);
                updateValue(landmarks[i], x, 0);
                updateValue(landmarks[i], y, 1);
                updateValue(landmarks[i], z, 2);
                //cout << i << " : " << x << ", " << y << ", " << z << endl;
            }
        }
    }else if(tokens[1] == "enter"){
        //bEnter = true;
    }else if(tokens[1] == "exit"){
        //offset = vec3(0);
    }
}

void Body::updateValue(Landmark & landmark, float _v, int xyz, bool bTransform){
    if(bTransform) _v = _v * scale.get()[xyz] + offset.get()[xyz];
    if(filterType == 0)
    {
        landmark.pos[xyz] = _v;
    }
    else if(filterType == 1)
    {
        // legacy lowpass filter
        float & v = landmark.pos[xyz];
        v = v * lowpass + _v * (1.0f - lowpass);
    }
    else if(filterType == 2)
    {
        // one euro
        landmark.pos[xyz] = landmark.filters[xyz].filter(_v, ofGetElapsedTimef());
    }else{
        ofLogError("Body") << "Unknown filterType: " << filterType;
    }
}

void Body::calc(){
    vec3 & hipL = landmarks[Landmark::LEFT_HIP].pos;
    vec3 & hipR = landmarks[Landmark::RIGHT_HIP].pos;
    hipCenter = hipR + (hipL - hipR) * 0.5;

    rootPos = hipCenter * vec3(1, 0, 1);

    //rootOri
    vec3 & shoulderL = landmarks[Landmark::LEFT_SHOULDER].pos;
    vec3 & shoulderR = landmarks[Landmark::RIGHT_SHOULDER].pos;
    shoulderCenter = shoulderR + (shoulderL - shoulderR) * 0.5;

    vec3 u = normalize(hipL - hipCenter);
    vec3 v = normalize(shoulderCenter - hipCenter);   // not sure if this is correct or not
    vec3 w = normalize(cross(u, v));
    
    if( std::isnan(u.x) || std::isnan(v.x) || std::isnan(w.x)){
        // probablly depth camera didnt detect skeleton, do nothing
    }else{
        glm::mat3 m = glm::mat3(u, v, w);
        glm::quat q = glm::toQuat(m);
        rootOri = glm::degrees(glm::eulerAngles(q));
        rootOri.x = 0;
        rootOri.z = 0;
    }
}

// draw the body
void Body::draw(float sSize, const ofColor & c){

    float len = sSize * 10;

    ofPushMatrix();
    ofTranslate( rootPos );

    // Root, yellow shere and arrow
    ofSetColor(255,255,0);
    ofDrawSphere(vec3(0), sSize);
    ofRotateYDeg(rootOri.y);
    ofDrawArrow(vec3(0), vec3(0,0,30), sSize*2.0f);
    ofPopMatrix();
    
    ofPushMatrix();
    auto it = landmarks.begin();
    for(; it!=landmarks.end(); it++){
        const int t = it->type;
        const vec3 & p = it->pos;
        ofSetColor(c);
        ofDrawSphere(p, sSize);
    }

    // Bones
    // better to use vbo mesh but doesn't matter for now    
    ofSetColor(255);
    ofDrawLine( landmarks[Landmark::LEFT_HIP].pos, landmarks[Landmark::RIGHT_HIP].pos );
    
    ofDrawLine( hipCenter, landmarks[Landmark::TORSO].pos );
    ofDrawLine( landmarks[Landmark::TORSO].pos, landmarks[Landmark::NECK].pos );
    ofDrawLine( landmarks[Landmark::NECK].pos, landmarks[Landmark::HEAD].pos );
    ofDrawLine( landmarks[Landmark::NECK].pos, landmarks[Landmark::LEFT_SHOULDER].pos );
    ofDrawLine( landmarks[Landmark::LEFT_SHOULDER].pos, landmarks[Landmark::LEFT_ELBOW].pos );
    ofDrawLine( landmarks[Landmark::LEFT_ELBOW].pos, landmarks[Landmark::LEFT_HAND].pos );
    
    ofDrawLine( landmarks[Landmark::NECK].pos, landmarks[Landmark::RIGHT_SHOULDER].pos );
    ofDrawLine( landmarks[Landmark::RIGHT_SHOULDER].pos, landmarks[Landmark::RIGHT_ELBOW].pos );
    ofDrawLine( landmarks[Landmark::RIGHT_ELBOW].pos, landmarks[Landmark::RIGHT_HAND].pos );

    ofDrawLine( landmarks[Landmark::LEFT_HIP].pos, landmarks[Landmark::LEFT_KNEE].pos );
    ofDrawLine( landmarks[Landmark::LEFT_KNEE].pos, landmarks[Landmark::LEFT_FOOT].pos );

    ofDrawLine( landmarks[Landmark::RIGHT_HIP].pos, landmarks[Landmark::RIGHT_KNEE].pos );
    ofDrawLine( landmarks[Landmark::RIGHT_KNEE].pos, landmarks[Landmark::RIGHT_FOOT].pos );

    ofPopMatrix();
}
