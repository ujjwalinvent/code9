/**
program to test the functions of morph.hpp
*/
#include <gtest/gtest.h>
#include "src/morph.hpp"
#include "img_assert.hpp"
//all the test code goes here



TEST(ErosionTest,TestingOnSmallSet0)
{
	IplImage *img = cvCreateImage(cvSize(5,5),8,1);
	int in[5][5] = {
			{ 0,0,0,0,0},
			{ 1,1,1,1,1},
			{ 0,1,1,1,0},
			{ 0,0,0,0,0},
			{ 0,0,0,0,0}
			};
	BwImage _img(img);
	cvZero(img);
	int i,j;
	for(i=0;i<img->height;i++)
		for(j=0;j<img->width;j++)
			if(in[i][j]) _img[i][j]=255;
	IplImage *se = cvCreateImage(cvSize(3,3),8,1);
	BwImage _se(se);
	cvZero(se);
	int searr[3][3] = { 
			  { 0,0,0},
			  { 1,1,1},
			  { 0,0,0}
			    };	
	for(i=0;i<se->height;i++)
		for(j=0;j<se->width;j++)
			if(searr[i][j]) _se[i][j] = 255;

	
	IplImage *out = get_clone(img);
	erode(img,se,1,1,out);
	BwImage _out(out);
	int outarr[5][5] = {
			   {0,0  ,0  ,0  ,0},
			   {0,255,255,255,0},
			   {0,0  ,255,0  ,0},
			   {0,0  ,0  ,0  ,0},
			   {0,0  ,0  ,0  ,0}};

	for(i=0;i<out->height;i++)
		for(j=0;j<out->width;j++)
			ASSERT_EQ(outarr[i][j],_out[i][j]);
	cvReleaseImage(&img);
	cvReleaseImage(&se);
	cvReleaseImage(&out);
}


TEST(DilationTest,TestingOnSmallSet0)
{
	IplImage *img = cvCreateImage(cvSize(5,5),8,1);
	int in[5][5] = {
			{ 0,0,0,0,0},
			{ 0,0,1,1,0},
			{ 0,0,1,0,0},
			{ 0,0,0,0,0},
			{ 0,0,0,0,0}
			};
	BwImage _img(img);
	cvZero(img);
	int i,j;
	for(i=0;i<img->height;i++)
		for(j=0;j<img->width;j++)
			if(in[i][j]) _img[i][j]=255;
//	print_img(img);
	IplImage *se = cvCreateImage(cvSize(3,3),8,1);
	BwImage _se(se);
	cvZero(se);
	int searr[3][3] = { 
			  { 0,0,0},
			  { 1,0,1},
			  { 0,0,0}
			    };	
	for(i=0;i<se->height;i++)
		for(j=0;j<se->width;j++)
			if(searr[i][j]) _se[i][j] = 255;

//	print_img(se);
	IplImage *out = get_clone(img);
	dilate(img,se,1,1,out);
	BwImage _out(out);
	int outarr[5][5] = {
			   {0,0  ,0  ,0  ,0  },
			   {0,255,255,255,255},
			   {0,255,0  ,255,0  },
			   {0,0  ,0  ,0  ,0  },
			   {0,0  ,0  ,0  ,0  }
			   };
//	print_img(out);
	for(i=0;i<img->height;i++)
		for(j=0;j<img->width;j++)
			ASSERT_EQ(outarr[i][j],_out[i][j]);	
	cvReleaseImage(&img);
	cvReleaseImage(&se);
	cvReleaseImage(&out);
}


