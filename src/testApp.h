#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include <fstream>

class testApp : public ofBaseApp {
public:

	void setup();
	void update();
	void draw();
	void exit();

	void drawPointCloud();

	void keyPressed(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	ofxKinect kinect;
	ofxCvColorImage colorImg;
	ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayImage2;
	ofxCvGrayscaleImage dil_se; //structuring element for dilation
	ofxCvGrayscaleImage out_img; //output image
	ofxCvGrayscaleImage out_img_e;
	ofxCvGrayscaleImage se1;
	ofxCvGrayscaleImage se2;

	ofxCvContourFinder contourFinder;
	
	ofSerial serial; //serial object of interfacing with arduino

	bool bThreshWithOpenCV;
	bool bDrawPointCloud;
	int nearThreshold;
	int farThreshold;
	int angle;
	int SIZE;
	enum {STOP = 0, 
		FORWARD = 1, 
		BACKWARD = 2, 
		LEFT = 3, 
		RIGHT = 4, 
		TILT_UP = 5, 
		TILT_DOWN = 6,
		PAN_LEFT = 7,
		PAN_RIGHT = 8,
		OPEN_GRIPPER = 9,
		CLOSE_GRIPPER=10,
		RESET_ARM =11};
	// used for viewing the point cloud
	ofEasyCam easyCam;
};
