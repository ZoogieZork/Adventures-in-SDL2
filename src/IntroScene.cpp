
/* IntroScene.cpp
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

#include "PagedTextDecor.h"
#include "ResStr.h"

#include "IntroScene.h"

namespace AISDL {

IntroScene::IntroScene(Director &director, Display &display) :
	SUPER(director, display, "Introduction")
{
}

IntroScene::~IntroScene()
{
}

void IntroScene::OnAction()
{
	if (!introTxt->NextPage(true)) {
		director.RequestNextScene();
	}
}

void IntroScene::Preload()
{
	const std::string dir = display.res.resDir + "/text/intro/";
	introTxt.reset(new PagedTextDecor(display, display.res.pixelFont,
		ResStr::Load(dir + "intro.txt"), 560, true));
}

void IntroScene::Reset()
{
	introTxt->FirstPage(true);
}

void IntroScene::Advance(Uint32 tick)
{
	introTxt->Advance(tick);
}

void IntroScene::RenderContent()
{
	SDL_SetRenderDrawColor(display.renderer, 0x00, 0x00, 0x1f, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(display.renderer);

	introTxt->Render(40, 40);
}

}  // namespace AISDL

