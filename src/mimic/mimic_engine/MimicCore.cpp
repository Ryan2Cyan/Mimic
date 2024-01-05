#include "MimicCore.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Environment.h"
#include "Camera.h"
#include "Light.h"
#include "ShadowMapper.h"
#include "InputHandler.h"
#include <mimic_render/Light.h>

#include <GL/glew.h>
#include <algorithm>
#include <SDL/SDL.h>

namespace MimicEngine
{
	std::shared_ptr<MimicCore> MimicCore::Initialise() 
	{ 
		// Initialise a new core and store a reference to itself. This "self" reference will
		// be accessed by game objects and components to access certain data contained within
		// core:
		std::shared_ptr<MimicCore> mimicCore = std::make_shared<MimicCore>();
		mimicCore->_self = mimicCore;
		mimicCore->_applicationRunning = false;
		bool initialised;

		// Initialise all key components:
		Logger::Init();
		initialised = Logger::GetCoreLogger() != nullptr;
		mimicCore->_window = MimicRender::Window::Initialise("[Mimic Engine] Dungeon Master's Tool Kit");
		initialised = mimicCore->_window != nullptr;

		mimicCore->_resourceManager = ResourceManager::Initialise();
		mimicCore->_resourceManager->_mimicCore = mimicCore->_self;
		mimicCore->_resourceManager->_self = mimicCore->_resourceManager;
		mimicCore->_environment = Environment::Initialise();
		mimicCore->_renderer = MimicRender::Renderer::Initialise();
		mimicCore->_environmentCubeMap = MimicRender::EnvironmentCubeMap::Initialise("rural_asphalt_road_4k.hdr", mimicCore->_window->GetAspectRatio(), mimicCore->_renderer);
		mimicCore->_shadowMapper = ShadowMapper::Initialise(glm::ivec2(4096, 4096));
		mimicCore->_inputHandler = InputHandler::Initialise(mimicCore->_applicationRunning);
		

		// Ensure that the core (and all of its components) are initialised:
		if (mimicCore == nullptr || !initialised)
		{
			MIMIC_LOG_FATAL("[MimicEngine::MimicCore] Unable to initialise MimicCore.");
			throw;
		}

		MIMIC_LOG_INFO("[MimicEngine::MimicCore] Mimic Core initialisation successful.");
		return mimicCore;
	}

	std::shared_ptr<ResourceManager> MimicCore::GetResourceManager() const
	{
		return _resourceManager;
	}

	std::shared_ptr<InputHandler> MimicCore::GetInputHandler() const
	{
		return _inputHandler;
	}

	std::shared_ptr<MimicRender::Window> MimicCore::GetWindow() const
	{
		return _window;
	}

	void MimicCore::Start()
	{
		for (auto gameObject : _gameObjects) gameObject->Start();
		 _applicationRunning = true;
	}

	void MimicCore::Update()
	{
		// Handle user inputs. If the user presses the escape key, exit the application.
		_inputHandler->Update();
		if(_inputHandler->IsKey(SDLK_ESCAPE)) _applicationRunning = false;

		// Re-calculate delta time for this frame.
		_environment->CalculateDeltaTime();

		// Update all scene objects.
		for (auto gameObject : _gameObjects) gameObject->Update();
		for (auto camera : _cameras) camera->Update();
	}

	void MimicCore::FixedUpdate()
	{
		// Wait for the fixed time tick before executing.
		if (!_environment->FixedTimeTick()) return;

		// Update all scene objects.
		for (auto gameObject : _gameObjects) gameObject->FixedUpdate();
	}

	void MimicCore::Draw()
	{
		// Clear depth and colour buffers, then return the viewport to the dimensions of window:
		_window->ClearBuffers();

		for (auto directLight : _directLights) _shadowMapper->_depthMapDirectLights.push_back(directLight->_renderDirectLight);
		_shadowMapper->RenderDirectLightDepthMaps(_renderer);
		_shadowMapper->_depthMapDirectLights.clear();

		_window->ResetViewPort();

		// For each camera, render each render object in the renderer and the environment map. Then
		// clear the render object list to prevent a memory leak:
		for (auto camera : _cameras)
		{
			_renderer->Draw(camera->_renderCamera);
			_renderer->DrawEnvironmentMap(camera->_renderCamera, _environmentCubeMap);
		}
		_renderer->ClearRenderQueue();
	}

