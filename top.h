#ifndef _TOP_H_
#define _TOP_H_

#include <hls_video.h>
#include <hls_math.h>
#include <ap_int.h>
#include <ap_fixed.h>
#include "hls_stream.h"

#define MAX_WIDTH 2048
#define MAX_HEIGHT 2048

typedef ap_int<13> int13;
typedef ap_int<14> int14;
typedef ap_fixed<14,2,AP_RND > float16;
typedef hls::stream<ap_axiu<24,1,1,1> >		AXI_STREAM;

void func_read(AXI_STREAM& m_axis_video, int13 hsize_in, int13 vsize_in,float angle,
		ap_int<16> *frame_buf);
void func(ap_uint<16> *frame_buf);
void func_all(AXI_STREAM& s_axis_video, AXI_STREAM& s_axis_video,int13 hsize_in, int13 vsize_in,
		float angle,ap_int<32> *frame_buf);

#endif
