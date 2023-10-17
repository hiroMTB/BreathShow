//
// Created by hiroshi on 22/11/11.
//
#include "ofMain.h"
#include "ofApp.h"
#include "RenderWindow.h"

RenderWindow::RenderWindow(){
}

RenderWindow::~RenderWindow(){
    removeListeners();
}

bool RenderWindow::bInitialised = false;
std::vector<string> RenderWindow::monitorNames;
void RenderWindow::initialise(){
    int numMonitors = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&numMonitors);
    int mainMonitor = 0;
    int xpos, ypos;
    int w,h;

    ofLog() << " =========== MONITOR LIST ==========";
    monitorNames.clear();
    for(int i=0;i<numMonitors;i++){
        string name = glfwGetMonitorName(monitors[i]);
//        glfwGetMonitorPos(monitors[i],&xpos,&ypos);
//        glfwGetMonitorPhysicalSize(monitors[i],&w1,&h1);
        
        const GLFWvidmode *desktopMode = glfwGetVideoMode(monitors[i]);
        w = desktopMode->width;
        h = desktopMode->height;
        //glfwGetMonitorWorkarea(monitors[i],&xpos,&ypos,&w,&h);
        
        glfwGetMonitorPos(monitors[i],&xpos,&ypos);
        bool isMainMonitor = false;
        
        isMainMonitor = monitors[i] == glfwGetPrimaryMonitor();
        if(isMainMonitor){
            mainMonitor = i;
        }

        name = ofToString(i,0) + ":" + name;
        monitorNames.emplace_back(name);
        ofLog() << (isMainMonitor?">> ":"") << "Monitor #" << i << " ["<<name<<"] => [pos: " << xpos << "," << ypos << "]  [size: " << w << "," << h<<"]  " << (isMainMonitor?" <<MAIN MONITOR>>":"");
    }

    ofLog() << " =================================";

    if(numMonitors>0){
//        glfwGetMonitorWorkarea(monitors[mainMonitor],&xpos,&ypos,&w1,&h1);
//        if(!(ofGetWindowPositionX() > xpos && ofGetWindowPositionX() < xpos+w1)){
//            ofLog() << "MoveWindow to Laptop Monitor";
//        }
        bInitialised = true;
    }else{
        ofLogError("RenderWindow") << "Monitor List empty.";
    }
}

int RenderWindow::findMonitorId(string monitorName){
    int numMonitors = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&numMonitors);
    bool found = false;
    for(int i=0;i<numMonitors;i++){
        string name = glfwGetMonitorName(monitors[i]);
        if(name.find(monitorName) != string::npos){
            return i;
        }
    }
    return -1;
}

GLFWmonitor* RenderWindow::findMonitor(string monitorName){

    // a monitor name includes id for example: 1:MY_MONITOR in case of same monitor name
    auto pos = monitorName.find(':');
    int id = -1;
    if( pos != std::string::npos){
        id = ofToInt(monitorName.substr(0,1));
        monitorName.erase(0, pos+1);
    }
    

    int numMonitors = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&numMonitors);
    bool found = false;
    for(int i=0;i<numMonitors;i++){
        string name = glfwGetMonitorName(monitors[i]);
        if(id==-1){
            if(name.find(monitorName) != string::npos){
                return monitors[i];
            }
        }else{
            if(id==i){
                if(name.find(monitorName) != string::npos){
                    return monitors[i];
                }
            }
        }
    }
    
    return nullptr;
}

void RenderWindow::setMonitor(string monitorName) {
    GLFWmonitor* monitor = findMonitor(monitorName);
    if(monitor) setMonitor(monitor);
}

void RenderWindow::setMonitor(GLFWmonitor* monitor) {
    int xpos, ypos;
    int w,h;
    // glfwGetMonitorWorkarea(monitor,&xpos,&ypos,&w,&h);
    const GLFWvidmode *desktopMode = glfwGetVideoMode(monitor);
    w = desktopMode->width;
    h = desktopMode->height;

    glfwGetMonitorPos(monitor,&xpos,&ypos);
    ofWindow->setWindowPosition(xpos,ypos);
    position.set(glm::vec2(xpos,ypos));
    //ofWindow->setWindowShape(w,h);
}

void RenderWindow::addListeners(){
    if(ofBaseWindow) {
        ofAddListener(ofBaseWindow->events().draw, this, &RenderWindow::draw);
        ofAddListener(ofBaseWindow->events().keyPressed, this, &RenderWindow::keyPressed);
        listeners.push( size.newListener( [&](vec2 &s){
            s.x = ofClamp(s.x, 100, size.getMax().x);
            s.y = ofClamp(s.y, 100, size.getMax().y);
            ofWindow->setWindowShape(s.x, s.y); }));
        listeners.push( position.newListener( [&](vec2 &s){ ofWindow->setWindowPosition(s.x, s.y); }));
        //listeners.push( scale.newListener( [&](vec2 &s){ cout << "scale changed" << endl; }));
    }
}

