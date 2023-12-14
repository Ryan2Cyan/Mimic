#include "MimicCore.h"
#include <mimic_engine/ResourceManager.h>
#include <mimic_engine/GameObject.h>
#include <mimic_engine/Environment.h>
#include <mimic_render/Window.h>
#include <mimic_render/Renderer.h>
#include <mimic_render/Light.h>

#include <GL/glew.h>

namespace MimicEngine
{
	//std::shared_ptr<Camera> MimicCore::CurrentCamera;
	// std::shared_ptr<ResourceManager> MimicCore::ResourceManager;
	//std::vector<std::shared_ptr<GameObject>> MimicCore::_gameObjects;
	//std::vector<std::shared_ptr<DirectLight>> MimicCore::_directLights;
	//std::vector<std::shared_ptr<PointLight>> MimicCore::_pointLights;
	//std::vector<std::shared_ptr<Camera>> MimicCore::_cameras;
	//std::shared_ptr<Renderer> MimicCore::_renderer;
	//std::shared_ptr<CubeMap> MimicCore::CubeMap;
	//std::shared_ptr<EnvironmentCubeMap> MimicCore::EnvironmentCubeMap;
	std::shared_ptr<MimicRender::Window> MimicCore::Window;
	//std::shared_ptr<Environment> MimicCore::_environment;

	std::shared_ptr<MimicCore> MimicCore::Initialise() 
	{ 
		// Initialise a new core and store a reference to itself. This "self" reference will
		// be accessed by game objects and components to access certain data contained within
		// core:
		std::shared_ptr<MimicCore> newMimicCore = std::make_shared<MimicCore>();
		newMimicCore->_self = newMimicCore;
		newMimicCore->ApplicationRunning = false;
		bool initialised;

		// Initialise the logger utility class:
		Logger::Init();
		initialised = Logger::GetCoreLogger() != nullptr;

		// Initialise SDL_Window, SDL_Renderer, & GL_Context, SDL, GLEW, and ImGui. Additionally sets up 
		// OpenGL for rendering:
		Window = MimicRender::Window::Initialise("[Mimic Engine] Dungeon Master's Tool Kit");
		initialised = Window != nullptr;

		// Init resource manager:
		// newMimicCore->ResourceManager = ResourceManager::Initialise();
		// newMimicCore->ResourceManager->_mimicCore = newMimicCore->_self;
		// newMimicCore->ResourceManager->_self = newMimicCore->ResourceManager;
		MIMIC_LOG_INFO("[Mimic::ResourceManager] Initialisation successful.");

		// init environment:
		// newMimicCore->_environment = newMimicCore->_environment->Initialise(70.0f);
		MIMIC_LOG_INFO("[Mimic::Environment] Initialisation successful.");

		// init renderer:
		// newMimicCore->_renderer = Renderer::Initialise();
		MIMIC_LOG_INFO("[Mimic::Renderer] Initialisation successful.");

		// EnvironmentCubeMap = EnvironmentCubeMap::Initialise();

		// Ensure that the core (and all of its components) are initialised:
		if (newMimicCore == nullptr || !initialised)
		{
			MIMIC_LOG_FATAL("[Mimic::MimicCore] Unable to initialise MimicCore.");
			throw;
		}

		return newMimicCore;
	}

	glm::ivec2 MimicCore::GetCurrentAspect()
	{
		return Window->GetAspectRatio();
	}

	void MimicCore::Start()
	{
		// for (auto gameObject : _gameObjects) gameObject->Start();
		// EnvironmentCubeMap->Load("rural_asphalt_road_4k.hdr");
	}

	void MimicCore::Update()
	{
		// _environment->CalculateDeltaTime();

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
		// _renderer->_renderQue.clear();
		// _cubeMap->Draw(CurrentCamera->_viewMatrix, CurrentCamera->_projectionMatrix);
		// EnvironmentCubeMap->Draw(CurrentCamera->_viewMatrix, CurrentCamera->_projectionMatrix);
	}

	//std::shared_ptr<GameObject> MimicCore::AddEmptyGameObject() noexcept
	//{
	//	std::shared_ptr<GameObject> emptyGameObject = std::make_shared<GameObject>();
	//	emptyGameObject->_self = emptyGameObject;
	//	emptyGameObject->_mimicCore = _self;
	//	emptyGameObject->Name = "EmptyGameObject_" + std::to_string(_gameObjects.size());

	//	_gameObjects.push_back(emptyGameObject);
	//	MIMIC_LOG_INFO("[Mimic::MimicCore] Added Mimic::GameObject: \"%\".", emptyGameObject->Name);
	//	return emptyGameObject;
	//}

	//std::shared_ptr<GameObject> MimicCore::AddEmptyGameObject(const std::string& name) noexcept
	//{
	//	std::shared_ptr<GameObject> emptyGameObject = std::make_shared<GameObject>();
	//	emptyGameObject->_self = emptyGameObject;
	//	emptyGameObject->_mimicCore = _self;
	//	emptyGameObject->Name = name;

	//	_gameObjects.push_back(emptyGameObject);
	//	MIMIC_LOG_INFO("[Mimic::MimicCore] Added Mimic::GameObject: \"%\".", emptyGameObject->Name);
	//	return emptyGameObject;
	//}

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

	//void MimicCore::AddGameObject(const std::shared_ptr<GameObject> gameObject) noexcept
	//{
	//	if (gameObject == nullptr)
	//	{
	//		MIMIC_LOG_WARNING("Attempted to add null game object to hierarchy.");
	//		return;
	//	}
	//	gameObject->_mimicCore = _self;
	//	MIMIC_LOG_INFO("[Mimic::MimicCore] Added Mimic::GameObject: \"%\".", gameObject->Name);
	//	_gameObjects.push_back(gameObject);
	//}

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