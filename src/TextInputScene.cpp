
/* TextInputScene.cpp
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

#include "FmtTextDecor.h"
#include "Level.h"
#include "LevelDecor.h"
#include "PagedTextDecor.h"
#include "Player.h"
#include "ResStr.h"
#include "SpriteMap.h"

#include "TextInputScene.h"

namespace AISDL {

TextInputScene::TextInputScene(Director &director, Display &display) :
	SUPER(director, display, "Introduction")
{
}

TextInputScene::~TextInputScene()
{
}

void TextInputScene::UpdatePlayerName()
{
	playerNameTxt->SetText(playerName);
}

void TextInputScene::OnKeyDown(SDL_KeyboardEvent &evt)
{
	if (evt.keysym.sym == SDLK_BACKSPACE) {
		if (!playerName.empty()) {
			// Find the last UTF-8 character sequence and remove it.
			auto iter = playerName.end();
			utf8::prior(iter, playerName.begin());
			playerName.erase(iter, playerName.end());

			UpdatePlayerName();
		}
	}
}

/**
 * Handle when the OS-native text input appends text.
 * @param evt The event.
 */
void TextInputScene::OnTextInput(SDL_TextInputEvent &evt)
{
	playerName += evt.text;
	UpdatePlayerName();
}

/**
 * Handle when the OS-native composition system changes.
 * @param evt The event.
 */
void TextInputScene::OnTextEditing(SDL_TextEditingEvent &evt)
{
	//TODO
	// This can get a bit... interesting, so we're leaving it out for now.
	// See: http://wiki.libsdl.org/Tutorials/TextInput
}

void TextInputScene::OnWalkOffEdgeLeft(std::shared_ptr<Player> player)
{
	player->SetPos(511, player->GetPosY());
}

void TextInputScene::OnWalkOffEdgeRight(std::shared_ptr<Player> player)
{
	player->SetPos(-31, player->GetPosY());
}

void TextInputScene::OnAction()
{
	director.RequestNextScene();
}

void TextInputScene::Preload()
{
	SUPER::Preload();

	level = Level::Load(display.res.resDir + "/levels/intro");
	levelDecor.reset(new LevelDecor(display, level, display.res.interiorTile));

	const std::string dir = display.res.resDir + "/text/textinput/";
	introTxt.reset(new PagedTextDecor(display, display.res.pixelFont,
		ResStr::Load(dir + "intro.txt"), 432));
}

void TextInputScene::Reload()
{
	SUPER::Reload();

	level->Reload();
}

void TextInputScene::Reset()
{
	SUPER::Reset();

	//FIXME: The current pixelFont does not have some of the Unicode characters
	//       we'd like to have.  
	playerName = "Ulric of Cogito";
	playerNameTxt.reset(new FmtTextDecor(display, display.res.pixelFont,
		playerName, 512));

	introTxt->FirstPage(PagedTextDecor::Anim::TYPEWRITER);

	auto player = director.GetMainPlayer();
	player->SetDirection(Player::Direction::DOWN);
	player->SetPos(256 - 16, 384 / 3);  // Centered on the screen.

	SDL_Log("Starting Unicode text input.");
	SDL_StartTextInput();
}

void TextInputScene::Cleanup()
{
	playerNameTxt.reset();

	SDL_Log("Stopping Unicode text input.");
	SDL_StopTextInput();
}

void TextInputScene::HandleEvent(SDL_Event &evt)
{
	switch (evt.type) {
	case SDL_KEYDOWN:
		OnKeyDown(evt.key);
		break;
	case SDL_TEXTINPUT:
		OnTextInput(evt.text);
		break;
	case SDL_TEXTEDITING:
		OnTextEditing(evt.edit);
		break;
	}
}

void TextInputScene::Advance(Uint32 lastTick, Uint32 tick)
{
	SUPER::Advance(lastTick, tick);

	introTxt->Advance(tick);

	playerNameCursor = (tick % 1000) < 500;
}

void TextInputScene::RenderContent()
{
	SDL_SetRenderDrawColor(display.renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(display.renderer);

	display.SetLowRes();
	levelDecor->Render();

	SUPER::RenderContent();

	display.SetLowRes();

	int introTxtWidth = introTxt->MeasureWidth();
	introTxt->Render(256 - (introTxtWidth / 2), 40);

	// Draw the text input area.
	int playerNameTxtWidth = playerNameTxt->MeasureWidth();
	playerNameTxt->Render(256 - (playerNameTxtWidth / 2), 80, 255,
		playerNameCursor);
}

}  // namespace AISDL

