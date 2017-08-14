#include <iostream>
#include "IExecutor.h"
#include "HelloWorld.hpp"
#include "Lesson2.hpp"
#include "Lesson3.hpp"

int main(int argc, char *argv[]) {
	//IExecutor *executor = new HelloWorld();
	//int ret = executor->execute(argc, argv);

	IExecutor *executor = new Lesson3();
	int ret = executor->execute(argc, argv);

	system("pause");
	return ret;
}