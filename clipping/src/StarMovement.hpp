#include "core/RenderJob.hpp"
#include <vector>
#include <core/Defines.hpp>

class StarMovement
{
public:
	StarMovement(const glm::recti& boundingBox);

	void Update();
	const glm::mat3f& GetTransform() const;
	void SetPosition(const glm::vec2i& pos);

	void Pause();
	void Resume();

private:
	glm::mat3f m_starTransform;
	glm::recti m_boundingBox;
	glm::vec2i m_position;
	float m_rotation;
	bool m_movementDirection[2];
	bool m_isPaused = false;
};
