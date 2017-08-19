#include "SimplestDecoder.h"
#include <iostream>
using namespace std;
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
//#include "libavutil/pixfmt.h"
}

int SimplestDecoder::execute(int argc, char * argv[]){
	AVFormatContext *pFormatCtx;
	int i, videoIndex;
	AVCodecContext *pCodecCtx;//解码器Context
	AVCodec *pCodec;
	AVFrame *pFrame;
	AVFrame *pFrameYUV;
	uint8_t *out_buffer;
	AVPacket *packet;
	int y_size;
	int ret, got_picture;
	struct SwsContext *img_convert_ctx;
	char filepath[] = "D:\\迅雷下载\\可疑的美容院HD1280高清中字.MP4";

	int frame_cnt;

	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();

	if (avformat_open_input(&pFormatCtx, filepath, NULL, NULL) != 0) {//打开视频文件
		printf("Couldn't open input stream.\n");
		return -1;
	}

	if (avformat_find_stream_info(pFormatCtx, NULL)<0) {//获取视频流描述信息
		printf("Couldn't find stream information.\n");
		return -2;
	}

	videoIndex = -1;
	for (int i = 0; i < pFormatCtx->nb_streams; i++) {//遍历文件中的所有流 找到视频流
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoIndex = i;
			break;
		}//end if
	}//end for i
	if (videoIndex == -1) {//未在文件中发现视频流
		printf("Didn't find a video stream.\n");
		return -3;
	}

	pCodecCtx = pFormatCtx->streams[videoIndex]->codec;//获取到视频流的编码器
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL) {
		printf("Codec not found.\n");
		return -4;
	}

	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {//打开视频编码器
		printf("Could not open codec.\n");
		return -5;
	}

	//输出视频信息
	int dur = pFormatCtx->duration / 1000000;
	cout << "视频时长 : "<<pFormatCtx->duration << endl;
	cout << "视频时长(s) : " << dur << endl;
	cout << "视频宽高 : " << pCodecCtx->width << " x " << pCodecCtx->height << endl;

	pFrame = av_frame_alloc();
	pFrameYUV = av_frame_alloc();

	out_buffer = (uint8_t *)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));

	avpicture_fill((AVPicture *)pFrameYUV, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
	packet = (AVPacket *)av_malloc(sizeof(AVPacket));

	//Output Info-----------------------------
	printf("--------------- File Information ----------------\n");
	av_dump_format(pFormatCtx, 0, filepath, 0);
	printf("-------------------------------------------------\n");
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
		pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

	frame_cnt = 0;

	while (av_read_frame(pFormatCtx, packet) >= 0) {
		if (packet->stream_index == videoIndex) {
			ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
			if (ret < 0) {
				printf("Decode Error.\n");
				return -1;
			}
			if (got_picture) {
				sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
					pFrameYUV->data, pFrameYUV->linesize);
				printf("Decoded frame index: %d\n", frame_cnt);
				frame_cnt++;
			}
		}
		av_free_packet(packet);
	}
	sws_freeContext(img_convert_ctx);

	av_frame_free(&pFrameYUV);
	av_frame_free(&pFrame);

	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);//关闭视频文件

	return 0;
}
