#ifndef MAIN_H
#define MAIN_H

#define STATUS_STATE_LIVES 0
#define STATUS_STATE_GAME 1
#define STATUS_STATE_GAMEOVER 2

#define NUM_STARS 100

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef struct{
	float x, y;
	float dy;
	short lives;
	int isDead;
	char *name;
} Man;

typedef struct{
	int x, y, w, h;
} Star;

typedef struct{
	int x, y, w, h;
} Ledge;

typedef struct{
	float scrollX;
	//Players
	Man man;

	//Stars
	Star stars[NUM_STARS];

	//Ledges
	Ledge ledges[100];

	//Images
	SDL_Texture *starTexture;
	SDL_Texture *rectTexture;
	SDL_Texture *ledgeTexture;
	SDL_Texture *labelTexture;
	SDL_Texture *fireTexture;
	
	//Fonts
	TTF_Font *fontTexture;

	//Time
	int time;
	int gameStatus;
	
	//Text
	int labelh, labelw;

	//Renderer
	SDL_Renderer *renderer;
} GameState;

#endif
