#include "Player.h"
#include <iostream>

extern "C" {
#include "SDL.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}

using namespace std;

int playVideoThread(void *param) {
	Player *play = (Player *)param;

	SDL_Event event;
	cout << "play thread " << endl;
	while (play->isPlay) {
		event.type = EVENT_REFRESH;
		SDL_PushEvent(&event);
		SDL_Delay(45);
		//cout << "play thread send message " << endl;
	}//end while
	cout << "play thread end" << endl;
	event.type = SDL_QUIT;
	SDL_PushEvent(&event);
	return 0;
}

int Player::execute(int agrc, char * agrv[]){
	AVFormatContext *pFormatCtx;
	int videoIndex;
	AVCodecContext *pCodecCtx;//������Context
	AVCodec *pCodec;
	AVFrame *pFrame;
	AVFrame *pFrameYUV;
	uint8_t *out_buffer;
	AVPacket *packet;
	int ret, got_picture;
	struct SwsContext *img_convert_ctx;
	char filepath[] = "D:\\BaiduNetdiskDownload\\Сѧ�ڿγ���Ƶ - ����FFmpeg+SDL����Ƶ������������(1)\\����\\testvideo\\��˿��ʿ.mov";

	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();

	if (avformat_open_input(&pFormatCtx, filepath, NULL, NULL) != 0) {//����Ƶ�ļ�
		printf("Couldn't open input stream.\n");
		return -1;
	}

	if (avformat_find_stream_info(pFormatCtx, NULL)<0) {//��ȡ��Ƶ��������Ϣ
		printf("Couldn't find stream information.\n");
		return -1;
	}

	videoIndex = -1;
	for (int i = 0; i < pFormatCtx->nb_streams; i++) {//�����ļ��е������� �ҵ���Ƶ��
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoIndex = i;
			break;
		}//end if
	}//end for i
	if (videoIndex == -1) {//δ���ļ��з�����Ƶ��
		printf("Didn't find a video stream.\n");
		return -1;
	}

	pCodecCtx = pFormatCtx->streams[videoIndex]->codec;//��ȡ����Ƶ���ı�����
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL) {
		printf("Codec not found.\n");
		return -1;
	}

	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {//����Ƶ������
		printf("Could not open codec.\n");
		return -1;
	}
	cout << "��Ƶ��� : " << pCodecCtx->width << " x " << pCodecCtx->height << endl;

	SDL_Window *window = SDL_CreateWindow("Play YUV420", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, pCodecCtx->width, pCodecCtx->height, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		std::cout << SDL_GetError() << std::endl;
		return -2;
	}

	SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (render == nullptr) {
		std::cout << SDL_GetError() << std::endl;
		return 3;
	}

	Uint32 pixformat = 0;
	pixformat = SDL_PIXELFORMAT_IYUV;
	SDL_Texture* sdlTexture = SDL_CreateTexture(render, pixformat, SDL_TEXTUREACCESS_STREAMING, 
		pCodecCtx->width, pCodecCtx->height);

	pFrameYUV = av_frame_alloc();

	out_buffer = (uint8_t *)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));//malloc a memory buffer to YUV420 codec
	avpicture_fill((AVPicture *)pFrameYUV, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);

	packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
		pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

	SDL_Thread *thread = SDL_CreateThread(playVideoThread, "play video", (void *)this);
	this->isPlay = true;
	SDL_Event e;//�¼�
	long frameCnt = 0;
	//const int buff_size = pCodecCtx->width * pCodecCtx->height * 16 / 8;
	pFrame = av_frame_alloc();
	while (!isQuit) {
		while (SDL_PollEvent(&e) && !isQuit) {
			switch (e.type)
			{
			case SDL_QUIT:
				onExit();
				break;
			case EVENT_REFRESH://render next frame graph
				int readFrameRet;
				while ((readFrameRet = av_read_frame(pFormatCtx, packet)) >= 0) {//read video frame
					if (packet->stream_index == videoIndex) {
						break;
					}
				}//end while

				if (readFrameRet < 0) {//read the end of file
					onExit();
					break;
				}

				ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
				if (got_picture) {
					sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
						pFrameYUV->data, pFrameYUV->linesize);
					SDL_UpdateTexture(sdlTexture, NULL, pFrameYUV->data[0], pFrameYUV->linesize[0]);
					SDL_RenderClear(render);
					SDL_RenderCopy(render, sdlTexture, nullptr, nullptr);
					frameCnt++;
					cout << "frame = " << frameCnt << endl;
					SDL_RenderPresent(render);
				}

				av_packet_unref(packet);
				break;
			default:
				break;
			}//end switch
		}//end while
	}//end while

	av_frame_free(&pFrameYUV);
	av_frame_free(&pFrame);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);//�ر���Ƶ�ļ�
	return 0;
}

void Player::onExit(){
	isPlay = false;
	isQuit = true;
}
