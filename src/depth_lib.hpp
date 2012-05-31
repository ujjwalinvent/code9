#pragma once
#include<string>
#include<iostream>
#include<fstream>
#include"cv_util.hpp"
#include"morph.hpp"
using namespace std;
#define USHORT_BUFFERSIZE 640*480*2
#define GRAY_BUFFERSIZE 640*480
#define RGB_BUFFERSIZE 640*480*3
#define HEIGHT 480
#define WIDTH 640



void make_structuring_element_b1(IplImage *se,const int &se_w,const int &se_h1,const int &se_h2)
{
	assert(se!=NULL);
	assert(se->nChannels==1);
	cvZero(se);
	int img_w = se->width;
	int img_h = se->height;
	//cout<<"*"<<img_w<<","<<img_h<<endl;
	int startj = (img_w - se_w)/2;	
	int starti = se_h1;
	fill_img_range(se,starti,img_h-se_h2,startj,startj+se_w,255);
	starti = img_h-se_h2;
	startj = 0;
	fill_img_range(se,starti,img_h,startj,img_w,255);
}

void make_structuring_element_b2(IplImage *se2,const int &w,const int &h1,const int &h2)
{
	assert(se2!=NULL);
	assert(se2->nChannels==1);
	cvZero(se2);
	int starti = 0;
	int startj = 0;
	int endi = h1;
	int endj = se2->width;
	fill_img_range(se2,starti,endi,startj,endj,255);
	starti = h1;
	startj = 0;
	endi = se2->height-h2;
	endj = w;
	fill_img_range(se2,starti,endi,startj,endj,255);
	starti = h1;
	startj = se2->width-w;
	endi = se2->height-h2;
	endj = se2->width;
	fill_img_range(se2,starti,endi,startj,endj,255);
}

/*
void make_structuring_element2(IplImage *se1,IplImage *se2,const int &w)
{
	assert(se1!=NULL);
	assert(se1->nChannels==1);
	assert(se2!=NULL);
	assert(se2->nChannels==1);
	IplImage *dil_se=get_bwimg(5,5);
	IplImage *se1_t = get_clone(se1);
	IplImage *se2_t = get_clone(se2);
	cvSet(dil_se,cvScalar(255));
	cvZero(se2_t);
	copy_bwimg(se1,se1_t);
//	cvSet(dil_se,cvScalar(255));

	for(int i=0;i<w;i++)
	{
		dilate(se1_t,dil_se,2,2,se2_t);
		copy_bwimg(se2_t,se1_t);
	}
	complement(se2_t,se2);
	cvReleaseImage(&dil_se);
	cvReleaseImage(&se2_t);
	cvReleaseImage(&se1_t);
}
*/

void depth_threshold(IplImage *gray_img,const int &nearThreshold,const int &farThreshold)
{
    BwImage _gray_img(gray_img);
    int i,j;
    for(i = 0; i < gray_img->height; i++)
    {
        for(j=0;j<gray_img->width;j++)
        {
        if(_gray_img[i][j] < nearThreshold && _gray_img[i][j] > farThreshold)
            _gray_img[i][j] = 255;
         else
            _gray_img[i][j] = 0;
        }
    }
}

int get_gray_depth(IplImage *gray_img,const int &x,const int &y)
{
	int sum=0,count=0;
	BwImage _gray_img(gray_img);
	for(int i = y; i<=y+10; i++ )
	{
		if(i>=0&&i<gray_img->height)
		{
			if(_gray_img[i][x]!=0)
			{
				sum+=(int)_gray_img[i][x];
				count++;
			}	
		}
	}
	if(count==0)
		return 0;
	else
		return (sum/count);
}

float get_depth_in_m(ofxKinect &kinect,const int &x,const int &y)
{
	//cout<<"get_depth_in_m\n";
	float sum=0,count=0,val;
	for(int i = y-5; i<=y+5;i++)
	{
		if(i>=0 && i<kinect.height)
		{
			val = kinect.getDistanceAt(x,i);
			if(val>0)
			{
				sum += val;
				count++;
			}
		}
	}
	if(count==0) return 0.0;
	else return sum/count;
	
}


