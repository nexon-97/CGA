#include <iostream>
#include "ArchimedeanSpiralRenderJob.hpp"

#include <core/Brezenheim.hpp>

ArchimedeanSpiralRenderJob::ArchimedeanSpiralRenderJob(const glm::vec2i& center, float linearSpeed, float radialSpeed, float accuracy)
{
	int maxLength = 300;

	static const float PI = 3.141592f;
	float fMaxLength = float(maxLength);
	float tMax = fMaxLength / linearSpeed;
	float phiMax = radialSpeed * tMax;
	float phiStep = phiMax / fMaxLength;
	int maxX = int(maxLength * accuracy);

	// Solid line
	m_points.push_back(center);
	for (int x = 1; x < maxX; x++)
	{
		float radius = float(x) / accuracy;
		float phi = x * phiStep / accuracy;

		glm::vec2i offset(cos(phi) * radius, sin(phi) * radius);
		m_points.push_back(center + offset);
	}

	// Dashed line
	m_pointsReverse.push_back(center);
	for (int x = -1; x > -maxX; x--)
	{
		float radius = float(x) / accuracy;
		float phi = x * phiStep / accuracy;

		glm::vec2i offset(cos(phi) * radius, sin(phi) * radius);
		m_pointsReverse.push_back(center + offset);
	}
}

void ArchimedeanSpiralRenderJob::Render(SDL_Window* wnd, SDL_Renderer* renderer)
{
	for (int i = 0, sz = m_points.size() - 1; i < sz; i++)
	{
		const auto& from = m_points[i];
		const auto& to = m_points[i + 1];

		cga::Brezenheim::DrawLine(renderer, from, to, glm::vec3i(255, 255, 255), false);
	}

	for (int i = 0, sz = m_pointsReverse.size() - 1; i < sz; i++)
	{
		const auto& from = m_pointsReverse[i];
		const auto& to = m_pointsReverse[i + 1];

		cga::Brezenheim::DrawLine(renderer, from, to, glm::vec3i(255, 255, 255), true);
	}
}
