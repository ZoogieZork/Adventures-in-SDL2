
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

class Display;

/**
 * Wrapper for SDL_ttf fonts.
 * @author Michael Imamura
 */
class Ttf {
	friend std::ostream &operator<<(std::ostream &os, const Ttf &ttf);
public:
	Ttf(Display &display, TTF_Font *font=nullptr);
	~Ttf();

public:
	static std::shared_ptr<Ttf> Load(Display &display,
		const std::string &filename, int size);

public:
	SDL_Texture *Texture(const Display &display, const std::string &s);
	void RenderText(const Display &display, int x, int y, int width,
		const std::string &s);

private:
	struct Glyph {
		Glyph() : avail(false) { }

		bool avail;
		SDL_Rect texRect;
		int layoutW, layoutH;
	};
	bool AddGlyph(SDL_Surface *surface, Uint16 ch, int &x, int y,
		Ttf::Glyph &glyph);
	void AddGlyphRange(SDL_Surface *surface, int &x, int &y, int lineHeight,
		Uint16 startCh, Uint16 endCh);
	void InitTypeCase();

private:
	TTF_Font *font;
	Display &display;
public:
	SDL_Texture *typeCase;
	std::vector<Glyph> glyphs;
};

inline std::ostream &operator<<(std::ostream &os, const Ttf &ttf)
{
	const char *fontName = TTF_FontFaceFamilyName(ttf.font);
	os << (fontName ? fontName : "Unnamed Font");

	const char *styleName = TTF_FontFaceStyleName(ttf.font);
	if (styleName) {
		os << ' ' << styleName;
	}

	return os;
}

}  // namespace AISDL

