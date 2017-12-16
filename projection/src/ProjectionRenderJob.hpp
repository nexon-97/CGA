#include "core/RenderJob.hpp"
#include <vector>
#include "core/VertexBuffer.hpp"
#include "core/Math.h"

class ProjectionRenderJob : public cga::RenderJob
{
public:
	enum class RenderMode
	{
		Wireframe,
		Solid,
		DepthBuffer,
	};

	ProjectionRenderJob(const glm::vec2i& center);

	virtual void Render(SDL_Window* wnd, SDL_Renderer* renderer) override;

private:
	void UpdateInput();


	cga::VertexBuffer m_vb;
	cga::VertexBuffer m_groundVb;

	float m_worldScale = 1.f;
	float m_cameraYaw = 0.f;
	cga::math::Matrix m_cameraMatrix;
	cga::math::Matrix m_projection;
	glm::vec3f m_worldUp;

	cga::math::Matrix m_cameraFirst;
	float m_fixedAxisRotationAngle = 0.f;
	glm::vec3f m_fixedAxisStart;
	glm::vec3f m_fixedAxisEnd;
	RenderMode m_renderMode = RenderMode::Wireframe;

	float m_cameraAngle = 0.f;
	float m_cameraOffset = 0.f;
	bool m_cameraDynamic = false;
};
