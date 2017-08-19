#include "HelloFFmpeg.h"
#include <stdio.h>

extern "C" {
#include "libavcodec\avcodec.h"
}


HelloFFmpeg::HelloFFmpeg(){
	cout << "init Hello ffmpeg haha" << endl;
}

HelloFFmpeg::~HelloFFmpeg(){
}

int HelloFFmpeg::execute(int agrc, char * agrv[]){
	//cout << "Hello World!" << endl;
	const char *codec_config = avcodec_configuration();
	cout << codec_config << endl;
	return 0;
}
