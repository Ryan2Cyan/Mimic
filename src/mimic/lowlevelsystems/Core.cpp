#include "Core.h"


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
	}

	std::shared_ptr<GameObject> MimicCore::AddEmptyGameObject()
	{
		std::shared_ptr<GameObject> emptyGameObject = std::make_shared<GameObject>();
		emptyGameObject->Self = emptyGameObject;

		GameObjects.push_back(emptyGameObject);
		return emptyGameObject;
	}

	void MimicCore::AddGameObject(std::shared_ptr<GameObject> gameObject)
	{
		gameObject->Self = gameObject;
		GameObjects.push_back(gameObject);
	}
}