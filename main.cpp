//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

// loads individual image as a texture
SDL_Texture* loadTexture(std::string path);

// the window we'll be rendering to
SDL_Window *gWindow = NULL;

// the window renderer
SDL_Renderer* gRenderer = NULL;

SDL_Texture* gTexture = NULL;

// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

// the images that correspond to a keypress
SDL_Surface* gKeyPressSurface[ KEY_PRESS_SURFACE_TOTAL ];

// current displayed image
SDL_Surface* gStretchedSurface = NULL;

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
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if ( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer  == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
            }
            else
            {
                // initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if(!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;

                }

            }


//            else
//            {
//                //Get window surface
//                gScreenSurface = SDL_GetWindowSurface( gWindow );
//            }
        }
    }
    return success;
}

SDL_Texture* loadTexture(std::string path)
{
    //the final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
   if (loadedSurface == NULL)
   {
       printf("Unable to load image at %s! SDL_image_erorr: %s\n", path.c_str(), IMG_GetError);
   }
   else
   {
       // create texture from surface pixels
       newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
       if (newTexture == NULL)
       {
           printf("Unable to create texture from %s! SDL Error:%s\n", path.c_str(), SDL_GetError());
       }
       else
       {
           //Get rid of old loaded surface
           SDL_FreeSurface(loadedSurface);
       }
   }

   return newTexture;
}

SDL_Surface* loadSurface(std::string path)
{
	// The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		// Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
		if (optimizedSurface == NULL)
		{
			printf("Unable to optimize the image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		// get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
}


bool loadMedia()
{
	//loading success flag
	bool success = true;

	// load PNG texture
	gTexture = loadTexture("texture.png");
	if (gTexture == NULL)
    {
	    printf("Failed to load texture image!\n");
	    success = false;
    }
	return success;
}

void close()
{
	//deallocate surface
	SDL_DestroyTexture( gTexture );
	gTexture = NULL;

	// Destroy window
	SDL_DestroyRenderer( gRenderer);
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	// Quit SDL subsystems
	IMG_Quit();
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

			gStretchedSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];

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
				//Clear screen
				SDL_RenderClear(gRenderer);

				// render texture to screen
				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	// free resources and close SDL
	close();

	return 0;
}

