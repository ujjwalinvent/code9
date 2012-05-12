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

	bool bThreshWithOpenCV;
	bool bDrawPointCloud;
	int nearThreshold;
	int farThreshold;
	int angle;
	int SIZE;
	// used for viewing the point cloud
	ofEasyCam easyCam;
};
