#include <lowlevelsystems/Mimic.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#define SDL_MAIN_HANDLED

using namespace Mimic;

#undef main
int main(int argc, char* argv[])
{
	{
		// game engine code:
		std::shared_ptr<MimicCore> dmtkCore = MimicCore::Initialise();

		std::shared_ptr<GameObject> cameraObject = dmtkCore->AddEmptyGameObject("Camera_1");
		cameraObject->Position = glm::vec3(0.0f, 0.0f, 2.0f);
		std::shared_ptr<Camera> mainCamera = cameraObject->AddComponent<Camera>();
		mainCamera->Initialise(MimicCore::Window->GetAspectRatio(), 45.0f);
		dmtkCore->AddCamera(mainCamera, true);

		std::shared_ptr<DirectLight> light1 = dmtkCore->AddDirectLight();
		light1->Position = glm::vec3(1.0f, 0.0f, -1.0f);
		light1->Colour = glm::vec3(10.0f, 10.0f, 10.0f);

		/*std::shared_ptr<PointLight> light1 = dmtkCore->AddPointLight();
		light1->Position = glm::vec3(1.0f, 0.0f, -1.0f);
		light1->Colour = glm::vec3(10.0f, 10.0f, 10.0f);*/

		std::shared_ptr<GameObject> light1GameObject = dmtkCore->AddEmptyGameObject();
		light1GameObject->Scale = glm::vec3(0.1f);
		std::shared_ptr<ModelRenderer> light1Renderer = light1GameObject->AddComponent<ModelRenderer>();
		light1Renderer->Initialise("sphere.obj"); 
		auto light1Material = light1Renderer->GetMaterial<PBRMaterial>();
		light1Material->Emissive = glm::vec3(1.0f);

	/*	std::shared_ptr<DirectLight> light2 = dmtkCore->AddLight();
		light2->Position = glm::vec3(1.0f, 0.0f, 1.0f);
		light2->Colour = glm::vec3(10.0f, 10.0f, 10.0f);

		std::shared_ptr<GameObject> light2GameObject = dmtkCore->AddEmptyGameObject();
		light2GameObject->Scale = glm::vec3(0.3f);
		std::shared_ptr<ModelRenderer> light2Renderer = light2GameObject->AddComponent<ModelRenderer>();
		light2Renderer->Initialise("sphere.obj");
		auto light2Material = light2Renderer->GetMaterial<PBRMaterial>();
		light2Material->Emissive = glm::vec3(255.0f);*/

		/*std::shared_ptr<Texture> emptyTexture = MimicCore::ResourceManager->CreateResource<Texture>(
			glm::ivec2(800, 800),
			Texture::MIMIC_2D_TEXTURE,
			Texture::MIMIC_MODEL_TEXTURE_PARAMS,
			Texture::MIMIC_RGB,
			Texture::MIMIC_RGB);*/

		// model:
		std::shared_ptr<GameObject> mushroomGameObject = dmtkCore->AddEmptyGameObject();
		mushroomGameObject->Scale = glm::vec3(1.0f, 1.0f, 1.0f);
		mushroomGameObject->Position = glm::vec3(0.0f, 0.0f, -3.0f);
		std::shared_ptr<ModelRenderer> mushroomRenderer = mushroomGameObject->AddComponent<ModelRenderer>();
		mushroomRenderer->Initialise("cube.obj");
		auto mushroomMaterial = mushroomRenderer->GetMaterial<PBRMaterial>();

		// manually load in texture maps:
		auto metallicTexture = MimicCore::ResourceManager->LoadResource<Texture>("rustediron2_metallic.png");
		metallicTexture->SetType(TextureType::MIMIC_METALLIC);
		mushroomMaterial->SetTextureMap(metallicTexture);
		mushroomMaterial->ManualMode = true;

		constexpr float maxRotAngle = 2.0f * 3.141592653589793238462643383f;
		std::shared_ptr<PerformanceCounter> performanceCounter = PerformanceCounter::Initialise();

		dmtkCore->Start();

		while (dmtkCore->ApplicationRunning)
		{
			//performanceCounter->StartPerformanceCounter();

			// handle human interface devices:
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				ImGui_ImplSDL2_ProcessEvent(&event);
				switch (event.type)
				{
					case SDL_QUIT:
					{
						dmtkCore->ApplicationRunning = false;
						break;
					}

					case SDL_KEYDOWN: { 
						switch (event.key.keysym.sym)
						{
						case SDLK_ESCAPE:
							dmtkCore->ApplicationRunning = false;
							break;
							// cases for other keypresses
						}
						break; 
					}
					case SDL_KEYUP: { break; }
				}
			}

			// display to window:
			glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			float cubeYRotation = mushroomGameObject->Rotation.y;
			cubeYRotation += DeltaTime() * 1.8f;
			while (cubeYRotation > (maxRotAngle)) cubeYRotation -= maxRotAngle;
			mushroomGameObject->Rotation.y = cubeYRotation;

			dmtkCore->Update();

			dmtkCore->Draw();

			// gui:
			ImGui_ImplOpenGL3_NewFrame(); 
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();

			// light controls:
			ImGui::Begin("Light");
			ImGui::SliderFloat3("Position##l1", &(light1->Position[0]), -5.0f, 5.0f);
			ImGui::SliderFloat3("Direction##l2", &(light1->Direction[0]), -1.0f, 1.0f);
			ImGui::SliderFloat3("Colour##l3", &(light1->Colour[0]), 0.0f, 100.0f);
			light1GameObject->Position = light1->Position;
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

			// light controls:
			/*ImGui::Begin("Light2");
			ImGui::SliderFloat3("Position##l21", &(light2->Position[0]), -5.0f, 5.0f);
			ImGui::SliderFloat3("Colour##l22", &(light2->Colour[0]), 0.0f, 100.0f);
			light2GameObject->Position = light2->Position;
			ImGui::End();*/

			// model controls:
			ImGui::Begin("Model");
			ImGui::SliderFloat3("Position##m1", &(mushroomGameObject->Position[0]), -250.0f, 250.0f);
			ImGui::SliderFloat3("Rotation##m2", &(mushroomGameObject->Rotation[0]), -5.0f, 5.0f);
			ImGui::End();

			// material controls:
			ImGui::Begin("PBR Material");
			ImGui::ColorEdit3("Albedo##m1", &(mushroomMaterial->Albedo[0]));
			ImGui::ColorEdit3("Emissive##m2", &(mushroomMaterial->Emissive[0]));
			ImGui::SliderFloat("Roughness##m4", &(mushroomMaterial->Roughness), 0.001f, 1.0f);
			ImGui::SliderFloat("Metallic##m5", &(mushroomMaterial->Metallic), 0.001f, 1.0f);
			ImGui::SliderFloat("Ambient Occlusion##m6", &(mushroomMaterial->AmbientOcclusion), 0.0f, 1.0f);
			// ImGui::SliderFloat("Alpha##m6", &(mushroomMaterial->Alpha), 0.0f, 1.0f);
			ImGui::End();

			// material controls:
			ImGui::Begin("Camera");
			ImGui::SliderFloat3("Position##c1", &(mainCamera->GetGameObject()->Position[0]), -5.0f, 5.0f);
			ImGui::SliderFloat3("Rotation##c2", &(mainCamera->Orientation[0]), -maxRotAngle, maxRotAngle);
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			MimicCore::Window->SwapWindow();
			/*MIMIC_LOG_INFO("FPS: %", performanceCounter->GetFPS());*/
		}
	}
	return 0;
}