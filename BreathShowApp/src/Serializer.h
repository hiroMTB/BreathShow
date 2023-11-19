#pragma once

#include "ofMain.h"

class Serializer
{
public:
    static bool load( ofParameterGroup & grp, const std::string& filepath){
        ofFile file(filepath);
        if(!file.exists()){
            ofLogError("Serializer") << "Load ERROR  : " << "file does not exist: " << filepath;
            return false;
        }else if (file.isDirectory()){
            ofLogError("Serializer") << "Load ERROR  : " << "this is directory: " << filepath;
            return false;
        }
        
        ofJson json = ofLoadJson(filepath);
        if(json.empty()){
            ofLogWarning("Serializer") << "Load WARN  : " << "empty json: " << filepath;
            return false;
        }

        if(grp.size() == 0){
            ofLogError("Serializer")  << "Load ERROR  : " << "empty parameterGroup: " << filepath;
            return false;
        }
        
        ofDeserialize(json, grp);
        string filename = file.getFileName();
        ofLogNotice("Serializer") << "Load SUCCESS: " << filename << "   ["<<file.getAbsolutePath()<<"]";
        return true;
    }
    
    static bool save( ofParameterGroup & grp, const std::string& filepath){
        if(grp.size() == 0){
            ofLogError("Serializer")  << "Save ERROR  : " << "empty parameterGroup: " << filepath;
            return false;
        }

        ofJson json;
        ofSerialize(json, grp);
        bool ok = ofSavePrettyJson(filepath, json);
        ofFile file(filepath);
        string filename = file.getFileName();
        if(ok){
            ofLogNotice("Serializer") << "Save SUCCESS: " << filename;
        }else{
            ofLogError("Serializer")  << "Save ERROR  : " << "json save error" << filename;
        }
        return ok;

    }
};

