#pragma once
// #include <lowlevelsystems/Camera.h>
#include <mimic_utility/Logger.h>
#include <mimic_render/Window.h>

#include <GLM/glm.hpp>
#include <memory>
#include <string>

using namespace MimicRender;

#define MIMIC_CURRENT_ASPECT()			::MimicEngine::MimicCore::GetCurrentAspect()

namespace MimicEngine
{
	// forward-declarations:
	struct GameObject;
	struct Window;
	struct ResourceManager;
	struct Environment;
	struct Camera;
	struct Renderer;
	struct DirectLight;
	struct PointLight;
	struct CubeMap;
	struct EnvironmentCubeMap;

	// #############################################################################
	// mimic core struct:
	// #############################################################################
	using namespace MimicRender;
	using namespace MimicUtility;

	struct MimicCore 
	{
		static std::shared_ptr<MimicCore> Initialise();

		 void Start();
		 void Update();
		 void Draw();

		/// <summary>
		/// Get current aspect ratio of application window.
		/// </summary>
		static glm::ivec2 GetCurrentAspect();

		// std::shared_ptr<GameObject> AddEmptyGameObject() noexcept;
		// std::shared_ptr<GameObject> AddEmptyGameObject(const std::string& name) noexcept;
		// std::shared_ptr<DirectLight> AddDirectLight() noexcept;
		// std::shared_ptr<PointLight> AddPointLight() noexcept;
		// void AddGameObject(const std::shared_ptr<GameObject> gameObject) noexcept;
		// void AddCamera(const std::shared_ptr<Camera> camera, const bool setToCurrent) noexcept;

		// static std::shared_ptr<Camera> CurrentCamera;
		static std::shared_ptr<ResourceManager> ResourceManager;
		static std::shared_ptr<MimicRender::Window> Window;
		// static std::shared_ptr<CubeMap> CubeMap;
		// static std::shared_ptr<EnvironmentCubeMap> EnvironmentCubeMap;
		bool ApplicationRunning;

	private:
		friend struct ModelRenderer; 
		friend struct PBRMaterial; 
		friend struct EnvironmentCubeMap;

		// static std::vector<std::shared_ptr<GameObject>> _gameObjects;
		// static std::vector<std::shared_ptr<Camera>> _cameras;
		// static std::vector<std::shared_ptr<DirectLight>> _directLights;
		// static std::vector<std::shared_ptr<PointLight>> _pointLights;
		// static std::shared_ptr<Renderer> _renderer;
		// static std::shared_ptr<Environment> _environment;
		std::weak_ptr<MimicCore> _self;
		bool _initialised;
	};
}