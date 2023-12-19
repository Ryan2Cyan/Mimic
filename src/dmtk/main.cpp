#include <mimic_engine/Mimic.h>

#include <array>
#include <algorithm>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#define SDL_MAIN_HANDLED

using namespace MimicEngine;
using namespace MimicRender;
using namespace MimicUtility;

struct PBRMaterialParameters
{
	explicit PBRMaterialParameters(const glm::vec3& albedo, const glm::vec3& emissive, const float& roughness, const float& ambientOcclusion, const float& alpha, const int& metallic)
		: Albedo(albedo), Emissive(emissive), Roughness(roughness), AmbientOcclusion(ambientOcclusion), Alpha(alpha)
	{
		Metallic = std::clamp(metallic, 0, 1);
	}
	glm::vec3 Albedo;
	glm::vec3 Emissive;
	float Roughness;
	float AmbientOcclusion;
	float Alpha;
	int Metallic;
};

#undef main
int main(int argc, char* argv[])
{
	{
		// Initialise core:
		std::shared_ptr<MimicCore> mimicCore = MimicCore::Initialise();

		// Initialise file loader struct and use it to store a path to the project's "assets/" directory:
		// const std::shared_ptr<FileLoader> fileLoader = FileLoader::Initialise();
		// const std::string assetPath = fileLoader->LocateDirectory("assets").generic_string();

		// Initialise renderer:
		// std::shared_ptr<MimicRender::Renderer> renderer = MimicRender::Renderer::Initialise();

		// Initialise shadow mapper, specifying the resolution of the depth maps it'll produce:
		// std::shared_ptr<ShadowMapper> shadowMapper = ShadowMapper::Initialise(glm::ivec2(4096, 4096));

		// Initialise shaders:
		static int shaderUsed = 0;
		// const std::shared_ptr<MimicRender::Shader> pbrShader = MimicRender::Shader::Initialise(fileLoader->LocateFileInDirectory(assetPath, "PBRShader.glsl"));
		// const std::shared_ptr<MimicRender::Shader> blinnPhongShader = MimicRender::Shader::Initialise(fileLoader->LocateFileInDirectory(assetPath, "BlinnPhongShader.glsl"));

		//// Blinn phong material parameters:
		//glm::vec3 objectColour = glm::vec3(0.8f);
		//float ambientStrength = 0.227f;
		//float specularStrength = 0.5f;
		//float shininess = 32.0f;

		//const std::shared_ptr<MimicRender::Shader> flatColourShader = MimicRender::Shader::Initialise(fileLoader->LocateFileInDirectory(assetPath, "FlatColourShader.glsl"));
		//// Flat colour material parameters:
		//constexpr glm::vec3 flatColour = glm::vec3(1.0f);

		// Initialise all data required for the PBR Shader subroutines. This looks messy here but will be cleaned
		// up in the GEP assignment.
		//std::array<unsigned int, 4> subroutineUniformIndices;
		//unsigned int albedoSubroutineUniform;
		//unsigned int normalSubroutineUniform;
		//unsigned int roughnessSubroutineUniform;
		//unsigned int metallicSubroutineUniform;
		//unsigned int albedoAuto;
		//unsigned int albedoManual;
		//unsigned int normalAuto;
		//unsigned int normalManual;
		//unsigned int roughnessAuto;
		//unsigned int roughnessManual;
		//unsigned int metallicAuto;
		//unsigned int metallicManual;

		//if (pbrShader)
		//{
		//	// Load all subroutine uniforms and store them in an array. The array will be used to set indices in 
		//	// PBR on draw function call.
		//	albedoSubroutineUniform = pbrShader->GetSubroutineUniform(GL_FRAGMENT_SHADER, "AlbedoMode");
		//	normalSubroutineUniform = pbrShader->GetSubroutineUniform(GL_FRAGMENT_SHADER, "NormalMode");
		//	roughnessSubroutineUniform = pbrShader->GetSubroutineUniform(GL_FRAGMENT_SHADER, "RoughnessMode");
		//	metallicSubroutineUniform = pbrShader->GetSubroutineUniform(GL_FRAGMENT_SHADER, "MetallicMode");
		//	subroutineUniformIndices = { albedoSubroutineUniform, normalSubroutineUniform, roughnessSubroutineUniform, metallicSubroutineUniform };

		//	// Load all specific subroutine functions. These come in two forms : AutoTexture and Manual. AutoTexture 
		//	// functions are used when the user has set a texture to the PBR parameter, otherwise (Manual) use a literal
		//	// value.
		//	albedoAuto = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateAlbedoAutoTexture");
		//	albedoManual = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateAlbedoManual");

		//	normalAuto = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateNormalAutoTexture");
		//	normalManual = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateNormalManual");

		//	roughnessAuto = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateRoughnessAutoTexture");
		//	roughnessManual = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateRoughnessManual");

		//	metallicAuto = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateMetallicAutoTexture");
		//	metallicManual = pbrShader->GetSubroutineIndex(GL_FRAGMENT_SHADER, "CalculateMetallicManual");
		//}

		// Initialise camera:
		std::shared_ptr<MimicEngine::Camera> camera = MimicEngine::Camera::Initialise(MimicCore::GetCurrentAspect(), 45.0f);
		camera->SetPosition(glm::vec3(0.0f, 0.54f, -5.937f));
		camera->SetOrientation(glm::vec3(0.0, -0.49f, -3.0f));

		// Initialise scene models:
		std::shared_ptr<GameObject> sphereGameObject = GameObject::Initialise(glm::vec3(0.0f, 0.0f, -14.3f), glm::vec3(0.0f), glm::vec3(1.0f));
		auto sphereModelRenderer = sphereGameObject->AddComponent<ModelRenderer>();
		auto spherePBRMaterial = PBRMaterial::Initialise();
		sphereModelRenderer->SetMaterial<PBRMaterial>(spherePBRMaterial);
		sphereModelRenderer->SetModel(MimicCore::ResourceManager->LoadResource<MimicEngine::Model>("sphere.obj"));

		// std::shared_ptr<MimicRender::Model> sphereModel = MimicRender::Model::Initialise(fileLoader->LocateFileInDirectory(assetPath, "sphere.obj"));
		// PBRMaterialParameters spherePBRMaterial = PBRMaterialParameters(glm::vec3(1.0f), glm::vec3(0.0), 0.5f, 0.5f, 1.0f, 0);

		/*std::shared_ptr<GameObject> groundGameObject = GameObject::Initialise(glm::vec3(0.0f, -1.5f, -50.0f), glm::vec3(0.0f), glm::vec3(43.45f, -0.5f, 50.0f));
		std::shared_ptr<MimicRender::Model> groundModel = MimicRender::Model::Initialise(fileLoader->LocateFileInDirectory(assetPath, "cube_3.obj"));
		PBRMaterialParameters groundPBRMaterial = PBRMaterialParameters(glm::vec3(0.6f), glm::vec3(0.0), 0.8f, 0.5f, 1.0f, 0);

		std::shared_ptr<MimicRender::Model> lightModel = MimicRender::Model::Initialise(fileLoader->LocateFileInDirectory(assetPath, "sphere.obj"));*/

		// const std::vector<std::shared_ptr<MimicRender::Model>> modelsForShadowMapping = { /*sphereModel,*/ groundModel};

		// Load textures: Note that the texture types (e.g. MIMIC_ALBEDO) aren't currently implemented. This will
		// become relevant in the GEP assignment.
		std::shared_ptr<MimicEngine::Texture> albedoTexture;
		std::shared_ptr<MimicEngine::Texture> normalTexture;
		std::shared_ptr<MimicEngine::Texture> roughnessTexture;
		std::shared_ptr<MimicEngine::Texture> metallicTexture;

		std::shared_ptr<MimicEngine::Texture> marbleAlbedoTexture		= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Marble016_1K-PNG_Color.png");
		std::shared_ptr<MimicEngine::Texture> marbleNormalTexture		= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Marble016_1K-PNG_NormalGL.png");
		std::shared_ptr<MimicEngine::Texture> marbleRoughnessTexture	= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Marble016_1K-PNG_Roughness.png");
		
		std::shared_ptr<MimicEngine::Texture> foilAlbedoTexture			= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Foil002_1K-PNG_Color.png");
		std::shared_ptr<MimicEngine::Texture> foilNormalTexture			= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Foil002_1K-PNG_NormalGL.png");
		std::shared_ptr<MimicEngine::Texture> foilRoughnessTexture		= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Foil002_1K-PNG_Roughness.png");
		std::shared_ptr<MimicEngine::Texture> foilMetallicTexture		= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Foil002_1K-PNG_Metalness.png");

		std::shared_ptr<MimicEngine::Texture> brickAlbedoTexture		= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Bricks084_1K-PNG_Color.png");
		std::shared_ptr<MimicEngine::Texture> brickNormalTexture		= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Bricks084_1K-PNG_NormalGL.png");
		std::shared_ptr<MimicEngine::Texture> brickRoughnessTexture		= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Bricks084_1K-PNG_Roughness.png");

		std::shared_ptr<MimicEngine::Texture> barkAlbedoTexture			= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Bark012_1K-PNG_Color.png");
		std::shared_ptr<MimicEngine::Texture> barkNormalTexture			= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Bark012_1K-PNG_NormalGL.png");
		std::shared_ptr<MimicEngine::Texture> barkRoughnessTexture		= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Bark012_1K-PNG_Roughness.png");

		std::shared_ptr<MimicEngine::Texture> groundAlbedoTexture		= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Ground054_1K-PNG_Color.png");
		std::shared_ptr<MimicEngine::Texture> groundNormalTexture		= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Ground054_1K-PNG_NormalGL.png");
		std::shared_ptr<MimicEngine::Texture> groundRoughnessTexture	= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Ground054_1K-PNG_Roughness.png");

		std::shared_ptr<MimicEngine::Texture> tiles1AlbedoTexture		= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Tiles078_1K-PNG_Color.png");
		std::shared_ptr<MimicEngine::Texture> tiles1NormalTexture		= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Tiles078_1K-PNG_NormalGL.png");
		std::shared_ptr<MimicEngine::Texture> tiles1RoughnessTexture	= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Tiles078_1K-PNG_Roughness.png");

		std::shared_ptr<MimicEngine::Texture> tiles2AlbedoTexture		= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Tiles093_1K-PNG_Color.png");
		std::shared_ptr<MimicEngine::Texture> tiles2NormalTexture		= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Tiles093_1K-PNG_NormalGL.png");
		std::shared_ptr<MimicEngine::Texture> tiles2RoughnessTexture	= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Tiles093_1K-PNG_Roughness.png");

		std::shared_ptr<MimicEngine::Texture> tiles3AlbedoTexture		= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Tiles101_1K-PNG_Color.png");
		std::shared_ptr<MimicEngine::Texture> tiles3NormalTexture		= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Tiles101_1K-PNG_NormalGL.png");
		std::shared_ptr<MimicEngine::Texture> tiles3RoughnessTexture	= MIMIC_LOAD_RESOURCE(MimicEngine::Texture, "Tiles101_1K-PNG_Roughness.png");

		// Initialise direct lights:
		std::vector<std::shared_ptr<MimicEngine::DirectLight>> directLights =
		{
			MimicEngine::DirectLight::Initialise(glm::vec3(0.0f, 5.0f, -14.0f), glm::vec3(-0.25f), glm::vec3(1.0))
		};

		// Initialise point lights:
		// std::vector<std::shared_ptr<MimicRender::PointLight>> pointLights = { };

		// load hdr environment map:
		// std::shared_ptr<MimicRender::EnvironmentCubeMap> environmentCubeMap = MimicRender::EnvironmentCubeMap::Initialise("rural_asphalt_road_4k.hdr", MIMIC_CURRENT_ASPECT(), renderer);

		//// render-shader lambdas:
		//PBRMaterialParameters pbrMaterial = PBRMaterialParameters(glm::vec3(0.6f), glm::vec3(0.0), 0.8f, 0.5f, 1.0f, 0);
		//std::function<void()> pbrOnDrawLamba = [&]()
		//{
		//	// load albedo (map_kd):
		//	if (albedoTexture)
		//	{
		//		subroutineUniformIndices[albedoSubroutineUniform] = albedoAuto;
		//		pbrShader->SetTexture("u_AlbedoMap", albedoTexture->GetId(), 0, MimicRender::Texture::MIMIC_2D_TEXTURE);
		//	}
		//	else
		//	{
		//		subroutineUniformIndices[albedoSubroutineUniform] = albedoManual;
		//		pbrShader->SetVector3("u_Albedo", pbrMaterial.Albedo);
		//	}

		//	// load normals (map_Bump):
		//	if (normalTexture)
		//	{
		//		subroutineUniformIndices[normalSubroutineUniform] = normalAuto;
		//		pbrShader->SetTexture("u_NormalMap", normalTexture->GetId(), 1, MimicRender::Texture::MIMIC_2D_TEXTURE);
		//	}
		//	else subroutineUniformIndices[normalSubroutineUniform] = normalManual;

		//	// load roughness(map_ks):
		//	if (roughnessTexture)
		//	{
		//		subroutineUniformIndices[roughnessSubroutineUniform] = roughnessAuto;
		//		pbrShader->SetTexture("u_RoughnessMap", roughnessTexture->GetId(), 2, MimicRender::Texture::MIMIC_2D_TEXTURE);
		//	}
		//	else
		//	{
		//		subroutineUniformIndices[roughnessSubroutineUniform] = roughnessManual;
		//		pbrShader->SetFloat("u_Roughness", pbrMaterial.Roughness);
		//	}

		//	// load metallic (must be specified by user):
		//	if (metallicTexture)
		//	{
		//		subroutineUniformIndices[metallicSubroutineUniform] = metallicAuto;
		//		pbrShader->SetTexture("u_MetallicMap", metallicTexture->GetId(), 3, MimicRender::Texture::MIMIC_2D_TEXTURE);
		//	}
		//	else
		//	{
		//		subroutineUniformIndices[metallicSubroutineUniform] = metallicManual;
		//		pbrShader->SetFloat("u_Metallic", pbrMaterial.Metallic);
		//	}

		//	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, subroutineUniformIndices.size(), subroutineUniformIndices.data());

		//	pbrShader->SetTexture("u_IrradianceMap", environmentCubeMap->GetIrradianceId(), 4, MimicRender::Texture::MIMIC_CUBEMAP_TEXTURE);
		//	pbrShader->SetTexture("u_PrefilterMap", environmentCubeMap->GetPreFilteredId(), 5, MimicRender::Texture::MIMIC_CUBEMAP_TEXTURE);
		//	pbrShader->SetTexture("u_BRDFLookupTexture", environmentCubeMap->GetBRDFId(), 6, MimicRender::Texture::MIMIC_2D_TEXTURE);

		//	pbrShader->SetVector3("u_Emissive", pbrMaterial.Emissive);
		//	pbrShader->SetFloat("u_Alpha", pbrMaterial.Alpha);
		//	pbrShader->SetFloat("u_AmbientOcclusion", pbrMaterial.AmbientOcclusion);

		//	// direct lights
		//	for (int i = 0; i < directLights.size(); i++)
		//	{
		//		const std::string index = std::to_string(i);
		//		const std::string currentLight = "u_DirectLights[" + index + "]";
		//		const std::string currentLightMatrix = "u_DirectLightMatrices[" + index + "]";
		//		const std::string currentShadowMap = "u_DirectShadowMaps[" + index + "]";

		//		pbrShader->SetVector3((currentLight + ".direction").c_str(), glm::normalize(-directLights[i]->Direction));
		//		const glm::vec4 colour = glm::vec4(directLights[i]->Colour.x, directLights[i]->Colour.y, directLights[i]->Colour.z, 1.0f);
		//		pbrShader->SetVector4((currentLight + ".colour").c_str(), colour);
		//		pbrShader->SetMat4(currentLightMatrix.c_str(), directLights[i]->GetLightMatrix());
		//		pbrShader->SetTexture(currentShadowMap.c_str(), directLights[i]->GetDepthMapTextureId(), 7, MimicRender::Texture::MIMIC_2D_TEXTURE);
		//	}
		//	pbrShader->SetInt("u_DirectLightsCount", directLights.size());


		//	// point lights:
		//	for (int i = 0; i < pointLights.size(); i++)
		//	{
		//		const std::string currentLight = "u_PointLights[" + std::to_string(i) + "]";

		//		pbrShader->SetVector3((currentLight + ".position").c_str(), pointLights[i]->Position);
		//		const glm::vec4 colour = glm::vec4(pointLights[i]->Colour.x, pointLights[i]->Colour.y, pointLights[i]->Colour.z, 1.0f);
		//		pbrShader->SetVector4((currentLight + ".colour").c_str(), colour);
		//		pbrShader->SetFloat((currentLight + ".constant").c_str(), pointLights[i]->GetConstant());
		//		pbrShader->SetFloat((currentLight + ".linear").c_str(), pointLights[i]->GetLinear());
		//		pbrShader->SetFloat((currentLight + ".quadratic").c_str(), pointLights[i]->GetQuadratic());
		//	}
		//	pbrShader->SetInt("u_PointLightsCount", pointLights.size());
		//};

		//// This value works with ImGUI to decide which material to render on the Stanford Bunny model:
		//static int itemCurrent = 0;
		//std::function<void()> barkPBROnDrawLamba = [&]()
		//{
		//	albedoTexture = barkAlbedoTexture;
		//	normalTexture = barkNormalTexture;
		//	roughnessTexture = barkRoughnessTexture;
		//	metallicTexture = nullptr;
		//	pbrOnDrawLamba();
		//};
		//std::function<void()> brickPBROnDrawLamba = [&]()
		//{
		//	albedoTexture = brickAlbedoTexture;
		//	normalTexture = brickNormalTexture;
		//	roughnessTexture = brickRoughnessTexture;
		//	metallicTexture = nullptr;
		//	pbrOnDrawLamba();
		//};
		//std::function<void()> foilPBROnDrawLamba = [&]()
		//{
		//	albedoTexture = foilAlbedoTexture;
		//	normalTexture = foilNormalTexture;
		//	roughnessTexture = foilRoughnessTexture;
		//	metallicTexture = foilMetallicTexture;
		//	pbrOnDrawLamba();
		//};
		//std::function<void()> marblePBROnDrawLamba = [&]()
		//{
		//	albedoTexture = marbleAlbedoTexture;
		//	normalTexture = marbleNormalTexture;
		//	roughnessTexture = marbleRoughnessTexture;
		//	metallicTexture = nullptr;
		//	pbrOnDrawLamba();
		//};
		//std::function<void()> groundPBROnDrawLamba = [&]()
		//{
		//	albedoTexture = groundAlbedoTexture;
		//	normalTexture = groundNormalTexture;
		//	roughnessTexture = groundRoughnessTexture;
		//	metallicTexture = nullptr;
		//	pbrOnDrawLamba();
		//};
		//std::function<void()> tiles1PBROnDrawLamba = [&]()
		//{
		//	albedoTexture = tiles1AlbedoTexture;
		//	normalTexture = tiles1NormalTexture;
		//	roughnessTexture = tiles1RoughnessTexture;
		//	metallicTexture = nullptr;
		//	pbrOnDrawLamba();
		//};
		//std::function<void()> tiles2PBROnDrawLamba = [&]()
		//{
		//	albedoTexture = tiles2AlbedoTexture;
		//	normalTexture = tiles2NormalTexture;
		//	roughnessTexture = tiles2RoughnessTexture;
		//	metallicTexture = nullptr;
		//	pbrOnDrawLamba();
		//};
		//std::function<void()> tiles3PBROnDrawLamba = [&]()
		//{
		//	albedoTexture = tiles3AlbedoTexture;
		//	normalTexture = tiles3NormalTexture;
		//	roughnessTexture = tiles3RoughnessTexture;
		//	metallicTexture = nullptr;
		//	pbrOnDrawLamba();
		//};
		//std::function<void()> noTexturesPBROnDrawLamba = [&]()
		//{
		//	albedoTexture = nullptr;
		//	normalTexture = nullptr;
		//	roughnessTexture = nullptr;
		//	metallicTexture = nullptr;
		//	pbrOnDrawLamba();
		//};
		//std::function<void()> blinnPhongOnDrawLamba = [&]()
		//{
		//	// Set uniforms:
		//	blinnPhongShader->SetVector3("u_ObjectColour", objectColour);
		//	blinnPhongShader->SetFloat("u_AmbientStrength", ambientStrength);
		//	blinnPhongShader->SetFloat("u_SpecularStrength", specularStrength);
		//	blinnPhongShader->SetFloat("u_Shininess", shininess);

		//	// Direct lights
		//	for (int i = 0; i < directLights.size(); i++)
		//	{
		//		const std::string index = std::to_string(i);
		//		const std::string currentLight = "u_DirectLights[" + index + "]";
		//		const std::string currentLightMatrix = "u_DirectLightMatrices[" + index + "]";
		//		const std::string currentShadowMap = "u_DirectShadowMaps[" + index + "]";

		//		blinnPhongShader->SetVector3((currentLight + ".direction").c_str(), glm::normalize(-directLights[i]->Direction));
		//		const glm::vec4 colour = glm::vec4(directLights[i]->Colour.x, directLights[i]->Colour.y, directLights[i]->Colour.z, 1.0f);
		//		blinnPhongShader->SetVector4((currentLight + ".colour").c_str(), glm::normalize(colour));
		//		blinnPhongShader->SetMat4(currentLightMatrix.c_str(), directLights[i]->GetLightMatrix());
		//		blinnPhongShader->SetTexture(currentShadowMap.c_str(), directLights[i]->GetDepthMapTextureId(), 0, MimicRender::Texture::MIMIC_2D_TEXTURE);
		//	}
		//	blinnPhongShader->SetInt("u_DirectLightsCount", directLights.size());
		//};
		//std::function<void()> sphereBPOnDrawLamba = [&]()
		//{
		//	// pbrMaterial = spherePBRMaterial;
		//	noTexturesPBROnDrawLamba();
		//};
		//std::function<void()> groundBPOnDrawLamba = [&]()
		//{
		//	pbrMaterial = groundPBRMaterial;
		//	noTexturesPBROnDrawLamba();
		//	normalTexture = marbleNormalTexture;
		//};
		//std::function<void()> flatColourOnDrawLamba = [&]()
		//{
		//	// Set uniforms:
		//	flatColourShader->SetVector3("u_Colour", flatColour);
		//};

		// #############################################################################
		// Game loop:
		// #############################################################################
		mimicCore->Start();
		MIMIC_PROFILE_BEGIN_SESSION("Render Loop", "MimicProfile.json");
		while (mimicCore->IsApplicationRunning())
		{
			// #############################################################################
			// Human interface devices:
			// #############################################################################
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				ImGui_ImplSDL2_ProcessEvent(&event);
				switch (event.type)
				{
					case SDL_QUIT:
					{
						mimicCore->Exit();
					}break;

					case SDL_KEYDOWN:
					{
						switch (event.key.keysym.sym)
						{
							case SDLK_ESCAPE:
							{
								mimicCore->Exit();
							}break;
						}
					}
					case SDL_KEYUP: { break; }
				}
			}

			// #############################################################################
			// Update scene:
			// #############################################################################
			mimicCore->Update();
		
			// groundModel->UpdateModelMatrix(groundGameObject->Position, groundGameObject->Rotation, groundGameObject->Scale);
			// lightModel->UpdateModelMatrix(directLights[0]->Position, glm::vec3(0.0f), glm::vec3(0.1f));

			// #############################################################################
			// Render scene:
			// #############################################################################
			{
				MIMIC_PROFILE_SCOPE("Draw Scene");
				{
					// update shadow maps:
					MIMIC_PROFILE_SCOPE("Render Depth Maps");
					// shadowMapper->RenderDirectLightDepthMaps(modelsForShadowMapping, directLights, renderer);
				}

				// Send meshes to renderer:
				// groundModel->QueueMeshesToDraw(pbrShader, groundBPOnDrawLamba, renderer);
				// lightModel->QueueMeshesToDraw(flatColourShader, flatColourOnDrawLamba, renderer);

				// Draw:
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				MimicCore::Window->ResetViewPort();
				mimicCore->Draw();
				// renderer->Draw(camera);
				
				// renderer->ClearRenderQueue();
				// renderer->DrawEnvironmentMap(camera, environmentCubeMap);
			}
			// #############################################################################
			// GUI:
			// #############################################################################
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");

			if (ImGui::CollapsingHeader("Sphere Transform"))
			{
				ImGui::SliderFloat3("Position##m1", &(sphereGameObject->Position[0]), -20.0f, 20.0f);
				ImGui::SliderFloat3("Rotation##m2", &(sphereGameObject->Rotation[0]), -5.0f, 5.0f);
				ImGui::SliderFloat3("Scale##m3", &(sphereGameObject->Scale[0]), 0.0f, 50.0f);
			}
			ImGui::Separator();

			if (ImGui::CollapsingHeader("Sphere Material"))
			{
				ImGui::RadioButton("PBR##pbr_mat0", &shaderUsed, 0); ImGui::SameLine();
				ImGui::RadioButton("Blinn Phong##pbr_mat1", &shaderUsed, 1);
				switch (shaderUsed)
				{
					case 0:
					{
						//const char* items[] = { "None", "Marble", "Bark", "Foil", "Brick", "Ground", "Tiles 1", "Tiles 2", "Tiles 3" };
						//ImGui::Combo("Material", &itemCurrent, items, IM_ARRAYSIZE(items));
						//if (itemCurrent == 0)
						//{
						//	//// PBR material parameters:
						//	//ImGui::ColorEdit3("Albedo##pbr_mat2", &(spherePBRMaterial.Albedo[0]));
						//	//ImGui::ColorEdit3("Emissive##pbr_mat3", &(spherePBRMaterial.Emissive[0]));
						//	//ImGui::SliderFloat("Roughness##pbr_mat4", &(spherePBRMaterial.Roughness), 0.001f, 1.0f);
						//	//ImGui::RadioButton("Dialectric##pbr_mat5", &spherePBRMaterial.Metallic, 0); ImGui::SameLine();
						//	//ImGui::RadioButton("Metallic##pbr_mat6", &spherePBRMaterial.Metallic, 1);
						//	//ImGui::SliderFloat("Ambient Occlusion##pbr_mat7", &(spherePBRMaterial.AmbientOcclusion), 0.0f, 1.0f);
						//}
					}break;
					case 1:
					{
						//// Blinn Phong material parameters:
						//ImGui::ColorEdit3("Object Colour##bp_mat1", &(objectColour[0]));
						//ImGui::SliderFloat("Ambient Strength##bp_mat3", &(ambientStrength), 0.0f, 1.0f);
						//ImGui::SliderFloat("Specular Strength##bp_mat4", &(specularStrength), 0.0f, 1.0f);
						//ImGui::SliderFloat("Shininess##bp_mat5", &(shininess), 0.0f, 100.0f);
					}break;
					default:
					break;
				}
			}
			ImGui::Separator();

			if (ImGui::CollapsingHeader("Ground Transform"))
			{
				/*ImGui::SliderFloat3("Position##gm1", &(groundGameObject->Position[0]), -20.0f, 20.0f);
				ImGui::SliderFloat3("Rotation##gm2", &(groundGameObject->Rotation[0]), -20.0f, 20.0f);
				ImGui::SliderFloat3("Scale##gm3", &(groundGameObject->Scale[0]), 0.0f, 50.0f);*/
			}
			ImGui::Separator();

			if (ImGui::CollapsingHeader("Ground Material"))
			{
				/*ImGui::ColorEdit3("Albedo##pbr_mat1", &(groundPBRMaterial.Albedo[0]));
				ImGui::ColorEdit3("Emissive##pbr_mat2", &(groundPBRMaterial.Emissive[0]));
				ImGui::SliderFloat("Roughness##pbr_mat3", &(groundPBRMaterial.Roughness), 0.001f, 1.0f);
				ImGui::RadioButton("Dialectric##pbr_mat4", &groundPBRMaterial.Metallic, 0); ImGui::SameLine();
				ImGui::RadioButton("Metallic##pbr_mat5", &groundPBRMaterial.Metallic, 1);
				ImGui::SliderFloat("Ambient Occlusion##m6", &(groundPBRMaterial.AmbientOcclusion), 0.0f, 1.0f);*/
			}
			ImGui::Separator();

			if (ImGui::CollapsingHeader("Camera"))
			{
				/*ImGui::SliderFloat3("Position##c1", &(camera->Position[0]), -20.0f, 20.0f);
				ImGui::SliderFloat3("Orientation##c2", &(camera->Orientation[0]), -5.0f, 5.0f);
				ImGui::SliderFloat2("Clipping Planes##c3", &(camera->ClippingPlane[0]), 0.001f, 500.0f);
				ImGui::SliderFloat("FOV##c4", &(camera->Fov), 0.0f, 100.0f);*/
			}
			ImGui::Separator();

			if (ImGui::CollapsingHeader("Direct Light Transform"))
			{
				/*ImGui::SliderFloat3("Position##dl1_2", &(directLights[0]->Position[0]), -20.0f, 20.0f);
				ImGui::SliderFloat3("Direction##dl2_2", &(directLights[0]->Direction[0]), -1.0f, 1.0f);
				ImGui::SliderFloat3("Colour##dl3_2", &(directLights[0]->Colour[0]), 0.0f, 100.0f);*/
			}
			ImGui::Separator();

			if (ImGui::CollapsingHeader("Framerate"))
			{
				// Fill an array of contiguous float values to plot
				// Tip: If your float aren't contiguous but part of a structure, you can pass a pointer to your first float
				// and the sizeof() of your structure in the "stride" parameter.
				static float values[90] = {};
				static int values_offset = 0;
				static double refresh_time = 0.0;
				if (refresh_time == 0.0) refresh_time = ImGui::GetTime();
				while (refresh_time < ImGui::GetTime()) // Create data at fixed 60 Hz rate for the demo
				{
					static float phase = 0.0f;
					values[values_offset] = ImGui::GetIO().Framerate;
					values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
					phase += 0.10f * values_offset;
					refresh_time += 1.0f / 60.0f;
				}

				// Plots can display overlay texts (in this example, we will display an average value):
				{
					float average = 0.0f;
					for (int n = 0; n < IM_ARRAYSIZE(values); n++)
						average += values[n];
					average /= (float)IM_ARRAYSIZE(values);
					char overlay[32];
					sprintf(overlay, "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
					ImGui::PlotLines("Lines", values, IM_ARRAYSIZE(values), values_offset, overlay, -1.0f, 1.0f, ImVec2(0, 80.0f));
				}
			}

			ImGui::End();


			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			MimicCore::Window->SwapWindow();
		}
		MIMIC_PROFILE_END_SESSION();
	}
}