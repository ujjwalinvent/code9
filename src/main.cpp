#include "testApp.h"
#include "ofAppGlutWindow.h"
#include "ofxOpenCv.h"


int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1366, 700, OF_WINDOW);
	ofRunApp(new testApp());
}
