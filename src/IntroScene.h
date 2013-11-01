
/* IntroScene.h
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

#include "SideScrollScene.h"

namespace AISDL {

class PagedTextDecor;
class PlayerDecor;

/**
 * All about the main event loop.
 * @author Michael Imamura
 */
class IntroScene : public SideScrollScene {
	typedef SideScrollScene SUPER;
public:
	IntroScene(Director &director, Display &display);
	virtual ~IntroScene();

public:
	// Scene
	virtual void OnAction();

	virtual void Preload();
	virtual void Reset();
	virtual void Advance(Uint32 tick);
	virtual void RenderContent();

private:
	std::unique_ptr<PagedTextDecor> introTxt;
};

}  // namespace AISDL

