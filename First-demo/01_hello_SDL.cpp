// Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <iostream>

// Screen dimension constants
const int screen_width = 2149;
const int screen_height = 1175;

// Starts SDL
bool init();
// Load Media
bool loadMedia();
// Free Memory and 
void close();

// The window we'll be rendering to
SDL_Window* window = NULL;
// The surface/canvas we put into the window
SDL_Surface* screenSurface = NULL;
// The image we load 
SDL_Surface* image = NULL;

bool init() {
	// A flag indicating initialization
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL failed to initialize, exit with code: " << SDL_GetError() << ".\n";
		success = false;
	}
	else {
		window = SDL_CreateWindow("My image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			std::cout << "Window cannot be created, exit with code: " << SDL_GetError() << ".\n";
			success = false;
		}
		else {
			screenSurface = SDL_GetWindowSurface(window);
		}
	}
	return success;
}

bool loadMedia() {
	// Similar to init(), we need a flag here
	bool success = true;

	image = SDL_LoadBMP("temp/test.bmp");
	if (image == NULL) {
		std::cout << "SDL failed to load BMP, exit with code: " << SDL_GetError() << ".\n";
		success = false;
	}
	
	return success;
}

void close() {
	
	// Deallocate surface
	SDL_FreeSurface(image);
	image = NULL;

	// Don't worry about the surface here, Destroy window would take care of it
	/*SDL_FreeSurface(screenSurface);
	screenSurface = NULL;*/

	// Clean up window
	SDL_DestroyWindow(window);
	window = NULL;
	screenSurface = NULL;

	SDL_Quit();
}

int main( int argc, char* args[] )
{
	// Boot the exe
	if (!init()) {
		printf("Failed to Initialize.\n");
	}
	else {
		if (!loadMedia()) {
			printf("Failed to Load media.\n");
		}
		else {
			// A event variable to keep track of event
			SDL_Event event;
			bool quit = false;
			while (!quit) {
				// Application event tracking, Handle event on queue.
				// It keeps processing, retrieving event from the queue until it is empty
				while (SDL_PollEvent(&event)) {
					if (event.type == SDL_QUIT) {
						quit = true;
					}
				}
				// Apply the image
				SDL_BlitSurface(image, NULL, screenSurface, NULL);
				/**
					By LazyFoo :
					By default, most rendering systems out there are double buffered. These two buffers are the front and back buffer.
					When you make draw calls like SDL_BlitSurface, you render to the back buffer. What you see on the screen is the front buffer.
					The reason we do this is because most frames require drawing multiple objects to the screen.
					If we only had a front buffer, we would be able to see the frame as things are being drawn to it which means we would see unfinished frames.
					So what we do is draw everything to the back buffer first and once we're done we swap the back and front buffer so now the user can see the finished frame.
				*/
				// Put the image on the canvas from buffer, update the frame
				SDL_UpdateWindowSurface(window);
			}
		}
	}

	close();
	return 0;
}
