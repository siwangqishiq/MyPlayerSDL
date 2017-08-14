#ifndef _Lesson_2_
#define _Lesson_2_

#include "SDL.h"
#include <iostream>
#include <string>
#include "IExecutor.h"

using namespace std;

class Lesson2 : public IExecutor {
private:
	SDL_Window *window = nullptr;
	SDL_Renderer *render = nullptr;

protected:
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	SDL_Texture *loadImage(string file) {
		SDL_Surface *loadImage = nullptr;
		SDL_Texture *texture = nullptr;

		loadImage = SDL_LoadBMP(file.c_str());

		if (loadImage != nullptr) {
			texture = SDL_CreateTextureFromSurface(render, loadImage);
			SDL_FreeSurface(loadImage);
		}else {
			cout << SDL_GetError() << endl;
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

		SDL_Texture    *background = nullptr;
		SDL_Texture	*image = nullptr;
		background = loadImage("background.bmp");
		image = loadImage("image.bmp");

		if (background == nullptr || image == nullptr)
			return 4;

		SDL_RenderClear(render);
		int bW, bH;
		SDL_QueryTexture(background, NULL, NULL, &bW, &bH);
		applySurface(0, 0, background, render);
		applySurface(bW, 0, background, render);
		applySurface(0, bH, background, render);
		applySurface(bW, bH, background, render);

		int iW, iH;
		SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
		int x = SCREEN_WIDTH / 2 - iW / 2;
		int y = SCREEN_HEIGHT / 2 - iH / 2;
		applySurface(x, y, image, render);

		SDL_RenderPresent(render);
		SDL_Delay(2000);

		SDL_DestroyTexture(background);
		SDL_DestroyTexture(image);
		SDL_DestroyRenderer(render);
		SDL_DestroyWindow(window);

		SDL_Quit();

		return 0;
	}
};

#endif // !_Lesson_2_
