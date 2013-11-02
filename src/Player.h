
/* Player.h
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

#pragma once

namespace AISDL {

/**
 * A player character, controlled by the user.
 * @author Michael Imamura
 */
class Player {
public:
	Player();

public:
	void SetName(const std::string &name);
	const std::string &GetName() const { return name; }

	void SetPos(float x, float y);
	float GetPosX() const { return posX; }
	float GetPosY() const { return posY; }

private:
	std::string name;
	float posX, posY;
};

}  // namespace AISDL

