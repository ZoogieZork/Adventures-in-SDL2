
/* Ttf.h
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

#include <SDL2/SDL_ttf.h>

namespace AISDL {

/**
 * Wrapper for SDL_ttf fonts.
 * @author Michael Imamura
 */
class Ttf {
public:
	Ttf(TTF_Font *font=nullptr);
	~Ttf();

public:
	static std::shared_ptr<Ttf> Load(const std::string &filename, int size);

private:
	TTF_Font *font;
};

}  // namespace AISDL

