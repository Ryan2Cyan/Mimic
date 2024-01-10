#pragma once
#include <mimic_utility/Logger.h>
#include <mimic_render/Renderer.h>

using namespace MimicRender;
using namespace MimicUtility;

namespace MimicRender
{
	struct Window;
	struct EnvironmentCubeMap;
	struct Renderer;
}

namespace MimicEngine
{
	// Forward-declarations:
	struct GameObject;
	struct ResourceManager;
	struct Environment;
	struct Camera;
	struct Renderer;
	struct DirectLight;
	struct PointLight;
	struct ShadowMapper;
	struct Camera;
	struct InputHandler;

	/// <summary>
	/// MimicEngine: Highest level component in the engine hierarchy. Calls most key engine functions
	/// (e.g. Start(), Update(), FixedUpdate(), and GuiUpdate()). 
	/// </summary>
	struct MimicCore 
	{
		static std::shared_ptr<MimicCore> Initialise();

		/// <summary> 
		/// Called once before the game loop starts running, starts the application. 
		/// </summary>
		void Start();

		/// <summary> 
		/// Called once per frame in the game loop. Updates all scene objects, cameras, and delta time. 
		/// </summary>
		void Update();

		/// <summary> 
		/// Called once per physics fixed time step in the game loop. Updates all physics objects. 
		/// </summary>
		void FixedUpdate();

		/// <summary> 
		/// Called once per frame at the end of the game loop. Updates all GUI components objects. 
		/// </summary>
		void GuiUpdate();

		/// <summary> 
		/// Called once per frame in the game loop. Draws all objects in the scene. 
		/// </summary>
		void Draw();

		/// <summary> 
		/// End the application. 
		/// </summary>
		void Exit();

		/// <summary> 
		/// Returns true if the application is running, false otherwise. 
		/// </summary>
		bool IsApplicationRunning();

		/// <summary>
		/// Loop through all scene GameObjects and returns a vector of a input component type.
		/// </summary>
		/// <typeparam name="T">A type of Component.</typeparam>
		/// <returns>A vector of inputted Component type.</returns>
		template<typename T> std::vector<std::shared_ptr<T>> Find() const
		{
			std::vector<std::shared_ptr<T>> result;
			for (const auto gameObject : _gameObjects)
			{
				for (const auto component : gameObject->_components)
				{
					if(std::shared_ptr<T> temp = std::dynamic_pointer_cast<T>(component)) result.push_back(temp);
				}
			}

			return result;
		}

		std::shared_ptr<ResourceManager> GetResourceManager() const;
		std::shared_ptr<InputHandler> GetInputHandler() const;
		std::shared_ptr<MimicRender::Window> GetWindow() const;
		std::shared_ptr<Camera> GetMainCamera() const;

		void SetMainCamera(const std::shared_ptr<Camera>& mainCamera);

		/// <summary>
		///  Add new Camera to the scene. MimicCore will store a reference to the newly added Camera.
		/// </summary>
		/// <param name="aspectRatio">Dimensions of the window, used in the creation of the Camera's projection matrix.</param>
		/// <param name="fov">[0, 90] Camera's field of view (observable area a person can see via an optical device) in degrees.</param>
		/// <param name="clippingPlane">[0, 170] Consists of two values, 'near' being x, 'far' being y. Controls the plane of visibility clipping away geometry in a specific viewport.</param>
		/// <returns>Newly instantiated camera.</returns>
		std::shared_ptr<Camera> AddCamera(const glm::vec2& aspectRatio, const float& fov, const glm::vec2& clippingPlane = glm::vec2(0.1f, 100.0f));

		/// <summary>
		/// Add new GameObject to the scene. MimicCore will store a reference to the newly added GameObject.
		/// </summary>
		/// <returns>Newly instantiated GameObject.</returns>
		std::shared_ptr<GameObject> AddGameObject();

		/// <summary>
		/// Add new GameObject to the scene. MimicCore will store a reference to the newly added GameObject.
		/// </summary>
		/// <param name="position">Initial position of the GameObject.</param>
		/// <param name="rotation">Initial rotation (eular angles) of the GameObject.</param>
		/// <param name="scale">Initial scale of the GameObject.</param>
		/// <returns>Newly instantiated GameObject.</returns>
		std::shared_ptr<GameObject> AddGameObject(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

		/// <summary>
		/// Add new DirectLight to the scene. MimicCore will store a reference to the newly added DirectLight .
		/// </summary>
		/// <param name="position">Initial position of the DirectLight. Orientation and placement of shadow maps are dependent on this value.</param>
		/// <param name="direction">Initial facing direction of the DirectLight.</param>
		/// <param name="colour">[0, 1] Initial colour (RGB) of the DirectLight.</param>
		/// <returns>Newly instantiated DirectLight.</returns>
		std::shared_ptr<DirectLight> AddDirectLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& colour);

	private:
		friend struct ModelRenderer; 
		friend struct PBRMaterial; 
		friend struct EnvironmentCubeMap;
		friend struct GameObject;
		friend struct Camera;
		friend struct DirectLight;
		friend struct ShadowMapper;
		friend struct InputHandler;

		/*void RemoveGameObject(const std::shared_ptr<GameObject>& gameObject);
		void RemoveCamera(const std::shared_ptr<Camera>& camera);
		void RemoveDirectLight(const std::shared_ptr<DirectLight>& directLight);*/
		// std::shared_ptr<PointLight> AddPointLight() noexcept;

		std::shared_ptr<Camera> _mainCamera;
		std::shared_ptr<Environment> _environment;
		std::shared_ptr<ResourceManager> _resourceManager;
		std::shared_ptr<InputHandler> _inputHandler;
		std::shared_ptr<Window> _window;
		std::shared_ptr<ShadowMapper> _shadowMapper;
		std::shared_ptr<MimicRender::Renderer> _renderer;

		std::list<std::shared_ptr<DirectLight>> _directLights;
		std::list<std::shared_ptr<GameObject>> _gameObjects;
		std::list<std::shared_ptr<Camera>> _cameras;

		std::shared_ptr<EnvironmentCubeMap> _environmentCubeMap;
		// static std::vector<std::shared_ptr<MimicRender::PointLight>> _pointLights;

		std::weak_ptr<MimicCore> _self;
		bool _applicationRunning;
		bool _initialised;
	};
}