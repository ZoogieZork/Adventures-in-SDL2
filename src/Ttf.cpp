
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

#include "utf8/utf8.h"

#include "Display.h"
#include "Exception.h"

#include "Ttf.h"

namespace AISDL {

namespace {
	const SDL_Color COL_WHITE = { 0xff, 0xff, 0xff, 0xff };

	const SDL_Color FMT_COLORS[] = {
		{ 0x00, 0x00, 0x00, 0xff },  // 0   ^0
		{ 0xff, 0x00, 0x00, 0xff },  // 1   ^1
		{ 0x00, 0xff, 0x00, 0xff },  // 2   ^2
		{ 0xff, 0xff, 0x00, 0xff },  // 3   ^3
		{ 0x00, 0x00, 0xff, 0xff },  // 4   ^4
		{ 0x00, 0xff, 0xff, 0xff },  // 5   ^5
		{ 0xff, 0x00, 0xff, 0xff },  // 6   ^6
		{ 0xff, 0xff, 0xff, 0xff },  // 7   ^7  default white
		{ 0xff, 0x7f, 0x00, 0xff },  // 8   ^8
		{ 0x7f, 0x7f, 0x7f, 0xff },  // 9   ^9
		{ 0xbf, 0xbf, 0xbf, 0xff },  // 10  ^:
		{ 0xbf, 0xbf, 0xbf, 0xff },  // 11  ^;
		{ 0x00, 0x7f, 0x00, 0xff },  // 12  ^<
		{ 0x7f, 0x7f, 0x00, 0xff },  // 13  ^=
		{ 0x00, 0x00, 0x7f, 0xff },  // 14  ^>
		{ 0x7f, 0x00, 0x00, 0xff },  // 15  ^?
		{ 0x7f, 0x3f, 0x00, 0xff },  // 16  ^@
		{ 0xff, 0x99, 0x19, 0xff },  // 17  ^a
		{ 0x00, 0x7f, 0x7f, 0xff },  // 18  ^b
		{ 0x7f, 0x00, 0x7f, 0xff },  // 19  ^c
		{ 0x00, 0x7f, 0xff, 0xff },  // 20  ^d
		{ 0x7f, 0x00, 0xff, 0xff },  // 21  ^e
		{ 0x33, 0x99, 0xcc, 0xff },  // 22  ^f  external link
		{ 0xcc, 0xff, 0xcc, 0xff },  // 23  ^g
		{ 0x00, 0x66, 0x33, 0xff },  // 24  ^h
		{ 0xff, 0x00, 0x33, 0xff },  // 25  ^i
		{ 0xb2, 0x19, 0x19, 0xff },  // 26  ^j
		{ 0x99, 0x33, 0x00, 0xff },  // 27  ^k
		{ 0xcc, 0x99, 0x33, 0xff },  // 28  ^l
		{ 0x99, 0x99, 0x33, 0xff },  // 29  ^m
		{ 0xff, 0xff, 0xbf, 0xff },  // 30  ^n
		{ 0xff, 0xff, 0x7f, 0xff },  // 31  ^o
	};
}

/**
 * Constructor.
 * @param display The display that will be rendered to.
 * @param font The SDL_ttf font to wrap (may be nullptr).
 * @param size The size of the font, in points (only tracked for descriptive
 *             purposes).
 */
Ttf::Ttf(Display &display, TTF_Font *font, int size) :
	font(font), size(size), display(display),
	typeCase(nullptr), glyphs(256)
{
	InitTypeCase();
}

Ttf::~Ttf()
{
	if (typeCase) SDL_DestroyTexture(typeCase);
	if (font) TTF_CloseFont(font);
}

/**
 * Load a font from a file.
 * @param display The display that will be rendered to.
 * @param filename The filename, relative to the current directory.
 * @param size The size, in points.
 * @return The loaded font.
 * @throws Exception The font could not be loaded.
 */
std::shared_ptr<Ttf> Ttf::Load(Display &display, const std::string &filename,
	int size)
{
	TTF_Font *font = TTF_OpenFont(filename.c_str(), size);
	if (!font) {
		throw Exception(TTF_GetError());
	}

	return std::make_shared<Ttf>(display, font, size);
}

/**
 * Render a text string to a texture.
 *
 * The text is always rendered as white with a transparent background.
 * The color can be changed by using SDL_SetTextureColorMod and
 * SDL_SetTextureAlphaMod on the returned texture.
 *
 * @param display The current display.
 * @param s The UTF-8 string to render.
 * @return The rendered text.
 */
SDL_Texture *Ttf::Texture(const Display &display, const std::string &s)
{
	// First render the text as solid white.
	SDL_Surface *surface = TTF_RenderUTF8_Blended(font, s.c_str(), COL_WHITE);
	if (!surface) {
		throw Exception(TTF_GetError());
	}

	// Convert the surface to a hardware-accelerated texture.
	SDL_Texture *retv = SDL_CreateTextureFromSurface(
		display.renderer, surface);
	SDL_FreeSurface(surface);

	return retv;
}

/**
 * Render a text string to the current render target.
 *
 * This supports basic formatting using newlines and Quake-style color
 * codes (e.g. "This is ^1red ^7and ^3yellow"), based on the following
 * chart: http://wolfwiki.anime.net/index.php/Color_Codes
 *
 * @param display The target display.
 * @param x The X coordinate.
 * @param y The Y coordinate.
 * @param width The maximum width of the rendered text (current unused).
 * @param s The string to render.
 */
void Ttf::RenderText(const Display &display, int x, int y, int width,
	const std::string &s)
{
	//TODO: Word-wrapping.

	SDL_Rect destRect = { x, y, 0, 0 };
	int lineHeight = TTF_FontLineSkip(font);

	// Reset default color to white.
	SDL_SetTextureColorMod(typeCase, 0xff, 0xff, 0xff);

	for (auto iter = s.cbegin(), iend = s.cend(); iter != iend; ) {
		uint32_t ch32 = utf8::next(iter, iend);

		// Only handle characters which we have included in the type case.
		if (ch32 > glyphs.size()) {
			SDL_Log("Unrenderable code point: %d", ch32);
			continue;
		}
		Uint16 ch = static_cast<Uint16>(ch32);

		switch (ch) {
			case '\n':  // Newlines.
				destRect.x = x;
				destRect.y += lineHeight;
				continue;

			case '^':  // Color codes.
				if (iter == iend) {
					return;
				}
				else {
					uint32_t idx = utf8::next(iter, iend);

					// Allow "^^" as an escape sequence for "^".
					if (idx == '^') break;

					const SDL_Color &color = FMT_COLORS[(idx + 16) & 31];
					SDL_SetTextureColorMod(typeCase, color.r, color.g, color.b);
				}
				continue;

			case '\r':  // Ignore CRs.
				continue;
		}

		const Glyph &glyph = glyphs[ch];
		if (!glyph.avail) continue;

		destRect.w = glyph.texRect.w;
		destRect.h = glyph.texRect.h;
		if (SDL_RenderCopy(display.renderer, typeCase,
			&glyph.texRect, &destRect) < 0)
		{
			throw Exception(SDL_GetError());
		}

		destRect.x += glyph.layoutW;
	}
}

bool Ttf::AddGlyph(SDL_Surface *surface, Uint16 ch, int &x, int y,
	Ttf::Glyph &glyph)
{
	// If the font doesn't have the glyph, just bail -- the glyph's "avail"
	// flag will stay false.
	if (!TTF_GlyphIsProvided(font, ch)) {
		return true;
	}

	SDL_Surface *glyphSurface = TTF_RenderGlyph_Blended(font, ch, COL_WHITE);
	if (!glyphSurface) {
		std::ostringstream oss;
		oss << "Could not render glyph at code point " << ch <<
			" for font " << *this;
		throw Exception(oss.str(), TTF_GetError());
	}

	if (x + glyphSurface->w + 1 >= surface->w ||
		y + glyphSurface->h + 1 >= surface->h)
	{
		SDL_FreeSurface(glyphSurface);
		return false;
	}

	// Height and width of destination rect are ignored with SDL_BlitSurface.
	SDL_Rect destRect = { x + 1, y + 1, 0, 0 };

	// First draw the shadow.
	SDL_SetSurfaceBlendMode(glyphSurface, SDL_BLENDMODE_NONE);
	SDL_SetSurfaceColorMod(glyphSurface, 0, 0, 0);
	SDL_SetSurfaceAlphaMod(glyphSurface, 0x7f);
	if (SDL_BlitSurface(glyphSurface, nullptr, surface, &destRect) < 0) {
		SDL_FreeSurface(glyphSurface);
		std::ostringstream oss;
		oss << "Could not blit shadow for glyph at code point " << ch <<
			" for font " << *this;
		throw Exception(oss.str(), SDL_GetError());
	}

	// Next draw the white text on top.
	destRect.x--;
	destRect.y--;
	SDL_SetSurfaceBlendMode(glyphSurface, SDL_BLENDMODE_BLEND);
	SDL_SetSurfaceColorMod(glyphSurface, 0xff, 0xff, 0xff);
	SDL_SetSurfaceAlphaMod(glyphSurface, 0xff);
	if (SDL_BlitSurface(glyphSurface, nullptr, surface, &destRect) < 0) {
		SDL_FreeSurface(glyphSurface);
		std::ostringstream oss;
		oss << "Could not blit glyph at code point " << ch <<
			" for font " << *this;
		throw Exception(oss.str(), SDL_GetError());
	}

	// Update the glyph info.
	glyph.avail = true;
	glyph.texRect.x = x;
	glyph.texRect.y = y;
	glyph.texRect.w = glyphSurface->w + 1;
	glyph.texRect.h = glyphSurface->h + 1;
	glyph.layoutW = glyphSurface->w;
	glyph.layoutH = glyphSurface->h;

	// Advance the X coordinate.
	x += glyph.texRect.w;

	SDL_FreeSurface(glyphSurface);

	return true;
}

void Ttf::AddGlyphRange(SDL_Surface *surface, int &x, int &y, int lineHeight,
	Uint16 startCh, Uint16 endCh)
{
	for (Uint16 ch = startCh; ch <= endCh; ch++) {
		if (!AddGlyph(surface, ch, x, y, glyphs[ch])) {
			x = 0;
			y += lineHeight;
			if (y >= surface->h) {
				std::ostringstream oss;
				oss << "Not enough space for glyphs for font " << *this;
				throw Exception(oss.str());
			}
			ch--;
		}
	}
}

void Ttf::InitTypeCase()
{
	// Create the surface.  This will be our drawing area.
	SDL_Surface *surface = nullptr;
	try {
		surface = display.NewAlphaSurface(512, 512);

		// One extra pixel for the shadow, plus one extra pixel to be safe.
		int lineHeight = TTF_FontHeight(font) + 2;

		int x = 0, y = 0;
		AddGlyphRange(surface, x, y, lineHeight, 0x20, 0x7e);
		AddGlyphRange(surface, x, y, lineHeight, 0xa1, 0xff);

		typeCase = SDL_CreateTextureFromSurface(display.renderer, surface);
		if (!typeCase) {
			std::ostringstream oss;
			oss << "Failed to create texture for type case for font " << *this;
			throw Exception(oss.str(), SDL_GetError());
		}
		SDL_SetTextureColorMod(typeCase, 0xff, 0xff, 0xff);
		SDL_SetTextureAlphaMod(typeCase, 0xff);
	}
	catch (...) {
		if (surface) SDL_FreeSurface(surface);
		throw;
	}

	SDL_FreeSurface(surface);
}

}  // namespace AISDL

