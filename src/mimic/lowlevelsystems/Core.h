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
	struct Window;

	// #############################################################################
	// mimic core struct:
	// #############################################################################
	struct MimicCore 
	{
		explicit MimicCore();
		~MimicCore();

		static std::shared_ptr<MimicCore> Initialise();
		void Update();
		float DeltaTime() const noexcept;
		std::shared_ptr<GameObject> AddEmptyGameObject();
		void AddGameObject(std::shared_ptr<GameObject> gameObject);
		void Run() const;

		std::vector<std::shared_ptr<GameObject>> GameObjects;
		std::shared_ptr<Window> Window;
		bool ApplicationRunning;

	private:
		float _deltaTime;
	};
}