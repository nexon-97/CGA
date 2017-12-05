#include "core/RenderJob.hpp"
#include <vector>
#include "core/VertexBuffer.hpp"

class ProjectionRenderJob : public cga::RenderJob
{
public:
	ProjectionRenderJob(const glm::vec2i& center);

	virtual void Render(SDL_Window* wnd, SDL_Renderer* renderer) override;

private:
	void UpdateInput();
	void UpdateCameraState();

	std::vector<glm::vec2i> m_points;
	std::vector<glm::vec2i> m_pointsReverse;
	cga::VertexBuffer m_vb;
	float m_cameraAngle = 0.f;
	float m_worldScale = 1.f;
	glm::mat4f m_cameraMatrix;
	glm::mat4f m_projection;

	glm::vec3f m_cameraPosition;
	glm::vec3f m_cameraTarget;
	glm::vec3f m_worldUp;
};
