#include "ClippingRenderJob.hpp"

ClippingRenderJob::ClippingRenderJob(sf::Vector2f size)
	: m_starMovement(sf::FloatRect(150.f, 150.f, size.x - 300.f, size.y - 300.f))
{
	m_backShape.append(sf::Vertex(sf::Vector2f(50.f, 50.f), sf::Color::Black));
	m_backShape.append(sf::Vertex(sf::Vector2f(size.x - 50.f, 50.f), sf::Color::Black));
	m_backShape.append(sf::Vertex(sf::Vector2f(size.x - 50.f, size.y - 50.f), sf::Color::Black));
	m_backShape.append(sf::Vertex(sf::Vector2f(50.f, size.y - 50.f), sf::Color::Black));
	m_backShape.append(sf::Vertex(sf::Vector2f(50.f, 50.f), sf::Color::Black));
	m_backShape.setPrimitiveType(sf::LineStrip);

	sf::Vector2f center(300.f, 300.f);
	GenerateStar(center, 150.f);
}

sf::Vector2f ClippingRenderJob::Lerp(const sf::Vector2f& a, const sf::Vector2f& b, float x)
{
	return sf::Vector2f(a.x + (b.x - a.x) * x, a.y + (b.y - a.y) * x);
}

void ClippingRenderJob::Render(sf::RenderWindow* wnd)
{
	m_starMovement.Update();

	sf::VertexArray starCopy = m_starShape;
	sf::Transform starTransform = m_starMovement.GetTransform().getTransform();
	for (int i = 0, sz = starCopy.getVertexCount(); i < sz; i++)
	{
		starCopy[i].position = starTransform.transformPoint(starCopy[i].position);
	}

	auto starClipResult = m_clipper.Clip(m_backShape, starCopy);

	wnd->draw(m_backShape);
	wnd->draw(starClipResult.remain);
	wnd->draw(starClipResult.clipped);
}

void ClippingRenderJob::GenerateStar(const sf::Vector2f& center, float radius)
{
	static const float PI = 3.1415f;
	static const float PI_DOUBLE = 2.f * PI;

	float sector = PI_DOUBLE / 5.f;
	sf::Vector2f starPoints[5];
	for (int i = 0; i < 5; i++)
	{
		float phi = sector * float(i);
		starPoints[i] = sf::Vector2f(sin(phi), -cos(phi)) * radius;
	}

	m_starShape.append(sf::Vertex(starPoints[0], sf::Color::Black));
	m_starShape.append(sf::Vertex(starPoints[2], sf::Color::Black));
	m_starShape.append(sf::Vertex(starPoints[4], sf::Color::Black));
	m_starShape.append(sf::Vertex(starPoints[1], sf::Color::Black));
	m_starShape.append(sf::Vertex(starPoints[3], sf::Color::Black));
	m_starShape.append(sf::Vertex(starPoints[0], sf::Color::Black));
	m_starShape.setPrimitiveType(sf::LineStrip);
}
