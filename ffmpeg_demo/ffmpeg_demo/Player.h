#pragma once
#include "IExecute.h"
#define EVENT_REFRESH SDL_USEREVENT + 2

class Player : public IExecute{
public:
	// ͨ�� IExecute �̳�
	virtual int execute(int agrc, char * agrv[]) override;

	void onExit();

	bool isPlay = false;
	bool isQuit = false;
};

