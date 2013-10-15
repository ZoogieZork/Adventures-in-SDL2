
/* App.cpp
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
#include "FinalScene.h"
#include "PreloadScene.h"
#include "ResStr.h"
#include "Scene.h"

#include "App.h"

namespace AISDL {

/**
 * Constructor.
 * @param startingScene Index of the scene to start.
 */
App::App(int startingScene) :
	SUPER(),
	display(),
	startingScene(startingScene), sceneIdx(-1)
{
	AddScene(std::make_shared<FinalScene>(*this, display));
}

App::~App()
{
}

/**
 * Add a scene to the end of the scene list.
 * @param scene The scene (may not be @c nullptr).
 */
void App::AddScene(std::shared_ptr<Scene> scene)
{
	scenes.emplace_back(std::move(scene));
}

/**
 * Handle when a key is pressed.
 * @param evt The key pressed event.
 */
void App::OnKeyDown(SDL_KeyboardEvent &evt)
{
	switch (evt.keysym.sym) {
	case SDLK_F5:
		ResStr::ReloadAll();
		break;
	}
}

/**
 * Main loop.
 */
void App::Run()
{
	bool quit = false;
	SDL_Event evt;

	// Always start with the preload scene.
	// When the preloader finishes, it'll request to switch to the next scene.
	std::shared_ptr<Scene> scene =
		std::make_shared<PreloadScene>(*this, display);

	while (!quit) {
		while (SDL_PollEvent(&evt) && !quit) {
			switch (evt.type) {
			case SDL_KEYDOWN:
				OnKeyDown(evt.key);
				break;

			case SDL_QUIT:
				quit = true;
				break;
			}
		}
		if (quit) break;

		RenderFrame(*scene);

		// If a new scene was requested, switch to it.
		if (nextScene) {
			scene = nextScene;
			nextScene.reset();
		}
	}

	SDL_Log("Shutting down.");
}

void App::RenderFrame(Scene &scene)
{
	scene.Advance(SDL_GetTicks());

	// We let the scene decide how to clear the frame.
	scene.Render();
	SDL_RenderPresent(display.renderer);
}

void App::RequestNextScene()
{
	if (sceneIdx == -1) {
		// The scene at -1 is the preload scene; it's followed by the
		// requested starting scene.
		sceneIdx = startingScene;
		if (startingScene < 0 || startingScene >= (int)scenes.size()) {
			std::ostringstream oss;
			oss << "Invalid starting scene index [" << startingScene << "] -- "
				"Starting scene must be in range 0.." << (scenes.size() - 1) <<
				" (inclusive)";
			throw Exception(oss.str());
		}
	}
	else {
		sceneIdx++;
	}

	SDL_Log("Switching to scene: %d", sceneIdx);

	nextScene = scenes[sceneIdx];
}

void App::RequestShutdown()
{
	// Handle the shutdown in a calm and orderly fashion, when the
	// main loop gets around to it.
	SDL_Event evt;
	evt.type = SDL_QUIT;
	SDL_PushEvent(&evt);
}

}  // namespace AISDL

