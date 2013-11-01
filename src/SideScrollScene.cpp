
/* SideScrollScene.cpp
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

#include "PlayerDecor.h"

#include "SideScrollScene.h"

namespace AISDL {

SideScrollScene::SideScrollScene(Director &director, Display &display,
	const std::string &title) :
	SUPER(director, display, title)
{
}

SideScrollScene::~SideScrollScene()
{
}

void SideScrollScene::Reset()
{
	auto player = director.GetMainPlayer();
	playerDecor.reset(new PlayerDecor(display, player,
		display.res.playerSprite));
}

void SideScrollScene::Advance(Uint32 tick)
{
}

void SideScrollScene::RenderContent()
{
	display.SetLowRes();

	playerDecor->Render();
}

}  // namespace AISDL
