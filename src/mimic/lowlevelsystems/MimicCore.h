#pragma once
#include <lowlevelsystems/Mimic.h>
#include <GLM/glm.hpp>
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
	struct Camera;

	// #############################################################################
	// mimic core struct:
	// #############################################################################
	struct MimicCore 
	{
		explicit MimicCore();
		~MimicCore();

		static std::shared_ptr<MimicCore> Initialise();

		void Update();
		glm::vec2 GetAspectRatio() const;
		std::shared_ptr<GameObject> AddEmptyGameObject();
		std::shared_ptr<GameObject> AddEmptyGameObject(const char* name);
		void AddGameObject(const std::shared_ptr<GameObject> gameObject);
		std::shared_ptr<Camera> AddNewCamera();

		std::vector<std::shared_ptr<GameObject>> GameObjects;
		std::vector<std::shared_ptr<Camera>> Cameras;
		std::shared_ptr<Camera> MainCamera;
		std::shared_ptr<Window> Window;
		std::shared_ptr<Environment> Environment;
		bool ApplicationRunning;

	private:
		std::weak_ptr<MimicCore> _self;
	};
}