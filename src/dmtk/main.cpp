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
#include <renderengine/CubeMap.h>
#include <renderengine/ShadowMapper.h>
#include <utility/FileLoader.h>
#include <utility/Logger.h>
#include <filesystem>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
		// create file loader:
		std::shared_ptr<FileLoader> fileLoader = FileLoader::Initialise();
		const std::string assetPath = fileLoader->LocateDirectory("assets").generic_string();

		// render engine code:
		const std::shared_ptr<Window> window = Window::Initialise("Mimic Render Library Test");

		// initialise renderer:
		std::shared_ptr<Renderer> renderer = Renderer::Initialise();

		// initialise shadow mapper:
		std::shared_ptr<ShadowMapper> shadowMapper = ShadowMapper::Initialise();

		// initialise shaders:
		const std::shared_ptr<Shader> pbrShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetPath, "PBRShader.glsl"));
		glm::vec3 albedo = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 emissive = glm::vec3(0.0f, 0.0f, 0.0f);
		float metallic = 0.25f;
		float roughness = 0.6f;
		float ambientOcclusion = 0.7f;
		float alpha = 1.0f;

		const std::shared_ptr<Shader> blinnPhongShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetPath, "BlinnPhongShader.glsl"));
		glm::vec3 objectColour = glm::vec3(0.8f);
		glm::vec3 lightColour = glm::vec3(0.3f, 0.3f, 0.3f);
		float ambientStrength = 0.8f;
		float diffuseStrength = 0.5f;
		float specularStrength = 0.5f;
		float shininess = 32.0f;

		const std::shared_ptr<Shader> flatColourShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetPath, "FlatColourShader.glsl"));
		constexpr glm::vec3 flatColour = glm::vec3(1.0f);


		// load shader subroutine uniforms:
		const unsigned int albedoSubroutineUniform = pbrShader->GetSubroutineUniform(GL_FRAGMENT_SHADER, "AlbedoMode");
		const unsigned int normalSubroutineUniform = pbrShader->GetSubroutineUniform(GL_FRAGMENT_SHADER, "NormalMode");
		const unsigned int roughnessSubroutineUniform = pbrShader->GetSubroutineUniform(GL_FRAGMENT_SHADER, "RoughnessMode");
		const unsigned int metallicSubroutineUniform = pbrShader->GetSubroutineUniform(GL_FRAGMENT_SHADER, "MetallicMode");
		std::vector<unsigned int> subroutineUniformIndices = { albedoSubroutineUniform, normalSubroutineUniform, roughnessSubroutineUniform, metallicSubroutineUniform };
		
		// load shader subroutine indices (functions):
		const unsigned int albedoAuto = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateAlbedoAutoTexture");
		const unsigned int albedoManual = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateAlbedoManual");

		const unsigned int normalAuto = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateNormalAutoTexture");
		const unsigned int normalManual = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateNormalManual");

		const unsigned int roughnessAuto = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateRoughnessAutoTexture");
		const unsigned int roughnessManual = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateRoughnessManual");

		const unsigned int metallicAuto = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateMetallicAutoTexture");
		const unsigned int metallicManual = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateMetallicManual");

		// create camera:
		std::shared_ptr<Camera> camera = Camera::Initialise(window->GetAspectRatio(), 45.0f);

		// create models:
		glm::vec3 rotation = glm::vec3(0.0f);
		std::shared_ptr<Model> model = Model::Initialise(fileLoader->LocateFileInDirectory(assetPath, "normal_rock_sphere.obj"));
		std::shared_ptr<Model> model1 = Model::Initialise(fileLoader->LocateFileInDirectory(assetPath, "normal_rock_sphere.obj"));
		std::shared_ptr<Model> model2 = Model::Initialise(fileLoader->LocateFileInDirectory(assetPath, "normal_rock_sphere.obj"));
		std::shared_ptr<Model> model3 = Model::Initialise(fileLoader->LocateFileInDirectory(assetPath, "normal_rock_sphere.obj"));
		std::shared_ptr<Model> ground = Model::Initialise(fileLoader->LocateFileInDirectory(assetPath, "cube.obj"));
		std::shared_ptr<Model> wall1 = Model::Initialise(fileLoader->LocateFileInDirectory(assetPath, "cube.obj"));
		glm::vec3 wallPos = glm::vec3(0.0f, 0.0f, -15.0f);
		glm::vec3 wallScale = glm::vec3(1.0f);
		std::shared_ptr<Model> lightModel = Model::Initialise(fileLoader->LocateFileInDirectory(assetPath, "normal_rock_sphere.obj"));

		const std::vector<std::shared_ptr<Model>> sceneModels = { model, model1, model2, wall1 };

		// create textures:
		std::shared_ptr<Texture> albedoTexture = Texture::Initialise(fileLoader->LocateFileInDirectory(assetPath, "rustediron2_basecolor.png"), window->GetAspectRatio(), Texture::MIMIC_2D_TEXTURE_PARAMS, TextureType::MIMIC_ALBEDO);
		std::shared_ptr<Texture> normalTexture = Texture::Initialise(fileLoader->LocateFileInDirectory(assetPath, "rustediron2_normal.png"), window->GetAspectRatio(), Texture::MIMIC_2D_TEXTURE_PARAMS, TextureType::MIMIC_NORMAL);
		std::shared_ptr<Texture> roughnessTexture = Texture::Initialise(fileLoader->LocateFileInDirectory(assetPath, "rustediron2_roughness.png"), window->GetAspectRatio(), Texture::MIMIC_2D_TEXTURE_PARAMS, TextureType::MIMIC_ROUGHNESS);
		std::shared_ptr<Texture> metallicTexture = Texture::Initialise(fileLoader->LocateFileInDirectory(assetPath, "rustediron2_metallic.png"), window->GetAspectRatio(), Texture::MIMIC_2D_TEXTURE_PARAMS, TextureType::MIMIC_METALLIC);

	/*	albedoTexture = nullptr;
		normalTexture = nullptr;
		roughnessTexture = nullptr;
		metallicTexture = nullptr;*/

		// load lights:
		std::vector<std::shared_ptr<DirectLight>> directLights =
		{
			DirectLight::Initialise(glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(0.5f, 0.1f, -0.5f), glm::vec3(70.0f, 20.0f, 15.0f)),
			DirectLight::Initialise(glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(0.5f, 0.1f, -0.5f), glm::vec3(70.0f, 20.0f, 15.0f))
		};

		std::vector<std::shared_ptr<PointLight>> pointLights =
		{
			PointLight::Initialise(glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(70.0f, 20.0f, 15.0f))
		};

		// load hdr environment map:
		std::shared_ptr<EnvironmentCubeMap> environmentCubeMap = EnvironmentCubeMap::Initialise("rural_asphalt_road_4k.hdr", window->GetAspectRatio(), renderer);

		// render-shader lambdas:
		std::function<void()> pbrOnDrawLamba = [&]()
		{
			// load albedo (map_kd):
			if (albedoTexture)
			{
				subroutineUniformIndices[albedoSubroutineUniform] = albedoAuto;
				pbrShader->SetTexture("u_AlbedoMap", albedoTexture->GetId(), 0, Texture::MIMIC_2D_TEXTURE);
			}
			else
			{
				subroutineUniformIndices[albedoSubroutineUniform] = albedoManual;
				pbrShader->SetVector3("u_Albedo", albedo);
			}

			// load normals (map_Bump):
			if (normalTexture)
			{
				subroutineUniformIndices[normalSubroutineUniform] = normalAuto;
				pbrShader->SetTexture("u_NormalMap", normalTexture->GetId(), 1, Texture::MIMIC_2D_TEXTURE);
			}
			else subroutineUniformIndices[normalSubroutineUniform] = normalManual;

			// load roughness(map_ks):
			if (roughnessTexture)
			{
				subroutineUniformIndices[roughnessSubroutineUniform] = roughnessAuto;
				pbrShader->SetTexture("u_RoughnessMap", roughnessTexture->GetId(), 2, Texture::MIMIC_2D_TEXTURE);
			}
			else
			{
				subroutineUniformIndices[roughnessSubroutineUniform] = roughnessManual;
				pbrShader->SetFloat("u_Roughness", roughness);
			}

			// load metallic (must be specified by user):
			if (metallicTexture)
			{
				subroutineUniformIndices[metallicSubroutineUniform] = metallicAuto;
				pbrShader->SetTexture("u_MetallicMap", metallicTexture->GetId(), 3, Texture::MIMIC_2D_TEXTURE);
			}
			else
			{
				subroutineUniformIndices[metallicSubroutineUniform] = metallicManual;
				pbrShader->SetFloat("u_Metallic", metallic);
			}

			glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, subroutineUniformIndices.size(), subroutineUniformIndices.data());

			pbrShader->SetTexture("u_IrradianceMap", environmentCubeMap->GetIrradianceId(), 4, Texture::MIMIC_CUBEMAP_TEXTURE);
			pbrShader->SetTexture("u_PrefilterMap", environmentCubeMap->GetPreFilteredId(), 5, Texture::MIMIC_CUBEMAP_TEXTURE);
			pbrShader->SetTexture("u_BRDFLookupTexture", environmentCubeMap->GetBRDFId(), 6, Texture::MIMIC_2D_TEXTURE);

			pbrShader->SetVector3("u_Emissive", emissive);
			pbrShader->SetFloat("u_Alpha", alpha);
			pbrShader->SetFloat("u_AmbientOcclusion", ambientOcclusion);

			// direct lights
			for (int i = 0; i < directLights.size(); i++)
			{
				const std::string index = std::to_string(i);
				const std::string currentLight = "u_DirectLights[" + index + "]";
				const std::string currentLightMatrix = "u_DirectLightMatrices[" + index + "]";
				const std::string currentShadowMap = "u_DirectShadowMaps[" + index + "]";

				pbrShader->SetVector3((currentLight + ".direction").c_str(), glm::normalize(-directLights[i]->Direction));
				const glm::vec4 colour = glm::vec4(directLights[i]->Colour.x, directLights[i]->Colour.y, directLights[i]->Colour.z, 1.0f);
				pbrShader->SetVector4((currentLight + ".colour").c_str(), colour);
				pbrShader->SetMat4(currentLightMatrix.c_str(), directLights[i]->GetLightMatrix());
				pbrShader->SetTexture(currentShadowMap.c_str(), directLights[i]->GetDepthMapTextureId(), 7, Texture::MIMIC_2D_TEXTURE);
			}
			pbrShader->SetInt("u_DirectLightsCount", directLights.size());


			// point lights:
			for (int i = 0; i < pointLights.size(); i++)
			{
				const std::string currentLight = "u_PointLights[" + std::to_string(i) + "]";

				pbrShader->SetVector3((currentLight + ".position").c_str(), pointLights[i]->Position);
				const glm::vec4 colour = glm::vec4(pointLights[i]->Colour.x, pointLights[i]->Colour.y, pointLights[i]->Colour.z, 1.0f);
				pbrShader->SetVector4((currentLight + ".colour").c_str(), colour);
				pbrShader->SetFloat((currentLight + ".constant").c_str(), pointLights[i]->GetConstant());
				pbrShader->SetFloat((currentLight + ".linear").c_str(), pointLights[i]->GetLinear());
				pbrShader->SetFloat((currentLight + ".quadratic").c_str(), pointLights[i]->GetQuadratic());
			}
			pbrShader->SetInt("u_PointLightsCount", pointLights.size());
		};

		std::function<void()> blinnPhongOnDrawLamba = [&]()
		{
			// set uniforms:
			// blinnPhongShader->SetTexture("u_ShadowMap", shadowMapper->GetDepthMapTextureId(1), 1, Texture::MIMIC_2D_TEXTURE);
			blinnPhongShader->SetVector3("u_ObjectColour", objectColour);
			// blinnPhongShader->SetVector3("u_LightColour", lightColour);
			// blinnPhongShader->SetVector3("u_LightPosition", directLights[1]->Position);
			blinnPhongShader->SetFloat("u_AmbientStrength", ambientStrength);
			blinnPhongShader->SetFloat("u_SpecularStrength", specularStrength);
			blinnPhongShader->SetFloat("u_Shininess", shininess);

			// direct lights
			for (int i = 0; i < directLights.size(); i++)
			{
				const std::string index = std::to_string(i);
				const std::string currentLight = "u_DirectLights[" + index + "]";
				const std::string currentLightMatrix = "u_DirectLightMatrices[" + index + "]";
				const std::string currentShadowMap = "u_DirectShadowMaps[" + index + "]";

				blinnPhongShader->SetVector3((currentLight + ".direction").c_str(), glm::normalize(-directLights[i]->Direction));
				const glm::vec4 colour = glm::vec4(directLights[i]->Colour.x, directLights[i]->Colour.y, directLights[i]->Colour.z, 1.0f);
				blinnPhongShader->SetVector4((currentLight + ".colour").c_str(), colour);
				blinnPhongShader->SetMat4(currentLightMatrix.c_str(), directLights[i]->GetLightMatrix());
				blinnPhongShader->SetTexture(currentShadowMap.c_str(), directLights[i]->GetDepthMapTextureId(), 0, Texture::MIMIC_2D_TEXTURE);
			}
			blinnPhongShader->SetInt("u_DirectLightsCount", directLights.size());
		};
		std::function<void()> flatColourOnDrawLamba = [&]()
		{
			// set uniforms:
			flatColourShader->SetVector3("u_Colour", flatColour);
		};

		// #############################################################################
		//game loop:
		// #############################################################################
		bool applicationRunning = true;
		while (applicationRunning)
		{
			// #############################################################################
			// human interface devices:
			// #############################################################################
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

			// #############################################################################
			// update scene:
			// #############################################################################
			camera->Update();
			model->UpdateModelMatrix(glm::vec3(0.0f, 0.0f, -4.0f),rotation, glm::vec3(1.0f));
			model1->UpdateModelMatrix(glm::vec3(-2.5f, 0.0f, -4.0f), rotation, glm::vec3(1.0f));
			model2->UpdateModelMatrix(glm::vec3(2.5f, 0.0f, -4.0f), rotation, glm::vec3(1.0f));
			model3->UpdateModelMatrix(glm::vec3(2.5f, 1.0f, -4.0f), rotation, glm::vec3(1.0f));
			wall1->UpdateModelMatrix(wallPos, glm::vec3(0.0), glm::vec3(7.143, 5.357, 1.163));
			lightModel->UpdateModelMatrix(directLights[0]->Position, rotation, glm::vec3(0.2f));

			// #############################################################################
			// render scene:
			// #############################################################################
			glClearColor(0.77f, 0.73f, 0.97f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// update shadow maps:
			shadowMapper->RenderDirectLightDepthMaps(sceneModels, directLights, renderer);

			// send meshes to renderer:
			model->QueMeshesToDraw(pbrShader, pbrOnDrawLamba, renderer);
			model1->QueMeshesToDraw(pbrShader, pbrOnDrawLamba, renderer);
			model2->QueMeshesToDraw(blinnPhongShader, blinnPhongOnDrawLamba, renderer);
			wall1->QueMeshesToDraw(blinnPhongShader, blinnPhongOnDrawLamba, renderer);
			// ground->QueMeshesToDraw(pbrShader, pbrOnDrawLamba, renderer);
			lightModel->QueMeshesToDraw(flatColourShader, flatColourOnDrawLamba, renderer);

			// draw:
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			window->ResetViewPort();
			renderer->Draw(camera);
			renderer->ClearRenderQue();
			renderer->DrawCubeMap(camera, environmentCubeMap);

			// #############################################################################
			// gui:
			// #############################################################################
			ImGui_ImplOpenGL3_NewFrame(); 
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();

			// display depth maps:
			ImGui::Image((void*)directLights[0]->GetDepthMapTextureId(), ImVec2(800, 800));
			ImGui::Image((void*)directLights[1]->GetDepthMapTextureId(), ImVec2(800, 800));
			// light controls:
			ImGui::Begin("Point Light");
			ImGui::SliderFloat3("Position##pl1", &(pointLights[0]->Position[0]), -10.0f, 10.0f);
			ImGui::SliderFloat3("Colour##pl3", &(pointLights[0]->Colour[0]), 0.0f, 100.0f);
			ImGui::End();

			ImGui::Begin("Direct Light");
			ImGui::SliderFloat3("Position##dl1", &(directLights[0]->Position[0]), -5.0f, 5.0f);
			ImGui::SliderFloat3("Direction##dl2", &(directLights[0]->Direction[0]), -1.0f, 1.0f);
			ImGui::SliderFloat3("Colour##dl3", &(directLights[0]->Colour[0]), 0.0f, 1.0f);
			ImGui::End();

			ImGui::Begin("Direct Light2");
			ImGui::SliderFloat3("Position##dl1_2", &(directLights[1]->Position[0]), -5.0f, 5.0f);
			ImGui::SliderFloat3("Direction##dl2_2", &(directLights[1]->Direction[0]), -1.0f, 1.0f);
			ImGui::SliderFloat3("Colour##dl3_2", &(directLights[1]->Colour[0]), 0.0f, 1.0f);
			ImGui::End();

			// pbr material controls:
			ImGui::Begin("PBR Material");
			ImGui::ColorEdit3("Albedo##pbr_mat1", &(albedo[0]));
			ImGui::ColorEdit3("Emissive##pbr_mat2", &(emissive[0]));
			ImGui::SliderFloat("Roughness##pbr_mat3", &(roughness), 0.001f, 1.0f);
			ImGui::SliderFloat("Metallic##pbr_mat4", &(metallic), 0.001f, 1.0f);
			ImGui::SliderFloat("Ambient Occlusion##m6", &(ambientOcclusion), 0.0f, 1.0f);
			ImGui::End();

			// phong material controls:
			ImGui::Begin("Phong Material");
			ImGui::ColorEdit3("Object Colour##phong_mat1", &(objectColour[0]));
			ImGui::ColorEdit3("Light Colouur##phong_mat2", &(lightColour[0]));
			ImGui::SliderFloat("Ambient Strength##phong_mat3", &(ambientStrength), 0.0f, 1.0f);
			ImGui::SliderFloat("Diffuse Strength##phong_mat4", &(diffuseStrength), 0.0f, 1.0f);
			ImGui::SliderFloat("Specular Strength##phong_mat5", &(specularStrength), 0.0f, 1.0f);
			ImGui::SliderFloat("Shininess##phong_mat6", &(shininess), 0.0f, 100.0f);
			ImGui::End();

			// model controls:
			ImGui::Begin("Model");
			ImGui::SliderFloat3("Rotation##m2", &(rotation[0]), -5.0f, 5.0f);
			ImGui::End();

			ImGui::Begin("Wall");
			ImGui::SliderFloat3("Position##w2", &(wallPos[0]), -50.0f, 50.0f);
			ImGui::SliderFloat3("Scale##w2", &(wallScale[0]), 0.0f, 50.0f);
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			window->SwapWindow();
		}
	}
}