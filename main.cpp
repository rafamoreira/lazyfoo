//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//starts up SDL and create window
bool init();

//loads media
bool loadMedia();

// frees media and shuts down SDL
void close();

// the window we'll be rendering to
SDL_Window *gWindow = NULL;

// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

// The image we will load and show on the screen
SDL_Surface *gXout = NULL;

bool init()
{
	// initialization flag
	bool success = true;

	//initialize SDL

	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
		                            SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if ( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}
	return success;
}

bool loadMedia()
{
	//loading success flag
	bool success = true;

	//load splash image
	gXout = SDL_LoadBMP( "../x.bmp" );
	if ( gXout == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", "hello_world.bmp",
		        SDL_GetError());
		success = false;
	}

	return success;
}

void close()
{
	//deallocate surface
	SDL_FreeSurface( gXout );
	gXout = NULL;

	// Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	// Quit SDL subsystems
	SDL_Quit();
}

int main(int argc, char *args[])
{

	//Startup SDL and create windows
	if ( !init())
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if ( !loadMedia())
		{
			printf( "Failed to load media!\n" );
		}
		else
		{

			//main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			// while application is running
			while (!quit)
			{
				// Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//user requests to quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}
				//apply the image
				SDL_BlitSurface( gXout, NULL, gScreenSurface, NULL );

				//update the surface
				SDL_UpdateWindowSurface( gWindow );
			}
		}
	}
	// free resources and close SDL
	close();

	return 0;
}

