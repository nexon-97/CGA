#include "core/RenderJob.hpp"
#include <vector>

namespace sf
{
class Vertex;
};

class RecursionRenderJob : public cga::RenderJob
{
public:
	RecursionRenderJob(glm::vec2i center, int recursionDepth, float rotationStep);

	virtual void Render(sf::RenderWindow* wnd) override;

private:
	void AddTriangle(glm::vec2i triangle[3]);
	glm::vec2i Lerp(const glm::vec2i& a, const glm::vec2i& b, float x);

	std::vector <sf::Vertex> m_lines;
	int m_recursionDepth;
};
