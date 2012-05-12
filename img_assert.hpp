#pragma once
inline void ASSERT_EQ_IMAGE(IplImage *expected_img,IplImage *actual_img)
{	
	ASSERT_TRUE(expected_img!=NULL);
	ASSERT_TRUE(actual_img!=NULL);
	ASSERT_TRUE(expected_img->width==actual_img->width);
	ASSERT_TRUE(expected_img->height==actual_img->height);
	BwImage _expected_img(expected_img),_actual_img(actual_img); 
	for(int i = 0 ; i < expected_img->height ;i++)
		for(int j = 0 ; j < expected_img->width ; j++)
			ASSERT_EQ(_expected_img[i][j],_actual_img[i][j]);
}
