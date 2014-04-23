#include <iostream>
#include "SDL2/SDL.h"

using namespace std;

const int SCHEIGHT      = 600;
const int SCWIDTH       = 400;
const char* TITLE       = "SuperShip 3000";

const Uint8 *state      = SDL_GetKeyboardState(NULL);
bool done               = false;

SDL_Window *window      ;
SDL_Renderer *rend      ;
SDL_Event event         ;
int toggleTime          = 0;
const int RENDEL        = 0;
const int SCALEMODE     = 0;

bool shipMv             ;
SDL_Rect shipRect       ;
SDL_Texture *ship       ;
float shipVel           = 0;
float shipPos           = 0;
const float SHIPACCEL     = 10;
const float SHIPDECEL     = 10;
const float FRICRATE    = .1;
const int SHIPW         = SCWIDTH/10;
const int SHIPH         = SCWIDTH/10 + 10;
const int XSTART        = SCWIDTH/2 - SCWIDTH/20;
const int SHIPY         = SCHEIGHT - (SCWIDTH/10 + 10);
const int WINCONST      = 10;
const int MAXSPEED      = 10;

const int BVEL          = 5;
const int BNUM          = 100;
SDL_Rect* bullets[BNUM] ;

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

     window = SDL_CreateWindow(TITLE, -1, -1, SCWIDTH, SCHEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL){
        logSDLError(cout, "CreateWindow");
        return 2;
    }

    rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (rend == NULL){
        logSDLError(cout, "CreateRenderer");
        return 3;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, SCALEMODE);  // make the scaled rendering look smoother.
    SDL_RenderSetLogicalSize(rend, SCWIDTH, SCHEIGHT);

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

void renderGame() {
    SDL_RenderClear(rend);
    renderTexture(ship, rend, shipRect);
    SDL_RenderPresent(rend);
    SDL_Delay(RENDEL);
}

void shootGun() {
    for (int i = 0; i < BNUM; i++) {

    }
}


void toggleFullscreen()
{
    Uint32 flags = (SDL_GetWindowFlags(window) ^ SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (SDL_SetWindowFullscreen(window, flags) < 0) // NOTE: this takes FLAGS as the second param, NOT true/false!
    {
        std::cout << "Toggling fullscreen mode failed: " << SDL_GetError() << std::endl;
    }
    if ((flags & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
    {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(rend, SCWIDTH, SCHEIGHT);
    }
    SDL_SetWindowSize(window, SCWIDTH, SCHEIGHT);
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

    if (state[SDL_SCANCODE_F]) {
        cout << SDL_GetTicks() - toggleTime << endl;
        if (SDL_GetTicks() - toggleTime > 250) {
            toggleFullscreen();
            toggleTime = SDL_GetTicks();
        }

    }



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

    shipVel = shipVel * (1.0 - FRICRATE);

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

