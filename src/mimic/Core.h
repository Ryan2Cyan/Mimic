#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <memory>
#include <vector>
#include "Entity.h"
#include "Component.h"

namespace Mimic
{
	struct Entity;

	struct MimicCore 
	{
		explicit MimicCore();
		~MimicCore();

		static std::shared_ptr<MimicCore> Initialize();
		void Update();
		float DeltaTime() const noexcept;
		std::shared_ptr<Entity> AddEntity();

		std::vector<std::shared_ptr<Entity>> Entities;
		SDL_Window* Window;
		SDL_Renderer* Renderer;
		SDL_GLContext GLContext;
		bool ApplicationRunning;

	private:
		float _deltaTime;
	};
}