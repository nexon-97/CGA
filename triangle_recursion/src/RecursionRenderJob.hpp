#include "core/RenderJob.hpp"
#include "SFML/Graphics.hpp"
#include <vector>

namespace sf
{
class Vertex;
};

class RecursionRenderJob : public cga::RenderJob
{
public:
	RecursionRenderJob(sf::Vector2f center, int recursionDepth, float rotationStep);

	virtual void Render(sf::RenderWindow* wnd) override;

private:
	void AddTriangle(sf::Vector2f triangle[3]);
	sf::Vector2f Lerp(const sf::Vector2f& a, const sf::Vector2f& b, float x);

	std::vector <sf::Vertex> m_lines;
	int m_recursionDepth;
};
