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
			"../src/dmtk/additional-files/models/curuthers/curuthers.obj",
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

			// display to window:
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			dmtkCore->Update();

		/*	triangleShader->SetMat4("u_Model", triangleModelMatrix);
			triangleShader->SetMat4("u_View", viewMatrix);
			triangleShader->SetMat4("u_Projection", projMatrix);
			triangleMesh.Draw(triangleShader);*/

			// render loop:
		}
	}
	return 0;
}