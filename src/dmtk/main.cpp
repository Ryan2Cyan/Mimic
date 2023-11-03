#include <lowlevelsystems/Mimic.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>
#include "imgui.h"
#define SDL_MAIN_HANDLED

using namespace Mimic;

#undef main
int main(int argc, char* argv[])
{
	{
		// game engine code:
		std::shared_ptr<MimicCore> dmtkCore = MimicCore::Initialise();
		MIMIC_LOG_INFO("Hello World!");


		std::shared_ptr<GameObject> cameraObject = dmtkCore->AddEmptyGameObject("Camera_1");
		cameraObject->Position = glm::vec3(0.0f, 0.0f, 3.0f);
		std::shared_ptr<Camera> mainCamera = cameraObject->AddComponent<Camera>();
		mainCamera->Initialise(dmtkCore->GetAspectRatio(), 45.0f);
		dmtkCore->AddCamera(mainCamera, true);

		std::shared_ptr<GameObject> mushroomGameObject = dmtkCore->AddEmptyGameObject();
		mushroomGameObject->AddComponent<ModelRenderer>()->Initialise("Mushrooms1.obj", "BasicShader.glsl");
		std::shared_ptr<ModelRenderer> mushroomModelRenderer = mushroomGameObject->GetComponent<ModelRenderer>();
		mushroomModelRenderer->AddMaterial<BasicMaterial>();
		/*mushroomGameObject->GetComponent<ModelRenderer>()->Initialise("Mushrooms1.obj", "Texture.glsl");*/
		/*mushroomModelRenderer->Initialise( "Mushrooms1.obj", "Texture.glsl" );*/

		/*std::shared_ptr<GameObject> explorerGameObject = dmtkCore->AddEmptyGameObject();
		explorerGameObject->Position = glm::vec3(0.5f, 0.0f, 0.0f);
		std::shared_ptr<ModelRenderer> explorerModelRenderer = explorerGameObject->AddComponent<ModelRenderer>();
		explorerModelRenderer->Initialise( "Hero_Forge_Explorer_Demo.stl", basicShader );*/
		
		constexpr float maxRotAngle = 2.0f * 3.141592653589793238462643383f;

		std::shared_ptr<PerformanceCounter> performanceCounter = PerformanceCounter::Initialise();

		// #############################################################################
		// game loop:
	    // #############################################################################
		while (dmtkCore->ApplicationRunning)
		{
			performanceCounter->StartPerformanceCounter();
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

			float cubeYRotation = mushroomGameObject->Rotation.y;
			cubeYRotation += DeltaTime() * 1.8f;
			while (cubeYRotation > (maxRotAngle)) cubeYRotation -= maxRotAngle;
			/*explorerGameObject->Rotation.y = cubeYRotation;*/
			mushroomGameObject->Rotation.y = cubeYRotation;

			dmtkCore->Update();
			dmtkCore->Draw();

			performanceCounter->EndPerformanceCounter();
			/*MIMIC_LOG_INFO("FPS: %", performanceCounter->GetFPS());*/
		}
	}
	return 0;
}