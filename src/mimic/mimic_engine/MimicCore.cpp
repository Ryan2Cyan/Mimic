#include "MimicCore.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Environment.h"
#include <mimic_render/Window.h>
#include <mimic_render/Light.h>

#include <GL/glew.h>
#include <algorithm>

namespace MimicEngine
{
	std::shared_ptr<MimicRender::Window> MimicCore::Window;
	std::shared_ptr<ResourceManager> MimicCore::ResourceManager;
	std::shared_ptr<MimicRender::Renderer> MimicCore::_renderer;
	std::shared_ptr<Environment> MimicCore::_environment;
	std::list<std::shared_ptr<GameObject>> MimicCore::_gameObjects;
	std::weak_ptr<MimicCore> MimicCore::_self;
	bool MimicCore::_applicationRunning;
	//std::shared_ptr<Camera> MimicCore::CurrentCamera;
	//std::vector<std::shared_ptr<DirectLight>> MimicCore::_directLights;
	//std::vector<std::shared_ptr<PointLight>> MimicCore::_pointLights;
	//std::vector<std::shared_ptr<Camera>> MimicCore::_cameras;
	//std::shared_ptr<CubeMap> MimicCore::CubeMap;
	//std::shared_ptr<EnvironmentCubeMap> MimicCore::EnvironmentCubeMap;

	std::shared_ptr<MimicCore> MimicCore::Initialise() 
	{ 
		// Initialise a new core and store a reference to itself. This "self" reference will
		// be accessed by game objects and components to access certain data contained within
		// core:
		std::shared_ptr<MimicCore> newMimicCore = std::make_shared<MimicCore>();
		newMimicCore->_self = newMimicCore;
		newMimicCore->_applicationRunning = false;
		bool initialised;

		// Initialise the logger utility class:
		Logger::Init();
		initialised = Logger::GetCoreLogger() != nullptr;

		// Initialise SDL_Window, SDL_Renderer, & GL_Context, SDL, GLEW, and ImGui. Additionally sets up 
		// OpenGL for rendering:
		Window = MimicRender::Window::Initialise("[Mimic Engine] Dungeon Master's Tool Kit");
		initialised = Window != nullptr;

		// Initialise resource manager:
		newMimicCore->ResourceManager = ResourceManager::Initialise();
		newMimicCore->ResourceManager->_mimicCore = newMimicCore->_self;
		newMimicCore->ResourceManager->_self = newMimicCore->ResourceManager;
		MIMIC_LOG_INFO("[MimicEngine::ResourceManager] Initialisation successful.");

		// Initialise environment:
		newMimicCore->_environment = Environment::Initialise();
		MIMIC_LOG_INFO("[MimicEngine::Environment] Initialisation successful.");

		// Initialise renderer:
		newMimicCore->_renderer = MimicRender::Renderer::Initialise();
		MIMIC_LOG_INFO("[MimicEngine::Renderer] Initialisation successful.");

		// EnvironmentCubeMap = EnvironmentCubeMap::Initialise();

		// Ensure that the core (and all of its components) are initialised:
		if (newMimicCore == nullptr || !initialised)
		{
			MIMIC_LOG_FATAL("[MimicEngine::MimicCore] Unable to initialise MimicCore.");
			throw;
		}

		MIMIC_LOG_INFO("[MimicEngine::MimicCore] Mimic Core initialisation successful.");
		return newMimicCore;
	}

	glm::ivec2 MimicCore::GetCurrentAspect()
	{
		return Window->GetAspectRatio();
	}

	std::shared_ptr<ResourceManager> MimicCore::GetResourceManager()
	{
		return ResourceManager;
	}

	void MimicCore::Start()
	{
		 for (auto gameObject : _gameObjects) gameObject->Start();
		// EnvironmentCubeMap->Load("rural_asphalt_road_4k.hdr");
		 _applicationRunning = true;
	}

	void MimicCore::Update()
	{
		_environment->CalculateDeltaTime();
		for (auto gameObject : _gameObjects)
		{
			gameObject->Update();
		}
		// update game objects:
		/*for (auto camera : _cameras)
		{
			for (auto gameObject : _gameObjects)
			{
				gameObject->Update();
			}
		}*/
	}

	void MimicCore::Draw()
	{
		Window->ClearBuffers();
		//for (auto camera : _cameras)
		//{
		//	/*_renderer->Draw(
		//		camera->GetGameObject()->Position, 
		//		camera->_viewMatrix, 
		//		camera->_projectionMatrix
		//	);*/
		//}
		_renderer->ClearRenderQueue();
		// EnvironmentCubeMap->Draw(CurrentCamera->_viewMatrix, CurrentCamera->_projectionMatrix);
	}

	void MimicCore::Exit()
	{
		_applicationRunning = false;
	}

	bool MimicCore::IsApplicationRunning() const
	{
		return _applicationRunning;
	}

	//std::shared_ptr<DirectLight> MimicCore::AddDirectLight() noexcept
	//{
	//	std::shared_ptr<DirectLight> newLight = std::make_shared<DirectLight>();
	//	newLight->Name = "DirectLight_" + _directLights.size();

	//	_directLights.push_back(newLight);
	//	MIMIC_LOG_INFO("[Mimic::MimicCore] Added Mimic::DirectLight: \"%\".", newLight->Name);
	//	return newLight;
	//}

	//std::shared_ptr<PointLight> MimicCore::AddPointLight() noexcept
	//{
	//	std::shared_ptr<PointLight> newLight = std::make_shared<PointLight>();
	//	newLight->Name = "PointLight_" + _directLights.size();

	//	_pointLights.push_back(newLight);
	//	MIMIC_LOG_INFO("[Mimic::MimicCore] Added Mimic::PointLight: \"%\".", newLight->Name);
	//	return newLight;
	//}

	void MimicCore::AddGameObject(const std::shared_ptr<GameObject>& gameObject)
	{
		if (gameObject == nullptr)
		{
			MIMIC_LOG_WARNING("[MimicEngine::MimicCore] Attempted to add null game object to hierarchy.");
			return;
		}
		gameObject->_mimicCore = _self;
		MIMIC_LOG_INFO("[MimicEngine::MimicCore] Added Mimic::GameObject: \"%\".", gameObject->Name);
		_gameObjects.push_back(gameObject);
	}

	void MimicCore::RemoveGameObject(const std::shared_ptr<GameObject>& gameObject)
	{
		if (gameObject != nullptr)
		{
			_gameObjects.remove(gameObject);
		}
	}

	//void MimicCore::AddCamera(const std::shared_ptr<Camera> camera, const bool setToCurrent) noexcept
	//{
	//	if (camera == nullptr)
	//	{
	//		MIMIC_LOG_WARNING("Attempted to add null camera to hierarchy.");
	//		return;
	//	}
	//	_cameras.push_back(camera);


	//	if (!setToCurrent) return;
	//	CurrentCamera = camera;
	//	MIMIC_LOG_INFO("[Mimic::MimicCore] Added Mimic::Camera: \"%\".", camera->GetGameObject()->Name);
	//}
}