TEST(ComplementTest,TestingOnSmallSet0)
{
	IplImage *img = cvCreateImage(cvSize(5,5),8,1);
	int in[5][5] = {
			{ 0,0,0,0,0},
			{ 1,1,1,1,1},
			{ 1,1,1,1,1},
			{ 0,0,0,0,0},
			{ 0,0,0,0,0}
			};
	BwImage _img(img);
	cvZero(img);
	int i,j;
	for(i=0;i<img->height;i++)
		for(j=0;j<img->width;j++)
			if(in[i][j]) _img[i][j]=255;

	IplImage *out = get_clone(img);
	complement(img,out);
	BwImage _out(out);
	int outarr[5][5] = {
			   {255,255,255,255,255},
			   {0  ,0  ,0  ,0  ,0  },
			   {0  ,0  ,0  ,0  ,0  },
			   {255,255,255,255,255},
			   {255,255,255,255,255 }
			   };

	for(i=0;i<out->height;i++)
		for(j=0;j<out->width;j++)
			ASSERT_EQ(outarr[i][j],_out[i][j]);

	cvReleaseImage(&img);
	cvReleaseImage(&out);
}

TEST(HitOrMissTest,testingOnSmallSet0)
{
	IplImage *in = get_bwimg(13,8);
	int inarr[104] =  {
			    0,0,0,0,0,0,0,0,0,0,0,0,0,
			    0,0,0,1,0,0,1,0,0,1,0,0,0,
			    0,0,0,1,0,0,0,1,1,1,1,1,0,
			    0,0,0,1,0,1,0,0,0,1,0,0,0,
			    0,1,0,1,1,1,0,0,0,1,0,0,0,
			    0,0,0,0,0,0,0,0,0,1,1,1,0,
			    0,1,1,1,1,1,1,1,0,0,0,0,0,
			    0,0,0,0,0,0,0,0,0,0,0,0,0
			   };
	fill_img(in,inarr);
//	print_img(in);

	int searr1[9] = { -1, 0,-1,
			   0, 1, 0,
			  -1,-1,-1 };

	int searr2[9] = { -1, 0,-1,
			   0, 1,-1,
			  -1, 0,-1 };

	int searr3[9] = { -1,-1,-1,
			   0, 1, 0,
			  -1,-0,-1 };

	int searr4[9] = { -1, 0,-1,
			  -1, 1, 0,
			  -1, 0,-1 };
	IplImage *se[4];
	for(int i= 0; i < 4;i++)
		se[i] = get_bwimg(3,3);
	fill_img(se[0],searr1);
	fill_img(se[1],searr2);
	fill_img(se[2],searr3);
	fill_img(se[3],searr4);
	IplImage *out[4];
	for(int i=0;i<4;i++)
		out[i] = get_clone(in);
	hit_or_miss(in,se[0],1,1,out[0]);
	hit_or_miss(in,se[1],1,1,out[1]);
	hit_or_miss(in,se[2],1,1,out[2]);
	hit_or_miss(in,se[3],1,1,out[3]);
	int outarr[104] =  {
			    0,0,0,0,0,0,0,0,0,0,0,0,0,
			    0,0,0,1,0,0,1,0,0,1,0,0,0,
			    0,0,0,0,0,0,0,1,0,0,0,1,0,
			    0,0,0,0,0,1,0,0,0,0,0,0,0,
			    0,1,0,0,0,0,0,0,0,0,0,0,0,
			    0,0,0,0,0,0,0,0,0,0,0,1,0,
			    0,1,0,0,0,0,0,1,0,0,0,0,0,
			    0,0,0,0,0,0,0,0,0,0,0,0,0
			   };
	IplImage *union_out = get_clone(in);
	IplImage *union_out2 = get_clone(in);
	union_op(union_out,4,out[0],out[1],out[2],out[3]);
	fill_img(union_out2,outarr);
	BwImage _union_out(union_out);
	BwImage _union_out2(union_out2);
	for(int i=0;i<in->height;i++)
		for(int j=0;j<in->width;j++)
			ASSERT_EQ(_union_out2[i][j],_union_out[i][j]);
	cvReleaseImage(&in);
	for(int i=0;i<4;i++)
		cvReleaseImage(&out[i]);
	for(int i=0;i<4;i++)
		cvReleaseImage(&se[i]);
	cvReleaseImage(&union_out);
	cvReleaseImage(&union_out2);
}

