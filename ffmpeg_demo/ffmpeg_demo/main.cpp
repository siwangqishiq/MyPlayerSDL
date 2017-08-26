#define  __STDC_CONSTANT_MACROS
#include <iostream>
#include "IExecute.h"
#include "HelloFFmpeg.h"
#include "SimplestDecoder.h"
#include "VideoExtractor.h"
#include "SimplePlayer.h"

int main(int argc , char *argv[]) {

	//IExecute *hello_ffmpeg = new HelloFFmpeg();
	//std::cout << "Hello World" << std::endl;
	//int ret = hello_ffmpeg->execute(argc , argv);
	//delete hello_ffmpeg;

	//IExecute *exe = new SimplestDecoder();

	//IExecute *exe = new VideoExtractor();

	IExecute *exe = new SimplePlayer();
	exe->execute(argc, argv);
	delete exe;

	system("pause");
	return 0;
}