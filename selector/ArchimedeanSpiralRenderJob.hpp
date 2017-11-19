#include "core/RenderJob.hpp"
#include "SFML/Graphics.hpp"
#include <vector>

namespace sf
{
class Vertex;
};

class ArchimedeanSpiralRenderJob : public cga::RenderJob
{
public:
	ArchimedeanSpiralRenderJob(sf::Vector2f center, float linearSpeed, float radialSpeed, float accuracy);

	virtual void Render(sf::RenderWindow* wnd) override;

private:
	std::vector <sf::Vertex> m_lines;
	std::vector <sf::Vertex> m_linesReverse;
};