TEST(HitOrMissTest,testingOnSmallSet1)
{
	IplImage *in = get_bwimg(10,10);
	int inarr[100] = {
			0,0,0,0,0,0,0,0,0,0,
			0,1,1,1,0,0,1,1,0,0,
			0,1,1,1,0,0,1,1,0,0,
			0,1,1,1,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,1,1,1,1,0,
			0,0,0,0,0,1,1,1,1,0,
			0,0,0,0,0,1,1,1,1,0,
			0,0,0,0,0,1,1,1,1,0,
			0,0,0,0,0,0,0,0,0,0
			};
	fill_img(in,inarr);
	IplImage *se = get_bwimg(5,5);
	int searr[25] = {
			0,0,0,0,0,
			0,1,1,1,0,
			0,1,1,1,0,
			0,1,1,1,0,
			0,0,0,0,0
			};
	fill_img(se,searr);
	IplImage *out = get_clone(in);
	hit_or_miss(in,se,2,2,out);
	
	int outarr[100] = {
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,1,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0
			};
	IplImage *out2 = get_clone(in);
	fill_img(out2,outarr);
	BwImage _out2(out2);
	BwImage _out(out);
	int i,j;
	for(i=0;i<out2->height;i++)
		for(j=0;j<out2->width;j++)
			ASSERT_EQ(_out2[i][j],_out[i][j]);
	cvReleaseImage(&in);
	cvReleaseImage(&out);
	cvReleaseImage(&out2);
	cvReleaseImage(&se);
}


TEST(HitOrMissTest2,testingOnSmallSet0)
{
	IplImage *in = get_bwimg(10,10);
	int inarr[100] = {
			0,0,0,0,0,0,0,0,0,0,
			0,1,1,1,0,0,1,1,0,0,
			0,1,1,1,0,0,1,1,0,0,
			0,1,1,1,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,1,1,1,1,0,
			0,0,0,0,0,1,1,1,1,0,
			0,0,0,0,0,1,1,1,1,0,
			0,0,0,0,0,1,1,1,1,0,
			0,0,0,0,0,0,0,0,0,0
			};
	fill_img(in,inarr);
	IplImage *se1 = get_bwimg(5,5);
	int searr1[25] = {
			0,0,0,0,0,
			0,1,1,1,0,
			0,1,1,1,0,
			0,1,1,1,0,
			0,0,0,0,0
			};
	fill_img(se1,searr1);
	IplImage *out = get_clone(in);
	IplImage *se2 = get_clone(se1);
	complement(se1,se2);
	hit_or_miss2(in,se1,se2,2,2,out);
	
	int outarr[100] = {
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,1,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0
			};
	IplImage *out2 = get_clone(in);
	fill_img(out2,outarr);
	ASSERT_EQ_IMAGE(out2,out);
	
	cvReleaseImage(&in);
	cvReleaseImage(&out);
	cvReleaseImage(&out2);
	cvReleaseImage(&se1);
	cvReleaseImage(&se2);
}


TEST(HitOrMissTest,testingForThinningOperation)
{
	IplImage *in = get_bwimg(13,7);
	int inarr[91] = {
			0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,1,1,1,1,1,1,1,1,1,1,1,0,
			0,1,1,1,1,1,1,1,1,1,0,0,0,
			0,1,1,1,1,1,1,1,1,1,0,0,0,
			0,1,1,1,1,1,1,1,1,1,0,0,0,
			0,1,1,1,0,0,1,1,1,1,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0
			};
	fill_img(in,inarr);
	IplImage *se = get_bwimg(3,3);
	int searr1[9] = {
			 0, 0, 0,
			-1, 1,-1,
			 1, 1, 1
			 };
	fill_img(se,searr1);
	int outarr[91] = {
			0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,1,0,0,0,0,0,0,0,1,1,1,0,
			0,1,1,1,1,1,1,1,1,1,0,0,0,
			0,1,1,1,1,1,1,1,1,1,0,0,0,
			0,1,1,1,1,1,1,1,1,1,0,0,0,
			0,1,1,1,0,0,1,1,1,1,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0
			};
	IplImage *out          = get_clone(in);
	IplImage *out_c        = get_clone(in);
	IplImage *expected_out = get_clone(in);
	IplImage *actual_out   = get_clone(in);
	fill_img(expected_out,outarr);
	hit_or_miss(in,se,1,1,out);
//	print_img(out);
	complement(out,out_c);
	intersection_op(in,out_c,actual_out);
	//do hit or miss then use A intersection complement(A hmt B)
//	print_img(actual_out);
	int i,j;
	BwImage _expected_out(expected_out);
	BwImage _actual_out(actual_out);
	for(i=0;i<actual_out->height;i++)
		for(j=0;j<actual_out->width;j++)
			ASSERT_EQ(_expected_out[i][j],_actual_out[i][j]);
	cvReleaseImage(&in);
	cvReleaseImage(&actual_out);
	cvReleaseImage(&expected_out);
	cvReleaseImage(&out);
	cvReleaseImage(&out_c);
	cvReleaseImage(&se);
	
}



