//
//  Shape.cpp
//

#include "Shape.h"
#include "ofApp.h"


Shape::Shape(){
    type = NONE;
    setParent(ofApp::get()->human.root);
}
