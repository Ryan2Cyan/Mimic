#include "Core.h"

namespace Mimic
{
	MimicCore::MimicCore()
	{
		 if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) throw std::runtime_error("Failed to initialize SDL");
		 SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		 SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		 SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		 Window = SDL_CreateWindow("DMTK", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		 SDL_assert(Window != NULL);

		 Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
		 SDL_assert(Renderer != NULL);

		 GLContext = SDL_GL_CreateContext(Window);
		 glewExperimental = GL_TRUE;
		 GLenum err = glewInit();
		 if (GLEW_OK != err) std::cerr << "Error: GLEW failed to initialise with message: " << glewGetErrorString(err) << std::endl;
		
		 ApplicationRunning = true;
		 _deltaTime = 0.0f;
		 /*glEnable(GL_DEPTH_TEST);*/
	}

	MimicCore::~MimicCore()
	{
		SDL_GL_DeleteContext(GLContext);
		SDL_DestroyWindow(Window);
		SDL_Quit();
	}

	std::shared_ptr<MimicCore> MimicCore::Initialize() 
	{ 
		return std::make_shared<MimicCore>(); 
	}

	void MimicCore::Update()
	{

	}

	 float MimicCore::DeltaTime() const noexcept
	 {
		 return _deltaTime;
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