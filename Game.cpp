#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

using namespace std;

const int SCHEIGHT = 600;
const int SCWIDTH = 400;
const char* TITLE = "SuperShip 3000";


using namespace std;

void logSDLError(ostream &os, const string &message){
	os << message << " error: " << SDL_GetError() << endl;
}

SDL_Texture* loadTexture(const string &file, SDL_Renderer *renderer){
	SDL_Texture *texture = NULL;
	SDL_Surface *image = SDL_LoadBMP(file.c_str());
	if (image != NULL){
		texture = SDL_CreateTextureFromSurface(renderer, image);
		SDL_FreeSurface(image);
		if (texture == NULL)
			logSDLError(cout, "CreateTextureFromSurface");
	}
	else
		logSDLError(cout, "IMG_Load");

	return texture;
}

void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, SDL_Rect sizeRect){
	SDL_RenderCopy(renderer, texture, NULL, &sizeRect);
}

int main() {

    if (SDL_Init(SDL_INIT_EVERYTHING) != NULL){
        logSDLError(cout, "SDL_Init");
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(TITLE, NULL, NULL, SCWIDTH, SCHEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL){
        logSDLError(cout, "CreateWindow");
        return 2;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL){
        logSDLError(cout, "CreateRenderer");
        return 3;
    }

    SDL_Texture *ship = loadTexture("assets/ship.bmp", renderer);
    SDL_Rect shipRect = {SCWIDTH/2 - SCWIDTH/20, SCHEIGHT - (SCWIDTH/10 + 10), SCWIDTH/10, SCWIDTH/10 + 10};
    if (ship == NULL) {
        return 4;
    }



    SDL_Event event;

    bool quit = false;

    float shipVel = 0;
    float shipPos = 0;
    bool shipMv;

    int time;

    while (! quit) {
        time = SDL_GetTicks();
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            quit = true;
        } else {
            if (event.type == SDL_KEYDOWN) {
                shipMv = true;
                if (event.key.keysym.sym == SDLK_RIGHT) {
                    shipVel += 5;
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    shipVel -= 5;
                } else if (event.key.keysym.sym == SDLK_SPACE) {
                    cout << "Shoot!" << endl;
                }
            } else if (event.type == SDL_KEYUP) {
                if (shipMv == true) {
                    if (event.key.keysym.sym == SDLK_RIGHT) {
                        shipVel -= 1;
                        shipMv = false;
                    } else if (event.key.keysym.sym == SDLK_LEFT) {
                        shipVel += 1;
                        shipMv = false;
                    }
                }
            }
        }

        if (shipVel > 10) {
            shipVel = 10;
        } else if (shipVel < -10) {
            shipVel = -10;
        }

        if (shipPos < 1) {
            shipPos = 1;
        } else if (shipPos > (SCWIDTH - shipRect.w - 1)) {
            shipPos = (SCWIDTH - shipRect.w - 1);
        }

        shipVel = shipVel * .9;

        shipPos += shipVel;
        shipRect.x = shipPos;
        SDL_RenderClear(renderer);
        renderTexture(ship, renderer, shipRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

