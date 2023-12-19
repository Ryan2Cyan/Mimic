#pragma once
// #include <lowlevelsystems/Camera.h>
#include <mimic_utility/Logger.h>
#include <mimic_render/Window.h>
#include <mimic_render/Renderer.h>

#include <GLM/glm.hpp>
#include <memory>
#include <string>
#include <list>

using namespace MimicRender;

#define MIMIC_CURRENT_ASPECT()					 ::MimicEngine::MimicCore::GetCurrentAspect()
#define MIMIC_LOAD_RESOURCE(T, fileName)		 ::MimicEngine::MimicCore::GetResourceManager()->LoadResource<T>(fileName)

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
	struct Camera;

	// #############################################################################
	// mimic core struct:
	// #############################################################################
	using namespace MimicRender;
	using namespace MimicUtility;

	struct MimicCore 
	{
		static std::shared_ptr<MimicCore> Initialise();

		/// <summary>
		/// Called once before the game loop starts running, starts the application.
		/// </summary>
		void Start();

		/// <summary>
		/// Called once per frame in the game loop. Updates all scene objects, cameras, and 
		/// delta time.
		/// </summary>
		void Update();

		/// <summary>
		/// Called once per frame in the game loop. Draws all objects in the scene.
		/// </summary>
		void Draw();

		/// <summary>
		/// End the application.
		/// </summary>
		void Exit();

		/// <summary>
		/// Returns true if the application is running, false otherwise.
		/// </summary>
		bool IsApplicationRunning() const;
		
		static glm::ivec2 GetCurrentAspect();
		static std::shared_ptr<ResourceManager> GetResourceManager();

		static std::shared_ptr<Camera> CurrentCamera;
		static std::shared_ptr<ResourceManager> ResourceManager;
		static std::shared_ptr<MimicRender::Window> Window;
		// static std::shared_ptr<EnvironmentCubeMap> EnvironmentCubeMap;

	private:
		friend struct ModelRenderer; 
		friend struct PBRMaterial; 
		friend struct EnvironmentCubeMap;
		friend struct GameObject;
		friend struct Camera;
		friend struct DirectLight;

		static void AddGameObject(const std::shared_ptr<GameObject>& gameObject);
		static void RemoveGameObject(const std::shared_ptr<GameObject>& gameObject);
		static void AddCamera(const std::shared_ptr<Camera>& camera);
		static void RemoveCamera(const std::shared_ptr<Camera>& camera);
		static void AddDirectLight(const std::shared_ptr<DirectLight>& directLight);
		static void RemoveDirectLight(const std::shared_ptr<DirectLight>& directLight);
		// std::shared_ptr<PointLight> AddPointLight() noexcept;

		static std::shared_ptr<MimicRender::Renderer> _renderer;
		static std::shared_ptr<Environment> _environment;
		static std::list<std::shared_ptr<GameObject>> _gameObjects;
		static std::list<std::shared_ptr<Camera>> _cameras;
		static std::list<std::shared_ptr<DirectLight>> _directLights;
		// static std::vector<std::shared_ptr<MimicRender::PointLight>> _pointLights;

		static std::weak_ptr<MimicCore> _self;
		static bool _applicationRunning;
		bool _initialised;
	};
}