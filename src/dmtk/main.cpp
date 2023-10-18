#include <mimic/Core.h>
#include <iostream>
#define SDL_MAIN_HANDLED

using namespace Mimic;

#undef main
int main(int argc, char* argv[])
{
	{
		// initialise:
		std::cout << "Hello Worldddd!" << std::endl;
		std::shared_ptr<MimicCore> dmtkCore = MimicCore::Initialize();
		std::shared_ptr<GameObject> newGameObject = dmtkCore->AddEmptyGameObject();
		std::shared_ptr<Renderer> newRenderer = newGameObject->AddComponent<Renderer>();
		const aiScene* scene = newRenderer->AssimpModelImport("../src/dmtk/additional-files/models/Mushrooms1_Obj/Mushrooms1.obj");

		// handle human interface devices:
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

			// render loop:
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// render triangle:
			newRenderer->Render();

			// display to window:
			SDL_GL_SwapWindow(dmtkCore->Window);
		}
	}
	return 0;
}