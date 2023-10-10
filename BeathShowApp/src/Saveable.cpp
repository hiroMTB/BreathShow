#include "Saveable.h"
#include "Util.h"

bool Saveable::load(const string& filepath)
{
 
    ofFile file(filepath);
    if(!file.exists()){
        ofLogError("Saveable") << "Load ERROR  : " << "file does not exist: " << filepath;
        return false;
    }else if (file.isDirectory()){
        ofLogError("Saveable") << "Load ERROR  : " << "this is directory: " << filepath;
        return false;
    }
    
    ofJson json = ofLoadJson(filepath);
    if(json.empty()){
        ofLogWarning("Saveable") << "Load WARN  : " << "empty json: " << filepath;
        return false;
    }

    if(getParametersRef().size() == 0){
        ofLogError("Saveable")  << "Load ERROR  : " << "empty parameterGroup: " << filepath;
        return false;
    }
    
    ofDeserialize(json, getParametersRef());
    string filename = file.getFileName();
    ofLogNotice("Saveable") << "Load SUCCESS: " << filename;
    return true;
}

bool Saveable::save(const string& filepath)
{
    if(getParametersRef().size() != 0){
        ofLogError("Saveable")  << "Save ERROR  : " << "empty parameterGroup: " << filepath;
        return false;
    }

    ofJson json;
    ofSerialize(json, getParametersRef());
    bool ok = ofSavePrettyJson(filepath, json);
    ofFile file(filepath);
    string filename = file.getFileName();
    if(ok){
        ofLogNotice("Saveable") << "Save SUCCESS: " << filename;
    }else{
        ofLogError("Saveable")  << "Save ERROR  : " << "json save error" << filename;
    }
    return ok;
}
