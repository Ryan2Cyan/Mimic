#pragma once
#include <lowlevelsystems/Mimic.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <memory>
#include <vector>

namespace Mimic
{
	// forward-declarations:
	struct GameObject;
	struct Window;
	struct Environment;

	// #############################################################################
	// mimic core struct:
	// #############################################################################
	struct MimicCore 
	{
		explicit MimicCore();
		~MimicCore();

		static std::shared_ptr<MimicCore> Initialise();

		void Update();
		std::shared_ptr<GameObject> AddEmptyGameObject();
		void AddGameObject(std::shared_ptr<GameObject> gameObject);

		std::vector<std::shared_ptr<GameObject>> GameObjects;
		std::shared_ptr<Window> Window;
		std::shared_ptr<Environment> Environment;
		bool ApplicationRunning;
	};
}