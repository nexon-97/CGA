#include "StarMovement.hpp"
#include <vector>
#include <core/Utils.hpp>

StarMovement::StarMovement(const glm::recti& boundingBox)
	: m_boundingBox(boundingBox)
{
	std::fill(std::begin(m_movementDirection), std::end(m_movementDirection), true);

	m_position = glm::vec2i(250, 250);
	m_rotation = 0.f;
}

void StarMovement::Update()
{
	if (m_isPaused) return;

	glm::vec2f delta(5.f, 8.f);
	if (!m_movementDirection[0]) delta.x *= -1.f;
	if (!m_movementDirection[1]) delta.y *= -1.f;

	float rotDelta = 0.05f;
	m_position += delta;
	m_rotation += rotDelta;

	if (m_position.x > m_boundingBox.bottomRight.x && m_movementDirection[0])
	{
		m_position.x = m_boundingBox.bottomRight.x;
		m_movementDirection[0] = false;
	}
	if (m_position.x < m_boundingBox.topLeft.x && !m_movementDirection[0])
	{
		m_position.x = m_boundingBox.topLeft.x;
		m_movementDirection[0] = true;
	}
	if (m_position.y > m_boundingBox.bottomRight.y && m_movementDirection[1])
	{
		m_position.y = m_boundingBox.bottomRight.y;
		m_movementDirection[1] = false;
	}
	if (m_position.y < m_boundingBox.topLeft.y && !m_movementDirection[1])
	{
		m_position.y = m_boundingBox.topLeft.y;
		m_movementDirection[1] = true;
	}

	m_starTransform = cga::Utils::ConstructTransform(m_position, m_rotation);
}

const glm::mat3f& StarMovement::GetTransform() const
{
	return m_starTransform;
}

void StarMovement::SetPosition(const glm::vec2i& pos)
{
	m_position = pos;
	m_starTransform = cga::Utils::ConstructTransform(m_position, m_rotation);
}

void StarMovement::Pause()
{
	m_isPaused = true;
}

void StarMovement::Resume()
{
	m_isPaused = false;
}
