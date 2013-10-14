
/* PreloadScene.cpp
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

#include "App.h"

#include "PreloadScene.h"

namespace AISDL {

PreloadScene::PreloadScene(App &director, Display &display) :
	SUPER(director, display),
	app(director), loading(false)
{
}

PreloadScene::~PreloadScene()
{
}

void PreloadScene::Advance(Uint32 tick)
{
	// Wait for the first frame to be rendered before preloading.
	if (loading) {
		//TODO: Preload incrementally.

		// Preload global assets.
		display.res.Preload(display);

		// Preload each of the scenes.
		app.ForEachScene([&](std::shared_ptr<Scene> &scene) {
			scene->Preload();
		});

		director.RequestNextScene();
	}
	else {
		loading = true;
		SDL_Log("Starting preload.");
	}
}

void PreloadScene::RenderContent()
{
	SDL_SetRenderDrawColor(display.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(display.renderer);
}

}  // namespace AISDL

