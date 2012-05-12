#pragma once
#include<cstdarg>
#include<cv.h>
#include<highgui.h>
#include<iostream>
#include<cstdio>
using namespace std;
/**
this file contains opencv related utilities.
*/

// Record the execution time of some code, in milliseconds.
#define DECLARE_TIMING(s)  int64 timeStart_##s; double timeDiff_##s; double timeTally_##s = 0; int countTally_##s = 0
#define START_TIMING(s)    timeStart_##s = cvGetTickCount()
#define STOP_TIMING(s) 	   timeDiff_##s = (double)(cvGetTickCount() - timeStart_##s); timeTally_##s += timeDiff_##s; countTally_##s++
#define GET_TIMING(s) 	   (double)(timeDiff_##s / (cvGetTickFrequency()*1000.0))
#define GET_AVERAGE_TIMING(s)   (double)(countTally_##s ? timeTally_##s/ ((double)countTally_##s * cvGetTickFrequency()*1000.0) : 0)
#define CLEAR_AVERAGE_TIMING(s) timeTally_##s = 0; countTally_##s = 0

/*
// Example:
DECLARE_TIMING(myTimer);
while (something)
{
    START_TIMING(myTimer);
    printf("Hello!\n");
    STOP_TIMING(myTimer);
}
printf("Execution time: %f ms.\n", GET_TIMING(myTimer) );
printf("Average time: %f ms per iteration.\n", GET_AVERAGE_TIMING(myTimer) );

*/

template<class T> class Image
{
//  private:
  public:
  IplImage* imgp;
  Image(IplImage* img=0) {imgp=img;}
  ~Image(){imgp=0;}
  void operator=(IplImage* img) {imgp=img;}
  inline T* operator[](const int rowIndx) {
    return ((T *)(imgp->imageData + rowIndx*imgp->widthStep));}
};

typedef struct{
  unsigned char b,g,r;
} RgbPixel;

typedef struct{
  float b,g,r;
} RgbPixelFloat;

typedef Image<RgbPixel>       RgbImage;
typedef Image<RgbPixelFloat>  RgbImageFloat;
typedef Image<unsigned char>  BwImage;
typedef Image<float>          BwImageFloat;


void print_img(IplImage *img)
{
    BwImage A2(img);
    int i,j;
    printf("\n");
    for(i=0;i<A2.imgp->height;i++)
    {
        for(j=0;j<A2.imgp->width;j++)
        {
	    switch(A2[i][j])
	    {
	    	case 255: printf("%2d",1);
			  break;
		case 254: printf("%2d",-1);
			  break;
	 	case 0 : printf("%2d",0);
	    }
        }
        printf("\n");
    }
    printf("\n");
}

inline IplImage *get_bwimg(int w,int h)
{
	return cvCreateImage(cvSize(w,h),8,1);
}

inline IplImage *get_rgbimg(int w,int h)
{
	return cvCreateImage(cvSize(w,h),8,3);
}

inline IplImage *get_clone(IplImage *img)
{
	return cvCreateImage(cvGetSize(img),img->depth,img->nChannels);
}


/**
* @brief function to fill image in a range
*
* @param img : the image to be filled
* @param i1 : the starti
* @param i2 : the endi
* @param j1 : the startj
* @param j2 : the endj
* @param val : the value used to fill
*/
inline void fill_img_range(IplImage *img,const int &i1,const int &i2,const int &j1,const int &j2,const unsigned char &val)
{
	int i,j;
	BwImage _img(img);
	for(i=i1;i<i2;i++)
		for(j=j1;j<j2;j++)
			_img[i][j] = val;
}


void fill_img(IplImage *img,int *arr)
{
	int i,j;
	cvZero(img);
	BwImage _img(img);
	for(i=0;i<img->height;i++)
	{	
		for(j=0;j<img->width;j++)
		{
			if(arr[j+i*(img->width)]==1) 
				_img[i][j] = 255;
			else if(arr[j+i*(img->width)]==-1) 
				_img[i][j] = 254;
		}
	}
}

bool isZero(IplImage *img)
{
	BwImage _img(img);
	int i,j;
	for(i=0;i<img->height;i++)
		for(j=0;j<img->width;j++)
			if(_img[i][j]) return false;
	return true;
}

bool is_img_changed(IplImage *old_img,IplImage *new_img)
{
	assert(old_img!=NULL);
	assert(new_img!=NULL);
	BwImage _old_img(old_img);
	BwImage _new_img(new_img);
	int i,j;
	for(i=0;i<old_img->height;i++)
		for(j=0;j<old_img->width;j++)
			if(_old_img[i][j]!=_new_img[i][j]) return true;
	
	return false;
}

