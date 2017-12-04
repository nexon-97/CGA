#include "RecursionRenderJob.hpp"
#include <glm/gtx/compatibility.hpp>
#include <core/Brezenheim.hpp>

RecursionRenderJob::RecursionRenderJob(const glm::vec2i& center, int recursionDepth, float rotationStep)
	: m_recursionDepth(recursionDepth)
{
	static const float PI = 3.1415f;
	static const float PI_DOUBLE = 2.f * PI;

	// Add initial equal-sized triangle
	glm::vec2i currentTriangle[3] {
		center + glm::vec2i(0.f, -240.f),
		center + glm::vec2i(-242.5f, 210.f),
		center + glm::vec2i(242.5f, 210.f),
	};
	glm::vec2i nextTriangle[3];

	AddTriangle(currentTriangle);

	float phi = rotationStep;
	for (int i = 0; i < recursionDepth; i++)
	{
		phi += rotationStep;

		float fProjectionSide = (fmodf(phi, PI_DOUBLE) / PI_DOUBLE) * 3.f;
		int projectionSide = int(fProjectionSide);
		float percentage = fmodf(fProjectionSide, 1.f);

		nextTriangle[0] = glm::lerp(glm::vec2f(currentTriangle[0]), glm::vec2f(currentTriangle[1]), percentage);
		nextTriangle[1] = glm::lerp(glm::vec2f(currentTriangle[1]), glm::vec2f(currentTriangle[2]), percentage);
		nextTriangle[2] = glm::lerp(glm::vec2f(currentTriangle[2]), glm::vec2f(currentTriangle[0]), percentage);
		memcpy_s(currentTriangle, sizeof(currentTriangle), nextTriangle, sizeof(nextTriangle));

		AddTriangle(currentTriangle);
	}
}

void RecursionRenderJob::AddTriangle(glm::vec2i triangle[3])
{
	m_points.push_back(triangle[0]);
	m_points.push_back(triangle[1]);
	m_points.push_back(triangle[2]);
	m_points.push_back(triangle[0]);
}

void RecursionRenderJob::Render(SDL_Window* wnd, SDL_Renderer* renderer)
{
	for (int i = 0, sz = m_points.size() - 1; i < sz; i++)
	{
		const auto& from = m_points[i];
		const auto& to = m_points[i + 1];

		cga::Brezenheim::DrawLine(renderer, from, to, glm::vec3i(255, 255, 255), false);
	}
}
