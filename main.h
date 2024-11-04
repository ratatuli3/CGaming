#ifndef MAIN_H
#define MAIN_H

#define STATUS_STATE_LIVES 0
#define STATUS_STATE_GAME 1
#define STATUS_STATE_GAMEOVER 2

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef struct{
	float x, y;
	float dy;
	short lives;
	char *name;
} Man;

typedef struct{
	int x, y;
} Star;

typedef struct{
	int x, y, w, h;
} Ledge;

typedef struct{
	
	//Players
	Man man;

	//Stars
	Star stars[100];

	//Ledges
	Ledge ledges[100];

	//Images
	SDL_Texture *starTexture;
	SDL_Texture *rectTexture;
	SDL_Texture *ledgeTexture;
	SDL_Texture *labelTexture;
	
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
