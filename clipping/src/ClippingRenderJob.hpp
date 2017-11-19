#include "core/RenderJob.hpp"
#include "StarMovement.hpp"
#include "Clipper.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

namespace sf
{
class Vertex;
};

class ClippingRenderJob : public cga::RenderJob
{
public:
	ClippingRenderJob(sf::Vector2f size);

	virtual void Render(sf::RenderWindow* wnd) override;

private:
	void GenerateStar(const sf::Vector2f& center, float radius);
	sf::Vector2f Lerp(const sf::Vector2f& a, const sf::Vector2f& b, float x);
	
	sf::Transformable m_starTransform;
	sf::VertexArray m_backShape;
	sf::VertexArray m_starShape;
	StarMovement m_starMovement;
	Clipper m_clipper;
};
