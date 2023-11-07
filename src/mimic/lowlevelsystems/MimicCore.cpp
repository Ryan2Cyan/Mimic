#include "MimicCore.h"
#include <utility/Logger.h>
#include <lowlevelsystems/Window.h>
#include <lowlevelsystems/ResourceManager.h>
#include <lowlevelsystems/GameObject.h>
#include <lowlevelsystems/Environment.h>
#include <renderengine/Renderer.h>
#include <renderengine/Light.h>
#include <GL/glew.h>


namespace Mimic
{
	std::shared_ptr<Camera> MimicCore::CurrentCamera;
	std::shared_ptr<ResourceManager> MimicCore::ResourceManager;
	std::vector<std::shared_ptr<GameObject>> MimicCore::_gameObjects;
	std::vector<std::shared_ptr<DirectLight>> MimicCore::_lights;
	std::vector<std::shared_ptr<Camera>> MimicCore::_cameras;
	std::shared_ptr<Renderer> MimicCore::_renderer;
	std::shared_ptr<Window> MimicCore::_window;
	std::shared_ptr<Environment> MimicCore::_environment;

	MimicCore::MimicCore()
	{
		Mimic::Logger::Init();

		// initialise SDL_Window, SDL_Renderer, & GL_Context:
		glm::vec2 aspectRatio = glm::vec2(1260.0f, 720.0f);
		_window = Window::Initialise("[Mimic Engine] Dungeon Master's Tool Kit", aspectRatio);

		// init glew:
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			MIMIC_LOG_FATAL("GLEW failed to initialise with message: %", glewGetErrorString(err));
			throw;
		}
		MIMIC_LOG_INFO("[GLEW] Initialisation successful.");

		ApplicationRunning = true;
		glEnable(GL_DEPTH_TEST);
	}

	MimicCore::~MimicCore() { }

	std::shared_ptr<MimicCore> MimicCore::Initialise() 
	{ 
		std::shared_ptr<MimicCore> newMimicCore = std::make_shared<MimicCore>();
		newMimicCore->_self = newMimicCore;

		// init resource manager:
		newMimicCore->ResourceManager = ResourceManager::Initialise();
		newMimicCore->ResourceManager->_mimicCore = newMimicCore->_self;
		newMimicCore->ResourceManager->_self = newMimicCore->ResourceManager;
		MIMIC_LOG_INFO("[Mimic::ResourceManager] Initialisation successful.");

		// init environment:
		newMimicCore->_environment = newMimicCore->_environment->Initialise(70.0f);
		MIMIC_LOG_INFO("[Mimic::Environment] Initialisation successful.");
		if(newMimicCore != nullptr) MIMIC_LOG_INFO("[Mimic::MimicCore] Initialisation successful.");

		// init renderer:
		newMimicCore->_renderer = Renderer::Initialise();
		MIMIC_LOG_INFO("[Mimic::Renderer] Initialisation successful.");

		return newMimicCore;
	}

	void MimicCore::Start()
	{
		for (auto gameObject : _gameObjects) gameObject->Start();
	}

	void MimicCore::Update()
	{
		// other updates:
		_environment->CalculateDeltaTime();

		// update game objects:
		for (auto camera : _cameras) for (auto gameObject : _gameObjects) gameObject->Update();
	}

	void MimicCore::Draw()
	{
		for (auto camera : _cameras)
		{
			_renderer->Draw(
				camera->GetGameObject()->_modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 
				camera->_viewMatrix, 
				camera->_projectionMatrix,
				_lights
			);
		}
		_renderer->_renderQue.clear();
		SDL_GL_SwapWindow(_window->_window);
	}

	const glm::vec2 MimicCore::GetAspectRatio() const noexcept
	{
		return _window->_aspectRatio;
	}

	std::shared_ptr<GameObject> MimicCore::AddEmptyGameObject() noexcept
	{
		std::shared_ptr<GameObject> emptyGameObject = std::make_shared<GameObject>();
		emptyGameObject->_self = emptyGameObject;
		emptyGameObject->_mimicCore = _self;
		emptyGameObject->Name = "EmptyGameObject_" + std::to_string(_gameObjects.size());

		_gameObjects.push_back(emptyGameObject);
		MIMIC_LOG_INFO("[Mimic::MimicCore] Added Mimic::GameObject: \"%\".", emptyGameObject->Name);
		return emptyGameObject;
	}

	std::shared_ptr<GameObject> MimicCore::AddEmptyGameObject(const std::string& name) noexcept
	{
		std::shared_ptr<GameObject> emptyGameObject = std::make_shared<GameObject>();
		emptyGameObject->_self = emptyGameObject;
		emptyGameObject->_mimicCore = _self;
		emptyGameObject->Name = name;

		_gameObjects.push_back(emptyGameObject);
		MIMIC_LOG_INFO("[Mimic::MimicCore] Added Mimic::GameObject: \"%\".", emptyGameObject->Name);
		return emptyGameObject;
	}

	std::shared_ptr<DirectLight> MimicCore::AddLight() noexcept
	{
		std::shared_ptr<DirectLight> newLight = std::make_shared<DirectLight>();
		newLight->Name = "Light_" + _lights.size();

		_lights.push_back(newLight);
		MIMIC_LOG_INFO("[Mimic::MimicCore] Added Mimic::Light: \"%\".", newLight->Name);
		return newLight;
	}

	void MimicCore::AddGameObject(const std::shared_ptr<GameObject> gameObject) noexcept
	{
		if (gameObject == nullptr)
		{
			MIMIC_LOG_WARNING("Attempted to add null game object to hierarchy.");
			return;
		}
		gameObject->_mimicCore = _self;
		MIMIC_LOG_INFO("[Mimic::MimicCore] Added Mimic::GameObject: \"%\".", gameObject->Name);
		_gameObjects.push_back(gameObject);
	}

	void MimicCore::AddCamera(const std::shared_ptr<Camera> camera, const bool setToCurrent) noexcept
	{
		if (camera == nullptr)
		{
			MIMIC_LOG_WARNING("Attempted to add null camera to hierarchy.");
			return;
		}
		_cameras.push_back(camera);


		if (!setToCurrent) return;
		CurrentCamera = camera;
		_renderer->_cachedViewMatrix = CurrentCamera->_viewMatrix;
		_renderer->_cachedProjectionMatrix = CurrentCamera->_projectionMatrix;
		MIMIC_LOG_INFO("[Mimic::MimicCore] Added Mimic::Camera: \"%\".", camera->GetGameObject()->Name);
	}
}