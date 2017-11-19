#include "core/RenderJob.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

namespace sf
{
class Vertex;
};

class StarMovement
{
public:
	StarMovement(sf::FloatRect boundingBox);

	void Update();
	const sf::Transformable& GetTransform() const;

private:
	sf::Transformable m_starTransform;
	sf::FloatRect m_boundingBox;
	bool m_movementDirection[2];
};
