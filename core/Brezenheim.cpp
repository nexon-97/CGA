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

void Brezenheim::DrawLine(SDL_Renderer* renderer, glm::vec2i from, glm::vec2i to, const glm::vec3i& color, bool dashed, bool useZbuffer, float* depths)
{
	auto zbuffer = App::GetInstance()->GetZBufferContent();
	auto wnd = App::GetInstance()->GetWindow();
	glm::vec2i wndSize;
	SDL_GetWindowSize(wnd, &wndSize.x, &wndSize.y);

	auto dashedDraw = [renderer, dashed, useZbuffer, zbuffer, wndSize](const glm::vec2i& point, int& dashFactor, float depth)
	{
		if (point.x < 0 || point.y < 0 || point.x >= wndSize.x || point.y >= wndSize.y) return;

		if (!dashed || (dashFactor / 10) % 2 == 1)
		{
			if (!useZbuffer)
			{
				SDL_RenderDrawPoint(renderer, point.x, point.y);
			}
			else 
			{
				int zbufferIdx = wndSize.y * point.y + point.x;
				if (zbuffer[zbufferIdx] > depth)
				{
					zbuffer[zbufferIdx] = depth;
					SDL_RenderDrawPoint(renderer, point.x, point.y);
				}
			}
		}

		dashFactor++;
	};

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
	if (useZbuffer)
	{
		dashedDraw(from, s_dashFactor, depths[0]);
	}
	else
	{
		dashedDraw(from, s_dashFactor, 0.f);
	}

	glm::vec2i delta(from.x - to.x, to.y - from.y);

	if (delta.y == 0)
	{
		int sign = delta.x < 0 ? -1 : 1;
		for (int i = from.x; i != to.x; i -= sign)
		{
			float depth = useZbuffer ? glm::lerp(depths[0], depths[1], float(i - from.x) / float(to.x - from.x)) : 0.f;
			dashedDraw(glm::vec2i(i, from.y), s_dashFactor, depth);
		}
	}
	else if (delta.x == 0)
	{
		int sign = delta.y < 0 ? -1 : 1;
		for (int i = from.y; i != to.y; i += sign)
		{
			float depth = useZbuffer ? glm::lerp(depths[0], depths[1], float(i - from.y) / float(to.y - from.y)) : 0.f;
			dashedDraw(glm::vec2i(from.x, i), s_dashFactor, depth);
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

				float depth = useZbuffer ? glm::lerp(depths[0], depths[1], float(current.y - from.y) / float(delta.x)) : 0.f;
				dashedDraw(current, s_dashFactor, depth);
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

				float depth = useZbuffer ? glm::lerp(depths[0], depths[1], float(current.y - from.y) / float(delta.y)) : 0.f;
				dashedDraw(current, s_dashFactor, depth);
			} while (current.x != to.x || current.y != to.y);
		}
	}
}

std::vector<glm::vec2i> Brezenheim::ConstructLine(glm::vec2i from, glm::vec2i to)
{
	int lastY = from.y;
	std::vector<glm::vec2i> result = { from };

	glm::vec2i delta(from.x - to.x, to.y - from.y);
	if (delta.y == 0)
	{
		int sign = delta.x < 0 ? -1 : 1;
		for (int i = from.x; i != to.x; i -= sign)
		{
			result.push_back(glm::vec2i(i, from.y));
		}
	}
	else if (delta.x == 0)
	{
		int sign = delta.y < 0 ? -1 : 1;
		for (int i = from.y; i != to.y; i += sign)
		{
			result.push_back(glm::vec2i(from.x, i));
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

				if (lastY != current.y)
				{
					result.push_back(current);
					lastY = current.y;
				}
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

				if (lastY != current.y)
				{
					result.push_back(current);
					lastY = current.y;
				}
			} while (current.x != to.x || current.y != to.y);
		}
	}

	return result;
}

void Brezenheim::DrawFilledQuad(SDL_Renderer* renderer, const glm::vec2i& pos, int size, const glm::vec3i& color)
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
		DrawLine(renderer, start[i], start[i + 1], color, dashed, false, nullptr);
	}
}

void Brezenheim::DrawLines(SDL_Renderer* renderer, glm::linei* lines, int count, const glm::vec3i& color, bool dashed)
{
	for (int i = 0; i < count; i++)
	{
		DrawLine(renderer, lines[i].topLeft, lines[i].bottomRight, color, dashed, false, nullptr);
	}
}

void Brezenheim::DrawSolidFace(SDL_Renderer* renderer, const glm::vec2i& p1, const glm::vec2i& p2, const glm::vec2i& p3, float* depths)
{
	// Sort vertices by y
	glm::vec2i const* vertexes[3] = { &p1, &p2, &p3 };

	if (vertexes[0]->y > vertexes[1]->y) { std::swap(vertexes[0], vertexes[1]); std::swap(depths[0], depths[1]); }
	if (vertexes[1]->y > vertexes[2]->y) { std::swap(vertexes[1], vertexes[2]); std::swap(depths[1], depths[2]); }
	if (vertexes[0]->y > vertexes[1]->y) { std::swap(vertexes[0], vertexes[1]); std::swap(depths[0], depths[1]); }

	float mu = float(vertexes[1]->y - vertexes[0]->y) / float(vertexes[2]->y - vertexes[0]->y);
	glm::vec2i breakpoint = glm::vec2i(int(vertexes[0]->x + float(vertexes[2]->x - vertexes[0]->x) * mu), vertexes[1]->y);
	auto l1 = ConstructLine(*vertexes[0], *vertexes[1]);
	auto l2 = ConstructLine(*vertexes[0], breakpoint);
	auto l3 = ConstructLine(*vertexes[1], *vertexes[2]);
	auto l4 = ConstructLine(breakpoint, *vertexes[2]);

	if (l1.size() == l2.size())
	{
		for (int i = 0, sz = l1.size(); i < sz; i++)
		{
			DrawLine(renderer, l1[i], l2[i], glm::vec3i(0, 0, 255), false, true, depths);
		}
	}

	if (l3.size() == l4.size())
	{
		for (int i = 0, sz = l3.size(); i < sz; i++)
		{
			DrawLine(renderer, l3[i], l4[i], glm::vec3i(0, 0, 255), false, true, depths);
		}
	}
}

void Brezenheim::DrawVertexBuffer(SDL_Renderer* renderer, VertexBuffer* buffer, math::Matrix& transform, bool solid)
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

		if (winding && solid)
		{
			float depths[] = { 0.f, 0.f, 0.f };
			DrawSolidFace(renderer, screenPoints[0], screenPoints[1], screenPoints[2], depths);
		}
	}

	// Draw rasterized lines
	for (const auto& line : rasterizedLines)
	{
		bool dashed = !line.visible;
		glm::vec3i color = line.visible ? glm::vec3i(255, 255, 255) : glm::vec3i(255, 0, 0);
		DrawLine(renderer, line.src, line.dest, color, dashed, false, nullptr);
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
