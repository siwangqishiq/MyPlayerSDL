#define  __STDC_CONSTANT_MACROS
#include <iostream>
#include "IExecute.h"
#include "HelloFFmpeg.h"

int main(int argc , char *argv[]) {

	IExecute *hello_ffmpeg = new HelloFFmpeg();
	//std::cout << "Hello World" << std::endl;
	int ret = hello_ffmpeg->execute(argc , argv);
	delete hello_ffmpeg;
	system("pause");
	return ret;
}