TEST(HitOrMissWTest,testingOnSmallSet0)
{	
	IplImage *in = get_bwimg(10,10);
	int inarr[100] = {
			0,0,0,0,0,0,0,0,0,0,
			0,1,1,1,0,0,0,0,0,0,
			0,1,1,1,0,0,0,0,0,0,
			0,1,1,1,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,1,1,1,1,0,
			0,0,0,0,0,1,1,1,1,0,
			0,0,0,0,0,1,1,1,1,0,
			0,0,0,0,0,1,1,1,1,0,
			0,0,0,0,0,0,0,0,0,0
			};
	fill_img(in,inarr);
	IplImage *se = get_bwimg(5,5);
	int searr[25] = {
			0,0,0,0,0,
			0,0,0,0,0,
			0,0,1,0,0,
			0,0,0,0,0,
			0,0,0,0,0
			};
	fill_img(se,searr);
	IplImage *out = get_clone(in);
	hit_or_miss_w(in,se,2,2,1,out);
	//print_img(out);	
	int outarr[100] = {
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,1,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0
			};
	IplImage *out2 = get_clone(in);
	fill_img(out2,outarr);
	BwImage _out2(out2);
	BwImage _out(out);
	int i,j;
	for(i=0;i<out2->height;i++)
		for(j=0;j<out2->width;j++)
			ASSERT_EQ(_out2[i][j],_out[i][j]);
	cvReleaseImage(&in);
	cvReleaseImage(&out);
	cvReleaseImage(&out2);
	cvReleaseImage(&se);
}


TEST(HitOrMissWTest,testingOnSmallSet1)
{	
	IplImage *in = get_bwimg(15,15);
	int inarr[225] = {
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
			0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
			0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
			0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
			0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,
			0,0,0,1,1,1,1,1,1,1,0,0,0,0,0
			};
	fill_img(in,inarr);
	IplImage *se = get_bwimg(7,7);
	int searr[49] = {
		      0,0,0,0,0,0,0,
		      0,0,0,0,0,0,0,
		      0,0,0,1,0,0,0,
		      0,0,0,1,0,0,0,
		      0,0,0,1,0,0,0,
		      1,1,1,1,1,1,1
			};
	fill_img(se,searr);
	IplImage *out = get_clone(in);
	hit_or_miss_w(in,se,3,3,1,out);
	//print_img(out);	
	int outarr[225] = {
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
			};
	IplImage *out2 = get_clone(in);
	fill_img(out2,outarr);
	BwImage _out2(out2);
	BwImage _out(out);
	int i,j;
	
	for(i=0;i<out2->height;i++)
		for(j=0;j<out2->width;j++)
			ASSERT_EQ(_out2[i][j],_out[i][j]);
	
	cvReleaseImage(&in);
	cvReleaseImage(&out);
	cvReleaseImage(&out2);
	cvReleaseImage(&se);
}

