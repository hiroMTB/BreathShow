#pragma once

#include "ofMain.h"
#include "ofxOneEuroFilter.h"

class Landmark {

public:
    enum Type
    {
        LEFT_HIP = 0,
        RIGHT_HIP = 1,
        TORSO = 2,
        NECK = 3,
        HEAD = 4,
        LEFT_SHOULDER = 5,
        RIGHT_SHOULDER = 6,
        LEFT_ELBOW = 7,
        RIGHT_ELBOW = 8,
        LEFT_HAND = 9,
        RIGHT_HAND = 10,
        LEFT_KNEE = 11,
        RIGHT_KNEE = 12,
        LEFT_FOOT = 13,
        RIGHT_FOOT = 14
    };

    Landmark(){}

    Landmark(const int & i, const glm::vec3 & _pos){
        type = (Type)i;
        pos = _pos;
    }

    Landmark(const Type & t, const glm::vec3 & _pos){
        type = t;
        pos = _pos;
    }

    void setupFilters(double frequency, double mincutoff, double beta, double dcutoff){
        filters.clear();
        filters.assign(3, ofxOneEuroFilter());
        for(auto filter : filters) {
            filter.setup(frequency, mincutoff, beta, dcutoff);
        }
    }

    static std::string getTypeString(const Type & t){
        std::string s;
        switch(t){
            case LEFT_HIP:      s = "LEFT_HIP";         break;
            case RIGHT_HIP:     s = "RIGHT_HIP";        break;
            case TORSO:         s = "TORSO";            break;
            case NECK:          s = "NECK";             break;
            case HEAD:          s = "HEAD";             break;
            case LEFT_SHOULDER: s = "LEFT_SHOULDER";    break;
            case RIGHT_SHOULDER:s = "RIGHT_SHOULDER";   break;
            case LEFT_ELBOW:    s = "LEFT_ELBOW";       break;
            case RIGHT_ELBOW:   s = "RIGHT_ELBOW";      break;
            case LEFT_HAND:     s = "LEFT_HAND";        break;
            case RIGHT_HAND:    s = "RIGHT_HAND";       break;
            case LEFT_KNEE:     s = "LEFT_KNEE";        break;
            case RIGHT_KNEE:    s = "RIGHT_KNEE";       break;
            case LEFT_FOOT:     s = "LEFT_FOOT";        break;
            case RIGHT_FOOT:    s = "RIGHT_FOOT";       break;  
            default:            s = "UNKNOWN";          break;
        }
        return s;
    }

    Type type;
    glm::vec3 pos;

    // filter
    vector<ofxOneEuroFilter> filters;
};
