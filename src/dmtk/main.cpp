#include <lowlevelsystems/Mimic.h>
#include <iostream>
#include "imgui.h"
#define SDL_MAIN_HANDLED

using namespace Mimic;

#undef main
int main(int argc, char* argv[])
{
	{
		// initialise:
		std::cout << "Hello Worldddd!" << std::endl;
		std::shared_ptr<MimicCore> dmtkCore = MimicCore::Initialise();
		std::shared_ptr<Camera> dmtkCamera = dmtkCore->AddNewCamera();
		std::shared_ptr<GameObject> newGameObject = dmtkCore->AddEmptyGameObject();
		std::shared_ptr<ModelRenderer> modelRenderer = newGameObject->AddComponent<ModelRenderer>();

		modelRenderer->Initialise(
			"../src/dmtk/additional-files/models/survival-guitar-backpack/source/Survival_BackPack_2/Survival_BackPack_2.fbx",
			"../src/mimic/shaders/VertexShader.txt",
			"../src/mimic/shaders/FragmentShader.txt");

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

			// render model:
			/*modelRenderer->Update();*/

			// display to window:
			dmtkCore->Update();
		}
	}
	return 0;
}