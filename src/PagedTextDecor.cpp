
/* PagedTextDecor.cpp
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

#include "FmtTextDecor.h"
#include "ResStr.h"

#include "PagedTextDecor.h"

namespace AISDL {

/**
 * Constructor.
 * @param display The target display.
 * @param font The font to use to render the text.
 * @param text The text itself.
 * @param width The maximum width of the text area (currently unused).
 * @param cursor Display a cursor after the rendered text.
 */
PagedTextDecor::PagedTextDecor(Display &display, std::shared_ptr<Ttf> font,
	std::shared_ptr<ResStr> text, int width, bool cursor) :
	display(display), font(font), text(text), width(width), cursor(cursor),
	pageNum(0),
	animating(false), animStart(0), animProgress(0)
{
	Rebuild();

	// Automatically rebuild the pages when text changes.
	this->text->SetOnReload([&]() {
		Rebuild();
	});
}

/**
 * Regenerate the renderables when the source text changes.
 */
void PagedTextDecor::Rebuild()
{
	const auto &tpages = text->pages;

	pages.clear();

	for (auto iter = tpages.cbegin(); iter != tpages.cend(); ++iter) {
		pages.emplace_back(std::make_shared<FmtTextDecor>(display, font,
			*iter, width, cursor));
	}

	numPages = tpages.size();
	if (pageNum >= numPages) {
		pageNum = numPages - 1;
	}
}

/**
 * Jump to the first page.
 * @param animate @c true to animate the display of the page.
 */
void PagedTextDecor::FirstPage(bool animate)
{
	animating = animate;
	animStart = SDL_GetTicks();

	pageNum = 0;
}

/**
 * Advance to the next page.
 * @param animate @c true to animate the display of the page.
 * @return @c true if the page changed, @c false if already at the end.
 */
bool PagedTextDecor::NextPage(bool animate)
{
	animating = animate;
	animStart = SDL_GetTicks();

	if (pageNum < numPages - 1) {
		pageNum++;
		return true;
	}
	else {
		return false;
	}
}

/**
 * Return to the previous page.
 * @param animate @c true to animate the display of the page.
 * @return @c true if the page changed, @c false if already at the first page.
 */
bool PagedTextDecor::PrevPage(bool animate)
{
	animating = animate;
	animStart = SDL_GetTicks();

	if (pageNum > 0) {
		pageNum--;
		return true;
	}
	else {
		return false;
	}
}

void PagedTextDecor::Advance(Uint32 tick)
{
	if (animating) {
		pages[pageNum]->SetCursorVisible(cursor);
		animProgress = (tick - animStart) / 10;
		if (animProgress >= (*text)[pageNum].size()) {
			animating = false;
		}
	}
	else {
		// Blink the cursor.
		pages[pageNum]->SetCursorVisible(cursor && ((tick % 1000) < 500));
	}
}

/**
 * Render the current page.
 * @param x The X coordinate.
 * @param y The Y coordinate.
 * @param alpha The opacity (0 is fully transparent, 255 is fully opaque).
 */
void PagedTextDecor::Render(int x, int y, int alpha) const
{
	pages[pageNum]->Render(x, y, alpha, animating ? animProgress : UINT_MAX);
}

}
