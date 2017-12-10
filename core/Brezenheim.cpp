#include "Brezenheim.hpp"
#include "Utils.hpp"
#include "App.hpp"

#include <SDL.h>
#include <utility>
#include <algorithm>

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

void Brezenheim::DrawQuad(SDL_Renderer* renderer, const glm::vec2i& pos, int size, const glm::vec3i& color)
{
	for (int i = - size / 2; i <= size / 2; i++)
	{
		for (int j = -size / 2; j <= size / 2; j++)
		{
			SDL_RenderDrawPoint(renderer, pos.x + i, pos.y + j);
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

void Brezenheim::DrawVertexBuffer(SDL_Renderer* renderer, VertexBuffer* buffer, math::Matrix& transform)
{
	const auto& points = buffer->GetPoints();
	const auto& indices = buffer->GetIndices();

	glm::vec2i wndSize;
	SDL_GetWindowSize(App::GetInstance()->GetWindow(), &wndSize.x, &wndSize.y);

	auto transposed = transform.Transpose();

	struct LineInfo
	{
		glm::vec2i src;
		glm::vec2i dest;
		bool visible;
	};

	std::vector<LineInfo> rasterizedLines;
	auto addRasterizedLine = [&rasterizedLines](const LineInfo& newLine)
	{
		auto it = std::find_if(rasterizedLines.begin(), rasterizedLines.end(),
			[newLine](const LineInfo& info) { 
				return (info.src == newLine.src && info.dest == newLine.dest) || (info.src == newLine.dest && info.dest == newLine.src); 
			});

		if (it == rasterizedLines.end())
		{
			rasterizedLines.push_back(newLine);
		}
		else
		{
			// Leave stored line visible, if there is a visibility collision
			if (it->visible != newLine.visible)
			{
				it->visible = true;
			}
		}
	};

	for (std::size_t i = 0; i < indices.size(); i += 3)
	{
		Face face;
		for (int j = 0; j < 3; j++)
		{
			auto p = points[indices[i + j]];
			face.points[j] = Utils::TransformPoint(p, transposed);
		}

		bool isInFrustrum = PerformFrustumCullingTest(face);
		if (!isInFrustrum) continue;

		auto edge1 = face.points[1] - face.points[0];
		auto edge2 = face.points[2] - face.points[1];
		auto crossing = glm::cross(glm::vec3f(edge1.x, edge1.y, 0.f), glm::vec3f(edge2.x, edge2.y, 0.f));
		bool winding = crossing.z > 0.f;

		// Convert to screen coordinates
		glm::vec2i screenPoints[3];
		for (int j = 0; j < 3; j++)
		{
			screenPoints[j] = Utils::ProjectPoint(face.points[j], wndSize);
		}
			
		addRasterizedLine({screenPoints[0], screenPoints[1], winding});
		addRasterizedLine({screenPoints[1], screenPoints[2], winding});
		addRasterizedLine({screenPoints[2], screenPoints[0], winding});
	}

	// Draw rasterized lines
	for (const auto& line : rasterizedLines)
	{
		bool dashed = !line.visible;
		glm::vec3i color = line.visible ? glm::vec3i(255, 255, 255) : glm::vec3i(255, 0, 0);
		DrawLine(renderer, line.src, line.dest, color, dashed);
	}
}

bool Brezenheim::PerformFrustumCullingTest(const Face& face)
{
	for (int j = 0; j < 3; j++)
	{
		auto& p = face.points[j];
		if ((p.x < -1.f || p.x > 1.f) && (p.y < -1.f || p.y > 1.f) && (p.z < -1.f || p.z > 1.f))
			return false;
	}

	return true;
}

bool Brezenheim::PerformDepthTest(const glm::vec3f& point)
{
	return true;
}

}
