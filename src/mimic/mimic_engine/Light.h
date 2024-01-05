#include <mimic_render/Light.h>

#include <string>

namespace MimicEngine
{
	struct MimicRender::Light;

	struct DirectLight
	{
		void SetPosition(const glm::vec3& position);
		void SetDirection(const glm::vec3& direction);
		void SetColour(const glm::vec3& colour);
		
		glm::vec3 GetPosition() const;
		glm::vec3 GetDirection() const;
		glm::vec3 GetColour() const;
		std::string Name;
		
	private:
		friend struct MimicCore;
		friend struct PBRMaterial;

		static std::shared_ptr<DirectLight> Initialise(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& colour);
		std::shared_ptr<MimicRender::DirectLight> _renderDirectLight; 
	};
}