/**
Test the time complexity of the algorithms: done
Testing on video from kinect with dilation and without dilation.
Testing of hit or miss transform for complexity:done
pinpoint the cluster of detected points
check every function for memory allocation problems.
because if any of the operation gets called in a loop then it will allocate too much memory
questions:
Is structuring element always square or can have a different shape?
Is dont care and not in the set are the same thing?
what happens when we erode a black image with zero structuring element?

*/
/**
Morphological Processing Library
Author : Ujjwal kumar
Version: 1.0
updated on: 11 May 2012
*/

#pragma once
#include<cassert>
#include<iostream>
#include<vector>
#include<cstdio>
#include<cstdarg>
#include<cv.h>
#include<highgui.h>
#include"cv_util.hpp"
using namespace std;

/*
#define IN_SET_VAL 255
#define NOT_IN_SET_VAL 0
#define NOT_IN_SET_VAL_C 254
#define DONT_CARE_VAL 253
*/

typedef struct Pixel{
    int row; //row index of the pixel
    int col; //column index of the pixel
} pixel;

void print_pixel_vector(vector<pixel> &pv)
{
    for(unsigned int i=0;i<pv.size();i++)
        cout<<pv[i].row<<","<<pv[i].col<<endl;
    cout<<endl;
}

/**
make set out of structuring element
Characteristic of structuring element:
A pixel is in the set of structuring element completely.
A pixel is not in the set of structuring element.
A pixel is not in the set and neither it is complemented in complement operation.
A pixel is in the set of structuring element but marked as dontCare.
*/

/**
* @brief this is just awesome
*
* @param bv
* @param se
* @param pi
* @param pj
*/
void make_set(vector<pixel> &bv,IplImage *se,const int pi,const int pj)
{
	
    BwImage B(se);
    int i,j;
    pixel p;
    for(i=0;i<se->height;i++)
    {
        for(j=0;j<se->width;j++)
        {
            if(B[i][j]==255) //if the value is 255 only then it is part of structuring element
            {
                p.row  = i - pi;
                p.col  = j - pj;
                bv.push_back(p);
            }
	}
    }
}
/**
Erosion operation
Input_img: the image to be eroded [must be single channel image]
Structuring_element: the structuring element used for erosion[must be single channel image]
out_img: the Image where we will store the output.
pi: the y coordinate of the pivot point[the row index of the pivot point]
pj: the x coordinate of the pivot point[the column index of the pivot point]
note: a pixel is considered to in the set if its non-zero
*/
void erode(IplImage *input_img, IplImage *structuring_element,const int pi,const int pj,IplImage *out_img)
{
//	cout<<"Erosion Operation\n";
	assert(input_img!=NULL);
	assert(structuring_element!=NULL);
	assert(out_img!=NULL);
	assert(input_img->nChannels==1);
	assert(structuring_element->nChannels==1);
	assert(out_img->nChannels==1);
    BwImage A(input_img);
    cvZero(out_img);
    BwImage _out_img(out_img);

    vector<pixel> bv;
    make_set(bv,structuring_element,pi,pj);

    int i,j,row,col;
    unsigned int k,count;
    for(i=0;i<input_img->height;i++)
    {
        for(j=0;j<input_img->width;j++)
        {
            count=0;
            for(k=0;k<bv.size();k++)
            {
                        row  = i + bv[k].row;
                        col  = j + bv[k].col;
                        if( row<0 || row>=(A.imgp->height) || col<0 || col>=(A.imgp->width))
                            break;
                        else if(A[row][col])
                            count++;
                        else if(!A[row][col])
                            break;

            }
            if(count!=0&&count==bv.size())   _out_img[i][j]=255;
        }
    }
}


