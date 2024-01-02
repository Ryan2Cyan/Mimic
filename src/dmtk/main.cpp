#include <mimic_engine/Mimic.h>

#include <array>
#include <algorithm>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#define SDL_MAIN_HANDLED

using namespace MimicEngine;
using namespace MimicUtility;

struct Simplex
{
	Simplex() : _size(0), _points({glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f) }) {}

	Simplex& operator=(const std::initializer_list<glm::vec3>& list)
	{
		for (auto v = list.begin(); v != list.end(); v++) _points[std::distance(list.begin(), v)] = *v;
		_size = list.size();
		return *this;
	}
	
	glm::vec3& operator[](const unsigned& i) 
	{
		return _points[i]; 
	}

	void PushFront(const glm::vec3& point)
	{
		_points = { point, _points[0], _points[1], _points[2] };
		_size = std::min(_size + 1, 4u);
	}

	unsigned GetSize() const
	{
		return _size;
	}

	auto Begin() const
	{
		return _points.begin();
	}

	auto End() const
	{
		return _points.end() - (4 - _size);
	}

private:
	std::array<glm::vec3, 4> _points;
	unsigned _size;
};

bool SameDirection(const glm::vec3& direction, const glm::vec3& ao)
{
	return glm::dot(direction, ao) > 0;
}

glm::vec3 Support(const std::shared_ptr<MeshCollider>& colA, const std::shared_ptr<MeshCollider>& colB, const glm::vec3& direction)
{
	auto a = colA->GetFarthestPoint(direction);
	auto b = colB->GetFarthestPoint(-direction);
	return a - b;
};

bool Line(Simplex& points, glm::vec3& direction)
{
	const glm::vec3 a = points[0];
	const glm::vec3 b = points[1];
	const glm::vec3 ab = b - a;
	const glm::vec3 ao = -a;

	if (SameDirection(ab, ao)) direction = glm::cross(glm::cross(ab, ao), ab);
	else
	{
		points = { a };
		direction = ao;
	}
	return false;
}

bool Triangle(Simplex& points, glm::vec3& direction)
{
	const glm::vec3 a = points[0];
	const glm::vec3 b = points[1];
	const glm::vec3 c = points[2];

	const glm::vec3 ab = b - a;
	const glm::vec3 ac = c - a;
	const glm::vec3 ao = -a;

	const glm::vec3 abc = glm::cross(ab, ac);

	if (SameDirection(glm::cross(abc, ac), ao))
	{
		if (SameDirection(ac, ao))
		{
			points = { a, c };
			direction = glm::cross(glm::cross(ac, ao), ac);
		}
		else return Line(points = { a, b }, direction);
	}
	else
	{
		if (SameDirection(glm::cross(ab, abc), ao)) return Line(points = { a, b }, direction);
		else
		{
			if (SameDirection(abc, ao)) direction = abc;
			else
			{
				points = { a, c, b };
				direction = -abc;
			}
		}
	}

	return false;
}

bool Tetrahedron(Simplex& points, glm::vec3& direction)
{
	const glm::vec3 a = points[0];
	const glm::vec3 b = points[1];
	const glm::vec3 c = points[2];
	const glm::vec3 d = points[3];

	const glm::vec3 ab = b - a;
	const glm::vec3 ac = c - a;
	const glm::vec3 ad = d - a;
	const glm::vec3 ao = -a;

	const glm::vec3 abc = glm::cross(ab, ac);
	const glm::vec3 acd = glm::cross(ac, ad);
	const glm::vec3 adb = glm::cross(ad, ab);

	if (SameDirection(abc, ao)) return Triangle(points = { a, b, c }, direction);
	if (SameDirection(acd, ao)) return Triangle(points = { a, c, d }, direction);
	if (SameDirection(adb, ao)) return Triangle(points = { a, d, b }, direction);
	
	return true;
}

bool NextSimplex(Simplex& points, glm::vec3& direction)
{
	switch (points.GetSize())
	{
		case 2: return Line(points, direction);
		case 3: return Triangle(points, direction);
		case 4: return Tetrahedron(points, direction);
		break;
	}
	MIMIC_LOG_WARNING("Error: GJK cannot identify point size of: %", points.GetSize());
	return false; 
}

