#include "MimicCore.h"
#include <string>


namespace Mimic
{
	MimicCore::MimicCore()
	{
		// initialise SDL_Window, SDL_Renderer, & GL_Context:
		glm::vec2 aspectRatio = glm::vec2(1260.0f, 720.0f);
		Window = Window->Initialise("DMTK", aspectRatio);
		Environment = Environment->Initialise(70.0f);

		// init glew:
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err) std::cerr << "Error: GLEW failed to initialise with message: " << glewGetErrorString(err) << std::endl;
		
		ApplicationRunning = true;
		glEnable(GL_DEPTH_TEST);
	}

	MimicCore::~MimicCore() { }

	std::shared_ptr<MimicCore> MimicCore::Initialise() 
	{ 
		std::shared_ptr<MimicCore> newMimicCore =  std::make_shared<MimicCore>();
		newMimicCore->_self = newMimicCore;
		return newMimicCore;
	}

	void MimicCore::Update()
	{
		Environment->CalculateDeltaTime();
		unsigned int length = Cameras.size();
		for (unsigned int i = 0; i < length; i++) Cameras[i]->Update();

		length = GameObjects.size();
		for (unsigned int i = 0; i < length; i++) GameObjects[i]->Update();

		// KARSTEN ADVICE:
		// Potentially add raytracer component:
		/*length = GameObjects.size();
		for (unsigned int i = 0; i < _cameras.length; i++)
		{
			MainCamera = _currentCamera;
			for (unsigned int j = 0; j < length; j++) GameObjects[i]->Display();
		}*/

		SDL_GL_SwapWindow(Window->_window);
	}

	glm::vec2 MimicCore::GetAspectRatio() const
	{
		return Window->AspectRatio;
	}

	std::shared_ptr<GameObject> MimicCore::AddEmptyGameObject()
	{
		std::shared_ptr<GameObject> emptyGameObject = std::make_shared<GameObject>();
		emptyGameObject->_self = emptyGameObject;
		emptyGameObject->_mimicCore = _self;
		emptyGameObject->Name = "EmptyGameObject_" + std::to_string(GameObjects.size());

		GameObjects.push_back(emptyGameObject);

		return emptyGameObject;
	}

	std::shared_ptr<GameObject> MimicCore::AddEmptyGameObject(const char* name)
	{
		std::shared_ptr<GameObject> emptyGameObject = std::make_shared<GameObject>();
		emptyGameObject->_self = emptyGameObject;
		emptyGameObject->_mimicCore = _self;
		emptyGameObject->Name = name;

		GameObjects.push_back(emptyGameObject);
		return emptyGameObject;
	}

	void MimicCore::AddGameObject(const std::shared_ptr<GameObject> gameObject)
	{
		/*gameObject->_mimicCore = this;*/
		GameObjects.push_back(gameObject);
	}

	std::shared_ptr<Camera> MimicCore::AddNewCamera()
	{
		std::shared_ptr<Camera> newCamera = std::make_shared<Camera>(Window->AspectRatio);
		newCamera->_self = newCamera;
		newCamera->_mimicCore = _self;

		if (Cameras.size() < 1)
		{
			MainCamera = newCamera;
			newCamera->MainCamera = true;
		}
		Cameras.push_back(newCamera);
		return newCamera;
	}
}