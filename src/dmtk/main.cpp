#include <lowlevelsystems/Mimic.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#include "imgui.h"
#define SDL_MAIN_HANDLED

using namespace Mimic;

#undef main
int main(int argc, char* argv[])
{
	{
		// initialise:
		std::cout << "Hello Worldddd!" << std::endl;
		std::shared_ptr<MimicCore> dmtkCore = MimicCore::Initialise();
		std::shared_ptr<Camera> dmtkCamera = dmtkCore->AddNewCamera();
		std::shared_ptr<GameObject> newGameObject = dmtkCore->AddEmptyGameObject();
		std::shared_ptr<ModelRenderer> modelRenderer = newGameObject->AddComponent<ModelRenderer>();

		/*std::vector <Vertex> triangleVertices {
			Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f)),
			Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f)),
			Vertex(glm::vec3(0.0f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f))
		};

		std::vector <unsigned int> triangleIndices {0, 1, 2};
		std::vector<Texture> triangleTextures{};

		Mesh triangleMesh = Mesh(triangleVertices, triangleIndices, triangleTextures);
		glm::mat4 triangleModelMatrix = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
		std::shared_ptr<Shader> triangleShader = std::make_shared<Shader>("../src/mimic/shaders/VertexShader.txt", "../src/mimic/shaders/FragmentShader.txt");

		glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -10.5f));
		glm::mat4 projMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);*/

		modelRenderer->Initialise(
			"../src/dmtk/additional-files/models/Mushrooms1_Obj/Mushrooms1_Obj/Mushrooms1.obj",
			"../src/mimic/shaders/VertexShader.txt",
			"../src/mimic/shaders/FragmentShader.txt");
		
		// handle human interface devices:
		while (dmtkCore->ApplicationRunning)
		{
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
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			dmtkCore->Update();

		/*	triangleShader->SetMat4("u_Model", triangleModelMatrix);
			triangleShader->SetMat4("u_View", viewMatrix);
			triangleShader->SetMat4("u_Projection", projMatrix);
			triangleMesh.Draw(triangleShader);*/

			// render loop:
		}
	}
	return 0;
}