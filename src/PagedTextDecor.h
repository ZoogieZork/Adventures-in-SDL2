
/* PagedTextDecor.h
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

#include "Display.h"
#include "Ttf.h"

namespace AISDL {

class FmtTextDecor;
class ResStr;

/**
 * A formatted text renderable that spans multiple pages.
 * @author Michael Imamura
 * @see FmtTextDecor
 */
class PagedTextDecor {
public:
	PagedTextDecor(Display &display, std::shared_ptr<Ttf> font,
		std::shared_ptr<ResStr> text, int width, bool cursor=false);

private:
	void Rebuild();

public:
	void FirstPage(bool animate=false);
	bool NextPage(bool animate=false);
	bool PrevPage(bool animate=false);
	void Advance(Uint32 tick);
	void Render(int x, int y, int alpha=0xff) const;

private:
	Display &display;
	std::shared_ptr<Ttf> font;
	std::shared_ptr<ResStr> text;
	std::vector<std::shared_ptr<const FmtTextDecor>> pages;
	int width;
	bool cursor;
	bool cursorVisible;

	size_t numPages;
	unsigned int pageNum;

	bool animating;
	Uint32 animStart;
	unsigned int animProgress;
};

}
