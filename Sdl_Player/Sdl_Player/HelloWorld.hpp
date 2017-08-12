#ifndef			_HelloWorld_
#define		_HelloWorld_

#include "SDL.h"
#include <iostream>
#include "IExecutor.h"

using namespace std;

class HelloWorld :public IExecutor {

public:
	int execute(int argc, char *argv[]) {
		cout << "Hello World" << endl;

		if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
			cout << SDL_GetError() << endl;
			return -1;
		}

		SDL_Window *win = nullptr;
		win = SDL_CreateWindow("HelloWorld", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
		if (win == nullptr) {
			cout << SDL_GetError() << endl;
			return -1;
		}

		SDL_Renderer *ren = nullptr;
		ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (ren == nullptr) {
			cout << SDL_GetError() << endl;
			return -1;
		}

		SDL_Surface *bmp = nullptr;
		bmp = SDL_LoadBMP("hello.bmp");
		if (bmp == nullptr) {
			cout << SDL_GetError() << endl;
			return -1;
		}

		SDL_Texture *texture = nullptr;
		texture = SDL_CreateTextureFromSurface(ren, bmp);
		SDL_FreeSurface(bmp);

		SDL_RenderClear(ren);
		SDL_RenderCopy(ren, texture, NULL, NULL);
		SDL_RenderPresent(ren);

		SDL_Delay(3000);

		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);

		SDL_Quit();
		return 0;
	}
};

#endif //