void copy_bwimg(IplImage *copy_from,IplImage *copy_to)
{
	assert(copy_from!=NULL);
	assert(copy_to!=NULL);
	BwImage _copy_from(copy_from);
	BwImage _copy_to(copy_to);
	int i,j;
	for(i=0;i<copy_from->height;i++)
		for(j=0;j<copy_from->width;j++)
			_copy_to[i][j] = _copy_from[i][j];
}


/*
#define PI 3.14159265
//NOT WORKING CORRECTLY
void rotate_left(IplImage *se)
{
	IplImage *se2 = get_clone(se);
	BwImage _se2(se2);
	BwImage _se(se);
	int i,j,i1,j1;
	for(i=0;i<se->width;i++)
	{	
		for(j=0;j<se->height;j++)
		{
			i = i-1;
			j = j-1;
			i1 = i*sin(90*PI/180)-j*cos(90*PI/180);
			j1 = i*sin(90*PI/180)+j*cos(90*PI/180);
			i1++;
			j1++;
			i++;
			j++;
			_se2[i1][j1] = _se[i][j];
		}
	}
	copy_bwimg(se2,se);
	cvReleaseImage(&se2);
}
*/
inline void disp_img(IplImage* img,char *name)
{
    cvNamedWindow(name);
    cvShowImage(name,img);
}

void cvShowManyImages(char* title, int nArgs, ...) {

    // img - Used for getting the arguments
    IplImage *img;

    // DispImage - the image in which input images are to be copied
    IplImage *DispImage;

    int size;
    int i;
    int m, n;
    int x, y;

    // w - Maximum number of images in a row
    // h - Maximum number of images in a column
    int w, h;

    // scale - How much we have to resize the image
    float scale;
    int max;

    // If the number of arguments is lesser than 0 or greater than 12
    // return without displaying
    if(nArgs <= 0) {
        printf("Number of arguments too small....\n");
        return;
    }
    else if(nArgs > 12) {
        printf("Number of arguments too large....\n");
        return;
    }
    // Determine the size of the image,
    // and the number of rows/cols
    // from number of arguments
    else if (nArgs == 1) {
        w = h = 1;
        size = 300;
    }
    else if (nArgs == 2) {
        w = 2; h = 1;
        size = 300;
    }
    else if (nArgs == 3 || nArgs == 4) {
        w = 2; h = 2;
        size = 300;
    }
    else if (nArgs == 5 || nArgs == 6) {
        w = 3; h = 2;
        size = 200;
    }
    else if (nArgs == 7 || nArgs == 8) {
        w = 4; h = 2;
        size = 200;
    }
    else {
        w = 4; h = 3;
        size = 150;
    }

    // Create a new 3 channel image
    DispImage = cvCreateImage( cvSize(100 + size*w, 60 + size*h), 8, 1 );
    //RGBDispImage = cvCreateImage( cvSize(100 + size*w, 60 + size*h), 8, 3 );

    // Used to get the arguments passed
    va_list args;
    va_start(args, nArgs);


    // Loop for nArgs number of arguments
    for (i = 0, m = 20, n = 20; i < nArgs; i++, m += (20 + size)) {

        // Get the Pointer to the IplImage
        img = va_arg(args, IplImage*);

        // Check whether it is NULL or not
        // If it is NULL, release the image, and return
        if(img == 0) {
            printf("Invalid arguments");
            cvReleaseImage(&DispImage);
            return;
        }

        // Find the width and height of the image
        x = img->width;
        y = img->height;

        // Find whether height or width is greater in order to resize the image
        max = (x > y)? x: y;

        // Find the scaling factor to resize the image
        scale = (float) ( (float) max / size );

        // Used to Align the images
        if( i % w == 0 && m!= 20) {
            m = 20;
            n+= 20 + size;
        }

        // Set the image ROI to display the current image
        cvSetImageROI(DispImage, cvRect(m, n, (int)( x/scale ), (int)( y/scale )));

        // Resize the input image and copy the it to the Single Big Image
        cvResize(img, DispImage);

        // Reset the ROI in order to display the next image
        cvResetImageROI(DispImage);
    }

    // Create a new window, and show the Single Big Image
    cvNamedWindow( title, 1 );
    cvShowImage( title, DispImage);

    cvWaitKey();
    cvDestroyWindow(title);

    // End the number of arguments
    va_end(args);

    // Release the Image Memory
    cvReleaseImage(&DispImage);
}

