#include <mimic_render/Light.h>

#include <string>

namespace MimicEngine
{
	struct MimicRender::Light;

	struct DirectLight
	{
		static std::shared_ptr<DirectLight> Initialise(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& colour);

		void SetPosition(const glm::vec3& position);
		void SetDirection(const glm::vec3& direction);
		void SetColour(const glm::vec3& colour);
		std::string Name;

	private:
		friend struct MimicCore;

		std::shared_ptr<MimicRender::DirectLight> _renderDirectLight;
	};
}