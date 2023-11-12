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
	// game engine code:
	std::shared_ptr<MimicCore> dmtkCore = MimicCore::Initialise();
	MIMIC_LOG_INFO("Hello World!");

	std::shared_ptr<GameObject> cameraObject = dmtkCore->AddEmptyGameObject("Camera_1");
	cameraObject->Position = glm::vec3(0.0f, 0.0f, 2.0f);
	std::shared_ptr<Camera> mainCamera = cameraObject->AddComponent<Camera>();
	mainCamera->Initialise(glm::vec2(800.0f, 800.0f), 45.0f);
	dmtkCore->AddCamera(mainCamera, true);

	std::shared_ptr<DirectLight> light1 = dmtkCore->AddLight();
	light1->Position = glm::vec3(1.0f, 0.0f, -5.0f);
	light1->Colour = glm::vec3(30.0f, 30.0f, 30.0f);

	std::shared_ptr<GameObject> mushroomGameObject = dmtkCore->AddEmptyGameObject();
	mushroomGameObject->Scale = glm::vec3(1.00f, 1.00f, 1.00f);
	mushroomGameObject->Position = glm::vec3(0.0f, 0.0f, -3.0f);

	std::shared_ptr<ModelRenderer> mushroomRenderer = mushroomGameObject->AddComponent<ModelRenderer>();
	mushroomRenderer->Initialise("normal_rock_sphere.obj");
	
	constexpr float maxRotAngle = 2.0f * 3.141592653589793238462643383f;
	std::shared_ptr<PerformanceCounter> performanceCounter = PerformanceCounter::Initialise();

	dmtkCore->Start();

	while (dmtkCore->ApplicationRunning)
	{
		// performanceCounter->StartPerformanceCounter();

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
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float cubeYRotation = mushroomGameObject->Rotation.x;
		cubeYRotation += DeltaTime() * 1.8f;
		while (cubeYRotation > (maxRotAngle)) cubeYRotation -= maxRotAngle;
		mushroomGameObject->Rotation.x = cubeYRotation;
		dmtkCore->Update();
		
		dmtkCore->Draw();

			// performanceCounter->EndPerformanceCounter();
			/*MIMIC_LOG_INFO("FPS: %", performanceCounter->GetFPS());*/
	}
	return 0;
}