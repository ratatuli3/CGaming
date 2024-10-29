#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <time.h>

typedef struct{
	int x, y;
	short life;
	char *name;
} Man;

typedef struct{
	int x, y;
} Star;

typedef struct{
	
	//Players
	Man man;

	//Stars
	Star stars[100];

	//Images
	SDL_Texture *starTexture;
	SDL_Texture *rectTexture;

	//Renderer
	SDL_Renderer *renderer;
} GameState;

void loadGame(GameState *game){
	SDL_Surface *starSurface, *rectSurface = NULL;
	game->man.x = 220;
	game->man.y = 140;	
	
	//Load images and create rendering textures from them
	starSurface = IMG_Load("../images/star.png");
	if(starSurface == NULL){
		printf("Cannot find star.png!\n\n");
		SDL_Quit();
		exit(1);
	}	

	//Load images and create rendering textures from them
	rectSurface = IMG_Load("../images/rect.png");
	if(rectSurface == NULL){
		printf("Cannot find rect.png!\n\n");
		SDL_Quit();
		exit(1);
	}

	game->starTexture = SDL_CreateTextureFromSurface(game->renderer, starSurface);
	SDL_FreeSurface(starSurface);
	
	game->rectTexture = SDL_CreateTextureFromSurface(game->renderer, rectSurface);
	SDL_FreeSurface(rectSurface);

	//Initialize stars
	for(int i = 0; i < 100; i++){
		game->stars[i].x = random()%640;
		game->stars[i].y = random()%480;
	}


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
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_LEFT]){
		game->man.x-=3;
	}
	if (state[SDL_SCANCODE_RIGHT]){
		game->man.x+=3;
	}
	if (state[SDL_SCANCODE_UP]){
		game->man.y-=3;
	}
	if (state[SDL_SCANCODE_DOWN]){
		game->man.y+=3;
	}
	
	return done;
}

void doRender(SDL_Renderer *renderer, GameState *game){
	// Set the drawing color to blue
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	// Clear the screen (to blue)
	SDL_RenderClear(renderer);

	// Set the drawing color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// Draw a rectangle at man's position
	SDL_Rect rect = {game->man.x, game->man.y, 50, 50};
	SDL_RenderCopy(renderer, game->rectTexture, NULL, &rect);
	//SDL_RenderFillRect(renderer, &rect);
	
	// Draw the star image
	for(int i = 0; i < 100; i++){
		SDL_Rect starRect = {game->stars[i].x, game->stars[i].y, 64, 64};
		SDL_RenderCopy(renderer, game->starTexture, NULL, &starRect);
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
		640,	// width
		480,	// height
		0	//flags
	);
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	gameState.renderer = renderer;
	loadGame(&gameState);
	
	// The window is open: enter program loop (see SDL_PollEvent)
	int done = 0;

	// Event Loop
	while(!done){
		// Check for events
		done = processEvents(window, &gameState);

		//Render display
		doRender(renderer, &gameState);
	}

	// Shutdown game and unload all memory
	SDL_DestroyTexture(gameState.starTexture);
	SDL_DestroyTexture(gameState.rectTexture);

	// Close and destroy the window
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	//Clean up
	SDL_Quit();
	return 0;
}
