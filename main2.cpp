#include "depth_lib.hpp"

void show_img()
{
    IplImage *rgb_img = get_rgbimg(640,480);
    IplImage *gray_img = get_bwimg(640,480);
    unsigned short *ushort_pixels;
    read_depth_file(0,&ushort_pixels,rgb_img,gray_img);
    disp_img(rgb_img,"rgb_img");
    disp_img(gray_img,"gray_img");
    cvWaitKey(0);
}

int main(int argc,char **argv)
{
//	show_img();
	read_depth_video_file("depth_video");
	return 0;
}

