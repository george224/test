#include "top.h"
#include <hls_opencv.h>

int main()
{
	int i,j;
	unsigned int pixel;
	// Load data in OpenCV image format
//	IplImage* src = cvLoadImage("lenna.jpg");//test_1080p.bmp
//
//	//Get input Image size
//	CvSize size_in;
//	size_in = cvGetSize(src);
//	float angle=45/180.0*CV_PI;
//	ap_int<32> *frame_buf = (ap_int<32> *)malloc(2048*2048*sizeof(ap_int<32>));
////	frame_buf = (ap_int<24> *)0x8000000;
//	memset(frame_buf,0,512*512*sizeof(ap_int<32>));
//	//Create Destination image
//	IplImage* dst = cvCreateImage(size_in, src->depth, src->nChannels);

	//Create the AXI4-Stream
//	AXI_STREAM src_axi, dst_axi;
//
//	// Convert OpenCV format to AXI4 Stream format
//	IplImage2AXIvideo(src, src_axi);

	// Call the function to be synthesized
//	func(src_axi, size_in.width,size_in.height,frame_buf);
	ap_uint<16> *frame_buf1 = (ap_uint<16> *)malloc(1920*1080*sizeof(ap_uint<16>));
	func(frame_buf1);

//	for(i=0;i<size_in.height;i++)
//	{
//		for(j=0;j<size_in.width;j++)
//		{
//			*(dst->imageData + i*dst->widthStep + j*dst->nChannels + 2) = (*(frame_buf+i*size_in.width + j))&0xff;
//			*(dst->imageData + i*dst->widthStep + j*dst->nChannels + 1) = (*(frame_buf+i*size_in.width + j))&0xff00>>8;
//			*(dst->imageData + i*dst->widthStep + j*dst->nChannels + 0) = (*(frame_buf+i*size_in.width + j))&0xff0000>>16;
////			pixel = *(frame_buf+i*size_in.width + j);
////			printf("frame_buf[%d]=0x%x\n",i*size_in.width + j,pixel);
//		}
//	}
	// Convert the AXI4 Stream data to OpenCV format
//	AXIvideo2IplImage(dst_axi, dst);

	// Standard OpenCV image functions
//	cvSaveImage("out.png", dst);
//
//	cvReleaseImage(&src);
//	cvReleaseImage(&dst);

	return 0;
}
