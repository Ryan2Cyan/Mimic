#pragma once
#include <mimic_utility/Logger.h>
#include <mimic_render/Window.h>
#include <mimic_render/Renderer.h>
#include <mimic_render/CubeMap.h>

#include <GLM/glm.hpp>
#include <memory>
#include <string>
#include <list>

using namespace MimicRender;

namespace MimicEngine
{
	// Forward-declarations:
	struct GameObject;
	struct Window;
	struct ResourceManager;
	struct Environment;
	struct Camera;
	struct Renderer;
	struct DirectLight;
	struct PointLight;
	struct EnvironmentCubeMap;
	struct ShadowMapper;
	struct Camera;
	struct InputHandler;

	// #############################################################################
	// Mimic Core Struct:
	// #############################################################################
	using namespace MimicRender;
	using namespace MimicUtility;

	struct MimicCore 
	{
		static std::shared_ptr<MimicCore> Initialise();

		/// <summary> Called once before the game loop starts running, starts the application. </summary>
		void Start();

		/// <summary> Called once per frame in the game loop. Updates all scene objects, cameras, and 
		/// delta time. </summary>
		void Update();

		/// <summary> Called once per frame in the game loop. Draws all objects in the scene. </summary>
		void Draw();

		/// <summary> End the application. </summary>
		void Exit();

		/// <summary> Returns true if the application is running, false otherwise. </summary>
		bool IsApplicationRunning();
		
		std::shared_ptr<ResourceManager> GetResourceManager() const;
		std::shared_ptr<InputHandler> GetInputHandler() const;
		std::shared_ptr<MimicRender::Window> GetWindow() const;

		std::shared_ptr<Camera> AddCamera(const glm::vec2& aspectRatio, const float& fov, const glm::vec2& clippingPlane = glm::vec2(0.1f, 100.0f));
		std::shared_ptr<GameObject> AddGameObject();
		std::shared_ptr<GameObject> AddGameObject(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
		std::shared_ptr<DirectLight> AddDirectLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& colour);

		std::shared_ptr<Camera> CurrentCamera;

	private:
		friend struct ModelRenderer; 
		friend struct PBRMaterial; 
		friend struct EnvironmentCubeMap;
		friend struct GameObject;
		friend struct Camera;
		friend struct DirectLight;
		friend struct ShadowMapper;
		friend struct InputHandler;

		/*void RemoveGameObject(const std::shared_ptr<GameObject>& gameObject);
		void RemoveCamera(const std::shared_ptr<Camera>& camera);
		void RemoveDirectLight(const std::shared_ptr<DirectLight>& directLight);*/
		// std::shared_ptr<PointLight> AddPointLight() noexcept;

		std::shared_ptr<Environment> _environment;
		std::shared_ptr<ResourceManager> _resourceManager;
		std::shared_ptr<InputHandler> _inputHandler;
		std::shared_ptr<MimicRender::Window> _window;
		std::shared_ptr<ShadowMapper> _shadowMapper;
		std::shared_ptr<MimicRender::Renderer> _renderer;

		std::list<std::shared_ptr<DirectLight>> _directLights;
		std::list<std::shared_ptr<GameObject>> _gameObjects;
		std::list<std::shared_ptr<Camera>> _cameras;

		std::shared_ptr<MimicRender::EnvironmentCubeMap> _environmentCubeMap;
		// static std::vector<std::shared_ptr<MimicRender::PointLight>> _pointLights;

		std::weak_ptr<MimicCore> _self;
		bool _applicationRunning;
		bool _initialised;
	};
}