TEST(ThinningTest,testingOnSmallSet0)
{

	IplImage *in = get_bwimg(13,7);
	int inarr[91] = {
			0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,1,1,1,1,1,1,1,1,1,1,1,0,
			0,1,1,1,1,1,1,1,1,1,0,0,0,
			0,1,1,1,1,1,1,1,1,1,0,0,0,
			0,1,1,1,1,1,1,1,1,1,0,0,0,
			0,1,1,1,0,0,1,1,1,1,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0
			};
	fill_img(in,inarr);
	int outarr[91] = {
			0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,1,0,0,0,0,0,0,0,1,1,1,0,
			0,1,1,0,0,0,0,0,1,1,0,0,0,
			0,0,1,1,1,1,1,1,1,0,0,0,0,
			0,1,1,0,0,0,0,0,1,0,0,0,0,
			0,1,0,0,0,0,0,0,1,1,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0
			};
	IplImage *expected_out = get_clone(in);
	IplImage *actual_out = get_clone(in);
	fill_img(expected_out,outarr);
	thinning_op(in,actual_out);
//	print_img(actual_out);
	BwImage _expected_out(expected_out);
	BwImage _actual_out(actual_out);
	int i,j;
	for(i=0;i<in->height;i++)
		for(j=0;j<in->width;j++)
			ASSERT_EQ(_expected_out[i][j],_actual_out[i][j]);
	cvReleaseImage(&actual_out);
	cvReleaseImage(&expected_out);
	cvReleaseImage(&in);
}

TEST(ErosionDilationTest,testingForWorstTimeComplexity)
{
	IplImage *in = get_bwimg(640,480);
	BwImage _in(in);
	int i,j;
	for(i=0;i<in->height;i++)
		for(j=0;j<in->width;j++)
			_in[i][j] = 255;

	IplImage *se = get_bwimg(3,3);
	int searr[9] = {
			1,1,1,
			1,1,1,
			1,1,1
			};
	fill_img(se,searr);
	IplImage *e_out = get_bwimg(640,480);
	IplImage *d_out = get_bwimg(640,480);
	int stress = 30;
	
	DECLARE_TIMING(e_timer);
	START_TIMING(e_timer);
	for(i = 0 ; i < stress; i++)
		erode(in,se,1,1,e_out);
	STOP_TIMING(e_timer);
	cout<<"Erosion Time Test ["<<stress<<"] Frames :"<<GET_TIMING(e_timer)<<" ms"<<endl;
	
	DECLARE_TIMING(d_timer);
	START_TIMING(d_timer);
	for(i=0;i<stress;i++)
		dilate(in,se,1,1,d_out);
	STOP_TIMING(d_timer);
	cout<<"Dilation Time Test ["<<stress<<"] frames :"<<GET_TIMING(d_timer)<<" ms"<<endl;


	IplImage *exp_e_out = get_bwimg(640,480);
	IplImage *exp_d_out = get_bwimg(640,480);
	
	BwImage _exp_e_out(exp_e_out);
	BwImage _exp_d_out(exp_d_out);
	copy_bwimg(in,exp_e_out);
	copy_bwimg(in,exp_d_out);

	for(i=0;i<e_out->height;i++)
		for(j=0;j<e_out->width;j++)
			if(i==0||i==479||j==0||j==639)
				_exp_e_out[i][j] = 0;

	BwImage _act_e_out(e_out);
	BwImage _act_d_out(d_out);

	for(i=0;i<in->height;i++)
		for(j=0;j<in->width;j++)
			ASSERT_EQ(_exp_e_out[i][j],_act_e_out[i][j]);
	
	
	for(i=0;i<in->height;i++)
		for(j=0;j<in->width;j++)
			ASSERT_EQ(_exp_d_out[i][j],_act_d_out[i][j]);
	
	cvReleaseImage(&in);
	cvReleaseImage(&se);
	cvReleaseImage(&e_out);
	cvReleaseImage(&d_out);
	cvReleaseImage(&exp_e_out);
	cvReleaseImage(&exp_d_out);
}

