#include "platform_class.hpp"
#include <SDL2/SDL.h>

platform_class::platform_class(char const* title, int window_w, int window_h, int texture_w, int texture_h)
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(title,
				       SDL_WINDOWPOS_CENTERED,
				       SDL_WINDOWPOS_CENTERED,
				       window_w,
				       window_h,
				       SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture	 = SDL_CreateTexture(renderer,
				     SDL_PIXELFORMAT_RGBA8888,
				     SDL_TEXTUREACCESS_STREAMING,
				     texture_w,
				     texture_h);
}

platform_class::~platform_class()
{
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void platform_class::Update(void const* buffer, int videoPitch)
{
	SDL_UpdateTexture(texture,	// The SDL texture that is being updated.	
			  nullptr,	// This would normally be a rectangle that defines the region of the texture to update. 
			  		// Passing nullptr means the entire texture is updated.
			  buffer,	// The pointer to the pixel data that will be copied into the texture. 
			  		// This is the image or frame that you want to display.
			  videoPitch);  // The number of bytes per row of pixel data in the buffer. This ensures the 
			  		// function reads the buffer correctly, especially when there's padding or alignment in the data.
	SDL_RenderClear(renderer);	// Clears the rendering target (the window) with the current drawing color.
					// If you don't call this, artifacts from the previous frames might still appear.
	SDL_RenderCopy(renderer,	// The SDL renderer that will perform the copy operation.
		       texture,		// The texture that contains the pixel data (updated earlier with SDL_UpdateTexture).
		       nullptr,		// the source rectangle: defines the part of the texture to copy. nullptr means it will copy the entire texture.
		       nullptr);	// destination rectangle: defines where the texture will be placed on the screen. 
		       			// nullptr means the texture will be stretched to cover the entire rendering target (the window).	
	SDL_RenderPresent(renderer); 	// Swaps the back buffer (where the rendering operations happen) to the front buffer (what is shown to the user). 
					// This is how double-buffering works in graphics applications, ensuring smooth rendering without flickering.

}

bool platform_class::ProcessInput(uint8_t* keys)
{
	bool running = true;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{	
			case SDL_QUIT:
				std::cout << "Goodbye!" << std::endl;
				running = false;
				break;
		
			case SDL_KEYDOWN:
			
				switch(e.key.keysym.sym)
				{
					case SDLK_ESCAPE:
					std::cout << "Escape Key Pressed" << std::endl;
					running = false;
					break;

					case SDLK_x:
					keys[0] = 1;
					break;

					case SDLK_1:
					keys[1] = 1;
					break;

					case SDLK_2:
					keys[2] = 1;
					break;

					case SDLK_3:
					keys[3] = 1;
					break;

					case SDLK_q:
					keys[4] = 1;
					break;

					case SDLK_w:
					keys[5] = 1;
					break;

					case SDLK_e:
					keys[6] = 1;
					break;

					case SDLK_a:
					keys[7] = 1;
					break;

					case SDLK_s:
					keys[8] = 1;
					break;

					case SDLK_d:
					keys[9] = 1;
					break;

					case SDLK_z:
					keys[0xA] = 1;
					break;

					case SDLK_c:
					keys[0xB] = 1;
					break;

					case SDLK_4:
					keys[0xC] = 1;
					break;

					case SDLK_r:
					keys[0xD] = 1;
					break;

					case SDLK_f:
					keys[0xE] = 1;
					break;

					case SDLK_v:
					keys[0xF] = 1;
					break;
				}
			case SDL_KEYUP:
				switch(e.key.keysym.sym)
				{
					case SDLK_x:
					keys[0] = 0;
					break;

					case SDLK_1:
					keys[1] = 0;
					break;

					case SDLK_2:
					keys[2] = 0;
					break;

					case SDLK_3:
					keys[3] = 0;
					break;

					case SDLK_q:
					keys[4] = 0;
					break;

					case SDLK_w:
					keys[5] = 0;
					break;

					case SDLK_e:
					keys[6] = 0;
					break;

					case SDLK_a:
					keys[7] = 0;
					break;

					case SDLK_s:
					keys[8] = 0;
					break;

					case SDLK_d:
					keys[9] = 0;
					break;

					case SDLK_z:
					keys[0xA] = 0;
					break;

					case SDLK_c:
					keys[0xB] = 0;
					break;

					case SDLK_4:
					keys[0xC] = 0;
					break;

					case SDLK_r:
					keys[0xD] = 0;
					break;

					case SDLK_f:
					keys[0xE] = 0;
					break;

					case SDLK_v:
					keys[0xF] = 0;
					break;
				}
				break;
		}
	}
	return running;
}







