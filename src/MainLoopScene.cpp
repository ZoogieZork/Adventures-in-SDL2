
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

#include "Level.h"
#include "LevelDecor.h"
#include "PagedTextDecor.h"
#include "Player.h"
#include "ResStr.h"
#include "SpriteMap.h"

#include "MainLoopScene.h"

namespace AISDL {

MainLoopScene::MainLoopScene(Director &director, Display &display) :
	SUPER(director, display, "The Main Loop")
{
}

MainLoopScene::~MainLoopScene()
{
}

void MainLoopScene::OnWalkOffEdgeLeft(std::shared_ptr<Player> player)
{
	if (levelLoop == 0) {
		//TODO: Trigger "Zigged Instead of Zagged" achievement.
		player->SetPos(-31, player->GetPosY());
	}
	else {
		if (levelLoop == 3) {
			level->HideLayer(1);
		}
		levelLoop--;
		player->SetPos(511, player->GetPosY());
	}
}

void MainLoopScene::OnWalkOffEdgeRight(std::shared_ptr<Player> player)
{
	if (levelLoop == 3) {
		player->SetPos(511, player->GetPosY());
	}
	else {
		levelLoop++;
		if (levelLoop == 3) {
			level->ShowLayer(1);
		}
		player->SetPos(-31, player->GetPosY());
	}
}

void MainLoopScene::OnAction()
{
	if (!eventTxt->NextPage(PagedTextDecor::Anim::FLING_UP)) {
		director.RequestNextScene();
	}
}

void MainLoopScene::Preload()
{
	SUPER::Preload();

	level = Level::Load(display.res.resDir + "/levels/mainloop");
	levelDecor.reset(new LevelDecor(display, level, display.res.interiorTile));

	const std::string dir = display.res.resDir + "/text/mainloop/";
	eventTxt.reset(new PagedTextDecor(display, display.res.bodyFont,
		ResStr::Load(dir + "mainloop.txt"), 864));
}

void MainLoopScene::Reload()
{
	SUPER::Reload();

	level->Reload();
}

void MainLoopScene::Reset()
{
	SUPER::Reset();

	levelLoop = 0;
	level->ShowAllLayers();
	level->HideLayer(1);

	eventTxt->FirstPage(PagedTextDecor::Anim::FLING_UP);

	auto player = director.GetMainPlayer();
	player->SetPos(40, 320);
}

void MainLoopScene::Advance(Uint32 lastTick, Uint32 tick)
{
	SUPER::Advance(lastTick, tick);

	eventTxt->Advance(tick);
}

void MainLoopScene::RenderContent()
{
	SDL_SetRenderDrawColor(display.renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(display.renderer);

	display.SetLowRes();
	levelDecor->Render();

	SUPER::RenderContent();

	display.SetHighRes();
	eventTxt->Render(80, 40);
}

}  // namespace AISDL