TEST(ErosionDilationTest,testingForBestTimeComplexity)
{
	IplImage *in = get_bwimg(640,480);
	BwImage _in(in);
	int i,j;
	for(i=0;i<in->height;i++)
		for(j=0;j<in->width;j++)
			_in[i][j] = 0;

	IplImage *se = get_bwimg(3,3);
	int searr[9] = {
			1,1,1,
			1,1,1,
			1,1,1
			};
	fill_img(se,searr);
	IplImage *e_out = get_bwimg(640,480);
	IplImage *d_out = get_bwimg(640,480);
	int stress = 30;
	
	DECLARE_TIMING(e_timer);
	START_TIMING(e_timer);
	for(i = 0 ; i < stress; i++)
		erode(in,se,1,1,e_out);
	STOP_TIMING(e_timer);
	cout<<"Erosion Time Test ["<<stress<<"] Frames :"<<GET_TIMING(e_timer)<<" ms"<<endl;
	
	DECLARE_TIMING(d_timer);
	START_TIMING(d_timer);
	for(i=0;i<stress;i++)
		dilate(in,se,1,1,d_out);
	STOP_TIMING(d_timer);
	cout<<"Dilation Time Test ["<<stress<<"] frames :"<<GET_TIMING(d_timer)<<" ms"<<endl;


	IplImage *exp_e_out = get_bwimg(640,480);
	IplImage *exp_d_out = get_bwimg(640,480);
	
	BwImage _exp_e_out(exp_e_out);
	BwImage _exp_d_out(exp_d_out);
	copy_bwimg(in,exp_e_out);
	copy_bwimg(in,exp_d_out);


	BwImage _act_e_out(e_out);
	BwImage _act_d_out(d_out);

	for(i=0;i<in->height;i++)
		for(j=0;j<in->width;j++)
			ASSERT_EQ(_exp_e_out[i][j],_act_e_out[i][j]);
	
	
	for(i=0;i<in->height;i++)
		for(j=0;j<in->width;j++)
			ASSERT_EQ(_exp_d_out[i][j],_act_d_out[i][j]);
	
	cvReleaseImage(&in);
	cvReleaseImage(&se);
	cvReleaseImage(&e_out);
	cvReleaseImage(&d_out);
	cvReleaseImage(&exp_e_out);
	cvReleaseImage(&exp_d_out);
}

TEST(HitOrMissTest,testingForTimeComplexity)
{
	IplImage *in = get_bwimg(640,480);
	cvSet(in,cvScalar(255));
	IplImage *se = get_bwimg(3,3);
	int searr[9] = {
			1,1,1,
			1,1,1,
			1,1,1
			};
	fill_img(se,searr);
	IplImage *out = get_clone(in);
	int stress = 30;

	DECLARE_TIMING(h_timer);
	START_TIMING(h_timer);
	for(int i=0;i<stress;i++)
		hit_or_miss(in,se,1,1,out);
	STOP_TIMING(h_timer);
	cout<<"Hit or Miss Time Test ["<<stress<<"] frames : "<<GET_TIMING(h_timer)<<endl;
//	print_img(in);
//	print_img(out);
	IplImage *exp_out = get_clone(in);
	cvSet(exp_out,cvScalar(0));
	ASSERT_EQ_IMAGE(exp_out,out);
	cvReleaseImage(&se);
	cvReleaseImage(&in);
	cvReleaseImage(&out);
	cvReleaseImage(&exp_out);
}

int main(int argc,char **argv)
{
	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}
//#else
/*
int main(int argc,char **argv)
{
	IplImage *in = cvLoadImage(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
	int i,j;
	IplImage *in2 = get_bwimg(in->width/4,in->height/4);
	cvResize(in,in2,CV_INTER_LINEAR);
	BwImage _in2(in2);
	for(i = 0; i < in2->height;i++)
	{
		for(j=0;j<in2->width;j++)
		{
			if(_in2[i][j]>100)
				_in2[i][j] = 255;
			else 
				_in2[i][j] = 0;
		}
	}
	IplImage *out = get_clone(in2);
	thinning_op(in2,out);
	string filename = argv[1];
	string filename_in,filename_out;
	filename_in = "in_"+filename;
	filename_out = "out_"+filename;
	cout<<filename_in<<": saved"<<endl;
	cout<<filename_out<<": saved"<<endl;
	cvSaveImage(filename_in.c_str(),in2);
	cvSaveImage(filename_out.c_str(),out);
	cvReleaseImage(&in);
	cvReleaseImage(&out);
	return 0;
}*/
