#include <iostream>
#include <deque>
#include <cmath>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

using namespace std;

const int SCHEIGHT      = 600;
const int SCWIDTH       = 400;
const char* TITLE       = "Supership3000";

const Uint8 *state      = SDL_GetKeyboardState(NULL);
bool done               = false;

SDL_Window *window      ;
SDL_Renderer *rend      ;
SDL_Event event         ;
int toggleTime          = 0;
const int TOGDEL        = 250;
const int RENDEL        = 0;
const int SCALEMODE     = 0;
int FPS                 = 60;
int FPC                 = 1000/FPS;
int frame               = 0;
int timeMes             ;
int capDel              ;
bool CAPFPS             = true;

bool shipMv             ;
SDL_Texture *shipTex       ;
float shipVel           = 0;
const float SHIPACCEL     = 5;
const float SHIPDECEL     = 2;
const float FRICRATE    = .1;
const float NOFRIC      = 1.0;
SDL_Rect shipRect       = {SCWIDTH/2 - SCWIDTH/20,  SCHEIGHT - (SCWIDTH/10 + 10), SCWIDTH/10, SCWIDTH/10 + 10};
const int WINCONST      = 10;
const int MAXSPEED      = 15;

SDL_Texture* bulTex     = NULL;
const int BVEL          = 3;
const int BNUM          = 100;
const int BSZ           = 10;
const int BULY          = shipRect.y - BSZ/2;
const int BREM         = -BSZ;
const int BDEL          = 75;
int bulTime             = -BDEL;
deque<SDL_Rect> bullets ;

void logSDLError(ostream &os, const string &message) {
	os << message << " error: " << SDL_GetError() << endl;
}

SDL_Texture* loadTexture(const string &file, SDL_Renderer *renderer) {
	SDL_Texture *texture = NULL;
	SDL_Surface *image = IMG_Load(file.c_str());
	if (image != NULL) {
		texture = SDL_CreateTextureFromSurface(renderer, image);
		SDL_FreeSurface(image);
		if (texture == NULL)
			logSDLError(cout, "CreateTextureFromSurface");
	}
	else
		logSDLError(cout, "IMG_Load");

	return texture;
}

void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, SDL_Rect sizeRect) {
	SDL_RenderCopy(renderer, texture, NULL, &sizeRect);
}

void addBullet() {
    SDL_Rect temp = {shipRect.x + shipRect.w/2 - BSZ/2, BULY, BSZ, BSZ};
    bullets.push_back(temp);
}


bool initGame() {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logSDLError(cout, "SDL_Init");
        return 1;
    }

     window = SDL_CreateWindow(TITLE, -1, -1, SCWIDTH, SCHEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        logSDLError(cout, "CreateWindow");
        return 2;
    }

    rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (rend == NULL) {
        logSDLError(cout, "CreateRenderer");
        return 3;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, SCALEMODE);
    SDL_RenderSetLogicalSize(rend, SCWIDTH, SCHEIGHT);

    shipTex = loadTexture("assets/ship.png", rend);
    if (shipTex == NULL) {
        return 4;
    }

    bulTex = loadTexture("assets/bullet.png", rend);
    if (bulTex == NULL) {
        return 5;
    }
    return true;
}

void renderGame() {
    timeMes = SDL_GetTicks();
    SDL_RenderClear(rend);
    renderTexture(shipTex, rend, shipRect);

    for (int i = 0; i < bullets.size(); i++) {
        renderTexture(bulTex, rend, bullets.at(i));
    }


    SDL_RenderPresent(rend);
    SDL_Delay(RENDEL);
    frame++;
    capDel = SDL_GetTicks() - timeMes;
    if(CAPFPS == true && capDel < FPC) {
        SDL_Delay(FPC - capDel);
    }
}

void toggleFullscreen()
{
    Uint32 flags = (SDL_GetWindowFlags(window) ^ SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (SDL_SetWindowFullscreen(window, flags) < 0) {
        cout << "Toggling fullscreen mode failed: " << SDL_GetError() << endl;
    }
    if ((flags & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0) {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(rend, SCWIDTH, SCHEIGHT);
    }
    SDL_SetWindowSize(window, SCWIDTH, SCHEIGHT);
}

void eventHandle() {
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT || state[SDL_SCANCODE_Q]) {
        done = true;
    }

    if (state[SDL_SCANCODE_SPACE]) {
        if (SDL_GetTicks() - bulTime > BDEL) {
                addBullet();
                bulTime = SDL_GetTicks();
        }

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
        if (SDL_GetTicks() - toggleTime > TOGDEL) {
            toggleFullscreen();
            toggleTime = SDL_GetTicks();
        }

    }



}



void updatePositions() {
    if (shipVel > MAXSPEED) {
        shipVel = MAXSPEED;
    } else if (shipVel < -MAXSPEED) {
        shipVel = -MAXSPEED;
    }

    if (shipRect.x < WINCONST) {
        shipRect.x = WINCONST;
    } else if (shipRect.x > (SCWIDTH - shipRect.w - WINCONST)) {
        shipRect.x = (SCWIDTH - shipRect.w - WINCONST);
    }

    shipVel = shipVel * (NOFRIC - FRICRATE);
    if (abs(shipVel) < .01) {
        shipVel = 0;
    }

    shipRect.x += shipVel;


    for (int i = 0; i < bullets.size(); i++) {
        if (bullets.front().y < BREM) {
            bullets.pop_front();
        }
    }

    for (int i = 0; i < bullets.size(); i++ ) {
        bullets.at(i).y -= BVEL;
    }
}


int main() {
    int i = 1;
    if (initGame()) {
        cout << "Successfully initialized." << endl;
        while (not done) {
            eventHandle();
            updatePositions();
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

