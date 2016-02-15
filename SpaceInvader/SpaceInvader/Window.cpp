#include "Window.h"

Window::Window(void) {}
Window::~Window(void) {}

void Window::createWindow(const char* title, int x, int y, int w, int h, Uint32 flags)
{
	window = NULL;
	screen = NULL;

	window = SDL_CreateWindow( title, x, y, w, h, flags);

	if(window==NULL)
		printf("SDL Window could not be created! Reason: ", SDL_GetError());
	else {
		screen = SDL_GetWindowSurface(window);

		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));

		SDL_UpdateWindowSurface(window);
	}
}

bool Window::windowRunning(SDL_Event event)
{
	if (event.type == SDL_QUIT)
		return false;
	else
		return true;
}

void Window::destroyWindow() 
{
	SDL_FreeSurface(screen);
	screen = NULL;

	SDL_DestroyWindow(window);
	window = NULL;
}

SDL_Window* Window::getWindow()
{
	return window;
}

SDL_Surface* Window::getWindowSurface()
{
	return screen;
}
