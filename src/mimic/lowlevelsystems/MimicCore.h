#pragma once
#include <lowlevelsystems/Camera.h>
#include <GLM/glm.hpp>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <memory>
#include <vector>

namespace Mimic
{
	// forward-declarations:
	struct GameObject;
	struct Window;
	struct ResourceManager;
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
		void Draw();
		glm::vec2 GetAspectRatio() const noexcept;
		std::shared_ptr<GameObject> AddEmptyGameObject() noexcept;
		std::shared_ptr<GameObject> AddEmptyGameObject(const char* name) noexcept;
		void AddGameObject(const std::shared_ptr<GameObject> gameObject) noexcept;
		void AddCamera(const std::shared_ptr<Camera> camera, const bool setToCurrent) noexcept;

		std::vector<std::shared_ptr<GameObject>> GameObjects;
		std::vector<std::shared_ptr<Camera>> Cameras;
		std::shared_ptr<Camera> CurrentCamera;
		std::shared_ptr<Window> Window;
		std::shared_ptr<ResourceManager> ResourceManager;
		std::shared_ptr<Environment> Environment;
		bool ApplicationRunning;

	private:
		std::weak_ptr<MimicCore> _self;
		
	};
}