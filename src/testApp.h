#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include <fstream>

//#define DEPTH_VIDEO_MODULE_ON

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
	void kinect_update();
	void process_depth_frame(ofxCvGrayscaleImage &frame);
	void process_color_frame();
	void follow_face(const int &face_depth,const int &x,const int &y);
	void send_command(int command);

	ofxKinect kinect;
	ofxCvColorImage colorImg;
	ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage frame_320_240;
	ofxCvGrayscaleImage dil_se; //structuring element for dilation
	ofxCvGrayscaleImage thresholded_img;
	ofxCvGrayscaleImage output_img; //output image
	ofxCvGrayscaleImage se1;
	ofxCvGrayscaleImage se2;
	stringstream status_str;
	ofxCvContourFinder contourFinder;
	int object_depth;	
	float object_depth_in_m;
	ofSerial serial; //serial object of interfacing with arduino

	bool bDrawPointCloud;
	bool BSERIAL_MODULE_ON;
	bool bNearObject;
	bool bNearFace;
	bool BFACE_TRACKER_MODULE_ON;
	CvHaarClassifierCascade *faceCascade;

	int nearThreshold;
	int farThreshold;
	int angle;
	int SE_WIDTH;
	int SE_HEIGHT;
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
		RESET_ARM =11,
		SET_TILT_ANGLE=12};
	// used for viewing the point cloud
	ofEasyCam easyCam;

	#ifdef DEPTH_VIDEO_MODULE_ON
	ifstream *video_file;
	IplImage *video_frame;
	ofxCvGrayscaleImage ovideo_frame;
	string file_name;
	int frame_count;
	int frame_index;
	char *depth_buffer;
	unsigned char *udepth_buffer;
	void depth_video_update();
	#endif
};
