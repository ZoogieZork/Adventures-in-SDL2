
/* FinalScene.cpp
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

#include "FinalScene.h"

namespace AISDL {

FinalScene::FinalScene(Director &director, Display &display) :
	SUPER(director, display),
	creditsTxt(nullptr)
{
}

FinalScene::~FinalScene()
{
	if (creditsTxt) SDL_DestroyTexture(creditsTxt);
}

void FinalScene::Advance(Uint32 tick)
{
	if (!creditsTxt) {
		creditsTxt = display.res.pixelFont->Texture(display,
			"This is test text.");
	}
}

void FinalScene::RenderContent()
{
	SDL_SetRenderDrawColor(display.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(display.renderer);

	if (creditsTxt) {
		display.RenderTexture(creditsTxt, 0, 0);
	}
}

}  // namespace AISDL

