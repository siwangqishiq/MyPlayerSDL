#include <iostream>
#include "IExecutor.h"
#include "HelloWorld.hpp"

int main(int argc, char *argv[]) {
	IExecutor *executor = new HelloWorld();
	int ret = executor->execute(argc, argv);
	system("pause");
	return ret;
}