#include "Renderer.h"

Renderer::Renderer(){
}

void Renderer::setup(const ofFbo & fbo){
    ofxTextureRecorder::Settings settings(fbo.getTexture());
    settings.imageFormat = OF_IMAGE_FORMAT_PNG;
    settings.numThreads = 12;
    settings.maxMemoryUsage = 9000000000;
    recorder.setup(settings);
}

Renderer::~Renderer(){
}

void Renderer::start(const std::string & dirpath){
    bool bExist = ofDirectory::doesDirectoryExist(dirpath, false);
    if(!bExist){
        ofDirectory::createDirectory(dirpath);
    }
    
    recorder.setFolderPath(dirpath+"/");
    bRender = true;    
    ofLogVerbose("Renderder") << "start render: " << dirpath;
}

void Renderer::stop(){
    bRender = false;
}

void Renderer::render(const ofTexture & tex){
    if(bRender){
        recorder.save(tex);
    }
}