	void MimicCore::Exit()
	{
		_applicationRunning = false;
	}

	bool MimicCore::IsApplicationRunning()
	{
		return _applicationRunning;
	}

	std::shared_ptr<GameObject> MimicCore::AddGameObject()
	{
		auto gameObject = GameObject::Initialise();
		if (gameObject == nullptr)
		{
			MIMIC_LOG_WARNING("[MimicEngine::MimicCore] Attempted to add null game object to hierarchy.");
			return nullptr;
		}
		gameObject->_mimicCore = _self;
		gameObject->Name = "GameObject_" + _gameObjects.size();
		MIMIC_LOG_INFO("[MimicEngine::MimicCore] Added Mimic::GameObject: \"%\".", gameObject->Name);
		_gameObjects.push_back(gameObject);
		return gameObject;
	}

	std::shared_ptr<GameObject> MimicCore::AddGameObject(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	{
		auto gameObject = GameObject::Initialise(position, rotation, scale);
		if (gameObject == nullptr)
		{
			MIMIC_LOG_WARNING("[MimicEngine::MimicCore] Attempted to add null GameObject to hierarchy.");
			return nullptr;
		}
		gameObject->_mimicCore = _self;
		gameObject->Name = "GameObject_" + _gameObjects.size();
		MIMIC_LOG_INFO("[MimicEngine::MimicCore] Added Mimic::GameObject: \"%\".", gameObject->Name);
		_gameObjects.push_back(gameObject);
		return gameObject;
	}

	/*void MimicCore::RemoveGameObject(const std::shared_ptr<GameObject>& gameObject)
	{
		if (gameObject != nullptr) _gameObjects.remove(gameObject);
	}*/

	std::shared_ptr<Camera> MimicCore::AddCamera(const glm::vec2& aspectRatio, const float& fov, const glm::vec2& clippingPlane)
	{
		auto camera = Camera::Initialise(aspectRatio, fov, clippingPlane);
		if (camera == nullptr)
		{
			MIMIC_LOG_WARNING("[MimicEngine::MimicCore] Attempted to add null Camera to hierarchy.");
			return nullptr;
		}

		// NOTE: A reference to the MimicCore may be needed later on:
		// camera->_mimicCore = _self;

		camera->Name = "Camera_" + _cameras.size();
		MIMIC_LOG_INFO("[MimicEngine::MimicCore] Added Mimic::Camera: \"%\".", camera->Name);
		_cameras.push_back(camera);
		return camera;
	}

	/*void MimicCore::RemoveCamera(const std::shared_ptr<Camera>& camera)
	{
		if (camera != nullptr) _cameras.remove(camera);
	}*/

	std::shared_ptr<DirectLight> MimicCore::AddDirectLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& colour)
	{
		auto directLight = std::shared_ptr<DirectLight>();
		if (directLight == nullptr)
		{
			MIMIC_LOG_WARNING("[MimicEngine::MimicCore] Attempted to add null DirectLight to hierarchy.");
			return nullptr;
		}

		directLight->Name = "DirectLight_" + _directLights.size();
		MIMIC_LOG_INFO("[MimicEngine::MimicCore] Added Mimic::GameObject: \"%\".", directLight->Name);
		_directLights.push_back(directLight);
		return directLight;
	}

	/*void MimicCore::RemoveDirectLight(const std::shared_ptr<DirectLight>& directLight)
	{
		if (directLight == nullptr) return;
		_directLights.remove(directLight);
	}*/

	/*std::shared_ptr<PointLight> MimicCore::AddPointLight() noexcept
	{
		std::shared_ptr<PointLight> newLight = std::make_shared<PointLight>();
		newLight->Name = "PointLight_" + _directLights.size();

		_pointLights.push_back(newLight);
		MIMIC_LOG_INFO("[Mimic::MimicCore] Added Mimic::PointLight: \"%\".", newLight->Name);
		return newLight;
	}*/
}