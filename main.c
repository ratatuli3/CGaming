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

	//Renderer
	SDL_Renderer *renderer;
} GameState;

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
            return 1;  // Would collide
        }
    }
    return 0;  // Wouldn't collide
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

	game->man.x = 220;
	game->man.y = 140;	
	
	//Initialize stars
	for(int i = 0; i < 100; i++){
		game->stars[i].x = random()%640;
		game->stars[i].y = random()%480;
	}

	//Initialize ledges
	for(int i = 0; i < 100; i++){
		game->ledges[i].w = 300;
		game->ledges[i].h = 97;
		game->ledges[i].x = i*256;
		game->ledges[i].y = 430;
	}
	game->ledges[99].x = 150;
	game->ledges[99].y = 200;
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
	float speed = 3;
	float nextX = game->man.x;
    float nextY = game->man.y;

	if (state[SDL_SCANCODE_LEFT]){
		nextX-=speed;
	}
	if (state[SDL_SCANCODE_RIGHT]){
		nextX+=speed;
	}
	if (state[SDL_SCANCODE_UP]){
		nextY-=speed;
	}
	if (state[SDL_SCANCODE_DOWN]){
		nextY+=speed;
	}

    // Only move if the next position wouldn't cause a collision
    if (!wouldCollide(game, nextX, nextY)) {
        game->man.x = nextX;
        game->man.y = nextY;
    } else {
        // Optionally try moving on just X or Y axis if diagonal movement failed
        if (!wouldCollide(game, nextX, game->man.y)) {
            game->man.x = nextX;  // Allow X movement
        }
        if (!wouldCollide(game, game->man.x, nextY)) {
            game->man.y = nextY;  // Allow Y movement
        }
    }
	
	return done;
}

void doRender(SDL_Renderer *renderer, GameState *game){
	// Set the drawing color to blue
	SDL_SetRenderDrawColor(renderer, 97, 133, 248, 255);

	// Clear the screen (to blue)
	SDL_RenderClear(renderer);

	// Draw the ledge images
	for(int i = 0; i < 100; i++){
		SDL_Rect ledgeRect = {game->ledges[i].x, game->ledges[i].y, game->ledges[i].w, game->ledges[i].h};
		SDL_RenderCopy(renderer, game->ledgeTexture, NULL, &ledgeRect);
	}

	// Draw a rectangle at man's position
	SDL_Rect rect = {game->man.x, game->man.y, 48, 48};
	SDL_RenderCopyEx(renderer, game->rectTexture, NULL, &rect, 0, NULL, 0);
	//SDL_RenderFillRect(renderer, &rect);

	// Draw the star images
//	for(int i = 0; i < 100; i++){
//		SDL_Rect starRect = {game->stars[i].x, game->stars[i].y, 64, 64};
//		SDL_RenderCopy(renderer, game->starTexture, NULL, &starRect);
//	}

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
