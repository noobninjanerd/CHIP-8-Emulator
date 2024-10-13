#pragma once

#include <cstdint>
#include <iostream>
#include <SDL2/SDL.h>

class platform_class
{
private:
	SDL_Window* window{};
	SDL_Renderer* renderer{};
	SDL_Texture* texture{};
public:
	platform_class(char const* title, int window_w, int window_h, int texture_w, int texture_h);	// constructor
	~platform_class();										// destructor
	void Update(void const* buffer, int videoPitch);
	bool ProcessInput(uint8_t* keys);
};
