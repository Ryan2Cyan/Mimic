#include <mimic/Core.h>
#include <iostream>
#define SDL_MAIN_HANDLED

using namespace Mimic;

void InitialiseSDL(std::shared_ptr<MimicCore> dmtkCore)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) throw std::runtime_error("Failed to initialize SDL");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	dmtkCore->Window = SDL_CreateWindow("DMTK", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	SDL_assert(dmtkCore->Window != NULL);

	dmtkCore->Renderer = SDL_CreateRenderer(dmtkCore->Window, -1, SDL_RENDERER_ACCELERATED);
	SDL_assert(dmtkCore->Renderer != NULL);
}


#undef main
int main(int argc, char* argv[])
{
	std::cout << "Hello Worldddd!" << std::endl;
	std::shared_ptr<MimicCore> dmtkCore = MimicCore::Initialize();
	InitialiseSDL(dmtkCore);
	dmtkCore->ApplicationRunning = true;

	while (dmtkCore->ApplicationRunning)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
				{
					dmtkCore->ApplicationRunning = false;
					break;
				}

				case SDL_KEYDOWN: { break; }
				case SDL_KEYUP: { break; }
			}
		}
	}

	dmtkCore->AddEntity();

	SDL_DestroyWindow(dmtkCore->Window);
	SDL_Quit();
	return 0;
}