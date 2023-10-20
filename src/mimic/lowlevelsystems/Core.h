#pragma once
#include <lowlevelsystems/Mimic.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <memory>
#include <vector>

namespace Mimic
{
	struct GameObject;

	// #############################################################################
	// mimic core struct:
	// #############################################################################
	struct MimicCore 
	{
		explicit MimicCore();
		~MimicCore();

		static std::shared_ptr<MimicCore> Initialize();
		void Update();
		float DeltaTime() const noexcept;
		std::shared_ptr<GameObject> AddEmptyGameObject();
		void AddGameObject(std::shared_ptr<GameObject> gameObject);

		std::vector<std::shared_ptr<GameObject>> GameObjects;
		SDL_Window* Window;
		SDL_Renderer* Renderer;
		SDL_GLContext GLContext;
		bool ApplicationRunning;

	private:
		float _deltaTime;
	};
}