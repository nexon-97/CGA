#include <iostream>

#include "ArchimedeanSpiralRenderJob.hpp"

ArchimedeanSpiralRenderJob::ArchimedeanSpiralRenderJob(
	sf::Vector2f center, float linearSpeed, float radialSpeed, float accuracy)
{
	int maxLength = 300;

	static const float PI = 3.141592f;
	float fMaxLength = float(maxLength);
	float tMax = fMaxLength / linearSpeed;
	float phiMax = radialSpeed * tMax;
	float phiStep = phiMax / fMaxLength;
	int maxX = int(maxLength * accuracy);

	// Solid line
	m_lines.push_back(sf::Vertex(center, sf::Color::Black));
	for (int x = 1; x < maxX; x++)
	{
		float radius = float(x) / accuracy;
		float phi = x * phiStep / accuracy;

		sf::Vector2f offset(cos(phi) * radius, sin(phi) * radius);
		sf::Vertex nextVertex(center + offset, sf::Color::Black);
		m_lines.push_back(nextVertex);
	}

	// Dashed line
	sf::Vertex lastVertex = sf::Vertex(center, sf::Color::Black);
	for (int x = -1; x > -maxX; x--)
	{
		float radius = float(x) / accuracy;
		float phi = x * phiStep / accuracy;

		sf::Vector2f offset(cos(phi) * radius, sin(phi) * radius);
		sf::Vertex nextVertex(center + offset);

		if (x % 2)
		{
			nextVertex.color = sf::Color::Transparent;
		}
		else
		{
			nextVertex.color = sf::Color::Black;
		}

		m_linesReverse.push_back(lastVertex);
		m_linesReverse.push_back(nextVertex);

		lastVertex = nextVertex;
	}
}

void ArchimedeanSpiralRenderJob::Render(sf::RenderWindow* wnd)
{
	wnd->draw(m_lines.data(), m_lines.size(), sf::LineStrip);
	wnd->draw(m_linesReverse.data(), m_linesReverse.size(), sf::Lines);
}
