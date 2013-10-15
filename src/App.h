
/* App.h
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

#include "Director.h"

namespace AISDL {

class Scene;

/**
 * The main scene manager.
 * @author Michael Imamura
 */
class App : public Director {
	typedef Director SUPER;

public:
	App(int startingScene=0);
	~App();

private:
	void AddScene(std::shared_ptr<Scene> scene);
public:
	template<typename Fn>
	void ForEachScene(Fn fn) {
		std::for_each(scenes.begin(), scenes.end(), fn);
	}

private:
	void OnKeyDown(SDL_KeyboardEvent &evt);

public:
	void Run();
private:
	void RenderFrame(Scene &scene);

public:
	// Director
	virtual void RequestNextScene();
	virtual void RequestShutdown();

private:
	Display display;
	int startingScene;
	int sceneIdx;
	std::vector<std::shared_ptr<Scene>> scenes;
	std::shared_ptr<Scene> nextScene;
};

}  // namespace AISDL