/**
Dilation  operation
Input_img: the image to be dilated [must be single channel image]
Structuring_element: the structuring element used for dilation[must be single channel image]
out_img: the image where we will store the output
pi: the y coordinate of the pivot point[the row index of the pivot point]
pj: the x coordinate of the pivot point[the column index of the pivot point]
note: a pixel is considered in the set if and only if it is non-zero.[In apperance: not black]
*/
void dilate(IplImage *input_img, IplImage *structuring_element,const int pi,const int pj,IplImage *out_img)
{
//    cout<<"Dilate Operation\n";
    assert(input_img!=NULL);
    assert(structuring_element!=NULL);
    assert(input_img->nChannels==1);
    assert(structuring_element->nChannels==1);
    
    cvZero(out_img);
    BwImage A(input_img);
    BwImage _out_img(out_img);

    vector<pixel> bv;
    make_set(bv,structuring_element,pi,pj);
    int i,j,row,col;
    unsigned int k;
    for(i=0;i<input_img->height;i++)
    {
        for(j=0;j<input_img->width;j++)
        {
            if(A[i][j])
            {
                for(k=0;k<bv.size();k++)
                {
                            row = i + bv[k].row;
                            col = j + bv[k].col;
                            if(row>=0 && row<(input_img->height) &&
                               col>=0 && col<(input_img->width))
                             _out_img[row][col] = 255;
                }
            }
        }
    }
}

void complement(IplImage *in,IplImage *out_img)
{
    assert(in!=NULL);
    assert(out_img!=NULL);
    cvZero(out_img);
    int i,j;
    BwImage A(in);
    BwImage A2(out_img);
    for(i=0;i<in->height;i++)
    {
        for(j=0;j<in->width;j++)
        {
            if(A[i][j]==255)
                A2[i][j] = 0;
            else if(A[i][j]==0)
                A2[i][j] = 255;
	    else 
	    	A2[i][j] = A[i][j];
        }
    }
}


//take the intersection of two single channel images
void intersection_op(IplImage *img1,IplImage *img2,IplImage *out_img)
{
    assert(img1!=NULL);
    assert(img2!=NULL);
    assert(out_img!=NULL);
    BwImage bimg1(img1);
    BwImage bimg2(img2);
    //IplImage *out_img = cvCreateImage(cvGetSize(img1),img1->depth,img1->nChannels);
    BwImage oimg(out_img);
    //cout<<"*"<<img1->height<<","<<img1->width<<endl;
    int i,j;
    for(i=0;i<img1->height;i++)
    {
        for(j=0;j<img1->width;j++)
        {
            //cout<<i<<","<<j<<endl;
            if((bimg1[i][j])&&(bimg2[i][j]))
                oimg[i][j] = 255;
            else
                oimg[i][j] = 0;
        }
    }
    //return out_img;
}

/**
out: to store the output of the union of images
nArgs: number of images on which union operation will take place
... : variable list of images 
*/
void union_op(IplImage *out,int nArgs, ...)
{
	assert(out!=NULL);
	va_list args;
	va_start(args,nArgs);
	BwImage _img;
	BwImage _out(out);
	cvZero(out);
	IplImage *img;
	int i,j,k;
	for(k = 0; k < nArgs;k++)
	{
		
		img = va_arg(args,IplImage *);
		assert(img!=NULL);
		_img=img;
		for(i = 0;i< out->height;i++)
			for(j=0;j<out->width;j++)
				_out[i][j] = _out[i][j]|_img[i][j];
	}
	va_end(args);
}
/*
void union_op(IplImage *img1,IplImage *img2,IplImage *out)
{
	assert(img1!=NULL);
	assert(img2!=NULL);
	assert(out!=NULL);

	BwImage bimg1(img1);
	BwImage bimg2(img2);

	BwImage _out(out);
	int i,j;
	for(i=0;i<img1->height;i++)
	{
		for(j=0;j<img1->width;j++)
		{
			if(bimg1[i][j]||bimg2[i][j])
				_out[i][j] = 255;
			else
				_out = 0;
		}	
	}
}
*/
/**
Input_img: the image on which the hit_or_miss transform will take place
structuring_element : the structuring element used for hit or miss transform.

*/
void hit_or_miss(IplImage *input_img,IplImage *structuring_element,const int pi,const int pj,IplImage *out_img)
{
    assert(input_img!=NULL);
    assert(structuring_element!=NULL);
    assert(out_img!=NULL);
    assert(input_img->nChannels==1);
    assert(structuring_element->nChannels==1);
    assert(out_img->nChannels==1);

    IplImage *output_img_e1 = get_clone(input_img);
    erode(input_img,structuring_element,pi,pj,output_img_e1);
//    print_img(output_img_e1);

    IplImage *output_img_c = get_clone(input_img); 
    complement(input_img,output_img_c);

    IplImage *se_c = get_clone(structuring_element);
    complement(structuring_element,se_c);
//    print_img(se_c);
   

    IplImage *output_img_e2 = get_clone(input_img);
    erode(output_img_c,se_c,pi,pj,output_img_e2);
//    cout<<"Input\n";
//    print_img(output_img_c);
//    cout<<"Erosion\n";
//    print_img(output_img_e2);
    intersection_op(output_img_e1,output_img_e2,out_img);
    cvReleaseImage(&output_img_e1);
    cvReleaseImage(&output_img_e2);
    cvReleaseImage(&output_img_c);
    cvReleaseImage(&se_c);
}

