#include <SDL2/SDL.h>
#include <stdio.h>
#include "main.h"

void initStatusLives(GameState *game){
	char str[128] = "";

	sprintf(str, "x %d", (int)game->man.lives);
	SDL_Color white = { 255, 255, 255, 255 };

	SDL_Surface *tmp = TTF_RenderText_Blended(game->fontTexture, str, white);	
	game->labelw = tmp->w;
	game->labelh = tmp->h;
	game->labelTexture = SDL_CreateTextureFromSurface(game->renderer, tmp);
	SDL_FreeSurface(tmp);
}

void drawStatusLives(GameState *game){
	SDL_Renderer *renderer = game->renderer;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	// Clear the screen
	SDL_RenderClear(renderer);
	
	// Draw a rectangle at man's position
	SDL_Rect rect = { 570, 450, 48, 48};
	SDL_RenderCopyEx(renderer, game->rectTexture, NULL, &rect, 0, NULL, 0);
	
	// Set the drawing color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	SDL_Rect textRect = { 630, 450, game->labelw+20, game->labelh };
	SDL_RenderCopy(renderer, game->labelTexture, NULL, &textRect);
}

void shutdownStatusLives(GameState *game){
	SDL_DestroyTexture(game->labelTexture);
	game->labelTexture = NULL;
}
