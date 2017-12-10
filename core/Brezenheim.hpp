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
	static void DrawLine(SDL_Renderer* renderer, glm::vec2i from, glm::vec2i to, const glm::vec3i& color, bool dashed, bool useZbuffer, float* depths);
	static std::vector<glm::vec2i> Brezenheim::ConstructLine(glm::vec2i from, glm::vec2i to);
	static void ResetDashFactor();

	static void DrawPolygon(SDL_Renderer* renderer, glm::vec2i* start, int count, const glm::vec3i& color, bool dashed);
	static void DrawLines(SDL_Renderer* renderer, glm::linei* start, int count, const glm::vec3i& color, bool dashed);
	static void DrawFilledQuad(SDL_Renderer* renderer, const glm::vec2i& pos, int size, const glm::vec3i& color);

	static void DrawVertexBuffer(SDL_Renderer* renderer, VertexBuffer* buffer, math::Matrix& transform, bool solid = false);

	static void DrawSolidFace(SDL_Renderer* renderer, const glm::vec2i& p1, const glm::vec2i& p2, const glm::vec2i& p3, float* depths);

	static bool PerformFrustumCullingTest(const Face& face);
	static bool PerformDepthTest(const glm::vec3f& point);

private:
	static int s_dashFactor;
};

}
