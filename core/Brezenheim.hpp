#pragma once
#include "Defines.hpp"
#include "VertexBuffer.hpp"
#include "core/Math.h"

struct SDL_Renderer;

namespace cga
{

struct Face
{
	glm::vec3f points[3];
};

class Brezenheim
{
public:
	static void DrawLine(SDL_Renderer* renderer, glm::vec2i from, glm::vec2i to, const glm::vec3i& color, bool dashed);
	static void ResetDashFactor();

	static void DrawPolygon(SDL_Renderer* renderer, glm::vec2i* start, int count, const glm::vec3i& color, bool dashed);
	static void DrawLines(SDL_Renderer* renderer, glm::linei* start, int count, const glm::vec3i& color, bool dashed);
	static void DrawQuad(SDL_Renderer* renderer, const glm::vec2i& pos, int size, const glm::vec3i& color);

	static void DrawVertexBuffer(SDL_Renderer* renderer, VertexBuffer* buffer, math::Matrix& transform);

	static bool PerformFrustumCullingTest(const Face& face);
	static bool PerformDepthTest(const glm::vec3f& point);

private:
	static int s_dashFactor;
};

}
