
/* TextInputScene.h
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

class Level;
class LevelDecor;
class PagedTextDecor;
class PlayerDecor;

/**
 * Give the main character a name via SDL Unicode text input.
 * @author Michael Imamura
 */
class TextInputScene : public SideScrollScene {
	typedef SideScrollScene SUPER;
public:
	TextInputScene(Director &director, Display &display);
	virtual ~TextInputScene();

private:
	void UpdatePlayerName();
		
	void OnKeyDown(SDL_KeyboardEvent &evt);
	void OnTextInput(SDL_TextInputEvent &evt);
	void OnTextEditing(SDL_TextEditingEvent &evt);

public:
	// SideScrollScene
	virtual void OnWalkOffEdgeLeft(std::shared_ptr<Player> player);
	virtual void OnWalkOffEdgeRight(std::shared_ptr<Player> player);

	// Scene
	virtual void OnAction();

	virtual void Preload();
	virtual void Reload();
	virtual void Reset();
	virtual void Cleanup();
	virtual void HandleEvent(SDL_Event &evt);
	virtual void Advance(Uint32 lastTick, Uint32 tick);
	virtual void RenderContent();

private:
	std::shared_ptr<Level> level;
	std::unique_ptr<LevelDecor> levelDecor;
	std::unique_ptr<PagedTextDecor> introTxt;
	std::unique_ptr<FmtTextDecor> playerNameTxt;
	std::string playerName;
	bool playerNameCursor;
};

}  // namespace AISDL

