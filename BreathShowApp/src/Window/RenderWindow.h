//
// Created by hiroshi on 22/11/11.
//

#ifndef CAR2001_ALL_IN_ONE_RENDERWINDOW_H
#define CAR2001_ALL_IN_ONE_RENDERWINDOW_H

#include "ofMain.h"
#include <GLFW/glfw3.h>

using glm::vec2;

class ofApp;

class RenderWindow {

public:
    RenderWindow();
    virtual ~RenderWindow();
    static void initialise();
    static std::vector<string> monitorNames;
    virtual void draw(ofEventArgs& args) = 0;
    virtual void keyPressed(ofKeyEventArgs & args){};

    void create(string title, shared_ptr<ofAppBaseWindow> mainWindow, shared_ptr<ofApp> mainApp);
    int findMonitorId(string monitorName);
    GLFWmonitor* findMonitor(string monitorName);
    void setMonitor(string monitorName);
    void setMonitor(GLFWmonitor* monitor);

    void addListeners();
    void removeListeners();

    // open / close
    void setOpen(bool bShow);
    bool isOpen();
    void toggleOpen();

    // fullscreen
    void setFull(bool bFull);
    bool isFull();
    void toggleFull();

    // io
    bool load(string filepath);
    bool save(string filepath);

    static bool bInitialised;

    shared_ptr<ofApp> app = nullptr;
    shared_ptr<ofAppBaseWindow> ofBaseWindow = nullptr;
    shared_ptr<ofAppGLFWWindow> ofWindow = nullptr;

    ofParameter<bool> bOpen{"Open", true};
    ofParameter<bool> bFull{"Full Screen", false};
    ofParameter<vec2> size{"size", vec2(1920/2, 1080/2), vec2(100), vec2(4000)};
    ofParameter<vec2> position{"position", vec2(0), vec2(0), vec2(8000)};
    ofParameter<float> scale{"scale", 0.5, 0.1, 1};
    ofParameter<int> monitorId{"monitor id", 0, 0, 3};
    int monitorSelected = 0;
    ofParameter<string> monitorName{"monitor_name","-"};

    ofParameterGroup grp{"Render Window", bOpen, bFull, size, position, scale, monitorId, monitorName};

    ofEventListeners listeners;
};

class MainWindow : public RenderWindow{

public:
    MainWindow(){
        grp.setName("Main Window");
    }
    virtual ~MainWindow(){}
    virtual void draw(ofEventArgs & args) override {};
    virtual void keyPressed(ofKeyEventArgs & args) override {};
};

class ProjectorWindow : public RenderWindow{

public:
    ProjectorWindow(){
        grp.setName("Projector Window");
    }
    virtual ~ProjectorWindow(){}
    virtual void draw(ofEventArgs & args) override;
    virtual void keyPressed(ofKeyEventArgs & args) override;
};



#endif //CAR2001_ALL_IN_ONE_RENDERWINDOW_H
