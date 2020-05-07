//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Key press surfaces constants
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

//starts up SDL and create window
bool init();

//loads media
bool loadMedia();

// frees media and shuts down SDL
void close();

// loads individual image
SDL_Surface* loadSurface(std::string path);

// the window we'll be rendering to
SDL_Window *gWindow = NULL;

// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

// the images that correspond to a keypress
SDL_Surface* gKeyPressSurface[ KEY_PRESS_SURFACE_TOTAL ];

// current displayed image
SDL_Surface* gCurrentSurface = NULL;

SDL_Surface* loadSurface(std::string path)
{
	//load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}

	return loadedSurface;
}

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

	//load default surface
	gKeyPressSurface[ KEY_PRESS_SURFACE_DEFAULT ] = loadSurface("press.bmp");
	if ( gKeyPressSurface[ KEY_PRESS_SURFACE_DEFAULT] == NULL )
	{
		printf( "Unable to load default image");
		success = false;
	}

	//load up surface
	gKeyPressSurface[KEY_PRESS_SURFACE_UP] = loadSurface("up.bmp");
	if (gKeyPressSurface[KEY_PRESS_SURFACE_UP] == NULL)
	{
		printf("Unable to load up image\n");
		success = false;
	}

	//load down surface
	gKeyPressSurface[KEY_PRESS_SURFACE_DOWN] = loadSurface("down.bmp");
	if (gKeyPressSurface[KEY_PRESS_SURFACE_DOWN] == NULL)
	{
		printf("Unable to load down image!\n");
		success = false;
	}

	// load left surface
	gKeyPressSurface[KEY_PRESS_SURFACE_LEFT] = loadSurface("left.bmp");
	if(gKeyPressSurface[KEY_PRESS_SURFACE_LEFT] == NULL)
	{
		printf("Unable to load left image!\n");
		success = false;
	}

	//load right surface
	gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT] = loadSurface("right.bmp");
	if(gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT] == NULL)
	{
		printf("Unable to load right image");
		success = false;
	}

	return success;
}

void close()
{
	//deallocate surface
	SDL_FreeSurface( gCurrentSurface );
	gCurrentSurface = NULL;

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

			gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];

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
					// user presses a key
					else if(e.type == SDL_KEYDOWN)
					{
						//select surfaces based on key presses
						switch(e.key.keysym.sym)
						{
							case SDLK_UP:
								gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_UP];
								break;

							case SDLK_DOWN:
								gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DOWN];
								break;

							case SDLK_LEFT:
								gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_LEFT];
								break;

							case SDLK_RIGHT:
								gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT];
								break;

							default:
								gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];
								break;
						}
					}
				}
				//apply the current image
				SDL_BlitSurface( gCurrentSurface, NULL, gScreenSurface, NULL );

				//update the surface
				SDL_UpdateWindowSurface( gWindow );
			}
		}
	}
	// free resources and close SDL
	close();

	return 0;
}