// Source: https://winter.dev/articles/gjk-algorithm
// Source: https://www.youtube.com/watch?v=Qupqu1xe7Io&t=576s
// Source: https://www.medien.ifi.lmu.de/lehre/ss10/ps/Ausarbeitung_Beispiel.pdf
bool GJKCollisionDetection(const std::shared_ptr<MeshCollider>& colA, const std::shared_ptr<MeshCollider>& colB)
{
	// Get initial support point in any direction:
	glm::vec3 support = Support(colA, colB, glm::vec3(1.0f, 0.0f, 0.0f));
	MIMIC_DEBUG_LOG("First Support: %, %, %", support.x, support.y, support.z);

	// Simplex is an array of points, max count is 4:
	Simplex points;
	points.PushFront(support);

	// New direction is towards the origin:
	glm::vec3 direction = -support;

	while (true)
	{
		support = Support(colA, colB, direction);
		auto dot = glm::dot(support, direction);
		MIMIC_DEBUG_LOG("First Dot: %", dot);

		if(dot <= 0)
		{
			return false; // No collision
		}

		points.PushFront(support);
		if (NextSimplex(points, direction)) return true; // Collision
	}
}

#undef main
int main(int argc, char* argv[])
{
	{
		// Initialise core:
		std::shared_ptr<MimicEngine::MimicCore> mimicCore = MimicEngine::MimicCore::Initialise();

		// Initialise camera:
		std::shared_ptr<MimicEngine::Camera> camera = MimicEngine::Camera::Initialise(MimicEngine::MimicCore::GetCurrentAspect(), 45.0f);
		camera->SetPosition(glm::vec3(0.0f, 0.54f, 10.0f));
		camera->SetOrientation(glm::vec3(0.0, 0.0f, -1.0f));

		// Initialise direct lights:
		std::vector<std::shared_ptr<MimicEngine::DirectLight>> directLights =
		{
			MimicEngine::DirectLight::Initialise(glm::vec3(0.0f, 5.0f, -14.0f), glm::vec3(-0.25f), glm::vec3(1.0))
		};

		// Initialise scene models:
		std::shared_ptr<GameObject> cube0 = GameObject::Initialise(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		auto cube0ModelRenderer = cube0->AddComponent<ModelRenderer>();
		auto cube0PBRMaterial = PBRMaterial::Initialise();
		cube0PBRMaterial->SetAlbedo(glm::vec3(1.0f, 0.0f, 0.0f));
		cube0ModelRenderer->SetMaterial<PBRMaterial>(cube0PBRMaterial);
		cube0ModelRenderer->SetModel(MimicCore::ResourceManager->LoadResource<MimicEngine::Model>("cube.obj"));
		auto cube0BoxCollider = cube0->AddComponent<MeshCollider>();

		std::shared_ptr<GameObject> cube1 = GameObject::Initialise(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		auto cube1ModelRenderer = cube1->AddComponent<ModelRenderer>();
		auto cube1PBRMaterial = PBRMaterial::Initialise();
		cube1PBRMaterial->SetAlbedo(glm::vec3(0.0f, 0.0f, 1.0f));
		cube1ModelRenderer->SetMaterial<PBRMaterial>(cube1PBRMaterial);
		cube1ModelRenderer->SetModel(MimicCore::ResourceManager->LoadResource<MimicEngine::Model>("sphere.obj"));
		auto cube1BoxCollider = cube1->AddComponent<MeshCollider>();

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
		while (MimicCore::IsApplicationRunning())
		{
			// #############################################################################
			// Update scene:
			// #############################################################################
			mimicCore->Update();

			// User Input:
			auto camPos = camera->GetPosition();
			auto camSpeed = 10.0f;
			if (MimicCore::InputHandler->IsKey(SDLK_d)) camPos.x += (camSpeed * DeltaTime());
			if (MimicCore::InputHandler->IsKey(SDLK_a)) camPos.x -= (camSpeed * DeltaTime());
			if (MimicCore::InputHandler->IsKey(SDLK_w)) camPos.z -= (camSpeed * DeltaTime());
			if (MimicCore::InputHandler->IsKey(SDLK_s)) camPos.z += (camSpeed * DeltaTime());
			if (MimicCore::InputHandler->IsKey(SDLK_q)) camPos.y += (camSpeed * DeltaTime());
			if (MimicCore::InputHandler->IsKey(SDLK_e)) camPos.y -= (camSpeed * DeltaTime());
			camera->SetPosition(camPos);

			if (MimicCore::InputHandler->IsKey(SDLK_f)) cube0->Position.x -= (camSpeed * DeltaTime());
			if (MimicCore::InputHandler->IsKey(SDLK_h)) cube0->Position.x += (camSpeed * DeltaTime());
			if (MimicCore::InputHandler->IsKey(SDLK_t)) cube0->Position.y += (camSpeed * DeltaTime());
			if (MimicCore::InputHandler->IsKey(SDLK_g)) cube0->Position.y -= (camSpeed * DeltaTime());
			if (MimicCore::InputHandler->IsKey(SDLK_r)) cube0->Position.z += (camSpeed * DeltaTime());
			if (MimicCore::InputHandler->IsKey(SDLK_y)) cube0->Position.z -= (camSpeed * DeltaTime());

			// Collisions:
			if (GJKCollisionDetection(cube0BoxCollider, cube1BoxCollider))
			{
				MIMIC_DEBUG_LOG("Colliding");
				cube0PBRMaterial->SetAlbedo(glm::vec3(1.0f, 0.0f, 0.0f));
			}
			else
			{
				cube0PBRMaterial->SetAlbedo(glm::vec3(0.2f, 0.4f, 0.9f));
			}

			// #############################################################################
			// Render scene:
			// #############################################################################
			mimicCore->Draw();

			// #############################################################################
//			// GUI:
//			// #############################################################################
//			ImGui_ImplOpenGL3_NewFrame();
//			ImGui_ImplSDL2_NewFrame();
//			ImGui::NewFrame();
//
//			ImGui::Begin("Settings");
//
//			ImGui::Image((void*)directLights[0]->_renderDirectLight->GetDepthMapTextureId(), ImVec2(800, 800));
//			if (ImGui::CollapsingHeader("Sphere Transform"))
//			{
//				ImGui::SliderFloat3("Position##m1", &(sphereGameObject->Position[0]), -20.0f, 20.0f);
//				ImGui::SliderFloat3("Rotation##m2", &(sphereGameObject->Rotation[0]), -5.0f, 5.0f);
//				ImGui::SliderFloat3("Scale##m3", &(sphereGameObject->Scale[0]), 0.0f, 50.0f);
//			}
//			ImGui::Separator();
//
//			if (ImGui::CollapsingHeader("Sphere Material"))
//			{
//				ImGui::RadioButton("PBR##pbr_mat0", &shaderUsed, 0); ImGui::SameLine();
//				ImGui::RadioButton("Blinn Phong##pbr_mat1", &shaderUsed, 1);
//				switch (shaderUsed)
//				{
//					case 0:
//					{
//						//const char* items[] = { "None", "Marble", "Bark", "Foil", "Brick", "Ground", "Tiles 1", "Tiles 2", "Tiles 3" };
//						//ImGui::Combo("Material", &itemCurrent, items, IM_ARRAYSIZE(items));
//						//if (itemCurrent == 0)
//						//{
//						//	//// PBR material parameters:
//						//	//ImGui::ColorEdit3("Albedo##pbr_mat2", &(spherePBRMaterial.Albedo[0]));
//						//	//ImGui::ColorEdit3("Emissive##pbr_mat3", &(spherePBRMaterial.Emissive[0]));
//						//	//ImGui::SliderFloat("Roughness##pbr_mat4", &(spherePBRMaterial.Roughness), 0.001f, 1.0f);
//						//	//ImGui::RadioButton("Dialectric##pbr_mat5", &spherePBRMaterial.Metallic, 0); ImGui::SameLine();
//						//	//ImGui::RadioButton("Metallic##pbr_mat6", &spherePBRMaterial.Metallic, 1);
//						//	//ImGui::SliderFloat("Ambient Occlusion##pbr_mat7", &(spherePBRMaterial.AmbientOcclusion), 0.0f, 1.0f);
//						//}
//					}break;
//					case 1:
//					{
//						//// Blinn Phong material parameters:
//						//ImGui::ColorEdit3("Object Colour##bp_mat1", &(objectColour[0]));
//						//ImGui::SliderFloat("Ambient Strength##bp_mat3", &(ambientStrength), 0.0f, 1.0f);
//						//ImGui::SliderFloat("Specular Strength##bp_mat4", &(specularStrength), 0.0f, 1.0f);
//						//ImGui::SliderFloat("Shininess##bp_mat5", &(shininess), 0.0f, 100.0f);
//					}break;
//					default:
//					break;
//				}
//			}
//			ImGui::Separator();
//
//			if (ImGui::CollapsingHeader("Ground Transform"))
//			{
//				/*ImGui::SliderFloat3("Position##gm1", &(groundGameObject->Position[0]), -20.0f, 20.0f);
//				ImGui::SliderFloat3("Rotation##gm2", &(groundGameObject->Rotation[0]), -20.0f, 20.0f);
//				ImGui::SliderFloat3("Scale##gm3", &(groundGameObject->Scale[0]), 0.0f, 50.0f);*/
//			}
//			ImGui::Separator();
//
//			if (ImGui::CollapsingHeader("Ground Material"))
//			{
//				/*ImGui::ColorEdit3("Albedo##pbr_mat1", &(groundPBRMaterial.Albedo[0]));
//				ImGui::ColorEdit3("Emissive##pbr_mat2", &(groundPBRMaterial.Emissive[0]));
//				ImGui::SliderFloat("Roughness##pbr_mat3", &(groundPBRMaterial.Roughness), 0.001f, 1.0f);
//				ImGui::RadioButton("Dialectric##pbr_mat4", &groundPBRMaterial.Metallic, 0); ImGui::SameLine();
//				ImGui::RadioButton("Metallic##pbr_mat5", &groundPBRMaterial.Metallic, 1);
//				ImGui::SliderFloat("Ambient Occlusion##m6", &(groundPBRMaterial.AmbientOcclusion), 0.0f, 1.0f);*/
//			}
//			ImGui::Separator();
//
//			if (ImGui::CollapsingHeader("Camera"))
//			{
//				/*ImGui::SliderFloat3("Position##c1", &(camera->Position[0]), -20.0f, 20.0f);
//				ImGui::SliderFloat3("Orientation##c2", &(camera->Orientation[0]), -5.0f, 5.0f);
//				ImGui::SliderFloat2("Clipping Planes##c3", &(camera->ClippingPlane[0]), 0.001f, 500.0f);
//				ImGui::SliderFloat("FOV##c4", &(camera->Fov), 0.0f, 100.0f);*/
//			}
//			ImGui::Separator();
//
//			if (ImGui::CollapsingHeader("Direct Light Transform"))
//			{
//				ImGui::SliderFloat3("Position##dl1_2", &(directLights[0]->_renderDirectLight->Position[0]), -20.0f, 20.0f);
//				ImGui::SliderFloat3("Direction##dl2_2", &(directLights[0]->_renderDirectLight->Direction[0]), -1.0f, 1.0f);
//				ImGui::SliderFloat3("Colour##dl3_2", &(directLights[0]->_renderDirectLight->Colour[0]), 0.0f, 100.0f);
//			}
//			ImGui::Separator();
//
//			if (ImGui::CollapsingHeader("Framerate"))
//			{
//				// Fill an array of contiguous float values to plot
//				// Tip: If your float aren't contiguous but part of a structure, you can pass a pointer to your first float
//				// and the sizeof() of your structure in the "stride" parameter.
//				static float values[90] = {};
//				static int values_offset = 0;
//				static double refresh_time = 0.0;
//				if (refresh_time == 0.0) refresh_time = ImGui::GetTime();
//				while (refresh_time < ImGui::GetTime()) // Create data at fixed 60 Hz rate for the demo
//				{
//					static float phase = 0.0f;
//					values[values_offset] = ImGui::GetIO().Framerate;
//					values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
//					phase += 0.10f * values_offset;
//					refresh_time += 1.0f / 60.0f;
//				}
//
//				// Plots can display overlay texts (in this example, we will display an average value):
//				{
//					float average = 0.0f;
//					for (int n = 0; n < IM_ARRAYSIZE(values); n++)
//						average += values[n];
//					average /= (float)IM_ARRAYSIZE(values);
//					char overlay[32];
//					sprintf(overlay, "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//					ImGui::PlotLines("Lines", values, IM_ARRAYSIZE(values), values_offset, overlay, -1.0f, 1.0f, ImVec2(0, 80.0f));
//				}
//			}
//
//			ImGui::End();
//
//
//			ImGui::Render();
//			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			MimicCore::Window->SwapWindow();
		}
	}
}