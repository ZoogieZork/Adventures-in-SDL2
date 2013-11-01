
/* PlayerDecor.cpp
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

#include "Display.h"
#include "Player.h"
#include "SpriteMap.h"

#include "PlayerDecor.h"

namespace AISDL {

/**
 * Constructor.
 * @param display The target display.
 * @param player The player model.
 * @param sprite The spritemap to use to render the player.
 */
PlayerDecor::PlayerDecor(Display &display, std::shared_ptr<Player> player,
	std::shared_ptr<SpriteMap> sprite) :
	display(display), player(player), sprite(sprite)
{
}

void PlayerDecor::Advance(Uint32 tick)
{
	auto p = player.lock();
	if (!p) return;

	//TODO: Animate walking.
}

void PlayerDecor::Render()
{
	auto p = player.lock();
	if (!p) return;

	int spriteIdx;
	switch (p->GetDirection()) {
	case Player::Direction::UP:    spriteIdx = 4;  break;
	case Player::Direction::RIGHT: spriteIdx = 16; break;
	case Player::Direction::DOWN:  spriteIdx = 0;  break;
	case Player::Direction::LEFT:  spriteIdx = 24; break;
	}

	//TODO: Apply world->screen transform for pos.
	sprite->Render(
		static_cast<int>(p->GetPosX()),
		static_cast<int>(p->GetPosY()),
		spriteIdx);
}

}  // namespace AISDL

