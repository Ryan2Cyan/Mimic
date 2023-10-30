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
		std::cout << "Hello Worldddd!" << std::endl;
		
		// game engine code:
		std::shared_ptr<MimicCore> dmtkCore = MimicCore::Initialise();
		std::shared_ptr<Shader> basicShader = dmtkCore->ResourceManager->LoadResource<Shader>("shaders/Texture.glsl");
		basicShader->Name = "Basic_Shader";


		std::shared_ptr<GameObject> cameraObject = dmtkCore->AddEmptyGameObject();
		cameraObject->Position = glm::vec3(0.0f, 0.0f, 3.0f);
		std::shared_ptr<Camera> mainCamera = cameraObject->AddComponent<Camera>();
		mainCamera->Initialise(dmtkCore->Window->AspectRatio, 45.0f);
		dmtkCore->AddCamera(mainCamera, true);


		std::shared_ptr<GameObject> paladinGameObject = dmtkCore->AddEmptyGameObject();
		std::shared_ptr<ModelRenderer> paladinModelRenderer = paladinGameObject->AddComponent<ModelRenderer>();
		paladinModelRenderer->Initialise( "models/Mushrooms1_Obj/Mushrooms1_Obj/Mushrooms1.obj", basicShader );

		/*std::shared_ptr<GameObject> explorerGameObject = dmtkCore->AddEmptyGameObject();
		explorerGameObject->Position = glm::vec3(0.5f, 0.0f, 0.0f);
		std::shared_ptr<ModelRenderer> explorerModelRenderer = explorerGameObject->AddComponent<ModelRenderer>();
		explorerModelRenderer->Initialise( "../src/dmtk/additional-files/models/Hero_Forge_Explorer_Demo/Hero_Forge_Explorer_Demo.stl", basicShader );*/
		
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

			float cubeYRotation = paladinGameObject->Rotation.y;
			cubeYRotation += DeltaTime() * 1.8f;
			while (cubeYRotation > (maxRotAngle)) cubeYRotation -= maxRotAngle;
			/*explorerGameObject->Rotation.y = cubeYRotation;*/
			paladinGameObject->Rotation.y = cubeYRotation;

			dmtkCore->Update();
			dmtkCore->Draw();
		}
	}
	return 0;
}