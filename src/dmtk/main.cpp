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
#include <utility/FileLoader.h>
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

		// initialise shaders:
		const std::shared_ptr<Shader> pbrShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetPath, "PBRShader.glsl"));
		glm::vec3 albedo = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 emissive = glm::vec3(0.0f, 0.0f, 0.0f);
		float metallic = 0.25f;
		float roughness = 0.6f;
		float ambientOcclusion = 0.7f;
		float alpha = 1.0f;

		const std::shared_ptr<Shader> blinnPhongShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetPath, "BlinnPhongShader.glsl"));
		glm::vec3 objectColour = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 lightColour = glm::vec3(0.3f, 0.3f, 0.3f);
		float ambientStrength = 0.8f;
		float diffuseStrength = 0.5f;
		float specularStrength = 0.5f;
		float shininess = 32.0f;

		const std::shared_ptr<Shader> flatColourShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetPath, "FlatColourShader.glsl"));
		constexpr glm::vec3 flatColour = glm::vec3(1.0f);

		const std::shared_ptr<Shader> depthMapShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetPath, "DepthMapShader.glsl"));
		const std::shared_ptr<Shader> depthMapDebugShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetPath, "DepthMapDebug.glsl"));


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
		std::shared_ptr<Model> lightModel = Model::Initialise(fileLoader->LocateFileInDirectory(assetPath, "normal_rock_sphere.obj"));

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
			DirectLight::Initialise(glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(0.5f, 0.1f, -0.5f), glm::vec3(70.0f, 20.0f, 15.0f))
		};

		std::vector<std::shared_ptr<PointLight>> pointLights =
		{
			PointLight::Initialise(glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(70.0f, 20.0f, 15.0f))
		};

		// shadow mapping:
		// std::shared_ptr<RenderTexture> depthMapRenderTexture = RenderTexture::Initialise();
		// depthMapRenderTexture->SetTexture(Texture::Initialise(glm::ivec2(1024, 1024), Texture::MIMIC_DEPTH_MAP_PARAMS, Texture::MIMIC_DEPTH_COMPONENT, Texture::MIMIC_DEPTH_COMPONENT));
		constexpr glm::vec2 lightProjectionClippingPlanes = glm::vec2(1.0f, 25.0f);
		const glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, lightProjectionClippingPlanes.x, lightProjectionClippingPlanes.y);
		const glm::mat4 lightView = glm::lookAt(
			glm::vec3(-2.0f, 4.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		const glm::mat4 lightSpaceMatrix = lightProjection * lightView;


		// load hdr environment map:
		std::shared_ptr<EnvironmentCubeMap> environmentCubeMap = EnvironmentCubeMap::Initialise("rural_asphalt_road_4k.hdr", window->GetAspectRatio(), renderer);

		// render-shader lambdas:
		std::function<void()> pbrOnDrawLamba = [&]()
		{
			// load albedo (map_kd):
			if (albedoTexture)
			{
				subroutineUniformIndices[albedoSubroutineUniform] = albedoAuto;
				pbrShader->SetTexture("u_AlbedoMap", albedoTexture->GetId(), 1); // texture unit slots start at 1.
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
				pbrShader->SetTexture("u_NormalMap", normalTexture->GetId(), 2);
			}
			else subroutineUniformIndices[normalSubroutineUniform] = normalManual;

			// load roughness(map_ks):
			if (roughnessTexture)
			{
				subroutineUniformIndices[roughnessSubroutineUniform] = roughnessAuto;
				pbrShader->SetTexture("u_RoughnessMap", roughnessTexture->GetId(), 3);
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
				pbrShader->SetTexture("u_MetallicMap", metallicTexture->GetId(), 4);
			}
			else
			{
				subroutineUniformIndices[metallicSubroutineUniform] = metallicManual;
				pbrShader->SetFloat("u_Metallic", metallic);
			}

			glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, subroutineUniformIndices.size(), subroutineUniformIndices.data());

			pbrShader->SetInt("u_IrradianceMap", 5);
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_CUBE_MAP, environmentCubeMap->GetIrradianceId());

			pbrShader->SetInt("u_PrefilterMap", 6);
			glActiveTexture(GL_TEXTURE6);
			glBindTexture(GL_TEXTURE_CUBE_MAP, environmentCubeMap->GetPreFilteredId());

			pbrShader->SetInt("u_BRDFLookupTexture", 7);
			glActiveTexture(GL_TEXTURE7);
			glBindTexture(GL_TEXTURE_2D, environmentCubeMap->GetBRDFId());

			pbrShader->SetVector3("u_Emissive", emissive);
			pbrShader->SetFloat("u_Alpha", alpha);
			pbrShader->SetFloat("u_AmbientOcclusion", ambientOcclusion);

			// direct lights
			for (int i = 0; i < directLights.size(); i++)
			{
				const std::string currentLight = "u_DirectLights[" + std::to_string(i) + "]";

				pbrShader->SetVector3((currentLight + ".direction").c_str(), glm::normalize(-directLights[i]->Direction));
				const glm::vec4 colour = glm::vec4(directLights[i]->Colour.x, directLights[i]->Colour.y, directLights[i]->Colour.z, 1.0f);
				pbrShader->SetVector4((currentLight + ".colour").c_str(), colour);
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
			// phongShader->SetMat4("u_NormalMatrix", normalMatrix);
			blinnPhongShader->SetVector3("u_ObjectColour", objectColour);
			blinnPhongShader->SetVector3("u_LightColour", lightColour);
			blinnPhongShader->SetVector3("u_LightPosition", pointLights[0]->Position);
			blinnPhongShader->SetFloat("u_AmbientStrength", ambientStrength);
			// phongShader->SetFloat("u_DiffuseStrength", diffuseStrength);
			blinnPhongShader->SetFloat("u_SpecularStrength", specularStrength);
			blinnPhongShader->SetFloat("u_Shininess", shininess);
		};
		std::function<void()> flatColourOnDrawLamba = [&]()
		{
			// set uniforms:
			flatColourShader->SetVector3("u_Colour", flatColour);
		};
		std::function<void()> depthMapOnDrawLamba = [&]()
		{
			// set uniforms:
			depthMapShader->SetMat4("u_LightSpaceMatrix", camera->_projectionMatrix * camera->_viewMatrix);
		};

		// init framebuffer:
		unsigned int depthMapFBO;
		glGenFramebuffers(1, &depthMapFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// init depth texture:
		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

		unsigned int depthMap;
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


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

			// update scene:
			glClearColor(0.77f, 0.73f, 0.97f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
			camera->Update();
			model->UpdateModelMatrix(glm::vec3(0.0f, 0.0f, -4.0f),rotation, glm::vec3(1.0f));
			model1->UpdateModelMatrix(glm::vec3(-2.5f, 0.0f, -4.0f), rotation, glm::vec3(1.0f));
			model2->UpdateModelMatrix(glm::vec3(2.5f, 0.0f, -4.0f), rotation, glm::vec3(1.0f));
			model3->UpdateModelMatrix(glm::vec3(2.5f, 1.0f, -4.0f), rotation, glm::vec3(1.0f));
			lightModel->UpdateModelMatrix(pointLights[0]->Position, rotation, glm::vec3(0.2f));


			
			////// update shadow maps:
			model->QueMeshesToDraw(depthMapShader, depthMapOnDrawLamba, renderer);
			//
			///*depthMapRenderTexture->Bind();
			//depthMapRenderTexture->BindTextureForRender(TextureTarget::MIMIC_TEXTURE_2D, RenderTexture::MIMIC_DEPTH_BUFFER_BIT | RenderTexture::MIMIC_NO_DRAW |
			//	RenderTexture::MIMIC_NO_READ);*/
			//glViewport(0, 0, 1024, 1024);

			//renderer->Draw(lightView, lightProjection);
			//renderer->ClearRenderQue();
			//depthMapRenderTexture->Unbind();
			//
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//glViewport(0, 0, window->GetAspectRatio().x, window->GetAspectRatio().y);

			// 1. first render to depth map
			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			renderer->Draw(lightView, lightProjection);
			renderer->ClearRenderQue();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// glBindTexture(GL_TEXTURE_2D, depthMap);
			//// render depth map to quad:
			//depthMapDebugShader->UseShader();
			//depthMapDebugShader->SetFloat("u_NearPlane", lightProjectionClippingPlanes.x);
			//depthMapDebugShader->SetFloat("u_FarPlane", lightProjectionClippingPlanes.y);
			//depthMapDebugShader->SetTexture("u_DepthMap", depthMap, 1);
			//renderer->DrawUnitQuad();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, window->GetAspectRatio().x, window->GetAspectRatio().y);

			// send meshes to renderer:
			model->QueMeshesToDraw(pbrShader, pbrOnDrawLamba, renderer);
			model1->QueMeshesToDraw(pbrShader, pbrOnDrawLamba, renderer);
			model2->QueMeshesToDraw(blinnPhongShader, blinnPhongOnDrawLamba, renderer);
			lightModel->QueMeshesToDraw(flatColourShader, flatColourOnDrawLamba, renderer);

			// draw:
			renderer->Draw(camera);
			renderer->ClearRenderQue();
			renderer->DrawCubeMap(camera, environmentCubeMap);

			// gui:
			ImGui_ImplOpenGL3_NewFrame(); 
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();

			// show texture:
			ImGui::Image((void*)depthMap, ImVec2(800, 800));
			// light controls:
			ImGui::Begin("Point Light");
			ImGui::SliderFloat3("Position##pl1", &(pointLights[0]->Position[0]), -10.0f, 10.0f);
			ImGui::SliderFloat3("Colour##pl3", &(pointLights[0]->Colour[0]), 0.0f, 100.0f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

			ImGui::Begin("Direct Light");
			ImGui::SliderFloat3("Position##dl1", &(directLights[0]->Position[0]), -5.0f, 5.0f);
			ImGui::SliderFloat3("Direction##dl2", &(directLights[0]->Direction[0]), -1.0f, 1.0f);
			ImGui::SliderFloat3("Colour##dl3", &(directLights[0]->Colour[0]), 0.0f, 100.0f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			window->SwapWindow();
		}
	}
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