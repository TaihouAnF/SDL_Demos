// Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

// Screen dimension constants
const int screen_width = 1000;
const int screen_height = 1000;

// Enum constants for key press
enum KeyPress {
	KEY_DEFAULT,	// 0
	KEY_UP,			// 1
	KEY_DOWN,		// 2
	KEY_LEFT,		// 3
	KEY_RIGHT,		// 4
	KEY_TOTAL_AMOUNT// 5
};

// A collection of string for pictures, only for this exercise
std::vector<std::string> paths = {
	"temp/test_0.bmp",
	"temp/test_1.bmp",
	"temp/test_2.bmp",
	"temp/test_3.bmp",
	"temp/test_4.bmp"
};

// Starts SDL
bool init();

/**
 * Load media files to the program.
 *
 * @return boolean indicating whether loaded succesfully.
 * 
 */
bool loadMedia();

// Free Memory and 
void close();

/**
 * Load bmp file into a SDL surface.
 * 
 * @param string that represent the path of the file.
 * @return pointer to the SDL surface we just loaded, 
 *		or Nullptr if failed to load.
 */
SDL_Surface* loadSurface(std::string path);

// The window we'll be rendering to
SDL_Window* window = NULL;

// The surface/canvas we put into the window
SDL_Surface* screenSurface = NULL;

// An array for storing images 
SDL_Surface* keyPressSurfaces[KEY_TOTAL_AMOUNT];

// The image we currently see
SDL_Surface* currentImage = NULL;

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

SDL_Surface* loadSurface(std::string path) {
	// Load the surface like we did previously, 
	// we need c_str here because SDL only supports C style string.
	SDL_Surface* loaded = SDL_LoadBMP(path.c_str());

	if (loaded == nullptr) {
		std::cout << "SDL failed to load BMP, exit with code: " << SDL_GetError() << ".\n";
	}
	return loaded;
}

bool loadMedia() {
	// Similar to init(), we need a flag here
	bool success = true;

	// Iterating through the array and assign each of them a picture,
	// paths is the array of string for path of images
	for (int i = 0; i < KEY_TOTAL_AMOUNT; ++i) {
		keyPressSurfaces[i] = loadSurface(paths[i]);
		if (keyPressSurfaces[i] == NULL) {
			std::cout << "SDL failed to load BMP, exit with code: " << SDL_GetError() << ".\n";
			success = false;
			break;
		}
	}

	return success;
}

void close() {
	
	// Deallocate surfaces
	for (int i = 0; i < KEY_TOTAL_AMOUNT; ++i) {
		SDL_FreeSurface(keyPressSurfaces[i]);
		keyPressSurfaces[i] = NULL;
	}
	
	// Don't worry about the surface here, Destroy window would take care of it
	/*SDL_FreeSurface(screenSurface); screenSurface = NULL;*/

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
			currentImage = keyPressSurfaces[KEY_DEFAULT];
			bool quit = false;
			while (!quit) {
				// Application event tracking, Handle event on queue.
				// It keeps processing, retrieving event from the queue until it is empty
				while (SDL_PollEvent(&event)) {
					if (event.type == SDL_QUIT) {
						quit = true;
					}
					else if (event.type == SDL_KEYDOWN) {
						// Pressed a Key
						switch (event.key.keysym.sym) {
							case SDLK_UP:
								currentImage = keyPressSurfaces[KEY_UP];
								break;

							case SDLK_DOWN:
								currentImage = keyPressSurfaces[KEY_DOWN];
								break;

							case SDLK_LEFT:
								currentImage = keyPressSurfaces[KEY_LEFT];
								break;

							case SDLK_RIGHT:
								currentImage = keyPressSurfaces[KEY_RIGHT];
								break;

							default:
								currentImage = keyPressSurfaces[KEY_DEFAULT];
								break;	
						}
					}
				}
				// Apply the image
				SDL_BlitSurface(currentImage, NULL, screenSurface, NULL);
				
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
