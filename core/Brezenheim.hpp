#pragma once
#include "Defines.hpp"

struct SDL_Renderer;

namespace cga
{

class Brezenheim
{
public:
	static void DrawLine(SDL_Renderer* renderer, glm::vec2i from, glm::vec2i to, const glm::vec3i& color, bool dashed);
	static void ResetDashFactor();

	static void DrawPolygon(SDL_Renderer* renderer, glm::vec2i* start, int count, const glm::vec3i& color, bool dashed);
	static void DrawLines(SDL_Renderer* renderer, glm::linei* start, int count, const glm::vec3i& color, bool dashed);

private:
	static int s_dashFactor;
};

}
