#include "MimicCore.h"


namespace Mimic
{
	MimicCore::MimicCore()
	{
		// initialise SDL_Window, SDL_Renderer, & GL_Context:
		Window = Window->Initialise("DMTK");
		Environment = Environment->Initialise(70.0f);

		// init glew:
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err) std::cerr << "Error: GLEW failed to initialise with message: " << glewGetErrorString(err) << std::endl;
		
		ApplicationRunning = true;
		/*glEnable(GL_DEPTH_TEST);*/
	}

	MimicCore::~MimicCore() { }

	std::shared_ptr<MimicCore> MimicCore::Initialise() 
	{ 
		return std::make_shared<MimicCore>(); 
	}

	void MimicCore::Update()
	{
		SDL_GL_SwapWindow(Window->_window);
		Environment->CalculateDeltaTime();

		// Update all GameObject model matrices:
		unsigned int length = GameObjects.size();
		for (unsigned int i = 0; i < length; i++) GameObjects[i]->Update();
	}

	std::shared_ptr<GameObject> MimicCore::AddEmptyGameObject()
	{
		std::shared_ptr<GameObject> emptyGameObject = std::make_shared<GameObject>();
		/*emptyGameObject->_mimicCore = this;*/

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
		std::shared_ptr<Camera> newCamera = std::make_shared<Camera>();
		newCamera->Self = newCamera;

		if (Cameras.size() < 1)
		{
			MainCamera = newCamera;
			newCamera->MainCamera = true;
		}
		Cameras.push_back(newCamera);
		return newCamera;
	}
}