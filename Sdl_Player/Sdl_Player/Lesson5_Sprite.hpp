#ifndef _Lesson_5_Sprite_
#define _Lesson_5_Sprite_

#include "SDL.h"
#include "SDL_image.h"
#include <stdexcept>
#include <string>
#include <iostream>
#include "IExecutor.h"

using namespace std;

class Lesson5_Sprite : public IExecutor {
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

	void applySurface(int x, int y, SDL_Texture *texture, SDL_Renderer *ren,SDL_Rect *clip = nullptr) {
		SDL_Rect pos;
		pos.x = x;
		pos.y = y;

		if (clip != nullptr) {
			pos.w = clip->w;
			pos.h  = clip->h;
		}else {
			SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
		}
		SDL_RenderCopy(ren, texture, clip, &pos);
	}

public:
	int execute(int argc, char *argv[]) {

		if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
			std::cout << SDL_GetError() << std::endl;
			return 1;
		}

		window = SDL_CreateWindow("Lesson5 Sprite", SDL_WINDOWPOS_CENTERED,
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
			image = loadImage("sprite.png");
		}
		catch (runtime_error &e) {
			cout << e.what() << std::endl;
			return 4;
		}


		int iW = 0, iH = 0;
		SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
		iW /= 2;
		iH /= 2;
		int useClip = 0;
		SDL_Rect clips[4];
		int column = 0;
		for (int i = 0; i < 4; i++) {
			if (i != 0 && i % 2 == 0)
				++column;

			clips[i].x = column * iW;
			clips[i].y = i % 2 * iH;
			clips[i].w = iW;
			clips[i].h = iH;
		}//end for i

		SDL_Event e;//ÊÂ¼þ
		bool quit = false;
		while (!quit) {
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				
				if (e.type == SDL_MOUSEBUTTONDOWN) {
					quit = true;
				}

				if (e.type == SDL_KEYDOWN) {
					switch (e.key.keysym.sym) {
					case SDLK_1:
						useClip = 0;
						break;
					case SDLK_2:
						useClip = 1;
						break;
					case SDLK_3:
						useClip = 2;
						break;
					case SDLK_4:
						useClip = 3;
						break;
						//For quitting, escape key
					case SDLK_ESCAPE:
						quit = true;
						break;
					default:
						break;
					}
				}
			}

			SDL_RenderClear(render);
			int x = SCREEN_WIDTH / 2 - iW;
			int y = SCREEN_HEIGHT / 2 - iH;
			applySurface(x, y, image, render, &clips[useClip]);
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

#endif // 
