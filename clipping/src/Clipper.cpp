#include "Clipper.hpp"

float Clipper::Scalar(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return a.x * b.x + a.y * b.y;
}

sf::Vector2f Clipper::FindNormal(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& z)
{
	sf::Vector2f d = b - a;
	sf::Vector2f n(-d.y, d.x);
	sf::Vector2f v(z.x - a.x, z.y - a.y);

	float dot = Scalar(v, n);
	if (!dot)
	{
		return sf::Vector2f();
	}
	else if (dot < 0) 
	{
		n *= -1.f;
	}

	return n;
}

Clipper::ClippingResult Clipper::Clip(const sf::VertexArray& clipper, const sf::VertexArray& clippee)
{
	Clipper::ClippingResult result;
	result.remain.setPrimitiveType(sf::Lines);
	result.clipped.setPrimitiveType(sf::Lines);

	for (int ptIdx = 0, sz = clippee.getVertexCount() - 1; ptIdx < sz; ptIdx++)
	{
		sf::Vector2f b = clippee[ptIdx + 1].position;
		sf::Vector2f a = clippee[ptIdx].position;

		sf::Vector2f d = b - a;
		sf::Vector2f dir(d.x, d.y);

		float tEnter = 0.0f;
		float tLeave = 1.0f;

		sf::Vector2f boundary = clipper[clipper.getVertexCount() - 2].position;
		for (int i = 0, count = clipper.getVertexCount() - 1; i < count; i++) 
		{
			sf::Vector2f edge_p0(clipper[i + 0].position);
			sf::Vector2f edge_p1(clipper[i + 1].position);

			sf::Vector2f norm(FindNormal(edge_p0, edge_p1, boundary));
			sf::Vector2f w(a.x - edge_p0.x, a.y - edge_p0.y);

			float num = Scalar(w, norm);
			float den = Scalar(dir, norm);

			if (!den) 
			{
				if (num < 0) 
				{
					break;
				} else {
					continue;
				}
			}

			float t = -num / den;
			if (den > 0)
			{
				tEnter = std::max(tEnter, t);
			} 
			else if (den < 0) 
			{
				tLeave = std::min(tLeave, t);
			}

			boundary = edge_p0;
		}

		if (tEnter > tLeave) 
		{
			continue;
		}

		sf::Vector2f clipped_p0;
		clipped_p0.x = a.x + (d.x * tEnter);
		clipped_p0.y = a.y + (d.y * tEnter);

		sf::Vector2f clipped_p1;
		clipped_p1.x = a.x + (d.x * tLeave);
		clipped_p1.y = a.y + (d.y * tLeave);

		result.remain.append(sf::Vertex(a, sf::Color::Red));
		result.remain.append(sf::Vertex(clipped_p1, sf::Color::Red));
		result.clipped.append(sf::Vertex(clipped_p0, sf::Color::Black));
		result.clipped.append(sf::Vertex(clipped_p1, sf::Color::Black));
		result.remain.append(sf::Vertex(clipped_p1, sf::Color::Red));
		result.remain.append(sf::Vertex(b, sf::Color::Red));
	}

	return result;
}
