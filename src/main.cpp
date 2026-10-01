#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
    // window settings - 1000 x 700 fits the layout in ofApp.cpp
    ofGLWindowSettings settings;
    settings.setSize(1000, 700);
    settings.windowMode = OF_WINDOW;

    auto window = ofCreateWindow(settings);

    ofRunApp(window, std::make_shared<ofApp>());
    ofRunMainLoop();
}
