#include <lowlevelsystems/Mimic.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#include "imgui.h"
#define SDL_MAIN_HANDLED

using namespace Mimic;

#undef main
int main(int argc, char* argv[])
{
	{
		// initialise:
		std::cout << "Hello Worldddd!" << std::endl;
		// core & model renderer are initialised:
		std::shared_ptr<MimicCore> dmtkCore = MimicCore::Initialise();
		std::shared_ptr<Camera> dmtkCamera = dmtkCore->AddNewCamera();
		std::shared_ptr<GameObject> newGameObject = dmtkCore->AddEmptyGameObject();
		std::shared_ptr<ModelRenderer> modelRenderer = newGameObject->AddComponent<ModelRenderer>();

		// initialisation goes correctly for the model, vertex, & fragment shaders:
		modelRenderer->Initialise(
			"../src/dmtk/additional-files/models/Mushrooms1_Obj/Mushrooms1_Obj/Mushrooms1.obj",
			"../src/dmtk/additional-files/shaders/VertexShader.txt",
			"../src/dmtk/additional-files/shaders/FragmentShader.txt");
		
		constexpr float maxRotAngle = 2.0f * 3.141592653589793238462643383f;


		// #############################################################################
		// game loop:
	    // #############################################################################
		while (dmtkCore->ApplicationRunning)
		{
			// handle human interface devices:
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

			// display to window:
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			float cubeYRotation = newGameObject->Rotation.y;
			cubeYRotation += DeltaTime() * 1.8f;
			while (cubeYRotation > (maxRotAngle)) cubeYRotation -= maxRotAngle;
			newGameObject->Rotation.y = cubeYRotation;

			dmtkCore->Update();
		}
	}
	return 0;
}