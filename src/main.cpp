
/*
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

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "App.h"
#include "Exception.h"

using namespace AISDL;

/// Initialize each of the SDL subsystems.
static void InitApp() {
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);

	SDL_Log("Starting up all SDL subsystems and libraries.");

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		throw Exception("SDL init", SDL_GetError());
	}

	int reqFmts = IMG_INIT_JPG | IMG_INIT_PNG;
	int actualFmts = IMG_Init(reqFmts);
	if ((actualFmts & reqFmts) != reqFmts) {
		throw Exception("SDL_image init", IMG_GetError());
	}

	if (TTF_Init() == -1) {
		throw Exception("SDL_ttf init", TTF_GetError());
	}
}

/// Shutdown each of the SDL subsystems.
static void ShutdownApp() {
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#else
int main(int argc, char** argv) {
#endif
	srand(static_cast<unsigned int>(time(nullptr)));

	try {
		InitApp();
		App().Run();
	}
	catch (Exception &ex) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"Adventures in SDL2", ex.what(), NULL);

		std::cerr << ex.what() << std::endl;

		return 1;
	}

	ShutdownApp();

	return 0;
}
