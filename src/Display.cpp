
/* Display.cpp
 *
 * Copyright (C) 2013 Michael Imamura
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

#include "StdAfx.h"

#include "Exception.h"

#include "Display.h"

namespace AISDL {

Display::Display() :
	window(nullptr), renderer(nullptr)
{
	window = SDL_CreateWindow("Adventures in SDL2", 0, 0, 640, 480,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if (!window) {
		throw Exception(SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		throw Exception(SDL_GetError());
	}
}

Display::~Display()
{
	if (renderer) SDL_DestroyRenderer(renderer);
	if (window) SDL_DestroyWindow(window);
}

/**
 * Create a new 32-bit OpenGL-compatible surface with an alpha channel.
 * @param w The width of the surface.
 * @param h The height of the surface.
 * @return The created surface.
 * @throws Exception The surface could not be created.
 */
SDL_Surface *Display::NewAlphaSurface(int w, int h)
{
	// Adapted from http://wiki.libsdl.org/SDL_CreateRGBSurface
	Uint32 rmask, gmask, bmask, amask;
#	if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
#	else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
#	endif

	SDL_Surface *surface = SDL_CreateRGBSurface(0, w, h, 32,
		rmask, gmask, bmask, amask);
	if (!surface) {
		throw Exception(SDL_GetError());
	}
	return surface;
}

/**
 * Simply render a texture to the screen.
 *
 * This handles the simple case where the whole texture is to be
 * rendered to a specific position.
 *
 * @param texture The texture to render (may not be @c nullptr);
 * @param x The destination
 */
void Display::RenderTexture(SDL_Texture *texture, int x, int y)
{
	int w, h;
	SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
	SDL_Rect destRect = { x, y, w, h };
	SDL_RenderCopy(renderer, texture, nullptr, &destRect);
}

}  // namespace AISDL
