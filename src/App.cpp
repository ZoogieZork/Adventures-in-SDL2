
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
	startingScene(startingScene), sceneIdx(-1),
	clockDecor(display)
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
 * Attempt to attach a game controller.
 * @param idx The joystick index.
 * @return @c true if attached successfully, @c false otherwise.
 */
bool App::AttachController(int idx)
{
	if (!SDL_IsGameController(idx)) return false;

	SDL_GameController *controller = SDL_GameControllerOpen(idx);
	if (!controller) {
		SDL_Log("Could not open controller %d: %s", idx, SDL_GetError());
		return false;
	}

	gameControllers.push_back(controller);

	SDL_Log("Attached controller %d (%s).", idx,
		SDL_GameControllerName(controller));

	return true;
}

/**
 * Handle when a controller button is pressed.
 * @param evt The button press event.
 */
void App::OnControllerButtonDown(SDL_ControllerButtonEvent &evt)
{
	const SDL_GameControllerButton btn =
		static_cast<SDL_GameControllerButton>(evt.button);

	SDL_Log("Controller button pressed [%d]: %s", btn,
		SDL_GameControllerGetStringForButton(btn));

	switch (btn) {
	case SDL_CONTROLLER_BUTTON_A:
		scene->OnAction();
		break;
	case SDL_CONTROLLER_BUTTON_B:
		scene->OnCancel();
		break;
	case SDL_CONTROLLER_BUTTON_X:
		scene->OnInteract();
		break;
	case SDL_CONTROLLER_BUTTON_LEFTSTICK:
		ResStr::ReloadAll();
		break;
	case SDL_CONTROLLER_BUTTON_BACK:
		clockDecor.Flash();
		break;
	case SDL_CONTROLLER_BUTTON_START:
		//TODO: Switch to TOC (last scene in list).
		break;
	case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
		//TODO: Jump to previous scene.
		break;
	case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
		//TODO: Jump to next scene.
		break;
	default:
		// Ignore.
		break;
	}
}

/**
 * Handle when a key is pressed.
 * @param evt The key pressed event.
 */
void App::OnKeyDown(SDL_KeyboardEvent &evt)
{
	switch (evt.keysym.sym) {
	case SDLK_RETURN:
		if (evt.keysym.mod & KMOD_ALT) {
			// Alt+Enter - Toggle fullscreen.
			display.ToggleFullscreen();
		} else {
			scene->OnAction();
		}
		break;
	case SDLK_ESCAPE:
		scene->OnCancel();
		break;
	case SDLK_e:
		scene->OnInteract();
		break;
	case SDLK_F5:
		ResStr::ReloadAll();
		break;
	case SDLK_TAB:
		clockDecor.Flash();
		break;
	case SDLK_HOME:
		//TODO: Switch to TOC (last scene in list).
		break;
	case SDLK_PAGEUP:
		//TODO: Jump to previous scene.
		break;
	case SDLK_PAGEDOWN:
		//TODO: Jump to next scene.
		break;
	default:
		// Ignore.
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
	scene = std::make_shared<PreloadScene>(*this, display);

	// Attach all already-plugged-in controllers.
	for (int i = 0; i < SDL_NumJoysticks(); i++) {
		AttachController(i);
	}

	while (!quit) {
		// Process all events that have been triggered since the last
		// frame was rendered.
		while (SDL_PollEvent(&evt) && !quit) {
			switch (evt.type) {
			case SDL_KEYDOWN:
				OnKeyDown(evt.key);
				break;

			// SDL_ControllerButtonEvent
			case SDL_CONTROLLERBUTTONDOWN:
				OnControllerButtonDown(evt.cbutton);
				break;

			// SDL_ControllerDeviceEvent
			case SDL_CONTROLLERDEVICEADDED:
				AttachController(evt.cdevice.which);
				break;
			case SDL_CONTROLLERDEVICEREMOVED:
				//TODO: Detach the controller.
				SDL_Log("Detach controller: %d", evt.cdevice.which);
				break;
			case SDL_CONTROLLERDEVICEREMAPPED:
				//TODO
				SDL_Log("Remapped controller: %d", evt.cdevice.which);
				break;

			case SDL_QUIT:
				quit = true;
				break;
			}
			// Let the current scene have a chance at handling the event.
			scene->HandleEvent(evt);
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
	auto tick = SDL_GetTicks();
	scene.Advance(tick);
	clockDecor.Advance(tick);

	// We let the scene decide how to clear the frame.
	scene.Render();
	clockDecor.Render();

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

