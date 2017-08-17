#ifndef _HelloFFmpeg_h_
#define _HelloFFmpeg_h_

#include <iostream>
#include "IExecute.h"

using namespace std;

class HelloFFmpeg : public IExecute
{
public:
	HelloFFmpeg();
	~HelloFFmpeg();

	int execute(int agrc, char *agrv[]);
};

#endif // !_HelloFFmpeg_h_




