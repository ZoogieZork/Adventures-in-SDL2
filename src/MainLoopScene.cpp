
/* MainLoopScene.cpp
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

#include "FmtTextDecor.h"
#include "ResStr.h"

#include "MainLoopScene.h"

namespace AISDL {

MainLoopScene::MainLoopScene(Director &director, Display &display) :
	SUPER(director, display, "The Main Loop")
{
}

MainLoopScene::~MainLoopScene()
{
}

void MainLoopScene::Preload()
{
}

void MainLoopScene::Advance(Uint32 lastTick, Uint32 tick)
{
}

void MainLoopScene::RenderContent()
{
	SDL_SetRenderDrawColor(display.renderer, 0x00, 0x3f, 0x00, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(display.renderer);
}

}  // namespace AISDL

