#include "SimplePlayer.h"
#include <iostream>
#include <stdio.h>

using namespace std;

//#define PLAY_FILE_NAME "qihun.yuv"   //640x480
//#define PIXEL_W 640
//#define PIXEL_H  480

#define PLAY_FILE_NAME "test_yuv420p_320x180.yuv"   //640x480
#define PIXEL_W 320
#define PIXEL_H  180

//test_yuv420p_320x180.yuv

bool isPlay = false;
bool quit = false;

#define EVENT_NEXT_FRAME SDL_USEREVENT + 1

int playThread(void *ptr) {
	SDL_Event event;

	cout << "play thread " << endl;
	while (isPlay) {
		event.type = EVENT_NEXT_FRAME;
		SDL_PushEvent(&event);
		SDL_Delay(25);
		//cout << "play thread send message " << endl;
	}//end while
	cout << "play thread end" << endl;
	event.type = SDL_QUIT;
	SDL_PushEvent(&event);
	return 0;
}

void onExit() {
	isPlay = false;
	quit = true;
}

//renderer one frame 
void reanderFrame(long frameCount) {
	cout << "frame = " << frameCount << endl;
}

int SimplePlayer::execute(int agrc, char * agrv[]){
	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
		cout << SDL_GetError() << endl;
		return 1;
	}

	window = SDL_CreateWindow("Play YUV420", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		std::cout << SDL_GetError() << std::endl;
		return 2;
	}

	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (render == nullptr) {
		std::cout << SDL_GetError() << std::endl;
		return 3;
	}

	FILE *fp = fopen(PLAY_FILE_NAME , "rb");
	if (!fp) {
		cout << "file open failed!" << endl;
		return -1;
	}

	Uint32 pixformat = 0;
	pixformat = SDL_PIXELFORMAT_IYUV;

	SDL_Texture* sdlTexture = SDL_CreateTexture(render, pixformat, SDL_TEXTUREACCESS_STREAMING , PIXEL_W, PIXEL_H);

	SDL_Thread *thread = SDL_CreateThread(playThread, "play thread", (void *)NULL);
	isPlay = true;

	SDL_Event e;//ÊÂ¼þ
	long frameCnt = 0;
	unsigned char frame_buffer[PIXEL_W * PIXEL_H *16 / 8];
	int result = -1;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			switch (e.type)
			{
			case SDL_QUIT:
				onExit();
				break;
			case EVENT_NEXT_FRAME://render next frame graph
				if (feof(fp)) {
					fclose(fp);
					onExit();
					return 0;
				}
				fread(frame_buffer, 1, PIXEL_W * PIXEL_H * 12 / 8, fp);
				result = SDL_UpdateTexture(sdlTexture, nullptr, frame_buffer, PIXEL_W);
				cout << "result = " << result << endl;

				SDL_RenderClear(render);
				SDL_RenderCopy(render, sdlTexture, nullptr, nullptr);
				frameCnt++;
				SDL_RenderPresent(render);
				break;
			default:
				break;
			}//end switch
		}//end while
	
	}//end while

	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
