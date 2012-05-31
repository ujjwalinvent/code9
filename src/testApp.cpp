/*
observation 
changes
dilation with 5 by 5 window
need to accurately balance far threshold with respect to depth
or we can decrease the depth until we find the glass a kind of auto scan algorithm
architecture change
make a struct for structuring element
and we put the pivot with the structuring element only

//TODO program the face tracker to follow the person release the object only when the person is near
//TODO draw the output of the face tracker on the color image if the face tracker module is on
*/
#include "testApp.h"
#include <fstream>
#include "depth_lib.hpp"
#include "morph.hpp"


//#define DEPTH_VIDEO_MODULE_ON

//--------------------------------------------------------------
void testApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	// enable depth->video image calibration
	kinect.setRegistration(true);
	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
    //kinect.init(false, false); // disable video image (faster fps)
#ifndef DEPTH_VIDEO_MODULE_ON
kinect.open();
#endif

	//serial MODULE is ON or not
	BSERIAL_MODULE_ON = true; 
	BFACE_TRACKER_MODULE_ON  = false;
	bNearFace = false;	
	bNearObject = false;
	//list all the serial devices.
	if(BSERIAL_MODULE_ON)
	{
	serial.enumerateDevices();
	serial.setup("/dev/ttyACM0",9600);
	send_command(SET_TILT_ANGLE);
	send_command(OPEN_GRIPPER);
	}
	
	if(BFACE_TRACKER_MODULE_ON)
	{
		faceCascade = get_haar_cascade();
	}

	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	frame_320_240.allocate(kinect.width/2,kinect.height/2);
	thresholded_img.allocate(kinect.width/2,kinect.height/2);
	output_img.allocate(kinect.width/2,kinect.height/2);	
	
	dil_se.allocate(5,5);
	SE_WIDTH = 40;
	SE_HEIGHT = 40;
	se1.allocate(SE_WIDTH,SE_HEIGHT); //structuring element b1
	se2.allocate(SE_WIDTH,SE_HEIGHT); //structuring element b2
	
	nearThreshold = 255;
	farThreshold = 140;
	ofSetFrameRate(30);

	// zero the tilt on startup
	angle = -8;
	kinect.setCameraTiltAngle(angle);

	// start from the front
	bDrawPointCloud = false;
	dil_se.set(255);
	make_structuring_element_b1(se1.getCvImage(),1,20,1);
	make_structuring_element_b2(se2.getCvImage(),1,1,10);
	ofBackground(0,0,50);
#ifdef DEPTH_VIDEO_MODULE_ON
cout<<"DEPTH_VIDEO_MODULE_ON"<<endl;
file_name="data/depth_video_6";
long size = get_file_size(file_name); //get the size of depth video file
assert(size%(GRAY_BUFFERSIZE)==0); //assert that the size is multiple of GRAY_BUFFERSIZE
frame_count=size/(GRAY_BUFFERSIZE); //get the No. of frames.
frame_index = 0;
video_file = new ifstream(file_name.c_str(),ios::in|ios::binary);
depth_buffer = new char[GRAY_BUFFERSIZE];	
udepth_buffer = (unsigned char*)depth_buffer;
ovideo_frame.allocate(kinect.width,kinect.height);
cout<<frame_count<<":Loaded"<<endl;
#endif

}

//--------------------------------------------------------------
void testApp::update() {
#ifdef DEPTH_VIDEO_MODULE_ON
	depth_video_update();
#else 
	kinect_update();
#endif
}


void testApp::kinect_update()
{
	kinect.update();
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
		// load grayscale depth image from the kinect source
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		if(BFACE_TRACKER_MODULE_ON)
			process_color_frame();
		else
			process_depth_frame(grayImage);
	}	
}

void testApp::process_color_frame()
{
//need to adjust the kinect tilt angle so the face is detected
			colorImg.setFromPixels(kinect.getPixels(),kinect.width,kinect.height);
			CvRect faceRect = detectFaceInImage(colorImg.getCvImage(),faceCascade);
			int face_x,face_y,face_depth;

			if(faceRect.width>0)
			{
				face_x = faceRect.x;
				face_y = faceRect.y;
				face_depth = get_gray_depth(grayImage.getCvImage(),face_x+faceRect.width/2,face_y+faceRect.height/2);
				follow_face(face_depth,face_x,face_y);
			}

			//TODO guide the robot till we reach near the person
}

