#include <mimic_engine/Mimic.h>

#include <array>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp> 
#include <iostream>
#include <cstdlib> 
#include <ctime>  
#define SDL_MAIN_HANDLED

using namespace MimicEngine;
using namespace MimicUtility;
using namespace MimicPhysics;

enum class SelectableObjectId
{
	BlueSphere, RedSphere, BlueCube, RedCube
};

struct SelectableObject
{
	SelectableObject(std::shared_ptr<GameObject> gameObject, SelectableObjectId id) : GameObject(gameObject), Id(id) {}
	SelectableObject() {}

	std::shared_ptr<GameObject> GameObject;
	SelectableObjectId Id;
	bool Selected = false;
};

#undef main
int main(int argc, char* argv[])
{
	srand(time(0));
	{
		// Initialise core.
		std::shared_ptr<MimicEngine::MimicCore> mimicCore = MimicEngine::MimicCore::Initialise();

		// Initialise camera [aspect ratio, and fov].
		std::shared_ptr<MimicEngine::Camera> camera = mimicCore->AddCamera(mimicCore->GetWindow()->GetAspectRatio(), 45.0f);
		camera->SetPosition(glm::vec3(0.0f, 0.0f, 10.0f));
		camera->SetOrientation(glm::vec3(0.0, 0.0f, -1.0f));

		// Initialise vector of direct lights [position, direction, & colour.
		std::vector<std::shared_ptr<MimicEngine::DirectLight>> directLights =
		{
			mimicCore->AddDirectLight(glm::vec3(0.0f, 5.0f, -14.0f), glm::vec3(-0.25f), glm::vec3(1.0))
		};

		// Load sfx.
		const auto squeakSFX = mimicCore->GetResourceManager()->LoadResource<AudioClip>("snd_squeak.ogg");
		const auto victorySFX = mimicCore->GetResourceManager()->LoadResource<AudioClip>("snd_dumbvictory.ogg");
		const auto failureSFX = mimicCore->GetResourceManager()->LoadResource<AudioClip>("snd_break2_c.ogg");

		// Initialise audio source.
		std::shared_ptr<GameObject> audio = mimicCore->AddGameObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		auto audioSource = audio->AddComponent<MimicEngine::AudioSource>();
		audioSource->SetAudioClip(squeakSFX);


		const auto red = glm::vec3(1.0f, 0.0f, 0.0f);
		const auto blue = glm::vec3(0.0f, 0.0f, 1.0f);
		const auto yellow = glm::vec3(0.9f, 0.9f, 0.1f);
		const auto scale = glm::vec3(0.4f);
		const auto bounds = glm::ivec2(6, 1);

		// Initialise scene models.
		// Blue Sphere:
		auto blueSphere = mimicCore->AddGameObject(glm::vec3(-6.7f, 3.7f, 0.0f), glm::vec3(0.0f), scale);
		auto blueSphereModelRenderer = blueSphere->AddComponent<ModelRenderer>();
		auto blueSpherePBRMaterial = blueSphereModelRenderer->GetMaterial<PBRMaterial>();
		blueSpherePBRMaterial->SetAlbedo(blue);
		blueSphereModelRenderer->SetModel(mimicCore->GetResourceManager()->LoadResource<MimicEngine::Model>("sphere.obj"));
	
		// Red Sphere:
		auto redSphere = mimicCore->AddGameObject(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(0.0f), scale);
		auto redSphereModelRenderer = redSphere->AddComponent<ModelRenderer>();
		auto redSpherePBRMaterial = redSphereModelRenderer->GetMaterial<PBRMaterial>();
		redSpherePBRMaterial->SetAlbedo(red);
		redSphereModelRenderer->SetModel(mimicCore->GetResourceManager()->LoadResource<MimicEngine::Model>("sphere.obj"));

		// Blue Cube:
		auto blueCube = mimicCore->AddGameObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), scale);
		auto blueCubeModelRenderer = blueCube->AddComponent<ModelRenderer>();
		auto blueCubePBRMaterial = blueCubeModelRenderer->GetMaterial<PBRMaterial>();
		blueCubePBRMaterial->SetAlbedo(blue);
		blueCubeModelRenderer->SetModel(mimicCore->GetResourceManager()->LoadResource<MimicEngine::Model>("cube.obj"));

		// Red Cube:
		auto redCube = mimicCore->AddGameObject(glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(0.0f), scale);
		auto redCubeModelRenderer = redCube->AddComponent<ModelRenderer>();
		auto redCubePBRMaterial = redCubeModelRenderer->GetMaterial<PBRMaterial>();
		redCubePBRMaterial->SetAlbedo(red);
		redCubeModelRenderer->SetModel(mimicCore->GetResourceManager()->LoadResource<MimicEngine::Model>("cube.obj"));

		// Initialise Game Elements.
		std::vector<SelectableObject> selectableGameObjects = {
			SelectableObject(blueSphere, SelectableObjectId::BlueSphere),
			SelectableObject(redSphere, SelectableObjectId::RedSphere),
			SelectableObject(blueCube, SelectableObjectId::BlueCube),
			SelectableObject(redCube, SelectableObjectId::RedCube)
		};
		bool victory = true;
		int currentlySelected;
		int score = 0;

		const auto disableAll = [&]()
		{

			for (size_t i = 0; i < selectableGameObjects.size(); i++)
			{
				selectableGameObjects[i].Selected = false;
				switch (i)
				{
					case 0: blueSpherePBRMaterial->SetAlbedo(blue);
						break;
					case 1: redSpherePBRMaterial->SetAlbedo(red);
						break;
					case 2: blueCubePBRMaterial->SetAlbedo(blue);
						break;
					case 3: redCubePBRMaterial->SetAlbedo(red);
						break;
					default: break;
				}
				
			}
		};

		blueSphere->OnSelected = [&]()
		{
			disableAll();
			blueSpherePBRMaterial->SetAlbedo(yellow);
			audioSource->StopSfx();
			audioSource->SetAudioClip(squeakSFX);
			audioSource->PlaySfx();
			selectableGameObjects[0].Selected = true;
		};

		redSphere->OnSelected = [&]()
		{
			disableAll();
			redSpherePBRMaterial->SetAlbedo(yellow);
			audioSource->StopSfx();
			audioSource->SetAudioClip(squeakSFX);
			audioSource->PlaySfx();
			selectableGameObjects[1].Selected = true;
		};

		blueCube->OnSelected = [&]()
		{
			disableAll();
			blueCubePBRMaterial->SetAlbedo(yellow);
			audioSource->StopSfx();
			audioSource->SetAudioClip(squeakSFX);
			audioSource->PlaySfx();
			selectableGameObjects[2].Selected = true;
		};

		redCube->OnSelected = [&]()
		{
			disableAll();
			redCubePBRMaterial->SetAlbedo(yellow);
			audioSource->StopSfx();
			audioSource->SetAudioClip(squeakSFX);
			audioSource->PlaySfx();
			selectableGameObjects[3].Selected = true;
		};

		// Initialise GUI elements.
		const auto resolution = mimicCore->GetWindow()->GetAspectRatio();
		std::shared_ptr<GameObject> button = mimicCore->AddGameObject(glm::vec3(resolution.x / 2.0f, resolution.y * 0.9f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		auto buttonGUI = button->AddComponent<Button>();
		buttonGUI->SetName("Submit");
		buttonGUI->SetSize(glm::vec2(400.0f, 100.0f));
		buttonGUI->SetFontScale(3.0f);
		buttonGUI->SetColourHSV(glm::vec3(0.4f, 0.6f, 0.55f));
		buttonGUI->SetHoverColourHSV(glm::vec3(0.4f, 0.6f, 0.65f));
		buttonGUI->SetActiveColourHSV(glm::vec3(0.4f, 0.6f, 0.7f));
		buttonGUI->OnPressed = [&]()
		{
			for (int i = 0; i < selectableGameObjects.size(); i++)
			{
				if (i == currentlySelected)
				{
					if (selectableGameObjects[i].Selected)
					{
						audioSource->SetAudioClip(victorySFX);
						audioSource->PlaySfx();
						victory = true;
						score++;
						disableAll();
					}
					else
					{
						audioSource->SetAudioClip(failureSFX);
						audioSource->PlaySfx();
						disableAll();
					}
				}
			}
		};

		std::shared_ptr<GameObject> text = mimicCore->AddGameObject(glm::vec3(resolution.x / 2.0f, resolution.y * 0.05f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		auto textGUI = text->AddComponent<Text>();
		textGUI->SetMessage("");
		textGUI->SetFontScale(3.0f);

		std::shared_ptr<GameObject> scoreText = mimicCore->AddGameObject(glm::vec3(resolution.x * 0.95, resolution.y * 0.05f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		auto scoreTextGUI = scoreText->AddComponent<Text>();
		scoreTextGUI->SetMessage("");
		scoreTextGUI->SetFontScale(3.0f);
		scoreTextGUI->SetColourHSV(glm::vec3(0.3f, 0.7f, 0.9f));

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

			if (victory)
			{
				currentlySelected = (rand() % 3) + 0;
				text->SetActive(true);
				std::string selected;				
				switch (currentlySelected)
				{
					case 0: selected = "blue sphere";
						break;
					case 1: selected = "red sphere";
						break;
					case 2: selected = "blue cube";
						break;
					case 3: selected = "red cube";
						break;
					default: break;
				}
				std::vector<glm::ivec2> positions;
				for (auto& gameObject : selectableGameObjects)
				{
					const auto rangeX = bounds.x - (-bounds.x) + 1;
					float randX = rand() % rangeX + (-bounds.x);
					const auto rangeY = bounds.y - (-bounds.y) + 1;
					float randY = rand() % rangeY + (-bounds.y);
					auto pos = glm::ivec2(randX, randY);

					if (positions.empty())
					{
						gameObject.GameObject->Position = glm::vec3(randX, randY, 0.0f);
						positions.push_back(pos);
					}
					else
					{
						bool findingPosition = true;
						while (findingPosition)
						{
							for (const auto position : positions)
							{
								if (pos == position)
								{
									randX = rand() % rangeX + (-bounds.x);
									randY = rand() % rangeY + (-bounds.y);
									pos = glm::ivec2(randX, randY);
									continue;
								}
							}
							findingPosition = false;
						}

						gameObject.GameObject->Position = glm::vec3(pos.x, pos.y, 0.0f);
						findingPosition = false;
						pos = glm::ivec2(randX, randY);
						positions.push_back(pos);
					}
				}
				textGUI->SetMessage("Select the: " + selected);
				scoreTextGUI->SetMessage("Score: " + std::to_string(score));
				victory = false;
			}
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



			// #############################################################################
			// Render scene:
			// #############################################################################
			mimicCore->Draw();

			// #############################################################################
			// GUI:
			// #############################################################################
			mimicCore->GuiUpdate();
		}
	}
}
