#include "MimicCore.h"
#include <utility/Logger.h>
#include <lowlevelsystems/Window.h>
#include <lowlevelsystems/ResourceManager.h>
#include <lowlevelsystems/GameObject.h>
#include <lowlevelsystems/Environment.h>
#include <string>


namespace Mimic
{
	MimicCore::MimicCore()
	{
		Mimic::Logger::Init();

		// initialise SDL_Window, SDL_Renderer, & GL_Context:
		glm::vec2 aspectRatio = glm::vec2(1260.0f, 720.0f);
		Window = Window->Initialise("DMTK", aspectRatio);

		// init glew:
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			MIMIC_LOG_FATAL("Error: GLEW failed to initialise with message: %", glewGetErrorString(err));
			throw;
		}
		
		ApplicationRunning = true;
		glEnable(GL_DEPTH_TEST);
	}

	MimicCore::~MimicCore() { }

	std::shared_ptr<MimicCore> MimicCore::Initialise() 
	{ 
		std::shared_ptr<MimicCore> newMimicCore = std::make_shared<MimicCore>();
		newMimicCore->_self = newMimicCore;

		// init resource manager:
		newMimicCore->ResourceManager = newMimicCore->ResourceManager->Initialise();
		newMimicCore->ResourceManager->_mimicCore = newMimicCore->_self;
		newMimicCore->ResourceManager->_self = newMimicCore->ResourceManager;

		// init environment:
		newMimicCore->Environment = newMimicCore->Environment->Initialise(70.0f);

		return newMimicCore;
	}

	void MimicCore::Update()
	{
		// other updates:
		Environment->CalculateDeltaTime();

		// update game objects:
		for (unsigned int i = 0; i < GameObjects.size(); i++) GameObjects[i]->Update();
	}

	void MimicCore::Draw()
	{
		// draw game objects:
		for (unsigned int i = 0; i < Cameras.size(); i++)
		{
			for (unsigned int i = 0; i < GameObjects.size(); i++)
			{
				GameObjects[i]->Draw();
			}
		}

		SDL_GL_SwapWindow(Window->_window);
	}

	glm::vec2 MimicCore::GetAspectRatio() const noexcept
	{
		return Window->AspectRatio;
	}

	std::shared_ptr<GameObject> MimicCore::AddEmptyGameObject() noexcept
	{
		std::shared_ptr<GameObject> emptyGameObject = std::make_shared<GameObject>();
		emptyGameObject->_self = emptyGameObject;
		emptyGameObject->_mimicCore = _self;
		emptyGameObject->Name = "EmptyGameObject_" + std::to_string(GameObjects.size());

		GameObjects.push_back(emptyGameObject);
		return emptyGameObject;
	}

	std::shared_ptr<GameObject> MimicCore::AddEmptyGameObject(const char* name) noexcept
	{
		std::shared_ptr<GameObject> emptyGameObject = std::make_shared<GameObject>();
		emptyGameObject->_self = emptyGameObject;
		emptyGameObject->_mimicCore = _self;
		emptyGameObject->Name = name;

		GameObjects.push_back(emptyGameObject);
		return emptyGameObject;
	}

	void MimicCore::AddGameObject(const std::shared_ptr<GameObject> gameObject) noexcept
	{
		if (gameObject == nullptr)
		{
			MIMIC_LOG_WARNING("Attempted to add null game object to hierarchy.");
			return;
		}
		gameObject->_mimicCore = _self;

		GameObjects.push_back(gameObject);
	}

	void MimicCore::AddCamera(const std::shared_ptr<Camera> camera, const bool setToCurrent) noexcept
	{
		if (camera == nullptr)
		{
			MIMIC_LOG_WARNING("Attempted to add null camera to hierarchy.");
			return;
		}
		Cameras.push_back(camera);

		if (!setToCurrent) return;
		CurrentCamera = camera;
	}

	/*void MimicCore::AddCamera(const std::shared_ptr<Camera> camera, const bool setToCurrent = false)
	{
		Cameras.push_back(camera);

		if (!setToCurrent) return;
		CurrentCamera = camera;
	}*/
}