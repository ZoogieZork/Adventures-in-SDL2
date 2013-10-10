
/* Ttf.cpp
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

#include "Exception.h"

#include "Ttf.h"

namespace AISDL {

/**
 * Constructor.
 * @param font The SDL_ttf font to wrap (may be nullptr).
 */
Ttf::Ttf(TTF_Font *font) :
	font(font)
{
}

Ttf::~Ttf()
{
	if (font) {
		TTF_CloseFont(font);
	}
}

/**
 * Load a font from a file.
 * @param filename The filename, relative to the current directory.
 * @param size The size, in points.
 * @return The loaded font.
 * @throws Exception The font could not be loaded.
 */
std::shared_ptr<Ttf> Ttf::Load(const std::string &filename, int size)
{
	TTF_Font *font = TTF_OpenFont(filename.c_str(), size);
	if (!font) {
		throw Exception(TTF_GetError());
	}

	return std::make_shared<Ttf>(font);
}

}  // namespace AISDL

