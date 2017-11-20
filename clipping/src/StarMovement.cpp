#include "StarMovement.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

StarMovement::StarMovement(sf::FloatRect boundingBox)
	: m_boundingBox(boundingBox)
{
	std::fill(std::begin(m_movementDirection), std::end(m_movementDirection), true);
	m_starTransform.setPosition(250.f, 250.f);
}

void StarMovement::Update()
{
	if (m_isPaused) return;

	auto position = m_starTransform.getPosition();
	auto rotation = m_starTransform.getRotation();

	sf::Vector2f delta(2.f, 2.f);
	if (!m_movementDirection[0]) delta.x *= -1.f;
	if (!m_movementDirection[1]) delta.y *= -1.f;

	float rotDelta = 0.25f;
	position += delta;
	rotation += rotDelta;

	if (position.x > m_boundingBox.left + m_boundingBox.width && m_movementDirection[0])
	{
		position.x = m_boundingBox.left + m_boundingBox.width;
		m_movementDirection[0] = false;
	}
	if (position.x < m_boundingBox.left && !m_movementDirection[0])
	{
		position.x = m_boundingBox.left;
		m_movementDirection[0] = true;
	}
	if (position.y > m_boundingBox.top + m_boundingBox.height && m_movementDirection[1])
	{
		position.y = m_boundingBox.top + m_boundingBox.height;
		m_movementDirection[1] = false;
	}
	if (position.y < m_boundingBox.top && !m_movementDirection[1])
	{
		position.y = m_boundingBox.top;
		m_movementDirection[1] = true;
	}

	m_starTransform.setPosition(position);
	m_starTransform.setRotation(rotation);
}

const sf::Transformable& StarMovement::GetTransform() const
{
	return m_starTransform;
}

void StarMovement::SetPosition(const sf::Vector2f& pos)
{
	m_starTransform.setPosition(pos);
}

void StarMovement::Pause()
{
	m_isPaused = true;
}

void StarMovement::Resume()
{
	m_isPaused = false;
}
