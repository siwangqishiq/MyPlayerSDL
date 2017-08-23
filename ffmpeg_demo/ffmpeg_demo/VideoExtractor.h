#pragma once
#include "IExecute.h"
class VideoExtractor : public IExecute{
public:
	int execute(int agrc, char *agrv[]);
};

