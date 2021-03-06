
/* LevelDecor.h
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

class Display;
class Layer;
class Level;
class SpriteMap;

/**
 * The renderer for a level.
 * @author Michael Imamura
 */
class LevelDecor {
public:
	LevelDecor(Display &display, std::shared_ptr<Level> level,
		std::shared_ptr<SpriteMap> defaultSprite);

private:
	void RenderLayer(const Layer &layer, size_t width, size_t height);
public:
	void Render();
	void RenderLayer(size_t index);

private:
	Display &display;
	std::shared_ptr<Level> level;
	std::shared_ptr<SpriteMap> defaultSprite;
};

}  // namespace AISDL
