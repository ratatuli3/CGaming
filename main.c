#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <time.h>
#include "main.h"
#include "status.h"


#define GRAVITY 0.3f

int wouldCollide(GameState *game, float nextX, float nextY) {
    float manw = 48, manh = 48;
    
    for(int i = 0; i < 100; i++) {
        float ledgex = game->ledges[i].x;
        float ledgey = game->ledges[i].y;
        float ledgew = game->ledges[i].w;
        float ledgeh = game->ledges[i].h;
        
        // Check if the NEXT position would overlap with this ledge
        if(nextX + manw > ledgex && nextX < ledgex + ledgew &&
           nextY + manh > ledgey && nextY < ledgey + ledgeh) {
            return 1;  // Would collide with ledge
        }
    }    
    for(int i = 0; i < NUM_STARS; i++) {
        float starx = game->stars[i].x;
        float stary = game->stars[i].y;
        float starw = game->stars[i].w;
        float starh = game->stars[i].h-20;
        
        // Check if the NEXT position would overlap with this ledge
        if(game->man.x + manw > starx && game->man.x < starx + starw &&
           game->man.y + manh > stary && game->man.y < stary + starh) {
            return 2;  // Would collide with star
        }
    }
    return 0;  // Wouldn't collide
}

void process(GameState *game){

	// Add time.
	game->time++;
	
	if(game->gameStatus == STATUS_STATE_LIVES){
		if(game->time > 120){
			shutdownStatusLives(game);
			game->gameStatus = STATUS_STATE_GAME;
		}
	}
	else if(game->gameStatus == STATUS_STATE_GAMEOVER){
		SDL_Quit();
		exit(0);
	}
	else if(game->gameStatus == STATUS_STATE_GAME && !game->man.isDead){
		Man *man = &game->man;
		const Uint8 *state = SDL_GetKeyboardState(NULL);

		float speed = 5;
		float nextX = man->x;
		float nextY = man->y + man->dy + GRAVITY;
		float nextDY = man->dy + GRAVITY;

		int jumpPressed = 0;

		if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]){
			nextX-=speed;
		}
		if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]){
			nextX+=speed;
		}
		if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) {
			//if (!man->dy) {
			//	man->dy = -15;
		//}
			jumpPressed = 1;
		}
		//}
		//if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]){
		//	nextY+=speed;
		//}
		else if (wouldCollide(game, man->x, man->y) == 2){ // If it collides with star
			game->man.isDead = 1;            
			game->deathCountdown = 120;
		}

		// Only move if the next position wouldn't cause a collision
		if (!wouldCollide(game, nextX, nextY)) {
			man->x = nextX;
			man->y = nextY;
			man->dy = nextDY;
		} else if (wouldCollide(game, nextX, nextY) == 1){ // If it collides with ledge
		// Optionally try moving on just X or Y axis if diagonal movement failed
			if (jumpPressed) {
				man->dy = -15;
				jumpPressed = 0;
			}
			if (nextDY < 0){ // This means if its touching the bottom cause velocity would only be negative if its already jumping
				man->dy = GRAVITY;
			} else if (man->dy != -15){
				man->dy = 0;
			}
			if (!wouldCollide(game, nextX, man->y)) { // If it doesn't collide horizontally
				man->x = nextX;  // Allow X movement
			}
			if (!wouldCollide(game, man->x, nextY)) { // If it doesn't collide vertically
				man->y = nextY;  // Allow Y movement
				man->dy = nextDY;
			}
		} 
	}
	// Handle death and respawn
	if (game->man.isDead) {
		if (game->deathCountdown > 0) {
			game->deathCountdown--;
		}
		else if (game->man.lives == 3){
			game->man.lives--;
		}
		else if (game->man.lives >= 0) {
			initStatusLives(game);
			game->man.lives--;
			game->gameStatus = STATUS_STATE_LIVES;
			game->time = 0;
			game->man.isDead = 0;
			game->man.x = 100;
			game->man.y = 240 - 40;
			game->man.dy = 0;
		} else if (game->man.lives < 0) {
			game->gameStatus = STATUS_STATE_GAMEOVER;
			game->time = 0;
		}
	}
	game->scrollX = -game->man.x+640;
	if(game->scrollX > 0){
		game->scrollX = 0;
	}
}

void loadGame(GameState *game){
	SDL_Surface *surface = NULL; // Whenever you IMG_Load the surface it frees up the memory of the surface so you can use it again
	
	//Load images and create rendering textures from them
	surface = IMG_Load("../images/star.png");
	if(surface == NULL){
		printf("Cannot find star.png!\n\n");
		SDL_Quit();
		exit(1);
	}
	
	game->starTexture = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	//Load images and create rendering textures from them
	surface = IMG_Load("../images/rect.png");
	if(surface == NULL){
		printf("Cannot find rect.png!\n\n");
		SDL_Quit();
		exit(1);
	}
	
	game->rectTexture = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	//Load images and create rendering textures from them
	surface = IMG_Load("../images/ledge.png");
	if(surface == NULL){
		printf("Cannot find ledge.png!\n\n");
		SDL_Quit();
		exit(1);
	}
	
	game->ledgeTexture = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	
	//Load images and create rendering textures from them
	surface = IMG_Load("../images/fire.png");
	if(surface == NULL){
		printf("Cannot find fire.png!\n\n");
		SDL_Quit();
		exit(1);
	}

	game->fireTexture = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	game->fontTexture = TTF_OpenFont("../fonts/ARCADECLASSIC.TTF", 48);
	if(!game->fontTexture){
		printf("Cannot find ARCADECLASSIC.TTF!\n\n");
		SDL_Quit();
		exit(1);
	}

	game->labelTexture = NULL;

	game->man.x = 220;
	game->man.y = 140;
	game->man.dy = 0;
	game->gameStatus = STATUS_STATE_LIVES;
	game->man.lives = 3;
	game->man.isDead = 0;

	initStatusLives(game);

	game->time = 0;
	game->scrollX = 0;
	game->deathCountdown = -1;
	
	//Initialize stars
	for(int i = 0; i < NUM_STARS; i++){
		game->stars[i].x = 320+random()%38400;
		game->stars[i].y = random()%1080;
		game->stars[i].w = 64;
		game->stars[i].h = 64;
	}

	//Initialize ledges
	for(int i = 0; i < 100; i++){
		game->ledges[i].w = 300;
		game->ledges[i].h = 97;
		game->ledges[i].x = i*256;
		game->ledges[i].y = 900;
	}
	game->ledges[99].x = 300;
	game->ledges[99].y = 670;
}

