
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

#include "Config.h"
#include "Player.h"
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

void SideScrollScene::Advance(Uint32 lastTick, Uint32 tick)
{
	const Config &cfg = Config::instance;

	auto move = director.SampleMovement();
	
	auto player = director.GetMainPlayer();

	float timeDiff = static_cast<float>(tick - lastTick);
	float dx = 0;
	if (timeDiff != 0 && move.x != 0) {
		dx = static_cast<float>(cfg.playerSpeed) *
			move.x * (timeDiff / 1000.0f);

		//TODO: Collision detection.
		player->Move(dx, 0);

		float px = player->GetPosX();
		if (px <= -32) {
			OnWalkOffEdgeLeft(player);
		}
		else if (px >= 512) {
			OnWalkOffEdgeRight(player);
		}
	}

	lastTick = tick;
}

void SideScrollScene::RenderContent()
{
	display.SetLowRes();

	playerDecor->Render();
}

}  // namespace AISDL
