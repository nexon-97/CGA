#include "Brezenheim.hpp"
#include "Utils.hpp"
#include "App.hpp"

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

void Brezenheim::DrawPolygon(SDL_Renderer* renderer, glm::vec2i* start, int count, const glm::vec3i& color, bool dashed)
{
	for (int i = 0, sz = count - 1; i < sz; i++)
	{
		DrawLine(renderer, start[i], start[i + 1], color, dashed);
	}
}

void Brezenheim::DrawLines(SDL_Renderer* renderer, glm::linei* lines, int count, const glm::vec3i& color, bool dashed)
{
	for (int i = 0; i < count; i++)
	{
		DrawLine(renderer, lines[i].topLeft, lines[i].bottomRight, color, dashed);
	}
}

void Brezenheim::DrawVertexBuffer(SDL_Renderer* renderer, VertexBuffer* buffer, const glm::mat4f& transform)
{
	const auto& points = buffer->GetPoints();
	const auto& indices = buffer->GetIndices();

	glm::vec2i wndSize;
	SDL_GetWindowSize(App::GetInstance()->GetWindow(), &wndSize.x, &wndSize.y);

	for (int i = 0; i < indices.size(); i += 3)
	{
		auto p1 = points[indices[i]];
		auto p2 = points[indices[i + 1]];
		auto p3 = points[indices[i + 2]];

		p1 = Utils::TransformPoint(p1, transform);
		p2 = Utils::TransformPoint(p2, transform);
		p3 = Utils::TransformPoint(p3, transform);

		glm::vec2i screenP1 = glm::vec2i(wndSize.x * (p1.x + 1.f) / 2.f, wndSize.y * (p1.y + 1.f) / 2.f);
		glm::vec2i screenP2 = glm::vec2i(wndSize.x * (p2.x + 1.f) / 2.f, wndSize.y * (p2.y + 1.f) / 2.f);
		glm::vec2i screenP3 = glm::vec2i(wndSize.x * (p3.x + 1.f) / 2.f, wndSize.y * (p3.y + 1.f) / 2.f);

		DrawLine(renderer, screenP1, screenP2, glm::vec3i(255, 255, 255), false);
		DrawLine(renderer, screenP2, screenP3, glm::vec3i(255, 255, 255), false);
		DrawLine(renderer, screenP3, screenP1, glm::vec3i(255, 255, 255), false);
	}
}

}
