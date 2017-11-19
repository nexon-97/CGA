#include "core/RenderJob.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

namespace sf
{
class Vertex;
};

class Clipper
{
public:
	Clipper() = default;

	struct ClippingResult
	{
		sf::VertexArray remain;
		sf::VertexArray clipped;
	};

	ClippingResult Clip(const sf::VertexArray& clipper, const sf::VertexArray& clippee);

private:
	float Scalar(const sf::Vector2f& a, const sf::Vector2f& b);
	sf::Vector2f FindNormal(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c);
};
