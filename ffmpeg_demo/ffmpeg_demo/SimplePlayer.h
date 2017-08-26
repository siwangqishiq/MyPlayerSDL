#pragma once
#include "SDL.h"
#include "IExecute.h"

class SimplePlayer : public IExecute
{
public:
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	SDL_Window *window = nullptr;
	SDL_Renderer *render = nullptr;

	// Í¨¹ý IExecute ¼Ì³Ð
	int execute(int agrc, char * agrv[]) override;
};

