#ifndef _Lesson_4_Main_Loop_
#define _Lesson_4_Main_Loop_

#include "SDL.h"
#include "SDL_image.h"
#include <stdexcept>
#include <string>
#include <iostream>
#include "IExecutor.h"

using namespace std;

class Lesson4MainLoop : public IExecutor {
private:
	SDL_Window *window = nullptr;
	SDL_Renderer *render = nullptr;

protected:
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	SDL_Texture *loadImage(string file) {
		SDL_Texture *texture = nullptr;

		texture = IMG_LoadTexture(render, file.c_str());

		if (texture == nullptr) {
			throw runtime_error("Failed to load image: " + file + IMG_GetError());
		}
		return texture;
	}

	void applySurface(int x, int y, SDL_Texture *texture, SDL_Renderer *ren) {
		SDL_Rect pos;
		pos.x = x;
		pos.y = y;

		SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
		SDL_RenderCopy(ren, texture, NULL, &pos);
	}

public:
	int execute(int argc, char *argv[]) {

		if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
			std::cout << SDL_GetError() << std::endl;
			return 1;
		}

		window = SDL_CreateWindow("Lesson 2", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == nullptr) {
			std::cout << SDL_GetError() << std::endl;
			return 2;
		}

		render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED
			| SDL_RENDERER_PRESENTVSYNC);
		if (render == nullptr) {
			std::cout << SDL_GetError() << std::endl;
			return 3;
		}

		SDL_Texture	*image = nullptr;
		try {
			image = loadImage("image2.png");
		}catch (runtime_error &e) {
			cout << e.what() << std::endl;
			return 4;
		}

		SDL_Event e;//ÊÂ¼þ
		bool quit = false;
		while (!quit) {
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				if (e.type == SDL_KEYDOWN) {
					quit = true;
				}
				if (e.type == SDL_MOUSEBUTTONDOWN) {
					quit = true;
				}
			}

			SDL_RenderClear(render);
			int iW, iH;
			SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
			int x = SCREEN_WIDTH / 2 - iW / 2;
			int y = SCREEN_HEIGHT / 2 - iH / 2;
			applySurface(x, y, image, render);
			SDL_RenderPresent(render);
		}//end while

		SDL_DestroyTexture(image);
		SDL_DestroyRenderer(render);
		SDL_DestroyWindow(window);

		SDL_Quit();
		IMG_Quit();

		return 0;
	}
};

#endif // !_Lesson_4_Main_Loop_
