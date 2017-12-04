#include "ClippingRenderJob.hpp"
#include "core/InputManager.hpp"

/*ClippingRenderJob::ClippingRenderJob(sf::Vector2f size)
	: m_starMovement(sf::FloatRect(150.f, 150.f, size.x - 300.f, size.y - 300.f))
{
	m_backShape.append(sf::Vertex(sf::Vector2f(50.f, 50.f), sf::Color::Black));
	m_backShape.append(sf::Vertex(sf::Vector2f(size.x - 50.f, 50.f), sf::Color::Black));
	m_backShape.append(sf::Vertex(sf::Vector2f(size.x - 50.f, size.y - 50.f), sf::Color::Black));
	m_backShape.append(sf::Vertex(sf::Vector2f(50.f, size.y - 50.f), sf::Color::Black));
	m_backShape.append(sf::Vertex(sf::Vector2f(50.f, 50.f), sf::Color::Black));
	m_backShape.setPrimitiveType(sf::LineStrip);

	m_boxShape.append(sf::Vertex(sf::Vector2f(100.f, 100.f), sf::Color::Black));
	m_boxShape.append(sf::Vertex(sf::Vector2f(250.f, 100.f), sf::Color::Black));
	m_boxShape.append(sf::Vertex(sf::Vector2f(250.f, 250.f), sf::Color::Black));
	m_boxShape.append(sf::Vertex(sf::Vector2f(100.f, 250.f), sf::Color::Black));
	m_boxShape.append(sf::Vertex(sf::Vector2f(100.f, 100.f), sf::Color::Black));
	m_boxShape.setPrimitiveType(sf::LineStrip);

	sf::Vector2f center(300.f, 300.f);
	GenerateStar(center, 150.f);
}

sf::Vector2f ClippingRenderJob::Lerp(const sf::Vector2f& a, const sf::Vector2f& b, float x)
{
	return sf::Vector2f(a.x + (b.x - a.x) * x, a.y + (b.y - a.y) * x);
}

void ClippingRenderJob::Render(sf::RenderWindow* wnd)
{
	auto inputManager = cga::InputManager::GetInstance();

	if (inputManager.IsMouseDown(sf::Mouse::Button::Left))
	{
		m_starMovement.Pause();
		m_starMovement.SetPosition(inputManager.GetMousePosition());
	}
	else
	{
		m_starMovement.Resume();
	}

	m_starMovement.Update();

	sf::VertexArray starCopy = m_starShape;
	sf::Transform starTransform = m_starMovement.GetTransform().getTransform();
	for (int i = 0, sz = starCopy.getVertexCount(); i < sz; i++)
	{
		starCopy[i].position = starTransform.transformPoint(starCopy[i].position);
	}

	auto boxClipResult = m_clipper.Clip(m_boxShape, starCopy, false);
	//auto wndClipResult = m_clipper.Clip(m_backShape, boxClipResult.remain, true);
	//auto wndClipResult = m_clipper.Clip(m_backShape, boxClipResult.remain, true);

	wnd->draw(m_backShape);
	wnd->draw(m_boxShape);

	//wnd->draw(wndClipResult.clipped);
	//wnd->draw(wndClipResult.remain);
	wnd->draw(boxClipResult.remain);
	wnd->draw(boxClipResult.clipped);
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
*/