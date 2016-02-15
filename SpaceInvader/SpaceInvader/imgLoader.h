#pragma once

using namespace std;

#include "standardLoad.h"

class imgLoader
{
	public:

		SDL_Surface* optimizedLoad(string path, SDL_Surface* screen) {

			SDL_Surface* optimizedSurface = NULL;
			SDL_Surface* loadedSurface = IMG_Load( path.c_str() );

			if( loadedSurface == NULL )
				printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
			else
			{
				optimizedSurface = SDL_ConvertSurface( loadedSurface, screen->format, NULL );
				if( optimizedSurface == NULL )
					printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );

				SDL_FreeSurface( loadedSurface );
			}

			return optimizedSurface;
		};

		bool renderToScreen(SDL_Surface* imgSurface, SDL_Rect* imgPosition, SDL_Rect* imgScale, SDL_Surface* screen) {

			if(SDL_BlitScaled(imgSurface, imgScale, screen, imgPosition) == 0)
				return true;
			else {
				printf("Error blitting image to screen. REASON: ", SDL_GetError());
				return false;
			}
		}
};

