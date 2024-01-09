#include "InputHandler.h"
#include "MimicCore.h"
#include "Camera.h"
#include "GameObject.h"
#include "ModelRenderer.h"
#include "Model.h"
#include "mimic_render/Camera.h"
#include "mimic_render/Triangle.h"
#include "mimic_render/Mesh.h"
#include <mimic_utility/Math.h>
#include <mimic_utility/Logger.h>
#include <mimic_physics/Algorithms.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

#include <SDL/SDL.h>


namespace MimicEngine
{
	std::shared_ptr<InputHandler> InputHandler::Initialise(bool& applicationRunning)
	{
		auto inputHandler = std::make_shared<InputHandler>();
		inputHandler->_applicationRunning = applicationRunning;
		return inputHandler;
	}

	bool InputHandler::IsKey(const int key)
	{
		if (_keys.size() < 1) return false;
		for (auto storedKey : _keys) if (storedKey == key) return true;
		return false;
	}

	bool InputHandler::IsKeyPressed(const int key)
	{
		if (_keysPressed.size() < 1) return false;

		for (auto storedKey : _keysPressed) if (storedKey == key) return true;
		return false;
	}

	bool InputHandler::IsKeyReleased(const int key)
	{
		if (_keysReleased.size() < 1) return false;

		for (auto storedKey : _keysReleased) if (storedKey == key) return true;
		return false;
	}

	glm::ivec2 InputHandler::GetCursorPosition() const
	{
		return _cursorPosition;
	}

	void InputHandler::Update()
	{
		ClearTemp();

		// Capture the mouse's current position on the screen.
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			switch (event.type)
			{
				case SDL_QUIT:
				{
					_applicationRunning = false;
				}break;

				case SDL_MOUSEMOTION:
				{
					SDL_GetMouseState(&_cursorPosition.x, &_cursorPosition.y);
				}break;

				case SDL_MOUSEBUTTONDOWN:
				{
					// Generate a ray from the camera to the current mouse position to intersect with
					// scene objects.
					const auto core = _mimicCore.lock();
					const auto ray = MousePositionProject(core->GetMainCamera(), _cursorPosition);

					// Loop through all GameObjects and check if the ray intersects with one of them.
					for (auto gameObject : core->_gameObjects)
					{
						// If you click on an already selected GameObject, unselect it:
						if (gameObject->_selected)
						{
							gameObject->_selected = false;
							if(gameObject->OnUnselected) gameObject->OnUnselected();
							continue;
						}
						// Don't continue to check if an object has already been selected.
						if (const auto& modelRenderer = gameObject->GetComponent<ModelRenderer>())
						{
							if (const auto& model = modelRenderer->_model->_renderModel)
							{
								const auto meshes = model->GetMeshes();
								const auto modelMat = gameObject->_modelMatrix;

								// Check all mesh triangles to check with ray-triangle intersection.
								bool foundIntersection = false;
								for (const auto& mesh : meshes)
								{
									if (foundIntersection) break;
									for (const auto& triangle : mesh->GetTriangles())
									{
										if (foundIntersection) break;
										const auto v0 = modelMat * glm::vec4(triangle.v0, 1.0f);
										const auto v1 = modelMat * glm::vec4(triangle.v1, 1.0f);
										const auto v2 = modelMat * glm::vec4(triangle.v2, 1.0f);

										foundIntersection = MimicPhysics::IntersectTriangle(ray.Origin, ray.Direction, v0, v1, v2);
									}
								}
								if (foundIntersection && !gameObject->_selected) if (gameObject->OnSelected) gameObject->OnSelected();
								if (!foundIntersection && gameObject->_selected) if (gameObject->OnUnselected) gameObject->OnUnselected();
								gameObject->_selected = foundIntersection;
							}
						}
					}
				}break;

				case SDL_KEYDOWN:
				{
					const auto key = event.key.keysym.sym;
					if (!IsKey(key))
					{
						_keys.push_back(key);
						_keysPressed.push_back(key);
						MIMIC_DEBUG_LOG("Pressed: %", key);
					}
				} break; 

				case SDL_KEYUP: 
				{ 
					const auto key = event.key.keysym.sym;
					_keys.remove(key);
					_keysReleased.push_back(key);
					MIMIC_DEBUG_LOG("Released: %", key);
					
				} break;
			}
		}
	}

	void InputHandler::ClearTemp()
	{
		_keysPressed.clear();
		_keysReleased.clear();
	}

	Ray InputHandler::MousePositionProject(const std::shared_ptr<Camera>& cam, const glm::ivec2& cursorPos) const
	{
		if (!cam) return Ray();

		// Convert resolution into normalized device coordinates.
		const glm::vec2 res = _mimicCore.lock()->GetWindow()->GetAspectRatio();
		auto rayClip = glm::vec4(
			(2.0f * cursorPos.x) / res.x - 1.0f,
			1.0f - (2.0f * cursorPos.y) / res.y,
			-1.0f,
			1.0f
		);

		auto rayEye = glm::inverse(cam->_renderCamera->GetProjectionMatrix()) * rayClip;
		rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);
		const auto rayWorld = glm::normalize(glm::inverse(cam->_renderCamera->GetViewMatrix()) * rayEye);
	
		// Ray's origin is the near plane, the direction is the difference between the far and near plane.
		return Ray(cam->GetPosition(), rayWorld);
	}
}