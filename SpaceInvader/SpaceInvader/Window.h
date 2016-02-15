#pragma once

#include "standardLoad.h"

class Window
{
	private:
		SDL_Window* window;
		SDL_Surface* screen;
		SDL_Event event;

	public:
		Window(void);
		~Window(void);

		void createWindow(const char* title, int x, int y, int w, int h, Uint32 flags);
		bool windowRunning(SDL_Event event);
		
		void destroyWindow();
		
		SDL_Window* getWindow();
		SDL_Surface* getWindowSurface();
		
};

