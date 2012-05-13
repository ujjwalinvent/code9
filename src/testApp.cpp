/*
observation 
changes
dilation with 5 by 5 window
need to accurately balance far threshold with respect to depth
or we can decrease the depth until we find the glass a kind of auto scan algorithm
architecture change
make a struct for structuring element
and we put the pivot with the structuring element only
*/
#include "testApp.h"
#include <fstream>
#include "depth_lib.hpp"
#include "morph.hpp"
//#include "src/serial_access.h"
//#include "src/data_access.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	// enable depth->video image calibration
	kinect.setRegistration(true);

	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
    //kinect.init(false, false); // disable video image (faster fps)
	kinect.open();

	//list all the serial devices.
	serial.enumerateDevices();
	serial.setup("/dev/ttyACM0",9600);

	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayImage2.allocate(kinect.width/2,kinect.height/2);
	dil_se.allocate(5,5);
	out_img.allocate(kinect.width/2,kinect.height/2);
	out_img_e.allocate(kinect.width/2,kinect.height/2);	
	SE_WIDTH = 40;
	SE_HEIGHT = 40;
	se1.allocate(SE_WIDTH,SE_HEIGHT); //structuring element b1
	se2.allocate(SE_WIDTH,SE_HEIGHT); //structuring element b2
	
	nearThreshold = 255;
	farThreshold = 0;
	bThreshWithOpenCV = true;
	ofSetFrameRate(30);

	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);

	// start from the front
	bDrawPointCloud = false;
	dil_se.set(255);
	make_structuring_element_b1(se1.getCvImage(),1,20,1);
	make_structuring_element_b2(se2.getCvImage(),1,1,5);
}

//--------------------------------------------------------------
void testApp::update() {

	ofBackground(100, 100, 100);

	kinect.update();
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {

		// load grayscale depth image from the kinect source
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		cvResize(grayImage.getCvImage(),grayImage2.getCvImage());
	//	grayImage.dilate_3x3();
		depth_threshold(grayImage2.getCvImage(),nearThreshold,farThreshold);
		dilate(grayImage2.getCvImage(),dil_se.getCvImage(),2,2,out_img.getCvImage());
		hit_or_miss2(out_img.getCvImage(),se1.getCvImage(),se2.getCvImage(),SE_HEIGHT/2,SE_WIDTH/2,out_img_e.getCvImage());
		//erode(out_img.getCvImage(),se1.getCvImage(),SIZE/2,SIZE/2,out_img_e.getCvImage());
		grayImage.flagImageChanged();
		grayImage2.flagImageChanged();	
		out_img.flagImageChanged();
		out_img_e.flagImageChanged();
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(out_img_e, 0,30*30,1, false);
		
	}

}

//--------------------------------------------------------------
void testApp::draw() {

	ofSetColor(255, 255, 255);
	if(bDrawPointCloud) {
		easyCam.begin();
		drawPointCloud();
		easyCam.end();
	} else {
		// draw from the live kinect
		kinect.drawDepth(0,  0,320,240);
		out_img.draw    (320,0,320,240);
		out_img_e.draw  (640,0,320,240);
		contourFinder.draw(640,0,320,240);
		stringstream glasstext;
		glasstext<<"Glass";
		if(contourFinder.blobs.size()>0)
		ofDrawBitmapString(glasstext.str(),640+contourFinder.blobs[0].centroid.x,contourFinder.blobs[0].centroid.y-15);

		kinect.draw     (0,240,320,240);
		se1.draw(320,240);
		se2.draw(320+SE_WIDTH,240);
		
		//out_img_e.draw(640, 0, 640, 480);
//		contourFinder.draw(640, 0, 640, 480);

	}

	// draw instructions
	ofSetColor(255, 255, 255);
	stringstream reportStream;
	reportStream << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
	<< ofToString(kinect.getMksAccel().y, 2) << " / "
	<< ofToString(kinect.getMksAccel().z, 2) << endl
	<< "press p to switch between images and point cloud, rotate the point cloud with the mouse" << endl
	<< "using opencv threshold = " << bThreshWithOpenCV <<" (press spacebar)" << endl
	<< "set near threshold " << nearThreshold << " (press: + -)" << endl
	<< "set far threshold " << farThreshold << " (press: < >)"  //num blobs found " << contourFinder.nBlobs
	<< ", fps: " << ofGetFrameRate() << endl
	<< "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl
	<< "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl;
	ofDrawBitmapString(reportStream.str(),650,500);

}

void testApp::drawPointCloud() {
	int w = 640;
	int h = 480;
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_POINTS);
	int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			if(kinect.getDistanceAt(x, y) > 0) {
				mesh.addColor(kinect.getColorAt(x,y));
				mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
			}
		}
	}
	glPointSize(3);
	ofPushMatrix();
	// the projected points are 'upside down' and 'backwards'
	ofScale(1, -1, -1);
	ofTranslate(0, 0, -1000); // center the points a bit
	glEnable(GL_DEPTH_TEST);
	mesh.drawVertices();
	glDisable(GL_DEPTH_TEST);
	ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
}

/*
void testApp::keyPressed(int key)
{
	switch(key)
	{
		case 'i': serial.writeByte(FORWARD);  break;
		case 'k': serial.writeByte(BACKWARD); break;
		case 'j': serial.writeByte(LEFT);     break;
		case 'l': serial.writeByte(RIGHT);    break;
		case 'o': serial.writeByte(OPEN_GRIPPER); break;
		case 'c': serial.writeByte(CLOSE_GRIPPER); break;
		case 'r': serial.writeByte(RESET_ARM); break;
		case OF_KEY_UP:    serial.writeByte(TILT_UP);   break;
		case OF_KEY_DOWN:  serial.writeByte(TILT_DOWN); break;
		case OF_KEY_LEFT:  serial.writeByte(PAN_LEFT);  break;
		case OF_KEY_RIGHT: serial.writeByte(PAN_RIGHT); break;
		default : serial.writeByte(STOP);     break;
	}
}
*/

void testApp::keyPressed (int key) {
	switch (key) {
		case ' ':
			bThreshWithOpenCV = !bThreshWithOpenCV;
			break;

		case'p':
			bDrawPointCloud = !bDrawPointCloud;
			break;

		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;

		case '<':
		case ',':
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;

		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;

		case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;

		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;

		case 'o':
			kinect.setCameraTiltAngle(angle); // go back to prev tilt
			kinect.open();
			break;

		case 'c':
			kinect.setCameraTiltAngle(0); // zero the tilt
			kinect.close();
			break;

		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;

		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
			break;
	}
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{}

