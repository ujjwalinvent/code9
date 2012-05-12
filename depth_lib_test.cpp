#include<gtest/gtest.h>
#include "src/depth_lib.hpp"
#include "img_assert.hpp"


TEST(make_structuring_element_test,testSet0)
{
	IplImage *se1 = get_bwimg(10,10);
	int searr1[100] = { 
					0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,1,1,0,0,0,0,
					0,0,0,0,1,1,0,0,0,0,
					0,0,0,0,1,1,0,0,0,0,
					0,0,0,0,1,1,0,0,0,0,
					0,0,0,0,1,1,0,0,0,0,
					1,1,1,1,1,1,1,1,1,1,
					1,1,1,1,1,1,1,1,1,1};
	fill_img(se1,searr1);
	IplImage *act_se1 = get_clone(se1);
	make_structuring_element_b1(act_se1,2,3,2);
	ASSERT_EQ_IMAGE(se1,act_se1);

	IplImage *se2 = get_bwimg(10,10);
	int searr2[100] = { 
					1,1,1,1,1,1,1,1,1,1,
					1,1,1,1,1,1,1,1,1,1,
					1,1,1,0,0,0,0,1,1,1,
					1,1,1,0,0,0,0,1,1,1,
					1,1,1,0,0,0,0,1,1,1,
					1,1,1,0,0,0,0,1,1,1,
					1,1,1,0,0,0,0,1,1,1,
					1,1,1,0,0,0,0,1,1,1,
					1,1,1,0,0,0,0,1,1,1,
					1,1,1,0,0,0,0,1,1,1};
	fill_img(se2,searr2);
	IplImage *act_se2 = get_clone(se2);
	make_structuring_element_b2(act_se2,3,2,0);
	//print_img(act_se1);
	//print_img(act_se2);
	ASSERT_EQ_IMAGE(se2,act_se2);
	cvReleaseImage(&se1);
	cvReleaseImage(&se2);
	cvReleaseImage(&act_se1);
	cvReleaseImage(&act_se2);
}


TEST(make_structuring_element_test,testSet1)
{
	IplImage *se1 = get_bwimg(11,11);
	int searr1[121] = { 
					0,0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,1,0,0,0,0,0,
					0,0,0,0,0,1,0,0,0,0,0,
					0,0,0,0,0,1,0,0,0,0,0,
					0,0,0,0,0,1,0,0,0,0,0,
					0,0,0,0,0,1,0,0,0,0,0,
					0,0,0,0,0,1,0,0,0,0,0,
					0,0,0,0,0,1,0,0,0,0,0,
					0,0,0,0,0,1,0,0,0,0,0};
	fill_img(se1,searr1);
	IplImage *act_se1 = get_clone(se1);
	make_structuring_element_b1(act_se1,1,3,0);
	ASSERT_EQ_IMAGE(se1,act_se1);

	IplImage *se2 = get_bwimg(11,11);
	int searr2[121] = { 
					1,1,1,1,1,1,1,1,1,1,1,
					1,0,0,0,0,0,0,0,0,0,1,
					1,0,0,0,0,0,0,0,0,0,1,
					1,0,0,0,0,0,0,0,0,0,1,
					1,0,0,0,0,0,0,0,0,0,1,
					1,0,0,0,0,0,0,0,0,0,1,
					1,0,0,0,0,0,0,0,0,0,1,
					1,0,0,0,0,0,0,0,0,0,1,
					1,0,0,0,0,0,0,0,0,0,1,
					0,0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,0};
	fill_img(se2,searr2);
	IplImage *act_se2 = get_clone(se2);
	make_structuring_element_b2(act_se2,1,1,2);
	//print_img(act_se1);
    //print_img(act_se2);
	ASSERT_EQ_IMAGE(se2,act_se2);
	cvReleaseImage(&se1);
	cvReleaseImage(&se2);
	cvReleaseImage(&act_se1);
	cvReleaseImage(&act_se2);
}


int main(int argc,char **argv)
{	
	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}
