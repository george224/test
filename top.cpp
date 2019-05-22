#include "top.h"

void func(ap_uint<16> *frame_buf)
{
#pragma HLS INTERFACE m_axi depth=2073600 port=frame_buf offset=direct
	ap_uint<16> i,j;
//	ap_uint<16> temp[1920];
//#pragma HLS RESOURCE variable=temp core=RAM_2P_BRAM

	for(i=0;i<1080;i++)
	{
#pragma HLS LOOP_TRIPCOUNT min=0 max=1080 avg=1080
		for(j=0;j<1920;j++)
		{
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=0 max=1920 avg=1920
			frame_buf[i] = 0x55AA;
		}
	}
}
void func_read(AXI_STREAM& m_axis_video, int13 hsize_in, int13 vsize_in,float angle,
		ap_int<16> *frame_buf)
{
#pragma HLS INTERFACE m_axi depth=2073600 port=frame_buf offset=slave
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS INTERFACE s_axilite port=angle offset=0x24 bundle=CONTROL_BUS
#pragma HLS INTERFACE s_axilite port=vsize_in offset=0x18 bundle=CONTROL_BUS
#pragma HLS INTERFACE s_axilite port=hsize_in offset=0x10 bundle=CONTROL_BUS
//#pragma HLS INTERFACE axis register port=s_axis_video bundle=INPUT_STREAM
#pragma HLS INTERFACE axis register port=m_axis_video bundle=OUTPUT_STREAM

//	ap_axiu<24, 1, 1, 1> video;
	ap_axiu<16, 1, 1, 1> video_out;

	int13 oldWidth = hsize_in;
	int13 oldHeight = vsize_in;
	int13 oldCenterRow = (oldHeight - 1) / 2;
	int13 oldCenterCol = (oldWidth - 1) / 2;

	int13 newWidth = hsize_in;
	int13 newHeight = vsize_in;
	int13 newCenterRow = (newHeight - 1) / 2;
	int13 newCenterCol = (newWidth - 1) / 2;

	int14 paddRow = -newCenterRow * hls::cosf(angle) + newCenterCol * hls::sinf(angle) + oldCenterRow;
	int14 paddCol = -newCenterRow * hls::cosf(angle) - newCenterCol * hls::sinf(angle) + oldCenterCol;

    int index=0;

    for (int13 row = 0; row < newHeight; row++)
    {
#pragma HLS LOOP_TRIPCOUNT min=0 max=2048 avg=1080
        for (int13 col = 0; col < newWidth; col++)
        {
#pragma HLS PIPELINE II=1
#pragma HLS loop_flatten off
#pragma HLS LOOP_TRIPCOUNT min=0 max=2048 avg=1920
            //先旋轉再偏移
        	int14 oldRow = row * hls::cos(angle) - col * hls::sinf(angle) + 0.5 + paddRow;
        	int14 oldCol = row * hls::cos(angle) + col * hls::sinf(angle) + 0.5 + paddCol;
            index = oldCol+oldRow*hsize_in;
            //防出界
            if (oldRow >= 0 && oldRow < oldHeight &&
                oldCol >= 0 && oldCol < oldWidth)
            {
//                output.at<cv::Vec3b>(row, col) = src.at<cv::Vec3b>(oldRow, oldCol);
            	video_out.data= frame_buf[index];
            }
            else
            {
//                output.at<cv::Vec3b>(row, col) = cv::Vec3b(0, 0, 0);
				video_out.data = 0;
            }

			video_out.keep = 15;
			if((row==0)&&(col==0))
			{
				video_out.user = 1;
				video_out.last = 0;
			}
			else if(col==hsize_in-1)
			{
				video_out.user = 0;
				video_out.last = 1;
			}
			else
			{
				video_out.user = 0;
				video_out.last = 0;
			}
			m_axis_video << video_out;
        }
    }
}
void func_all(AXI_STREAM& s_axis_video,AXI_STREAM& m_axis_video, int13 hsize_in, int13 vsize_in,
		float angle,ap_int<32> *frame_buf)
{
#pragma HLS INTERFACE m_axi depth=67108864 port=frame_buf offset=slave
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS INTERFACE s_axilite port=angle offset=0x24 bundle=CONTROL_BUS
#pragma HLS INTERFACE s_axilite port=vsize_in offset=0x18 bundle=CONTROL_BUS
#pragma HLS INTERFACE s_axilite port=hsize_in offset=0x10 bundle=CONTROL_BUS
#pragma HLS INTERFACE axis register port=s_axis_video bundle=INPUT_STREAM
#pragma HLS INTERFACE axis register port=m_axis_video bundle=OUTPUT_STREAM

	ap_axiu<24, 1, 1, 1> video;
	ap_axiu<24, 1, 1, 1> video_out;

	int13 oldWidth = hsize_in;
	int13 oldHeight = vsize_in;
	int13 oldCenterRow = (oldHeight - 1) / 2;
	int13 oldCenterCol = (oldWidth - 1) / 2;

	int13 newWidth = hsize_in;
	int13 newHeight = vsize_in;
	int13 newCenterRow = (newHeight - 1) / 2;
	int13 newCenterCol = (newWidth - 1) / 2;

	int14 paddRow = -newCenterRow * hls::cosf(angle) + newCenterCol * hls::sinf(angle) + oldCenterRow;
	int14 paddCol = -newCenterRow * hls::cosf(angle) - newCenterCol * hls::sinf(angle) + oldCenterCol;

    int index=0;

    for (int13 row = 0; row < newHeight; row++)
    {
#pragma HLS LOOP_TRIPCOUNT min=0 max=2048 avg=1080
        for (int13 col = 0; col < newWidth; col++)
        {
#pragma HLS PIPELINE II=1 rewind
#pragma HLS loop_flatten off
#pragma HLS LOOP_TRIPCOUNT min=0 max=2048 avg=1920
        	s_axis_video >> video;
        	index = col+row*hsize_in;
        	frame_buf[index] = video.data;
        }
    }

    for (int13 row = 0; row < newHeight; row++)
    {
#pragma HLS LOOP_TRIPCOUNT min=0 max=2048 avg=1080
        for (int13 col = 0; col < newWidth; col++)
        {
#pragma HLS PIPELINE II=1
#pragma HLS loop_flatten off
#pragma HLS LOOP_TRIPCOUNT min=0 max=2048 avg=1920
            //先旋轉再偏移
        	int14 oldRow = row * hls::cos(angle) - col * hls::sinf(angle) + 0.5 + paddRow;
        	int14 oldCol = row * hls::cos(angle) + col * hls::sinf(angle) + 0.5 + paddCol;
            index = oldCol+oldRow*hsize_in;
            //防出界
            if (oldRow >= 0 && oldRow < oldHeight &&
                oldCol >= 0 && oldCol < oldWidth)
            {
//                output.at<cv::Vec3b>(row, col) = src.at<cv::Vec3b>(oldRow, oldCol);
            	video_out.data= frame_buf[index];
            }
            else
            {
//                output.at<cv::Vec3b>(row, col) = cv::Vec3b(0, 0, 0);
				video_out.data = 0;
            }

			video_out.keep = 15;
			if((row==0)&&(col==0))
			{
				video_out.user = 1;
				video_out.last = 0;
			}
			else if(col==hsize_in-1)
			{
				video_out.user = 0;
				video_out.last = 1;
			}
			else
			{
				video_out.user = 0;
				video_out.last = 0;
			}
			m_axis_video << video_out;
        }
    }
}

