
/* ImageScene.cpp
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

#include "Conversation.h"
#include "Level.h"
#include "LevelDecor.h"
#include "PagedTextDecor.h"
#include "Player.h"
#include "ResStr.h"
#include "SpriteMap.h"

#include "ImageScene.h"

namespace AISDL {

ImageScene::ImageScene(Director &director, Display &display) :
	SUPER(director, display, "SDL_image")
{
}

ImageScene::~ImageScene()
{
}

void ImageScene::OnWalkOffEdgeLeft(std::shared_ptr<Player> player)
{
	player->SetPos(511, player->GetPosY());
}

void ImageScene::OnWalkOffEdgeRight(std::shared_ptr<Player> player)
{
	player->SetPos(-31, player->GetPosY());
}

void ImageScene::OnInteract()
{
	convo->Next();
}

void ImageScene::OnAction()
{
	director.RequestNextScene();
}

void ImageScene::Preload()
{
	SUPER::Preload();

	level = Level::Load(display.res.resDir + "/levels/image");
	levelDecor.reset(new LevelDecor(display, level, display.res.interiorTile));

	const std::string dir = display.res.resDir + "/text/image/";
	convo.reset(new Conversation(ResStr::Load(dir + "dialog.txt")));
}

void ImageScene::Reload()
{
	SUPER::Reload();

	level->Reload();
}

void ImageScene::Reset()
{
	SUPER::Reset();

	auto player = director.GetMainPlayer();
	player->SetPos(40, 320);
	convo->Start(player);
}

void ImageScene::Advance(Uint32 lastTick, Uint32 tick)
{
	SUPER::Advance(lastTick, tick);

	//TODO: Fade-in paintings.
}

void ImageScene::RenderContent()
{
	SDL_SetRenderDrawColor(display.renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(display.renderer);

	display.SetLowRes();
	levelDecor->Render();

	SUPER::RenderContent();
}

}  // namespace AISDL
