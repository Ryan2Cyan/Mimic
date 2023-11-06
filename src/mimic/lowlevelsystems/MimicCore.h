#pragma once
#include <lowlevelsystems/Camera.h>
#include <GLM/glm.hpp>
#include <memory>
#include <string>

namespace Mimic
{
	// forward-declarations:
	struct GameObject;
	struct Window;
	struct ResourceManager;
	struct Environment;
	struct Camera;
	struct Renderer;
	struct Light;

	// #############################################################################
	// mimic core struct:
	// #############################################################################
	struct MimicCore 
	{
		explicit MimicCore();
		~MimicCore();

		static std::shared_ptr<MimicCore> Initialise();

		void Start();
		void Update();
		void Draw();
		const glm::vec2 GetAspectRatio() const noexcept;
		std::shared_ptr<GameObject> AddEmptyGameObject() noexcept;
		std::shared_ptr<GameObject> AddEmptyGameObject(const std::string& name) noexcept;
		void AddGameObject(const std::shared_ptr<GameObject> gameObject) noexcept;
		void AddCamera(const std::shared_ptr<Camera> camera, const bool setToCurrent) noexcept;

		static std::shared_ptr<Camera> CurrentCamera;
		static std::shared_ptr<ResourceManager> ResourceManager;
		bool ApplicationRunning;

	private:
		friend struct ModelRenderer; // needs renderer

		static std::vector<std::shared_ptr<GameObject>> _gameObjects;
		static std::vector<std::shared_ptr<Camera>> _cameras;
		static std::vector<std::shared_ptr<Light>> _lights;
		static std::shared_ptr<Renderer> _renderer;
		static std::shared_ptr<Window> _window;
		static std::shared_ptr<Environment> _environment;
		std::weak_ptr<MimicCore> _self;
	};
}