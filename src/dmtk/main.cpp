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
#include <utility/PerformanceCounter.h>
#include <array>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#define SDL_MAIN_HANDLED

using namespace MimicUtil;
using namespace MimicRender;

// Temporary structs that'll be intergrated into the engine proper during GEP implementaion:
struct Transform 
{
	explicit Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
		: Position(position), Rotation(rotation), Scale(scale) { }

	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;
};

#undef main
int main(int argc, char* argv[])
{
	{
		// Initialise file loader struct and use it to store a path to the project's "assets/" directory:
		const std::shared_ptr<FileLoader> fileLoader = FileLoader::Initialise();
		const std::string assetPath = fileLoader->LocateDirectory("assets").generic_string();

		// Initialise window that will display the program and interface with SDL, OpenGL, and ImGUI:
		const std::shared_ptr<Window> window = Window::Initialise("Mimic Render Library Test");

		// Initialise renderer:
		std::shared_ptr<Renderer> renderer = Renderer::Initialise();

		// Initialise shadow mapper, specifying the resolution of the depth maps it'll produce:
		std::shared_ptr<ShadowMapper> shadowMapper = ShadowMapper::Initialise(glm::ivec2(4096, 4096));

		// Initialise shaders:
		const std::shared_ptr<Shader> pbrShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetPath, "PBRShader.glsl"));
		// Physically-based rendering material parameters:
		glm::vec3 albedo = glm::vec3(0.3f);
		glm::vec3 emissive = glm::vec3(0.0f, 0.0f, 0.0f);
		float metallic = 0.5f;
		float roughness = 0.5f;
		float ambientOcclusion = 0.8f;
		float alpha = 1.0f;

		const std::shared_ptr<Shader> blinnPhongShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetPath, "BlinnPhongShader.glsl"));
		// Blinn phong material parameters:
		glm::vec3 objectColour = glm::vec3(0.8f);
		glm::vec3 lightColour = glm::vec3(0.3f, 0.3f, 0.3f);
		float ambientStrength = 0.227f;
		float specularStrength = 0.5f;
		float shininess = 32.0f;


		const std::shared_ptr<Shader> flatColourShader = Shader::Initialise(fileLoader->LocateFileInDirectory(assetPath, "FlatColourShader.glsl"));
		// Flat colour material parameters:
		constexpr glm::vec3 flatColour = glm::vec3(1.0f);


		// Initialise all data required for the PBR Shader subroutines. This looks messy here but will be cleaned
		// up in the GEP assignment.
		std::array<unsigned int, 4> subroutineUniformIndices;
		unsigned int albedoSubroutineUniform;
		unsigned int normalSubroutineUniform;
		unsigned int roughnessSubroutineUniform;
		unsigned int metallicSubroutineUniform;
		unsigned int albedoAuto;
		unsigned int albedoManual;
		unsigned int normalAuto;
		unsigned int normalManual;
		unsigned int roughnessAuto;
		unsigned int roughnessManual;
		unsigned int metallicAuto;
		unsigned int metallicManual;

		if (pbrShader)
		{
			// Load all subroutine uniforms and store them in an array. The array will be used to set indices in 
			// PBR on draw function call.
			albedoSubroutineUniform = pbrShader->GetSubroutineUniform(GL_FRAGMENT_SHADER, "AlbedoMode");
			normalSubroutineUniform = pbrShader->GetSubroutineUniform(GL_FRAGMENT_SHADER, "NormalMode");
			roughnessSubroutineUniform = pbrShader->GetSubroutineUniform(GL_FRAGMENT_SHADER, "RoughnessMode");
			metallicSubroutineUniform = pbrShader->GetSubroutineUniform(GL_FRAGMENT_SHADER, "MetallicMode");
			subroutineUniformIndices = { albedoSubroutineUniform, normalSubroutineUniform, roughnessSubroutineUniform, metallicSubroutineUniform };

			// Load all specific subroutine functions. These come in two forms : AutoTexture and Manual. AutoTexture 
			// functions are used when the user has set a texture to the PBR parameter, otherwise (Manual) use a literal
			// value.
			albedoAuto = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateAlbedoAutoTexture");
			albedoManual = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateAlbedoManual");

			normalAuto = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateNormalAutoTexture");
			normalManual = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateNormalManual");

			roughnessAuto = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateRoughnessAutoTexture");
			roughnessManual = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateRoughnessManual");

			metallicAuto = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateMetallicAutoTexture");
			metallicManual = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateMetallicManual");
		}

		// Initialise camera:
		std::shared_ptr<Camera> camera = Camera::Initialise(window->GetAspectRatio(), 45.0f);
		camera->Position = glm::vec3(0.0f, 0.54f, -5.937f);
		camera->Orientation = glm::vec3(0.0, -0.49f, -3.0f);

		// Initialise scene models:
		std::shared_ptr<Model> sphereModel = Model::Initialise(fileLoader->LocateFileInDirectory(assetPath, "sphere.obj"));
		Transform sphereTransform = Transform(glm::vec3(0.0f, 0.0f, -14.3f), glm::vec3(0.0f), glm::vec3(1.0f));

		std::shared_ptr<Model> wallModel = Model::Initialise(fileLoader->LocateFileInDirectory(assetPath, "cube.obj"));
		Transform wallTransform = Transform(glm::vec3(0.0f, -1.5f, -50.0f), glm::vec3(0.0f), glm::vec3(43.45f, -0.5f, 50.0f));

		std::shared_ptr<Model> lightModel = Model::Initialise(fileLoader->LocateFileInDirectory(assetPath, "normal_rock_sphere.obj"));

		const std::vector<std::shared_ptr<Model>> modelsForShadowMapping = { sphereModel, wallModel };

		// Load textures: Note that the texture types (e.g. MIMIC_ALBEDO) aren't currently implemented. This will
		// become relevant in the GEP assignment.
		std::shared_ptr<Texture> albedoTexture;
		std::shared_ptr<Texture> normalTexture;
		std::shared_ptr<Texture> roughnessTexture;
		std::shared_ptr<Texture> metallicTexture;

		std::shared_ptr<Texture> marbleAlbedoTexture = Texture::Initialise(fileLoader->LocateFileInDirectory(assetPath, "Marble016_1K-PNG_Color.png"), window->GetAspectRatio(), Texture::MIMIC_2D_TEXTURE_PARAMS, TextureType::MIMIC_ALBEDO);
		std::shared_ptr<Texture> marbleNormalTexture = Texture::Initialise(fileLoader->LocateFileInDirectory(assetPath, "Marble016_1K-PNG_NormalGL.png"), window->GetAspectRatio(), Texture::MIMIC_2D_TEXTURE_PARAMS, TextureType::MIMIC_NORMAL);
		std::shared_ptr<Texture> marbleRoughnessTexture = Texture::Initialise(fileLoader->LocateFileInDirectory(assetPath, "Marble016_1K-PNG_Roughness.png"), window->GetAspectRatio(), Texture::MIMIC_2D_TEXTURE_PARAMS, TextureType::MIMIC_ROUGHNESS);
		
		std::shared_ptr<Texture> foilAlbedoTexture = Texture::Initialise(fileLoader->LocateFileInDirectory(assetPath, "Foil002_1K-PNG_Color.png"), window->GetAspectRatio(), Texture::MIMIC_2D_TEXTURE_PARAMS, TextureType::MIMIC_ALBEDO);
		std::shared_ptr<Texture> foilNormalTexture = Texture::Initialise(fileLoader->LocateFileInDirectory(assetPath, "Foil002_1K-PNG_NormalGL.png"), window->GetAspectRatio(), Texture::MIMIC_2D_TEXTURE_PARAMS, TextureType::MIMIC_NORMAL);
		std::shared_ptr<Texture> foilRoughnessTexture = Texture::Initialise(fileLoader->LocateFileInDirectory(assetPath, "Foil002_1K-PNG_Roughness.png"), window->GetAspectRatio(), Texture::MIMIC_2D_TEXTURE_PARAMS, TextureType::MIMIC_ROUGHNESS);
		std::shared_ptr<Texture> foilMetallicTexture = Texture::Initialise(fileLoader->LocateFileInDirectory(assetPath, "Foil002_1K-PNG_Metalness.png"), window->GetAspectRatio(), Texture::MIMIC_2D_TEXTURE_PARAMS, TextureType::MIMIC_METALLIC);

		std::shared_ptr<Texture> brickAlbedoTexture = Texture::Initialise(fileLoader->LocateFileInDirectory(assetPath, "Bricks084_1K-PNG_Color.png"), window->GetAspectRatio(), Texture::MIMIC_2D_TEXTURE_PARAMS, TextureType::MIMIC_ALBEDO);
		std::shared_ptr<Texture> brickNormalTexture = Texture::Initialise(fileLoader->LocateFileInDirectory(assetPath, "Bricks084_1K-PNG_NormalGL.png"), window->GetAspectRatio(), Texture::MIMIC_2D_TEXTURE_PARAMS, TextureType::MIMIC_NORMAL);
		std::shared_ptr<Texture> brickRoughnessTexture = Texture::Initialise(fileLoader->LocateFileInDirectory(assetPath, "Bricks084_1K-PNG_Roughness.png"), window->GetAspectRatio(), Texture::MIMIC_2D_TEXTURE_PARAMS, TextureType::MIMIC_ROUGHNESS);

		std::shared_ptr<Texture> barkAlbedoTexture = Texture::Initialise(fileLoader->LocateFileInDirectory(assetPath, "Bark012_1K-PNG_Color.png"), window->GetAspectRatio(), Texture::MIMIC_2D_TEXTURE_PARAMS, TextureType::MIMIC_ALBEDO);
		std::shared_ptr<Texture> barkNormalTexture = Texture::Initialise(fileLoader->LocateFileInDirectory(assetPath, "Bark012_1K-PNG_NormalGL.png"), window->GetAspectRatio(), Texture::MIMIC_2D_TEXTURE_PARAMS, TextureType::MIMIC_NORMAL);
		std::shared_ptr<Texture> barkRoughnessTexture = Texture::Initialise(fileLoader->LocateFileInDirectory(assetPath, "Bark012_1K-PNG_Roughness.png"), window->GetAspectRatio(), Texture::MIMIC_2D_TEXTURE_PARAMS, TextureType::MIMIC_ROUGHNESS);

		// Initialise direct lights:
		std::vector<std::shared_ptr<DirectLight>> directLights =
		{
			DirectLight::Initialise(glm::vec3(2.72f, 8.8f, -5.8f), glm::vec3(-0.24f, -1.0f, -0.246f), glm::vec3(100.0f))
		};

		// Initialise point lights:
		std::vector<std::shared_ptr<PointLight>> pointLights = { };

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

		// This value works with ImGUI to decide which material to render on the Stanford Bunny model:
		static int itemCurrent = 0;
		std::function<void()> barkPBROnDrawLamba = [&]()
		{
			albedoTexture = barkAlbedoTexture;
			normalTexture = barkNormalTexture;
			roughnessTexture = barkRoughnessTexture;
			metallicTexture = nullptr;
			metallic = 0.5f;
			pbrOnDrawLamba();
		};
		std::function<void()> brickPBROnDrawLamba = [&]()
		{
			albedoTexture = brickAlbedoTexture;
			normalTexture = brickNormalTexture;
			roughnessTexture = brickRoughnessTexture;
			metallicTexture = nullptr;
			metallic = 0.5f;
			pbrOnDrawLamba();
		};
		std::function<void()> foilPBROnDrawLamba = [&]()
		{
			albedoTexture = foilAlbedoTexture;
			normalTexture = foilNormalTexture;
			roughnessTexture = foilRoughnessTexture;
			metallicTexture = foilMetallicTexture;
			metallic = 0.5f;
			pbrOnDrawLamba();
		};
		std::function<void()> marblePBROnDrawLamba = [&]()
		{
			albedoTexture = marbleAlbedoTexture;
			normalTexture = marbleNormalTexture;
			roughnessTexture = marbleRoughnessTexture;
			metallicTexture = nullptr;
			metallic = 0.99f;
			pbrOnDrawLamba();
		};

		std::function<void()> noTexturesPBROnDrawLamba = [&]()
		{
			albedoTexture = nullptr;
			normalTexture = nullptr;
			roughnessTexture = nullptr;
			metallicTexture = nullptr;
			pbrOnDrawLamba();
		};

		std::function<void()> blinnPhongOnDrawLamba = [&]()
		{
			// set uniforms:
			blinnPhongShader->SetVector3("u_ObjectColour", objectColour);
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
				blinnPhongShader->SetVector4((currentLight + ".colour").c_str(), glm::normalize(colour));
				blinnPhongShader->SetMat4(currentLightMatrix.c_str(), directLights[i]->GetLightMatrix());
				blinnPhongShader->SetTexture(currentShadowMap.c_str(), directLights[i]->GetDepthMapTextureId(), 0, Texture::MIMIC_2D_TEXTURE);
			}
			blinnPhongShader->SetInt("u_DirectLightsCount", directLights.size());
		};

		std::function<void()> sphereBPOnDrawLamba = [&]()
		{
			objectColour = glm::vec3(0.9f);
			blinnPhongOnDrawLamba();
		};
		std::function<void()> wallBPOnDrawLamba = [&]()
		{
			objectColour = glm::vec3(0.98f);
			blinnPhongOnDrawLamba();
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
		MIMIC_PROFILE_BEGIN_SESSION("Render Loop", "MimicProfile.json");
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
			sphereModel->UpdateModelMatrix(sphereTransform.Position, sphereTransform.Rotation, sphereTransform.Scale);
			wallModel->UpdateModelMatrix(wallTransform.Position, wallTransform.Rotation, wallTransform.Scale);
			lightModel->UpdateModelMatrix(directLights[0]->Position, glm::vec3(0.0), glm::vec3(0.2f));

			// #############################################################################
			// render scene:
			// #############################################################################
			{
				MIMIC_PROFILE_SCOPE("Draw Scene");
				glClearColor(0.77f, 0.73f, 0.97f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				{
					// update shadow maps:
					MIMIC_PROFILE_SCOPE("Render Depth Maps");
					shadowMapper->RenderDirectLightDepthMaps(modelsForShadowMapping, directLights, renderer);
				}
				// send meshes to renderer:
				switch (itemCurrent)
				{
					case 0:
					{
						sphereModel->QueueMeshesToDraw(pbrShader, noTexturesPBROnDrawLamba, renderer);
					}break;
					case 1:
					{
						sphereModel->QueueMeshesToDraw(pbrShader, marblePBROnDrawLamba, renderer);
					}break;
					case 2:
					{
						sphereModel->QueueMeshesToDraw(pbrShader, barkPBROnDrawLamba, renderer);
					}break;
					case 3:
					{
						sphereModel->QueueMeshesToDraw(pbrShader, foilPBROnDrawLamba, renderer);
					}break;
					case 4:
					{
						sphereModel->QueueMeshesToDraw(pbrShader, brickPBROnDrawLamba, renderer);
					}break;
					default:
						break;
					}
				wallModel->QueueMeshesToDraw(blinnPhongShader, wallBPOnDrawLamba, renderer);

				// draw:
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				window->ResetViewPort();
				renderer->Draw(camera);
				
				renderer->ClearRenderQueue();
				renderer->DrawEnvironmentMap(camera, environmentCubeMap);
			}
			// #############################################################################
			// gui:
			// #############################################################################
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();

			ImGui::ShowDemoWindow();

			ImGui::Begin("Settings");
			const char* items[] = { "None", "Marble", "Bark", "Foil", "Brick"};
			ImGui::Combo("Material", &itemCurrent, items, IM_ARRAYSIZE(items));
			ImGui::SameLine(); 
			ImGui::End(); 

			/*ImGui::Begin("Settings");
			bool beginComboResult = ImGui::BeginCombo("Material", "Marble");
			ImGui::Combo("Material", 0, { "Marble, Bark, Wood, Gold Tinfoil" }, 4);
			if(beginComboResult) ImGui::EndCombo();
			ImGui::End();*/

			/*ImGui::SliderFloat3("Position##dl1", &(directLights[0]->Position[0]), -30.0f, 30.0f);
			ImGui::SliderFloat3("Direction##dl2", &(directLights[0]->Direction[0]), -1.0f, 1.0f);
			ImGui::SliderFloat3("Colour##dl3", &(directLights[0]->Colour[0]), 0.0f, 100.0f);*/

			/*ImGui::Begin("Direct Light2");
			ImGui::SliderFloat3("Position##dl1_2", &(directLights[1]->Position[0]), -5.0f, 5.0f);
			ImGui::SliderFloat3("Direction##dl2_2", &(directLights[1]->Direction[0]), -1.0f, 1.0f);
			ImGui::SliderFloat3("Colour##dl3_2", &(directLights[1]->Colour[0]), 0.0f, 1.0f);
			ImGui::End();*/

			//// pbr material controls:
			//ImGui::Begin("PBR Material");
			//ImGui::ColorEdit3("Albedo##pbr_mat1", &(albedo[0]));
			//ImGui::ColorEdit3("Emissive##pbr_mat2", &(emissive[0]));
			//ImGui::SliderFloat("Roughness##pbr_mat3", &(roughness), 0.001f, 1.0f);
			//ImGui::SliderFloat("Metallic##pbr_mat4", &(metallic), 0.001f, 1.0f);
			//ImGui::SliderFloat("Ambient Occlusion##m6", &(ambientOcclusion), 0.0f, 1.0f);
			//ImGui::End();

			//// phong material controls:
			//ImGui::Begin("Phong Material");
			//ImGui::ColorEdit3("Object Colour##phong_mat1", &(objectColour[0]));
			//ImGui::ColorEdit3("Light Colouur##phong_mat2", &(lightColour[0]));
			//ImGui::SliderFloat("Ambient Strength##phong_mat3", &(ambientStrength), 0.0f, 1.0f);
			//ImGui::SliderFloat("Specular Strength##phong_mat5", &(specularStrength), 0.0f, 1.0f);
			//ImGui::SliderFloat("Shininess##phong_mat6", &(shininess), 0.0f, 100.0f);
			//ImGui::End();

			//// camera controls:
			//ImGui::Begin("Camera");
			//ImGui::SliderFloat3("Position##c1", &(camera->Position[0]), -20.0f, 20.0f);
			//ImGui::SliderFloat3("Orientation##c2", &(camera->Orientation[0]), -5.0f, 5.0f);
			//ImGui::End();

			//// model controls:
			///*ImGui::Begin("Model");
			//ImGui::SliderFloat3("Position##m1", &(position[0]), -20.0f, 20.0f);
			//ImGui::SliderFloat3("Rotation##m2", &(rotation[0]), -5.0f, 5.0f);
			//ImGui::End();*/

			///*ImGui::Begin("Wall");
			//ImGui::SliderFloat3("Position##w2", &(wallPos[0]), -50.0f, 50.0f);
			//ImGui::SliderFloat3("Rotation##w2", &(wallRot[0]), -50.0f, 50.0f);
			//ImGui::SliderFloat3("Scale##w3", &(wallScale[0]), 0.0f, 50.0f);
			//ImGui::End();*/

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			window->SwapWindow();
		}
		MIMIC_PROFILE_END_SESSION();
	}
}