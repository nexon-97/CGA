#include "Brezenheim.hpp"

#include <SDL.h>
#include <utility>

namespace cga
{

int Brezenheim::s_dashFactor = 0;

void Brezenheim::ResetDashFactor()
{
	s_dashFactor = 0;
}

void Brezenheim::DrawLine(SDL_Renderer* renderer, glm::vec2i from, glm::vec2i to, const glm::vec3i& color, bool dashed)
{
	auto dashedDraw = [renderer, dashed](const glm::vec2i& point, int& dashFactor)
	{
		if (!dashed || (dashFactor / 10) % 2 == 1)
		{
			SDL_RenderDrawPoint(renderer, point.x, point.y);
		}

		dashFactor++;
	};

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
	dashedDraw(from, s_dashFactor);

	glm::vec2i delta(from.x - to.x, to.y - from.y);

	if (delta.y == 0)
	{
		int sign = delta.x < 0 ? -1 : 1;
		for (int i = from.x; i != to.x; i -= sign)
		{
			dashedDraw(glm::vec2i(i, from.y), s_dashFactor);
		}
	}
	else if (delta.x == 0)
	{
		int sign = delta.y < 0 ? -1 : 1;
		for (int i = from.y; i != to.y; i += sign)
		{
			dashedDraw(glm::vec2i(from.x, i), s_dashFactor);
		}
	}
	else
	{
		int f = 0;
		glm::vec2i sign(delta.x < 0 ? -1 : 1, delta.y < 0 ? -1 : 1);
		glm::vec2i signedDelta = delta * sign;
		glm::vec2i current = from;

		if (abs(delta.y) <= abs(delta.x))
		{
			do {
				f += signedDelta.y;
				if (f > 0)
				{
					f -= signedDelta.x;
					current.y += sign.y;
				}

				current.x -= sign.x;

				dashedDraw(current, s_dashFactor);
			} while (current.x != to.x || current.y != to.y);
		}
		else
		{
			do {
				f += signedDelta.x;
				if (f > 0) {
					f -= signedDelta.y;
					current.x -= sign.x;
				}

				current.y += sign.y;

				dashedDraw(current, s_dashFactor);
			} while (current.x != to.x || current.y != to.y);
		}
	}
}

}