void hit_or_miss_w(IplImage *in,IplImage *se,const int pi,const int pj,const int w,IplImage *out)
{
    assert(in!=NULL);
    assert(se!=NULL);
    assert(out!=NULL);
    assert(in->nChannels==1);
    assert(se->nChannels==1);
    assert(out->nChannels==1);

    IplImage *out_img_e1 = get_clone(in);
    erode(in,se,pi,pj,out_img_e1);
//    print_img(output_img_e1);

    IplImage *out_img_c = get_clone(in); 
    complement(in,out_img_c);

    int dil_searr[] = {
    		   1,1,1,
    		   1,1,1,
		  	   1,1,1
    			};
    IplImage *dil_se = get_bwimg(3,3);
    IplImage *se2 = get_clone(se);
    fill_img(dil_se,dil_searr);
    for(int i=0;i<w;i++)
    {
    	dilate(se,dil_se,1,1,se2);
	copy_bwimg(se2,se);
    }
    //print_img(se);
    IplImage *se_c = get_clone(se);
    complement(se,se_c);
    //print_img(se_c);
   

    IplImage *out_img_e2 = get_clone(in);
    erode(out_img_c,se_c,pi,pj,out_img_e2);
//    cout<<"Input\n";
//    print_img(output_img_c);
//    cout<<"Erosion\n";
//    print_img(output_img_e2);
    intersection_op(out_img_e1,out_img_e2,out);
    cvReleaseImage(&out_img_e1);
    cvReleaseImage(&out_img_e2);
    cvReleaseImage(&out_img_c);
    cvReleaseImage(&se_c);
    cvReleaseImage(&se2);
    cvReleaseImage(&dil_se);
}


void hit_or_miss2(IplImage *in,IplImage *se1,IplImage *se2,const int pi,const int pj,IplImage *out)
{
    assert(in!=NULL);
    assert(se1!=NULL);
    assert(se2!=NULL);
    assert(out!=NULL);
    assert(in->nChannels==1);
    assert(se1->nChannels==1);
    assert(se2->nChannels==1);
    assert(out->nChannels==1);

    IplImage *out_img_e1 = get_clone(in);
    erode(in,se1,pi,pj,out_img_e1);
    IplImage *out_img_c = get_clone(in); 
    complement(in,out_img_c);
    IplImage *out_img_e2 = get_clone(in);
    erode(out_img_c,se2,pi,pj,out_img_e2);
    intersection_op(out_img_e1,out_img_e2,out);

    cvReleaseImage(&out_img_e1);
    cvReleaseImage(&out_img_e2);
    cvReleaseImage(&out_img_c);
}

void thinning_op(IplImage *in,IplImage *out)
{
	BwImage _in(in);
	int searr0[9] = {
			 0, 0, 0,
			-1, 1,-1,
			 1, 1, 1
			};
	int searr1[9] = {
			-1, 0, 0,
			 1, 1, 0,
			 1, 1,-1
			};
	int searr2[9] = {
			 1,-1, 0,
			 1, 1, 0,
			 1,-1, 0
			};
	int searr3[9] = {
			 1, 1,-1,
			 1, 1, 0,
			-1, 0, 0
			};
	int searr4[9] = {
			 1, 1, 1,
			-1, 1,-1,
			 0, 0, 0
			};
	int searr5[9] = {
			-1, 1, 1,
			 0, 1, 1,
			 0, 0,-1
			};
	int searr6[9] = {
			 0,-1, 1,
			 0, 1, 1,
			 0,-1, 1
			};
	int searr7[9] = {
			 0, 0,-1,
			 0, 1, 1,
			-1, 1, 1
			};
	IplImage *se[8];
	for(int i = 0 ; i < 8;i++)
		se[i] = get_bwimg(3,3);
	fill_img(se[0],searr0);
	fill_img(se[1],searr1);
	fill_img(se[2],searr2);
	fill_img(se[3],searr3);
	fill_img(se[4],searr4);
	fill_img(se[5],searr5);
	fill_img(se[6],searr6);
	fill_img(se[7],searr7);
	
	IplImage *out2          = get_clone(in);
	IplImage *out2_c        = get_clone(in);
	int i = 0;
	int count=0;
	while(true)
	{
		count=0;
		for(i=0;i<8;i++)
		{
			hit_or_miss(in,se[i],1,1,out2);
			if(isZero(out2)) count++;
		//	else count--;
			complement(out2,out2_c);	
			intersection_op(in,out2_c,out);
			in = out;
		}
		if(count==8) break; //all structuring element detected nothing
	}
	for(i=0;i<8;i++)
		cvReleaseImage(&se[i]);
	cvReleaseImage(&out2);
	cvReleaseImage(&out2_c);
}

