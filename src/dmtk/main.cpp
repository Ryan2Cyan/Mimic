#include <mimic_engine/Mimic.h>

#include <array>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp> 
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#define SDL_MAIN_HANDLED

using namespace MimicEngine;
using namespace MimicUtility;
using namespace MimicPhysics;

#undef main
int main(int argc, char* argv[])
{
	{
		// Initialise core.
		std::shared_ptr<MimicEngine::MimicCore> mimicCore = MimicEngine::MimicCore::Initialise();

		// Initialise camera [aspect ratio, and fov].
		std::shared_ptr<MimicEngine::Camera> camera = mimicCore->AddCamera(mimicCore->GetWindow()->GetAspectRatio(), 45.0f);
		camera->SetPosition(glm::vec3(0.0f, 0.0f, 10.0f));
		camera->SetOrientation(glm::vec3(0.0, 0.0f, -1.0f));

		// Initialise vector of direct lights [position, direction, & colour].
		std::vector<std::shared_ptr<MimicEngine::DirectLight>> directLights =
		{
			mimicCore->AddDirectLight(glm::vec3(0.0f, 5.0f, -14.0f), glm::vec3(-0.25f), glm::vec3(1.0))
		};

		// Load sfx: 
		const auto squeak = mimicCore->GetResourceManager()->LoadResource<AudioClip>("snd_squeak.ogg");

		// Initialise scene models.
		std::shared_ptr<GameObject> sphere = mimicCore->AddGameObject(glm::vec3(0.2f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		auto sphereModelRenderer = sphere->AddComponent<ModelRenderer>();
		auto spherePBRMaterial = sphereModelRenderer->GetMaterial<PBRMaterial>();
		auto sphereRigidbody = sphere->AddComponent<Rigidbody>();
		sphereModelRenderer->SetModel(mimicCore->GetResourceManager()->LoadResource<MimicEngine::Model>("sphere.obj"));
		auto sphereMeshCollider = sphere->AddComponent<MimicEngine::MeshCollider>();
		auto sphereAudioSource = sphere->AddComponent<MimicEngine::AudioSource>();
		sphereAudioSource->SetAudioClip(squeak);
		sphere->OnSelected = [&mat = spherePBRMaterial, &audioSrc = sphereAudioSource]() 
		{
			mat->SetAlbedo(glm::vec3(0.9f, 0.9f, 0.1f));
			audioSrc->PlaySfx();
		};
		sphere->OnUnselected = [&mat = spherePBRMaterial, &audioSrc = sphereAudioSource]() 
		{
			mat->SetAlbedo(glm::vec3(0.0f, 0.0f, 1.0f)); 
			audioSrc->PlaySfx();
		};
		sphere->WhileSelected = [&]()
		{

			const glm::vec2 res = mimicCore->GetWindow()->GetAspectRatio();
			const glm::ivec2 cursorPos = mimicCore->GetInputHandler()->GetCursorPosition();
			const auto distance = glm::length(sphere->Position - camera->GetPosition());
			const auto min = 1.0f * distance;
			const auto max = 2.0f * distance;
			auto rayClip = glm::vec4(
				(max * cursorPos.x) / res.x - min,
				min - (max * cursorPos.y) / res.y,
				-1.0f,
				1.0f
			);

			auto rayEye = glm::inverse(camera->GetProjectionMatrix()) * rayClip;
			rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);
			const auto rayWorld = camera->GetViewMatrix() * rayEye;
			sphere->Position = glm::vec3(rayWorld.x, rayWorld.y, sphere->Position.z);
		};

		std::shared_ptr<GameObject> cube = mimicCore->AddGameObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		auto cubeModelRenderer = cube->AddComponent<ModelRenderer>();
		auto cubePBRMaterial = cubeModelRenderer->GetMaterial<PBRMaterial>();
		auto cubeRigidbody = cube->AddComponent<Rigidbody>();
		cubePBRMaterial->SetAlbedo(glm::vec3(0.0f, 0.0f, 1.0f));
		cubeModelRenderer->SetModel(mimicCore->GetResourceManager()->LoadResource<MimicEngine::Model>("cube.obj"));
		auto cubeBoxCollider = cube->AddComponent<MimicEngine::BoxCollider>();
		cubeBoxCollider->SetSize(glm::vec3(1.0f));
		auto cubeAudioSource = cube->AddComponent<MimicEngine::AudioSource>();
		cubeAudioSource->SetAudioClip(squeak);
		cube->OnSelected = [&mat = cubePBRMaterial, &audioSrc = cubeAudioSource]()
		{
			mat->SetAlbedo(glm::vec3(0.9f, 0.9f, 0.1f)); 
			audioSrc->PlaySfx();
		};
		cube->OnUnselected = [&mat = cubePBRMaterial, &audioSrc = cubeAudioSource]()
		{ 
			mat->SetAlbedo(glm::vec3(0.0f, 0.0f, 1.0f)); 
			audioSrc->PlaySfx();
		};
		cube->WhileSelected = [&]() 
		{
			
			const glm::vec2 res = mimicCore->GetWindow()->GetAspectRatio();
			const glm::ivec2 cursorPos = mimicCore->GetInputHandler()->GetCursorPosition();
			const auto distance = glm::length(cube->Position - camera->GetPosition());
			const auto min = 1.0f * distance;
			const auto max = 2.0f * distance;
			auto rayClip = glm::vec4(
				(max * cursorPos.x) / res.x - min,
				min - (max * cursorPos.y) / res.y,
				-1.0f,
				1.0f
			);

			auto rayEye = glm::inverse(camera->GetProjectionMatrix()) * rayClip;
			rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);
			const auto rayWorld = camera->GetViewMatrix() * rayEye;
			cube->Position = glm::vec3(rayWorld.x, rayWorld.y, cube->Position.z);
		};

		/*std::shared_ptr<MimicEngine::GameObject> groundGameObject = GameObject::Initialise(glm::vec3(0.0f, -1.5f, 0.0f), glm::vec3(0.0f), glm::vec3(43.45f, -0.5f, 50.0f));
		auto groundModelRenderer = groundGameObject->AddComponent<ModelRenderer>();
		auto groundPBRMaterial = PBRMaterial::Initialise();
		groundModelRenderer->SetMaterial<PBRMaterial>(groundPBRMaterial);
		groundModelRenderer->SetModel(MimicCore::ResourceManager->LoadResource<MimicEngine::Model>("cube.obj"));*/


		mimicCore->Start();
		// #############################################################################
		// Game loop:
		// #############################################################################
		bool applicationRunning = true;
		while (mimicCore->IsApplicationRunning())
		{
			// #############################################################################
			// Update scene:
			// #############################################################################
			mimicCore->FixedUpdate();
			mimicCore->Update();

			// User-defined keyboard input. The MimicCore's input handler can check if any particular
			// key is down, pressed, or released each frame.
			auto camPos = camera->GetPosition();
			auto camSpeed = 10.0f;
			const auto inputHandler = mimicCore->GetInputHandler();
			if (inputHandler->IsKey(SDLK_d)) camPos.x += (camSpeed * DeltaTime());
			if (inputHandler->IsKey(SDLK_a)) camPos.x -= (camSpeed * DeltaTime());
			if (inputHandler->IsKey(SDLK_w)) camPos.z -= (camSpeed * DeltaTime());
			if (inputHandler->IsKey(SDLK_s)) camPos.z += (camSpeed * DeltaTime());
			if (inputHandler->IsKey(SDLK_q)) camPos.y += (camSpeed * DeltaTime());
			if (inputHandler->IsKey(SDLK_e)) camPos.y -= (camSpeed * DeltaTime());
			camera->SetPosition(camPos);

			/*if (inputHandler->IsKey(SDLK_f)) cube0->Position.x -= (camSpeed * DeltaTime());
			if (inputHandler->IsKey(SDLK_h)) cube0->Position.x += (camSpeed * DeltaTime());
			if (inputHandler->IsKey(SDLK_t)) cube0->Position.y += (camSpeed * DeltaTime());
			if (inputHandler->IsKey(SDLK_g)) cube0->Position.y -= (camSpeed * DeltaTime());
			if (inputHandler->IsKey(SDLK_r)) cube0->Position.z += (camSpeed * DeltaTime());
			if (inputHandler->IsKey(SDLK_y)) cube0->Position.z -= (camSpeed * DeltaTime());*/

			// #############################################################################
			// Render scene:
			// #############################################################################
			mimicCore->Draw();
			// #############################################################################
//			// GUI:
//			// #############################################################################
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();

			ImGui::ShowDemoWindow();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			mimicCore->GetWindow()->SwapWindow();
		}
	}
}