void testApp::follow_face(const int &face_depth,const int &face_x, const int &face_y)
{
			const int CW = 100;
			const int TH = 200;
			//const float THM = 0.50;
			int x1 = (320-CW)/2;
			int x2 = x1+CW;
			

			if( face_depth<TH && !bNearFace)
			{
			 	//code to adjust the bot at the center.
				if( face_x>=x1 && face_x<=x2 )
				{
					status_str.str("");
					status_str<<"SENDING FORWARD COMMAND";
				    send_command(FORWARD); //send forward command to arduino
				}	
				if (face_x<x1)
				{
					status_str.str("");
					status_str<<"SENDING LEFT COMMAND";
					send_command(LEFT); // send left command to arduino
				}
				if (face_x>x2)
				{
					status_str.str("");
					status_str<< "SENDING RIGHT COMMAND";
					send_command(RIGHT); // send right command to arduino
				}
			}
			else if(face_depth>TH)
			{
				//immediately stop the bot	
				send_command(STOP);
				status_str.str("");
				status_str<<"NEAR OBJECT RUNNING GRABBER MODULE";
				bNearFace = true;
				send_command(OPEN_GRIPPER); //we are near the person know release the object
				ofSleepMillis(5000);
			}
}

inline void testApp::send_command(int command)
{
	if(BSERIAL_MODULE_ON)
		serial.writeByte(command);
}

void testApp::process_depth_frame(ofxCvGrayscaleImage &frame)
{
		cvResize(frame.getCvImage(),frame_320_240.getCvImage());
		//frame_320_240.dilate_3x3();
		thresholded_img = frame_320_240;
		depth_threshold(thresholded_img.getCvImage(),nearThreshold,farThreshold);
		thresholded_img.dilate_3x3();
		//dilate(grayImage2.getCvImage(),dil_se.getCvImage(),2,2,out_img.getCvImage());
    	hit_or_miss2(thresholded_img.getCvImage(),se1.getCvImage(),se2.getCvImage(),SE_HEIGHT/2,SE_WIDTH/2,output_img.getCvImage());
		//erode(out_img.getCvImage(),se1.getCvImage(),SIZE/2,SIZE/2,out_img_e.getCvImage());
		contourFinder.findContours(output_img, 0,30*30,1,false,true);
		if(contourFinder.blobs.size()==0)
		{
			status_str.str("");
			status_str<< "SCANNING..."<<(farThreshold/255.0)*100.0<<" %";
			send_command(STOP); //send stop bot command here until we do not find the object again.
			if(farThreshold<255) farThreshold+=2;
			//start scanning again
			if(farThreshold>=255) farThreshold=100;
		}
		else
		{
			const int CW = 50;
			const int TH = 246;
			//const float THM = 0.50;
			int x1 = (320-CW)/2;
			int x2 = x1+CW;
			int object_x = contourFinder.blobs[0].centroid.x;
			int object_y = contourFinder.blobs[0].centroid.y;
			//BwImage _frame(frame.getCvImage());
			//the depth calculation need to done in a robust way.
			object_depth = get_gray_depth(frame_320_240.getCvImage(),object_x,object_y); //_frame[object_y][object_x]; //get the depth of the glass
			//object_depth_in_m = get_depth_in_m(kinect,object_x*2,object_y*2); //multiplied by 2 because we are working with 320*240 frames
			cout<<object_x<<","<<x1<<","<<x2<<endl;
			if(object_depth>200)
			{
				kinect.setCameraTiltAngle(-15);
				//make_structuring_element_b1(se1.getCvImage(),1,20,0);
			}

			if(object_depth>230)
			{
				make_structuring_element_b1(se1.getCvImage(),1,20,0);
				se1.flagImageChanged();
			}
			

			if( object_depth<TH && !bNearObject)
			{
			 	//code to adjust the bot at the center.
				if( object_x>=x1 && object_x<=x2 )
				{
					status_str.str("");
					status_str<<"SENDING FORWARD COMMAND";
				    send_command(FORWARD); //send forward command to arduino
				}	
				if (object_x<x1)
				{
					status_str.str("");
					status_str<<"SENDING LEFT COMMAND";
					send_command(LEFT); // send left command to arduino
				}
				if (object_x>x2)
				{
					status_str.str("");
					status_str<< "SENDING RIGHT COMMAND";
					send_command(RIGHT); // send right command to arduino
				}
			}
			else if(object_depth>TH)
			{
				//immediately stop the bot	
				send_command(STOP);
				status_str.str("");
				status_str<<"NEAR OBJECT RUNNING GRABBER MODULE";
				
				const int center_x = 160;
				int shift = abs(object_x-center_x); //shift of the object from the center of the bot
				cout<<"SHIFT:"<<shift<<endl;
				//adjust the pan according to the shift
				if(object_x>center_x)
				{
					send_command(90-(int)(0.5*shift));
				}
				else
				{
					send_command(90+(int)(0.5*shift));
				}
				ofSleepMillis(200);
				
				bNearObject = true;
				send_command(CLOSE_GRIPPER);
				ofSleepMillis(2000);
				
				for(int i=0;i<150;i++)
				{
					send_command(BACKWARD);
					ofSleepMillis(100);
				}
				ofExit(0);
				/*
				for(int i=0;i<250;i++)
				{
					send_command(RIGHT);
					ofSleepMillis(20);
				}


				for(int i=0;i<40;i++)
				{
					send_command(FORWARD);
					ofSleepMillis(100);
				}
				*/
				//TODO move backward then turn 180 degrees set BFACE_TRACKER_MODULE_ON = true	
				
				//TODO run grabber module
				//wait for the notification from arduino that the object is grabbed
				//use serial.readbyte() perodically check for notification.
				//on arduino use serial.write(val) command to send some value to this application
			}
			
		}
		frame.flagImageChanged();
		frame_320_240.flagImageChanged();	
		thresholded_img.flagImageChanged();
		output_img.flagImageChanged();
	
}

