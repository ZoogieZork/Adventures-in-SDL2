
/* Player.cpp
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

#include "Player.h"

namespace AISDL {

Player::Player() :
	name("???"),
	posX(0), posY(0), direction(Direction::RIGHT)
{
}

void Player::SetName(const std::string &name)
{
	this->name = name;
}

void Player::SetPos(float x, float y)
{
	this->posX = x;
	this->posY = y;
}

/**
 * Move the player by the specified units.
 * The direction will be updated automatically.
 * @param dx The units to move horizontally.
 * @param dy The units to move vertically.
 */
void Player::Move(float dx, float dy)
{
	this->posX += dx;
	this->posY += dy;

	if (dx == 0) {
		if (dy < 0) {
			direction = Direction::UP;
		}
		else if (dy > 0) {
			direction = Direction::DOWN;
		}
	}
	else if (dx < 0) {
		direction = Direction::LEFT;
	}
	else {
		direction = Direction::RIGHT;
	}
}

/**
 * Retrieve the direction the player is facing.
 * @param direction The direction.
 */
void Player::SetDirection(Direction::type direction)
{
	this->direction = direction;
}

}  // namespace AISDL