void RenderWindow::removeListeners(){
    if(ofBaseWindow) {
        ofRemoveListener(ofBaseWindow->events().draw, this, &RenderWindow::draw);
        ofRemoveListener(ofBaseWindow->events().keyPressed, this, &RenderWindow::keyPressed);
        listeners.unsubscribeAll();
    }
}

void RenderWindow::create(string title, shared_ptr<ofAppBaseWindow> mainWindow, shared_ptr<ofApp> mainApp) {
    if (ofBaseWindow== nullptr) {
        ofLogNotice("RenderWindow") << "create() " << title;
        ofGLFWWindowSettings settings;
        settings.setGLVersion(4, 1);
        settings.setSize(size.get().x, size.get().y);
        settings.setPosition(position);
        settings.resizable = true;
        settings.numSamples = 0;
        settings.decorated = true;
        if(mainWindow) settings.shareContextWith = mainWindow;
        settings.title = title;
        settings.visible = bOpen;
        settings.windowMode = bFull ? OF_FULLSCREEN : OF_WINDOW;

        ofBaseWindow = ofCreateWindow(settings);
        // ofBaseWindow->setVerticalSync(false);
        ofBaseWindow->setVerticalSync(true);

        if(bOpen) addListeners();
        ofWindow = std::dynamic_pointer_cast<ofAppGLFWWindow>(ofBaseWindow);

        app = mainApp;

        if(!RenderWindow::bInitialised) RenderWindow::initialise();
        auto monitorId = findMonitorId(monitorName);
        auto monitor = findMonitor(monitorName);
        if(monitor){
            ofLogNotice() << "FOUND MONITOR AND MOVING IT >> " << monitorName;
            setMonitor(monitor);
        }
    }
}

void RenderWindow::setOpen(bool _bOpen){
    ofLogNotice("RenderWindow") << "setOpen() : " << (_bOpen ? "Show" : "Hide");
    GLFWwindow * glfwWindow = ofWindow->getGLFWWindow();
    if(glfwWindow ) {
        if (_bOpen) {
            glfwShowWindow(glfwWindow);
//            glfwSetWindowShouldClose(glfwWindow, 0);
            //ofGetMainLoop()->addWindow(ofWindow);
//            ofWindow->addCallbacks();
            setFull(bFull);
            addListeners();
        } else {
            glfwHideWindow(glfwWindow);
//            glfwSetWindowShouldClose(glfwWindow, 1);
//            ofWindow->removeCallbacks();
//            ofWindow->setWindowShouldClose();
            //ofGetMainLoop()->removeWindow(ofWindow);
            removeListeners();
        }
        bOpen = _bOpen;
    }
}

bool RenderWindow::isOpen(){
    GLFWwindow * glfwWindow = ofWindow->getGLFWWindow();
    if(glfwWindow) return glfwGetWindowAttrib(glfwWindow, GLFW_VISIBLE);
//  if(glfwWindow) return !glfwWindowShouldClose(glfwWindow);
    else return false;
}

void RenderWindow::toggleOpen() {
    bool b = isOpen();
    setOpen(!b);
}

void RenderWindow::setFull(bool _bFull){
    ofWindow->setFullscreen(_bFull);
    bFull = _bFull;
}

bool RenderWindow::isFull(){
    return ofWindow->getWindowMode() == OF_FULLSCREEN; // || ofWindow->getWindowMode() == OF_GAME_MODE;
}

void RenderWindow::toggleFull() {
    bool b = isFull();
    setFull(!b);
}

bool RenderWindow::save(string filepath){
    ofJson json;
    string tmp = grp.getName();
    grp.setName("Window");
    ofSerialize(json, grp);
    grp.setName(tmp);

    return ofSavePrettyJson(filepath, json);
}

bool RenderWindow::load(string filepath){
    if(ofFile::doesFileExist(filepath)) {
        ofJson json = ofLoadJson(filepath);
        if (!json.is_null()) {
            ofJson winPrm = json["Window"];
            if (!winPrm.is_null()) {
                bOpen.fromString(winPrm.value("Open", "0"));
                bFull.fromString(winPrm.value("Full_Screen", "0"));
                size.fromString(winPrm.value("size", "100,100"));
                position.fromString(winPrm.value("position", "0,0"));
                scale.fromString(winPrm.value("scale", "1"));
                monitorName.fromString(winPrm.value("monitor_name", "1"));
                monitorId.fromString(winPrm.value("monitor_id", "0"));
                monitorSelected = monitorId.get();
                ofLogNotice("RenderWindow") << "monitorName :  " << monitorName;
                
                setFull(bFull);
                return true;
            }
        }
    }
    return false;
}
