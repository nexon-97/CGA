#include "RecursionRenderJob.hpp"

RecursionRenderJob::RecursionRenderJob(sf::Vector2f center, int recursionDepth, float rotationStep)
	: m_recursionDepth(recursionDepth)
{
	static const float PI = 3.1415f;
	static const float PI_DOUBLE = 2.f * PI;

	// Add initial equal-sized triangle
	sf::Vector2f currentTriangle[3] {
		center + sf::Vector2f(0.f, -280.f),
		center + sf::Vector2f(-242.5f, 140.f),
		center + sf::Vector2f(242.5f, 140.f),
	};
	sf::Vector2f nextTriangle[3];

	AddTriangle(currentTriangle);

	float phi = rotationStep;
	for (int i = 0; i < recursionDepth; i++)
	{
		phi += rotationStep;

		float fProjectionSide = (fmodf(phi, PI_DOUBLE) / PI_DOUBLE) * 3.f;
		int projectionSide = int(fProjectionSide);
		float percentage = fmodf(fProjectionSide, 1.f);

		nextTriangle[0] = Lerp(currentTriangle[0], currentTriangle[1], percentage);
		nextTriangle[1] = Lerp(currentTriangle[1], currentTriangle[2], percentage);
		nextTriangle[2] = Lerp(currentTriangle[2], currentTriangle[0], percentage);
		memcpy_s(currentTriangle, sizeof(currentTriangle), nextTriangle, sizeof(nextTriangle));

		AddTriangle(currentTriangle);
	}
}

void RecursionRenderJob::AddTriangle(sf::Vector2f triangle[3])
{
	m_lines.emplace_back(triangle[0], sf::Color::Black);
	m_lines.emplace_back(triangle[1], sf::Color::Black);
	m_lines.emplace_back(triangle[2], sf::Color::Black);
	m_lines.emplace_back(triangle[0], sf::Color::Black);
}

sf::Vector2f RecursionRenderJob::Lerp(const sf::Vector2f& a, const sf::Vector2f& b, float x)
{
	return sf::Vector2f(a.x + (b.x - a.x) * x, a.y + (b.y - a.y) * x);
}

void RecursionRenderJob::Render(sf::RenderWindow* wnd)
{
	wnd->draw(m_lines.data(), m_lines.size(), sf::LineStrip);
}