#ifdef DEPTH_VIDEO_MODULE_ON
void testApp::depth_video_update()
{
	if(frame_index < frame_count)
	{
	//cout<<"frame:"<<frame_index<<endl;
	video_frame = ovideo_frame.getCvImage();
	BwImage _g_img(video_frame);
	if(video_file->is_open()) 
	{
		video_file->read(depth_buffer,GRAY_BUFFERSIZE);
		int i,j;		
		for(i=0;i<HEIGHT;i++)
        		for(j=0;j<WIDTH;j++)
            		_g_img[i][j] = udepth_buffer[j+i*WIDTH];
		//processing goes here.
		process_depth_frame(ovideo_frame);
		frame_index++;
    }
   	else{	
   		cout<<"unable to open: "<<file_name<<endl;  
   		}
	}
}
#endif

//--------------------------------------------------------------
void testApp::draw() {

	ofSetColor(255, 255, 255);
	if(bDrawPointCloud) {
		easyCam.begin();
		drawPointCloud();
		easyCam.end();
	} else {
#ifdef DEPTH_VIDEO_MODULE_ON
	ovideo_frame.draw(0,0,320,240); //if depth video module on draw video frame
#else
	frame_320_240.draw(0,0,320,240);
	//kinect.drawDepth (0,0,320,240); //else draw frame recieved from kinect
#endif
	thresholded_img.draw    (320,0,320,240);
	output_img.draw  (640,0,320,240);
	contourFinder.draw(640,0,320,240);
	if(contourFinder.blobs.size()>0)
	{
		int cx = contourFinder.blobs[0].centroid.x;
		int cy = contourFinder.blobs[0].centroid.y;
		stringstream objecttext;
		objecttext<<"object["<<cx<<","<<cy<<"]:"<<object_depth;
		ofDrawBitmapString(objecttext.str(),640+cx,cy-15);
		ofDrawBitmapString(objecttext.str(),320+cx,cy-15);
	}
#ifndef DEPTH_VIDEO_MODULE_ON
	kinect.draw(0,240,320,240);
#endif
		se1.draw(320,240);
		se2.draw(320+SE_WIDTH,240);
	}

	// draw instructions
	ofSetColor(255, 255, 255);
	ofDrawBitmapString(status_str.str(),320,300);

	stringstream reportStream;
	reportStream << "Press P for Point Cloud" << endl
	<< "near threshold: " << nearThreshold<< endl
	<< "far  threshold: " << farThreshold<<endl   //num blobs found " << contourFinder.nBlobs
	<< "FPS: " << ofGetFrameRate() << endl
	<< "KINECT ANGLE:"<<angle<<endl
	<< "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;
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
	if(BSERIAL_MODULE_ON)
	{
		send_command(STOP);
		send_command(RESET_ARM);
		send_command(CLOSE_GRIPPER);
		serial.close();
	}
#ifdef DEPTH_VIDEO_MODULE_ON
	video_file->close();
#endif
}


void testApp::keyPressed(int key)
{
/*
	switch(key)
	{
		case 'i': serial.writeByte(FORWARD);  break;
		case 'k': serial.writeByte(BACKWARD); break;
		case 'j': serial.writeByte(LEFT);     break;
		case 'l': serial.writeByte(RIGHT);    break;
		case 'o': serial.writeByte(OPEN_GRIPPER); break;
		case 'c': serial.writeByte(CLOSE_GRIPPER); break;
		case 'r': serial.writeByte(RESET_ARM); break;
		case OF_KEY_UP:   serial.writeByte(12); break;// serial.writeByte(TILT_UP);   break;
		case OF_KEY_DOWN: serial.writeByte(13); break;// serial.writeByte(TILT_DOWN); break;
		case OF_KEY_LEFT:  serial.writeByte(PAN_LEFT);  break;
		case OF_KEY_RIGHT: serial.writeByte(PAN_RIGHT); break;
		default : serial.writeByte(STOP);     break;
	}
*/	
}

/*
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
*/
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