/**
 A opening B = (A erosion B) dilation B
*/
void opening(IplImage *in,IplImage *se,const int pi,const int pj,IplImage *out)
{
	assert(in!=NULL);
	assert(se!=NULL);
	assert(out!=NULL);
	IplImage *out_e = get_clone(in);
	erode(in,se,pi,pj,out_e);
	dilate(out_e,se,pi,pj,out);
	cvReleaseImage(&out_e);
}

/**
 A closing B = (A dilation B) erosion B
*/
void closing(IplImage *in,IplImage *se,const int pi,const int pj,IplImage *out)
{
	assert(in!=NULL);
	assert(se!=NULL);
	assert(out!=NULL);
	IplImage *out_d = get_clone(in);
	dilate(in,se,pi,pj,out_d);
	erode(out_d,se,pi,pj,out);
	cvReleaseImage(&out_d);
}

/**
Calculates A-B;
A-B = intersetcion(A,complement(B));
*/
void subtract(IplImage *A,IplImage *B,IplImage *out)
{
	IplImage *B_c  = get_clone(B);
	intersection_op(A,B_c,out);
	cvReleaseImage(&B_c);
}

/**
A dilation B- Aerosion B
*/
void morph_gradient(IplImage *in,IplImage *se,const int pi,const int pj,IplImage *out)
{
	assert(in!=NULL);
	assert(se!=NULL);
	assert(out!=NULL);
	IplImage *out_d = get_clone(in);
	IplImage *out_e = get_clone(in);
	dilate(in,se,pi,pj,out_d);
	erode(out,se,pi,pj,out_e);
	subtract(out_d,out_e,out);
	cvReleaseImage(&out_d);
	cvReleaseImage(&out_e);
}

/**
A - A erosion B
*/
void internal_gradient(IplImage *in,IplImage *se,const int pi,const int pj,IplImage *out)
{		
	assert(in!=NULL);
	assert(se!=NULL);
	assert(out!=NULL);
	IplImage *out_e = get_clone(in);
	erode(in,se,pi,pj,out_e);
	subtract(in,out_e,out);
	cvReleaseImage(&out_e);
}


/**
A dilation B - A
*/
void external_gradient(IplImage *in,IplImage *se,const int pi,const int pj,IplImage *out)
{
	assert(in!=NULL);
	assert(se!=NULL);
	assert(out!=NULL);
	IplImage *out_d = get_clone(in);
	dilate(in,se,pi,pj,out_d);
	subtract(out_d,in,out);
	cvReleaseImage(&out_d);
}

//A - A opening B
void white_top_hat_transform(IplImage *in,IplImage *se,const int pi,const int pj,IplImage *out)
{
	assert(in!=NULL);
	assert(se!=NULL);
	assert(out!=NULL);
	IplImage *out_o = get_clone(in);
	opening(in,se,pi,pj,out_o);
	subtract(in,out_o,out);
	cvReleaseImage(&out_o);
}
//A closing B - A
void black_top_hat_transform(IplImage *in,IplImage *se,const int pi,const int pj,IplImage *out)
{
	assert(in!=NULL);
	assert(se!=NULL);
	assert(out!=NULL);
	IplImage *out_c = get_clone(in);
	closing(in,se,pi,pj,out_c);
	subtract(out_c,in,out);
	cvReleaseImage(&out_c);
}

/*
//TO DO Functions
void convex_hull()
void thickining()
*/







//#define MORPH_TEST

