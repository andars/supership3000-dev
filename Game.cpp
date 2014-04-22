#include <iostream>
#include "SDL2/SDL.h"

using namespace std;

const int SCHEIGHT      = 600;
const int SCWIDTH       = 400;
const char* TITLE       = "SuperShip 3000";
const int WINCONST      = 5;
const int MAXSPEED      = 6;

const Uint8 *state      = SDL_GetKeyboardState(NULL);
bool done               = false;

SDL_Window *window      ;
SDL_Renderer *rend      ;
SDL_Event event         ;

bool shipMv             ;
SDL_Rect shipRect       ;
SDL_Texture *ship       ;
float shipVel           = 0;
float shipPos           = 0;
const int SHIPACCEL     = 1;
const int SHIPDECEL     = .5;
const int SHIPW         = SCWIDTH/10;
const int SHIPH         = SCWIDTH/10 + 10;
const int XSTART        = SCWIDTH/2 - SCWIDTH/20;
const int SHIPY         = SCHEIGHT - (SCWIDTH/10 + 10);

const int BVEL          = 5;
SDL_Rect* bullets[100]  ;

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


bool initGame() {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        logSDLError(cout, "SDL_Init");
        return 1;
    }

     window = SDL_CreateWindow(TITLE, -1, -1, SCWIDTH, SCHEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL){
        logSDLError(cout, "CreateWindow");
        return 2;
    }

    rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (rend == NULL){
        logSDLError(cout, "CreateRenderer");
        return 3;
    }

    ship = loadTexture("assets/ship.bmp", rend);
    shipRect.x = XSTART;
    shipRect.y = SHIPY;
    shipRect.w = SHIPW;
    shipRect.h = SHIPH;
    if (ship == NULL) {
        return 4;
    }
    return true;
}

void eventHandle() {
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT) {
        done = true;
    }

    if (state[SDL_SCANCODE_SPACE]) {
        cout << "Shoot at time:" << SDL_GetTicks() << "." << endl;
    }
    if (state[SDL_SCANCODE_LEFT]) {
         shipMv = true;
         shipVel -= SHIPACCEL;
    } else {
        if (shipMv == true) {
            shipVel += SHIPDECEL;
            shipMv = false;
        }
    }


    if (state[SDL_SCANCODE_RIGHT]) {
        shipMv = true;
        shipVel += SHIPACCEL;
    } else {
        if (shipMv == true) {
            shipVel -= SHIPDECEL;
            shipMv = false;
        }
    }
}

void renderGame() {
    SDL_RenderClear(rend);
    renderTexture(ship, rend, shipRect);
    SDL_RenderPresent(rend);
    SDL_Delay(5);
}


void moveShip() {
    if (shipVel > MAXSPEED) {
        shipVel = MAXSPEED;
    } else if (shipVel < -MAXSPEED) {
        shipVel = -MAXSPEED;
    }

    if (shipPos < WINCONST) {
        shipPos = WINCONST;
    } else if (shipPos > (SCWIDTH - SHIPW - WINCONST)) {
        shipPos = (SCWIDTH - SHIPW - WINCONST);
    }

    shipVel = shipVel * .95;

    shipPos += shipVel;
    shipRect.x = shipPos;
}


int main() {
    int i = 1;
    if (initGame()) {
        cout << "Successfully initialized." << endl;
        while (not done) {
            eventHandle();
            moveShip();
            renderGame();
        }
    } else {
        cout << "Failed to initialize.";
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    cout << "Successfully exited." << endl;
    return 0;
}

