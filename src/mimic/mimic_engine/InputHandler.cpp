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

	void InputHandler::Update()
	{
		ClearTemp();
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
				{
					_applicationRunning = false;
				}break;

				case SDL_MOUSEMOTION:
				{
					// Capture the mouse's current position on the screen.
					SDL_GetMouseState(&_cursorPosition.x, &_cursorPosition.y);
				}break;

				case SDL_MOUSEBUTTONDOWN:
				{
					// Generate a ray from the camera to the current mouse position to intersect with
					// scene objects.
					const auto core = _mimicCore.lock();
					const auto ray = MousePositionProject(core->GetMainCamera());
					auto foundIntersection = false;

					// Loop through all GameObjects and check if the ray intersects with one of them.
					for (auto gameObject : core->_gameObjects)
					{
						gameObject->_cursorIntersected = false;

						// Don't continue to check if an object has already been selected.
						if (foundIntersection) continue;
						if (const auto& model = gameObject->GetComponent<ModelRenderer>()->_model->_renderModel)
						{
							const auto meshes = model->GetMeshes();
							const auto modelMat = gameObject->_modelMatrix;
							const auto invModel = glm::inverse(modelMat);
							const auto viewMat = core->GetMainCamera()->_renderCamera->GetViewMatrix();
							const auto o = invModel * glm::vec4(ray.Origin, 1.0f);
							const auto d = invModel * glm::vec4(ray.Direction, 0.0f);

							// Check all mesh triangles to check with ray-triangle intersection.
							for (const auto& mesh : meshes)
							{
								if (foundIntersection) break;
								for (const auto& triangle : mesh->GetTriangles())
								{
									const auto v0 = triangle.v0;
									const auto v1 = triangle.v1;
									const auto v2 = triangle.v2;

									foundIntersection = MimicPhysics::IntersectTriangle(ray.Origin, ray.Direction, v0, v1, v2);
									if (foundIntersection)
									{
										gameObject->_cursorIntersected = true;
										MIMIC_DEBUG_LOG("Intersection Detected on: %", gameObject->Name);
										return;
									}
								}
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

	Ray InputHandler::MousePositionProject(const std::shared_ptr<Camera>& cam) const
	{
		if (!cam) return Ray();

		// Convert resolution into normalized device coordinates.
		const glm::vec2 res = _mimicCore.lock()->GetWindow()->GetAspectRatio();
		auto ndc = glm::vec4(0.0f, 0.0f, MimicUtility::ConvertToRange((float)_cursorPosition.x, 0.0f, res.x, -1.0f, 1.0f),
			MimicUtility::ConvertToRange((float)_cursorPosition.y, 0.0f, res.y, -1.0f, 1.0f));

		// Pass through the projection matrix.
		ndc = cam->_renderCamera->GetProjectionMatrix() * ndc;

		// Pass through the view matrix twice, one for the near plane, one for the far. 
		// These planes are distinguished by negating the z-axis.
		const auto view = cam->_renderCamera->GetViewMatrix();
		const auto near = view * glm::vec4(ndc.x, ndc.y, -1.0f, ndc.w);
		const auto far = view * glm::vec4(ndc.x, ndc.y, 1.0f, ndc.w);

		// Ray's origin is the near plane, the direction is the difference between the far and near plane.
		return Ray(near, glm::normalize(far - near));
	}
}