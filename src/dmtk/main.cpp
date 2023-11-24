//#include <lowlevelsystems/Mimic.h>
#include <renderengine/Light.h>
#include <renderengine/Mesh.h>
#include <renderengine/Model.h>
#include <renderengine/Renderer.h>
#include <renderengine/RenderTexture.h>
#include <renderengine/Shader.h>
#include <renderengine/Texture.h>
#include <renderengine/Vertex.h>
#include <renderengine/Window.h>
#include <renderengine/Camera.h>
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
using namespace MimicRender;

#undef main
int main(int argc, char* argv[])
{
	{
		const std::string assetPath = "../assets/";
		// render engine code:
		std::shared_ptr<Window> window = Window::Initialise("Mimic Render Library Test");

		// initialise renderer:
		std::shared_ptr<Renderer> renderer = Renderer::Initialise();

		// create shader:
		const std::shared_ptr<Shader> pbrShader = Shader::Initialise(assetPath + "shaders/PBRShader.glsl");

		// load shader subroutine uniforms:
		const unsigned int albedoSubroutineUniform = pbrShader->GetSubroutineUniform(GL_FRAGMENT_SHADER, "AlbedoMode");
		const unsigned int normalSubroutineUniform = pbrShader->GetSubroutineUniform(GL_FRAGMENT_SHADER, "NormalMode");
		const unsigned int roughnessSubroutineUniform = pbrShader->GetSubroutineUniform(GL_FRAGMENT_SHADER, "RoughnessMode");
		const unsigned int metallicSubroutineUniform = pbrShader->GetSubroutineUniform(GL_FRAGMENT_SHADER, "MetallicMode");
		std::vector<unsigned int> _subroutineUniformIndices = { albedoSubroutineUniform, normalSubroutineUniform, roughnessSubroutineUniform, metallicSubroutineUniform };
		
		// load shader subroutine indices (functions):
		const unsigned int albedoAuto = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateAlbedoAutoTexture");
		const unsigned int albedoManual = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateAlbedoManual");

		const unsigned int normalAuto = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateNormalAutoTexture");
		const unsigned int normalManual = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateNormalManual");

		const unsigned int roughnessAuto = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateRoughnessAutoTexture");
		const unsigned int roughnessManual = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateRoughnessManual");

		const unsigned int metallicAuto = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateMetallicAutoTexture");
		const unsigned int metallicManual = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateMetallicManual");

		//std::function<void()> materialOnDrawLambda = [&]()
		//{
		//	// load albedo (map_kd):
		//	if (!_albedoTexture.expired() && !ManualMode)
		//	{
		//		_subroutineIndices[_albedoSubroutineUniform] = _autoAlbedo;
		//		shader->SetTexture("u_AlbedoMap", _albedoTexture.lock()->_id, 1); // texture unit slots start at 1.
		//	}
		//	else
		//	{
		//		_subroutineIndices[_albedoSubroutineUniform] = _manualAlbedo;
		//		shader->SetVector3("u_Albedo", Albedo);
		//	}

		//	// load roughness(map_ks):
		//	if (!_roughnessTexture.expired() && !ManualMode)
		//	{
		//		_subroutineIndices[_roughnessSubroutineUniform] = _autoRoughness;
		//		shader->SetTexture("u_RoughnessMap", _roughnessTexture.lock()->_id, 2);
		//	}
		//	else
		//	{
		//		_subroutineIndices[_roughnessSubroutineUniform] = _manualRoughness;
		//		shader->SetFloat("u_Roughness", Roughness);
		//	}


		//	// load normals (map_Bump):
		//	if (!_normalTexture.expired() && !ManualMode)
		//	{
		//		_subroutineIndices[_normalSubroutineUniform] = _autoNormal;
		//		shader->SetTexture("u_NormalMap", _normalTexture.lock()->_id, 3);
		//	}
		//	else _subroutineIndices[_normalSubroutineUniform] = _manualNormal;

		//	// load metallic (must be specified by user):
		//	if (!_metallicTexture.expired() && !ManualMode)
		//	{
		//		_subroutineIndices[_metallicSubroutineUniform] = _autoMetallic;
		//		shader->SetTexture("u_MetallicMap", _metallicTexture.lock()->_id, 4);
		//	}
		//	else
		//	{
		//		_subroutineIndices[_metallicSubroutineUniform] = _manualMetallic;
		//		shader->SetFloat("u_Metallic", Metallic);
		//	}

		//	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, _subroutineIndices.size(), _subroutineIndices.data());

		//	/*shader->SetInt("u_IrradianceMap", 5);
		//	glActiveTexture(GL_TEXTURE5);
		//	glBindTexture(GL_TEXTURE_CUBE_MAP, MimicCore::EnvironmentCubeMap->_irradianceRenderTexture->_texture->_id);

		//	shader->SetInt("u_PrefilterMap", 6);
		//	glActiveTexture(GL_TEXTURE6);
		//	glBindTexture(GL_TEXTURE_CUBE_MAP, MimicCore::EnvironmentCubeMap->_prefilteredMapRenderTexture->_texture->_id);

		//	shader->SetInt("u_BRDFLookupTexture", 7);
		//	glActiveTexture(GL_TEXTURE7);
		//	glBindTexture(GL_TEXTURE_2D, MimicCore::EnvironmentCubeMap->_brdfConvolutedRenderTexture->_texture->_id);*/

		//	shader->SetVector3("u_Emissive", Emissive);
		//	shader->SetFloat("u_Alpha", Alpha);
		//	shader->SetFloat("u_AmbientOcclusion", AmbientOcclusion);

		//	// direct lights:
		//	//const std::vector<std::shared_ptr<DirectLight>> directLights = MimicCore::_directLights;
		//	//for (int i = 0; i < directLights.size(); i++)
		//	//{
		//	//	const std::string currentLight = "u_DirectLights[" + std::to_string(i) + "]";

		//	//	shader->SetVector3((currentLight + ".direction").c_str(), glm::normalize(-directLights[i]->Direction));
		//	//	const glm::vec4 colour = glm::vec4(directLights[i]->Colour.x, directLights[i]->Colour.y, directLights[i]->Colour.z, 1.0f);
		//	//	shader->SetVector4((currentLight + ".colour").c_str(), colour);
		//	//}
		//	//shader->SetInt("u_DirectLightsCount", directLights.size());


		//	//// point lights:
		//	//const std::vector<std::shared_ptr<PointLight>> pointLights = MimicCore::_pointLights;
		//	//for (int i = 0; i < pointLights.size(); i++)
		//	//{
		//	//	const std::string currentLight = "u_PointLights[" + std::to_string(i) + "]";

		//	//	shader->SetVector3((currentLight + ".position").c_str(), pointLights[i]->Position);
		//	//	const glm::vec4 colour = glm::vec4(pointLights[i]->Colour.x, pointLights[i]->Colour.y, pointLights[i]->Colour.z, 1.0f);
		//	//	shader->SetVector4((currentLight + ".colour").c_str(), colour);
		//	//	shader->SetFloat((currentLight + ".constant").c_str(), pointLights[i]->Constant);
		//	//	shader->SetFloat((currentLight + ".linear").c_str(), pointLights[i]->Linear);
		//	//	shader->SetFloat((currentLight + ".quadratic").c_str(), pointLights[i]->Quadratic);
		//	//}
		//	//shader->SetInt("u_PointLightsCount", pointLights.size());
		//};

		// create camera:
		std::shared_ptr<Camera> camera = Camera::Initialise(window->GetAspectRatio(), 45.0f);

		// create mesh:
		const std::vector<unsigned int> indices{
			2, 6, 7,
			2, 3, 7,

			0, 4, 5,
			0, 1, 5,

			0, 2, 6,
			0, 4, 6,

			1, 3, 7,
			1, 5, 7,

			0, 2, 3,
			0, 1, 3,

			4, 6, 7,
			4, 5, 7
		};

		const std::vector<std::shared_ptr<Vertex>> vertices{
			Vertex::Initialise(glm::vec3(-1.0f, -1.0f, 0.5f), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec2(0.0)),
			Vertex::Initialise(glm::vec3(1.0f, 1.0f, 0.5f), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec2(0.0)),
			Vertex::Initialise(glm::vec3(1.0f, 1.0f, 0.5f), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec2(0.0)),
			Vertex::Initialise(glm::vec3(-1.0f, -1.0f, -0.5f), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec2(0.0)),
			Vertex::Initialise(glm::vec3(1.0f, -1.0f, -0.5f), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec2(0.0)),
			Vertex::Initialise(glm::vec3(-1.0f, 1.0f, -0.5f), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec2(0.0)),
			Vertex::Initialise(glm::vec3(1.0f, 1.0f, -0.5f), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec2(0.0)),
		};

		std::shared_ptr<Mesh> cubeMesh = Mesh::Initialise(vertices, indices);

		// create model:
		std::shared_ptr<Model> cubeModel = Model::Initialise();
		cubeModel->AddMesh(cubeMesh);

		// create textures:
		std::shared_ptr<Texture> albedoTexture = Texture::Initialise(assetPath + "models/Normal_Rock_Sphere/rustediron2_basecolor.png", TextureType::MIMIC_ALBEDO);

		// render loop:
		bool applicationRunning = true;
		while (applicationRunning)
		{
			// handle human interface devices:
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				ImGui_ImplSDL2_ProcessEvent(&event);
				switch (event.type)
				{
					case SDL_QUIT:
					{
						applicationRunning = false;
						break;
					}

					case SDL_KEYDOWN:
					{
						switch (event.key.keysym.sym)
						{
							case SDLK_ESCAPE:
							applicationRunning = false;
							break;
						}
						break;
					}
					case SDL_KEYUP: { break; }
				}
			}
		
			glClearColor(0.37f, 0.33f, 0.17f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			camera->Update();

			// send meshes to renderer:
			// cubeModel->QueMeshesToDraw(pbrShader, materialOnDrawLambda, renderer);
			
			// draw:
			renderer->Draw(camera);

			window->SwapWindow();
		}
	}
	return 0;
}


		// game engine code:
		// std::shared_ptr<MimicCore> dmtkCore = MimicCore::Initialise();

		//std::shared_ptr<GameObject> cameraObject = dmtkCore->AddEmptyGameObject("Camera_1");
		//cameraObject->Position = glm::vec3(0.0f, 0.0f, 2.0f);
		//std::shared_ptr<Camera> mainCamera = cameraObject->AddComponent<Camera>();
		//mainCamera->Initialise(MimicCore::Window->GetAspectRatio(), 45.0f);
		//dmtkCore->AddCamera(mainCamera, true);

		//std::shared_ptr<DirectLight> light1 = dmtkCore->AddDirectLight();
		//light1->Position = glm::vec3(1.0f, 0.0f, -1.0f);
		//light1->Colour = glm::vec3(10.0f, 10.0f, 10.0f);

		///*std::shared_ptr<PointLight> light1 = dmtkCore->AddPointLight();
		//light1->Position = glm::vec3(1.0f, 0.0f, -1.0f);
		//light1->Colour = glm::vec3(10.0f, 10.0f, 10.0f);*/

		//std::shared_ptr<GameObject> light1GameObject = dmtkCore->AddEmptyGameObject();
		//light1GameObject->Scale = glm::vec3(0.1f);
		//std::shared_ptr<ModelRenderer> light1Renderer = light1GameObject->AddComponent<ModelRenderer>();
		//light1Renderer->Initialise("sphere.obj"); 
		//auto light1Material = light1Renderer->GetMaterial<PBRMaterial>();
		//light1Material->Emissive = glm::vec3(1.0f);

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

		//// model:
		//std::shared_ptr<GameObject> mushroomGameObject = dmtkCore->AddEmptyGameObject();
		//mushroomGameObject->Scale = glm::vec3(1.0f, 1.0f, 1.0f);
		//mushroomGameObject->Position = glm::vec3(0.0f, 0.0f, -3.0f);
		//std::shared_ptr<ModelRenderer> mushroomRenderer = mushroomGameObject->AddComponent<ModelRenderer>();
		//mushroomRenderer->Initialise("cube.obj");
		//auto mushroomMaterial = mushroomRenderer->GetMaterial<PBRMaterial>();

		//// manually load in texture maps:
		//auto metallicTexture = MimicCore::ResourceManager->LoadResource<Texture>("rustediron2_metallic.png");
		//metallicTexture->SetType(TextureType::MIMIC_METALLIC);
		//mushroomMaterial->SetTextureMap(metallicTexture);
		//mushroomMaterial->ManualMode = true;

		//constexpr float maxRotAngle = 2.0f * 3.141592653589793238462643383f;
		//std::shared_ptr<PerformanceCounter> performanceCounter = PerformanceCounter::Initialise();

		//dmtkCore->Start();

		//while (dmtkCore->ApplicationRunning)
		//{
		//	//performanceCounter->StartPerformanceCounter();

		//	// handle human interface devices:
		//	SDL_Event event;
		//	while (SDL_PollEvent(&event))
		//	{
		//		ImGui_ImplSDL2_ProcessEvent(&event);
		//		switch (event.type)
		//		{
		//			case SDL_QUIT:
		//			{
		//				dmtkCore->ApplicationRunning = false;
		//				break;
		//			}

		//			case SDL_KEYDOWN: { 
		//				switch (event.key.keysym.sym)
		//				{
		//				case SDLK_ESCAPE:
		//					dmtkCore->ApplicationRunning = false;
		//					break;
		//					// cases for other keypresses
		//				}
		//				break; 
		//			}
		//			case SDL_KEYUP: { break; }
		//		}
		//	}

		//	// display to window:
		//	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//	float cubeYRotation = mushroomGameObject->Rotation.y;
		//	cubeYRotation += DeltaTime() * 1.8f;
		//	while (cubeYRotation > (maxRotAngle)) cubeYRotation -= maxRotAngle;
		//	mushroomGameObject->Rotation.y = cubeYRotation;

		//	dmtkCore->Update();

		//	dmtkCore->Draw();

		//	// gui:
		//	ImGui_ImplOpenGL3_NewFrame(); 
		//	ImGui_ImplSDL2_NewFrame();
		//	ImGui::NewFrame();

		//	// light controls:
		//	ImGui::Begin("Light");
		//	ImGui::SliderFloat3("Position##l1", &(light1->Position[0]), -5.0f, 5.0f);
		//	ImGui::SliderFloat3("Direction##l2", &(light1->Direction[0]), -1.0f, 1.0f);
		//	ImGui::SliderFloat3("Colour##l3", &(light1->Colour[0]), 0.0f, 100.0f);
		//	light1GameObject->Position = light1->Position;
		//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//	ImGui::End();

		//	// light controls:
		//	/*ImGui::Begin("Light2");
		//	ImGui::SliderFloat3("Position##l21", &(light2->Position[0]), -5.0f, 5.0f);
		//	ImGui::SliderFloat3("Colour##l22", &(light2->Colour[0]), 0.0f, 100.0f);
		//	light2GameObject->Position = light2->Position;
		//	ImGui::End();*/

		//	// model controls:
		//	ImGui::Begin("Model");
		//	ImGui::SliderFloat3("Position##m1", &(mushroomGameObject->Position[0]), -250.0f, 250.0f);
		//	ImGui::SliderFloat3("Rotation##m2", &(mushroomGameObject->Rotation[0]), -5.0f, 5.0f);
		//	ImGui::End();

		//	// material controls:
		//	ImGui::Begin("PBR Material");
		//	ImGui::ColorEdit3("Albedo##m1", &(mushroomMaterial->Albedo[0]));
		//	ImGui::ColorEdit3("Emissive##m2", &(mushroomMaterial->Emissive[0]));
		//	ImGui::SliderFloat("Roughness##m4", &(mushroomMaterial->Roughness), 0.001f, 1.0f);
		//	ImGui::SliderFloat("Metallic##m5", &(mushroomMaterial->Metallic), 0.001f, 1.0f);
		//	ImGui::SliderFloat("Ambient Occlusion##m6", &(mushroomMaterial->AmbientOcclusion), 0.0f, 1.0f);
		//	// ImGui::SliderFloat("Alpha##m6", &(mushroomMaterial->Alpha), 0.0f, 1.0f);
		//	ImGui::End();

		//	// material controls:
		//	ImGui::Begin("Camera");
		//	ImGui::SliderFloat3("Position##c1", &(mainCamera->GetGameObject()->Position[0]), -5.0f, 5.0f);
		//	ImGui::SliderFloat3("Rotation##c2", &(mainCamera->Orientation[0]), -maxRotAngle, maxRotAngle);
		//	ImGui::End();

		//	ImGui::Render();
		//	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//	MimicCore::Window->SwapWindow();
		//	/*MIMIC_LOG_INFO("FPS: %", performanceCounter->GetFPS());*/
		//}
	//}
//	return 0;
//}