int processEvents(SDL_Window *window, GameState *game){
	SDL_Event event;
	int done = 0;

	while(SDL_PollEvent(&event)){ // this periodically check for events which i guess would be commands given to the thing
		switch(event.type){
			case SDL_WINDOWEVENT_CLOSE:{
					if(window){
						SDL_DestroyWindow(window);
						window = NULL;
						done = 1;
					}
			}
			break;
			case SDL_KEYDOWN:{ // when a key has been pressed on the keyboard, this will probably be very useful
				switch(event.key.keysym.sym){
					case SDLK_ESCAPE: // like escape
					done = 1;
					break;
					case SDLK_RIGHT: // keep in mind these cases are a one button mash thing differently from below where its a button hold
					break;
					case SDLK_LEFT:
					break;
					case SDLK_UP:
					break;
					case SDLK_DOWN:
					break;
					case SDLK_d:
					break;
					case SDLK_a:
					break;
					case SDLK_w:
					break;
					case SDLK_s:
					break;
				}
			}
			break;
			case SDL_QUIT:
				// quit out of the game
				done = 1;
			break;
		}
	}

	// This is basically that if you keep a button pressed it will keep getting that value as a command.
	// Before we had to constantly mash the button.


	
	return done;
}

void doRender(SDL_Renderer *renderer, GameState *game){

	if(game->gameStatus == STATUS_STATE_LIVES){
		drawStatusLives(game);
	} 
	else if(game->gameStatus == STATUS_STATE_GAME){

		// Set the drawing color to blue
		SDL_SetRenderDrawColor(renderer, 97, 133, 248, 255);

		// Clear the screen (to blue)
		SDL_RenderClear(renderer);

		// Draw the ledge images
		for(int i = 0; i < 100; i++){
			SDL_Rect ledgeRect = {game->scrollX+game->ledges[i].x, game->ledges[i].y, game->ledges[i].w, game->ledges[i].h};
			SDL_RenderCopy(renderer, game->ledgeTexture, NULL, &ledgeRect);
		}

		// Draw a rectangle at man's position
		SDL_Rect rect = {game->scrollX+game->man.x, game->man.y, 48, 48};
		SDL_RenderCopyEx(renderer, game->rectTexture, NULL, &rect, 0, NULL, 0);
		
		if(game->man.isDead){
			// Draw a rectangle at man's position
			SDL_Rect rect = {game->scrollX+game->man.x-15, game->man.y-20, 75, 75};
			SDL_RenderCopyEx(renderer, game->fireTexture, NULL, &rect, 0, NULL, game->time%20 < 10);
		}

		//Draw the star images
		for(int i = 0; i < NUM_STARS; i++){
			SDL_Rect starRect = {game->scrollX+game->stars[i].x, game->stars[i].y, game->stars[i].w, game->stars[i].h};
			SDL_RenderCopy(renderer, game->starTexture, NULL, &starRect);
		}
	}
	//After drawing we show what we've drawn
	SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]){

	GameState gameState;
	SDL_Window *window = NULL;	// Declare a window
	SDL_Renderer *renderer = NULL;	// Declare a renderer

	SDL_Init(SDL_INIT_VIDEO); //Initialize SDL2
	
	srandom((int)time(NULL));

	//Create an application window with the following settings:
	window = SDL_CreateWindow(
		"Square Game",		// Window Title
		SDL_WINDOWPOS_UNDEFINED,// initial x position
		SDL_WINDOWPOS_UNDEFINED,// initial y position
		1280,	// width
		960,	// height
		0	//flags
	);
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	gameState.renderer = renderer;

	TTF_Init(); // Initialize font system

	loadGame(&gameState);
	
	// The window is open: enter program loop (see SDL_PollEvent)
	int done = 0;

	// Event Loop
	while(!done){
		// Check for events
		done = processEvents(window, &gameState);

		process(&gameState);

		//Render display
		doRender(renderer, &gameState);
	}

	// Shutdown game and unload all memory
	SDL_DestroyTexture(gameState.starTexture);
	SDL_DestroyTexture(gameState.rectTexture);
	SDL_DestroyTexture(gameState.ledgeTexture);

	if(gameState.labelTexture != NULL){
		SDL_DestroyTexture(gameState.labelTexture);
	}
	
	TTF_CloseFont(gameState.fontTexture);

	// Close and destroy the window
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	// Close fonts
	TTF_Quit();

	//Clean up
	SDL_Quit();
	return 0;
}