float get_depth_in_meters(unsigned short *ushort_pixels,const int &i,const int &j)
{
	return (ushort_pixels[i*WIDTH+j]/1000.0);
}

long get_file_size(const string &file_name)
{
	long begin,end;
	ifstream v_file(file_name.c_str());
	begin = v_file.tellg();
	v_file.seekg(0,ios::end);
	end = v_file.tellg();
	v_file.close();
	return (end-begin);
}

void read_depth_video_file(const string &file_name)
{
	long size = get_file_size(file_name);
	assert(size%(GRAY_BUFFERSIZE)==0);
	int frame_count=size/(GRAY_BUFFERSIZE);

	ifstream v_file(file_name.c_str(),ios::in|ios::binary);
	IplImage *g_img = get_bwimg(WIDTH,HEIGHT);
	assert(g_img!=NULL);
	BwImage _g_img(g_img);
	cvNamedWindow("depth_video");
	char *buffer = new char[GRAY_BUFFERSIZE];	
    	unsigned char *buffer2 = (unsigned char*)buffer;
   	int i,j,k;
	cout<<"FRAME_COUNT:"<<frame_count<<endl;
	for(k=0;k<frame_count;k++)
	{
		cout<<"k:"<<k<<endl;
		if(v_file.is_open())
        		v_file.read(buffer,GRAY_BUFFERSIZE);
    		else {
			cout<<"unable to open: "<<file_name<<endl;
			break;
    		}
		
		for(i=0;i<HEIGHT;i++)
    		{
        		for(j=0;j<WIDTH;j++)
        		{
            		_g_img[i][j] = buffer2[j+i*WIDTH];
        		}
    		}
		//processing goes here.
		cvShowImage("depth_video",g_img);
		cvWaitKey(0);
	}
}

void process_depth_frame(IplImage *g_img)
{
	//TODO code 
}

void read_depth_file(const int &file_counter,unsigned short **ushort_pixels,IplImage *ipl_rgb_img,IplImage *ipl_gray_img)
{
    stringstream ushort_filename;
    stringstream gray_filename;
    stringstream rgb_filename;
    ushort_filename<<"data/ushort"<<file_counter;
    gray_filename<<"data/gray"<<file_counter;
    rgb_filename<<"data/rgb"<<file_counter;
    RgbImage rgb_img(ipl_rgb_img);
    BwImage gray_img(ipl_gray_img);

    ifstream ushort_file(ushort_filename.str().c_str(),ios::in|ios::binary);
    ifstream gray_file(gray_filename.str().c_str(),ios::in|ios::binary);
    ifstream rgb_file(rgb_filename.str().c_str(),ios::in|ios::binary);
    char *ushort_buffer = new char[USHORT_BUFFERSIZE];
    char *gray_buffer = new char[GRAY_BUFFERSIZE];
    char *rgb_buffer = new char[RGB_BUFFERSIZE];

    if(ushort_file.is_open())
        ushort_file.read(ushort_buffer,USHORT_BUFFERSIZE);
    else cout<<"unable to open: "<<ushort_filename<<endl;

    if(gray_file.is_open())
        gray_file.read(gray_buffer,GRAY_BUFFERSIZE);
    else cout<<"unable to open: "<<gray_filename<<endl;

    if(rgb_file.is_open())
        rgb_file.read(rgb_buffer,RGB_BUFFERSIZE);
    else cout<<"unable to open: "<<rgb_filename<<endl;

    *ushort_pixels = (ushort *)ushort_buffer;
    unsigned char *gray_buffer2 = (unsigned char*)gray_buffer;
    int i,j;
    for(i=0;i<HEIGHT;i++)
    {
        for(j=0;j<WIDTH;j++)
        {
            gray_img[i][j] = gray_buffer2[j+i*WIDTH];
        }
    }

    for(i=0;i<HEIGHT;i++)
    {
        for(j=0;j<WIDTH;j++)
        {
            rgb_img[i][j].r = rgb_buffer[(j+i*WIDTH)*3+0];
            rgb_img[i][j].g = rgb_buffer[(j+i*WIDTH)*3+1];
            rgb_img[i][j].b = rgb_buffer[(j+i*WIDTH)*3+2];
        }
    }

    ushort_file.close();
    gray_file.close();
    rgb_file.close();
}

